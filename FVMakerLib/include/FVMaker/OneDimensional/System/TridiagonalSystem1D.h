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

// Representa um sistema linear tridiagonal gerado por discretizacoes 1D.
class TridiagonalSystem1D final {
public:
    // Cria um objeto TridiagonalSystem1D com os dados fornecidos.
    explicit TridiagonalSystem1D(Size size);

    // Cria um objeto TridiagonalSystem1D com os dados fornecidos.
    TridiagonalSystem1D(
        std::vector<Real> lower,
        std::vector<Real> diagonal,
        std::vector<Real> upper,
        DenseVector rhs
    );

    // Retorna o identificador estavel desta classe na biblioteca.
    [[nodiscard]] static constexpr ID id() noexcept {
        return ID{
            "OneDimensional",
            "TridiagonalSystem1D",
            "fvm.1d.system.TridiagonalSystem1D"
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

    // Retorna a informacao size associada ao objeto.
    [[nodiscard]] Size size() const noexcept;

    // Retorna a informacao lower armazenada no objeto.
    [[nodiscard]] std::span<Real> lower() noexcept;
    // Retorna a informacao lower armazenada no objeto.
    [[nodiscard]] std::span<const Real> lower() const noexcept;

    // Retorna a informacao diagonal armazenada no objeto.
    [[nodiscard]] std::span<Real> diagonal() noexcept;
    // Retorna a informacao diagonal armazenada no objeto.
    [[nodiscard]] std::span<const Real> diagonal() const noexcept;

    // Retorna a informacao upper armazenada no objeto.
    [[nodiscard]] std::span<Real> upper() noexcept;
    // Retorna a informacao upper armazenada no objeto.
    [[nodiscard]] std::span<const Real> upper() const noexcept;

    // Realiza a operacao rhs definida por esta interface.
    [[nodiscard]] DenseVector& rhs() noexcept;
    // Retorna a informacao rhs associada ao objeto.
    [[nodiscard]] const DenseVector& rhs() const noexcept;

    // Define a informacao set row usada pelo objeto.
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

    // Verifica se as hipoteses numericas e estruturais foram atendidas.
    void validate() const;
};

// Realiza a operacao multiply definida por esta interface.
void multiply(
    const TridiagonalSystem1D& system,
    const DenseVector& x,
    DenseVector& y
);

// Realiza a operacao multiply transpose definida por esta interface.
void multiply_transpose(
    const TridiagonalSystem1D& system,
    const DenseVector& x,
    DenseVector& y
);

// Realiza a operacao multiply definida por esta interface.
[[nodiscard]] DenseVector multiply(
    const TridiagonalSystem1D& system,
    const DenseVector& x
);

// Realiza a operacao multiply transpose definida por esta interface.
[[nodiscard]] DenseVector multiply_transpose(
    const TridiagonalSystem1D& system,
    const DenseVector& x
);

// Define o comportamento do operador usado por esta abstracao.
[[nodiscard]] DenseVector operator*(
    const TridiagonalSystem1D& system,
    const DenseVector& x
);

}  // namespace fvm
