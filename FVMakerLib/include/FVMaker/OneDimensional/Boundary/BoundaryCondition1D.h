// ============================================================================
// Arquivo: BoundaryCondition1D.h
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Declara Boundary Condition 1 D no contexto de OneDimensional / Boundary.
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

// Adicione ESTES includes no início do arquivo
#include <iostream>   // Para std::ostream
#include <ostream>    // Para operadores << padrão
#include <string>     // Para std::string
#include <functional>
#include <string_view>

#include <FVMaker/Core/ID.h>
#include <FVMaker/Core/Types.h>



namespace fvm {

using StoredBoundaryFunction1D = std::function<Real(Real, Real)>;

enum class BoundarySide1D {
    left,
    right,
};

enum class BoundaryConditionKind1D {
    linear,
    dirichlet,
    neumann,
    robin,
};

class BoundaryCondition1D final {
public:



public:
    BoundaryCondition1D(Real alpha, Real beta, Real gamma);

    BoundaryCondition1D(
        StoredBoundaryFunction1D alpha,
        StoredBoundaryFunction1D beta,
        StoredBoundaryFunction1D gamma
    );

    [[nodiscard]] static constexpr ID id() noexcept {
        return ID{
            "OneDimensional",
            "BoundaryCondition1D",
            "fvm.1d.boundary.BoundaryCondition1D"
        };
    }

    [[nodiscard]] static constexpr std::string_view class_name() noexcept {
        return id().class_name();
    }

    [[nodiscard]] static constexpr std::string_view class_id() noexcept {
        return id().class_id();
    }

    [[nodiscard]] static BoundaryCondition1D linear(
        Real alpha,
        Real beta,
        Real gamma
    );

    [[nodiscard]] static BoundaryCondition1D linear(
        StoredBoundaryFunction1D alpha,
        StoredBoundaryFunction1D beta,
        StoredBoundaryFunction1D gamma
    );

    [[nodiscard]] static BoundaryCondition1D dirichlet(Real value);
    [[nodiscard]] static BoundaryCondition1D dirichlet(
        StoredBoundaryFunction1D value
    );

    [[nodiscard]] static BoundaryCondition1D neumann(Real derivative);
    [[nodiscard]] static BoundaryCondition1D neumann(
        StoredBoundaryFunction1D derivative
    );

    [[nodiscard]] static BoundaryCondition1D robin(
        Real alpha,
        Real beta,
        Real gamma
    );

    [[nodiscard]] static BoundaryCondition1D robin(
        StoredBoundaryFunction1D alpha,
        StoredBoundaryFunction1D beta,
        StoredBoundaryFunction1D gamma
    );

    [[nodiscard]] BoundaryConditionKind1D kind() const noexcept;
    [[nodiscard]] std::string_view kind_name() const noexcept;
    [[nodiscard]] bool is_dirichlet() const noexcept;
    [[nodiscard]] bool is_neumann() const noexcept;
    [[nodiscard]] bool is_robin() const noexcept;

    [[nodiscard]] Real alpha(Real position, Real time = Real{}) const;
    [[nodiscard]] Real beta(Real position, Real time = Real{}) const;
    [[nodiscard]] Real gamma(Real position, Real time = Real{}) const;

private:
    BoundaryCondition1D(
        BoundaryConditionKind1D kind,
        StoredBoundaryFunction1D alpha,
        StoredBoundaryFunction1D beta,
        StoredBoundaryFunction1D gamma
    );

    BoundaryConditionKind1D kind_{BoundaryConditionKind1D::linear};
    StoredBoundaryFunction1D alpha_;
    StoredBoundaryFunction1D beta_;
    StoredBoundaryFunction1D gamma_;

    void validate_at_reference_point() const;
};

class BoundarySet1D final {
public:
    BoundarySet1D(BoundaryCondition1D left, BoundaryCondition1D right);

    [[nodiscard]] static constexpr ID id() noexcept {
        return ID{
            "OneDimensional",
            "BoundarySet1D",
            "fvm.1d.boundary.BoundarySet1D"
        };
    }

    [[nodiscard]] static constexpr std::string_view class_name() noexcept {
        return id().class_name();
    }

    [[nodiscard]] static constexpr std::string_view class_id() noexcept {
        return id().class_id();
    }

    [[nodiscard]] const BoundaryCondition1D& left() const noexcept;
    [[nodiscard]] const BoundaryCondition1D& right() const noexcept;
    [[nodiscard]] const BoundaryCondition1D& at(BoundarySide1D side) const noexcept;

private:
    BoundaryCondition1D left_;
    BoundaryCondition1D right_;
};

[[nodiscard]] BoundaryCondition1D linear_boundary_1d(
    Real alpha,
    Real beta,
    Real gamma
);

[[nodiscard]] BoundaryCondition1D linear_boundary_1d(
    StoredBoundaryFunction1D alpha,
    StoredBoundaryFunction1D beta,
    StoredBoundaryFunction1D gamma
);

[[nodiscard]] BoundaryCondition1D dirichlet_1d(Real value);
[[nodiscard]] BoundaryCondition1D dirichlet_1d(StoredBoundaryFunction1D value);
[[nodiscard]] BoundaryCondition1D neumann_1d(Real derivative);
[[nodiscard]] BoundaryCondition1D neumann_1d(StoredBoundaryFunction1D derivative);
[[nodiscard]] BoundaryCondition1D robin_1d(Real alpha, Real beta, Real gamma);
[[nodiscard]] BoundaryCondition1D robin_1d(
    StoredBoundaryFunction1D alpha,
    StoredBoundaryFunction1D beta,
    StoredBoundaryFunction1D gamma
);

}  // namespace fvm


std::ostream& operator<<(std::ostream&, const fvm::BoundaryCondition1D&);

