# C++ Quant Developer Roadmap
> Documento de contexto persistente — use este arquivo para retomar o aprendizado em qualquer sessão, com qualquer IA.

---

## 🤖 Instruções para IA (leia isto primeiro)

Se você é uma IA recebendo este documento como contexto, siga estas instruções:

1. **Identifique a fase atual** na seção `## 📍 Estado de Progresso` abaixo
2. **Leia o perfil do aluno** na seção `## 👤 Perfil`
3. **Retome exatamente** do ponto marcado com `[ ] EM ANDAMENTO` ou do próximo item após o último `[x] CONCLUÍDO`
4. **Não re-explique** conceitos já marcados como concluídos, a não ser que o aluno peça revisão explicitamente
5. **Mantenha o estilo**: direto, sem romantização, comparações com Java/Go quando relevante, foco em performance real
6. **Ao final de cada sessão**, peça ao aluno para atualizar as checkboxes neste arquivo antes de encerrar
7. **API Binance em uso**: dados públicos, sem autenticação necessária. WebSocket: `wss://stream.binance.com:9443` / REST: `https://api.binance.com`
8. **Idioma**: responda sempre em português brasileiro

---

## 👤 Perfil do Aluno

| Campo | Valor |
|-------|-------|
| **Linguagens conhecidas** | Java (backend sênior), Go (experiência considerável) |
| **Sistema operacional** | Arch Linux |
| **Estilo de aprendizado** | Aprende fazendo — precisa de projeto real para internalizar conceitos |
| **Objetivo final** | Tornar-se Quantitative Developer em C++ |
| **Motivação** | Mercado paga bem, poucos candidatos qualificados |
| **Nível C++ atual** | Zero (nunca instalou, nunca compilou) |

**Transferências de conhecimento importantes:**
- Java → entende arquitetura de sistemas, OOP, gerenciamento de dependências
- Go → entende concorrência (goroutines/channels), vai ajudar a aprender atomics/lock-free mais rápido
- Ambas → não tem hábito de pensar em memória manual — esse é o principal ajuste mental da Fase 1

---

## 📍 Estado de Progresso

> **Atualize este bloco ao final de cada sessão de estudo.**
> Marque `[x]` para concluído, `[~]` para em andamento, `[ ]` para não iniciado.

### Fase 0 — Setup & Mindset
- [ ] Instalação do toolchain (gcc, clang, cmake, ninja, gdb, valgrind, perf)
- [ ] Instalação dos sanitizers (ASan, TSan, UBSan)
- [ ] CMakeLists.txt base criado com flags corretas
- [ ] Primeiro programa compilado (`Hello, low latency world`)
- [ ] Compilação com `-O2` e com `-fsanitize=address` testada

### Fase 1 — C++ de verdade (memória & ownership)
- [ ] Stack vs Heap entendido (comparação com Java)
- [ ] Ponteiros (`*`, `&`) — leitura e escrita
- [ ] RAII — o padrão que substitui GC
- [ ] Construtores e destrutores
- [ ] Copy vs Move semantics
- [ ] Rule of 5
- [ ] `const` correctness
- [ ] `string_view` (zero-copy strings)
- [ ] **Projeto 1a**: HashMap do zero (open addressing)
  - [ ] Insert / Get / Delete
  - [ ] Resize automático
  - [ ] Benchmark vs `std::unordered_map`
- [ ] **Projeto 1b**: String pool / intern table para símbolos de ativos

### Fase 2 — C++ moderno + performance
- [ ] Templates básicos
- [ ] Concepts (C++20)
- [ ] STL: `vector`, `array`, `map`, `unordered_map`
- [ ] Cache line (64 bytes) — o que é e por que importa
- [ ] AoS vs SoA (Array of Structs vs Struct of Arrays)
- [ ] Branch prediction — como ajudar o compilador
- [ ] `constexpr` e avaliação em tempo de compilação
- [ ] `inline` e o custo real de chamadas de função
- [ ] Uso do `perf` para medir cache misses
- [ ] **Projeto 2**: Order Book
  - [ ] Estrutura Bid/Ask com price levels
  - [ ] Matching engine simples
  - [ ] Benchmark: ordens processadas por segundo
  - [ ] Familiarização com formato de dados Binance (price + quantity)

