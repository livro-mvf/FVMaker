// ----------------------------------------------------------------------------
// File: Field1D.h
// Project: FVMaker
// Version: 0.1.0
// Description: Defines scalar fields stored on one-dimensional control volumes.
// Author: FVMaker Team
// License: GPL-3.0-or-later
// ----------------------------------------------------------------------------

#pragma once

// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <concepts>
#include <functional>
#include <span>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

// ----------------------------------------------------------------------------
// FVMaker includes
// ----------------------------------------------------------------------------
#include <FVMaker/Core/ID.h>
#include <FVMaker/Core/Types.h>
#include <FVMaker/ErrorHandling/ErrorCatalog.h>
#include <FVMaker/ErrorHandling/ThrowError.h>
#include <FVMaker/OneDimensional/Field/FieldView1D.h>
#include <FVMaker/OneDimensional/Grid/GridView1D.h>

namespace fvm {

class Field1D final {
public:
    Field1D(GridView1D grid, std::string name);

    Field1D(
        GridView1D grid,
        std::string name,
        Real uniform_value,
        bool keep_history = false
    );

    Field1D(
        GridView1D grid,
        std::string name,
        std::vector<Real> values,
        bool keep_history = false
    );

    template <class Function>
        requires std::invocable<Function, Real> &&
                 std::convertible_to<std::invoke_result_t<Function, Real>, Real>
    [[nodiscard]] static Field1D from_function(
        GridView1D grid,
        std::string name,
        Function&& function,
        bool keep_history = false
    ) {
        std::vector<Real> values(grid.num_volumes());
        const auto centers = grid.centers();

        for (Size i = 0; i < values.size(); ++i) {
            values[i] = static_cast<Real>(std::invoke(function, centers[i]));
        }

        return Field1D{
            grid,
            std::move(name),
            std::move(values),
            keep_history
        };
    }

    [[nodiscard]] static constexpr ID id() noexcept {
        return ID{
            "OneDimensional",
            "Field1D",
            "fvm.1d.field.Field1D"
        };
    }

    [[nodiscard]] static constexpr std::string_view class_name() noexcept {
        return id().class_name();
    }

    [[nodiscard]] static constexpr std::string_view class_id() noexcept {
        return id().class_id();
    }

    [[nodiscard]] const GridView1D& grid() const noexcept;
    [[nodiscard]] std::string_view name() const noexcept;
    void rename(std::string name);

    [[nodiscard]] Size size() const noexcept;
    [[nodiscard]] bool empty() const noexcept;

    [[nodiscard]] std::span<Real> values() noexcept;
    [[nodiscard]] std::span<const Real> values() const noexcept;

    [[nodiscard]] Real& operator[](Size index) noexcept;
    [[nodiscard]] const Real& operator[](Size index) const noexcept;

    void fill(Real value);

    template <class Function>
        requires std::invocable<Function, Real> &&
                 std::convertible_to<std::invoke_result_t<Function, Real>, Real>
    void assign_function(Function&& function) {
        const auto centers = grid_.centers();

        for (Size i = 0; i < values_.size(); ++i) {
            values_[i] = static_cast<Real>(std::invoke(function, centers[i]));
        }
    }

    void assign_values(std::vector<Real> values);

    [[nodiscard]] FieldView1D view() noexcept;

    [[nodiscard]] bool history_enabled() const noexcept;
    [[nodiscard]] Size history_size() const noexcept;
    [[nodiscard]] std::span<const Real> history_step(Size index) const;
    void save_state();
    void clear_history();

private:
    GridView1D grid_;
    std::string name_;
    std::vector<Real> values_;
    bool keep_history_{false};
    std::vector<Real> history_;

    void validate_size() const;
    void record_initial_state();
};

}  // namespace fvm
