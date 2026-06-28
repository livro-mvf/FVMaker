// ============================================================================
// Arquivo: cellCentered.cpp
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Implementa cell Centered no contexto de Grid / GridPattern.
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

#include <FVMaker/Grid/GridPattern/cellCentered.h>

GRID_NAMESPACE_OPEN

/**
 * @brief Cria uma cópia (clone) deste objeto CellCentered
 * @return std::shared_ptr<AbstractGridPattern> apontando para a nova instância clonada
 *
 * @details Utiliza o construtor de cópia padrão para criar uma nova instância idêntica.
 * A instância criada é gerenciada por um shared_ptr para permitir gerenciamento automático
 * de memória.
 *
 * @note Este método é uma implementação do método virtual puro da classe base AbstractGridPattern
 * @post O objeto retornado é uma cópia exata do objeto original
 *
 * @see AbstractGridPattern::Clone()
 */
std::shared_ptr<AbstractGridPattern> CellCentered::Clone() const {
    return std::make_shared<CellCentered>(*this);
}

/**
 * @brief Retorna o tipo do padrão de malha
 * @return "Volume Centrado" - string que identifica este padrão de malha
 *
 * @details A string retornada é utilizada para:
 * - Identificação do tipo de padrão em logs e mensagens
 * - Serialização/deserialização de objetos
 * - Propósitos de depuração e diagnóstico
 *
 * @note Este método é uma implementação do método virtual puro da classe base AbstractGridPattern
 * @see AbstractGridPattern::TipoPadraoMalha()
 */
std::string CellCentered::TipoPadraoMalha() const {
    return "Volume Centrado";
}

GRID_NAMESPACE_CLOSE