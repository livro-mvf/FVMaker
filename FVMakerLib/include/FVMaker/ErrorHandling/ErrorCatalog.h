// ============================================================================
// Arquivo: ErrorCatalog.h
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Declara Error Catalog no contexto de ErrorHandling.
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
// FVMaker includes
// ----------------------------------------------------------------------------
#include <FVMaker/ErrorHandling/ErrorCodes.h>
#include <FVMaker/ErrorHandling/ErrorDescriptor.h>

namespace fvm::error_catalog {

inline constexpr ErrorDescriptor kInvalidArgument{
    .code = ::fvm::error_code::kInvalidArgument,
    .message = "Invalid argument.",
    .category = "Core",
};

inline constexpr ErrorDescriptor kOutOfRange{
    .code = ::fvm::error_code::kOutOfRange,
    .message = "Value is out of range.",
    .category = "Core",
};

inline constexpr ErrorDescriptor kNotImplemented{
    .code = ::fvm::error_code::kNotImplemented,
    .message = "Requested functionality is not implemented.",
    .category = "Core",
};

inline constexpr ErrorDescriptor kInternalError{
    .code = ::fvm::error_code::kInternalError,
    .message = "Internal FVMaker error.",
    .category = "Core",
};

inline constexpr ErrorDescriptor kInvalidTolerance{
    .code = ::fvm::error_code::kInvalidTolerance,
    .message = "The tolerance value is invalid.",
    .category = "Core",
};

inline constexpr ErrorDescriptor kMissingMesh{
    .code = ::fvm::error_code::kMissingMesh,
    .message = "A mesh supplied by FVGridMaker is required.",
    .category = "Mesh",
};

inline constexpr ErrorDescriptor kInvalidMeshDimension{
    .code = ::fvm::error_code::kInvalidMeshDimension,
    .message = "The mesh dimension is invalid for this operation.",
    .category = "Mesh",
};

inline constexpr ErrorDescriptor kIncompatibleMesh{
    .code = ::fvm::error_code::kIncompatibleMesh,
    .message = "The mesh is incompatible with the requested operation.",
    .category = "Mesh",
};

inline constexpr ErrorDescriptor kInvalidFieldSize{
    .code = ::fvm::error_code::kInvalidFieldSize,
    .message = "The field size is incompatible with the mesh.",
    .category = "Field",
};

inline constexpr ErrorDescriptor kMissingField{
    .code = ::fvm::error_code::kMissingField,
    .message = "A required field is missing.",
    .category = "Field",
};

inline constexpr ErrorDescriptor kInvalidCoefficient{
    .code = ::fvm::error_code::kInvalidCoefficient,
    .message = "The coefficient value or layout is invalid.",
    .category = "Coefficient",
};

inline constexpr ErrorDescriptor kInvalidSource{
    .code = ::fvm::error_code::kInvalidSource,
    .message = "The source term is invalid.",
    .category = "Source",
};

inline constexpr ErrorDescriptor kInvalidBoundaryCondition{
    .code = ::fvm::error_code::kInvalidBoundaryCondition,
    .message = "The boundary condition is invalid.",
    .category = "Boundary",
};

inline constexpr ErrorDescriptor kMissingBoundaryCondition{
    .code = ::fvm::error_code::kMissingBoundaryCondition,
    .message = "A required boundary condition is missing.",
    .category = "Boundary",
};

inline constexpr ErrorDescriptor kInvalidTerm{
    .code = ::fvm::error_code::kInvalidTerm,
    .message = "The equation term is invalid.",
    .category = "Term",
};

inline constexpr ErrorDescriptor kIncompatibleTerms{
    .code = ::fvm::error_code::kIncompatibleTerms,
    .message = "Equation terms are incompatible.",
    .category = "Term",
};

inline constexpr ErrorDescriptor kAssemblyFailed{
    .code = ::fvm::error_code::kAssemblyFailed,
    .message = "The finite-volume system assembly failed.",
    .category = "Assembly",
};

inline constexpr ErrorDescriptor kSingularSystem{
    .code = ::fvm::error_code::kSingularSystem,
    .message = "The linear system is singular.",
    .category = "System",
};

inline constexpr ErrorDescriptor kInvalidSystemSize{
    .code = ::fvm::error_code::kInvalidSystemSize,
    .message = "The linear system size is invalid.",
    .category = "System",
};

inline constexpr ErrorDescriptor kSolverDidNotConverge{
    .code = ::fvm::error_code::kSolverDidNotConverge,
    .message = "The solver did not converge.",
    .category = "Solver",
};

inline constexpr ErrorDescriptor kInvalidTimeStep{
    .code = ::fvm::error_code::kInvalidTimeStep,
    .message = "The time step must be positive.",
    .category = "Time",
};

inline constexpr ErrorDescriptor kInvalidTimeInterval{
    .code = ::fvm::error_code::kInvalidTimeInterval,
    .message = "The time interval is invalid.",
    .category = "Time",
};

inline constexpr ErrorDescriptor kOutputFileOpenFailed{
    .code = ::fvm::error_code::kOutputFileOpenFailed,
    .message = "Output file could not be opened.",
    .category = "Output",
};

inline constexpr ErrorDescriptor kOutputFileWriteFailed{
    .code = ::fvm::error_code::kOutputFileWriteFailed,
    .message = "Output file could not be written.",
    .category = "Output",
};

}  // namespace fvm::error_catalog
