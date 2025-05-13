//==============================================================================
// Nome        : CellSizeAnalyser1D.hpp
// Autor       : João Flávio Vieira de Vasconcellos
// Versão      : 2.0
// Descrição   : Analisa o tamanho das células (Δx) em malhas 1D. Calcula
//               Δx mínimo, máximo e médio usando o vetor Δx das faces
//               (DFaceCoordinate) fornecido pela própria malha.
// Licença     : GNU GPL v3
//==============================================================================

#pragma once

// -----------------------------------------------------------------------------
// INCLUDES – C++
// -----------------------------------------------------------------------------
#include <algorithm>     // std::min_element / std::max_element
#include <numeric>       // std::accumulate
#include <iomanip>       // std::setprecision

// -----------------------------------------------------------------------------
// INCLUDES – FVMaker
// -----------------------------------------------------------------------------
#include <FVMaker/Misc/Namespace.h>
#include <FVMaker/Misc/Configure.h>
#include <FVMaker/Grid/GridAnalysis/GridAnalyser.h>

GRID_NAMESPACE_OPEN   // abre namespace fvm { namespace grd {

/**
 * @brief Analisa o tamanho das células (Δx) em malhas 1D.
 *
 * Obtém Δx diretamente de `grid.DFaceCoordinate()`,
 * sem recalcular diferenças de faces.
 *
 * - Δx_min = min_i Δx_i  
 * - Δx_max = max_i Δx_i  
 * - Δx_avg = (1/N) ∑_i Δx_i
 *
 * @tparam GridType tipo da malha (ex.: UniformGrid1D)
 */
template <typename GridType>
class CellSizeAnalyser1D : public GridAnalyser<GridType>
{
public:
    explicit CellSizeAnalyser1D(const GridType& grid)
        : GridAnalyser<GridType>(grid),
          dx_min_(0.0), dx_max_(0.0), dx_avg_(0.0) {}

    /// Executa a análise do vetor Δx fornecido pela malha
    void analyse() override
    {
        const auto& dx = this->grid_.DFaceCoordinate();  // já pronto
        if (dx.empty()) return;

        dx_min_ = *std::min_element(dx.begin(), dx.end());
        dx_max_ = *std::max_element(dx.begin(), dx.end());
        dx_avg_ = std::accumulate(dx.begin(), dx.end(), Real(0.0)) / dx.size();
    }

    /// Impressão formatada — usado pelo operator<< da base
    void print(std::ostream& os) const override
    {
        os << std::fixed << std::setprecision(6)
           << "Cell Size Analysis (1D Grid)\n"
           << " - Min Δx : " << dx_min_ << '\n'
           << " - Max Δx : " << dx_max_ << '\n'
           << " - Avg Δx : " << dx_avg_;
    }

private:
    Real dx_min_, dx_max_, dx_avg_;
};

GRID_NAMESPACE_CLOSE  // fecha }
