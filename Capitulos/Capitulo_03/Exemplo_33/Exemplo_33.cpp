//------------------------------------------------------------------------------
// Exemplo_33.cpp
//------------------------------------------------------------------------------
// Programa de exemplo para demonstração do método de diferenças finitas
// na geração de malhas unidimensionais com distribuição exponencial.
//
// Este programa implementa a formulação exponencial para concentração de nós
// em uma extremidade do domínio, conforme exemplo do livro.
//
// Autor: João Flávio Vieira de Vasconcellos
// Versão: 1.0.0
// Data: 2025-12-01
// Testado: GCC 14.2.0 em 2025-12-01
//------------------------------------------------------------------------------

/**
 * @file Exemplo_33.cpp
 * @brief Exemplo didático de geração de malha unidimensional exponencial
 * 
 * Este programa demonstra o uso do método de diferenças finitas para
 * gerar uma malha unidimensional com espaçamento variável segundo uma
 * distribuição exponencial, concentrando nós em uma extremidade.
 * 
 * Equação implementada:
 * x_i = x_I + L * (exp(β·i) - 1) / (exp(β·(N-1)) - 1)
 * 
 * Onde:
 * - x_I: coordenada inicial do domínio
 * - L: comprimento total do domínio
 * - β: parâmetro de concentração (β > 0)
 * - N: número total de nós
 * - i: índice do nó (0 ≤ i ≤ N-1)
 * 
 * Características:
 * - β → 0: recupera malha uniforme
 * - β grande: concentra nós na extremidade inicial (x = x_I)
 * 
 * Objetivos pedagógicos:
 * 1. Demonstrar distribuição exponencial em malhas
 * 2. Mostrar transição suave entre malhas uniformes e concentradas
 * 3. Ilustrar boas práticas de programação numérica
 * 4. Apresentar validação rigorosa de parâmetros
 * 
 * Referência: Livro "Solução Numérica de Equações Diferenciais - 
 *              O Método de Volumes Finitos" (Exemplo 2.2)
 * 
 * @copyright Copyright (c) 2024
 * @license GNU GPL v3
 */

//------------------------------------------------------------------------------
// Seção 1: Inclusões Necessárias
//------------------------------------------------------------------------------

#include <algorithm>    // Para algoritmos numéricos
#include <chrono>       // Para medir tempo de execução
#include <cmath>        // Para funções matemáticas (exp, fabs)
#include <execution>    // Para execução paralela  
#include <iomanip>      // Para formatação de saída
#include <iostream>     // Para entrada/saída padrão
#include <limits>       // Para limites numéricos
#include <stdexcept>    // Para exceções padrão
#include <vector>       // Para vetores dinâmicos

//------------------------------------------------------------------------------
// Seção 2: Definições e Constantes
//------------------------------------------------------------------------------

/**
 * @typedef Real
 * @brief Tipo para números de ponto flutuante
 * 
 * Facilita a mudança de precisão (double para float, se necessário)
 */
using Real = double;

/**
 * @typedef VetorReal
 * @brief Vetor de números reais
 */
using VetorReal = std::vector<Real>;

/**
 * @def VERSION_MAJOR
 * @brief Versão principal do programa
 */
#define VERSION_MAJOR 1

/**
 * @def VERSION_MINOR
 * @brief Versão secundária do programa
 */
#define VERSION_MINOR 0

/**
 * @def VERSION_PATCH
 * @brief Versão de correção do programa
 */
#define VERSION_PATCH 0

/**
 * @def BUILD_DATE
 * @brief Data de compilação do programa
 */
#define BUILD_DATE __DATE__

/**
 * @def BUILD_TIME
 * @brief Hora de compilação do programa
 */
#define BUILD_TIME __TIME__

/**
 * @def EPSILON
 * @brief Tolerância para comparações numéricas
 */
constexpr Real EPSILON = 1.0e-12;

/**
 * @def LIMITE_PARALELO
 * @brief Limite de nós para ativar paralelização
 */
constexpr unsigned LIMITE_PARALELO = 10000;

/**
 * @def LIMITE_IMPRESSAO
 * @brief Limite de nós para impressão completa
 */
constexpr unsigned LIMITE_IMPRESSAO = 10;

//------------------------------------------------------------------------------
// Seção 3: Protótipos de Funções
//------------------------------------------------------------------------------