### Fase 3 — Sistemas de baixa latência
- [ ] `std::thread` e ciclo de vida de threads
- [ ] `std::mutex` e `std::lock_guard`
- [ ] `std::atomic` — operações atômicas
- [ ] Memory order: `relaxed`, `acquire`, `release`, `seq_cst`
- [ ] False sharing (cache line entre threads)
- [ ] SPSC ring buffer lock-free
- [ ] TCP sockets em C++ (POSIX)
- [ ] WebSocket — protocolo e handshake
- [ ] Parser de JSON sem alocação (simdjson ou manual)
- [ ] **Projeto 3a**: Binance WebSocket Feed (dados reais ao vivo)
  - [ ] Conexão em `wss://stream.binance.com:9443/ws/btcusdt@depth`
  - [ ] Parse do JSON de order book update
  - [ ] Atualização do Order Book da Fase 2 com dados reais
  - [ ] Medição de latência end-to-end
- [ ] **Projeto 3b**: Ring Buffer lock-free
  - [ ] SPSC ring buffer entre thread de rede e thread de processamento
  - [ ] Benchmark vs versão com mutex
  - [ ] Integração com o feed da Binance

### Fase 4 — Fundamentos Quant (paralelo)
- [ ] Conceito de derivativo (o que é, para que serve)
- [ ] Volatilidade histórica — cálculo manual
- [ ] Black-Scholes — derivação intuitiva (sem decorar fórmula)
- [ ] Greeks: Delta, Gamma, Theta, Vega
- [ ] Monte Carlo simulation — conceito e implementação
- [ ] Movimento Browniano — intuição
- [ ] Market making — o que é, como funciona
- [ ] Arbitragem — tipos e conceitos
- [ ] Sharpe Ratio e métricas de risco
- [ ] **Projeto 4**: Pricing Engine com dados Binance
  - [ ] Busca de klines (candlesticks) via REST: `GET /api/v3/klines`
  - [ ] Cálculo de volatilidade histórica com dados reais
  - [ ] Implementação de Black-Scholes
  - [ ] Monte Carlo simulation
  - [ ] Comparação preço teórico vs mercado

### Fase 5 — Sistema completo (Mini HFT com Binance)
- [ ] Arquitetura geral desenhada e documentada
- [ ] Pipeline: Feed → Parser → Ring Buffer → Order Book
- [ ] Strategy Engine implementado
- [ ] Estratégia escolhida e implementada (spread arb ou market making)
- [ ] Order Simulator (paper trading — sem dinheiro real)
- [ ] P&L Logger com timestamp de microssegundo
- [ ] Medição de latência com RDTSC
- [ ] Flamegraph gerado e analisado
- [ ] Valgrind massif: análise de uso de memória
- [ ] Sistema rodando ao vivo por >1 hora sem crash
- [ ] README do projeto escrito (para portfólio)

---

## ⚙️ Fase 0 — Setup & Mindset

**Duração estimada:** 1–3 dias  
**Objetivo:** Ambiente profissional rodando no Arch Linux antes de escrever qualquer lógica

### Instalação

```bash
# Toolchain principal
sudo pacman -S base-devel clang gcc cmake ninja gdb lldb valgrind

# Ferramentas de profiling e análise
sudo pacman -S perf linux-tools

# Utilitários
sudo pacman -S git clang-tools-extra  # inclui clang-tidy e clang-format
```

### Estrutura de projeto base

```
cpp-quant/
├── CMakeLists.txt
├── src/
│   └── main.cpp
├── tests/
│   └── CMakeLists.txt
└── scripts/
    ├── build_debug.sh     # com sanitizers
    └── build_release.sh   # com -O3
```

### CMakeLists.txt base

```cmake
cmake_minimum_required(VERSION 3.20)
project(cpp_quant CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Flags de warning (obrigatório desde o dia 1)
add_compile_options(-Wall -Wextra -Wpedantic)

# Target principal
add_executable(main src/main.cpp)

# Build types
# Debug: cmake -DCMAKE_BUILD_TYPE=Debug ..
# Release: cmake -DCMAKE_BUILD_TYPE=Release ..

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(main PRIVATE
        -fsanitize=address,undefined
        -fno-omit-frame-pointer
        -g
    )
    target_link_options(main PRIVATE -fsanitize=address,undefined)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_options(main PRIVATE -O3 -march=native)
endif()
```

### Scripts de build

```bash
# scripts/build_debug.sh
#!/bin/bash
mkdir -p build_debug && cd build_debug
cmake .. -DCMAKE_BUILD_TYPE=Debug -G Ninja
ninja -j$(nproc)
```

