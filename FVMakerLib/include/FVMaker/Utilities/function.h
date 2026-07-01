// ============================================================================
// Arquivo: function.h
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Declara function no contexto de Utilities.
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

#include <functional>

//==============================================================================
// Incluições do FVMAKER
//==============================================================================

#include <FVMaker/Grid/abstractGrid.h>     // Definições da Grid; 
using fvm::grd::AbstractGrid;
#include <FVMaker/Grid/gridDimension.h>              ///< Definições da Grid; 
//#include <vector>
//#include <memory>
//#include <stdexcept>
//#include <utility>
//#include <tuple>
//#include <FVMaker/Misc/type.h>      // Define Real e possivelmente VecReal
//                                     // para Grid2D ou Grid3D, incluir os headers correspondentes

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
// Representa uma funcao escalar usada em modelos numericos.
template <typename T>
class Function {

//==============================================================================
// Construtores / Destrutora
//==============================================================================

    using DataType = typename GridDim<T>::DataType;
    
public:

    // Cria um objeto Function com os dados fornecidos.
    explicit Function (const T& _grid) : grid_(_grid) {}
    // Finaliza um objeto Function.
    ~Function() noexcept  = default;

//==============================================================================
// Definição da função a ser utilizada pelo function
//==============================================================================

public:

    // Define a informacao set function usada pelo objeto.
    void setFunction (std::function<PairData(const DataType&)> _function) {
        function_ = std::move(_function);
    }

    // Realiza a operacao fx definida por esta interface.
    [[nodiscard]] inline PairData Fx (const DataType& _data) const {
        return function_(_data);
    };
//
//    /// Avalia a função para um único ponto (representado como std::vector<Real>).
//    double Fx(const std::vector<Real>& point) const {
//        if (!function_) {
//            throw std::runtime_error("Function not set!");
//        }
//        return function_(point);
//    }
//
//    /// Avalia a função para todos os centros dos volumes definidos na grid.
//    /// Este método acessa os centros via grid_.PtrSCCentreCoordinate() e
//    /// converte cada centro para um std::vector<Real> (usando os métodos auxiliares).
//    std::vector<double> evaluate() const {
//        if (!function_) {
//            throw std::runtime_error("Function not set!");
//        }
//
//        std::vector<double> results;
//        // Obtém os centros da grid. Espera-se que esse método retorne um ponteiro (smart pointer)
//        // para um container contendo os centros.
//        auto centers = grid_.PtrSCCentreCoordinate();
//        results.reserve(centers->size());
//
//        for (const auto& center : *centers) {
//            // Converte o centro para std::vector<Real> conforme sua dimensão:
//            std::vector<Real> point = convertToVector(center);
//            results.push_back(Fx(point));
//        }
//        return results;
//    }
//
private:
    
    const T&                                    grid_;  ///< Referência para a grid usada
    std::function<PairData(const DataType&)>    function_; ///< Função matemática
//
//    // Funções auxiliares para converter o dado do centro para std::vector<Real>
//    // Para grid 1D: assume que o centro é do tipo Real.
//    std::vector<Real> convertToVector(const Real& x) const {
//        return {x};
//    }
//
//    // Para grid 2D: assume que o centro é do tipo std::pair<Real, Real>.
//    std::vector<Real> convertToVector(const std::pair<Real, Real>& xy) const {
//        return {xy.first, xy.second};
//    }
//
//    // Para grid 3D: assume que o centro é do tipo std::tuple<Real, Real, Real>.
//    std::vector<Real> convertToVector(const std::tuple<Real, Real, Real>& xyz) const {
//        return { std::get<0>(xyz), std::get<1>(xyz), std::get<2>(xyz) };
//    }
};

FVMAKER_NAMESPACE_CLOSE
