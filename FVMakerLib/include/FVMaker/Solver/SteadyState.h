// ============================================================================
// Arquivo: SteadyState.h
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Declara Steady State no contexto de Solver.
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

#include <string_view>

#include <FVMaker/Core/ID.h>
#include <FVMaker/Core/Types.h>

namespace fvm {

// Representa o resultado de uma evolucao ate estado estacionario.
class SteadyState final {
public:
    Real absolute_tolerance{1.0e-10};
    Real relative_tolerance{1.0e-8};
    Size max_iterations{10000};

    // Retorna o identificador estavel desta classe na biblioteca.
    [[nodiscard]] static constexpr ID id() noexcept {
        return ID{"Solver", "SteadyState", "fvm.solver.SteadyState"};
    }

    // Retorna o nome curto da classe para diagnostico e documentacao.
    [[nodiscard]] static constexpr std::string_view class_name() noexcept {
        return id().class_name();
    }

    // Retorna o identificador completo da classe na hierarquia da biblioteca.
    [[nodiscard]] static constexpr std::string_view class_id() noexcept {
        return id().class_id();
    }

    // Realiza a operacao effective tolerance definida por esta interface.
    [[nodiscard]] Real effective_tolerance(Real reference_norm) const;
    // Verifica se as hipoteses numericas e estruturais foram atendidas.
    void validate() const;
};

}  // namespace fvm
