// ============================================================================
// Arquivo: ManufacturedSolution1D.h
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Declara Manufactured Solution 1 D no contexto de Functions.
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

// Representa o conceito de manufactured solution1 d dentro da biblioteca FVMaker.
struct ManufacturedSolution1D final {
    StoredScalarFunction1D phi;
    StoredScalarFunction1D dphi_dx;
    StoredScalarFunction1D d2phi_dx2;

    // Retorna a informacao value armazenada no objeto.
    [[nodiscard]] Real value(Real x) const {
        return phi(x);
    }

    // Realiza a operacao first derivative definida por esta interface.
    [[nodiscard]] Real first_derivative(Real x) const {
        return dphi_dx(x);
    }

    // Realiza a operacao second derivative definida por esta interface.
    [[nodiscard]] Real second_derivative(Real x) const {
        return d2phi_dx2(x);
    }
};

}  // namespace fvm