/**
 * @brief Imprime informações da versão do programa
 */
void ImprimirVersao();

/**
 * @brief Valida os parâmetros de entrada da malha exponencial
 * 
 * @param x_inicio Coordenada inicial
 * @param comprimento Comprimento total do domínio
 * @param beta Parâmetro de concentração exponencial
 * @param n_nos Número de nós
 * @throws std::invalid_argument Se algum parâmetro for inválido
 */
void ValidarParametrosExponencial(
    [[maybe_unused]] const Real& x_inicio,
    const Real& comprimento,
    const Real& beta,
    const unsigned& n_nos
);

/**
 * @brief Calcula fator de normalização para a distribuição exponencial
 * 
 * @param beta Parâmetro de concentração
 * @param n_nos Número de nós
 * @return Real Fator de normalização D = exp(β·(N-1)) - 1
 * 
 * @note Implementação robusta para valores extremos de β
 */
[[nodiscard]] Real CalcularFatorNormalizacao(
    const Real& beta,
    const unsigned& n_nos
) noexcept;

/**
 * @brief Calcula coordenada de um nó específico usando fórmula exponencial
 * 
 * @param x_inicio Coordenada inicial
 * @param comprimento Comprimento total
 * @param beta Parâmetro de concentração
 * @param indice Índice do nó (0 ≤ i ≤ N-1)
 * @param fator_norm Fator de normalização pré-calculado
 * @return Real Coordenada x_i do nó
 * 
 * @note Função pura e livre de efeitos colaterais
 */
[[nodiscard]] Real CalcularCoordenadaExponencial(
    const Real& x_inicio,
    const Real& comprimento,
    const Real& beta,
    const unsigned& indice,
    const Real& fator_norm
) noexcept;

/**
 * @brief Calcula malha exponencial usando algoritmo sequencial
 * 
 * @param coordenadas Vetor para armazenar coordenadas (pré-alocado)
 * @param x_inicio Coordenada inicial
 * @param comprimento Comprimento total
 * @param beta Parâmetro de concentração
 * @param n_nos Número de nós
 */
void CalcularMalhaExponencialSequencial(
    VetorReal& coordenadas,
    const Real& x_inicio,
    const Real& comprimento,
    const Real& beta,
    const unsigned& n_nos
);

/**
 * @brief Calcula malha exponencial usando algoritmo paralelo (C++17)
 * 
 * @param coordenadas Vetor para armazenar coordenadas (pré-alocado)
 * @param x_inicio Coordenada inicial
 * @param comprimento Comprimento total
 * @param beta Parâmetro de concentração
 * @param n_nos Número de nós
 */
void CalcularMalhaExponencialParalelo(
    VetorReal& coordenadas,
    const Real& x_inicio,
    const Real& comprimento,
    const Real& beta,
    const unsigned& n_nos
);

/**
 * @brief Seleciona automaticamente o melhor algoritmo baseado no tamanho
 * 
 * @param coordenadas Vetor para armazenar coordenadas
 * @param x_inicio Coordenada inicial
 * @param comprimento Comprimento total
 * @param beta Parâmetro de concentração
 * @param n_nos Número de nós
 * @return std::string Nome do algoritmo usado
 */
std::string CalcularMalhaExponencial(
    VetorReal& coordenadas,
    const Real& x_inicio,
    const Real& comprimento,
    const Real& beta,
    const unsigned& n_nos
);

/**
 * @brief Imprime os resultados formatados da malha exponencial
 * 
 * @param coordenadas Vetor de coordenadas calculadas
 * @param x_inicio Coordenada inicial
 * @param comprimento Comprimento total
 * @param beta Parâmetro de concentração
 * @param n_nos Número de nós
 * @param algoritmo Nome do algoritmo usado
 * @param tempo_ms Tempo de execução em milissegundos
 */
void ImprimirResultadosExponencial(
    const VetorReal& coordenadas,
    const Real& x_inicio,
    const Real& comprimento,
    const Real& beta,
    const unsigned& n_nos,
    const std::string& algoritmo,
    const Real& tempo_ms
);

//------------------------------------------------------------------------------
// Seção 4: Função Principal
//------------------------------------------------------------------------------

