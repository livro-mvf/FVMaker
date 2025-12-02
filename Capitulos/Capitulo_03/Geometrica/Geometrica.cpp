//------------------------------------------------------------------------------
// Exemplo_32.cpp
//------------------------------------------------------------------------------
// Programa de exemplo para demonstração do método de diferenças finitas
// na geração de malhas unidimensionais com progressão geométrica.
//
// Este programa é uma versão simplificada e didática que mostra os conceitos
// fundamentais da geração de malhas sem dependências externas complexas.
//
// Autor: João Flávio Vieira de Vasconcellos
// Versão: 1.0.0
// Data: 2025-12-01
// Testado: GCC 14.2.0 em 2025-12-01
//------------------------------------------------------------------------------

/**
 * @file Exemplo_32.cpp
 * @brief Exemplo didático de geração de malha unidimensional
 * 
 * Este programa demonstra o uso do método de diferenças finitas para
 * gerar uma malha unidimensional com espaçamento variável segundo uma
 * progressão geométrica.
 * 
 * Objetivos pedagógicos:
 * 1. Demonstrar o conceito de progressão geométrica em malhas
 * 2. Mostrar a implementação de algoritmos sequenciais e paralelos
 * 3. Ilustrar boas práticas de documentação em C++
 * 4. Apresentar técnicas de validação de entrada
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

#include <chrono>       // Para medir tempo de execução
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

//------------------------------------------------------------------------------
// Seção 3: Protótipos de Funções
//------------------------------------------------------------------------------

/**
 * @brief Imprime informações da versão do programa
 */
void ImprimirVersao();

/**
 * @brief Calcula potência inteira de forma eficiente
 * 
 * @param base Número base
 * @param expoente Expoente inteiro não negativo
 * @return Real base^expoente
 */
 [[nodiscard]] Real PotenciaInteira(const Real& base, const int& expoente) noexcept;

/**
 * @brief Valida os parâmetros de entrada da malha
 * 
 * @param x_inicio Coordenada inicial
 * @param comprimento Comprimento total do domínio
 * @param razao Razão da progressão geométrica
 * @param n_nos Número de nós
 * @throws std::invalid_argument Se algum parâmetro for inválido
 */
void ValidarParametros  (   [[maybe_unused]] const Real&    x_inicio
                        ,   const Real&                     comprimento
                        ,   const Real&                     razao
                        ,   const unsigned&                 n_nos
                        );

/**
 * @brief Calcula malha usando algoritmo sequencial
 * 
 * @param coordenadas Vetor para armazenar coordenadas (pré-alocado)
 * @param x_inicio Coordenada inicial
 * @param comprimento Comprimento total
 * @param razao Razão da progressão
 * @param n_nos Número de nós
 */
void CalcularMalhaSequencial    (   VetorReal& coordenadas
                                ,   const Real& x_inicio
                                ,   const Real& comprimento
                                ,   const Real& razao
                                ,   const unsigned& n_nos
                               );


/**
 * @brief Calcula malha usando algoritmo paralelo (C++17)
 * 
 * @param coordenadas Vetor para armazenar coordenadas (pré-alocado)
 * @param x_inicio Coordenada inicial
 * @param comprimento Comprimento total
 * @param razao Razão da progressão
 * @param n_nos Número de nós
 */
void CalcularMalhaParalelo  (   VetorReal& coordenadas
                            ,   const Real& x_inicio
                            ,   const Real& comprimento
                            ,   const Real& razao
                            ,   const unsigned& n_nos
                            );

/**
 * @brief Seleciona automaticamente o melhor algoritmo
 * 
 * @param coordenadas Vetor para armazenar coordenadas
 * @param x_inicio Coordenada inicial
 * @param comprimento Comprimento total
 * @param razao Razão da progressão
 * @param n_nos Número de nós
 * @return std::string Nome do algoritmo usado
 */
std::string CalcularMalhas  (   VetorReal& coordenadas,
                                const Real& x_inicio,
                                const Real& comprimento,
                                const Real& razao,
                                const unsigned& n_nos
                            );