```bash
# scripts/build_release.sh
#!/bin/bash
mkdir -p build_release && cd build_release
cmake .. -DCMAKE_BUILD_TYPE=Release -G Ninja
ninja -j$(nproc)
```

### Primeiro programa (test de sanidade)

```cpp
// src/main.cpp
#include <iostream>
#include <chrono>

int main() {
    auto start = std::chrono::high_resolution_clock::now();

    std::cout << "Hello, low latency world\n";

    auto end = std::chrono::high_resolution_clock::now();
    auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

    std::cout << "Tempo: " << ns << " ns\n";
    return 0;
}
```

---

## 🧱 Fase 1 — C++ de Verdade (memória & ownership)

**Duração estimada:** 3–5 semanas  
**Pré-requisito:** Fase 0 concluída  
**Objetivo:** Sair do mindset GC/JVM e entender que em C++ você é o gerenciador de memória

### Conceitos-chave com contexto Java/Go

| Conceito | Java/Go | C++ |
|----------|---------|-----|
| Memória de objetos | Heap sempre, GC coleta | Stack (automático) ou Heap (manual via `new`) |
| Destruição | GC não determinístico | Determinístico: destrutor roda quando sai de escopo |
| "Ownership" | GC rastreia referências | Você define quem é dono, quando libera |
| `null` seguro | NullPointerException | Ponteiro cru pode ser inválido sem aviso |
| Gerenciado | `new Object()` — GC cuida | `new Foo()` — você chama `delete`, ou usa `unique_ptr` |

### RAII — o conceito mais importante do C++

RAII (Resource Acquisition Is Initialization) é o padrão que substitui GC. Recurso adquirido no construtor, liberado no destrutor. Quando o objeto sai de escopo — mesmo com exceção — o destrutor roda. Sempre.

```cpp
// Sem RAII (estilo C — não faça isso)
FILE* f = fopen("data.bin", "rb");
// ... se acontecer exceção aqui, fopen vaza
fclose(f);

// Com RAII (estilo C++ correto)
std::ifstream f("data.bin");  // abre no construtor
// ... exceções? sem problema
// quando f sai de escopo, fecha automaticamente
```

### Smart pointers

```cpp
#include <memory>

// unique_ptr: um único dono. Cópia não permitida, só move.
auto ptr = std::make_unique<int>(42);
auto ptr2 = std::move(ptr);  // ptr agora é nullptr

// shared_ptr: múltiplos donos. Ref count.
// ATENÇÃO: tem overhead — use só quando realmente precisar de múltiplos donos
auto shared = std::make_shared<double>(3.14);
```

### Projetos da Fase 1

#### Projeto 1a — HashMap do zero

Implemente `HashMap<K, V>` com open addressing (linear probing).

**Interface esperada:**
```cpp
template<typename K, typename V>
class HashMap {
public:
    void insert(const K& key, const V& value);
    V* get(const K& key);          // retorna nullptr se não existe
    bool remove(const K& key);
    size_t size() const;
    float load_factor() const;     // quando > 0.75, faz resize
private:
    // sua implementação
};
```

**Objetivos de aprendizado:**
- Alocar/desalocar arrays manualmente
- Implementar resize (reinsere tudo)
- Enfrentar um segfault real e debugar com gdb
- Comparar performance vs `std::unordered_map` com `perf`

#### Projeto 1b — String intern table

```cpp
class SymbolPool {
public:
    // Retorna string_view estável para o símbolo
    // "BTCUSDT" -> sempre o mesmo ponteiro interno
    std::string_view intern(std::string_view symbol);
    size_t size() const;
};
```

Usar `std::string_view` ao invés de `std::string` é zero-copy — você referencia memória existente sem copiar. Fundamental para parsing de mensagens de mercado.

---

## ⚡ Fase 2 — C++ Moderno + Performance

**Duração estimada:** 5–8 semanas  
**Pré-requisito:** Fase 1 concluída  
**Objetivo:** Escrever C++ que é rápido de verdade, não apenas correto

### Cache — o conceito que muda tudo

Acessar RAM: ~100ns. Acessar cache L1: ~1ns. A CPU carrega dados em blocos de **64 bytes** (cache line). Se seus dados estão espalhados na memória, você paga 100ns por acesso. Se estão contíguos, paga 1ns.

