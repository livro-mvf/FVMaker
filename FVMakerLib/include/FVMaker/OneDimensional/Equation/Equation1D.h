// ============================================================================
// Arquivo: Equation1D.h
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Declara Equation 1 D no contexto de OneDimensional / Equation.
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
#include <FVMaker/OneDimensional/Boundary/BoundaryCondition1D.h>
#include <FVMaker/OneDimensional/Grid/GridView1D.h>
#include <FVMaker/OneDimensional/Operator/Laplacian1D.h>
#include <FVMaker/OneDimensional/Source/LinearizedSource1D.h>

namespace fvm {

// Representa uma equacao 1D formada por malha, operador, fonte e contornos.
class Equation1D final {
public:
    // Cria um objeto Equation1D com os dados fornecidos.
    Equation1D(
        GridView1D grid,
        Laplacian1D laplacian,
        LinearizedSource1D source,
        BoundarySet1D boundaries
    );

    // Retorna o identificador estavel desta classe na biblioteca.
    [[nodiscard]] static constexpr ID id() noexcept {
        return ID{
            "OneDimensional",
            "Equation1D",
            "fvm.1d.equation.Equation1D"
        };
    }

    // Retorna o nome curto da classe para diagnostico e documentacao.
    [[nodiscard]] static constexpr std::string_view class_name() noexcept {
        return id().class_name();
    }

    // Retorna o identificador completo da classe na hierarquia da biblioteca.
    [[nodiscard]] static constexpr std::string_view class_id() noexcept {
        return id().class_id();
    }

    // Retorna a informacao grid associada ao objeto.
    [[nodiscard]] const GridView1D& grid() const noexcept;
    // Retorna a informacao laplacian associada ao objeto.
    [[nodiscard]] const Laplacian1D& laplacian() const noexcept;
    // Retorna a informacao source associada ao objeto.
    [[nodiscard]] const LinearizedSource1D& source() const noexcept;
    // Retorna a informacao boundaries associada ao objeto.
    [[nodiscard]] const BoundarySet1D& boundaries() const noexcept;

private:
    GridView1D grid_;
    Laplacian1D laplacian_;
    LinearizedSource1D source_;
    BoundarySet1D boundaries_;

    // Verifica se as hipoteses numericas e estruturais foram atendidas.
    void validate() const;
};

}  // namespace fvm
