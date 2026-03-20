#pragma once
#include <cstddef>
#include <functional>
#include <optional>
#include <stdexcept>
#include <vector>

// HashMap com open addressing + linear probing
// K = tipo da chave, V = tipo do valor
template<typename K, typename V>
class HashMap {
public:
    explicit HashMap(size_t initial_capacity = 16)
        : capacity_(initial_capacity)
        , size_(0)
        , buckets_(initial_capacity)
    {}

    // insere ou sobrescreve
    void insert(const K& key, const V& value) {
        if (load_factor() > 0.75f) {
            resize();
        }
        size_t idx = find_slot(key);
        if (!buckets_[idx].occupied) {
            ++size_;
        }
        buckets_[idx] = {key, value, true};
    }

    // retorna nullptr se nao existe
    V* get(const K& key) {
        size_t idx = probe(key);
        if (idx == capacity_) return nullptr;
        return &buckets_[idx].value;
    }

    bool remove(const K& key) {
        // remocao em open addressing e complicada:
        // marcar como "tombstone" para nao quebrar probing
        size_t idx = probe(key);
        if (idx == capacity_) return false;
        buckets_[idx].occupied = false;
        buckets_[idx].tombstone = true;
        --size_;
        return true;
    }

    size_t size()     const { return size_; }
    size_t capacity() const { return capacity_; }
    float  load_factor() const {
        return static_cast<float>(size_) / static_cast<float>(capacity_);
    }

private:
    struct Bucket {
        K    key{};
        V    value{};
        bool occupied  = false;
        bool tombstone = false;
    };

    // encontra slot para insercao (slot vazio ou com a mesma chave)
    size_t find_slot(const K& key) {
        size_t idx = hash(key);
        while (buckets_[idx].occupied && buckets_[idx].key != key) {
            idx = (idx + 1) % capacity_;
        }
        return idx;
    }

    // encontra slot de uma chave existente (retorna capacity_ se nao achar)
    size_t probe(const K& key) {
        size_t idx  = hash(key);
        size_t seen = 0;
        while (seen < capacity_) {
            auto& b = buckets_[idx];
            if (!b.occupied && !b.tombstone) return capacity_;
            if (b.occupied && b.key == key)  return idx;
            idx = (idx + 1) % capacity_;
            ++seen;
        }
        return capacity_;
    }

    void resize() {
        size_t new_cap = capacity_ * 2;
        std::vector<Bucket> new_buckets(new_cap);

        // reinsere todos os elementos no novo array
        //size_t old_cap = capacity_;
        capacity_ = new_cap;

        auto old_buckets = std::move(buckets_);
        buckets_ = std::move(new_buckets);
        size_ = 0;

        for (auto& b : old_buckets) {
            if (b.occupied) {
                insert(b.key, b.value);
            }
        }
    }

    size_t hash(const K& key) const {
        return std::hash<K>{}(key) % capacity_;
    }

    size_t capacity_;
    size_t size_;
    std::vector<Bucket> buckets_;
};
