//==============================================================================
// Nome        : Programa_22.cpp
// Autor       : Joao Flavio Vieira de Vasconcellos
// Versao      : 2.3
// Descricao   : Programa para geracao de malha aleatoria unidimensional
//               (versao serial/paralela com STL e lambdas separadas)
// Livro       : Codigo 2.2 do livro:
//               "Solucao Numerica de Equacoes Diferenciais - O Metodo de Volumes Finitos"
// Testado     : gcc 13.2 usando c++23 em 2024-06-20
//==============================================================================

/**
 * @file Programa_22.cpp
 * @brief Programa para geracao de malha aleatoria unidimensional
 * 
 * Versao com implementacoes serial e paralela usando STL,
 * com lambdas separadas das operacoes STL e geracao aleatoria melhorada.
 * 
 * @version 2.3
 * @date 2024-06-20
 * @author Joao Flavio Vasconcellos
 * 
 * @copyright Copyright (c) 2024
 * @license GNU GPL v3
 */

//==============================================================================
//      Inclusoes da Biblioteca Padrao do C++
//==============================================================================

#include <execution>       // Para std::execution::par
#include <functional>      // Para std::function<Real()>
#include <iomanip>         // Para std::setw, std::setprecision
#include <iostream>        // Para std::cout, std::endl
#include <random>          // Para std::random_device, std::mt19937
#include <vector>          // Para std::vector

//==============================================================================
//      Definicoes de tipos
//==============================================================================

using Real = double;                   // Tipo para numeros reais
using VetorReal = std::vector<Real>;   // Vetor de numeros reais

//==============================================================================
//      Prototipos das funcoes
//==============================================================================

auto CriarGeradorAleatorio(const Real&, const Real&) -> std::function<Real()>;
void GerarMalhaSerial(VetorReal&, const Real&, const Real&);
void GerarMalhaParalela(VetorReal&, const Real&, const Real&);
void ImprimirResultados(const VetorReal&, const Real&, const Real&, const unsigned&);

//==============================================================================
//      Funcao principal
//==============================================================================

int main() {
//==============================================================================
//      Parametros da malha
//==============================================================================

    constexpr Real XINIT = 0;                   // Coordenada inicial da malha
    constexpr Real LENGTH = 4;                  // Comprimento da malha
    constexpr unsigned NNOS = 11;               // Numero de nos
    constexpr unsigned NNOSSERIAL = 1000;       // Limite para usar versao serial

    if (NNOS < 2) {
        std::cerr << "Erro: Numero de nos deve ser >= 2\n";
        return EXIT_FAILURE;
    }

    VetorReal xNo(NNOS);

//==============================================================================
//      Selecao automatica da versao (serial/paralela)
//==============================================================================

    if (NNOS < NNOSSERIAL) {
        GerarMalhaSerial(xNo, XINIT, LENGTH);
    } else {
        GerarMalhaParalela(xNo, XINIT, LENGTH);
    }

    ImprimirResultados(xNo, LENGTH, XINIT, NNOS);
    
    return EXIT_SUCCESS;
}

//==============================================================================
//      Implementacao das funcoes
//==============================================================================

/**
 * @brief Cria gerador aleatorio com mt19937 e random_device
 * 
 * @param _xInit Coordenada inicial do dominio
 * @param _length Comprimento total do dominio
 * @return std::function<Real()> Funcao geradora de numeros aleatorios
 */
auto CriarGeradorAleatorio (    const Real& _xInit, 
                                const Real& _length) -> std::function<Real()> {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<Real> dist(_xInit, _xInit + _length);

    return [gen = std::move(gen), dist = std::move(dist)]() mutable {
        return dist(gen);
    };
}

/**
 * @brief Versao serial da geracao de malha
 * 
 * @param _xNo Vetor para armazenar as coordenadas dos nos
 * @param _xInit Coordenada inicial do dominio
 * @param _length Comprimento total do dominio
 */
void GerarMalhaSerial   (   VetorReal&      _xNo, 
                            const Real&     _xInit, 
                            const Real&     _length) {
    
    auto aleatorio = CriarGeradorAleatorio(_xInit, _length);
    
    auto PreencherAleatorio = [&](auto& elemento) {
        elemento = aleatorio();
    };

    std::for_each   (   std::next(_xNo.begin()), 
                        std::prev(_xNo.end()), 
                        PreencherAleatorio);
    
    _xNo.front() = _xInit;
    _xNo.back() = _xInit + _length;
    
    std::sort(std::next(    _xNo.begin()), 
                            std::prev(_xNo.end()));
}

/**
 * @brief Versao paralela da geracao de malha
 * 
 * @param _xNo Vetor para armazenar as coordenadas dos nos
 * @param _xInit Coordenada inicial do dominio
 * @param _length Comprimento total do dominio
 */
void GerarMalhaParalela (   VetorReal&      _xNo, 
                            const Real&     _xInit, 
                            const Real&     _length) {

    // Cria um gerador por thread para thread-safety
    thread_local auto aleatorio = CriarGeradorAleatorio(_xInit, _length);
    
    auto PreencherAleatorio = [](auto& elemento) {
        elemento = aleatorio();
    };

    
    std::for_each(std::execution::par,
                 std::next(_xNo.begin()),
                 std::prev(_xNo.end()),
                 PreencherAleatorio);
    
    _xNo.front() = _xInit;
    _xNo.back() = _xInit + _length;

    std::sort(  std::execution::par, 
                std::next(_xNo.begin()),
                std::prev(_xNo.end()));
}

/**
 * @brief Imprime os resultados formatados
 * 
 * @param _xNo Vetor com as coordenadas dos nos
 * @param _length Comprimento total do dominio
 * @param _xInit Coordenada inicial do dominio
 * @param _nNos Numero total de nos
 */
void ImprimirResultados(    const VetorReal&    _xNo,
                            const Real& _length,
                            const Real& _xInit,
                            const unsigned& _nNos) {
   
    constexpr unsigned LSIZE = 25;
    constexpr unsigned PRECISION = 6;
    
    std::cout << "\nDADOS DA MALHA ALEATORIA\n";
    std::cout << std::string(LSIZE + 4, '=') << "\n";
    std::cout << std::fixed << std::setprecision(3);
    std::cout << "Comprimento total: " << std::setw(10) << _length << "\n";
    std::cout << "Coordenada inicial:" << std::setw(10) << _xInit << "\n";
    std::cout << "Numero de nos:     " << std::setw(10) << _nNos << "\n";
    std::cout << std::string(LSIZE + 4, '=') << "\n\n";

    std::cout << "COORDENADAS DOS NOS\n";
    std::cout << std::string(LSIZE, '=') << "\n";
    std::cout << std::setw(5) << "No" << std::setw(20) << "Posicao" << "\n";
    std::cout << std::string(LSIZE, '=') << "\n";

    auto ImprimirNo = [PRECISION](unsigned i, Real valor) {
        std::cout << std::setw(5) << i 
                  << std::scientific << std::setprecision(PRECISION)
                  << std::setw(20) << valor << "\n";
    };

    int contador(1);
    for (auto no : _xNo) {
        ImprimirNo(contador++, no);
    }

    std::cout << std::string(LSIZE, '=') << "\n\n";
}