```cpp
// LENTO — AoS (Array of Structs)
// Quando você acessa só os preços, carrega também quantity e timestamp (lixo)
struct Order { double price; double quantity; int64_t timestamp; };
std::vector<Order> orders;

// RÁPIDO — SoA (Struct of Arrays)
// Preços ficam contíguos: uma cache line = 8 preços seguidos
struct OrderBook {
    std::vector<double> prices;
    std::vector<double> quantities;
    std::vector<int64_t> timestamps;
};
```

### Projeto 2 — Order Book

**Estrutura esperada:**
```cpp
struct PriceLevel {
    double price;
    double quantity;
};

class OrderBook {
public:
    void update_bid(double price, double quantity);  // qty=0 remove o nível
    void update_ask(double price, double quantity);
    double best_bid() const;
    double best_ask() const;
    double mid_price() const;
    double spread() const;

    // Para o benchmark
    void process_batch(std::span<PriceLevel> bids, std::span<PriceLevel> asks);

private:
    // Dica: use std::map<double, double, std::greater<double>> para bids
    //       use std::map<double, double> para asks
    // Ou implemente com array ordenado para menor latência
};
```

**Formato de dados Binance** (que você vai usar na Fase 3):
```json
{
  "bids": [["43521.50", "0.123"], ["43521.00", "0.456"]],
  "asks": [["43522.00", "0.789"], ["43522.50", "0.321"]]
}
```

---

## 🔌 Fase 3 — Sistemas de Baixa Latência

**Duração estimada:** 6–9 semanas  
**Pré-requisito:** Fase 2 concluída  
**Objetivo:** Concorrência real, rede real, dados reais da Binance

### API Binance — referência rápida

**WebSocket (streaming em tempo real — sem autenticação):**
```
wss://stream.binance.com:9443/ws/<stream>

Streams úteis:
  btcusdt@depth          — order book updates (diff)
  btcusdt@depth@100ms    — updates agregados a cada 100ms
  btcusdt@trade          — trades em tempo real
  btcusdt@bookTicker     — best bid/ask em tempo real
  !bookTicker            — best bid/ask de TODOS os pares
```

**REST (dados históricos — sem autenticação para endpoints públicos):**
```
GET https://api.binance.com/api/v3/depth?symbol=BTCUSDT&limit=100
GET https://api.binance.com/api/v3/klines?symbol=BTCUSDT&interval=1h&limit=500
GET https://api.binance.com/api/v3/ticker/bookTicker?symbol=BTCUSDT
GET https://api.binance.com/api/v3/trades?symbol=BTCUSDT&limit=1000
```

**Exemplo de mensagem `@depth` (order book update):**
```json
{
  "e": "depthUpdate",
  "E": 1672531200000,
  "s": "BTCUSDT",
  "U": 157,
  "u": 160,
  "b": [["43521.50", "0.00000000"]],
  "a": [["43522.00", "1.23456789"]]
}
```
`b` = bids, `a` = asks. Quantidade `"0.00000000"` = remover o nível.

### Projeto 3a — Binance WebSocket Feed

**Dependências necessárias:**
```bash
sudo pacman -S openssl boost
# ou use libwebsockets:
sudo pacman -S libwebsockets
```

**Estrutura do projeto:**
```cpp
class BinanceFeed {
public:
    BinanceFeed(const std::string& symbol, OrderBook& book);
    void connect();    // inicia WebSocket
    void run();        // loop de eventos (bloqueante)
    void stop();

    // Callback chamado a cada mensagem
    std::function<void(uint64_t event_time, uint64_t receive_time)> on_update;

private:
    void parse_depth_update(std::string_view json);
    OrderBook& book_;
};
```

**Medição de latência:**
```cpp
// Timestamp de recebimento em nanossegundos
auto receive_ns = std::chrono::high_resolution_clock::now()
                    .time_since_epoch().count();

// event_time vem do campo "E" da mensagem (milliseconds)
uint64_t event_ms = json["E"];
uint64_t latency_ms = (receive_ns / 1'000'000) - event_ms;
```

### Projeto 3b — Ring Buffer lock-free

```cpp
// SPSC = Single Producer Single Consumer
// Thread de rede produz, thread de processamento consome
// Sem mutex = sem context switch = latência determinística
template<typename T, size_t N>
class SPSCRingBuffer {
    static_assert((N & (N-1)) == 0, "N deve ser potência de 2");
public:
    bool push(const T& item);  // chamado pela thread produtora
    bool pop(T& item);         // chamado pela thread consumidora
    size_t size() const;

private:
    alignas(64) std::atomic<size_t> head_{0};  // alignas(64) = uma cache line cada
    alignas(64) std::atomic<size_t> tail_{0};
    T buffer_[N];
};
```

