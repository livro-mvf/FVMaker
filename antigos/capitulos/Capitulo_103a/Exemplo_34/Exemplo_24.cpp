// ============================================================================
// Arquivo: Exemplo_24.cpp
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Implementa Exemplo 24 no contexto de antigos / capitulos / Capitulo_103a / Exemplo_34.
// Autor: João Flávio Vieira de Vasconcellos
//
// SPDX-FileCopyrightText: 2026 João Flávio Vieira de Vasconcellos
// SPDX-License-Identifier: BSD-3-Clause
//
// Este arquivo faz parte do FVMaker.
//
// Licença: BSD 3-Clause.
// É permitido usar, copiar, modificar e redistribuir este arquivo, em código-fonte
// ou forma binária, com ou sem modificações, desde que sejam preservados os avisos
// de copyright, esta identificação de licença e as condições descritas no arquivo
// LICENSE.md.
//
// O nome do autor, de colaboradores ou de instituições associadas ao projeto não
// pode ser usado para endossar ou promover produtos derivados sem autorização
// prévia por escrito.
//
// Este software é fornecido sem garantias de qualquer natureza. Consulte o arquivo
// LICENSE.md, na raiz do repositório, para o texto completo da licença.
// ============================================================================

#include <algorithm>    // Para algoritmos de ordenação e geração
#include <chrono>       // Para medir tempo de execução
#include <execution>    // Para execução paralela  
#include <iomanip>      // Para formatação de saída
#include <iostream>     // Para entrada/saída padrão
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
 * @def LIMITE_PARALELO
 * @brief Limite de nós para ativar ordenação paralela
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
 * @brief Calcula malha aleatória usando ordenação paralela
 * 
 * @param coordenadas Vetor para armazenar coordenadas (pré-alocado)
 * @param x_inicio Coordenada inicial
 * @param comprimento Comprimento total
 * @param n_nos Número de nós
 */
void CalcularMalhaAleatoriaParalela(
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
void ImprimirResultadosAleatorios(
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
    constexpr Real COMPRIMENTO = 1.0;  // Comprimento total do domínio
    constexpr unsigned N_NOS = 11;     // Número de nós da malha
    
    //--------------------------------------------------------------------------
    // Validação dos Parâmetros
    //--------------------------------------------------------------------------
    try {
        ValidarParametrosAleatorios(X_INICIO, COMPRIMENTO, N_NOS);
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
    
    if (erro > 1e-10) {
        std::cerr << "  ⚠ Aviso: Possível erro no cálculo. "
                  << "Diferença: " << erro << "\n";
    }
    
    //--------------------------------------------------------------------------
    // Impressão dos Resultados
    //--------------------------------------------------------------------------
    ImprimirResultadosAleatorios(
        coordenadas, X_INICIO, COMPRIMENTO, N_NOS,
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
    std::cout << "  Exemplo 33: Geração de Malha Unidimensional Aleatória\n";
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
    
    // Valida se os cálculos são seguros
    if (n_nos > 1000000) {
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
    std::mt19937 gerador(std::random_device{}());
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

void CalcularMalhaAleatoriaParalela(
    VetorReal& coordenadas,
    const Real& x_inicio,
    const Real& comprimento,
    const unsigned& n_nos
) {
    // Inicializa gerador de números aleatórios
    std::mt19937 gerador(std::random_device{}());
    std::uniform_real_distribution<Real> distribuicao(x_inicio, x_inicio + comprimento);
    
    // Função lambda para gerar coordenadas aleatórias
    auto coordenadaAleatoria = [&]() { return distribuicao(gerador); };
    
    // Preenche N-2 nós internos com coordenadas aleatórias
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
        std::cout << "  Usando algoritmo com ordenação paralela (" << n_nos 
                  << " nós >= " << LIMITE_PARALELO << ")\n";
        CalcularMalhaAleatoriaParalela(
            coordenadas, x_inicio, comprimento, n_nos
        );
        return "Aleatória com ordenação paralela";
    } else {
        std::cout << "  Usando algoritmo sequencial (" << n_nos 
                  << " nós < " << LIMITE_PARALELO << ")\n";
        CalcularMalhaAleatoriaSequencial(
            coordenadas, x_inicio, comprimento, n_nos
        );
        return "Aleatória sequencial";
    }
}

void ImprimirResultadosAleatorios(
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
    std::cout << "  Coordenada inicial (x_I):     " << std::setw(14) << x_inicio << "\n";
    std::cout << "  Comprimento total (L):        " << std::setw(14) << comprimento << "\n";
    std::cout << "  Número de nós (N):            " << std::setw(14) << n_nos << "\n";
    std::cout << "  Algoritmo usado:              " << std::setw(14) << algoritmo << "\n";
    std::cout << "  Tempo de cálculo:             " << std::setw(14) << tempo_ms << " ms\n";
    std::cout << std::string(size, '-') << std::endl;   
    
    //--------------------------------------------------------------------------
    // Estatísticas da Malha
    //--------------------------------------------------------------------------
    if (n_nos >= 2) {
        // Calcula estatísticas dos espaçamentos
        VetorReal espacamentos;
        espacamentos.reserve(n_nos - 1);
        
        for (unsigned i = 1; i < n_nos; ++i) {
            espacamentos.push_back(coordenadas[i] - coordenadas[i-1]);
        }
        
        auto [min_it, max_it] = std::minmax_element(espacamentos.begin(), espacamentos.end());
        Real menor_espacamento = *min_it;
        Real maior_espacamento = *max_it;
        Real media_espacamentos = comprimento / (n_nos - 1);
        
        std::cout << "\n    ESTATÍSTICAS DOS ESPAÇAMENTOS:\n";
        std::cout << std::string(size, '-') << std::endl;   
        std::cout << std::scientific << std::setprecision(6);
        std::cout << "  Menor espaçamento:         " << std::setw(14) << menor_espacamento << "\n";
        std::cout << "  Maior espaçamento:         " << std::setw(14) << maior_espacamento << "\n";
        std::cout << "  Razão (maior/menor):       " << std::setw(14) 
                  << (menor_espacamento > 1e-12 ? maior_espacamento / menor_espacamento : 0.0) << "\n";
        std::cout << "  Espaçamento médio:         " << std::setw(14) << media_espacamentos << "\n";
        std::cout << "  Desvio da média:           " << std::setw(14) 
                  << std::abs(maior_espacamento - media_espacamentos) / media_espacamentos * 100.0 << " %\n";
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
    
    if (erro_final < 1e-10) {
        std::cout << "    Status:            ✓ Cálculo preciso\n";
    } else if (erro_final < 1e-6) {
        std::cout << "    Status:            ⚠ Pequeno erro numérico\n";
    } else {
        std::cout << "    Status:            ✗ Possível erro no algoritmo\n";
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