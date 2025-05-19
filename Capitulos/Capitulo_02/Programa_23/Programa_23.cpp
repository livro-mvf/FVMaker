//==============================================================================
// Nome        : Programa_23.cpp
// Autor       : Joao Flavio Vieira de Vasconcellos
// Versao      : 1.6
// Descricao   : Programa para geracao de malha simetrica unidimensional
//               com distribuicao uniforme (versao serial/paralela)
// Livro       : Exercicio Computacional 2.3 do livro:
//               "Solucao Numerica de Equacoes Diferenciais - O Metodo de Volumes Finitos"
// Testado     : gcc 13.2 usando c++23 em 2024-06-20
//==============================================================================

/**
 * @file Programa_23.cpp
 * @brief Programa para geracao de malha simetrica unidimensional com distribuicao uniforme
 * 
 * Versao com implementacoes serial e paralela usando STL,
 * com geracao aleatoria simetrica em relacao ao centro do dominio usando distribuicao uniforme.
 * 
 * @version 1.6
 * @date 2024-06-20
 * @author Joao Flavio Vasconcellos
 * 
 * @copyright Copyright (c) 2024
 * @license GNU GPL v3
 */

//==============================================================================
//      Inclusoes da Biblioteca Padrao do C++
//==============================================================================

#include <algorithm>       // Para std::for_each, std::sort, std::generate_n
#include <execution>       // Para std::execution::par
#include <functional>      // Para std::function<Real()>
#include <iomanip>         // Para std::setw, std::setprecision
#include <iostream>        // Para std::cout, std::endl
#include <random>          // Para std::random_device, std::mt19937
#include <ranges>          // Para std::views::iota 
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
    
    constexpr Real XINIT = -3;                  // Coordenada inicial da malha
    constexpr Real LENGTH = 4;                 // Comprimento da malha
    constexpr unsigned NNOS = 11;               // Numero de nos
    constexpr unsigned NNOSSERIAL = 1000;      // Limite para usar versao serial

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

    ImprimirResultados(xNo, XINIT, LENGTH, NNOS);
    
    return EXIT_SUCCESS;
}

//==============================================================================
//      Implementacao das funcoes
//==============================================================================

/**
 * @brief Cria gerador aleatorio com mt19937 e random_device
 * 
 * @param _xInit Coordenada inicial do dominio
 * @param _xMax Coordenada maxima do dominio
 * @return std::function<Real()> Funcao geradora de numeros aleatorios
 */
auto CriarGeradorAleatorio(const Real& _xInit, const Real& _xMax) -> std::function<Real()> {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<Real> dist(_xInit, _xMax);

    return [gen = std::move(gen), dist = std::move(dist)]() mutable {
        return dist(gen);
    };
}

/**
 * @brief Versao serial da geracao de malha simetrica
 * 
 * @param _xNo Vetor para armazenar as coordenadas dos nos
 * @param _xInit Coordenada inicial do dominio
 * @param _length Comprimento total do dominio
 */
void GerarMalhaSerial   (   VetorReal&      _xNo, 
                            const Real&     _xInit, 
                            const Real& _length) {
    
    const Real xCenter = _xInit + _length / 2.0;
    auto aleatorio = CriarGeradorAleatorio(_xInit, _xInit + _length); // Corrigido o intervalo
    
    _xNo.front() = _xInit;
    _xNo.back() = _xInit + _length;

    const int totalNodes = _xNo.size();
    const bool isEven = (totalNodes % 2 == 0);
    const int halfNodes = isEven ? (totalNodes / 2 - 1) : ((totalNodes - 1) / 2);

    auto PreencherAleatorio = [&](auto& elemento) {
        elemento = aleatorio();
    };
    
    std::for_each(std::next(_xNo.begin()), 
                 std::next(_xNo.begin(), halfNodes + 1),
                 PreencherAleatorio);

    for (int i = 1; i <= halfNodes; ++i) {  
        _xNo[totalNodes - 1 - i] = 2 * xCenter - _xNo[i]; 
    }

    if (!isEven) {
        _xNo[halfNodes] = xCenter;
    }
    
    std::sort(  std::next(_xNo.begin()), 
                std::prev(_xNo.end()));
}