int main() {
    
    //--------------------------------------------------------------------------
    // Inicio da execução
    //--------------------------------------------------------------------------
    ImprimirVersao();
    
    //--------------------------------------------------------------------------
    // Parâmetros da Malha de Exemplo (Caso β = 1.0)
    //--------------------------------------------------------------------------
    constexpr Real X_INICIO = 0.0;     // Coordenada inicial do domínio
    constexpr Real COMPRIMENTO = 1.0;  // Comprimento total do domínio
    constexpr Real BETA = 1.0;         // Parâmetro de concentração
    constexpr unsigned N_NOS = 11;     // Número de nós da malha
    
    //--------------------------------------------------------------------------
    // Validação dos Parâmetros
    //--------------------------------------------------------------------------
    try {
        ValidarParametrosExponencial(X_INICIO, COMPRIMENTO, BETA, N_NOS);
        std::cout << "  ✓ Parâmetros validados com sucesso\n";
    } catch (const std::exception& e) {
        std::cerr << "  ✗ Erro na validação: " << e.what() << "\n";
        return EXIT_FAILURE;
    }
    
    //--------------------------------------------------------------------------
    // Preparação do Vetor de Coordenadas
    //--------------------------------------------------------------------------
    VetorReal coordenadas(N_NOS);
    
    //--------------------------------------------------------------------------
    // Cálculo da Malha com Medição de Tempo
    //--------------------------------------------------------------------------
    auto inicio = std::chrono::high_resolution_clock::now();
    
    std::string algoritmo_usado = CalcularMalhaExponencial(
        coordenadas, X_INICIO, COMPRIMENTO, BETA, N_NOS
    );
    
    auto fim = std::chrono::high_resolution_clock::now();
    auto duracao = std::chrono::duration_cast<std::chrono::microseconds>(fim - inicio);
    double tempo_ms = duracao.count() / 1000.0;
    
    //--------------------------------------------------------------------------
    // Verificação dos Resultados
    //--------------------------------------------------------------------------
    if (coordenadas.empty()) {
        std::cerr << "✗ Erro: Nenhuma coordenada calculada\n";
        return EXIT_FAILURE;
    }
    
    // Verifica se a última coordenada está próxima do valor esperado
    Real coordenada_final_esperada = X_INICIO + COMPRIMENTO;
    Real erro = std::abs(coordenadas.back() - coordenada_final_esperada);
    
    if (erro > EPSILON) {
        std::cerr << "  ⚠ Aviso: Possível erro no cálculo. "
                  << "Diferença: " << erro << "\n";
    }
    
    //--------------------------------------------------------------------------
    // Impressão dos Resultados
    //--------------------------------------------------------------------------
    ImprimirResultadosExponencial(
        coordenadas, X_INICIO, COMPRIMENTO, BETA, N_NOS,
        algoritmo_usado, tempo_ms
    );
    
    return EXIT_SUCCESS;
}

//==============================================================================
// Seção 5: Implementação das Funções
//==============================================================================

void ImprimirVersao() {
    const int size(80);   
    std::cout << "\n\n";
    std::cout << std::string(size, '-') << std::endl;    
    std::cout << "  Exemplo 33: Geração de Malha Unidimensional Exponencial\n";
    std::cout << "  (Concentração de nós em uma extremidade)\n";
    std::cout << "  Versão: " << VERSION_MAJOR 
              << "." 
              << VERSION_MINOR << "." << VERSION_PATCH << "\n";
    std::cout << "  Compilado em: " << BUILD_DATE << " :  " 
              << std::setw(8) << std::left << BUILD_TIME << "\n";
    std::cout << "  Autor: João Flávio Vieira de Vasconcellos\n";
    std::cout << std::string(size, '-') << std::endl << "\n";    
}

void ValidarParametrosExponencial(
    [[maybe_unused]] const Real& x_inicio,
    const Real& comprimento,
    const Real& beta,
    const unsigned& n_nos
) {
    // Valida número de nós
    if (n_nos < 2) {
        throw std::invalid_argument("Número de nós deve ser pelo menos 2");
    }
    
    // Valida comprimento
    if (comprimento <= 0.0) {
        throw std::invalid_argument("Comprimento deve ser positivo");
    }
    
    // Valida parâmetro beta
    if (beta < 0.0) {
        throw std::invalid_argument("β deve ser não-negativo (β ≥ 0)");
    }
    
    // Valida se os cálculos numéricos são seguros
    if (n_nos > 1000000) {
        throw std::invalid_argument("Número de nós muito grande para exemplo");
    }
    
    // Valida se beta não causará overflow na exponencial
    if (beta > 100.0 && n_nos > 1000) {
        throw std::invalid_argument(
            "β muito grande pode causar overflow em exp(β·(N-1))"
        );
    }
    
    // Caso especial: β = 0 resulta em malha uniforme (é permitido)
    if (std::abs(beta) < EPSILON) {
        std::cout << "  Nota: β ≈ 0, malha será aproximadamente uniforme\n";
    }
}

