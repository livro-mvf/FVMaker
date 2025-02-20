#pragma once

//==============================================================================
// Name        : Grid1D.h
// Author      : João Flávio Vieira de Vasconcellos
// Version     : 1.1
// Description : Definição da classe Grid1D, que representa uma grade unidimensional
//               na biblioteca FVMaker.
//
// Copyright   : Copyright (C) 2024 João Flávio Vasconcellos
//               (jflavio at iprj.uerj.br)
//
// License     : Este programa é software livre: você pode redistribuí-lo e/ou
//               modificá-lo sob os termos da Licença Pública Geral GNU, conforme
//               publicada pela Free Software Foundation, versão 3 da licença,
//               ou (a seu critério) qualquer versão posterior.
//
//               Este programa é distribuído na esperança de que seja útil,
//               mas SEM QUALQUER GARANTIA; sem mesmo a garantia implícita de
//               COMERCIABILIDADE ou ADEQUAÇÃO A UM DETERMINADO PROPÓSITO. Consulte
//               a Licença Pública Geral GNU para mais detalhes.
//
//               Você deve ter recebido uma cópia da Licença Pública Geral GNU
//               junto com este programa. Caso contrário, veja <https://www.gnu.org/licenses/>.
//==============================================================================

//==============================================================================
// Includes da biblioteca padrão do C++
//==============================================================================

//==============================================================================
// Includes da biblioteca FVMaker
//==============================================================================

#include <FVMaker/Grid/Grid.h>
#include <FVMaker/Misc/type.h>
#include <FVMaker/Misc/namespace.h>

GRID_NAMESPACE_OPEN

/**
 * @class Grid1D
 * @brief Representa uma grade unidimensional.
 *
 * A classe Grid1D herda de Grid e é usada para representar grades
 * unidimensionais em simulações numéricas usando volumes finitos.
 */

template<typename TypePattern>
class Grid1D : public Grid <TypePattern> {
    
//==============================================================================
// Construtores e destrutora
//==============================================================================
    
public:
    
    Grid1D() noexcept = default;
    Grid1D(const Grid1D&) noexcept = default;
    virtual ~Grid1D() noexcept = default;
    Grid1D (const int&, const Real&, const Real& = 0);
    
    Grid1D(Grid1D&&) = delete;
    
//==============================================================================
// Sobrecarga de operadores
//==============================================================================
    
public:
    
    Grid1D& operator=(const Grid1D&) = delete;
    Grid1D& operator=(Grid1D&&) = delete;
    
    friend std::ostream& operator<< <> (std::ostream&, const Grid1D<TypePattern>&);
    
//==============================================================================
// Funções Virtuais
//==============================================================================
    
public:
    
    [[nodiscard]] virtual std::unique_ptr<Grid1D>Clone() const = 0;
    [[nodiscard]] virtual bool GeraMalha() = 0;

    
//==============================================================================
// Funções 
//==============================================================================

    void InitVector ();
    
//==============================================================================
// Funções inline
//==============================================================================

    [[nodiscard]] inline size_t size () const {return nVol;};
    [[nodiscard]] inline size_t NVol () const {return nVol;};
    [[nodiscard]] inline Real Lenght () const {return lenght;};
    [[nodiscard]] inline Real XInit () const {return xIni;};
    [[nodiscard]] inline Real XEnd () const {return xIni + lenght;};
    [[nodiscard]] inline bool empty () const {return size() == 0;};
    [[nodiscard]] inline bool GridSetup() const {return gridSetup;};

//
//    /**
//     * @brief Construtor padrão.
//     */
//    Grid1D() noexcept = default;
//
//    /**
//     * @brief Construtor de cópia.
//     * 
//     * @param other Objeto a ser copiado.
//     */
//    Grid1D(const Grid1D& other) noexcept = default;
//
//    /**
//     * @brief Construtor de movimento.
//     * 
//     * @param other Objeto a ser movido.
//     */
//    Grid1D(Grid1D&& other) noexcept = default;
//
//    /**
//     * @brief Destrutora padrão.
//     */
//    ~Grid1D() noexcept = default;
//
////==============================================================================
//// Operadores
////==============================================================================
//
//    /**
//     * @brief Operador de atribuição por cópia.
//     * 
//     * @param other Objeto a ser copiado.
//     * @return Referência para o objeto atual.
//     */
//    Grid1D& operator=(const Grid1D& other) = default;
//
//    /**
//     * @brief Operador de atribuição por movimento.
//     * 
//     * @param other Objeto a ser movido.
//     * @return Referência para o objeto atual.
//     */
//    Grid1D& operator=(Grid1D&& other) noexcept = default;
    
protected:

    VecReal                         xFace;
    VecReal                         xCentro;
    VecReal                         dxFace;
    VecReal                         dxCentro;
    Real                            xIni = 0;
    Real                            lenght = 0;
    int                             nVol = 0;
    bool                            gridSetup = false;
    std::shared_ptr<TypePattern>    typePattern  = nullptr;
    
};

GRID_NAMESPACE_CLOSE

#include <FVMaker/Grid/Grid1D/Grid1D.hpp>