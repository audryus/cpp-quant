#pragma once
#include <map>
#include <functional>
#include <cstdint>
#include <stdexcept>

// Order Book com dois lados:
//   Bids: compradoras -- ordenadas do maior para o menor preco
//   Asks: vendedoras  -- ordenadas do menor para o maior preco
//
// O spread e a diferenca entre o melhor ask e o melhor bid.
// Mid price e a media entre os dois.
//
// Formato Binance: ["43521.50", "0.123"]
//   price    = 43521.50
//   quantity = 0.123
//   quantity == 0.0 significa: remover esse nivel

class OrderBook {
public:
    // bids: maior preco primeiro (std::greater)
    using BidMap = std::map<double, double, std::greater<double>>;
    // asks: menor preco primeiro (default)
    using AskMap = std::map<double, double>;

    void update_bid(double price, double quantity) {
        if (quantity == 0.0) {
            bids_.erase(price);
        } else {
            bids_[price] = quantity;
        }
    }

    void update_ask(double price, double quantity) {
        if (quantity == 0.0) {
            asks_.erase(price);
        } else {
            asks_[price] = quantity;
        }
    }

    double best_bid() const {
        if (bids_.empty()) return 0.0;
        return bids_.begin()->first;
    }

    double best_ask() const {
        if (asks_.empty()) return 0.0;
        return asks_.begin()->first;
    }

    double mid_price() const {
        return (best_bid() + best_ask()) / 2.0;
    }

    double spread() const {
        return best_ask() - best_bid();
    }

    // imbalance: pressao compradora vs vendedora nos N melhores niveis
    // > 0.5 = mais pressao compradora
    // < 0.5 = mais pressao vendedora
    double imbalance(int levels = 5) const {
        double bid_qty = 0.0;
        double ask_qty = 0.0;
        int i = 0;
        for (auto& [p, q] : bids_) { if (i++ >= levels) break; bid_qty += q; }
        i = 0;
        for (auto& [p, q] : asks_) { if (i++ >= levels) break; ask_qty += q; }
        double total = bid_qty + ask_qty;
        return total == 0.0 ? 0.5 : bid_qty / total;
    }

    size_t bid_levels() const { return bids_.size(); }
    size_t ask_levels() const { return asks_.size(); }

    const BidMap& bids() const { return bids_; }
    const AskMap& asks() const { return asks_; }

private:
    BidMap bids_;
    AskMap asks_;
};
