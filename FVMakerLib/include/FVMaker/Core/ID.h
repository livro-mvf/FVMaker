// ============================================================================
// Arquivo: ID.h
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Declara ID no contexto de Core.
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
#include <string_view>

namespace fvm {

// Representa identificadores estaveis de classes e modulos da biblioteca.
class ID final {
public:
    // Cria um objeto ID com os dados fornecidos.
    constexpr ID() noexcept = default;

    // Cria um objeto ID com os dados fornecidos.
    constexpr ID(
        std::string_view module,
        std::string_view class_name,
        std::string_view class_id
    ) noexcept
        : module_(module),
          class_name_(class_name),
          class_id_(class_id) {}

    // Retorna a informacao module associada ao objeto.
    [[nodiscard]] constexpr std::string_view module() const noexcept {
        return module_;
    }

    // Retorna o nome curto da classe para diagnostico e documentacao.
    [[nodiscard]] constexpr std::string_view class_name() const noexcept {
        return class_name_;
    }

    // Retorna o identificador completo da classe na hierarquia da biblioteca.
    [[nodiscard]] constexpr std::string_view class_id() const noexcept {
        return class_id_;
    }

private:
    std::string_view module_ = "Unknown";
    std::string_view class_name_ = "Unknown";
    std::string_view class_id_ = "fvm.unknown.Unknown";
};

}  // namespace fvm
