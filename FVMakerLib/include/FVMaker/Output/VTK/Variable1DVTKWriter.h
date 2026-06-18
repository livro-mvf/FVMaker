// ----------------------------------------------------------------------------
// File: Variable1DVTKWriter.h
// Project: FVMaker
// Description: Declares a legacy VTK writer for 1D variables.
// ----------------------------------------------------------------------------

#pragma once

#include <filesystem>
#include <iosfwd>
#include <string_view>

#include <FVMaker/Core/ID.h>
#include <FVMaker/OneDimensional/Variable/Variable1D.h>

namespace fvm {

class Variable1DVTKWriter final {
public:
    [[nodiscard]] static constexpr ID id() noexcept {
        return ID{
            "Output",
            "Variable1DVTKWriter",
            "fvm.output.vtk.Variable1DVTKWriter"
        };
    }

    [[nodiscard]] static constexpr std::string_view class_name() noexcept {
        return id().class_name();
    }

    [[nodiscard]] static constexpr std::string_view class_id() noexcept {
        return id().class_id();
    }

    static void write(const Variable1D& variable, std::ostream& output);
    static void write(
        const Variable1D& variable,
        const std::filesystem::path& filepath
    );
};

}  // namespace fvm
