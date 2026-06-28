// ============================================================================
// Arquivo: MalhaAleatoria1D.cpp
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Implementa Malha Aleatoria 1 D no contexto de antigos / capitulos / Capitulo_103a / Malhas / MalhasNaoUniformes / MalhaAleatoria.
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

#include <FVMaker/Grid/Grid1D/randomGrid1D.h>  ///< Definição da classe RandomGrid1D para geração
                                               ///< da malha aleatória
#include <FVMaker/Grid/GridPattern/cellCentered.h>
#include <FVMaker/Grid/GridPattern/faceCentered.h>
#include <FVMaker/Grid/Grid1D/uniformGrid1D.h>
#include <FVMaker/Grid/GridAnalysis/gridReport.h>

using fvm::grd::CellCentered;
using fvm::grd::FaceCentered;
using fvm::grd::RandomGrid1D;

//==============================================================================
//      Função principal
//==============================================================================

/**
 * @brief Função principal que gera e exibe uma malha aleatória unidimensional.
 *
 * A função define os parâmetros de configuração da malha, gera a malha
 * utilizando a classe RandomGrid1D e imprime os dados de entrada, bem como
 * as coordenadas dos centros dos volumes e das faces, formatadamente na
 * saída padrão.
 *
 * @return int Código de saída do programa. Retorna EXIT_SUCCESS se a
 *             execução for bem-sucedida.
 */
int main() {
    
    //==============================================================================
    //      Dados de configuração da malha
    //==============================================================================
    
    const Real LENGTH(4.0);    ///< Comprimento total do domínio
    const Real XINIT(-2.0);    ///< Coordenada inicial do domínio
    const int NVOL(10);        ///< Número de volumes da malha

    //==============================================================================
    //      Impressão dos dados da malha
    //==============================================================================
    
    auto flags = std::cout.flags();
    std::cout << "Dados iniciais da malha de volumes finitos aleatória\n";
    fvm::PrintLine(std::cout);
    std::cout << std::fixed << std::setprecision(3);
    std::cout << "Comprimento do domínio unidimensional: " 
              << std::setw(10) << LENGTH << "\n";
    std::cout << "Coordenada inicial do domínio:         " 
              << std::setw(10) << XINIT << "\n";
    std::cout << "Número de volumes finitos:              " 
              << std::setw(10) << NVOL << "\n";
    fvm::PrintLine(std::cout);
    
    //==============================================================================
    //      Geração e exibição da malha aleatória usando Volume Centrado
    //==============================================================================
    
    RandomGrid1D<CellCentered> ug1D(NVOL, LENGTH, XINIT);
    std::cout << "\nImpressão das coordenadas dos volumes "
                 "(Volume Centrado):";
    std::cout << ug1D << "\n";
    
    //==============================================================================
    //      Geração e exibição da malha aleatória usando Face Centrada
    //==============================================================================
    
    fvm::grd::RandomGrid1D<FaceCentered> ug2D(NVOL, LENGTH, XINIT);
    std::cout << "\nImpressão das coordenadas dos volumes "
                 "(Face Centrada):";
    std::cout << ug2D << "\n";
    
    //==============================================================================
    //  Análise da malha com os analisadores desenvolvidos
    //==============================================================================

fvm::grd::GridReport<RandomGrid1D<CellCentered>> relatorio(ug1D);

    relatorio.analyse();        // roda todos os analisadores 1D
    std::cout << relatorio;     // mostra o resultado agregado


/*
    std::cout << "\n";
    fvm::grd::CellSizeAnalyser1D< RandomGrid1D<CellCentered> > analyser_dx(ug1D);
    analyser_dx.analyse();
    std::cout << analyser_dx << "\n\n";

    fvm::grd::SmoothnessAnalyser1D< RandomGrid1D<CellCentered> > analyser_smooth(ug1D);
    analyser_smooth.analyse();
    std::cout << analyser_smooth << "\n\n";

    fvm::grd::UniformityAnalyser1D< RandomGrid1D<CellCentered> > analyser_uniform(ug1D);
    analyser_uniform.analyse();
    std::cout << analyser_uniform << "\n";


    fvm::grd::SpacingRatioAnalyser1D< RandomGrid1D<CellCentered> > analyser_ratio(ug1D);
    analyser_ratio.analyse();
    std::cout << analyser_ratio << std::endl;

*/
    //==============================================================================
    //     Fim do programa
    //==============================================================================
    
    return EXIT_SUCCESS;
}
