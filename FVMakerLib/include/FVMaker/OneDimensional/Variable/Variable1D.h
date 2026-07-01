// ============================================================================
// Arquivo: Variable1D.h
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Declara Variable 1 D no contexto de OneDimensional / Variable.
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

// Representa o conceito de variable difference norms1 d dentro da biblioteca FVMaker.
struct VariableDifferenceNorms1D final {
    Real infinity{};
    Real l1{};
    Real l2{};
};

// Representa uma variavel numerica definida sobre uma malha 1D.
class Variable1D final {
public:
    // Cria um objeto Variable1D com os dados fornecidos.
    explicit Variable1D(GridView1D grid, std::string name = "phi");
    // Cria um objeto Variable1D com os dados fornecidos.
    Variable1D(GridView1D grid, std::string name, Real initial_value);
    // Cria um objeto Variable1D com os dados fornecidos.
    Variable1D(GridView1D grid, std::string name, std::vector<Real> phi);

    // Retorna o identificador estavel desta classe na biblioteca.
    [[nodiscard]] static constexpr ID id() noexcept {
        return ID{
            "OneDimensional",
            "Variable1D",
            "fvm.1d.variable.Variable1D"
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

    // Retorna a informacao grid associada ao objeto.
    [[nodiscard]] const GridView1D& grid() const noexcept;
    // Retorna a informacao name associada ao objeto.
    [[nodiscard]] std::string_view name() const noexcept;
    // Realiza a operacao rename definida por esta interface.
    void rename(std::string name);

    // Retorna a informacao size associada ao objeto.
    [[nodiscard]] Size size() const noexcept;
    // Retorna a informacao empty associada ao objeto.
    [[nodiscard]] bool empty() const noexcept;

    // Realiza a operacao phi definida por esta interface.
    [[nodiscard]] DenseVector& phi() noexcept;
    // Retorna a informacao phi associada ao objeto.
    [[nodiscard]] const DenseVector& phi() const noexcept;

    // Realiza a operacao phi ant definida por esta interface.
    [[nodiscard]] DenseVector& phi_ant() noexcept;
    // Retorna a informacao phi ant associada ao objeto.
    [[nodiscard]] const DenseVector& phi_ant() const noexcept;

    // Realiza a operacao phi iter definida por esta interface.
    [[nodiscard]] DenseVector& phi_iter() noexcept;
    // Retorna a informacao phi iter associada ao objeto.
    [[nodiscard]] const DenseVector& phi_iter() const noexcept;

    // Realiza a operacao phi analit definida por esta interface.
    [[nodiscard]] DenseVector& phi_analit() noexcept;
    // Retorna a informacao phi analit associada ao objeto.
    [[nodiscard]] const DenseVector& phi_analit() const noexcept;

    // Realiza a operacao fill definida por esta interface.
    void fill(Real value);
    // Realiza a operacao assign phi definida por esta interface.
    void assign_phi(DenseVector values);
    // Realiza a operacao assign phi definida por esta interface.
    void assign_phi(std::vector<Real> values);
    // Realiza a operacao update phi definida por esta interface.
    void update_phi(DenseVector values);
    // Realiza a operacao update phi definida por esta interface.
    void update_phi(const SolveResult& result);
    // Realiza a operacao save iteration definida por esta interface.
    void save_iteration(DenseVector values);
    // Realiza a operacao advance iteration to phi definida por esta interface.
    void advance_iteration_to_phi();

    template <class Function>
        requires std::invocable<Function, Real> &&
                 std::convertible_to<std::invoke_result_t<Function, Real>, Real>
    // Realiza a operacao assign analytic definida por esta interface.
    void assign_analytic(Function&& function) {
        const auto centers = grid_.centers();

        for (Size i = 0; i < phi_analit_.size(); ++i) {
            phi_analit_[i] =
                static_cast<Real>(std::invoke(function, centers[i]));
        }
    }

    // Realiza a operacao analytic error definida por esta interface.
    [[nodiscard]] DenseVector analytic_error() const;
    // Realiza a operacao difference to previous definida por esta interface.
    [[nodiscard]] DenseVector difference_to_previous() const;
    // Realiza a operacao difference to iteration definida por esta interface.
    [[nodiscard]] DenseVector difference_to_iteration() const;
    // Realiza a operacao difference to analytic definida por esta interface.
    [[nodiscard]] DenseVector difference_to_analytic() const;

    // Realiza a operacao norms to previous definida por esta interface.
    [[nodiscard]] VariableDifferenceNorms1D norms_to_previous() const;
    // Realiza a operacao norms to iteration definida por esta interface.
    [[nodiscard]] VariableDifferenceNorms1D norms_to_iteration() const;
    // Realiza a operacao norms to analytic definida por esta interface.
    [[nodiscard]] VariableDifferenceNorms1D norms_to_analytic() const;

private:
    GridView1D grid_;
    std::string name_;
    DenseVector phi_;
    DenseVector phi_ant_;
    DenseVector phi_iter_;
    DenseVector phi_analit_;

    // Verifica se as hipoteses numericas e estruturais foram atendidas.
    void validate_size(const DenseVector& values) const;
    // Realiza a operacao copy phi to previous definida por esta interface.
    void copy_phi_to_previous();
    // Realiza a operacao difference to definida por esta interface.
    [[nodiscard]] DenseVector difference_to(const DenseVector& reference) const;
    // Realiza a operacao norms of definida por esta interface.
    [[nodiscard]] static VariableDifferenceNorms1D norms_of(
        const DenseVector& difference
    );
};

// Escreve uma representacao textual do objeto no fluxo de saida.
std::ostream& operator<<(std::ostream& os, const Variable1D& variable);

}  // namespace fvm
