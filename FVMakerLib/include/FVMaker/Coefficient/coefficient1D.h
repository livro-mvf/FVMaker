// ============================================================================
// Arquivo: coefficient1D.h
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Declara coefficient 1 D no contexto de Coefficient.
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

#pragma once

//==============================================================================
// Includes da biblioteca padrão do C++
//==============================================================================
#include <iostream>

//==============================================================================
// Incluições do FVMAKER
//==============================================================================

#include <FVMaker/Coefficient/abstractCoefficient.h>              ///< Definições da classe base de coeficientes; 


FVMAKER_NAMESPACE_OPEN

/**
 * @brief Classe genérica para representar uma função matemática definida
 * sobre os centros dos volumes de uma grid.
 *
 * A classe assume que o grid fornece um método (ex.: PtrSCCentreCoordinate())
 * que retorna um ponteiro para um container dos centros dos volumes.
 * Cada centro é convertido para um std::vector<Real> contendo as coordenadas,
 * possibilitando usar a mesma interface para grid 1D, 2D, 3D, etc.
 */
// Representa um coeficiente unidimensional constante ou calculado por funcao.
template <typename T>
class Coefficient1D {

public :
    

    
    
//==============================================================================
// Construtores e destrutora
//==============================================================================
    
public:
    

    // Cria um objeto Coefficient1D com os dados fornecidos.
    Coefficient1D(const Coefficient1D&) noexcept = default;
    // Finaliza um objeto Coefficient1D.
    ~Coefficient1D() noexcept = default;
    // Cria um objeto Coefficient1D com os dados fornecidos.
    Coefficient1D(Coefficient1D&&) = delete;    
    
    // Cria um objeto Coefficient1D com os dados fornecidos.
    explicit Coefficient1D (const T& _grid, const Real& = 1.0) : AbstractCoefficient<T>(_grid) {
        this->coeff_.assign(this->grid_.size(), 1.0);
    };
    
    // Cria um objeto Coefficient1D com os dados fornecidos.
    explicit Coefficient1D(const AbstractCoefficient<T>& _other)
            : AbstractCoefficient<T>( _other.Grid())  // chama construtor base com "other.grid_"
    {
    // Copia dados do "other" para this->coeff_
        this->coeff_ = *(_other.VectorCoeff());
    }
//==============================================================================
// Sobrecarga de operadores
//==============================================================================
    
public:
    
    Coefficient1D& operator=(const Coefficient1D&) = delete;
    Coefficient1D& operator=(Coefficient1D&&) = delete;
    
    // Escreve uma representacao textual do objeto no fluxo de saida.
    template <typename U>
    friend std::ostream& operator<< (std::ostream&, const Coefficient1D<U>&);    
};


FVMAKER_NAMESPACE_CLOSE

#include <FVMaker/Coefficient/coefficient1D.hpp>