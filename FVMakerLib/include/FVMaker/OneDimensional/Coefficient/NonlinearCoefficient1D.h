// ============================================================================
// Arquivo: NonlinearCoefficient1D.h
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Declara Nonlinear Coefficient 1 D no contexto de OneDimensional / Coefficient.
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

#include <functional>
#include <string_view>

#include <FVMaker/Algebra/DenseVector.h>
#include <FVMaker/Core/ID.h>
#include <FVMaker/Core/Types.h>
#include <FVMaker/OneDimensional/Coefficient/DiffusionCoefficient1D.h>
#include <FVMaker/OneDimensional/Grid/GridView1D.h>

namespace fvm {

// Representa coeficientes que podem depender de x e da propria solucao.
class NonlinearCoefficient1D final {
public:
    using Function = std::function<Real(Real phi, Real x)>;

    // Cria um objeto NonlinearCoefficient1D com os dados fornecidos.
    explicit NonlinearCoefficient1D(Function function);

    // Retorna o identificador estavel desta classe na biblioteca.
    [[nodiscard]] static constexpr ID id() noexcept {
        return ID{
            "OneDimensional",
            "NonlinearCoefficient1D",
            "fvm.1d.coefficient.NonlinearCoefficient1D"
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

    // Realiza a operacao cell values definida por esta interface.
    [[nodiscard]] DenseVector cell_values(
        const GridView1D& grid,
        const DenseVector& phi
    ) const;

    // Realiza a operacao arithmetic face values definida por esta interface.
    [[nodiscard]] DiffusionCoefficient1D arithmetic_face_values(
        const GridView1D& grid,
        const DenseVector& phi
    ) const;

    // Realiza a operacao harmonic face values definida por esta interface.
    [[nodiscard]] DiffusionCoefficient1D harmonic_face_values(
        const GridView1D& grid,
        const DenseVector& phi
    ) const;

private:
    Function function_;
};

// Realiza a operacao nonlinear coefficient 1d definida por esta interface.
template <class Function>
[[nodiscard]] NonlinearCoefficient1D nonlinear_coefficient_1d(
    Function function
) {
    return NonlinearCoefficient1D{
        NonlinearCoefficient1D::Function{function}
    };
}

}  // namespace fvm
