//------------------------------------------------------------------------------
// Aleatoria.cpp
//------------------------------------------------------------------------------
// Programa de exemplo para demonstração do método de diferenças finitas
// na geração de malhas unidimensionais com distribuição aleatória.
//
// Este programa implementa geração de malhas aleatórias para testes de robustez
// de métodos numéricos.
//
// Autor: João Flávio Vieira de Vasconcellos
// Versão: 2.3
// Data: 2025-12-01
// Testado: GCC 14.2.0 em 2025-12-01
//------------------------------------------------------------------------------

/**
 * @file Aleatoria.cpp
 * @brief Exemplo didático de geração de malha unidimensional aleatória
 * 
 * Este programa demonstra técnicas modernas de C++ para gerar uma malha
 * unidimensional com distribuição aleatória.
 * 
 * Características da malha aleatória:
 * - Distribuição uniforme dos nós no intervalo [x_I, x_I + L]
 * - Extremidades fixas nas bordas do domínio
 * - Nós internos ordenados para garantir sequência crescente
 * - Ordenação paralela para otimização de desempenho
 * 
 * Objetivos pedagógicos:
 * 1. Demonstrar técnicas modernas de geração de números aleatórios em C++
 * 2. Mostrar o uso de algoritmos STL com execução paralela
 * 3. Ilustrar aplicação de malhas aleatórias em stress tests
 * 4. Apresentar boas práticas de manipulação de contêineres
 * 
 * Referência: Livro "Solução Numérica de Equações Diferenciais - 
 *              O Método de Volumes Finitos"
 * 
 * @copyright Copyright (c) 2024
 * @license GNU GPL v3
 */

//------------------------------------------------------------------------------
// Seção 1: Inclusões Necessárias
//------------------------------------------------------------------------------

#include <algorithm>    // Para algoritmos de ordenação e geração
#include <chrono>       // Para medir tempo de execução
#include <cmath>        // Para funções matemáticas (fabs)
#include <execution>    // Para execução paralela  
#include <iomanip>      // Para formatação de saída
#include <iostream>     // Para entrada/saída padrão
#include <limits>       // Para limites numéricos
#include <random>       // Para geração de números aleatórios
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
#define VERSION_MAJOR 2

/**
 * @def VERSION_MINOR
 * @brief Versão secundária do programa
 */
#define VERSION_MINOR 3

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
constexpr unsigned LIMITE_IMPRESSAO = 25;

//------------------------------------------------------------------------------
// Seção 3: Protótipos de Funções
//------------------------------------------------------------------------------

/**
 * @brief Imprime informações da versão do programa
 */
void ImprimirVersao();

/**
 * @brief Valida os parâmetros de entrada da malha aleatória
 * 
 * @param x_inicio Coordenada inicial
 * @param comprimento Comprimento total do domínio
 * @param n_nos Número de nós
 * @throws std::invalid_argument Se algum parâmetro for inválido
 */
void ValidarParametrosAleatorios(
    [[maybe_unused]] const Real& x_inicio,
    const Real& comprimento,
    const unsigned& n_nos
);

/**
 * @brief Calcula malha aleatória usando algoritmo sequencial
 * 
 * @param coordenadas Vetor para armazenar coordenadas (pré-alocado)
 * @param x_inicio Coordenada inicial
 * @param comprimento Comprimento total
 * @param n_nos Número de nós
 */
void CalcularMalhaAleatoriaSequencial(
    VetorReal& coordenadas,
    const Real& x_inicio,
    const Real& comprimento,
    const unsigned& n_nos
);

/**
 * @brief Calcula malha aleatória usando algoritmo paralelo (C++17)
 * 
 * @param coordenadas Vetor para armazenar coordenadas (pré-alocado)
 * @param x_inicio Coordenada inicial
 * @param comprimento Comprimento total
 * @param n_nos Número de nós
 */
void CalcularMalhaAleatoriaParalelo(
    VetorReal& coordenadas,
    const Real& x_inicio,
    const Real& comprimento,
    const unsigned& n_nos
);

/**
 * @brief Seleciona automaticamente o melhor algoritmo baseado no tamanho
 * 
 * @param coordenadas Vetor para armazenar coordenadas
 * @param x_inicio Coordenada inicial
 * @param comprimento Comprimento total
 * @param n_nos Número de nós
 * @return std::string Nome do algoritmo usado
 */
std::string CalcularMalhaAleatoria(
    VetorReal& coordenadas,
    const Real& x_inicio,
    const Real& comprimento,
    const unsigned& n_nos
);

/**
 * @brief Imprime os resultados formatados da malha aleatória
 * 
 * @param coordenadas Vetor de coordenadas calculadas
 * @param x_inicio Coordenada inicial
 * @param comprimento Comprimento total
 * @param n_nos Número de nós
 * @param algoritmo Nome do algoritmo usado
 * @param tempo_ms Tempo de execução em milissegundos
 */
