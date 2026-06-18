// ----------------------------------------------------------------------------
// File: StopCriteria.h
// Project: FVMaker
// Description: Defines flexible stopping criteria for iterative solvers.
// ----------------------------------------------------------------------------

#pragma once

#include <span>
#include <string_view>
#include <vector>

#include <FVMaker/Algebra/DenseVector.h>
#include <FVMaker/Algebra/ErrorNorms.h>
#include <FVMaker/Core/ID.h>
#include <FVMaker/Core/Types.h>

namespace fvm {

enum class StopCriterionKind {
    correction_absolute,
    correction_relative_solution,
    correction_relative_range,
    correction_rms,
    correction_weighted_rms,
    residual_absolute,
    residual_relative_initial,
    max_iterations,
};

[[nodiscard]] std::string_view name(StopCriterionKind kind) noexcept;

struct StopCriterion final {
    StopCriterionKind kind{StopCriterionKind::residual_absolute};
    Real tolerance{1.0e-10};
    NormType norm{NormType::infinity};
};

struct StopCriteriaState final {
    Size iteration{};
    Size max_iterations{};
    const DenseVector* solution{};
    const DenseVector* correction{};
    const DenseVector* residual{};
    Real initial_residual_norm{};
    std::span<const Real> weights{};
};

struct StopCriteriaEvaluation final {
    bool converged{false};
    bool reached_iteration_limit{false};
    StopCriterionKind criterion{StopCriterionKind::max_iterations};
    Real value{};
    Real tolerance{};
};

class StopCriteria final {
public:
    StopCriteria() = default;
    explicit StopCriteria(std::vector<StopCriterion> criteria);

    [[nodiscard]] static constexpr ID id() noexcept {
        return ID{"Solver", "StopCriteria", "fvm.solver.StopCriteria"};
    }

    [[nodiscard]] static constexpr std::string_view class_name() noexcept {
        return id().class_name();
    }

    [[nodiscard]] static constexpr std::string_view class_id() noexcept {
        return id().class_id();
    }

    [[nodiscard]] static StopCriteria residual_absolute(Real tolerance);
    [[nodiscard]] static StopCriteria residual_relative_initial(Real tolerance);
    [[nodiscard]] static StopCriteria correction_absolute(Real tolerance);
    [[nodiscard]] static StopCriteria chapter_defaults(Real tolerance);

    void add(StopCriterion criterion);
    [[nodiscard]] bool empty() const noexcept;
    [[nodiscard]] std::span<const StopCriterion> criteria() const noexcept;

    [[nodiscard]] StopCriteriaEvaluation evaluate(
        const StopCriteriaState& state
    ) const;

    void validate() const;

private:
    std::vector<StopCriterion> criteria_;
};

}  // namespace fvm
