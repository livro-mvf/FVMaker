// ============================================================================
// Arquivo: PentadiagonalSystem1D.h
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Declara Pentadiagonal System 1 D no contexto de OneDimensional / System.
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
#include <vector>

#include <FVMaker/Algebra/DenseVector.h>
#include <FVMaker/Core/ID.h>
#include <FVMaker/Core/Types.h>

namespace fvm {

class PentadiagonalSystem1D final {
public:
    explicit PentadiagonalSystem1D(Size size);

    PentadiagonalSystem1D(
        std::vector<Real> lower_second,
        std::vector<Real> lower_first,
        std::vector<Real> diagonal,
        std::vector<Real> upper_first,
        std::vector<Real> upper_second,
        DenseVector rhs
    );

    [[nodiscard]] static constexpr ID id() noexcept {
        return ID{
            "OneDimensional",
            "PentadiagonalSystem1D",
            "fvm.1d.system.PentadiagonalSystem1D"
        };
    }

    [[nodiscard]] static constexpr std::string_view class_name() noexcept {
        return id().class_name();
    }

    [[nodiscard]] static constexpr std::string_view class_id() noexcept {
        return id().class_id();
    }

    [[nodiscard]] Size size() const noexcept;

    [[nodiscard]] std::span<Real> lower_second() noexcept;
    [[nodiscard]] std::span<const Real> lower_second() const noexcept;

    [[nodiscard]] std::span<Real> lower_first() noexcept;
    [[nodiscard]] std::span<const Real> lower_first() const noexcept;

    [[nodiscard]] std::span<Real> diagonal() noexcept;
    [[nodiscard]] std::span<const Real> diagonal() const noexcept;

    [[nodiscard]] std::span<Real> upper_first() noexcept;
    [[nodiscard]] std::span<const Real> upper_first() const noexcept;

    [[nodiscard]] std::span<Real> upper_second() noexcept;
    [[nodiscard]] std::span<const Real> upper_second() const noexcept;

    [[nodiscard]] DenseVector& rhs() noexcept;
    [[nodiscard]] const DenseVector& rhs() const noexcept;

    void set_row(
        Size row,
        Real lower_second,
        Real lower_first,
        Real diagonal,
        Real upper_first,
        Real upper_second,
        Real rhs
    );

private:
    std::vector<Real> lower_second_;
    std::vector<Real> lower_first_;
    std::vector<Real> diagonal_;
    std::vector<Real> upper_first_;
    std::vector<Real> upper_second_;
    DenseVector rhs_;

    void validate() const;
};

}  // namespace fvm
