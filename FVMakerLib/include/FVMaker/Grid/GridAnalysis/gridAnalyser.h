// ============================================================================
// Arquivo: gridAnalyser.h
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Declara grid Analyser no contexto de Grid / GridAnalysis.
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

// ==============================================================================
// INCLUDES - C++
// ==============================================================================
#include <iostream>

// ==============================================================================
// INCLUDES - FVMaker
// ==============================================================================
#include <FVMaker/Misc/namespace.h>

GRID_NAMESPACE_OPEN

/**
 * @brief Classe base abstrata para análises de malhas.
 * 
 * Esta classe define a interface para todas as classes de análise de malhas.
 * Toda classe concreta que produz resultados deverá implementar o método
 * `analyse()` e o método `print()`, que permite a saída formatada com `operator<<`.
 * 
 * @tparam GridType Tipo da malha a ser analisada (por exemplo: UniformGrid1D).
 */
template <typename GridType>
class GridAnalyser {
public:
    /**
     * @brief Construtor que recebe uma referência constante à malha.
     * 
     * @param grid Referência à malha a ser analisada.
     */
    explicit GridAnalyser(const GridType& _grid) : grid(_grid) {}

    /**
     * @brief Método padrão que executa a análise.
     * 
     * Cada classe concreta deve implementar sua lógica específica de análise.
     */
    void analyse() {}

    /**
     * @brief Método padrão para impressão formatada.
     * 
     * Este método é chamado pelo operador `<<` para apresentar os resultados.
     * 
     * @param os Stream de saída (como std::cout).
     */
    void print(std::ostream&) const {}

    /**
     * @brief Destrutor padrão.
     */
    ~GridAnalyser() = default;

    /**
     * @brief Sobrecarga do operador `<<` para qualquer classe concreta.
     * 
     * Permite usar `std::cout << object` diretamente para imprimir o resultado.
     * 
     * @param os Stream de saída.
     * @param analyser Referência ao analisador a ser impresso.
     * @return std::ostream& Stream modificado.
     */
    friend std::ostream& operator<<(std::ostream& os, const GridAnalyser& analyser) {
        analyser.print(os);
        return os;
    }

protected:
    const GridType& grid;  ///< Referência constante à malha analisada
};

GRID_NAMESPACE_CLOSE
