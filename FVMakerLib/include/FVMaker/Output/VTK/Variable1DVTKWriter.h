// ============================================================================
// Arquivo: Variable1DVTKWriter.h
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Declara Variable 1 DVTKWriter no contexto de Output / VTK.
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

#include <filesystem>
#include <iosfwd>
#include <string_view>

#include <FVMaker/Core/ID.h>
#include <FVMaker/OneDimensional/Variable/Variable1D.h>

namespace fvm {

// Escreve variaveis 1D em formato VTK para pos-processamento.
class Variable1DVTKWriter final {
public:
    // Retorna o identificador estavel desta classe na biblioteca.
    [[nodiscard]] static constexpr ID id() noexcept {
        return ID{
            "Output",
            "Variable1DVTKWriter",
            "fvm.output.vtk.Variable1DVTKWriter"
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

    // Realiza a operacao write definida por esta interface.
    static void write(const Variable1D& variable, std::ostream& output);
    // Realiza a operacao write definida por esta interface.
    static void write(
        const Variable1D& variable,
        const std::filesystem::path& filepath
    );
};

}  // namespace fvm
