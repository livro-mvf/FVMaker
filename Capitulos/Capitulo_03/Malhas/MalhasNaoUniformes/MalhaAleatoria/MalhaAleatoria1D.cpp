//==============================================================================
// Nome        : MalhaAleatoria1D.cpp
// Autor       : Joao Flavio Vieira de Vasconcellos
// Versão      : 1.1
// Descrição   : Programa que calcula as coordenadas das faces e dos centros
//               dos volumes para uma malha aleatória unidimensional de
//               volumes finitos
// Livro       : Código do livro
//               Solução Numérica de Equações Diferenciais -
//               O Método de Volumes Finitos
// Testado     : gcc 10.2 usando c++20 em 12 / mar / 2025
//
// Direitos autorais : Copyright (C) <2025> Joao Flavio Vasconcellos
//                    (jflavio at iprj.uerj.br)
//
// Este programa é software livre: você pode redistribuí-lo e/ou
// modificá-lo sob os termos da Licença Pública Geral GNU, conforme
// publicada pela Free Software Foundation, na versão 3 da Licença ou
// (a seu critério) qualquer versão posterior.
//
// Este programa é distribuído na esperança de que seja útil, mas SEM
// NENHUMA GARANTIA; sem mesmo a garantia implícita de COMERCIABILIDADE
// ou ADEQUAÇÃO A QUALQUER PROPÓSITO EM PARTICULAR. Consulte a Licença
// Pública Geral GNU para mais detalhes.
//
// Você deve ter recebido uma cópia da Licença Pública Geral GNU junto
// com este programa. Se não, veja <http://www.gnu.org/licenses/>.
//==============================================================================

/**
 * @file MalhaAleatoria1D.cpp
 * @brief Calcula as coordenadas das faces e dos centros dos volumes para uma
 *        malha aleatória.
 *
 * @details
 * Este programa demonstra como gerar uma malha aleatória unidimensional
 * para o método dos volumes finitos, utilizando a classe RandomGrid1D da
 * biblioteca FVMaker.
 *
 * A malha é definida pelos seguintes parâmetros:
 *  - Comprimento total do domínio.
 *  - Coordenada inicial do domínio.
 *  - Número de volumes (ou células) da malha.
 *
 * O fluxo do programa é o seguinte:
 *  1. Configura os parâmetros de entrada e os exibe formatadamente.
 *  2. Gera a malha aleatória utilizando a classe RandomGrid1D.
 *  3. Imprime as coordenadas dos centros dos volumes e das faces,
 *     demonstrando dois exemplos:
 *      - Volume Centrado.
 *      - Face Centrada.
 *
 * @version 1.1
 * @date 2025-03-12
 * @author 
 *         João Flávio Vasconcellos
 *
 * @copyright
 *         Copyright (c) 2025 João Flávio Vasconcellos.
 * @license GNU GPL v3
 */

//==============================================================================
//      Inclusões da Biblioteca FVMaker
//==============================================================================

#include <FVMAKER/Grid/Grid1D/RandomGrid1D.h>  ///< Definição da classe RandomGrid1D para geração
                                               ///< da malha aleatória
#include <FVMAKER/Grid/GridPattern/CellCentered.h>
#include <FVMAKER/Grid/GridPattern/FaceCentered.h>
#include <FVMAKER/Grid/Grid1D/UniformGrid1D.h>
#include <FVMAKER/Grid/GridAnalysis/GridReport.h>

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
