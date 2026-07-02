// ============================================================================
// Arquivo: Assembler1D.h
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Declara Assembler 1 D no contexto de OneDimensional / Assembly.
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

#include <iosfwd>

#include <FVMaker/OneDimensional/Equation/Equation1D.h>
#include <FVMaker/OneDimensional/System/TridiagonalSystem1D.h>

namespace fvm {

// Monta a representacao algebrica associada aos dados fornecidos.
[[nodiscard]] TridiagonalSystem1D assemble_laplacian_1d(
    const GridView1D& grid,
    const Laplacian1D& laplacian,
    const BoundarySet1D& boundaries,
    Real time = Real{}
);

// Monta a representacao algebrica associada aos dados fornecidos.
[[nodiscard]] TridiagonalSystem1D assemble_steady_1d(
    const Equation1D& equation,
    Real time = Real{}
);

// Monta a equacao com assemble_steady_1d e imprime A_W, A_P, A_E e B_P na
// convencao do livro (ver operator<<(std::ostream&, const
// TridiagonalSystem1D&)). Equivale a `os << assemble_steady_1d(equation)`,
// mas permite escrever diretamente `std::cout << equation`.
//
// A montagem tem custo O(N); nao ha problema em chamar este operador varias
// vezes para inspecionar a mesma equacao, mas para reaproveitar o sistema
// montado (por exemplo, para tambem resolve-lo) prefira montar uma vez com
// assemble_steady_1d e imprimir o TridiagonalSystem1D resultante.
std::ostream& operator<<(std::ostream& os, const Equation1D& equation);

}  // namespace fvm