Real CalcularFatorNormalizacao(
    const Real& beta,
    const unsigned& n_nos
) noexcept {
    // Caso β = 0: retorna (N-1) para evitar divisão por zero
    if (std::abs(beta) < EPSILON) {
        return static_cast<Real>(n_nos - 1);
    }
    
    // Calcula D = exp(β·(N-1)) - 1
    Real expoente = beta * static_cast<Real>(n_nos - 1);
    
    // Proteção contra overflow
    if (expoente > 700.0) {  // exp(700) ≈ 10^304, próximo ao limite do double
        std::cerr << "  ⚠ Aviso: exp(β·(N-1)) muito grande, possível perda de precisão\n";
    }
    
    return std::exp(expoente) - 1.0;
}

Real CalcularCoordenadaExponencial(
    const Real& x_inicio,
    const Real& comprimento,
    const Real& beta,
    const unsigned& indice,
    const Real& fator_norm
) noexcept {
    // Caso β = 0: fórmula uniforme x_i = x_I + L·i/(N-1)
    if (std::abs(beta) < EPSILON) {
        Real fator = static_cast<Real>(indice) / static_cast<Real>(fator_norm);
        return x_inicio + comprimento * fator;
    }
    
    // Fórmula exponencial: x_i = x_I + L·(exp(β·i) - 1)/D
    Real expoente = beta * static_cast<Real>(indice);
    Real numerador = std::exp(expoente) - 1.0;
    
    return x_inicio + comprimento * (numerador / fator_norm);
}

void CalcularMalhaExponencialSequencial(
    VetorReal& coordenadas,
    const Real& x_inicio,
    const Real& comprimento,
    const Real& beta,
    const unsigned& n_nos
) {
    // Pré-calcula fator de normalização (evita recalcular para cada nó)
    Real fator_norm = CalcularFatorNormalizacao(beta, n_nos);
    
    // Calcula coordenadas sequencialmente
    for (unsigned i = 0; i < n_nos; ++i) {
        coordenadas[i] = CalcularCoordenadaExponencial(
            x_inicio, comprimento, beta, i, fator_norm
        );
    }
}

void CalcularMalhaExponencialParalelo(
    VetorReal& coordenadas,
    const Real& x_inicio,
    const Real& comprimento,
    const Real& beta,
    const unsigned& n_nos
) {
    // Pré-calcula fator de normalização
    Real fator_norm = CalcularFatorNormalizacao(beta, n_nos);
    
    // Cria vetor de índices
    std::vector<unsigned> indices(n_nos);
    for (unsigned i = 0; i < n_nos; ++i) {
        indices[i] = i;
    }
    
    // Calcula coordenadas em paralelo usando transform
    std::transform(
        std::execution::par,
        indices.begin(),
        indices.end(),
        coordenadas.begin(),
        [x_inicio, comprimento, beta, fator_norm](unsigned i) {
            return CalcularCoordenadaExponencial(
                x_inicio, comprimento, beta, i, fator_norm
            );
        }
    );
}

std::string CalcularMalhaExponencial(
    VetorReal& coordenadas,
    const Real& x_inicio,
    const Real& comprimento,
    const Real& beta,
    const unsigned& n_nos
) {
    if (n_nos >= LIMITE_PARALELO) {
        std::cout << "  Usando algoritmo paralelo (" << n_nos 
                  << " nós >= " << LIMITE_PARALELO << ")\n";
        CalcularMalhaExponencialParalelo(
            coordenadas, x_inicio, comprimento, beta, n_nos
        );
        return "Paralelo (C++17 Execution Policies)";
    } else {
        std::cout << "  Usando algoritmo sequencial (" << n_nos 
                  << " nós < " << LIMITE_PARALELO << ")\n";
        CalcularMalhaExponencialSequencial(
            coordenadas, x_inicio, comprimento, beta, n_nos
        );
        return "Sequencial (Loop Tradicional)";
    }
}

