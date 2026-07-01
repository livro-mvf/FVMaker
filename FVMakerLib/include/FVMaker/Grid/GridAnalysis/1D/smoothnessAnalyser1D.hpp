// ============================================================================
// Arquivo: smoothnessAnalyser1D.hpp
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Declara smoothness Analyser 1 D no contexto de Grid / GridAnalysis / 1D.
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
#include <vector>
#include <algorithm>       // std::max
#include <cmath>           // std::abs
#include <numeric>         // std::transform_reduce
#include <execution>       // std::execution::par
#include <iomanip>         // std::setprecision

// -----------------------------------------------------------------------------
// INCLUDES – FVMaker
// -----------------------------------------------------------------------------
#include <FVMaker/Misc/namespace.h>
#include <FVMaker/Misc/configure.h>
#include <FVMaker/Grid/GridAnalysis/gridAnalyser.h>

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
class SmoothnessAnalyser1D {
public:
    explicit SmoothnessAnalyser1D(const GridType& _grid)
        : GridAnalyser<GridType>(_grid), smoothness_(Real(0)) {}

    /// Threshold de volumes acima do qual usar paralelismo
    static constexpr std::size_t ParallelThreshold = 10000;

    /// Executa a escolha entre análise sequencial ou paralela
    void analyse() {
        const auto& dx = this->grid.GetDFace();
        const std::size_t N = dx.size();
        if (N < 2) {
            smoothness_ = Real(0);
            return;
        }
        if (this->grid.NVol() > ParallelThreshold) {
            analyseParallel(dx);
        } else {
            analyseSequential(dx);
        }
    }

    /// Impressão formatada – usado pelo operator<<.
    void print(std::ostream& os) const {
        os << std::fixed << std::setprecision(6)
           << "Analise de suavidade (1D Grid): "
           << " Maxima variacao de Δx: " << smoothness_;
    }

private:
    /// Cálculo sequencial da suavidade
    void analyseSequential(const VecReal& dx) {
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
    void analyseParallel(const VecReal& dx) {
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
