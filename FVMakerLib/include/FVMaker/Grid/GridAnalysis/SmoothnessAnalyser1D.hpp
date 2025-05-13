//==============================================================================
// Nome        : SmoothnessAnalyser1D.hpp
// Autor       : João Flávio Vieira de Vasconcellos
// Versão      : 2.2
// Descrição   : Analisa a suavidade de malhas 1D separando a lógica em
//               métodos sequencial e paralelo, escolhidos por threshold de NVol.
// Licença     : GNU GPL v3
//==============================================================================

#pragma once

// -----------------------------------------------------------------------------
// INCLUDES – C++
// -----------------------------------------------------------------------------
#include <vector>
#include <algorithm>       // std::max
#include <cmath>           // std::abs
#include <numeric>         // std::transform_reduce
#include <execution>       // std::execution::par
#include <iomanip>         // std::setprecision

// -----------------------------------------------------------------------------
// INCLUDES – FVMaker
// -----------------------------------------------------------------------------
#include <FVMaker/Misc/Namespace.h>
#include <FVMaker/Misc/Configure.h>
#include <FVMaker/Grid/GridAnalysis/GridAnalyser.h>

GRID_NAMESPACE_OPEN    // abre: namespace fvm { namespace grd {

/**
 * @brief Analisa a suavidade de uma malha 1D.
 *
 * Calcula a maior variação relativa entre larguras de células adjacentes:
 *
 *     smoothness = max_i |(dx[i+1] - dx[i]) / dx[i]|
 *
 * onde dx = grid.DFaceCoordinate().
 *
 * Se o número de volumes (grid.NVol()) exceder ParallelThreshold, usa
 * transform_reduce paralelo; caso contrário, loop sequencial.
 *
 * @tparam GridType tipo da malha (ex.: UniformGrid1D)
 */
template <typename GridType>
class SmoothnessAnalyser1D : public GridAnalyser<GridType> {
public:
    explicit SmoothnessAnalyser1D(const GridType& grid)
        : GridAnalyser<GridType>(grid), smoothness_(Real(0)) {}

    /// Threshold de volumes acima do qual usar paralelismo
    static constexpr std::size_t ParallelThreshold = 10000;

    /// Executa a escolha entre análise sequencial ou paralela
    void analyse() override {
        const auto& dx = this->grid_.DFaceCoordinate();
        const std::size_t N = dx.size();
        if (N < 2) {
            smoothness_ = Real(0);
            return;
        }
        if (this->grid_.NVol() > ParallelThreshold) {
            analyseParallel(dx);
        } else {
            analyseSequential(dx);
        }
    }

    /// Impressão formatada – usado pelo operator<<.
    void print(std::ostream& os) const override {
        os << std::fixed << std::setprecision(6)
           << "Smoothness Analysis (1D Grid)\n"
           << " - Max relative Δx change: " << smoothness_;
    }

private:
    /// Cálculo sequencial da suavidade
    void analyseSequential(const std::vector<Real>& dx) {
        Real max_rel{0};
        const std::size_t N = dx.size();
        for (std::size_t i = 0; i + 1 < N; ++i) {
            if (dx[i] == Real(0)) continue;
            Real rel = std::abs((dx[i + 1] - dx[i]) / dx[i]);
            if (rel > max_rel) max_rel = rel;
        }
        smoothness_ = max_rel;
    }

    /// Cálculo paralelo da suavidade usando transform_reduce
    void analyseParallel(const std::vector<Real>& dx) {
        smoothness_ = std::transform_reduce(
            std::execution::par,
            dx.begin(), dx.end() - 1,
            dx.begin() + 1,
            Real(0),
            /* reduce_op */ [](Real a, Real b) { return a > b ? a : b; },
            /* transform_op */ [](Real a, Real b) {
                return a != Real(0)
                    ? std::abs((b - a) / a)
                    : Real(0);
            }
        );
    }

    Real smoothness_;  ///< Maior variação relativa entre Δx consecutivos
};

GRID_NAMESPACE_CLOSE   // fecha: } }
