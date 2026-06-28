// ============================================================================
// Arquivo: ErrorCodes.h
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Declara Error Codes no contexto de ErrorHandling.
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
#include <string_view>

namespace fvm::error_code {

inline constexpr std::string_view kInvalidArgument =
    "FVM.CORE.INVALID_ARGUMENT";

inline constexpr std::string_view kOutOfRange = "FVM.CORE.OUT_OF_RANGE";

inline constexpr std::string_view kNotImplemented =
    "FVM.CORE.NOT_IMPLEMENTED";

inline constexpr std::string_view kInternalError =
    "FVM.CORE.INTERNAL_ERROR";

inline constexpr std::string_view kInvalidTolerance =
    "FVM.CORE.INVALID_TOLERANCE";

inline constexpr std::string_view kMissingMesh = "FVM.MESH.MISSING_MESH";

inline constexpr std::string_view kInvalidMeshDimension =
    "FVM.MESH.INVALID_DIMENSION";

inline constexpr std::string_view kIncompatibleMesh =
    "FVM.MESH.INCOMPATIBLE_MESH";

inline constexpr std::string_view kInvalidFieldSize =
    "FVM.FIELD.INVALID_SIZE";

inline constexpr std::string_view kMissingField = "FVM.FIELD.MISSING_FIELD";

inline constexpr std::string_view kInvalidCoefficient =
    "FVM.COEFFICIENT.INVALID_COEFFICIENT";

inline constexpr std::string_view kInvalidSource =
    "FVM.SOURCE.INVALID_SOURCE";

inline constexpr std::string_view kInvalidBoundaryCondition =
    "FVM.BOUNDARY.INVALID_CONDITION";

inline constexpr std::string_view kMissingBoundaryCondition =
    "FVM.BOUNDARY.MISSING_CONDITION";

inline constexpr std::string_view kInvalidTerm = "FVM.TERM.INVALID_TERM";

inline constexpr std::string_view kIncompatibleTerms =
    "FVM.TERM.INCOMPATIBLE_TERMS";

inline constexpr std::string_view kAssemblyFailed =
    "FVM.ASSEMBLY.FAILED";

inline constexpr std::string_view kSingularSystem =
    "FVM.SYSTEM.SINGULAR_SYSTEM";

inline constexpr std::string_view kInvalidSystemSize =
    "FVM.SYSTEM.INVALID_SIZE";

inline constexpr std::string_view kSolverDidNotConverge =
    "FVM.SOLVER.DID_NOT_CONVERGE";

inline constexpr std::string_view kInvalidTimeStep =
    "FVM.TIME.INVALID_TIME_STEP";

inline constexpr std::string_view kInvalidTimeInterval =
    "FVM.TIME.INVALID_INTERVAL";

inline constexpr std::string_view kOutputFileOpenFailed =
    "FVM.OUTPUT.FILE_OPEN_FAILED";

inline constexpr std::string_view kOutputFileWriteFailed =
    "FVM.OUTPUT.FILE_WRITE_FAILED";

}  // namespace fvm::error_code
