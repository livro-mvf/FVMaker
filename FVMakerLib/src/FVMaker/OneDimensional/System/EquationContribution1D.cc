// ----------------------------------------------------------------------------
// File: EquationContribution1D.cc
// Project: FVMaker
// Description: Implements additive 1D equation contributions.
// ----------------------------------------------------------------------------

#include <vector>

#include <FVMaker/ErrorHandling/ErrorCatalog.h>
#include <FVMaker/ErrorHandling/ThrowError.h>
#include <FVMaker/OneDimensional/System/EquationContribution1D.h>

namespace fvm {

EquationContribution1D::EquationContribution1D(Size size)
    : aw_(size),
      ap_(size),
      ae_(size),
      bp_(size) {
    validate();
}

Size EquationContribution1D::size() const noexcept {
    return ap_.size();
}

DenseVector& EquationContribution1D::aw() noexcept {
    return aw_;
}

const DenseVector& EquationContribution1D::aw() const noexcept {
    return aw_;
}

DenseVector& EquationContribution1D::ap() noexcept {
    return ap_;
}

const DenseVector& EquationContribution1D::ap() const noexcept {
    return ap_;
}

DenseVector& EquationContribution1D::ae() noexcept {
    return ae_;
}

const DenseVector& EquationContribution1D::ae() const noexcept {
    return ae_;
}

DenseVector& EquationContribution1D::bp() noexcept {
    return bp_;
}

const DenseVector& EquationContribution1D::bp() const noexcept {
    return bp_;
}

EquationContribution1D& EquationContribution1D::operator+=(
    const EquationContribution1D& other
) {
    require(size() == other.size(), error_catalog::kInvalidSystemSize, id());

    for (Size i = 0; i < size(); ++i) {
        aw_[i] += other.aw_[i];
        ap_[i] += other.ap_[i];
        ae_[i] += other.ae_[i];
        bp_[i] += other.bp_[i];
    }

    return *this;
}

void EquationContribution1D::validate() const {
    require(size() > 0, error_catalog::kInvalidSystemSize, id());
    require(aw_.size() == size(), error_catalog::kInvalidSystemSize, id());
    require(ae_.size() == size(), error_catalog::kInvalidSystemSize, id());
    require(bp_.size() == size(), error_catalog::kInvalidSystemSize, id());
}

EquationContribution1D operator+(
    EquationContribution1D lhs,
    const EquationContribution1D& rhs
) {
    lhs += rhs;
    return lhs;
}

TridiagonalSystem1D to_tridiagonal_system(
    const EquationContribution1D& contribution
) {
    const Size n = contribution.size();

    std::vector<Real> lower(n > 0 ? n - 1 : 0);
    std::vector<Real> diagonal(n);
    std::vector<Real> upper(n > 0 ? n - 1 : 0);
    DenseVector rhs{n};

    for (Size i = 0; i < n; ++i) {
        diagonal[i] = contribution.ap()[i];
        rhs[i] = contribution.bp()[i];

        if (i > 0) {
            lower[i - 1] = -contribution.aw()[i];
        }

        if (i + 1 < n) {
            upper[i] = -contribution.ae()[i];
        }
    }

    return TridiagonalSystem1D{
        std::move(lower),
        std::move(diagonal),
        std::move(upper),
        std::move(rhs)
    };
}

}  // namespace fvm
