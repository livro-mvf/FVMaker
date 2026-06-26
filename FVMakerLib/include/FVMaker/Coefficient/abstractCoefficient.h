// #pragma once

// //==============================================================================
// // Name        : AbstractCoefficient.h
// // Author      : João Flávio Vieira de Vasconcellos
// // Version     : 1.1
// // Description : Definição da classe AbstractCoefficient, para representar
// //               uma função matemática definida sobre os centros dos volumes
// //               de uma malha na biblioteca FVMaker.
// //
// // Copyright   : Copyright (C) 2025 João Flávio Vasconcellos
// //               (jflavio at iprj.uerj.br)
// //
// // License     : Este programa é software livre: você pode redistribuí-lo e/ou
// //               modificá-lo sob os termos da Licença Pública Geral GNU, conforme
// //               publicada pela Free Software Foundation, versão 3 da licença,
// //               ou (a seu critério) qualquer versão posterior.
// //
// //               Este programa é distribuído na esperança de que seja útil,
// //               mas SEM QUALQUER GARANTIA; sem mesmo a garantia implícita de
// //               COMERCIABILIDADE ou ADEQUAÇÃO A UM DETERMINADO PROPÓSITO.
// //               Consulte a Licença Pública Geral GNU para mais detalhes.
// //
// //               Você deve ter recebido uma cópia da Licença Pública Geral GNU
// //               junto com este programa. Caso contrário, veja
// //               <https://www.gnu.org/licenses/>.
// //==============================================================================



// //==============================================================================
// // Includes da biblioteca FVMaker
// //==============================================================================
// #include <FVMaker/Error/FVMakerException.h>   // FVMakerException
// #include <FVMaker/Grid/gridDimension.h>       // GridDim

// FVMAKER_NAMESPACE_OPEN

// /**
//  * @defgroup Coefficient
//  * @{
//  */

// /**
//  * @brief Classe genérica que representa um conjunto de coeficientes
//  *        sobre os centros dos volumes de uma malha.
//  *
//  * @tparam T Tipo da malha utilizada.
//  * 
//  * Esta classe faz parte do grupo @ref Coefficient.
//  */
// template <typename T>
// class AbstractCoefficient {
  
// public:
      
//   using DataType = typename GridDim<T>::DataType;

// //==============================================================================
// // Construtores e destrutora
// //==============================================================================

// public:

//   /**
//    * @brief Construtor de cópia padrão.
//    */
//   AbstractCoefficient() noexcept = default;

//   /**
//    * @brief Construtor de cópia padrão.
//    */
//   AbstractCoefficient(const AbstractCoefficient&) noexcept = default;

//   /**
//    * @brief Destrutor virtual padrão.
//    */
//   virtual ~AbstractCoefficient() noexcept = default;

//   /**
//    * @brief Construtor que recebe uma referência para a malha.
//    * 
//    * Lança exceção FVMakerException se a malha estiver vazia.
//    * 
//    * @param _grid Grid a ser utilizado.
//    */
//   explicit AbstractCoefficient(const T& _grid) : grid_(_grid) {
//     try {
//       if (this->grid_.empty()) {
//         throw fvm::FVMakerException(ErrorCode::UndefiniedGrid);
//       }
//     } catch (const fvm::FVMakerException& e) {
//       std::cerr << "\n\n";
//       PrintLine(std::cerr);
//       std::cerr << "Exceção capturada: " << e.what() << "\n";
//       PrintLine(std::cerr);
//       std::cerr << "\n\n";
//       exit(EXIT_FAILURE);
//     }

//     coeff_.resize(this->grid_.NVol());
//   }

// //==============================================================================
// // Funções inline
// //==============================================================================
   
// public:
      

//   /**
//    * @brief Verifica se o grid ou o vetor de coeficientes está vazio.
//    * 
//    * @return true se estiver vazio, caso contrário false.
//    */
//   [[nodiscard]] inline bool empty() const {
//     return this->coeff_.empty() || this->grid_.empty();
//   }

//   [[nodiscard]] inline size_t NVol() const {
//     return  this->grid_.NVol();
//   }
    
      
//   /**
//    * @brief Retorna ponteiro para o grid.
//    * 
//    * @return Ponteiro constante para o grid.
//    */
//   [[nodiscard]] inline const T& Grid() const {
//     return grid_;
//   }

//   /**
//    * @brief Retorna ponteiro para o vetor de coeficientes.
//    * 
//    * @return Ponteiro constante para std::vector<DataType>.
//    */
//   [[nodiscard]] inline const std::vector<DataType>* VectorCoeff() const {
//     return &coeff_;
//   }
  
//  //   [[nodiscard]] inline std::shared_ptr<const std::vector<DataType>> PtrSCCoeff() const {return std::make_shared<const std::vector<DataType>>(coeff_);};
  
// //==============================================================================
// // Dados da classe
// //==============================================================================

// protected:
//   /**
//    * @brief Referência para a malha usada.
//    * 
//    * O tempo de vida da malha deve ser maior que o deste objeto.
//    */
//   const T& grid_;

//   /**
//    * @brief Vetor que armazena os coeficientes associados a cada volume.
//    */
//   std::vector<DataType> coeff_;
// };

// template<typename TypePattern>
// using SharedAbstractCoefficient = std::shared_ptr<AbstractCoefficient<TypePattern>>;
// template<typename TypePattern>
// using SharedConstAbstractCoefficient = std::shared_ptr<AbstractCoefficient<TypePattern> const>;
// template<typename TypePattern>
// using UniqueAbstractCoefficient = std::unique_ptr<AbstractCoefficient<TypePattern>>;
// template<typename TypePattern>
// using UniqueConstAbstractCoefficient = std::unique_ptr<AbstractCoefficient<TypePattern> const>;


// /** @} */ // end of Coefficient group

// FVMAKER_NAMESPACE_CLOSE

// #include <FVMaker/Coefficient/abstractCoefficient.hpp>


