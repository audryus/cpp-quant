// src/feed/binance_feed.cpp
#include "binance_feed.hpp"
#include "../utils/timer.hpp"

#include <boost/beast/core.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/beast/websocket/ssl.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/context.hpp>
#include <nlohmann/json.hpp>

#include <iostream>
#include <string>

namespace beast     = boost::beast;
namespace websocket = beast::websocket;
namespace net       = boost::asio;
namespace ssl       = net::ssl;
using tcp           = net::ip::tcp;
using json          = nlohmann::json;

BinanceFeed::BinanceFeed(OrderBook& book) : book_(book) {}
BinanceFeed::~BinanceFeed() { stop(); }

void BinanceFeed::run(const std::string& symbol, const std::string& stream) {
    running_ = true;

    const std::string host   = "stream.binance.com";
    const std::string port   = "9443";
    const std::string target = "/ws/" + symbol + "@" + stream;

    net::io_context ioc;
    ssl::context    ctx(ssl::context::tlsv12_client);
    ctx.set_default_verify_paths();

    tcp::resolver resolver(ioc);
    websocket::stream<beast::ssl_stream<tcp::socket>> ws(ioc, ctx);

    // resolve DNS
    auto results = resolver.resolve(host, port);

    // conecta TCP
    auto ep = net::connect(beast::get_lowest_layer(ws), results);

    // SNI -- obrigatorio para TLS
    if (!SSL_set_tlsext_host_name(ws.next_layer().native_handle(), host.c_str())) {
        throw beast::system_error(
            beast::error_code(static_cast<int>(::ERR_get_error()),
                              net::error::get_ssl_category()));
    }

    // handshake TLS
    ws.next_layer().handshake(ssl::stream_base::client);

    // handshake WebSocket
    ws.set_option(websocket::stream_base::decorator([](websocket::request_type& req) {
        req.set(beast::http::field::user_agent, "cpp-quant/1.0");
    }));
    ws.handshake(host + ":" + std::to_string(ep.port()), target);

    std::cout << "[feed] conectado: wss://" << host << target << "\n";

    beast::flat_buffer buffer;

    while (running_) {
        buffer.clear();
        beast::error_code ec;
        ws.read(buffer, ec);

        if (ec) {
            if (running_) std::cerr << "[feed] erro: " << ec.message() << "\n";
            break;
        }

        int64_t recv_ms = utils::now_ms();
        std::string_view msg(static_cast<const char*>(buffer.data().data()),
                             buffer.data().size());

        parse_depth_update(msg, recv_ms);
    }

    beast::error_code ec;
    ws.close(websocket::close_code::normal, ec);
}

void BinanceFeed::stop() {
    running_ = false;
}

void BinanceFeed::parse_depth_update(std::string_view msg, int64_t recv_ms) {
    try {
        auto j = json::parse(msg);

        // campo "E" = event time em milliseconds
        int64_t event_ms = j.value("E", (int64_t)0);

        // bids: array de ["price", "quantity"]
        for (auto& level : j["b"]) {
            double price = std::stod(level[0].get<std::string>());
            double qty   = std::stod(level[1].get<std::string>());
            book_.update_bid(price, qty);
        }

        // asks
        for (auto& level : j["a"]) {
            double price = std::stod(level[0].get<std::string>());
            double qty   = std::stod(level[1].get<std::string>());
            book_.update_ask(price, qty);
        }

        if (on_update_) on_update_(event_ms, recv_ms);

    } catch (const std::exception& e) {
        std::cerr << "[feed] parse error: " << e.what() << "\n";
    }
}
