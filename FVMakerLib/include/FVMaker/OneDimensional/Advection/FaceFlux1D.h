// ============================================================================
// Arquivo: FaceFlux1D.h
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Declara Face Flux 1 D no contexto de OneDimensional / Advection.
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

#include <span>
#include <string_view>

#include <FVMaker/Algebra/DenseVector.h>
#include <FVMaker/Core/ID.h>
#include <FVMaker/Core/Types.h>
#include <FVMaker/OneDimensional/Grid/GridView1D.h>

namespace fvm {

class FaceFlux1D final {
public:
    explicit FaceFlux1D(DenseVector values);

    [[nodiscard]] static constexpr ID id() noexcept {
        return ID{"OneDimensional", "FaceFlux1D", "fvm.1d.advection.FaceFlux1D"};
    }

    [[nodiscard]] static constexpr std::string_view class_name() noexcept {
        return id().class_name();
    }

    [[nodiscard]] static constexpr std::string_view class_id() noexcept {
        return id().class_id();
    }

    [[nodiscard]] Size size() const noexcept;
    [[nodiscard]] std::span<const Real> values() const noexcept;
    [[nodiscard]] Real value(Size face) const noexcept;

private:
    DenseVector values_;

    void validate() const;
};

using MassFlux1D = FaceFlux1D;

[[nodiscard]] FaceFlux1D uniform_face_flux_1d(
    const GridView1D& grid,
    Real value
);

[[nodiscard]] FaceFlux1D vector_face_flux_1d(
    const GridView1D& grid,
    DenseVector values
);

[[nodiscard]] Real face_peclet_1d(Real mass_flux, Real diffusion);

}  // namespace fvm
