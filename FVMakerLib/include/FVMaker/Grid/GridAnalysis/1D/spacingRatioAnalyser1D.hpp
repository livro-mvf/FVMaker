//==============================================================================
// Nome        : SpacingRatioAnalyser1D.hpp
// Autor       : João Flávio Vieira de Vasconcellos
// Versão      : 1.0
// Descrição   : Analisa a malha 1D calculando a máxima razão entre espaçamentos
//               adjacentes:
//                   max_i ( Δx_i/Δx_{i+1}, Δx_{i+1}/Δx_i )
//               Usa diretamente o vetor Δx das faces (DFaceCoordinate).
// Licença     : GNU GPL v3
//==============================================================================

#pragma once

// -----------------------------------------------------------------------------
// INCLUDES – C++
// -----------------------------------------------------------------------------
#include <algorithm>  // std::max
#include <cmath>      // std::abs
#include <iomanip>    // std::setprecision

// -----------------------------------------------------------------------------
// INCLUDES – FVMaker
// -----------------------------------------------------------------------------
#include <FVMaker/Misc/Namespace.h>
#include <FVMaker/Misc/Configure.h>
#include <FVMaker/Grid/GridAnalysis/GridAnalyser.h>

GRID_NAMESPACE_OPEN    // abre: namespace fvm { namespace grd {

/**
 * @brief Analisa a razão máxima de espaçamentos entre células adjacentes em 1D.
 *
 * Para cada i:
 *     r_i = max( Δx_i / Δx_{i+1}, Δx_{i+1} / Δx_i )
 * Retorna max_i r_i.
 *
 * Usa o vetor de Δx pré-computado pela malha:
 *     const auto& dx = grid.DFaceCoordinate();
 *
 * @tparam GridType tipo da malha (ex.: UniformGrid1D)
 */
template <typename GridType>
class SpacingRatioAnalyser1D : public GridAnalyser<GridType>
{
public:
    explicit SpacingRatioAnalyser1D(const GridType& _grid)
        : GridAnalyser<GridType>(_grid), max_ratio_(0.0) {}

    /// Executa a análise de razão de espaçamentos.
    void analyse() override {
        const auto& dx = this->grid.GetDFace();
        const std::size_t N = dx.size();
        if (N < 2) {
            max_ratio_ = 0.0;
            return;
        }

        Real r_max = 0.0;
        for (std::size_t i = 0; i + 1 < N; ++i) {
            if (dx[i] == Real(0.0) || dx[i+1] == Real(0.0))
                continue;
            Real r1 = dx[i]   / dx[i+1];
            Real r2 = dx[i+1] / dx[i];
            r_max = std::max(r_max, std::max(r1, r2));
        }
        max_ratio_ = r_max;
    }

    /// Imprime o resultado formatado.
    void print(std::ostream& os) const override {
        os << std::fixed << std::setprecision(6)
           << "Spacing Ratio Analysis (1D Grid)\n"
           << " - Max spacing ratio: " << max_ratio_;
    }

private:
    Real max_ratio_;  ///< Razão máxima entre Δx adjacentes
};

GRID_NAMESPACE_CLOSE   // fecha: } }