---

## 📐 Fase 4 — Fundamentos Quant (paralelo)

**Duração estimada:** 4–6 semanas (roda em paralelo com Fases 2 e 3)  
**Objetivo:** Matemática suficiente para implementar modelos e conversar com quants

### Conceitos essenciais

**Volatilidade histórica** (o que você vai calcular com dados Binance):
```
σ_anual = desvio_padrão(retornos_diários) × √252
retorno_diário[i] = ln(close[i] / close[i-1])
```

**Black-Scholes** (preço de uma opção call europeia):
```
C = S × N(d1) - K × e^(-rT) × N(d2)

d1 = [ln(S/K) + (r + σ²/2) × T] / (σ × √T)
d2 = d1 - σ × √T

S = preço atual do ativo
K = preço de exercício (strike)
r = taxa livre de risco
T = tempo até vencimento (em anos)
σ = volatilidade anualizada
N() = CDF da distribuição normal
```

**Binance REST para dados históricos:**
```cpp
// GET /api/v3/klines?symbol=BTCUSDT&interval=1d&limit=365
// Resposta: array de [open_time, open, high, low, close, volume, ...]
// Use close prices para calcular retornos e volatilidade
```

### Projeto 4 — Pricing Engine

```cpp
struct BlackScholesResult {
    double call_price;
    double put_price;
    double delta;      // ∂C/∂S
    double gamma;      // ∂²C/∂S²
    double theta;      // ∂C/∂t
    double vega;       // ∂C/∂σ
};

class PricingEngine {
public:
    // Usa dados históricos da Binance para calcular σ
    double historical_volatility(const std::vector<double>& closes, int window = 30);

    BlackScholesResult black_scholes(
        double S,  // spot price (do bookTicker Binance)
        double K,  // strike
        double r,  // risk-free rate (use 0.05 como placeholder)
        double T,  // time to expiry em anos
        double sigma
    );

    // Monte Carlo como validação do Black-Scholes
    double monte_carlo_call(double S, double K, double r, double T,
                           double sigma, int n_paths = 100000);
};
```

---

## 🚀 Fase 5 — Sistema Completo (Mini HFT com Binance)

**Duração estimada:** 8–12 semanas  
**Pré-requisito:** Todas as fases anteriores  
**Objetivo:** Sistema de produção em portfólio — paper trading ao vivo

### Arquitetura final

```
┌─────────────────────────────────────────────────────┐
│                   BinanceFeed                        │
│  wss://stream.binance.com — @depth + @bookTicker     │
└──────────────────┬──────────────────────────────────┘
                   │ SPSCRingBuffer<MarketEvent, 4096>
                   ▼
┌─────────────────────────────────────────────────────┐
│                  MarketDataProcessor                  │
│  - Parser JSON (zero-allocation)                     │
│  - Atualiza OrderBook                                │
│  - Calcula mid price, spread, imbalance              │
└──────────────────┬──────────────────────────────────┘
                   │ SPSCRingBuffer<Signal, 1024>
                   ▼
┌─────────────────────────────────────────────────────┐
│                  StrategyEngine                       │
│  Opção A: Spread Arbitrage BTC/ETH                   │
│  Opção B: Simple Market Making                       │
│  Opção C: Mean Reversion no spread                   │
└──────────────────┬──────────────────────────────────┘
                   │
                   ▼
┌─────────────────────────────────────────────────────┐
│                  OrderSimulator                       │
│  - Paper trading (sem dinheiro real)                 │
│  - Simula fills com slippage realista                │
│  - Mantém posição e P&L                              │
└──────────────────┬──────────────────────────────────┘
                   │
                   ▼
┌─────────────────────────────────────────────────────┐
│                  LatencyLogger                        │
│  - RDTSC timestamps em cada estágio                  │
│  - Histograma de latência (p50/p95/p99/p999)         │
│  - Output: CSV para análise                          │
└─────────────────────────────────────────────────────┘
```

### Medição de latência com RDTSC

