//==============================================================================
// Nome        : GridReport.h
// Autor       : Joao Flavio Vieira de Vasconcellos
// Versao      : 1.1
// Descricao   : Agregador de analisadores de qualidade de malha no FVMaker.
//               Gera relatorios especificos por dimensao em tempo de
//               compilacao sem ramos condicionais em tempo de execucao.
// Licenca     : GNU GPL v3
//==============================================================================

/**
 * @file GridReport.h
 * @brief Gerador de relatorio para grades estruturadas
 * @ingroup GridAnalysis
 *
 * `GridReport<GridT>` reune todas as classes de analise aplicaveis a um tipo
 * de grade e executa-as em uma unica chamada. A escolha da dimensao ocorre em
 * tempo de compilacao via especializacao de template e concepts, sem enums ou
 * ifs.
 *
 * Exemplo de uso
 * @code
 * UniformGrid1D grade{ 200, 1.0 };
 * GridReport<UniformGrid1D> relatorio{ grade };
 * relatorio.analyse();
 * std::cout << relatorio;
 * @endcode
 *
 * Observacao: Comentarios e blocos Doxygen estao em portugues sem acentos.
 */

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
#include <FVMaker/Grid/GridAnalysis/GridAnalyser.h>
#include <FVMaker/Grid/GridDimension.h>
// 1D analysers
#include <FVMaker/Grid/GridAnalysis/1D/CellSizeAnalyser1D.h>
#include <FVMaker/Grid/GridAnalysis/1D/SmoothnessAnalyser1D.h>
#include <FVMaker/Grid/GridAnalysis/1D/UniformityAnalyser1D.h>
// Inclusoes futuras para 2D e 3D

#include <FVMaker/Misc/Namespace.h>

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
