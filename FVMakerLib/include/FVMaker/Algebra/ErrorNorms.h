// ============================================================================
// Arquivo: ErrorNorms.h
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Declara Error Norms no contexto de Algebra.
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
// FVMaker includes
// ----------------------------------------------------------------------------
#include <FVMaker/Algebra/DenseVector.h>
#include <FVMaker/Core/Types.h>

namespace fvm {

enum class NormType {
    infinity,
    l1,
    l2,
    rms,
};

class VectorNorms final {
public:
    [[nodiscard]] static Real evaluate(
        const DenseVector& vector,
        NormType type
    ) noexcept;

    [[nodiscard]] static Real infinity(const DenseVector& vector) noexcept;
    [[nodiscard]] static Real l1(const DenseVector& vector) noexcept;
    [[nodiscard]] static Real l2(const DenseVector& vector) noexcept;
    [[nodiscard]] static Real rms(const DenseVector& vector) noexcept;
};

[[nodiscard]] Real norm_infinity(const DenseVector& vector) noexcept;
[[nodiscard]] Real norm_l1(const DenseVector& vector) noexcept;
[[nodiscard]] Real norm_l2(const DenseVector& vector) noexcept;
[[nodiscard]] Real norm_rms(const DenseVector& vector) noexcept;

}  // namespace fvm
