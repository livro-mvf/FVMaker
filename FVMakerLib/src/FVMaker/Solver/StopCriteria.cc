// ----------------------------------------------------------------------------
// File: StopCriteria.cc
// Project: FVMaker
// Description: Implements flexible stopping criteria for iterative solvers.
// ----------------------------------------------------------------------------

#include <algorithm>
#include <cmath>
#include <limits>
#include <utility>

#include <FVMaker/ErrorHandling/ErrorCatalog.h>
#include <FVMaker/ErrorHandling/ThrowError.h>
#include <FVMaker/Solver/StopCriteria.h>

namespace fvm {

namespace {

[[nodiscard]] Real safe_reference(Real value) noexcept {
    return std::max(std::abs(value), Real{1});
}

[[nodiscard]] Real vector_range(const DenseVector& values) noexcept {
    if (values.empty()) {
        return Real{};
    }

    auto [min_it, max_it] = std::minmax_element(
        values.values().begin(),
        values.values().end()
    );

    return *max_it - *min_it;
}

[[nodiscard]] Real rms(const DenseVector& values) noexcept {
    if (values.empty()) {
        return Real{};
    }

    Real sum{};

    for (Real value : values.values()) {
        sum += value * value;
    }

    return std::sqrt(sum / static_cast<Real>(values.size()));
}

[[nodiscard]] Real weighted_rms(
    const DenseVector& values,
    std::span<const Real> weights
) {
    require(
        weights.size() == values.size(),
        error_catalog::kInvalidSystemSize,
        StopCriteria::id()
    );

    Real weighted_sum{};
    Real weight_sum{};

    for (Size i = 0; i < values.size(); ++i) {
        require(
            std::isfinite(weights[i]) && weights[i] >= Real{},
            error_catalog::kInvalidArgument,
            StopCriteria::id()
        );

        weighted_sum += values[i] * values[i] * weights[i];
        weight_sum += weights[i];
    }

    if (weight_sum <= Real{}) {
        return Real{};
    }

    return std::sqrt(weighted_sum / weight_sum);
}

[[nodiscard]] Real criterion_value(
    const StopCriterion& criterion,
    const StopCriteriaState& state
) {
    switch (criterion.kind) {
        case StopCriterionKind::correction_absolute:
            require(state.correction != nullptr, error_catalog::kInvalidArgument, StopCriteria::id());
            return VectorNorms::evaluate(*state.correction, criterion.norm);

        case StopCriterionKind::correction_relative_solution:
            require(state.correction != nullptr, error_catalog::kInvalidArgument, StopCriteria::id());
            require(state.solution != nullptr, error_catalog::kInvalidArgument, StopCriteria::id());
            return VectorNorms::evaluate(*state.correction, criterion.norm)
                   / safe_reference(VectorNorms::evaluate(*state.solution, criterion.norm));

        case StopCriterionKind::correction_relative_range:
            require(state.correction != nullptr, error_catalog::kInvalidArgument, StopCriteria::id());
            require(state.solution != nullptr, error_catalog::kInvalidArgument, StopCriteria::id());
            return VectorNorms::evaluate(*state.correction, criterion.norm)
                   / safe_reference(vector_range(*state.solution));

        case StopCriterionKind::correction_rms:
            require(state.correction != nullptr, error_catalog::kInvalidArgument, StopCriteria::id());
            return rms(*state.correction);

        case StopCriterionKind::correction_weighted_rms:
            require(state.correction != nullptr, error_catalog::kInvalidArgument, StopCriteria::id());
            return weighted_rms(*state.correction, state.weights);

        case StopCriterionKind::residual_absolute:
            require(state.residual != nullptr, error_catalog::kInvalidArgument, StopCriteria::id());
            return VectorNorms::evaluate(*state.residual, criterion.norm);

        case StopCriterionKind::residual_relative_initial:
            require(state.residual != nullptr, error_catalog::kInvalidArgument, StopCriteria::id());
            return VectorNorms::evaluate(*state.residual, criterion.norm)
                   / safe_reference(state.initial_residual_norm);

        case StopCriterionKind::max_iterations:
            return static_cast<Real>(state.iteration);
    }

    return std::numeric_limits<Real>::infinity();
}

}  // namespace

std::string_view name(StopCriterionKind kind) noexcept {
    switch (kind) {
        case StopCriterionKind::correction_absolute:
            return "correction_absolute";
        case StopCriterionKind::correction_relative_solution:
            return "correction_relative_solution";
        case StopCriterionKind::correction_relative_range:
            return "correction_relative_range";
        case StopCriterionKind::correction_rms:
            return "correction_rms";
        case StopCriterionKind::correction_weighted_rms:
            return "correction_weighted_rms";
        case StopCriterionKind::residual_absolute:
            return "residual_absolute";
        case StopCriterionKind::residual_relative_initial:
            return "residual_relative_initial";
        case StopCriterionKind::max_iterations:
            return "max_iterations";
    }

    return "unknown";
}

StopCriteria::StopCriteria(std::vector<StopCriterion> criteria)
    : criteria_(std::move(criteria)) {
    validate();
}

StopCriteria StopCriteria::residual_absolute(Real tolerance) {
    return StopCriteria{{StopCriterion{StopCriterionKind::residual_absolute, tolerance}}};
}

StopCriteria StopCriteria::residual_relative_initial(Real tolerance) {
    return StopCriteria{{StopCriterion{StopCriterionKind::residual_relative_initial, tolerance}}};
}

StopCriteria StopCriteria::correction_absolute(Real tolerance) {
    return StopCriteria{{StopCriterion{StopCriterionKind::correction_absolute, tolerance}}};
}

StopCriteria StopCriteria::chapter_defaults(Real tolerance) {
    return StopCriteria{{
        StopCriterion{StopCriterionKind::correction_absolute, tolerance},
        StopCriterion{StopCriterionKind::residual_relative_initial, tolerance},
    }};
}

void StopCriteria::add(StopCriterion criterion) {
    criteria_.push_back(criterion);
    validate();
}

bool StopCriteria::empty() const noexcept {
    return criteria_.empty();
}

std::span<const StopCriterion> StopCriteria::criteria() const noexcept {
    return criteria_;
}

StopCriteriaEvaluation StopCriteria::evaluate(
    const StopCriteriaState& state
) const {
    validate();

    for (const StopCriterion& criterion : criteria_) {
        const Real value = criterion_value(criterion, state);

        if (value <= criterion.tolerance) {
            return StopCriteriaEvaluation{
                .converged = true,
                .reached_iteration_limit = false,
                .criterion = criterion.kind,
                .value = value,
                .tolerance = criterion.tolerance,
            };
        }
    }

    if (state.max_iterations > 0 && state.iteration >= state.max_iterations) {
        return StopCriteriaEvaluation{
            .converged = false,
            .reached_iteration_limit = true,
            .criterion = StopCriterionKind::max_iterations,
            .value = static_cast<Real>(state.iteration),
            .tolerance = static_cast<Real>(state.max_iterations),
        };
    }

    return StopCriteriaEvaluation{};
}

void StopCriteria::validate() const {
    for (const StopCriterion& criterion : criteria_) {
        require(
            std::isfinite(criterion.tolerance) && criterion.tolerance >= Real{},
            error_catalog::kInvalidTolerance,
            StopCriteria::id()
        );
    }
}

}  // namespace fvm
