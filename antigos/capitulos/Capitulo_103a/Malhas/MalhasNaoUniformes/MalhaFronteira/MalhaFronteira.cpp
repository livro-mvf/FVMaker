// ============================================================================
// Arquivo: MalhaFronteira.cpp
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Implementa Malha Fronteira no contexto de antigos / capitulos / Capitulo_103a / Malhas / MalhasNaoUniformes / MalhaFronteira.
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

#include <FVMaker/Grid/Grid1D/RMGrid1D.h> ///< Definição da classe RMGrid para geração da malha nao uniforme
#include <FVMaker/Grid/GridPattern/cellCentered.h>
#include <FVMaker/Grid/GridPattern/faceCentered.h>

using fvm::grd::CellCentered;
using fvm::grd::FaceCentered;
using fvm::grd::RMGrid1D;

//==============================================================================
//      Função principal
//==============================================================================

/**
 * @brief Função principal que gera e exibe uma malha uniforme unidimensional.
 * 
 * A função define os parâmetros de configuração da malha, gera a malha utilizando a classe UniformGrid1D,
 * e imprime os dados de entrada e a malha resultante formatadamente na saída padrão.
 * 
 * @return int Código de saída do programa. Retorna EXIT_SUCCESS se a execução for bem-sucedida.
 */
int main() {
    
//==============================================================================
//      Dados de configuração da malha
//==============================================================================

    const Real LENGTH(4.0);    ///< Comprimento total do domínio
    const Real XINIT(12.0);    ///< Coordenada inicial do domínio
    const int NVOL(10);        ///< Número de volumes da malha
    const Real BETA(1.005);     ///<Beta
//==============================================================================
//      Impressao dos dados da malha
//==============================================================================

    auto flags = std::cout.flags();
    std::cout << "Dados iniciais do Malha Uniforme\n";
    fvm::PrintLine(std::cout);
    std::cout << std::fixed << std::setprecision(3);
    std::cout << "Comprimento do domínio unidimensional: " << std::setw(10) << LENGTH << "\n";
    std::cout << "Coordenada inicial da malha:           " << std::setw(10) << XINIT << "\n";
    std::cout << "Número de volumes finitos:             " << std::setw(10) << NVOL << "\n";
    std::cout << "Fator de concentração beta:            " << std::setw(10) << BETA << "\n";
    fvm::PrintLine(std::cout);

//==============================================================================
//      Geração e exibição da malha 
//==============================================================================

    fvm::grd::RMGrid1D<CellCentered> malhaCellCentered(BETA, NVOL, LENGTH, XINIT);
    std::cout << "\nMalha do tipo Volume Centrado";
    std::cout << malhaCellCentered << "\n";

    std::cout << "\nMalha do tipo Face Centrada";
    fvm::grd::RMGrid1D<FaceCentered> malhaFaceCentered(BETA, NVOL, LENGTH, XINIT);
    std::cout << malhaFaceCentered << "\n";

//==============================================================================
//      Acessando os vetores da malha
//==============================================================================
    
//auto    xCentro = *malhaFronteira1D.PtrUCCentreCoordinate();


//==============================================================================
//     Fim do programa
//==============================================================================

    return EXIT_SUCCESS;
}
