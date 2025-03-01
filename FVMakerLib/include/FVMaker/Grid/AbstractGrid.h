#pragma once

//==============================================================================
// Nome        : Abstract.h
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
// Abertura do namespace FVMaker::AbstractGrid
//==============================================================================

GRID_NAMESPACE_OPEN

/**
 * @class AbstractGrid
 * @brief Classe base para todas as classes de malhas (1D, 2D, 3D, etc.).
 *
 * Esta classe fornece uma interface comum para as classes derivadas, mas
 * não armazena as coordenadas. As classes derivadas (como AbstractGrid1D, AbstractGrid2D, etc.)
 * devem implementar a lógica para gerar e armazenar as coordenadas específicas.
 */

template<typename TypePattern>
class AbstractGrid {

//==============================================================================
// Construtores / destrutora
//==============================================================================
    
public:
    
    AbstractGrid() noexcept = default;
    AbstractGrid(const AbstractGrid&) noexcept = default;
    virtual ~AbstractGrid() noexcept = default;
    AbstractGrid(AbstractGrid&&) = delete;
    
//==============================================================================
// Sobrecarga de operadores
//==============================================================================
    
public:
    
    AbstractGrid& operator=(const AbstractGrid&) = delete;
    AbstractGrid& operator=(AbstractGrid&&) = delete;
    
//==============================================================================
// funções puramente virtuais
//==============================================================================
    
public:
    
    virtual int Dimension () const = 0;
    
//public:
//   
//    /**
//     * @brief Construtora default da classe AbstractGrid.
//     */
//
//    AbstractGrid() noexcept = default;
//
//     /**
//     * @brief Construtor de cópia da classe AbstractGrid.
//     * 
//     * @param other Objeto a ser copiado.
//     */
//
//    AbstractGrid(const AbstractGrid& other) noexcept = default;
//    
//    /**
//     * @brief Destrutora da classe AbstractGrid.
//     */
//
//    virtual ~AbstractGrid() noexcept = default;
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
//     * @param grid Referência para o objeto AbstractGrid.
//     * @return Referência para o fluxo de saída.
//     */
//
//    friend std::ostream& operator<<(std::ostream& os, const AbstractGrid& grid) {
//        grid.printCoordinates(os);
//        return os;
//    }
//
};

#include <FVMAKER/Grid/AbstractGrid.hpp>

GRID_NAMESPACE_CLOSE