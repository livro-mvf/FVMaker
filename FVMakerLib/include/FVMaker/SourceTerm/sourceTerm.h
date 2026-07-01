// ============================================================================
// Arquivo: sourceTerm.h
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Declara source Term no contexto de SourceTerm.
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


//==============================================================================
// Incluições do FVMAKER
//==============================================================================

#include <FVMaker/Grid/abstractGrid.h>     // Definições da Grid; 
#include <FVMaker/Grid/gridDimension.h>       // GridDim
#include <FVMaker/Utilities/function.h>        ///< Definição da classe Function

using fvm::grd::AbstractGrid;

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
template <typename T>
class SourceTerm {

public:
      
  using DataType = typename GridDim<T>::DataType;

//==============================================================================
// Construtores e destrutora
//==============================================================================

public:

  /**
   * @brief Construtor de cópia padrão.
   */
  SourceTerm(const SourceTerm&) noexcept = default;

  /**
   * @brief Destrutor padrão.
   */
  ~SourceTerm() noexcept = default;

  /**
   * @brief Construtor que recebe uma referência para a malha.
   * 
   * Lança exceção FVMakerException se a malha estiver vazia.
   * 
   * @param _grid Grid a ser utilizado.
   */
                

  explicit SourceTerm   (   const T& _grid
                        ,   const Function<T>*  _funcao = nullptr
                        ) 
                        :   grid_(_grid) 
                        ,   sourceFunction_(_funcao) {
    try {
      if (this->grid_.empty()) {
        throw fvm::FVMakerException(ErrorCode::UndefiniedGrid);
      }
    } catch (const fvm::FVMakerException& e) {
      std::cerr << "\n\n";
      PrintLine(std::cerr);
      std::cerr << "Exceção capturada: " << e.what() << "\n";
      PrintLine(std::cerr);
      std::cerr << "\n\n";
      exit(EXIT_FAILURE);
    }
    
    vecSource_.resize(this->grid_.size(), PairData(0,0));
  }

//==============================================================================
// Sobrecarga de operadores
//==============================================================================
    
public:
    
    SourceTerm& operator=(const SourceTerm&) = delete;
    SourceTerm& operator=(SourceTerm&&) = delete;
    
    template <typename U>
    friend std::ostream& operator<< (std::ostream&, const SourceTerm<U>&);


//==============================================================================
// Funções inline
//==============================================================================
    
public:

  [[nodiscard]] inline size_t NVol() const {
    return  this->grid_.NVol();
  }

  [[nodiscard]] inline const T& Grid() const {
    return grid_;
  }

//==============================================================================
// Funcoes
//==============================================================================

  [[nodiscard]] bool EvaluateSourceFunction ();
    
public:

//==============================================================================
// Dados da classe
//==============================================================================

protected:
  /**
   * @brief Referência para a malha usada.
   * 
   * O tempo de vida da malha deve ser maior que o deste objeto.
   */
    const T&                grid_; 
    VecSource               vecSource_;
    const Function<T>*      sourceFunction_;    
};

FVMAKER_NAMESPACE_CLOSE

#include <FVMaker/SourceTerm/sourceTerm.hpp>   
