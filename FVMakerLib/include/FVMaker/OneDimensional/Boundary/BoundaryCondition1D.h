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

// Identifica o lado esquerdo ou direito de uma malha 1D.
enum class BoundarySide1D {
    left,
    right,
};

// Classifica o tipo matematico da condicao de contorno 1D.
enum class BoundaryConditionKind1D {
    linear,
    dirichlet,
    neumann,
    robin,
};

// Representa uma condicao de contorno em uma extremidade da malha 1D.
class BoundaryCondition1D final {
public:



public:
    // Cria um objeto BoundaryCondition1D com os dados fornecidos.
    BoundaryCondition1D(Real alpha, Real beta, Real gamma);

    // Cria um objeto BoundaryCondition1D com os dados fornecidos.
    BoundaryCondition1D(
        StoredBoundaryFunction1D alpha,
        StoredBoundaryFunction1D beta,
        StoredBoundaryFunction1D gamma
    );

    // Retorna o identificador estavel desta classe na biblioteca.
    [[nodiscard]] static constexpr ID id() noexcept {
        return ID{
            "OneDimensional",
            "BoundaryCondition1D",
            "fvm.1d.boundary.BoundaryCondition1D"
        };
    }

    // Retorna o nome curto da classe para diagnostico e documentacao.
    [[nodiscard]] static constexpr std::string_view class_name() noexcept {
        return id().class_name();
    }

    // Retorna o identificador completo da classe na hierarquia da biblioteca.
    [[nodiscard]] static constexpr std::string_view class_id() noexcept {
        return id().class_id();
    }

    // Realiza a operacao linear definida por esta interface.
    [[nodiscard]] static BoundaryCondition1D linear(
        Real alpha,
        Real beta,
        Real gamma
    );

    // Realiza a operacao linear definida por esta interface.
    [[nodiscard]] static BoundaryCondition1D linear(
        StoredBoundaryFunction1D alpha,
        StoredBoundaryFunction1D beta,
        StoredBoundaryFunction1D gamma
    );

    // Realiza a operacao dirichlet definida por esta interface.
    [[nodiscard]] static BoundaryCondition1D dirichlet(Real value);
    // Realiza a operacao dirichlet definida por esta interface.
    [[nodiscard]] static BoundaryCondition1D dirichlet(
        StoredBoundaryFunction1D value
    );

    // Realiza a operacao neumann definida por esta interface.
    [[nodiscard]] static BoundaryCondition1D neumann(Real derivative);
    // Realiza a operacao neumann definida por esta interface.
    [[nodiscard]] static BoundaryCondition1D neumann(
        StoredBoundaryFunction1D derivative
    );

    // Realiza a operacao robin definida por esta interface.
    [[nodiscard]] static BoundaryCondition1D robin(
        Real alpha,
        Real beta,
        Real gamma
    );

    // Realiza a operacao robin definida por esta interface.
    [[nodiscard]] static BoundaryCondition1D robin(
        StoredBoundaryFunction1D alpha,
        StoredBoundaryFunction1D beta,
        StoredBoundaryFunction1D gamma
    );

    // Retorna a informacao kind associada ao objeto.
    [[nodiscard]] BoundaryConditionKind1D kind() const noexcept;
    // Retorna a informacao kind name associada ao objeto.
    [[nodiscard]] std::string_view kind_name() const noexcept;
    // Informa se a condicao is dirichlet e verdadeira.
    [[nodiscard]] bool is_dirichlet() const noexcept;
    // Informa se a condicao is neumann e verdadeira.
    [[nodiscard]] bool is_neumann() const noexcept;
    // Informa se a condicao is robin e verdadeira.
    [[nodiscard]] bool is_robin() const noexcept;

    // Realiza a operacao alpha definida por esta interface.
    [[nodiscard]] Real alpha(Real position, Real time = Real{}) const;
    // Realiza a operacao beta definida por esta interface.
    [[nodiscard]] Real beta(Real position, Real time = Real{}) const;
    // Realiza a operacao gamma definida por esta interface.
    [[nodiscard]] Real gamma(Real position, Real time = Real{}) const;

private:
    // Cria um objeto BoundaryCondition1D com os dados fornecidos.
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

    // Verifica se as hipoteses numericas e estruturais foram atendidas.
    void validate_at_reference_point() const;
};

// Agrupa as condicoes de contorno esquerda e direita de um problema 1D.
class BoundarySet1D final {
public:
    // Cria um objeto BoundarySet1D com os dados fornecidos.
    BoundarySet1D(BoundaryCondition1D left, BoundaryCondition1D right);

    // Retorna o identificador estavel desta classe na biblioteca.
    [[nodiscard]] static constexpr ID id() noexcept {
        return ID{
            "OneDimensional",
            "BoundarySet1D",
            "fvm.1d.boundary.BoundarySet1D"
        };
    }

    // Retorna o nome curto da classe para diagnostico e documentacao.
    [[nodiscard]] static constexpr std::string_view class_name() noexcept {
        return id().class_name();
    }

    // Retorna o identificador completo da classe na hierarquia da biblioteca.
    [[nodiscard]] static constexpr std::string_view class_id() noexcept {
        return id().class_id();
    }

    // Retorna a informacao left armazenada no objeto.
    [[nodiscard]] const BoundaryCondition1D& left() const noexcept;
    // Retorna a informacao right armazenada no objeto.
    [[nodiscard]] const BoundaryCondition1D& right() const noexcept;
    // Retorna a informacao at armazenada no objeto.
    [[nodiscard]] const BoundaryCondition1D& at(BoundarySide1D side) const noexcept;

private:
    BoundaryCondition1D left_;
    BoundaryCondition1D right_;
};

// Realiza a operacao linear boundary 1d definida por esta interface.
[[nodiscard]] BoundaryCondition1D linear_boundary_1d(
    Real alpha,
    Real beta,
    Real gamma
);

// Realiza a operacao linear boundary 1d definida por esta interface.
[[nodiscard]] BoundaryCondition1D linear_boundary_1d(
    StoredBoundaryFunction1D alpha,
    StoredBoundaryFunction1D beta,
    StoredBoundaryFunction1D gamma
);

// Realiza a operacao dirichlet 1d definida por esta interface.
[[nodiscard]] BoundaryCondition1D dirichlet_1d(Real value);
// Realiza a operacao dirichlet 1d definida por esta interface.
[[nodiscard]] BoundaryCondition1D dirichlet_1d(StoredBoundaryFunction1D value);
// Realiza a operacao neumann 1d definida por esta interface.
[[nodiscard]] BoundaryCondition1D neumann_1d(Real derivative);
// Realiza a operacao neumann 1d definida por esta interface.
[[nodiscard]] BoundaryCondition1D neumann_1d(StoredBoundaryFunction1D derivative);
// Realiza a operacao robin 1d definida por esta interface.
[[nodiscard]] BoundaryCondition1D robin_1d(Real alpha, Real beta, Real gamma);
// Realiza a operacao robin 1d definida por esta interface.
[[nodiscard]] BoundaryCondition1D robin_1d(
    StoredBoundaryFunction1D alpha,
    StoredBoundaryFunction1D beta,
    StoredBoundaryFunction1D gamma
);

}  // namespace fvm


// Escreve uma representacao textual do objeto no fluxo de saida.
std::ostream& operator<<(std::ostream&, const fvm::BoundaryCondition1D&);