void ImprimirResultadosExponencial(
    const VetorReal& coordenadas,
    const Real& x_inicio,
    const Real& comprimento,
    const Real& beta,
    const unsigned& n_nos,
    const std::string& algoritmo,
    const Real& tempo_ms
) {
    const int size(80);   
    
    //--------------------------------------------------------------------------
    // Cabeçalho dos Resultados
    //--------------------------------------------------------------------------
    std::cout << "\n\n";
    std::cout << std::string(size, '-') << std::endl;   
    std::cout << "  RESULTADOS DA SIMULAÇÃO - MALHA EXPONENCIAL\n";
    std::cout << std::string(size, '-') << std::endl;   
    
    //--------------------------------------------------------------------------
    // Parâmetros Usados
    //--------------------------------------------------------------------------
    std::cout << "\n    PARÂMETROS DA MALHA:\n";
    std::cout << std::string(size, '-') << std::endl;   
    std::cout << std::fixed << std::setprecision(6);
    std::cout << "  Coordenada inicial (x_I):     " << std::setw(14) << x_inicio << "\n";
    std::cout << "  Comprimento total (L):        " << std::setw(14) << comprimento << "\n";
    std::cout << "  Parâmetro de concentração (β):" << std::setw(14) << beta << "\n";
    std::cout << "  Número de nós (N):            " << std::setw(14) << n_nos << "\n";
    std::cout << "  Algoritmo usado:              " << std::setw(14) << algoritmo << "\n";
    std::cout << "  Tempo de cálculo:             " << std::setw(14) << tempo_ms << " ms\n";
    std::cout << std::string(size, '-') << std::endl;   
    
    //--------------------------------------------------------------------------
    // Estatísticas da Malha
    //--------------------------------------------------------------------------
    if (n_nos >= 2) {
        Real menor_espacamento = coordenadas[1] - coordenadas[0];
        Real maior_espacamento = coordenadas[n_nos-1] - coordenadas[n_nos-2];
        Real razao_espacamentos = (menor_espacamento > EPSILON) ? 
                                  maior_espacamento / menor_espacamento : 
                                  std::numeric_limits<Real>::infinity();
        
        std::cout << "\n    ESTATÍSTICAS DA MALHA:\n";
        std::cout << std::string(size, '-') << std::endl;   
        std::cout << std::scientific << std::setprecision(6);
        std::cout << "  Menor espaçamento:         " << std::setw(14) << menor_espacamento << "\n";
        std::cout << "  Maior espaçamento:         " << std::setw(14) << maior_espacamento << "\n";
        std::cout << "  Razão (maior/menor):       " << std::setw(14) << razao_espacamentos << "\n";
        std::cout << "  Espaçamento médio:         " << std::setw(14) << comprimento/(n_nos-1) << "\n";
        std::cout << std::string(size, '-') << std::endl;   
    }
    
    //--------------------------------------------------------------------------
    // Coordenadas dos Nós
    //--------------------------------------------------------------------------
    std::cout << "\n    COORDENADAS DOS " << n_nos << " NÓS:\n";
    std::cout << std::string(size, '-') << std::endl;   
    
    // Mostra apenas os primeiros e últimos nós se houver muitos
    if (n_nos <= LIMITE_IMPRESSAO) {
        // Mostra todos os nós
        std::cout << std::setw(6) << "Nó (i)" << std::setw(20) << "Coordenada (x_i)" 
                  << std::setw(20) << "Espaçamento (Δx_i)" << "\n";
        std::cout << std::string(46, '-') << "\n";
        
        std::cout << std::scientific << std::setprecision(10);
        for (unsigned i = 0; i < n_nos; ++i) {
            std::cout << std::setw(6) << i;
            std::cout << std::setw(20) << coordenadas[i];
            
            if (i > 0) {
                Real espacamento = coordenadas[i] - coordenadas[i-1];
                std::cout << std::setw(20) << espacamento;
            } else {
                std::cout << std::setw(20) << "---";
            }
            std::cout << "\n";
        }
    } else {
        // Mostra apenas os primeiros e últimos nós
        std::cout << std::setw(6) << "Nó (i)" << std::setw(20) << "Coordenada (x_i)" 
                  << std::setw(20) << "Espaçamento (Δx_i)" << "\n";
        std::cout << std::string(46, '-') << "\n";
        
        std::cout << std::scientific << std::setprecision(10);
        
        // Primeiros 3 nós
        for (unsigned i = 0; i < 3; ++i) {
            std::cout << std::setw(6) << i;
            std::cout << std::setw(20) << coordenadas[i];
            
            if (i > 0) {
                Real espacamento = coordenadas[i] - coordenadas[i-1];
                std::cout << std::setw(20) << espacamento;
            } else {
                std::cout << std::setw(20) << "---";
            }
            std::cout << "\n";
        }
        std::cout << std::setw(6) << "..." << std::setw(20) << "..." 
                  << std::setw(20) << "..." << "\n";
        
        // Últimos 3 nós
        for (unsigned i = n_nos-3; i < n_nos; ++i) {
            std::cout << std::setw(6) << i;
            std::cout << std::setw(20) << coordenadas[i];
            
            if (i > 0) {
                Real espacamento = coordenadas[i] - coordenadas[i-1];
                std::cout << std::setw(20) << espacamento;
            } else {
                std::cout << std::setw(20) << "---";
            }
            std::cout << "\n";
        }
        
        std::cout << "\n    Mostrando apenas primeiros e últimos 3 nós ";
        std::cout << "(" << n_nos << " nós no total)\n";
    }
    
    std::cout << std::string(size, '-') << "\n";
    
    //--------------------------------------------------------------------------
    // Verificação Final
    //--------------------------------------------------------------------------
    Real erro_final = std::abs(coordenadas.back() - (x_inicio + comprimento));
    
    std::cout << "\n    VERIFICAÇÃO:\n";
    std::cout << std::string(size, '-') << std::endl;   
    std::cout << std::scientific << std::setprecision(2);
    std::cout << "    Última coordenada: " << coordenadas.back() << "\n";
    std::cout << "    Valor esperado:    " << x_inicio + comprimento << "\n";
    std::cout << "    Erro absoluto:     " << erro_final << "\n";
    
    if (erro_final < EPSILON) {
        std::cout << "    Status:            ✓ Cálculo preciso\n";
    } else if (erro_final < 1e-6) {
        std::cout << "    Status:            ⚠ Pequeno erro numérico\n";
    } else {
        std::cout << "    Status:            ✗ Possível erro no algoritmo\n";
    }
    
    std::cout << std::string(size, '-') << std::endl;   
    
    //--------------------------------------------------------------------------
    // Análise do Comportamento
    //--------------------------------------------------------------------------
    std::cout << "\n    ANÁLISE DO COMPORTAMENTO (β = " << beta << "):\n";
    std::cout << std::string(size, '-') << std::endl;   
    
    if (beta < 0.1) {
        std::cout << "  - Malha quase uniforme (β → 0)\n";
        std::cout << "  - Espaçamentos aproximadamente constantes\n";
        std::cout << "  - Útil para domínios homogêneos\n";
    } else if (beta < 1.0) {
        std::cout << "  - Concentração moderada na extremidade inicial\n";
        std::cout << "  - Transição suave entre espaçamentos\n";
        std::cout << "  - Ideal para camadas limites suaves\n";
    } else if (beta < 5.0) {
        std::cout << "  - Forte concentração na extremidade inicial\n";
        std::cout << "  - Grande variação nos espaçamentos\n";
        std::cout << "  - Adequado para altos gradientes iniciais\n";
    } else {
        std::cout << "  - Concentração extrema na extremidade inicial\n";
        std::cout << "  - Primeiro espaçamento muito pequeno\n";
        std::cout << "  - Útil para simulação de fenômenos de fronteira\n";
    }
    
    std::cout << std::string(size, '-') << std::endl;   
    
    //--------------------------------------------------------------------------
    // Mensagem Final Educativa
    //--------------------------------------------------------------------------
    std::cout << "\n    CONCEITOS DEMONSTRADOS:\n";
    std::cout << std::string(size, '-') << std::endl;   
    std::cout << "    1. Distribuição exponencial em malhas\n";
    std::cout << "    2. Fórmula: x_i = x_I + L·[exp(β·i) - 1]/[exp(β·(N-1)) - 1]\n";
    std::cout << "    3. Transição β → 0 recupera malha uniforme\n";
    std::cout << "    4. β grande concentra nós em x = x_I\n";
    std::cout << "    5. Robustez numérica para casos extremos\n";
    std::cout << std::string(size, '-') << std::endl;   
}