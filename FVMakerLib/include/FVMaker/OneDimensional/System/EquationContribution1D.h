// ============================================================================
// Arquivo: EquationContribution1D.h
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Declara Equation Contribution 1 D no contexto de OneDimensional / System.
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

#include <iosfwd>
#include <string_view>

#include <FVMaker/Algebra/DenseVector.h>
#include <FVMaker/Core/ID.h>
#include <FVMaker/Core/Types.h>
#include <FVMaker/OneDimensional/System/TridiagonalSystem1D.h>

namespace fvm {

class EquationContribution1D final {
public:
    explicit EquationContribution1D(Size size);

    [[nodiscard]] static constexpr ID id() noexcept {
        return ID{
            "OneDimensional",
            "EquationContribution1D",
            "fvm.1d.system.EquationContribution1D"
        };
    }

    [[nodiscard]] static constexpr std::string_view class_name() noexcept {
        return id().class_name();
    }

    [[nodiscard]] static constexpr std::string_view class_id() noexcept {
        return id().class_id();
    }

    [[nodiscard]] Size size() const noexcept;

    [[nodiscard]] DenseVector& aw() noexcept;
    [[nodiscard]] const DenseVector& aw() const noexcept;

    [[nodiscard]] DenseVector& ap() noexcept;
    [[nodiscard]] const DenseVector& ap() const noexcept;

    [[nodiscard]] DenseVector& ae() noexcept;
    [[nodiscard]] const DenseVector& ae() const noexcept;

    [[nodiscard]] DenseVector& bp() noexcept;
    [[nodiscard]] const DenseVector& bp() const noexcept;

    EquationContribution1D& operator+=(const EquationContribution1D& other);

private:
    DenseVector aw_;
    DenseVector ap_;
    DenseVector ae_;
    DenseVector bp_;

    void validate() const;
};

[[nodiscard]] EquationContribution1D operator+(
    EquationContribution1D lhs,
    const EquationContribution1D& rhs
);

[[nodiscard]] TridiagonalSystem1D to_tridiagonal_system(
    const EquationContribution1D& contribution
);

std::ostream& operator<<(
    std::ostream& os,
    const EquationContribution1D& contribution
);

}  // namespace fvm
