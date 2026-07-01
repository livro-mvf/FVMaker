// ============================================================================
// Arquivo: Field1D.h
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Declara Field 1 D no contexto de OneDimensional / Field.
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

// Representa valores escalares associados aos pontos de uma malha 1D.
class Field1D final {
public:
    // Cria um objeto Field1D com os dados fornecidos.
    Field1D(GridView1D grid, std::string name);

    // Cria um objeto Field1D com os dados fornecidos.
    Field1D(
        GridView1D grid,
        std::string name,
        Real uniform_value,
        bool keep_history = false
    );

    // Cria um objeto Field1D com os dados fornecidos.
    Field1D(
        GridView1D grid,
        std::string name,
        std::vector<Real> values,
        bool keep_history = false
    );

    template <class Function>
        requires std::invocable<Function, Real> &&
                 std::convertible_to<std::invoke_result_t<Function, Real>, Real>
    // Realiza a operacao from function definida por esta interface.
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

    // Retorna o identificador estavel desta classe na biblioteca.
    [[nodiscard]] static constexpr ID id() noexcept {
        return ID{
            "OneDimensional",
            "Field1D",
            "fvm.1d.field.Field1D"
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

    // Retorna a informacao values armazenada no objeto.
    [[nodiscard]] std::span<Real> values() noexcept;
    // Retorna a informacao values armazenada no objeto.
    [[nodiscard]] std::span<const Real> values() const noexcept;

    // Define o comportamento do operador usado por esta abstracao.
    [[nodiscard]] Real& operator[](Size index) noexcept;
    // Define o comportamento do operador usado por esta abstracao.
    [[nodiscard]] const Real& operator[](Size index) const noexcept;

    // Realiza a operacao fill definida por esta interface.
    void fill(Real value);

    template <class Function>
        requires std::invocable<Function, Real> &&
                 std::convertible_to<std::invoke_result_t<Function, Real>, Real>
    // Realiza a operacao assign function definida por esta interface.
    void assign_function(Function&& function) {
        const auto centers = grid_.centers();

        for (Size i = 0; i < values_.size(); ++i) {
            values_[i] = static_cast<Real>(std::invoke(function, centers[i]));
        }
    }

    // Realiza a operacao assign values definida por esta interface.
    void assign_values(std::vector<Real> values);

    // Realiza a operacao view definida por esta interface.
    [[nodiscard]] FieldView1D view() noexcept;

    // Retorna a informacao history enabled associada ao objeto.
    [[nodiscard]] bool history_enabled() const noexcept;
    // Retorna a informacao history size associada ao objeto.
    [[nodiscard]] Size history_size() const noexcept;
    // Realiza a operacao history step definida por esta interface.
    [[nodiscard]] std::span<const Real> history_step(Size index) const;
    // Realiza a operacao save state definida por esta interface.
    void save_state();
    // Realiza a operacao clear history definida por esta interface.
    void clear_history();

private:
    GridView1D grid_;
    std::string name_;
    std::vector<Real> values_;
    bool keep_history_{false};
    std::vector<Real> history_;

    // Verifica se as hipoteses numericas e estruturais foram atendidas.
    void validate_size() const;
    // Realiza a operacao record initial state definida por esta interface.
    void record_initial_state();
};

}  // namespace fvm