/**
 * @brief Versao paralela da geracao de malha simetrica
 * 
 * @param _xNo Vetor para armazenar as coordenadas dos nos
 * @param _xInit Coordenada inicial do dominio
 * @param _length Comprimento total do dominio
 */
void GerarMalhaParalela (   VetorReal&      _xNo, 
                            const Real&     _xInit, 
                            const Real&     _length) {
 
    const Real xCenter = _xInit + _length / 2.0;
    auto aleatorio = CriarGeradorAleatorio(_xInit, xCenter);

    _xNo.front() = _xInit;
    _xNo.back() = _xInit + _length;

    const int totalNodes = _xNo.size();
    const bool isEven   = (totalNodes % 2 == 0);
    const int halfNodes = isEven ? (totalNodes / 2 - 1) : ((totalNodes - 1) / 2);

    auto first = std::next(_xNo.begin());
    auto last = std::next(_xNo.begin(), halfNodes + 1);

    auto PreencherAleatorio = [&](auto& elemento) {
        elemento = aleatorio();
    };
    
    std::for_each   (   std::execution::par, 
                        first, 
                        last, 
                        PreencherAleatorio);

    const Real dx = _xNo.back() + _xInit;
    
    auto indices = std::views::iota(1, halfNodes + (isEven ? 1 : 0));
    
    auto CriarSimetria = [&](int i) {
        _xNo[totalNodes - 1 - i] = _xNo.back() + _xInit - _xNo[i];
    };

    
    std::for_each   (   std::execution::par, 
                        indices.begin(), 
                        indices.end(),
                        CriarSimetria);

    if (!isEven) {
        _xNo[halfNodes] = xCenter;
    }

    std::sort   (   std::execution::par, 
                    std::next(_xNo.begin()), 
                    std::prev(_xNo.end()));
    
}


/**
 * @brief Imprime os resultados formatados
 * 
 * @param _xNo Vetor com as coordenadas dos nos
 * @param _xInit Coordenada inicial do dominio
 * @param _length Comprimento total do dominio
 * @param _nNos Numero total de nos
 */
void ImprimirResultados(const VetorReal& _xNo,
                       const Real& _xInit,
                       const Real& _length,
                       const unsigned& _nNos) {
    constexpr unsigned LSIZE = 45;
    constexpr unsigned PRECISION = 6;
    const Real xCenter = _xInit + _length / 2.0;
    
    std::cout << "\nDADOS DA MALHA SIMETRICA\n";
    std::cout << std::string(LSIZE + 4, '=') << "\n";
    std::cout << std::fixed << std::setprecision(3);
    std::cout << "Comprimento total:      " << std::setw(10) << _length << "\n";
    std::cout << "Coordenada inicial:     " << std::setw(10) << _xInit << "\n";
    std::cout << "Numero de nos:          " << std::setw(10) << _nNos << "\n";
    std::cout << std::string(LSIZE + 4, '=') << "\n\n";

    std::cout << "COORDENADAS DOS NOS\n";
    std::cout << std::string(LSIZE, '=') << "\n";
    std::cout << std::setw(5) << "No" 
              << std::setw(20) << "xNo"    
              << std::setw(20) << "xCenter - xNo" << "\n";
    std::cout << std::string(LSIZE, '=') << "\n";

    auto ImprimirNo = [PRECISION, xCenter](unsigned i, Real valor) {
        std::cout << std::setw(5) << i 
                  << std::scientific << std::setprecision(PRECISION)
                  << std::setw(20) << valor
                  << std::setw(20) << xCenter - valor << "\n";
    };

    unsigned contador(0);
    for (auto no : _xNo) {
        ImprimirNo(contador++, no);
    }

    std::cout << std::string(LSIZE, '=') << "\n\n";
}