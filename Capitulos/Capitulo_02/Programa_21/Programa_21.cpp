//==============================================================================
// Nome        : Programa_21.cpp
// Autor       : Joao Flavio Vieira de Vasconcellos
// Versao      : 2.2
// Descricao   : Programa para calculo de coordenadas de nos em malha unidimensional
//               com progressao geometrica usando o metodo de diferencas finitas
// Livro       : Codigo 2.1 do livro:
//               "Solucao Numerica de Equacoes Diferenciais - O Metodo de Volumes Finitos"
// Testado     : gcc 13.2 usando c++23 em 2024-06-20
//==============================================================================

/**
 * @file Programa_21.cpp
 * @brief Programa para calculo de coordenadas de nos em malha unidimensional com progressao geometrica
 * 
 * Este programa gera uma malha unidimensional onde os nos sao distribuidos segundo
 * uma progressao geometrica. Versao com implementacoes paralela e sequencial.
 * 
 * @version 2.2
 * @date 2024-06-20
 * @author Joao Flavio Vasconcellos
 * 
 * @copyright Copyright (c) 2024
 * @license GNU GPL v3
 */

//==============================================================================
//      Inclusoes da Biblioteca Padrao do C++
//==============================================================================

#include <algorithm>       // Para std::for_each, std::generate
#include <execution>       // Para std::execution::par
#include <iomanip>         // Para std::setw, std::setprecision
#include <iostream>        // Para std::cout, std::endl
#include <numeric>         // Para std::inclusive_scan
#include <vector>          // Para std::vector

//==============================================================================
//      Definicoes de tipos
//==============================================================================

using Real = double;                   // Tipo para numeros reais
using VetorReal = std::vector<Real>;   // Vetor de numeros reais

//==============================================================================
//      Prototipos das funcoes
//==============================================================================

constexpr Real ipow(Real _base, int _exp) noexcept;
void CalcularSequencial(VetorReal&, const Real&, const Real&, const Real&, const unsigned&);
void CalcularParalelo(VetorReal&, const Real&, const Real&, const Real&, const unsigned&);
void ImprimirResultados(const VetorReal&, const Real&, const Real&, const Real&, const unsigned&);

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
    constexpr Real RATIO = 1.05;                // Razao da progressao geometrica
    

    if (NNOS < 2) {
        std::cerr << "Erro: Numero de nos deve ser >= 2\n";
        return EXIT_FAILURE;
    }
    
    VetorReal xCentro(NNOS);

//==============================================================================
//      Selecao automatica da versao (serial/paralela)
//==============================================================================
    
    if (NNOS < NNOSSERIAL) {
        CalcularSequencial(xCentro, XINIT, LENGTH, RATIO, NNOS);
    } else {
        CalcularParalelo(xCentro, XINIT, LENGTH, RATIO, NNOS);
    }

    ImprimirResultados(xCentro, XINIT, LENGTH, RATIO, NNOS);
    
    return EXIT_SUCCESS;
}

//==============================================================================
//     Funcoes do codigo
//==============================================================================

/**
 * @brief Calcula a potencia de um numero real com expoente inteiro
 * 
 * @param _base     Base da potencia
 * @param _exp      Expoente inteiro
 * @return Real     Resultado da potencia
 */
constexpr Real ipow(Real _base, int _exp) noexcept {

    if (_exp < 0) return 0.0;
    
    Real result = 1.0;
    while (_exp > 0) {
        if (_exp % 2 == 1) {
            result *= _base;
        }
        _base *= _base;
        _exp /= 2;
    }

    return result;
}

/**
 * @brief Versao serial da geracao de malha
 * 
 * @param _xNo Vetor para armazenar as coordenadas dos nos
 * @param _xInit Coordenada inicial do dominio
 * @param _length Comprimento total do dominio
 * @param _ratio Razao da geracao da malha geometrica
 * @param _nNos Comprimento total do dominio
 */
void CalcularSequencial(    VetorReal&          _xNode,
                            const Real&         _xInit,
                            const Real&         _length,
                            const Real&         _ratio,
                            const unsigned&     _nNos) {

    Real delta = _length * (_ratio - 1.0) / (ipow(_ratio, _nNos - 1) - 1.0);;
    Real x = _xInit;
    auto geomProg = [&](Real& _x) {
        _x = x;
        x += delta;
        delta *= _ratio;
    };
    
    std::for_each(_xNode.begin(), _xNode.end(), geomProg);
}

/**
 * @brief Versao paralela da geracao de malha
 * 
 * @param _xNo Vetor para armazenar as coordenadas dos nos
 * @param _xInit Coordenada inicial do dominio
 * @param _length Comprimento total do dominio
 * @param _ratio Razao da geracao da malha geometrica
 * @param _nNos Comprimento total do dominio
 */
void CalcularParalelo(  VetorReal&          _xNode,
                        const Real&         _xInit,
                        const Real&         _length,
                        const Real&         _ratio,
                        const unsigned&     _nNos) {
    
    VetorReal deltas(_nNos);
    deltas[0] = 0.0;

    Real delta = _length * (_ratio - 1.0) / (ipow(_ratio, _nNos - 1) - 1.0);;

    
    for (unsigned i = 1; i < _nNos; ++i) {
        deltas[i] = delta;
        delta *= _ratio;
    }

    std::inclusive_scan(    std::execution::par,
                            deltas.begin(), 
                            deltas.end(),
                            deltas.begin());

    auto Soma = [_xInit](const Real& _valor) { return _xInit + _valor; };

    std::transform  (   std::execution::par,
                        deltas.begin(),
                        deltas.end(),
                        _xNode.begin(),
                        Soma);
}

/**
 * @brief Imprime os resultados formatados
 * 
 * @param _xNo Vetor para armazenar as coordenadas dos nos
 * @param _xInit Coordenada inicial do dominio
 * @param _length Comprimento total do dominio
 * @param _ratio Razao da geracao da malha geometrica
 * @param _nNos Comprimento total do dominio
 */
void ImprimirResultados(    const VetorReal&    _xNo,
                            const Real&         _xInit,
                            const Real&         _length,
                            const Real&         _ratio,
                            const unsigned&     _nNos) {
    constexpr unsigned LSIZE = 25;
    constexpr unsigned PRECISION = 6;
    
    std::cout << "\nDADOS DA MALHA GEOMETRICA\n";
    std::cout << std::string(LSIZE + 4, '=') << "\n";
    std::cout << std::fixed << std::setprecision(3);
    std::cout << "Comprimento total: " << std::setw(10) << _length << "\n";
    std::cout << "Coordenada inicial:" << std::setw(10) << _xInit << "\n";
    std::cout << "Razao geometrica:  " << std::setw(10) << _ratio << "\n";
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