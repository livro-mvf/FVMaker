// ============================================================================
// Arquivo: TridiagonalSystem1D.h
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Declara Tridiagonal System 1 D no contexto de OneDimensional / System.
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
#include <vector>

// ----------------------------------------------------------------------------
// FVMaker includes
// ----------------------------------------------------------------------------
#include <FVMaker/Algebra/DenseVector.h>
#include <FVMaker/Core/ID.h>
#include <FVMaker/Core/Types.h>

namespace fvm {

class TridiagonalSystem1D final {
public:
    explicit TridiagonalSystem1D(Size size);

    TridiagonalSystem1D(
        std::vector<Real> lower,
        std::vector<Real> diagonal,
        std::vector<Real> upper,
        DenseVector rhs
    );

    [[nodiscard]] static constexpr ID id() noexcept {
        return ID{
            "OneDimensional",
            "TridiagonalSystem1D",
            "fvm.1d.system.TridiagonalSystem1D"
        };
    }

    [[nodiscard]] static constexpr std::string_view class_name() noexcept {
        return id().class_name();
    }

    [[nodiscard]] static constexpr std::string_view class_id() noexcept {
        return id().class_id();
    }

    [[nodiscard]] Size size() const noexcept;

    [[nodiscard]] std::span<Real> lower() noexcept;
    [[nodiscard]] std::span<const Real> lower() const noexcept;

    [[nodiscard]] std::span<Real> diagonal() noexcept;
    [[nodiscard]] std::span<const Real> diagonal() const noexcept;

    [[nodiscard]] std::span<Real> upper() noexcept;
    [[nodiscard]] std::span<const Real> upper() const noexcept;

    [[nodiscard]] DenseVector& rhs() noexcept;
    [[nodiscard]] const DenseVector& rhs() const noexcept;

    void set_row(
        Size row,
        Real lower,
        Real diagonal,
        Real upper,
        Real rhs
    );

private:
    std::vector<Real> lower_;
    std::vector<Real> diagonal_;
    std::vector<Real> upper_;
    DenseVector rhs_;

    void validate() const;
};

void multiply(
    const TridiagonalSystem1D& system,
    const DenseVector& x,
    DenseVector& y
);

void multiply_transpose(
    const TridiagonalSystem1D& system,
    const DenseVector& x,
    DenseVector& y
);

[[nodiscard]] DenseVector multiply(
    const TridiagonalSystem1D& system,
    const DenseVector& x
);

[[nodiscard]] DenseVector multiply_transpose(
    const TridiagonalSystem1D& system,
    const DenseVector& x
);

[[nodiscard]] DenseVector operator*(
    const TridiagonalSystem1D& system,
    const DenseVector& x
);

}  // namespace fvm