```cpp
// RDTSC = Read Time-Stamp Counter
// Precisão de nanosegundo, overhead mínimo (~10 ciclos)
inline uint64_t rdtsc() {
    uint32_t lo, hi;
    __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
    return ((uint64_t)hi << 32) | lo;
}

// Uso no pipeline
uint64_t t0 = rdtsc(); // recebeu do socket
uint64_t t1 = rdtsc(); // terminou parse
uint64_t t2 = rdtsc(); // terminou update do order book
uint64_t t3 = rdtsc(); // strategy tomou decisão

// Converter ciclos para nanosegundos:
// nanoseconds = cycles / (cpu_freq_ghz)
// ex: 3.5GHz → 1 ciclo = ~0.286ns → 1000 ciclos = ~286ns
```

### Estratégia A — Spread Arbitrage BTC/ETH (recomendada para começar)

```
Ideia: O ratio BTC/ETH tende a ser estável.
Quando o spread desvia do histórico, há uma oportunidade.

ratio = mid_price_BTC / mid_price_ETH
z_score = (ratio - media_movel) / desvio_padrao_movel

Se z_score > 2.0  → BTC caro relativo ao ETH → "vende" BTC, "compra" ETH
Se z_score < -2.0 → ETH caro relativo ao BTC → "compra" BTC, "vende" ETH
Fecha posição quando z_score retorna a 0
```

---

## 🛠️ Ferramentas de Profiling

### perf — análise de CPU

```bash
# Rodar com coleta de eventos
perf stat ./build_release/main

# Cache misses (fundamental para validar AoS vs SoA)
perf stat -e cache-misses,cache-references,instructions,cycles ./main

# Flamegraph (instalar perf + flamegraph.pl)
perf record -g ./main
perf script | stackcollapse-perf.pl | flamegraph.pl > flame.svg
```

### valgrind — memória e cache

```bash
# Erros de memória
valgrind --leak-check=full ./main

# Simulação de cache (cachegrind)
valgrind --tool=cachegrind ./main
cg_annotate cachegrind.out.*

# Heap profiling
valgrind --tool=massif ./main
ms_print massif.out.* | head -50
```

### ASan/TSan (sanitizers no código)

```bash
# Já configurado no CMakeLists.txt para Debug build
# ASan: detecta buffer overflow, use-after-free, double-free
# TSan: detecta data races entre threads
# UBSan: detecta undefined behavior (signed overflow, etc.)

./scripts/build_debug.sh
./build_debug/main  # erros aparecem automaticamente em stderr
```

---

## 📚 Referências

### Livros (em ordem de prioridade)

1. **C++ Primer** (Lippman) — fundação sólida, não pule
2. **Effective Modern C++** (Meyers) — move semantics, smart pointers, lambdas
3. **C++ High Performance** (Andriaan/Hansson) — o livro do quant dev
4. **The Art of Writing Efficient Programs** (Pikus) — performance avançada

### Sites e recursos

- **cppreference.com** — documentação definitiva da STL
- **godbolt.org** — vê o assembly gerado em tempo real (fundamental para entender otimizações)
- **quick-bench.com** — micro-benchmarks rápidos no browser
- **Binance API docs**: https://binance-docs.github.io/apidocs/spot/en/

### Empresas quant dev que contratam (pesquise no LinkedIn/Glassdoor)

- Jane Street, Citadel, Two Sigma, DE Shaw (top tier, C++ pesado)
- XTX Markets, Virtu Financial, Jump Trading (HFT puro)
- Optiver, IMC Trading, Flow Traders (market making)
- Aquis Exchange, IG Group (fintech com foco em matching engines)

### Termos técnicos para entrevistas

| Termo | O que é |
|-------|---------|
| **Latency** | Tempo entre evento de mercado e resposta do sistema |
| **Throughput** | Mensagens processadas por segundo |
| **Jitter** | Variação na latência (p99 - p50) |
| **Fill rate** | % de ordens executadas |
| **Slippage** | Diferença entre preço esperado e executado |
| **NBBO** | National Best Bid and Offer |
| **Market microstructure** | Como os mecanismos de mercado funcionam internamente |

---

## 📝 Log de Sessões

> Adicione uma entrada ao final de cada sessão de estudo.

```
[DATA] — [FASE] — O que foi feito / O que ficou pendente / Dúvidas para próxima sessão
```

_Exemplo:_
```
[2025-03-20] — Fase 0 — Instalei toolchain completo. CMakeLists.txt base funcionando.
               Primeiro programa compilado com ASan ativo. Próximo: Fase 1, stack vs heap.
```

---

*Documento gerado em: 2025-03-20*  
*Versão do roadmap: 1.0*  
*Atualizar este campo quando fizer revisões significativas no plano*
