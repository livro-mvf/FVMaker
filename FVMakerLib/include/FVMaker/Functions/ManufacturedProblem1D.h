// ============================================================================
// Arquivo: ManufacturedProblem1D.h
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Declara Manufactured Problem 1 D no contexto de Functions.
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
#include <string>
#include <string_view>
#include <utility>
#include <vector>

// ----------------------------------------------------------------------------
// FVMaker includes
// ----------------------------------------------------------------------------
#include <FVMaker/ErrorHandling/ErrorCatalog.h>
#include <FVMaker/ErrorHandling/ThrowError.h>
#include <FVMaker/Functions/ManufacturedSolution1D.h>
#include <FVMaker/Functions/VariableCoefficient1D.h>

namespace fvm {

// Representa o conceito de named coefficient1 d dentro da biblioteca FVMaker.
struct NamedCoefficient1D final {
    std::string name;
    VariableCoefficient1D coefficient;
};

// Agrupa solucao manufaturada, coeficiente e fonte para testes 1D.
class ManufacturedProblem1D final {
public:
    // Cria um objeto ManufacturedProblem1D com os dados fornecidos.
    explicit ManufacturedProblem1D(ManufacturedSolution1D solution)
        : solution_(std::move(solution)) {}

    // Cria um objeto ManufacturedProblem1D com os dados fornecidos.
    ManufacturedProblem1D(
        ManufacturedSolution1D solution,
        std::vector<NamedCoefficient1D> coefficients
    )
        : solution_(std::move(solution)),
          coefficients_(std::move(coefficients)) {}

    // Retorna a informacao solution associada ao objeto.
    [[nodiscard]] const ManufacturedSolution1D& solution() const noexcept {
        return solution_;
    }

    // Retorna a informacao coefficients associada ao objeto.
    [[nodiscard]] const std::vector<NamedCoefficient1D>& coefficients() const noexcept {
        return coefficients_;
    }

    // Retorna a informacao coefficient count associada ao objeto.
    [[nodiscard]] Size coefficient_count() const noexcept {
        return coefficients_.size();
    }

    // Acrescenta a informacao add coefficient ao objeto.
    void add_coefficient(std::string name, VariableCoefficient1D coefficient) {
        coefficients_.push_back(
            NamedCoefficient1D{
                .name = std::move(name),
                .coefficient = std::move(coefficient),
            }
        );
    }

    // Realiza a operacao coefficient definida por esta interface.
    [[nodiscard]] const VariableCoefficient1D& coefficient(
        std::string_view name
    ) const {
        for (const NamedCoefficient1D& item : coefficients_) {
            if (item.name == name) {
                return item.coefficient;
            }
        }

        throw_error(
            error_catalog::kInvalidCoefficient,
            ID{
                "Functions",
                "ManufacturedProblem1D",
                "fvm.functions.ManufacturedProblem1D"
            }
        );
    }

private:
    ManufacturedSolution1D solution_;
    std::vector<NamedCoefficient1D> coefficients_;
};

}  // namespace fvm
