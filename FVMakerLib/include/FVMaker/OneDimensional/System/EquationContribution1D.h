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

// Armazena coeficientes algebricos produzidos por termos de uma equacao 1D.
class EquationContribution1D final {
public:
    // Cria um objeto EquationContribution1D com os dados fornecidos.
    explicit EquationContribution1D(Size size);

    // Retorna o identificador estavel desta classe na biblioteca.
    [[nodiscard]] static constexpr ID id() noexcept {
        return ID{
            "OneDimensional",
            "EquationContribution1D",
            "fvm.1d.system.EquationContribution1D"
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

    // Realiza a operacao aw definida por esta interface.
    [[nodiscard]] DenseVector& aw() noexcept;
    // Retorna a informacao aw associada ao objeto.
    [[nodiscard]] const DenseVector& aw() const noexcept;

    // Realiza a operacao ap definida por esta interface.
    [[nodiscard]] DenseVector& ap() noexcept;
    // Retorna a informacao ap associada ao objeto.
    [[nodiscard]] const DenseVector& ap() const noexcept;

    // Realiza a operacao ae definida por esta interface.
    [[nodiscard]] DenseVector& ae() noexcept;
    // Retorna a informacao ae associada ao objeto.
    [[nodiscard]] const DenseVector& ae() const noexcept;

    // Realiza a operacao bp definida por esta interface.
    [[nodiscard]] DenseVector& bp() noexcept;
    // Retorna a informacao bp associada ao objeto.
    [[nodiscard]] const DenseVector& bp() const noexcept;

    EquationContribution1D& operator+=(const EquationContribution1D& other);

private:
    DenseVector aw_;
    DenseVector ap_;
    DenseVector ae_;
    DenseVector bp_;

    // Verifica se as hipoteses numericas e estruturais foram atendidas.
    void validate() const;
};

// Define o comportamento do operador usado por esta abstracao.
[[nodiscard]] EquationContribution1D operator+(
    EquationContribution1D lhs,
    const EquationContribution1D& rhs
);

// Realiza a operacao to tridiagonal system definida por esta interface.
[[nodiscard]] TridiagonalSystem1D to_tridiagonal_system(
    const EquationContribution1D& contribution
);

// Escreve uma representacao textual do objeto no fluxo de saida.
std::ostream& operator<<(
    std::ostream& os,
    const EquationContribution1D& contribution
);

}  // namespace fvm
