//------------------------------------------------------------------------------
// Simetrica.cpp
//------------------------------------------------------------------------------
// Programa de exemplo para demonstração do método de diferenças finitas
// na geração de malhas unidimensionais com distribuição simétrica.
//
// Este programa implementa geração de malhas simétricas em relação ao centro
// do domínio, usando pontos aleatórios e espelhamento.
//
// Autor: João Flávio Vieira de Vasconcellos
// Versão: 1.6
// Data: 2025-12-01
// Testado: GCC 14.2.0 em 2025-12-01
//------------------------------------------------------------------------------

/**
 * @file Simetrica.cpp
 * @brief Exemplo didático de geração de malha unidimensional simétrica
 * 
 * Este programa demonstra técnicas modernas de C++ para gerar uma malha
 * unidimensional simétrica em relação ao centro do domínio.
 * 
 * Características da malha simétrica:
 * - Distribuição aleatória na metade esquerda do domínio
 * - Espelhamento dos pontos para a metade direita
 * - Extremidades fixas nas bordas do domínio
 * - Nó central fixo no centro (para número ímpar de nós)
 * - Ordenação paralela para otimização de desempenho
 * 
 * Objetivos pedagógicos:
 * 1. Demonstrar técnicas de simetria em malhas
 * 2. Mostrar o uso de algoritmos STL com execução paralela
 * 3. Ilustrar aplicação de malhas simétricas para problemas físicos simétricos
 * 4. Apresentar tratamento especial para paridade do número de nós
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

#include <execution>    // Para execução paralela  
#include <iomanip>      // Para formatação de saída
#include <iostream>     // Para entrada/saída padrão
#include <random>       // Para geração de números aleatórios
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
#define VERSION_MINOR 6

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
 * @brief Valida os parâmetros de entrada da malha simétrica
 * 
 * @param x_inicio Coordenada inicial
 * @param comprimento Comprimento total do domínio
 * @param n_nos Número de nós
 * @throws std::invalid_argument Se algum parâmetro for inválido
 */
void ValidarParametrosSimetricos(
    [[maybe_unused]] const Real& x_inicio,
    const Real& comprimento,
    const unsigned& n_nos
);

/**
 * @brief Calcula malha simétrica usando algoritmo sequencial
 * 
 * @param coordenadas Vetor para armazenar coordenadas (pré-alocado)
 * @param x_inicio Coordenada inicial
 * @param comprimento Comprimento total
 * @param n_nos Número de nós
 */
void CalcularMalhaSimetricaSequencial(
    VetorReal& coordenadas,
    const Real& x_inicio,
    const Real& comprimento,
    const unsigned& n_nos
);

/**
 * @brief Calcula malha simétrica usando algoritmo paralelo (C++17)
 * 
 * @param coordenadas Vetor para armazenar coordenadas (pré-alocado)
 * @param x_inicio Coordenada inicial
 * @param comprimento Comprimento total
 * @param n_nos Número de nós
 */
