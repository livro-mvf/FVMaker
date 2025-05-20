//==============================================================================
// Nome        : UniformityAnalyser1D.hpp
// Autor       : João Flávio Vieira de Vasconcellos
// Versão      : 2.0
// Descrição   : Mede a uniformidade de malhas 1D.  Calcula o coeficiente de
//               variação (CV) dos comprimentos de célula Δx obtidos diretamente
//               do vetor dxFace_ fornecido pela malha 1D.
//               • CV = σ(Δx) / média(Δx)
// Licença     : GNU GPL v3
//==============================================================================

#pragma once

// -----------------------------------------------------------------------------
// INCLUDES – C++
// -----------------------------------------------------------------------------
#include <numeric>           // std::accumulate
#include <algorithm>         // std::min/max_element
#include <cmath>             // std::sqrt
#include <iomanip>           // std::setprecision

// -----------------------------------------------------------------------------
// INCLUDES – FVMaker
// -----------------------------------------------------------------------------
#include <FVMaker/Misc/Namespace.h>
#include <FVMaker/Misc/Configure.h>
#include <FVMaker/Grid/GridAnalysis/GridAnalyser.h>

GRID_NAMESPACE_OPEN        // => namespace fvm { namespace grd {

/**
 * @brief Analisa a uniformidade de uma malha 1D.
 *
 * Usa o vetor Δx (faces) que a malha fornece por `DFaceCoordinate()`
 * e calcula o coeficiente de variação:
 *
 *     CV = σ(Δx) / média(Δx) ,
 *     onde σ é o desvio padrão amostral.
 *
 * @tparam GridType tipo da malha (ex.: UniformGrid1D)
 */
template <typename GridType>
class UniformityAnalyser1D : public GridAnalyser<GridType>
{
public:
    explicit UniformityAnalyser1D(const GridType& _grid)
        : GridAnalyser<GridType>(_grid) {}

    /* --------------------------------------------------------------------- */
    /** @brief Executa a análise (cálculo do CV). */
    void analyse() override
    {
        const auto& dx = this->grid.GetDFace();   // vetor Δx pronto
        const std::size_t N = dx.size();
        if (N == 0) { cv_ = 0.0; return; }

        // média
        const Real mean = std::accumulate(dx.begin(), dx.end(), Real(0.0)) / N;
        if (mean == Real(0.0)) { cv_ = 0.0; return; }

        // desvio padrão
        Real sq_sum = 0.0;
        for (const auto& d : dx)
            sq_sum += (d - mean) * (d - mean);

        const Real std_dev = std::sqrt(sq_sum / N);
        cv_ = std_dev / mean;
    }

    /* --------------------------------------------------------------------- */
    /** @brief Saída formatada (usado por operator<<). */
    void print(std::ostream& os) const override
    {
        os << std::fixed << std::setprecision(6)
           << "Analise de uniformidade (1D Grid): "
           << "Coeficiente de variacao (Δx): " << cv_;
    }

private:
    Real cv_{0.0};   ///< Coeficiente de variação das larguras de célula
};

GRID_NAMESPACE_CLOSE       // } }
