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

//==============================================================================
// Includes da biblioteca padrão do C++
//==============================================================================

#include <algorithm>     // std::min_element / std::max_element
#include <numeric>       // std::accumulate
#include <iomanip>       // std::setprecision

//==============================================================================
// Includes da biblioteca FVMaker
//==============================================================================

#include <FVMaker/Misc/Configure.h>
#include <FVMaker/Misc/Namespace.h>
#include <FVMaker/Grid/GridAnalysis/GridAnalyser.h>

GRID_NAMESPACE_OPEN   
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

explicit 
CellSizeAnalyser1D (const GridType& _grid) :  
        GridAnalyser<GridType>  (_grid)
        ,   dx_min_(0.0)
        ,   dx_max_(0.0)
        ,   dx_avg_(0.0) {}

    /// Executa a análise do vetor Delta x fornecido pela malha
void analyse() override {
        const auto& dx = this->grid.GetDFace(); 
        if (dx.empty()) return;

        dx_min_ = *std::min_element(dx.begin(), dx.end());
        dx_max_ = *std::max_element(dx.begin(), dx.end());
        dx_avg_ = std::accumulate(dx.begin(), dx.end(), Real(0.0)) / dx.size();
}

    /// Impressão formatada — usado pelo operator<< da base
    void print(std::ostream& _os) const override
    {
        _os << std::fixed << std::setprecision(6)
           << "Analise de tamanho do volume (1D Grid): "
           << " - Min Δx : " << dx_min_ 
           << " - Max Δx : " << dx_max_ 
           << " - Avg Δx : " << dx_avg_;
    }

private:

    Real    dx_min_ = 0.0, 
            dx_max_ = 0.0, 
            dx_avg_ = 0.0;
};

GRID_NAMESPACE_CLOSE  // fecha }
