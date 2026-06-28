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

class Equation1D final {
public:
    Equation1D(
        GridView1D grid,
        Laplacian1D laplacian,
        LinearizedSource1D source,
        BoundarySet1D boundaries
    );

    [[nodiscard]] static constexpr ID id() noexcept {
        return ID{
            "OneDimensional",
            "Equation1D",
            "fvm.1d.equation.Equation1D"
        };
    }

    [[nodiscard]] static constexpr std::string_view class_name() noexcept {
        return id().class_name();
    }

    [[nodiscard]] static constexpr std::string_view class_id() noexcept {
        return id().class_id();
    }

    [[nodiscard]] const GridView1D& grid() const noexcept;
    [[nodiscard]] const Laplacian1D& laplacian() const noexcept;
    [[nodiscard]] const LinearizedSource1D& source() const noexcept;
    [[nodiscard]] const BoundarySet1D& boundaries() const noexcept;

private:
    GridView1D grid_;
    Laplacian1D laplacian_;
    LinearizedSource1D source_;
    BoundarySet1D boundaries_;

    void validate() const;
};

}  // namespace fvm
