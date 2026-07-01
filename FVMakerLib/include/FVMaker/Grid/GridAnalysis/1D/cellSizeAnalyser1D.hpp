// ============================================================================
// Arquivo: cellSizeAnalyser1D.hpp
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Declara cell Size Analyser 1 D no contexto de Grid / GridAnalysis / 1D.
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

//==============================================================================
// Includes da biblioteca padrão do C++
//==============================================================================

#include <algorithm>     // std::min_element / std::max_element
#include <numeric>       // std::accumulate
#include <iomanip>       // std::setprecision

//==============================================================================
// Includes da biblioteca FVMaker
//==============================================================================

#include <FVMaker/Misc/configure.h>
#include <FVMaker/Misc/namespace.h>
#include <FVMaker/Grid/GridAnalysis/gridAnalyser.h>

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
class CellSizeAnalyser1D
{
public:

explicit 
// Realiza a operacao cell size analyser1 d definida por esta interface.
CellSizeAnalyser1D (const GridType& _grid) :  
        GridAnalyser<GridType>  (_grid)
        ,   dx_min_(0.0)
        ,   dx_max_(0.0)
        ,   dx_avg_(0.0) {}

    /// Executa a análise do vetor Delta x fornecido pela malha
void analyse() {
        const auto& dx = this->grid.GetDFace(); 
        if (dx.empty()) return;

        dx_min_ = *std::min_element(dx.begin(), dx.end());
        dx_max_ = *std::max_element(dx.begin(), dx.end());
        dx_avg_ = std::accumulate(dx.begin(), dx.end(), Real(0.0)) / dx.size();
}

    /// Impressão formatada — usado pelo operator<< da base
    void print(std::ostream& _os) const
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
