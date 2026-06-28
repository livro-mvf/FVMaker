// ============================================================================
// Arquivo: Exponencial.cpp
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Implementa Exponencial no contexto de antigos / capitulos / Capitulo_103b / Exponencial.
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

#include <cmath>        // Para funções matemáticas (exp, fabs)
#include <execution>    // Para execução paralela  
#include <iomanip>      // Para formatação de saída
#include <iostream>     // Para entrada/saída padrão
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
constexpr unsigned LIMITE_IMPRESSAO = 25;

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
 * @return Real Fator de normalização D = exp(beta·(N-1)) - 1
 * 
 * @note Implementação robusta para valores extremos de beta
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
 * @param indice Índice do nó (0 <= i <= N-1)
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
void ImprimirResultados(
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
    // Parâmetros da Malha de Exemplo (Caso beta = 1.0)
    //--------------------------------------------------------------------------
    constexpr Real X_INICIO = -1.0;     // Coordenada inicial do domínio
    constexpr Real COMPRIMENTO = 2.0;  // Comprimento total do domínio
    constexpr Real BETA = 1;       // Parâmetro de concentração
    constexpr unsigned N_NOS = 11;     // Número de nós da malha
    
    //--------------------------------------------------------------------------
    // Validação dos Parâmetros
    //--------------------------------------------------------------------------
    try {
        ValidarParametrosExponencial(X_INICIO, COMPRIMENTO, BETA, N_NOS);
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
    ImprimirResultados(
            coordenadas, 
            X_INICIO, 
            COMPRIMENTO, 
            BETA, 
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
    std::cout << "  Programa Exponencial: Geração de Malha Unidimensional Exponencial\n";
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
        throw std::invalid_argument("beta deve ser não-negativo (beta ≥ 0)");
    }
    
    // Valida se os cálculos numéricos são seguros
    if (n_nos > LIMITE_PARALELO * 10) {
        throw std::invalid_argument("Número de nós muito grande para exemplo");
    }
    
    // Valida se beta não causará overflow na exponencial
    if (beta > 100.0 && n_nos > 1000) {
        throw std::invalid_argument(
            "beta muito grande pode causar overflow em exp(beta·(N-1))"
        );
    }
    
    // Caso especial: beta = 0 resulta em malha uniforme (é permitido)
    if (std::abs(beta) < EPSILON) {
        std::cout << "  Nota: beta -> 0, malha será aproximadamente uniforme\n";
    }
}

Real CalcularFatorNormalizacao(
    const Real& beta,
    const unsigned& n_nos
) noexcept {
    // Caso beta = 0: retorna (N-1) para evitar divisão por zero
    if (std::abs(beta) < EPSILON) {
        return static_cast<Real>(n_nos - 1);
    }
    
    // Calcula D = exp(beta·(N-1)) - 1
    Real expoente = beta * static_cast<Real>(n_nos - 1);
    
    // Proteção contra overflow
    if (expoente > 700.0) {  // exp(700) -> 10^304, próximo ao limite do double
        std::cerr << "  ⚠ Aviso: exp(beta·(N-1)) muito grande, possível perda de precisão\n";
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
    // Caso beta = 0: fórmula uniforme x_i = x_I + L·i/(N-1)
    if (std::abs(beta) < EPSILON) {
        Real fator = static_cast<Real>(indice) / static_cast<Real>(fator_norm);
        return x_inicio + comprimento * fator;
    }
    
    // Fórmula exponencial: x_i = x_I + L·(exp(beta·i) - 1)/D
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

void ImprimirResultados(
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
    std::cout << "  Coordenada inicial (x_I):           " << std::setw(14) << x_inicio << "\n";
    std::cout << "  Comprimento total (L):              " << std::setw(14) << comprimento << "\n";
    std::cout << "  Parâmetro de concentração (beta):   " << std::setw(14) << beta << "\n";
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
    // Análise do Comportamento
    //--------------------------------------------------------------------------
    std::cout << "\n    ANÁLISE DO COMPORTAMENTO (beta = " << beta << "):\n";
    std::cout << std::string(size, '-') << std::endl;   
    
    if (beta < 0.1) {
        std::cout << "  - Malha quase uniforme (beta → 0)\n";
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
    std::cout << "    2. Fórmula: x_i = x_I + L·[exp(beta·i) - 1]/[exp(beta·(N-1)) - 1]\n";
    std::cout << "    3. Transição beta → 0 recupera malha uniforme\n";
    std::cout << "    4. beta grande concentra nós em x = x_I\n";
    std::cout << "    5. Robustez numérica para casos extremos\n";
    std::cout << std::string(size, '-') << std::endl;   
}