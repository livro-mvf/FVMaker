// ============================================================================
// Arquivo: gridReport.h
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Declara grid Report no contexto de Grid / GridAnalysis.
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

//=============================================================================
// INCLUDES – C++
//=============================================================================
#include <tuple>
#include <utility>
#include <ostream>

//=============================================================================
// INCLUDES – FVMaker
//=============================================================================
#include <FVMaker/Grid/GridAnalysis/gridAnalyser.h>
#include <FVMaker/Grid/gridDimension.h>
// 1D analysers
#include <FVMaker/Grid/GridAnalysis/1D/cellSizeAnalyser1D.h>
#include <FVMaker/Grid/GridAnalysis/1D/smoothnessAnalyser1D.h>
#include <FVMaker/Grid/GridAnalysis/1D/uniformityAnalyser1D.h>
// Inclusoes futuras para 2D e 3D

#include <FVMaker/Misc/namespace.h>

GRID_NAMESPACE_OPEN

//=============================================================================
// Encaminhamentos futuros (2D e 3D)
//=============================================================================
// template<typename GridT> class CellSizeAnalyser2D;
// template<typename GridT> class SmoothnessAnalyser2D;
// ...

//=============================================================================
// Classe base auxiliar
//=============================================================================

namespace detail {

/**
 * @brief Base CRTP que provem funcoes de analise e impressao para qualquer
 *        tupla de analisadores.
 *
 * @tparam GridT  Tipo de grade concreto.
 * @tparam TupleT Tupla de tipos de analisadores.
 */
template <typename GridT, typename TupleT>
class GridReportBase {
protected:
    using Analysers = TupleT;

public:
    explicit GridReportBase(const GridT& grid) noexcept :
        grid_{ grid },
        analysers_{ makeAnalysers_(std::make_index_sequence<
            std::tuple_size_v<Analysers>>{}) } {}

    /// Executa todos os analisadores
    void analyse() {
        std::apply([&](auto&... a) { (a.analyse(), ...); }, analysers_);
    }

    /// Escreve resultados agregados em um stream
    void print(std::ostream& os) const {
        std::apply([&](const auto&... a) {
            ((os << a << '\n'), ...);
        }, analysers_);
    }

    friend std::ostream& operator<<(std::ostream& os,
                                    const GridReportBase& r) {
        r.print(os);
        return os;
    }

protected:
    const GridT& grid_;
    Analysers    analysers_;

private:
    template <std::size_t... I>
    Analysers makeAnalysers_(std::index_sequence<I...>) {
        return Analysers{ typename std::tuple_element<I, Analysers>::type{ grid_ }... };
    }
};

} // namespace detail

//=============================================================================
// Template primario – gera erro em compilacao para grades nao suportadas
//=============================================================================

template <typename GridT, int Dim = GridDim<GridT>::value>
class GridReport {
    static_assert(Dim != -1,
                  "GridReport: grid sem suporte ou trait GridDim ausente");
};

//=============================================================================
// Especializacao 1D
//=============================================================================

template <typename GridT>
requires Is1DGrid<GridT>
class GridReport<GridT, 1>
    : public detail::GridReportBase<
          GridT,
          std::tuple<
              CellSizeAnalyser1D<GridT>,
              SmoothnessAnalyser1D<GridT>,
              UniformityAnalyser1D<GridT>>> {

    using Base = detail::GridReportBase<
        GridT,
        std::tuple<
            CellSizeAnalyser1D<GridT>,
            SmoothnessAnalyser1D<GridT>,
            UniformityAnalyser1D<GridT>>>;

public:
    explicit GridReport(const GridT& grid) noexcept : Base{ grid } {}
};

//=============================================================================
// TODO: especializacoes 2D e 3D (adicionar quando implementadas)
//=============================================================================

GRID_NAMESPACE_CLOSE
