// ============================================================================
// Arquivo: Advection1D.h
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Declara Advection 1 D no contexto de OneDimensional / Advection.
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

#include <string_view>

#include <FVMaker/Core/ID.h>
#include <FVMaker/OneDimensional/Advection/FaceFlux1D.h>
#include <FVMaker/OneDimensional/Advection/Interpolation1D.h>

namespace fvm {

// Representa a contribuicao advectiva em uma equacao unidimensional.
class Advection1D final {
public:
    // Cria um objeto Advection1D com os dados fornecidos.
    explicit Advection1D(FaceFlux1D face_flux);

    // Retorna o identificador estavel desta classe na biblioteca.
    [[nodiscard]] static constexpr ID id() noexcept {
        return ID{"OneDimensional", "Advection1D", "fvm.1d.advection.Advection1D"};
    }

    // Retorna o nome curto da classe para diagnostico e documentacao.
    [[nodiscard]] static constexpr std::string_view class_name() noexcept {
        return id().class_name();
    }

    // Retorna o identificador completo da classe na hierarquia da biblioteca.
    [[nodiscard]] static constexpr std::string_view class_id() noexcept {
        return id().class_id();
    }

    // Retorna a informacao face flux associada ao objeto.
    [[nodiscard]] const FaceFlux1D& face_flux() const noexcept;

private:
    FaceFlux1D face_flux_;
};

// Realiza a operacao advective face values 1d definida por esta interface.
template <class Scheme>
[[nodiscard]] DenseVector advective_face_values_1d(
    const GridView1D& grid,
    const DenseVector& cell_values,
    const Advection1D& advection,
    Scheme scheme
) {
    return interpolate_faces_1d(
        grid,
        cell_values,
        advection.face_flux(),
        scheme
    );
}

}  // namespace fvm