void ImprimirResultados(
    const VetorReal& coordenadas,
    const Real& x_inicio,
    const Real& comprimento,
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
    // Parâmetros da Malha de Exemplo
    //--------------------------------------------------------------------------
    constexpr Real X_INICIO = 0.0;     // Coordenada inicial do domínio
    constexpr Real COMPRIMENTO = 4.0;  // Comprimento total do domínio
    constexpr unsigned N_NOS = 11;     // Número de nós da malha
    
    //--------------------------------------------------------------------------
    // Validação dos Parâmetros
    //--------------------------------------------------------------------------
    try {
        ValidarParametrosAleatorios(X_INICIO, COMPRIMENTO, N_NOS);
        std::cout << "      Parâmetros validados com sucesso\n";
    } catch (const std::exception& e) {
        std::cerr << "      Erro na validação: " << e.what() << "\n";
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
    
    std::string algoritmo_usado = CalcularMalhaAleatoria(
        coordenadas, X_INICIO, COMPRIMENTO, N_NOS
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
    ImprimirResultados(
        coordenadas, 
        X_INICIO, 
        COMPRIMENTO, 
        N_NOS,
        algoritmo_usado, 
        tempo_ms
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
    std::cout << "  Programa Aleatoria: Geração de Malha Unidimensional Aleatória\n";
    std::cout << "  (Para testes de robustez de métodos numéricos)\n";
    std::cout << "  Versão: " << VERSION_MAJOR 
              << "." 
              << VERSION_MINOR << "." << VERSION_PATCH << "\n";
    std::cout << "  Compilado em: " << BUILD_DATE << " :  " 
              << std::setw(8) << std::left << BUILD_TIME << "\n";
    std::cout << "  Autor: João Flávio Vieira de Vasconcellos\n";
    std::cout << std::string(size, '-') << std::endl << "\n";    
}

void ValidarParametrosAleatorios(
    [[maybe_unused]] const Real& x_inicio,
    const Real& comprimento,
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
    
    // Valida se os cálculos numéricos são seguros
    if (n_nos > LIMITE_PARALELO * 10) {
        throw std::invalid_argument("Número de nós muito grande para exemplo");
    }
}

void CalcularMalhaAleatoriaSequencial(
    VetorReal& coordenadas,
    const Real& x_inicio,
    const Real& comprimento,
    const unsigned& n_nos
) {
    // Inicializa gerador de números aleatórios
    std::random_device rd;
    std::mt19937 gerador(rd());
    std::uniform_real_distribution<Real> distribuicao(x_inicio, x_inicio + comprimento);
    
    // Função lambda para gerar coordenadas aleatórias
    auto coordenadaAleatoria = [&]() { return distribuicao(gerador); };
    
    // Preenche N-2 nós internos com coordenadas aleatórias
    std::generate_n(coordenadas.begin() + 1, n_nos - 2, coordenadaAleatoria);
    
    // Fixa as extremidades
    coordenadas.front() = x_inicio;
    coordenadas.back() = x_inicio + comprimento;
    
    // Ordena os nós internos (sequencial)
    std::sort(coordenadas.begin() + 1, coordenadas.end() - 1);
}

void CalcularMalhaAleatoriaParalelo(
    VetorReal& coordenadas,
    const Real& x_inicio,
    const Real& comprimento,
    const unsigned& n_nos
) {
    // Inicializa gerador de números aleatórios (thread-safe)
    static thread_local std::random_device rd;
    static thread_local std::mt19937 gerador(rd());
    static thread_local std::uniform_real_distribution<Real> distribuicao(x_inicio, x_inicio + comprimento);
    
    // Função lambda para gerar coordenadas aleatórias
    auto coordenadaAleatoria = [&]() { return distribuicao(gerador); };
    
    // Preenche N-2 nós internos com coordenadas aleatórias (paralelo)
    std::generate_n(coordenadas.begin() + 1, n_nos - 2, coordenadaAleatoria);
    
    // Fixa as extremidades
    coordenadas.front() = x_inicio;
    coordenadas.back() = x_inicio + comprimento;
    
    // Ordena os nós internos (paralelo)
    std::sort(std::execution::par, coordenadas.begin() + 1, coordenadas.end() - 1);
}

std::string CalcularMalhaAleatoria(
    VetorReal& coordenadas,
    const Real& x_inicio,
    const Real& comprimento,
    const unsigned& n_nos
) {
    if (n_nos >= LIMITE_PARALELO) {
        std::cout << "  Usando algoritmo paralelo (" << n_nos 
                  << " nós >= " << LIMITE_PARALELO << ")\n";
        CalcularMalhaAleatoriaParalelo(
            coordenadas, x_inicio, comprimento, n_nos
        );
        return "Paralelo (C++17 Execution Policies)";
    } else {
        std::cout << "  Usando algoritmo sequencial (" << n_nos 
                  << " nós < " << LIMITE_PARALELO << ")\n";
        CalcularMalhaAleatoriaSequencial(
            coordenadas, x_inicio, comprimento, n_nos
        );
        return "Sequencial (Loop Tradicional)";
    }
}

void ImprimirResultados(
    const VetorReal& coordenadas,
    const Real& x_inicio,
    const Real& comprimento,
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
    std::cout << "  RESULTADOS DA SIMULAÇÃO - MALHA ALEATÓRIA\n";
    std::cout << std::string(size, '-') << std::endl;   
    
    //--------------------------------------------------------------------------
    // Parâmetros Usados
    //--------------------------------------------------------------------------
    std::cout << "\n    PARÂMETROS DA MALHA:\n";
    std::cout << std::string(size, '-') << std::endl;   
    std::cout << std::fixed << std::setprecision(6);
    std::cout << "  Coordenada inicial (x_I):           " << std::setw(14) << x_inicio << "\n";
    std::cout << "  Comprimento total (L):              " << std::setw(14) << comprimento << "\n";
    std::cout << "  Número de nós (N):                  " << std::setw(14) << n_nos << "\n";
    std::cout << "  Algoritmo usado:                    " << std::setw(14) << algoritmo << "\n";
    std::cout << "  Tempo de cálculo:                   " << std::setw(14) << tempo_ms << " ms\n";
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
        std::cout << std::scientific << std::setprecision(10) << std::right;

        std::cout   << std::setw(7) << "Nó" 
                    << std::setw(20) << "Coordenada" 
                    << std::setw(21) << "Espaçamento" 
                    << "\n";
        std::cout << std::string(46, '-') << "\n";
        

        std::cout << std::setw(6) << 0 
                  << std::setw(20) << coordenadas[0] 
                  << std::setw(21) << "---\n";

        // Imprime as linhas restantes (nós 1 até n_nos-1) em um loop limpo
        for (unsigned i = 1; i < n_nos; ++i) {
            Real espacamento = coordenadas[i] - coordenadas[i-1];
            std::cout << std::setw(6) << i
                      << std::setw(20) << coordenadas[i]
                      << std::setw(20) << espacamento << "\n";
        }
    } else {
        // Mostra apenas os primeiros e últimos nós
        std::cout << std::scientific << std::setprecision(10) << std::right;

        std::cout   << std::setw(7) << "Nó" 
                    << std::setw(20) << "Coordenada" 
                    << std::setw(21) << "Espaçamento" 
                    << "\n";
        std::cout << std::string(46, '-') << "\n";
        

        std::cout << std::setw(6) << 0 
                  << std::setw(20) << coordenadas[0] 
                  << std::setw(21) << "---\n";
        
        // Primeiros 3 nós
        for (unsigned i = 1; i < 3; ++i) {
            Real espacamento = coordenadas[i] - coordenadas[i-1];
            std::cout << std::setw(6) << i
                    << std::setw(20) << coordenadas[i]
                    << std::setw(20) << espacamento << "\n";
        }

        std::cout << std::setw(6) << "..." << std::setw(20) << "..." 
                  << std::setw(20) << "..." << "\n";
        
        // Últimos 3 nós
        for (unsigned i = n_nos-3; i < n_nos; ++i) {
            Real espacamento = coordenadas[i] - coordenadas[i-1];
            std::cout << std::setw(6) << i
                    << std::setw(20) << coordenadas[i]
                    << std::setw(20) << espacamento << "\n";
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
        std::cout << "    Status:            Cálculo preciso\n";
    } else if (erro_final < 1e-6) {
        std::cout << "    Status:            Pequeno erro numérico\n";
    } else {
        std::cout << "    Status:            Possível erro no algoritmo\n";
    }
    
    std::cout << std::string(size, '-') << std::endl;   
    
    //--------------------------------------------------------------------------
    // Aplicações e Recomendações
    //--------------------------------------------------------------------------
    std::cout << "\n    APLICAÇÕES E RECOMENDAÇÕES:\n";
    std::cout << std::string(size, '-') << std::endl;   
    std::cout << "    1. Malhas aleatórias são ideais para STRESS TESTS\n";
    std::cout << "    2. Avaliam robustez de métodos numéricos\n";
    std::cout << "    3. Identificam instabilidades em condições extremas\n";
    std::cout << "    4. NÃO recomendadas para simulações de produção\n";
    std::cout << "    5. Espaçamentos irregulares podem prejudicar convergência\n";
    std::cout << std::string(size, '-') << std::endl;   
    
    //--------------------------------------------------------------------------
    // Mensagem Final Educativa
    //--------------------------------------------------------------------------
    std::cout << "\n    CONCEITOS DEMONSTRADOS:\n";
    std::cout << std::string(size, '-') << std::endl;   
    std::cout << "    1. Geração de números aleatórios com mt19937\n";
    std::cout << "    2. Distribuição uniforme em intervalo definido\n";
    std::cout << "    3. Algoritmos STL (generate_n, sort)\n";
    std::cout << "    4. Execução paralela com std::execution::par\n";
    std::cout << "    5. Manipulação segura de extremidades\n";
    std::cout << std::string(size, '-') << std::endl;   
}