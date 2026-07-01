// ============================================================================
// Arquivo: Laplacian1D.h
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Declara Laplacian 1 D no contexto de OneDimensional / Operator.
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

#include <FVMaker/Algebra/DenseVector.h>
#include <FVMaker/Core/ID.h>
#include <FVMaker/Core/Types.h>
#include <FVMaker/OneDimensional/Coefficient/DiffusionCoefficient1D.h>
#include <FVMaker/OneDimensional/Grid/GridView1D.h>

namespace fvm {

// Representa o termo laplaciano, ou difusivo, discretizado em 1D.
class Laplacian1D final {
public:
    // Cria um objeto Laplacian1D com os dados fornecidos.
    explicit Laplacian1D(Real coefficient = Real{1.0});
    // Cria um objeto Laplacian1D com os dados fornecidos.
    explicit Laplacian1D(DiffusionCoefficient1D coefficient);

    // Retorna o identificador estavel desta classe na biblioteca.
    [[nodiscard]] static constexpr ID id() noexcept {
        return ID{
            "OneDimensional",
            "Laplacian1D",
            "fvm.1d.operator.Laplacian1D"
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

    // Retorna a informacao coefficient associada ao objeto.
    [[nodiscard]] Real coefficient() const noexcept;
    // Informa se a condicao has variable coefficient e verdadeira.
    [[nodiscard]] bool has_variable_coefficient() const noexcept;
    // Realiza a operacao face coefficient definida por esta interface.
    [[nodiscard]] Real face_coefficient(const GridView1D& grid, Size face) const;

private:
    Real coefficient_{1.0};
    DenseVector face_coefficients_;

    // Verifica se as hipoteses numericas e estruturais foram atendidas.
    void validate() const;
};

}  // namespace fvm
