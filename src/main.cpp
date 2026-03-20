#include <iostream>
#include <iomanip>
#include <cassert>
#include <chrono>
#include "orderbook.hpp"

void print_book(const OrderBook& book, int levels = 5) {
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "  ASKS (" << book.ask_levels() << " niveis)\n";
    int i = 0;
    for (auto& [p, q] : book.asks()) {
        if (i++ >= levels) break;
        std::cout << "    " << p << "  |  " << q << "\n";
    }
    std::cout << "  --- spread: " << book.spread()
              << "  mid: " << book.mid_price() << " ---\n";
    i = 0;
    for (auto& [p, q] : book.bids()) {
        if (i++ >= levels) break;
        std::cout << "    " << p << "  |  " << q << "\n";
    }
    std::cout << "  BIDS (" << book.bid_levels() << " niveis)\n";
    std::cout << "  imbalance: " << book.imbalance() << "\n\n";
}

int main() {
    OrderBook book;

    // --- snapshot inicial (formato Binance /api/v3/depth) ---
    std::cout << "=== Snapshot inicial ===\n";
    // asks
    book.update_ask(43522.00, 0.789);
    book.update_ask(43522.50, 0.321);
    book.update_ask(43523.00, 1.100);
    book.update_ask(43523.50, 0.654);
    book.update_ask(43524.00, 2.000);
    // bids
    book.update_bid(43521.50, 0.123);
    book.update_bid(43521.00, 0.456);
    book.update_bid(43520.50, 0.789);
    book.update_bid(43520.00, 1.234);
    book.update_bid(43519.50, 0.567);

    print_book(book);

    assert(book.best_ask() == 43522.00);
    assert(book.best_bid() == 43521.50);
    assert(book.spread()   == 43522.00 - 43521.50);

    // --- update: simula mensagem @depth da Binance ---
    std::cout << "=== Apos update de mercado ===\n";
    book.update_ask(43522.00, 0.0);    // removeu melhor ask
    book.update_ask(43521.80, 1.500);  // novo nivel mais agressivo
    book.update_bid(43521.50, 0.0);    // removeu melhor bid
    book.update_bid(43521.70, 0.900);  // novo bid mais agressivo

    print_book(book);

    // spread fechou -- mercado ficou mais liquido
    assert(book.best_ask() == 43521.80);
    assert(book.best_bid() == 43521.70);
    std::cout << "spread fechou para: " << book.spread() << "\n\n";

    // --- benchmark: 100k updates ---
    std::cout << "=== Benchmark: 100k updates ===\n";
    auto t0 = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < 100'000; ++i) {
        double price = 43500.0 + (i % 100) * 0.1;
        double qty   = (i % 10 == 0) ? 0.0 : 1.0;  // 10% removals
        if (i % 2 == 0) book.update_bid(price, qty);
        else             book.update_ask(price, qty);
    }

    auto t1 = std::chrono::high_resolution_clock::now();
    auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count();
    std::cout << "100k updates: " << ns / 1'000'000 << " ms\n";
    std::cout << "por update:   " << ns / 100'000   << " ns\n";

    return 0;
}
