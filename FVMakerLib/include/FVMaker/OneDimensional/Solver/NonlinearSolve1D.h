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

class PicardProblem1D final {
public:
    PicardProblem1D(
        GridView1D grid,
        NonlinearCoefficient1D coefficient,
        LinearizedSource1D source,
        BoundarySet1D boundaries
    );

    [[nodiscard]] static constexpr ID id() noexcept {
        return ID{
            "OneDimensional",
            "PicardProblem1D",
            "fvm.1d.solver.PicardProblem1D"
        };
    }

    [[nodiscard]] static constexpr std::string_view class_name() noexcept {
        return id().class_name();
    }

    [[nodiscard]] static constexpr std::string_view class_id() noexcept {
        return id().class_id();
    }

    [[nodiscard]] const GridView1D& grid() const noexcept;
    [[nodiscard]] const NonlinearCoefficient1D& coefficient() const noexcept;
    [[nodiscard]] const LinearizedSource1D& source() const noexcept;
    [[nodiscard]] const BoundarySet1D& boundaries() const noexcept;

private:
    GridView1D grid_;
    NonlinearCoefficient1D coefficient_;
    LinearizedSource1D source_;
    BoundarySet1D boundaries_;

    void validate() const;
};

template <class LinearSolver = TDMA>
[[nodiscard]] PicardResult picard_solve_1d(
    const PicardProblem1D& problem,
    DenseVector initial_solution,
    const PicardOptions& picard_options = {},
    const SteadyState& linear_control = {},
    Real time = Real{}
) {
    require(
        initial_solution.size() == problem.grid().num_volumes(),
        error_catalog::kInvalidFieldSize,
        PicardProblem1D::id()
    );

    return picard_iteration(
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

template <class LinearSolver = TDMA>
[[nodiscard]] PicardResult newton_solve_1d_fake(
    const PicardProblem1D&,
    DenseVector,
    const PicardOptions& = {},
    const SteadyState& = {},
    Real = Real{}
) {
    require(
        false,
        error_catalog::kNotImplemented,
        ID{"OneDimensional", "NewtonSolve1D", "fvm.1d.solver.NewtonSolve1D"}
    );

    return {};
}

}  // namespace fvm
