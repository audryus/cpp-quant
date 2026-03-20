// src/main.cpp
#include <iostream>
#include <iomanip>
#include <thread>
#include <atomic>
#include <csignal>
#include "orderbook.hpp"
#include "feed/binance_feed.hpp"
#include "utils/timer.hpp"
#include "utils/spsc_queue.hpp"

std::atomic<bool> g_running{true};
void signal_handler(int) { g_running = false; }

struct MarketEvent {
    int64_t event_ms;
    int64_t recv_ms;
};

void print_book(const OrderBook& book, int64_t count, int64_t avg_latency) {
    std::cout << "\033[2J\033[H" << std::fixed;
    std::cout << "=== BTCUSDT Order Book (live) ===\n\n";
    int i = 0;
    for (auto& [p, q] : book.asks()) {
        if (i++ >= 5) break;
        std::cout << "  ASK  " << std::setprecision(2) << p
                  << "  |  "   << std::setprecision(4) << q << "\n";
    }
    std::cout << "\n  mid: "      << std::setprecision(2) << book.mid_price()
              << "  spread: "     << std::setprecision(4) << book.spread()
              << "\n  imbalance: " << std::setprecision(3) << book.imbalance()
              << "\n\n";
    i = 0;
    for (auto& [p, q] : book.bids()) {
        if (i++ >= 5) break;
        std::cout << "  BID  " << std::setprecision(2) << p
                  << "  |  "   << std::setprecision(4) << q << "\n";
    }
    std::cout << "\n  updates: "     << count
              << "  latencia media: " << avg_latency << " ms\n";
}

int main() {
    std::signal(SIGINT, signal_handler);

    OrderBook book;
    SPSCQueue<MarketEvent, 4096> queue;

    // --- thread de rede: so faz push na fila ---
    BinanceFeed feed(book);
    feed.set_on_update([&](int64_t event_ms, int64_t recv_ms) {
        queue.push({event_ms, recv_ms});
    });

    std::thread feed_thread([&]() {
        try {
            feed.run("btcusdt", "depth");
        } catch (const std::exception& e) {
            std::cerr << "[erro] " << e.what() << "\n";
            g_running = false;
        }
    });

    // --- thread principal: consome a fila ---
    int64_t update_count   = 0;
    int64_t total_latency  = 0;

    while (g_running) {
        // drena tudo que estiver na fila
        while (auto ev = queue.pop()) {
            ++update_count;
            total_latency += ev->recv_ms - ev->event_ms;

            if (update_count % 10 == 0) {
                print_book(book, update_count, total_latency / update_count);
            }
        }
        // sem mensagens -- yield para nao queimar CPU
        std::this_thread::yield();
    }

    feed.stop();
    feed_thread.join();

    std::cout << "\n[encerrado] total updates: " << update_count << "\n";
    return 0;
}
