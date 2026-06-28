// ============================================================================
// Arquivo: Interpolation1D.h
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Declara Interpolation 1 D no contexto de OneDimensional / Advection.
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

#include <cmath>

#include <FVMaker/Algebra/DenseVector.h>
#include <FVMaker/Core/ID.h>
#include <FVMaker/Core/Types.h>
#include <FVMaker/ErrorHandling/ErrorCatalog.h>
#include <FVMaker/ErrorHandling/ThrowError.h>
#include <FVMaker/OneDimensional/Advection/FaceFlux1D.h>
#include <FVMaker/OneDimensional/Grid/GridView1D.h>

namespace fvm {

struct Interpolation1D final {
    [[nodiscard]] static constexpr ID id() noexcept {
        return ID{"OneDimensional", "Interpolation1D", "fvm.1d.advection.Interpolation1D"};
    }
};

struct Upwind1D final {};
struct CentralDifference1D final {};
struct PowerLaw1D final {};
struct Exponential1D final {};
struct QUICK1D final {};

[[nodiscard]] inline Real central_face_value_1d(Real west, Real east) {
    return Real{0.5} * (west + east);
}

[[nodiscard]] inline Real upwind_face_value_1d(
    Real west,
    Real east,
    Real face_flux
) {
    return face_flux >= Real{} ? west : east;
}

[[nodiscard]] inline Real interpolate_internal_face_value_1d(
    CentralDifference1D,
    Real west,
    Real east,
    Real
) {
    return central_face_value_1d(west, east);
}

[[nodiscard]] inline Real interpolate_internal_face_value_1d(
    Upwind1D,
    Real west,
    Real east,
    Real face_flux
) {
    return upwind_face_value_1d(west, east, face_flux);
}

[[nodiscard]] inline Real interpolate_internal_face_value_1d(
    PowerLaw1D,
    Real west,
    Real east,
    Real face_flux
) {
    return std::abs(face_flux) < Real{1.0}
        ? central_face_value_1d(west, east)
        : upwind_face_value_1d(west, east, face_flux);
}

[[nodiscard]] inline Real interpolate_internal_face_value_1d(
    Exponential1D,
    Real west,
    Real east,
    Real face_flux
) {
    return std::abs(face_flux) < Real{1.0}
        ? central_face_value_1d(west, east)
        : upwind_face_value_1d(west, east, face_flux);
}

[[nodiscard]] inline Real interpolate_internal_face_value_1d(
    QUICK1D,
    Real west,
    Real east,
    Real face_flux
) {
    return upwind_face_value_1d(west, east, face_flux);
}

template <class Scheme>
[[nodiscard]] DenseVector interpolate_faces_1d(
    const GridView1D& grid,
    const DenseVector& cell_values,
    const FaceFlux1D& face_flux,
    Scheme scheme
) {
    require(
        cell_values.size() == grid.num_volumes(),
        error_catalog::kInvalidFieldSize,
        Interpolation1D::id()
    );

    require(
        face_flux.size() == grid.num_faces(),
        error_catalog::kInvalidArgument,
        Interpolation1D::id()
    );

    DenseVector face_values{grid.num_faces()};
    face_values[0] = cell_values[0];
    face_values[grid.num_faces() - 1] = cell_values[grid.num_volumes() - 1];

    for (Size face = 1; face + 1 < grid.num_faces(); ++face) {
        face_values[face] = interpolate_internal_face_value_1d(
            scheme,
            cell_values[face - 1],
            cell_values[face],
            face_flux.value(face)
        );
    }

    return face_values;
}

}  // namespace fvm
