// ============================================================================
// Arquivo: ex_FakeBlocks.cpp
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Implementa ex Fake Blocks no contexto de projeto.
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

#include <iostream>

#include <FVMaker/Roadmap/FakeBlocks.h>

int main() {
    std::cout << "Provisional roadmap calls\n";
    std::cout << "=========================\n";

    for (const fvm::roadmap::FakeBlockResult block :
         fvm::roadmap::remaining_blocks()) {
        std::cout << "Block " << block.block << ": " << block.name
                  << " callable=" << std::boolalpha << block.callable
                  << " provisional=" << block.provisional << '\n';
    }

    return fvm::roadmap::all_remaining_blocks_callable() ? 0 : 1;
}
