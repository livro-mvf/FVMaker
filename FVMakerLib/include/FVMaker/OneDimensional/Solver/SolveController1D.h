// ============================================================================
// Arquivo: SolveController1D.h
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Declara Solve Controller 1 D no contexto de OneDimensional / Solver.
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

#include <algorithm>
#include <concepts>
#include <cmath>
#include <utility>

#include <FVMaker/Algebra/ErrorNorms.h>
#include <FVMaker/ErrorHandling/ErrorCatalog.h>
#include <FVMaker/ErrorHandling/ThrowError.h>
#include <FVMaker/OneDimensional/Assembly/Assembler1D.h>
#include <FVMaker/OneDimensional/Equation/Equation1D.h>
#include <FVMaker/OneDimensional/Solver/BiCG.h>
#include <FVMaker/OneDimensional/Solver/BiCGSTAB.h>
#include <FVMaker/OneDimensional/Solver/ConjugateGradient.h>
#include <FVMaker/OneDimensional/Solver/GaussSeidel.h>
#include <FVMaker/OneDimensional/Solver/Jacobi.h>
#include <FVMaker/OneDimensional/Solver/LinearSolverOptions1D.h>
#include <FVMaker/OneDimensional/Solver/TDMA.h>
#include <FVMaker/OneDimensional/System/AlgebraicResidual1D.h>
#include <FVMaker/Solver/IterativeSolverOptions.h>
#include <FVMaker/Solver/SolveResult.h>
#include <FVMaker/Solver/SteadyState.h>

namespace fvm {

[[nodiscard]] inline Real rhs_norm_infinity(
    const TridiagonalSystem1D& system
) {
    return norm_infinity(system.rhs());
}

[[nodiscard]] inline Real relative_residual_norm(
    Real residual_norm,
    Real reference_norm
) {
    return residual_norm / std::max(reference_norm, Real{1.0});
}

inline void complete_solve_result(
    SolveResult& result,
    const TridiagonalSystem1D& system,
    const SteadyState& control,
    Real initial_residual_norm
) {
    const Real reference_norm = rhs_norm_infinity(system);
    result.initial_residual_norm = initial_residual_norm;
    result.relative_residual_norm = relative_residual_norm(
        result.residual_norm,
        reference_norm
    );
    result.requested_tolerance = control.effective_tolerance(reference_norm);
    result.reached_absolute_tolerance =
        result.residual_norm <= control.absolute_tolerance;
    result.reached_relative_tolerance =
        result.relative_residual_norm <= control.relative_tolerance;
    result.converged =
        result.reached_absolute_tolerance || result.reached_relative_tolerance;
}

template <class LinearSolver>
concept DirectTridiagonalSolver1D = requires(const TridiagonalSystem1D& system) {
    { LinearSolver::solve(system) } -> std::same_as<SolveResult>;
};

template <class LinearSolver>
concept IterativeTridiagonalSolver1D = requires(
    const TridiagonalSystem1D& system,
    IterativeSolverOptions options
) {
    { LinearSolver::solve(system, options) } -> std::same_as<SolveResult>;
};

template <class LinearSolver = TDMA>
[[nodiscard]] SolveResult solve_steady_system_1d(
    const TridiagonalSystem1D& system,
    const SteadyState& control = {}
) {
    control.validate();

    const DenseVector zero{system.size()};
    const Real initial_residual_norm = norm_infinity(
        algebraic_residual(system, zero)
    );
    const Real effective_tolerance =
        control.effective_tolerance(rhs_norm_infinity(system));

    SolveResult result;

    if constexpr (IterativeTridiagonalSolver1D<LinearSolver>) {
        result = LinearSolver::solve(
            system,
            IterativeSolverOptions{
                .tolerance = effective_tolerance,
                .max_iterations = control.max_iterations
            }
        );
    } else if constexpr (DirectTridiagonalSolver1D<LinearSolver>) {
        result = LinearSolver::solve(system);
    } else {
        static_assert(
            IterativeTridiagonalSolver1D<LinearSolver>
                || DirectTridiagonalSolver1D<LinearSolver>,
            "LinearSolver must expose solve(system) or solve(system, options)."
        );
    }

    complete_solve_result(result, system, control, initial_residual_norm);

    return result;
}

[[nodiscard]] inline SolveResult solve_steady_system_1d(
    const TridiagonalSystem1D& system,
    const LinearSolverOptions1D& options
) {
    options.validate();

    switch (options.solver) {
        case LinearSolverKind1D::tdma:
            return solve_steady_system_1d<TDMA>(system, options.steady_state());
        case LinearSolverKind1D::jacobi:
            return solve_steady_system_1d<Jacobi>(system, options.steady_state());
        case LinearSolverKind1D::gauss_seidel:
            return solve_steady_system_1d<GaussSeidel>(
                system,
                options.steady_state()
            );
        case LinearSolverKind1D::conjugate_gradient:
            return solve_steady_system_1d<ConjugateGradient>(
                system,
                options.steady_state()
            );
        case LinearSolverKind1D::bicg:
            return solve_steady_system_1d<BiCG>(system, options.steady_state());
        case LinearSolverKind1D::bicgstab:
            return solve_steady_system_1d<BiCGSTAB>(
                system,
                options.steady_state()
            );
    }

    require(false, error_catalog::kInvalidArgument, LinearSolverOptions1D::id());
    return {};
}

template <class LinearSolver = TDMA>
[[nodiscard]] SolveResult solve_steady_1d(
    const Equation1D& equation,
    const SteadyState& control = {},
    Real time = Real{}
) {
    return solve_steady_system_1d<LinearSolver>(
        assemble_steady_1d(equation, time),
        control
    );
}

[[nodiscard]] inline SolveResult solve_steady_1d(
    const Equation1D& equation,
    const LinearSolverOptions1D& options,
    Real time = Real{}
) {
    return solve_steady_system_1d(
        assemble_steady_1d(equation, time),
        options
    );
}

}  // namespace fvm