/**
 * @brief Imprime os resultados formatados
 * 
 * @param coordenadas Vetor de coordenadas calculadas
 * @param x_inicio Coordenada inicial
 * @param comprimento Comprimento total
 * @param razao Razão da progressão
 * @param n_nos Número de nós
 * @param algoritmo Nome do algoritmo usado
 * @param tempo_ms Tempo de execução em milissegundos
 */
void ImprimirResultados (   const VetorReal& coordenadas
                        ,   const Real& x_inicio
                        ,    const Real& comprimento
                        ,    const Real& razao
                        ,    const unsigned& n_nos
                        ,    const std::string& algoritmo
                        ,    const Real& tempo_ms
                        ); 

//------------------------------------------------------------------------------
// Seção 4: Função Principal
//------------------------------------------------------------------------------

int main() {

//------------------------------------------------------------------------------
// Inicio da execução
//------------------------------------------------------------------------------
    ImprimirVersao();
    
//------------------------------------------------------------------------------
// Parâmetros da Malha de Exemplo
//------------------------------------------------------------------------------

constexpr Real X_INICIO = 0.0;     // Coordenada inicial do domínio
constexpr Real COMPRIMENTO = 4.0;  // Comprimento total do domínio
constexpr Real RAZAO = 1.05;       // Razão da progressão geométrica
constexpr unsigned N_NOS = 11;     // Número de nós da malha
    
//------------------------------------------------------------------------------
// Validação dos Parâmetros
//------------------------------------------------------------------------------
    
    try {
        ValidarParametros(X_INICIO, COMPRIMENTO, RAZAO, N_NOS);
        std::cout << "  Parâmetros validados com sucesso\n";
    } catch (const std::exception& e) {
        std::cerr << "  Erro na validação: " << e.what() << "\n";
        return EXIT_FAILURE;
    }
    
//------------------------------------------------------------------------------
// Preparação do Vetor de Coordenadas
//------------------------------------------------------------------------------
VetorReal coordenadas(N_NOS);
    
//------------------------------------------------------------------------------
// Cálculo da Malha com Medição de Tempo
//------------------------------------------------------------------------------
auto inicio = std::chrono::high_resolution_clock::now();
    
std::string algoritmo_usado = CalcularMalhas(coordenadas,
                                                           X_INICIO,
                                                           COMPRIMENTO,
                                                           RAZAO,
                                                           N_NOS);
    
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
        std::cerr << "  Aviso: Possível erro no cálculo. "
                  << "Diferença: " << erro << "\n";
    }
    
    //--------------------------------------------------------------------------
    // Impressão dos Resultados
    //--------------------------------------------------------------------------
    ImprimirResultados  (   coordenadas
                        ,   X_INICIO
                        ,   COMPRIMENTO
                        ,   RAZAO
                        ,   N_NOS
                        ,   algoritmo_usado
                        ,   tempo_ms
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
    std::cout << "  Exemplo 32: Geração de Malha Unidimensional para Diferenças Finitas\n";
    std::cout << "  Versão: " << VERSION_MAJOR 
              << "." 
              << VERSION_MINOR << "." << VERSION_PATCH << "\n";
    std::cout << "  Compilado em: " << BUILD_DATE << " :  " 
              << std::setw(8) << std::left << BUILD_TIME << "\n";
    std::cout << "  Autor: João Flávio Vieira de Vasconcellos\n";
    std::cout << std::string(size, '-') << std::endl << "\n";    
}

Real PotenciaInteira    (   const Real& _base
                        ,   const int& _expoente
                        )   noexcept {

    // Caso base: expoente negativo ou zero
    if (_expoente < 0) return 0.0;
    if (_expoente == 0) return 1.0;
    if (_base == 0.0) return 0.0;
    
    // Algoritmo de exponenciação binária (eficiente)
    Real resultado = 1.0;
    Real base(_base);
    int  expoente(_expoente);


    while (expoente > 0) {
        // Se o expoente é ímpar, multiplica pelo fator atual
        if (expoente % 2 == 1) {
            resultado *= base;
        }
        
        // Eleva a base ao quadrado
        base *= base;
        
        // Divide o expoente por 2
        expoente /= 2;
    }
    
    return resultado;
}

