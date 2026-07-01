// ============================================================================
// Arquivo: GhostCells1D.h
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Declara Ghost Cells 1 D no contexto de OneDimensional / Boundary.
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

// Representa celulas fantasmas usadas para impor contornos em malhas 1D.
class GhostCells1D final {
public:
    // Cria um objeto GhostCells1D com os dados fornecidos.
    GhostCells1D() = default;
    // Cria um objeto GhostCells1D com os dados fornecidos.
    GhostCells1D(Size left, Size right);

    // Retorna o identificador estavel desta classe na biblioteca.
    [[nodiscard]] static constexpr ID id() noexcept {
        return ID{
            "OneDimensional",
            "GhostCells1D",
            "fvm.1d.boundary.GhostCells1D"
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

    // Retorna a informacao max per side associada ao objeto.
    [[nodiscard]] static constexpr Size max_per_side() noexcept {
        return 2;
    }

    // Retorna a informacao left armazenada no objeto.
    [[nodiscard]] Size left() const noexcept;
    // Retorna a informacao right armazenada no objeto.
    [[nodiscard]] Size right() const noexcept;
    // Retorna a informacao total associada ao objeto.
    [[nodiscard]] Size total() const noexcept;
    // Retorna a informacao empty associada ao objeto.
    [[nodiscard]] bool empty() const noexcept;

    // Retorna a informacao first physical index associada ao objeto.
    [[nodiscard]] Size first_physical_index() const noexcept;
    // Retorna a informacao storage size associada ao objeto.
    [[nodiscard]] Size storage_size(Size physical_size) const noexcept;
    // Retorna a informacao storage index associada ao objeto.
    [[nodiscard]] Size storage_index(Size physical_index) const noexcept;

private:
    Size left_{0};
    Size right_{0};

    // Verifica se as hipoteses numericas e estruturais foram atendidas.
    void validate() const;
};

}  // namespace fvm
