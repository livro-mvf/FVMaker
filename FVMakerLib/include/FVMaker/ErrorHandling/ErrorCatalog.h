// ----------------------------------------------------------------------------
// File: ErrorCatalog.h
// Project: FVMaker
// Version: 0.1.0
// Description: Defines the built-in FVMaker error catalogue.
// Author: FVMaker Team
// License: GPL-3.0-or-later
// ----------------------------------------------------------------------------

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