void ValidarParametros  (   [[maybe_unused]] const Real&        x_inicio
                        ,   const Real&                         comprimento
                        ,   const Real&                         razao
                        ,   const unsigned&                     n_nos
                        ) {
    
    // Valida número de nós
    if (n_nos < 2) {
        throw std::invalid_argument("Número de nós deve ser pelo menos 2");
    }
    
    // Valida comprimento
    if (comprimento <= 0.0) {
        throw std::invalid_argument("Comprimento deve ser positivo");
    }
    
    // Valida razão
    if (razao <= 0.0) {
        throw std::invalid_argument("Razão deve ser positiva");
    }
    
    if (razao == 1.0) {
        throw std::invalid_argument("Razão não pode ser 1.0 (malha uniforme)");
    }    
    
    // Valida se os cálculos numéricos são seguros
    if (n_nos > 1000000) {
        throw std::invalid_argument("Número de nós muito grande para exemplo");
    }
    
    // Valida se a razão não causará overflow
    if (razao > 1000.0 && n_nos > 100) {
        throw std::invalid_argument("Razão muito grande pode causar overflow");
    }

}

void CalcularMalhaSequencial    (   VetorReal& coordenadas
                                ,   const Real& x_inicio
                                ,   const Real& comprimento
                                ,   const Real& razao
                                ,   const unsigned& n_nos
                                ) {

    // Passo 1: Calcula o incremento inicial (delta)
    // Fórmula: Δ = L * (r - 1) / (r^(N-1) - 1)
    Real delta = comprimento * (razao - 1.0) / 
                 (PotenciaInteira(razao, n_nos - 1) - 1.0);
    
    // Passo 2: Gera as coordenadas sequencialmente
    Real x_atual = x_inicio;
    
    for (unsigned i = 0; i < n_nos; ++i) {
        coordenadas[i] = x_atual;
        x_atual += delta;
        delta *= razao;  // Progressão geométrica
    }
}

void CalcularMalhaParalela      (   VetorReal& coordenadas
                                ,   const Real& x_inicio
                                ,   const Real& comprimento
                                ,   const Real& razao
                                ,   const unsigned& n_nos
                                ) {
    // Passo 1: Cria vetor de incrementos individuais
    VetorReal incrementos(n_nos);
    incrementos[0] = 0.0;  // Primeiro nó não tem incremento
    
    // Calcula o incremento inicial
    Real delta = comprimento * (razao - 1.0) / 
                 (PotenciaInteira(razao, n_nos - 1) - 1.0);
    
    // Preenche os incrementos seguindo a progressão geométrica
    for (unsigned i = 1; i < n_nos; ++i) {
        incrementos[i] = delta;
        delta *= razao;
    }
    
    // Passo 2: Soma cumulativa paralela dos incrementos
    std::inclusive_scan(std::execution::par,
                       incrementos.begin(),
                       incrementos.end(),
                       incrementos.begin());
    
    // Passo 3: Soma a coordenada inicial a todos os elementos
    std::transform(std::execution::par,
                  incrementos.begin(),
                  incrementos.end(),
                  coordenadas.begin(),
                  [x_inicio](Real inc) { return x_inicio + inc; });
}

std::string CalcularMalhas  (    VetorReal&  coordenadas
                            ,    const Real& x_inicio
                            ,    const Real& comprimento
                            ,    const Real& razao
                            ,    const unsigned& n_nos
                            ) {
    // Limite para usar paralelização (ajustável)
    constexpr unsigned LIMITE_PARALELO = 10000;
    
    if (n_nos >= LIMITE_PARALELO) {
        std::cout << "  Usando algoritmo paralelo (" << n_nos 
                  << " nós >= " << LIMITE_PARALELO << ")\n";
        CalcularMalhaParalela(coordenadas, x_inicio, comprimento, razao, n_nos);
        return "Paralelo (C++17 Execution Policies)";
    } else {
        std::cout << "  Usando algoritmo sequencial (" << n_nos 
                  << " nós < " << LIMITE_PARALELO << ")\n";
        CalcularMalhaSequencial(coordenadas, x_inicio, comprimento, razao, n_nos);
        return "Sequencial (Loop Tradicional)";
    }
}

