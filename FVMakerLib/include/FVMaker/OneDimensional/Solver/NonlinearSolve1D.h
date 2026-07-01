// ============================================================================
// Arquivo: NonlinearSolve1D.h
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Declara Nonlinear Solve 1 D no contexto de OneDimensional / Solver.
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
#include <utility>

#include <FVMaker/Algebra/DenseVector.h>
#include <FVMaker/Core/ID.h>
#include <FVMaker/Core/Types.h>
#include <FVMaker/ErrorHandling/ErrorCatalog.h>
#include <FVMaker/ErrorHandling/ThrowError.h>
#include <FVMaker/OneDimensional/Boundary/BoundaryCondition1D.h>
#include <FVMaker/OneDimensional/Coefficient/NonlinearCoefficient1D.h>
#include <FVMaker/OneDimensional/Equation/Equation1D.h>
#include <FVMaker/OneDimensional/Grid/GridView1D.h>
#include <FVMaker/OneDimensional/Operator/Laplacian1D.h>
#include <FVMaker/OneDimensional/Solver/SolveController1D.h>
#include <FVMaker/OneDimensional/Solver/TDMA.h>
#include <FVMaker/OneDimensional/Source/LinearizedSource1D.h>
#include <FVMaker/Solver/PicardIteration.h>
#include <FVMaker/Solver/SteadyState.h>

namespace fvm {

// Agrupa os objetos necessarios a uma iteracao de Picard em 1D.
class PicardProblem1D final {
public:
    // Cria um objeto PicardProblem1D com os dados fornecidos.
    PicardProblem1D(
        GridView1D grid,
        NonlinearCoefficient1D coefficient,
        LinearizedSource1D source,
        BoundarySet1D boundaries
    );

    // Retorna o identificador estavel desta classe na biblioteca.
    [[nodiscard]] static constexpr ID id() noexcept {
        return ID{
            "OneDimensional",
            "PicardProblem1D",
            "fvm.1d.solver.PicardProblem1D"
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
    // Retorna a informacao coefficient associada ao objeto.
    [[nodiscard]] const NonlinearCoefficient1D& coefficient() const noexcept;
    // Retorna a informacao source associada ao objeto.
    [[nodiscard]] const LinearizedSource1D& source() const noexcept;
    // Retorna a informacao boundaries associada ao objeto.
    [[nodiscard]] const BoundarySet1D& boundaries() const noexcept;

private:
    GridView1D grid_;
    NonlinearCoefficient1D coefficient_;
    LinearizedSource1D source_;
    BoundarySet1D boundaries_;

    // Verifica se as hipoteses numericas e estruturais foram atendidas.
    void validate() const;
};

// Realiza a operacao picard solve 1d definida por esta interface.
template <class LinearSolver = TDMA>
[[nodiscard]] PicardResult picard_solve_1d(
    const PicardProblem1D& problem,
    DenseVector initial_solution,
    const PicardOptions& picard_options = {},
    const SteadyState& linear_control = {},
    Real time = Real{}
) {
    // Realiza a operacao require definida por esta interface.
    require(
        initial_solution.size() == problem.grid().num_volumes(),
        error_catalog::kInvalidFieldSize,
        PicardProblem1D::id()
    );

    return picard_iteration(
        // Realiza a operacao move definida por esta interface.
        std::move(initial_solution),
        [&](const DenseVector& phi) {
            const DiffusionCoefficient1D face_coefficient =
                problem.coefficient().harmonic_face_values(problem.grid(), phi);

            const Equation1D linearized_equation{
                problem.grid(),
                Laplacian1D{face_coefficient},
                problem.source(),
                problem.boundaries()
            };

            return solve_steady_1d<LinearSolver>(
                linearized_equation,
                linear_control,
                time
            ).solution;
        },
        picard_options
    );
}

// Realiza a operacao newton solve 1d fake definida por esta interface.
template <class LinearSolver = TDMA>
[[nodiscard]] PicardResult newton_solve_1d_fake(
    const PicardProblem1D&,
    DenseVector,
    const PicardOptions& = {},
    const SteadyState& = {},
    Real = Real{}
) {
    // Realiza a operacao require definida por esta interface.
    require(
        false,
        error_catalog::kNotImplemented,
        ID{"OneDimensional", "NewtonSolve1D", "fvm.1d.solver.NewtonSolve1D"}
    );

    return {};
}

}  // namespace fvm