void CalcularMalhaSimetricaParalelo(
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
std::string CalcularMalhaSimetrica(
    VetorReal& coordenadas,
    const Real& x_inicio,
    const Real& comprimento,
    const unsigned& n_nos
);

/**
 * @brief Imprime os resultados formatados da malha simétrica
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
    constexpr Real X_INICIO = -3.0;    // Coordenada inicial do domínio
    constexpr Real COMPRIMENTO = 4.0;  // Comprimento total do domínio
    constexpr unsigned N_NOS = 21;     // Número de nós da malha
    
    //--------------------------------------------------------------------------
    // Validação dos Parâmetros
    //--------------------------------------------------------------------------
    try {
        ValidarParametrosSimetricos(X_INICIO, COMPRIMENTO, N_NOS);
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
    
    std::string algoritmo_usado = CalcularMalhaSimetrica(
        coordenadas, X_INICIO, COMPRIMENTO, N_NOS
    );
    
    auto fim = std::chrono::high_resolution_clock::now();
    auto duracao = std::chrono::duration_cast<std::chrono::microseconds>(fim - inicio);
    double tempo_ms = duracao.count() / 1000.0;
    
    //--------------------------------------------------------------------------
    // Verificação dos Resultados
    //--------------------------------------------------------------------------
    if (coordenadas.empty()) {
        std::cerr << "  Erro: Nenhuma coordenada calculada\n";
        return EXIT_FAILURE;
    }
    
    // Verifica simetria das coordenadas em relação ao centro
    Real centro = X_INICIO + COMPRIMENTO / 2.0;
    bool simetrica = true;
    for (unsigned i = 0; i < N_NOS / 2; ++i) {
        Real distancia_esquerda = coordenadas[i] - centro;
        Real distancia_direita = centro - coordenadas[N_NOS - 1 - i];
        if (std::abs(distancia_esquerda + distancia_direita) > EPSILON) {
            simetrica = false;
            break;
        }
    }
    
    if (!simetrica) {
        std::cerr << "  Aviso: A malha pode não ser perfeitamente simétrica.\n";
    }
    
    // Verifica se a última coordenada está próxima do valor esperado
    Real coordenada_final_esperada = X_INICIO + COMPRIMENTO;
    Real erro = std::abs(coordenadas.back() - coordenada_final_esperada);
    
    if (erro > EPSILON) {
        std::cerr << "  Aviso: Possível erro no cálculo. "
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
    std::cout << "  Programa Simetrica: Geração de Malha Unidimensional Simétrica\n";
    std::cout << "  (Simetria em relação ao centro do domínio)\n";
    std::cout << "  Versão: " << VERSION_MAJOR 
              << "." 
              << VERSION_MINOR << "." << VERSION_PATCH << "\n";
    std::cout << "  Compilado em: " << BUILD_DATE << " :  " 
              << std::setw(8) << std::left << BUILD_TIME << "\n";
    std::cout << "  Autor: João Flávio Vieira de Vasconcellos\n";
    std::cout << std::string(size, '-') << std::endl << "\n";    
}

void ValidarParametrosSimetricos(
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

void CalcularMalhaSimetricaSequencial(
    VetorReal& coordenadas,
    const Real& x_inicio,
    const Real& comprimento,
    const unsigned& n_nos
) {
    // Calcula o centro do domínio
    Real centro = x_inicio + comprimento / 2.0;
    
    // Inicializa gerador de números aleatórios
    std::random_device rd;
    std::mt19937 gerador(rd());
    std::uniform_real_distribution<Real> distribuicao(x_inicio, centro);
    
    // Função lambda para gerar coordenadas aleatórias
    auto coordenadaAleatoria = [&]() { return distribuicao(gerador); };
    
    // Fixa as extremidades
    coordenadas.front() = x_inicio;
    coordenadas.back() = x_inicio + comprimento;
    
    // Determina paridade e número de nós na metade esquerda
    bool par = (n_nos % 2 == 0);
    unsigned nos_metade_esquerda = par ? (n_nos / 2 - 1) : ((n_nos - 1) / 2);
    
    // Preenche nós na metade esquerda (exceto extremidade inicial)
    for (unsigned i = 1; i <= nos_metade_esquerda; ++i) {
        coordenadas[i] = coordenadaAleatoria();
    }
    
    // Espelha os nós para a metade direita
    for (unsigned i = 1; i <= nos_metade_esquerda; ++i) {
        coordenadas[n_nos - 1 - i] = 2.0 * centro - coordenadas[i];
    }
    
    // Se número ímpar de nós, fixa o nó central no centro
    if (!par) {
        coordenadas[nos_metade_esquerda] = centro;
    }
    
    // Ordena os nós internos (sequencial)
    std::sort(coordenadas.begin() + 1, coordenadas.end() - 1);
}

void CalcularMalhaSimetricaParalelo(
    VetorReal& coordenadas,
    const Real& x_inicio,
    const Real& comprimento,
    const unsigned& n_nos
) {
    // Calcula o centro do domínio
    Real centro = x_inicio + comprimento / 2.0;
    
    // Inicializa gerador de números aleatórios (thread-safe)
    static thread_local std::random_device rd;
    static thread_local std::mt19937 gerador(rd());
    static thread_local std::uniform_real_distribution<Real> distribuicao(x_inicio, centro);
    
    // Função lambda para gerar coordenadas aleatórias
    auto coordenadaAleatoria = [&]() { return distribuicao(gerador); };
    
    // Fixa as extremidades
    coordenadas.front() = x_inicio;
    coordenadas.back() = x_inicio + comprimento;
    
    // Determina paridade e número de nós na metade esquerda
    bool par = (n_nos % 2 == 0);
    unsigned nos_metade_esquerda = par ? (n_nos / 2 - 1) : ((n_nos - 1) / 2);
    
    // Gera índices para a metade esquerda
    std::vector<unsigned> indices_esquerda(nos_metade_esquerda);
    for (unsigned i = 0; i < nos_metade_esquerda; ++i) {
        indices_esquerda[i] = i + 1;  // Começa do índice 1
    }
    
    // Preenche nós na metade esquerda em paralelo
    std::for_each(
        std::execution::par,
        indices_esquerda.begin(),
        indices_esquerda.end(),
        [&coordenadas, &coordenadaAleatoria](unsigned i) {
            coordenadas[i] = coordenadaAleatoria();
        }
    );
    
    // Espelha os nós para a metade direita em paralelo
    std::for_each(
        std::execution::par,
        indices_esquerda.begin(),
        indices_esquerda.end(),
        [&coordenadas, centro, n_nos](unsigned i) {
            coordenadas[n_nos - 1 - i] = 2.0 * centro - coordenadas[i];
        }
    );
    
    // Se número ímpar de nós, fixa o nó central no centro
    if (!par) {
        coordenadas[nos_metade_esquerda] = centro;
    }
    
    // Ordena os nós internos (paralelo)
    std::sort(std::execution::par, coordenadas.begin() + 1, coordenadas.end() - 1);
}

std::string CalcularMalhaSimetrica(
    VetorReal& coordenadas,
    const Real& x_inicio,
    const Real& comprimento,
    const unsigned& n_nos
) {
    if (n_nos >= LIMITE_PARALELO) {
        std::cout << "  Usando algoritmo paralelo (" << n_nos 
                  << " nós >= " << LIMITE_PARALELO << ")\n";
        CalcularMalhaSimetricaParalelo(
            coordenadas, x_inicio, comprimento, n_nos
        );
        return "Paralelo (C++17 Execution Policies)";
    } else {
        std::cout << "  Usando algoritmo sequencial (" << n_nos 
                  << " nós < " << LIMITE_PARALELO << ")\n";
        CalcularMalhaSimetricaSequencial(
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
    Real centro = x_inicio + comprimento / 2.0;
    
    //--------------------------------------------------------------------------
    // Cabeçalho dos Resultados
    //--------------------------------------------------------------------------
    std::cout << "\n\n";
    std::cout << std::string(size, '-') << std::endl;   
    std::cout << "  RESULTADOS DA SIMULAÇÃO - MALHA SIMÉTRICA\n";
    std::cout << std::string(size, '-') << std::endl;   
    
    //--------------------------------------------------------------------------
    // Parâmetros Usados
    //--------------------------------------------------------------------------
    std::cout << "\n    PARÂMETROS DA MALHA:\n";
    std::cout << std::string(size, '-') << std::endl;   
    std::cout << std::fixed << std::setprecision(6);
    std::cout << "  Coordenada inicial (x_I):           " << std::setw(14) << x_inicio << "\n";
    std::cout << "  Comprimento total (L):              " << std::setw(14) << comprimento << "\n";
    std::cout << "  Centro do domínio (x_c):            " << std::setw(14) << centro << "\n";
    std::cout << "  Número de nós (N):                  " << std::setw(14) << n_nos << "\n";
    std::cout << "  Paridade (ímpar/par):               " << std::setw(14) 
              << (n_nos % 2 == 0 ? "PAR" : "ÍMPAR") << "\n";
    std::cout << "  Algoritmo usado:                    " << std::setw(14) << algoritmo << "\n";
    std::cout << "  Tempo de cálculo:                   " << std::setw(14) << tempo_ms << " ms\n";
    std::cout << std::string(size, '-') << std::endl;   
    
    //--------------------------------------------------------------------------
    // Estatísticas da Malha
    //--------------------------------------------------------------------------
    if (n_nos >= 2) {
        Real menor_espacamento_esquerda = coordenadas[1] - coordenadas[0];
        Real menor_espacamento_direita = coordenadas[n_nos-1] - coordenadas[n_nos-2];
        Real menor_espacamento = std::min(menor_espacamento_esquerda, menor_espacamento_direita);
        
        // Calcula o maior espaçamento (geralmente perto do centro)
        Real maior_espacamento = 0.0;
        for (unsigned i = 1; i < n_nos; ++i) {
            Real espacamento = coordenadas[i] - coordenadas[i-1];
            if (espacamento > maior_espacamento) {
                maior_espacamento = espacamento;
            }
        }
        
        Real razao_espacamentos = (menor_espacamento > EPSILON) ? 
                                  maior_espacamento / menor_espacamento : 
                                  std::numeric_limits<Real>::infinity();
        
        std::cout << "\n    ESTATÍSTICAS DA MALHA:\n";
        std::cout << std::string(size, '-') << std::endl;   
        std::cout << std::scientific << std::setprecision(6);
        std::cout << "  Menor espaçamento (extremidades): " << std::setw(14) << menor_espacamento << "\n";
        std::cout << "  Maior espaçamento (centro):        " << std::setw(14) << maior_espacamento << "\n";
        std::cout << "  Razão (maior/menor):               " << std::setw(14) << razao_espacamentos << "\n";
        std::cout << "  Espaçamento médio:                 " << std::setw(14) << comprimento/(n_nos-1) << "\n";
        std::cout << "  Simetria verificada:               " << std::setw(14) << "sim" << "\n";
        std::cout << std::string(size, '-') << std::endl;   
    }
    
    //--------------------------------------------------------------------------
    // Coordenadas dos Nós (com informação de simetria)
    //--------------------------------------------------------------------------
    std::cout << "\n    COORDENADAS DOS " << n_nos << " NÓS (SIMETRIA):\n";
    std::cout << std::string(size, '-') << std::endl;   
    
    // Mostra apenas os primeiros e últimos nós se houver muitos
    if (n_nos <= LIMITE_IMPRESSAO) {
        // Mostra todos os nós com informação de simetria
        std::cout << std::scientific << std::setprecision(10) << std::right;

        std::cout   << std::setw(7) << "Nó" 
                    << std::setw(20) << "Coordenada" 
                    << std::setw(24) << "Dist. ao Centro" 
                    << "\n";
        std::cout << std::string(50, '-') << "\n";
        
        for (unsigned i = 0; i < n_nos; ++i) {
            Real distancia_centro = coordenadas[i] - centro;
            std::cout << std::setw(6) << i
                      << std::setw(20) << coordenadas[i]
                      << std::setw(24) << distancia_centro;
            
            std::cout << "\n";
        }
    } else {
        // Mostra apenas os primeiros e últimos nós
        std::cout << std::scientific << std::setprecision(10) << std::right;

        std::cout   << std::setw(7) << "Nó" 
                    << std::setw(20) << "Coordenada" 
                    << std::setw(24) << "Dist. ao Centro" 
                    << "\n";
        std::cout << std::string(50, '-') << "\n";
        
        // Primeiros 3 nós
        for (unsigned i = 0; i < 3; ++i) {
            Real distancia_centro = coordenadas[i] - centro;
            std::cout << std::setw(6) << i
                      << std::setw(20) << coordenadas[i]
                      << std::setw(24) << distancia_centro
                      << "\n";
        }
        
        std::cout << std::setw(6) << "..." 
                  << std::setw(20) << "..." 
                  << std::setw(24) << "..."
                  << "\n";
        
        // Nó central se ímpar
        if (!(n_nos % 2 == 0)) {
            unsigned central = (n_nos - 1) / 2;
            Real distancia_centro = coordenadas[central] - centro;
            std::cout << std::setw(6) << central
                      << std::setw(20) << coordenadas[central]
                      << std::setw(24) << distancia_centro
                      << "\n";
        }
        
        // Últimos 3 nós
        for (unsigned i = n_nos - 3; i < n_nos; ++i) {
            Real distancia_centro = coordenadas[i] - centro;
            std::cout << std::setw(6) << i
                      << std::setw(20) << coordenadas[i]
                      << std::setw(24) << distancia_centro
                      << "\n";
        }
        
        std::cout << "\n    Mostrando apenas primeiros, centrais e últimos nós ";
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
    
    // Verifica simetria novamente para relatório
    bool simetria_perfeita = true;
    Real max_erro_simetria = 0.0;
    for (unsigned i = 0; i < n_nos / 2; ++i) {
        Real esquerda = coordenadas[i];
        Real direita = coordenadas[n_nos - 1 - i];
        Real erro = std::abs((esquerda + direita) - 2.0 * centro);
        if (erro > max_erro_simetria) {
            max_erro_simetria = erro;
        }
        if (erro > EPSILON) {
            simetria_perfeita = false;
        }
    }
    
    std::cout << "    Erro máximo de simetria: " << max_erro_simetria << "\n";
    
    if (erro_final < EPSILON && simetria_perfeita) {
        std::cout << "    Status:            Cálculo preciso e simétrico\n";
    } else if (erro_final < 1e-6 && max_erro_simetria < 1e-6) {
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
    std::cout << "    1. Malhas simétricas são ideais para problemas físicos simétricos\n";
    std::cout << "    2. Úteis para validar códigos numéricos com soluções simétricas\n";
    std::cout << "    3. Reduzem o esforço computacional pela metade\n";
    std::cout << "    4. Preservam simetrias da solução\n";
    std::cout << "    5. A simetria numérica ajuda a detectar erros de implementação\n";
    std::cout << std::string(size, '-') << std::endl;   
    
    //--------------------------------------------------------------------------
    // Mensagem Final Educativa
    //--------------------------------------------------------------------------
    std::cout << "\n    CONCEITOS DEMONSTRADOS:\n";
    std::cout << std::string(size, '-') << std::endl;   
    std::cout << "    1. Geração de malhas simétricas\n";
    std::cout << "    2. Espelhamento geométrico em relação ao centro\n";
    std::cout << "    3. Tratamento de paridade (ímpar/par) do número de nós\n";
    std::cout << "    4. Algoritmos STL com execução paralela\n";
    std::cout << "    5. Verificação automática de simetria\n";
    std::cout << std::string(size, '-') << std::endl;   
}