void ImprimirResultados (   const VetorReal& coordenadas
                        ,   const Real& x_inicio
                        ,    const Real& comprimento
                        ,    const Real& razao
                        ,    const unsigned& n_nos
                        ,    const std::string& algoritmo
                        ,    const Real& tempo_ms
                        ) {
    //--------------------------------------------------------------------------
    // Cabeçalho dos Resultados
    //--------------------------------------------------------------------------

 const int size(80);   
    std::cout << "\n\n";
    std::cout << std::string(size, '-') << std::endl;   
    std::cout << "  RESULTADOS DA SIMULAÇÃO\n";
    std::cout << std::string(size, '-') << std::endl;   
    
    //--------------------------------------------------------------------------
    // Parâmetros Usados
    //--------------------------------------------------------------------------
    std::cout << "\n    PARÂMETROS DA MALHA:\n";
    std::cout << std::string(size, '-') << std::endl;   
    std::cout << std::fixed << std::setprecision(3);
    std::cout << "  Coordenada inicial (xInicio): " << std::setw(10) << x_inicio << "\n";
    std::cout << "  Comprimento total (L):        " << std::setw(10) << comprimento << "\n";
    std::cout << "  Razão geométrica (r):         " << std::setw(10) << razao << "\n";
    std::cout << "  Número de nós (N):            " << std::setw(10) << n_nos << "\n";
    std::cout << "  Algoritmo usado:              " << std::setw(10) << algoritmo << "\n";
    std::cout << "  Tempo de cálculo:             " << std::setw(10) << tempo_ms << " ms\n";
    std::cout << std::string(size, '-') << std::endl;   
    
    //--------------------------------------------------------------------------
    // Estatísticas da Malha
    //--------------------------------------------------------------------------
    if (n_nos >= 2) {
        Real menor_espacamento = coordenadas[1] - coordenadas[0];
        Real maior_espacamento = coordenadas[n_nos-1] - coordenadas[n_nos-2];
        Real razao_espacamentos = maior_espacamento / menor_espacamento;
        
        std::cout << "\n    ESTATÍSTICAS DA MALHA:\n";
        std::cout << std::string(size, '-') << std::endl;   
        std::cout << std::scientific << std::setprecision(4);
        std::cout << "  Menor espaçamento:    " << std::setw(12) << menor_espacamento << "\n";
        std::cout << "  Maior espaçamento:    " << std::setw(12) << maior_espacamento << "\n";
        std::cout << "  Razão (maior/menor):  " << std::setw(12) << razao_espacamentos << "\n";
        std::cout << "  Esperado (r^(N-2)):   " << std::setw(12) 
                  << PotenciaInteira(razao, n_nos-2) << "\n";
        std::cout << std::string(size, '-') << std::endl;   
    }
    
    //--------------------------------------------------------------------------
    // Coordenadas dos Nós (limitado para não poluir a saída)
    //--------------------------------------------------------------------------
    std::cout << "\n    COORDENADAS DOS " << n_nos << " NÓS:\n";
    std::cout << std::string(size, '-') << std::endl;   
    
    // Mostra apenas os primeiros e últimos 3 nós se houver muitos
    const unsigned LIMITE_IMPRESSAO = 25;
    
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
    
    if (erro_final < 1e-10) {
        std::cout << "    Status:            Cálculo preciso\n";
    } else if (erro_final < 1e-6) {
        std::cout << "    Status:            Pequeno erro numérico\n";
    } else {
        std::cout << "    Status:            Possível erro no algoritmo\n";
    }
    
    std::cout << std::string(size, '-') << std::endl;   
    
    //--------------------------------------------------------------------------
    // Mensagem Final Educativa
    //--------------------------------------------------------------------------
    std::cout << "\n    CONCEITOS DEMONSTRADOS:\n";
    std::cout << std::string(size, '-') << std::endl;   
    std::cout << "    1. Progressão geométrica em malhas\n";
    std::cout << "    2. Método de diferenças finitas\n";
    std::cout << "    3. Seleção automática de algoritmos\n";
    std::cout << "    4. Paralelização com C++17\n";
    std::cout << "    5. Validação de resultados\n";
    std::cout << std::string(size, '-') << std::endl;   

}