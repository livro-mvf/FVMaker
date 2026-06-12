// ----------------------------------------------------------------------------
// File: ManufacturedProblem1D.h
// Project: FVMaker
// Version: 0.1.0
// Description: Defines a manufactured 1D problem with named coefficients.
// Author: FVMaker Team
// License: GPL-3.0-or-later
// ----------------------------------------------------------------------------

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

struct NamedCoefficient1D final {
    std::string name;
    VariableCoefficient1D coefficient;
};

class ManufacturedProblem1D final {
public:
    explicit ManufacturedProblem1D(ManufacturedSolution1D solution)
        : solution_(std::move(solution)) {}

    ManufacturedProblem1D(
        ManufacturedSolution1D solution,
        std::vector<NamedCoefficient1D> coefficients
    )
        : solution_(std::move(solution)),
          coefficients_(std::move(coefficients)) {}

    [[nodiscard]] const ManufacturedSolution1D& solution() const noexcept {
        return solution_;
    }

    [[nodiscard]] const std::vector<NamedCoefficient1D>& coefficients() const noexcept {
        return coefficients_;
    }

    [[nodiscard]] Size coefficient_count() const noexcept {
        return coefficients_.size();
    }

    void add_coefficient(std::string name, VariableCoefficient1D coefficient) {
        coefficients_.push_back(
            NamedCoefficient1D{
                .name = std::move(name),
                .coefficient = std::move(coefficient),
            }
        );
    }

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
