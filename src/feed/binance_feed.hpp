// src/feed/binance_feed.hpp
#pragma once
#include <string>
#include <functional>
#include <memory>
#include <atomic>
#include "../orderbook.hpp"

// forward declarations -- evita incluir boost inteiro no header
namespace boost::asio { class io_context; }

class BinanceFeed {
public:
    // callback chamado a cada update do order book
    // args: event_time_ms (da mensagem), receive_time_ms (agora)
    using OnUpdate = std::function<void(int64_t event_ms, int64_t recv_ms)>;

    explicit BinanceFeed(OrderBook& book);
    ~BinanceFeed();

    void set_on_update(OnUpdate cb) { on_update_ = std::move(cb); }

    // conecta e roda -- bloqueante, chame em thread separada
    void run(const std::string& symbol = "btcusdt",
             const std::string& stream = "depth");

    void stop();

private:
    void parse_depth_update(std::string_view json, int64_t recv_ms);

    OrderBook&  book_;
    OnUpdate    on_update_;
    std::atomic<bool> running_{false};
};
