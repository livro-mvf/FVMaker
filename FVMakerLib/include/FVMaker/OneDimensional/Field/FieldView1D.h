// ============================================================================
// Arquivo: FieldView1D.h
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Declara Field View 1 D no contexto de OneDimensional / Field.
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
#include <span>
#include <string_view>

// ----------------------------------------------------------------------------
// FVMaker includes
// ----------------------------------------------------------------------------
#include <FVMaker/Core/ID.h>
#include <FVMaker/Core/Types.h>
#include <FVMaker/OneDimensional/Grid/GridView1D.h>

namespace fvm {

// Oferece uma visao leve e nao proprietaria de um campo escalar 1D.
class FieldView1D final {
public:
    // Cria um objeto FieldView1D com os dados fornecidos.
    FieldView1D(GridView1D grid, std::string_view name, std::span<Real> values);

    // Retorna o identificador estavel desta classe na biblioteca.
    [[nodiscard]] static constexpr ID id() noexcept {
        return ID{
            "OneDimensional",
            "FieldView1D",
            "fvm.1d.field.FieldView1D"
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

private:
    GridView1D grid_;
    std::string_view name_;
    std::span<Real> values_;

    // Verifica se as hipoteses numericas e estruturais foram atendidas.
    void validate() const;
};

}  // namespace fvm
