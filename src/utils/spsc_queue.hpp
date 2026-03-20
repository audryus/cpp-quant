#pragma once
#include <atomic>
#include <cstddef>
#include <optional>

// Single Producer Single Consumer queue -- lock-free
// T    = tipo dos elementos
// N    = capacidade (deve ser potencia de 2)
// Thread de rede  → push()
// Thread de proc  → pop()
template<typename T, size_t N>
class SPSCQueue {
    static_assert((N & (N - 1)) == 0, "N deve ser potencia de 2");

public:
    // chamado pela thread produtora (rede)
    bool push(const T& item) {
        size_t head = head_.load(std::memory_order_relaxed);
        size_t next = (head + 1) & mask_;

        if (next == tail_.load(std::memory_order_acquire))
            return false;  // fila cheia

        buf_[head] = item;
        head_.store(next, std::memory_order_release);
        return true;
    }

    // chamado pela thread consumidora (processamento)
    std::optional<T> pop() {
        size_t tail = tail_.load(std::memory_order_relaxed);

        if (tail == head_.load(std::memory_order_acquire))
            return std::nullopt;  // fila vazia

        T item = buf_[tail];
        tail_.store((tail + 1) & mask_, std::memory_order_release);
        return item;
    }

    size_t size() const {
        size_t h = head_.load(std::memory_order_acquire);
        size_t t = tail_.load(std::memory_order_acquire);
        return (h - t) & mask_;
    }

    bool empty() const { return size() == 0; }

private:
    static constexpr size_t mask_ = N - 1;

    // cada atomic em sua propria cache line -- evita false sharing
    alignas(64) std::atomic<size_t> head_{0};
    alignas(64) std::atomic<size_t> tail_{0};

    T buf_[N];
};
