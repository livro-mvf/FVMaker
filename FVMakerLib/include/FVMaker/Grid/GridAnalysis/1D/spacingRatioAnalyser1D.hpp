// ============================================================================
// Arquivo: spacingRatioAnalyser1D.hpp
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Declara spacing Ratio Analyser 1 D no contexto de Grid / GridAnalysis / 1D.
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
