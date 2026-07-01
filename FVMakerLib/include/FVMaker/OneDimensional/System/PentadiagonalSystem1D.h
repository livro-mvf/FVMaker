// ============================================================================
// Arquivo: PentadiagonalSystem1D.h
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Declara Pentadiagonal System 1 D no contexto de OneDimensional / System.
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

#include <span>
#include <string_view>
#include <vector>

#include <FVMaker/Algebra/DenseVector.h>
#include <FVMaker/Core/ID.h>
#include <FVMaker/Core/Types.h>

namespace fvm {

// Representa um sistema pentadiagonal usado por estenceis 1D mais largos.
class PentadiagonalSystem1D final {
public:
    // Cria um objeto PentadiagonalSystem1D com os dados fornecidos.
    explicit PentadiagonalSystem1D(Size size);

    // Cria um objeto PentadiagonalSystem1D com os dados fornecidos.
    PentadiagonalSystem1D(
        std::vector<Real> lower_second,
        std::vector<Real> lower_first,
        std::vector<Real> diagonal,
        std::vector<Real> upper_first,
        std::vector<Real> upper_second,
        DenseVector rhs
    );

    // Retorna o identificador estavel desta classe na biblioteca.
    [[nodiscard]] static constexpr ID id() noexcept {
        return ID{
            "OneDimensional",
            "PentadiagonalSystem1D",
            "fvm.1d.system.PentadiagonalSystem1D"
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

    // Retorna a informacao size associada ao objeto.
    [[nodiscard]] Size size() const noexcept;

    // Realiza a operacao lower second definida por esta interface.
    [[nodiscard]] std::span<Real> lower_second() noexcept;
    // Retorna a informacao lower second associada ao objeto.
    [[nodiscard]] std::span<const Real> lower_second() const noexcept;

    // Realiza a operacao lower first definida por esta interface.
    [[nodiscard]] std::span<Real> lower_first() noexcept;
    // Retorna a informacao lower first associada ao objeto.
    [[nodiscard]] std::span<const Real> lower_first() const noexcept;

    // Retorna a informacao diagonal armazenada no objeto.
    [[nodiscard]] std::span<Real> diagonal() noexcept;
    // Retorna a informacao diagonal armazenada no objeto.
    [[nodiscard]] std::span<const Real> diagonal() const noexcept;

    // Realiza a operacao upper first definida por esta interface.
    [[nodiscard]] std::span<Real> upper_first() noexcept;
    // Retorna a informacao upper first associada ao objeto.
    [[nodiscard]] std::span<const Real> upper_first() const noexcept;

    // Realiza a operacao upper second definida por esta interface.
    [[nodiscard]] std::span<Real> upper_second() noexcept;
    // Retorna a informacao upper second associada ao objeto.
    [[nodiscard]] std::span<const Real> upper_second() const noexcept;

    // Realiza a operacao rhs definida por esta interface.
    [[nodiscard]] DenseVector& rhs() noexcept;
    // Retorna a informacao rhs associada ao objeto.
    [[nodiscard]] const DenseVector& rhs() const noexcept;

    // Define a informacao set row usada pelo objeto.
    void set_row(
        Size row,
        Real lower_second,
        Real lower_first,
        Real diagonal,
        Real upper_first,
        Real upper_second,
        Real rhs
    );

private:
    std::vector<Real> lower_second_;
    std::vector<Real> lower_first_;
    std::vector<Real> diagonal_;
    std::vector<Real> upper_first_;
    std::vector<Real> upper_second_;
    DenseVector rhs_;

    // Verifica se as hipoteses numericas e estruturais foram atendidas.
    void validate() const;
};

}  // namespace fvm
