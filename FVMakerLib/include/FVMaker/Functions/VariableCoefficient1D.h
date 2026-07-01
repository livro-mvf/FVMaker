// ============================================================================
// Arquivo: VariableCoefficient1D.h
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Declara Variable Coefficient 1 D no contexto de Functions.
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
#include <FVMaker/Core/Types.h>
#include <FVMaker/Functions/FunctionTypes.h>

namespace fvm {

// Representa o conceito de variable coefficient1 d dentro da biblioteca FVMaker.
struct VariableCoefficient1D final {
    StoredScalarFunction1D gamma;
    StoredScalarFunction1D dgamma_dx;

    // Retorna a informacao value armazenada no objeto.
    [[nodiscard]] Real value(Real x) const {
        return gamma(x);
    }

    // Realiza a operacao derivative definida por esta interface.
    [[nodiscard]] Real derivative(Real x) const {
        return dgamma_dx(x);
    }
};

}  // namespace fvm
