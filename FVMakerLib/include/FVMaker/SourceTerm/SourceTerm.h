#pragma once

//==============================================================================
// Includes da biblioteca padrão do C++
//==============================================================================


//==============================================================================
// Incluições do FVMAKER
//==============================================================================

#include <FVMaker/Grid/AbstractGrid.h>     // Definições da Grid; 
#include <FVMaker/Grid/GridDimension.h>       // GridDim
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
   * @brief Destrutor virtual padrão.
   */
  virtual ~SourceTerm() noexcept = default;

  /**
   * @brief Construtor que recebe uma referência para a malha.
   * 
   * Lança exceção FVMakerException se a malha estiver vazia.
   * 
   * @param _grid Grid a ser utilizado.
   */
  explicit SourceTerm(const T& _grid) : grid_(_grid) {
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
    
    vecSource.resize(this->grid_.size(), SourceData(0,0));
  }

   inline const T& getGridRef() const {
      return grid_;
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
// Dados da classe
//==============================================================================

protected:
  /**
   * @brief Referência para a malha usada.
   * 
   * O tempo de vida da malha deve ser maior que o deste objeto.
   */
    const T&        grid_; 
    VecSource       vecSource;

};

FVMAKER_NAMESPACE_CLOSE

#include <FVMaker/SourceTerm/SourceTerm.hpp>   
