// ============================================================================
// Arquivo: uniformityAnalyser1D.hpp
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Declara uniformity Analyser 1 D no contexto de Grid / GridAnalysis / 1D.
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
#include <FVMaker/Misc/namespace.h>
#include <FVMaker/Misc/configure.h>
#include <FVMaker/Grid/GridAnalysis/gridAnalyser.h>

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
class UniformityAnalyser1D
{
public:
    explicit UniformityAnalyser1D(const GridType& _grid)
        : GridAnalyser<GridType>(_grid) {}

    /* --------------------------------------------------------------------- */
    /** @brief Executa a análise (cálculo do CV). */
    void analyse()
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
    void print(std::ostream& os) const
    {
        os << std::fixed << std::setprecision(6)
           << "Analise de uniformidade (1D Grid): "
           << "Coeficiente de variacao (Δx): " << cv_;
    }

private:
    Real cv_{0.0};   ///< Coeficiente de variação das larguras de célula
};

GRID_NAMESPACE_CLOSE       // } }
