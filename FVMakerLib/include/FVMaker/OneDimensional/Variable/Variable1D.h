// ----------------------------------------------------------------------------
// File: Variable1D.h
// Project: FVMaker
// Description: Defines a 1D finite-volume variable with common solution states.
// ----------------------------------------------------------------------------

#pragma once

#include <concepts>
#include <functional>
#include <iosfwd>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include <FVMaker/Algebra/DenseVector.h>
#include <FVMaker/Core/ID.h>
#include <FVMaker/Core/Types.h>
#include <FVMaker/OneDimensional/Grid/GridView1D.h>
#include <FVMaker/Solver/SolveResult.h>

namespace fvm {

struct VariableDifferenceNorms1D final {
    Real infinity{};
    Real l1{};
    Real l2{};
};

class Variable1D final {
public:
    explicit Variable1D(GridView1D grid, std::string name = "phi");
    Variable1D(GridView1D grid, std::string name, Real initial_value);
    Variable1D(GridView1D grid, std::string name, std::vector<Real> phi);

    [[nodiscard]] static constexpr ID id() noexcept {
        return ID{
            "OneDimensional",
            "Variable1D",
            "fvm.1d.variable.Variable1D"
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

    [[nodiscard]] DenseVector& phi() noexcept;
    [[nodiscard]] const DenseVector& phi() const noexcept;

    [[nodiscard]] DenseVector& phi_ant() noexcept;
    [[nodiscard]] const DenseVector& phi_ant() const noexcept;

    [[nodiscard]] DenseVector& phi_iter() noexcept;
    [[nodiscard]] const DenseVector& phi_iter() const noexcept;

    [[nodiscard]] DenseVector& phi_analit() noexcept;
    [[nodiscard]] const DenseVector& phi_analit() const noexcept;

    void fill(Real value);
    void assign_phi(DenseVector values);
    void assign_phi(std::vector<Real> values);
    void update_phi(DenseVector values);
    void update_phi(const SolveResult& result);
    void save_iteration(DenseVector values);
    void advance_iteration_to_phi();

    template <class Function>
        requires std::invocable<Function, Real> &&
                 std::convertible_to<std::invoke_result_t<Function, Real>, Real>
    void assign_analytic(Function&& function) {
        const auto centers = grid_.centers();

        for (Size i = 0; i < phi_analit_.size(); ++i) {
            phi_analit_[i] =
                static_cast<Real>(std::invoke(function, centers[i]));
        }
    }

    [[nodiscard]] DenseVector analytic_error() const;
    [[nodiscard]] DenseVector difference_to_previous() const;
    [[nodiscard]] DenseVector difference_to_iteration() const;
    [[nodiscard]] DenseVector difference_to_analytic() const;

    [[nodiscard]] VariableDifferenceNorms1D norms_to_previous() const;
    [[nodiscard]] VariableDifferenceNorms1D norms_to_iteration() const;
    [[nodiscard]] VariableDifferenceNorms1D norms_to_analytic() const;

private:
    GridView1D grid_;
    std::string name_;
    DenseVector phi_;
    DenseVector phi_ant_;
    DenseVector phi_iter_;
    DenseVector phi_analit_;

    void validate_size(const DenseVector& values) const;
    void copy_phi_to_previous();
    [[nodiscard]] DenseVector difference_to(const DenseVector& reference) const;
    [[nodiscard]] static VariableDifferenceNorms1D norms_of(
        const DenseVector& difference
    );
};

std::ostream& operator<<(std::ostream& os, const Variable1D& variable);

}  // namespace fvm
