#pragma once

//==============================================================================
// Nome        : Grid.h
// Autor       : João Flávio Vieira de Vasconcellos
// Versão      : 1.0
// Descrição   : Classe base para todas as classes de geração de malhas.
//               Define a interface comum para as classes derivadas de malhas 1D, 2D, 3D, etc.
//==============================================================================

////==============================================================================
//// Includes do C++
////==============================================================================
//
//#include <memory>
//#include <iosfwd>  // Para declaração antecipada de std::ostream
//
//==============================================================================
// FVMAKER includes
//==============================================================================

#include <FVMaker/Misc/namespace.h>
#include <FVMaker/Misc/type.h>

//==============================================================================
// Abertura do namespace FVMaker::Grid
//==============================================================================

GRID_NAMESPACE_OPEN

/**
 * @class Grid
 * @brief Classe base para todas as classes de malhas (1D, 2D, 3D, etc.).
 *
 * Esta classe fornece uma interface comum para as classes derivadas, mas
 * não armazena as coordenadas. As classes derivadas (como Grid1D, Grid2D, etc.)
 * devem implementar a lógica para gerar e armazenar as coordenadas específicas.
 */

template<typename TypePattern>
class Grid {

//==============================================================================
// Construtores / destrutora
//==============================================================================
    
public:
    
    Grid() noexcept = default;
    Grid(const Grid&) noexcept = default;
    virtual ~Grid() noexcept = default;
    Grid(Grid&&) = delete;
    
//==============================================================================
// Sobrecarga de operadores
//==============================================================================
    
public:
    
    Grid& operator=(const Grid&) = delete;
    Grid& operator=(Grid&&) = delete;
    
//==============================================================================
// funções puramente virtuais
//==============================================================================
    
public:
    
    virtual int Dimension () const = 0;
    
//public:
//   
//    /**
//     * @brief Construtora default da classe Grid.
//     */
//
//    Grid() noexcept = default;
//
//     /**
//     * @brief Construtor de cópia da classe Grid.
//     * 
//     * @param other Objeto a ser copiado.
//     */
//
//    Grid(const Grid& other) noexcept = default;
//    
//    /**
//     * @brief Destrutora da classe Grid.
//     */
//
//    virtual ~Grid() noexcept = default;
//
////==============================================================================
//// Funções da classe
////==============================================================================
//    
//protected:    
//    
//    /**
//     * @brief Função puramente virtual para gerar as coordenadas das faces e centros.
//     * As classes derivadas devem implementar esse método.
//     */
//    virtual void generate() = 0;
//
//    /**
//     * @brief Método virtual puro para imprimir as coordenadas.
//     * Cada classe derivada implementará como imprimir as coordenadas específicas.
//     */
//    virtual void printCoordinates(std::ostream& os) const = 0;
//    
//public:    
//
//    /**
//     * @brief Sobrecarga do operador de saída para imprimir as coordenadas.
//     * @param os Fluxo de saída.
//     * @param grid Referência para o objeto Grid.
//     * @return Referência para o fluxo de saída.
//     */
//
//    friend std::ostream& operator<<(std::ostream& os, const Grid& grid) {
//        grid.printCoordinates(os);
//        return os;
//    }
//
};

#include <FVMAKER/Grid/Grid.hpp>

GRID_NAMESPACE_CLOSE