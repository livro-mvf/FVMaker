// ============================================================================
// Arquivo: faceCentered.cpp
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Implementa face Centered no contexto de Grid / GridPattern.
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

#include <FVMaker/Grid/GridPattern/faceCentered.h>

GRID_NAMESPACE_OPEN

/**
 * @brief Cria uma copia (clone) deste objeto FaceCentered
 * @return std::shared_ptr<AbstractGridPattern> apontando para nova instancia clonada
 *
 * @note Utiliza o construtor de copia padrao para criar nova instancia
 */
std::shared_ptr<AbstractGridPattern> FaceCentered::Clone() const {
    return std::make_shared<FaceCentered>(*this);
}

/**
 * @brief Retorna o tipo do padrao de malha
 * @return "Face Centrada" - string que identifica este padrao de malha
 *
 * @note A string retornada e usada para identificacao e logging do padrao
 */
std::string FaceCentered::TipoPadraoMalha() const {
    return "Face Centrada";
}

GRID_NAMESPACE_CLOSE