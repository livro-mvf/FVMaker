// ============================================================================
// Arquivo: GridView1D.h
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Declara Grid View 1 D no contexto de OneDimensional / Grid.
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

// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <span>
#include <string_view>

// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/OneDimensional/Axis1D/Axis1D.h>

// ----------------------------------------------------------------------------
// FVMaker includes
// ----------------------------------------------------------------------------
#include <FVMaker/Core/ID.h>
#include <FVMaker/Core/Types.h>
#include <FVMaker/OneDimensional/Grid/GridMetrics1D.h>

namespace fvm {

class GridView1D final {
public:
    explicit GridView1D(const fvgrid::Axis1D& axis);

    explicit GridView1D(const fvgrid::Axis1D* axis);

    [[nodiscard]] static constexpr ID id() noexcept {
        return ID{
            "OneDimensional",
            "GridView1D",
            "fvm.1d.grid.GridView1D"
        };
    }

    [[nodiscard]] static constexpr std::string_view class_name() noexcept {
        return id().class_name();
    }

    [[nodiscard]] static constexpr std::string_view class_id() noexcept {
        return id().class_id();
    }

    [[nodiscard]] Size num_volumes() const noexcept;
    [[nodiscard]] Size num_cells() const noexcept;
    [[nodiscard]] Size num_faces() const noexcept;

    [[nodiscard]] std::span<const Real> faces() const noexcept;
    [[nodiscard]] std::span<const Real> centers() const noexcept;
    [[nodiscard]] std::span<const Real> dx_faces() const noexcept;
    [[nodiscard]] std::span<const Real> dx_centers() const noexcept;
    [[nodiscard]] std::span<const Real> cell_lengths() const noexcept;

    [[nodiscard]] Real xmin() const noexcept;
    [[nodiscard]] Real xmax() const noexcept;
    [[nodiscard]] Real length() const noexcept;

    [[nodiscard]] std::string_view pattern_name() const noexcept;
    [[nodiscard]] GridMetrics1D metrics() const noexcept;

private:
    const fvgrid::Axis1D* axis_{nullptr};

    void validate() const;
};

}  // namespace fvm
