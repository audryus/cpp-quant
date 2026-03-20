#pragma once
#include <string>
#include <string_view>
#include <unordered_set>

class SymbolPool {
public:
    // Recebe qualquer sequencia de chars, retorna string_view estavel.
    // "Estavel" = o ponteiro nunca invalida enquanto o pool existir.
    std::string_view intern(std::string_view symbol) {
        // unordered_set garante que cada string e armazenada uma unica vez.
        // emplace retorna {iterator, bool} -- o iterator aponta para o elemento
        // existente ou para o recem-inserido.
        auto [it, inserted] = pool_.emplace(symbol);
        return *it;
    }

    size_t size() const { return pool_.size(); }

private:
    // set de strings -- cada string unica armazenada exatamente uma vez
    std::unordered_set<std::string> pool_;
};
