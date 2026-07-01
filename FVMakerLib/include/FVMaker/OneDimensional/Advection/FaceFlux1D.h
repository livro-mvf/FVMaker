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

// Armazena fluxos nas faces usados por termos advectivos em 1D.
class FaceFlux1D final {
public:
    // Cria um objeto FaceFlux1D com os dados fornecidos.
    explicit FaceFlux1D(DenseVector values);

    // Retorna o identificador estavel desta classe na biblioteca.
    [[nodiscard]] static constexpr ID id() noexcept {
        return ID{"OneDimensional", "FaceFlux1D", "fvm.1d.advection.FaceFlux1D"};
    }

    // Retorna o nome curto da classe para diagnostico e documentacao.
    [[nodiscard]] static constexpr std::string_view class_name() noexcept {
        return id().class_name();
    }

    // Retorna o identificador completo da classe na hierarquia da biblioteca.
    [[nodiscard]] static constexpr std::string_view class_id() noexcept {
        return id().class_id();
    }

    // Retorna a informacao size associada ao objeto.
    [[nodiscard]] Size size() const noexcept;
    // Retorna a informacao values armazenada no objeto.
    [[nodiscard]] std::span<const Real> values() const noexcept;
    // Retorna a informacao value armazenada no objeto.
    [[nodiscard]] Real value(Size face) const noexcept;

private:
    DenseVector values_;

    // Verifica se as hipoteses numericas e estruturais foram atendidas.
    void validate() const;
};

using MassFlux1D = FaceFlux1D;

// Realiza a operacao uniform face flux 1d definida por esta interface.
[[nodiscard]] FaceFlux1D uniform_face_flux_1d(
    const GridView1D& grid,
    Real value
);

// Realiza a operacao vector face flux 1d definida por esta interface.
[[nodiscard]] FaceFlux1D vector_face_flux_1d(
    const GridView1D& grid,
    DenseVector values
);

// Realiza a operacao face peclet 1d definida por esta interface.
[[nodiscard]] Real face_peclet_1d(Real mass_flux, Real diffusion);

}  // namespace fvm
