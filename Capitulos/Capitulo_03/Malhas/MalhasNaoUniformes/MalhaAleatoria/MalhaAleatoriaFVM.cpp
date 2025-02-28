////==============================================================================
//// Nome        : MalhaRandome.cpp
//// Autor       : Joao Flavio Vieira de Vasconcellos
//// Versão      : 1.1
//// Descrição   : Programa que calcula as coordenadas dos nós
////               para uma malha uniforme unidimensional para volumes finitos
//// Livro       : Código 2.2 do livro
////               Solução Numérica de Equações Diferenciais - 
////               O Método de Volumes Finitos
//// Testado     : gcc 10.2 usando c++20 em 17 / fev / 2024 
////
//// Direitos autorais : Copyright (C) <2025> Joao Flavio Vasconcellos 
////                    (jflavio at iprj.uerj.br)
////
//// Este programa é software livre: você pode redistribuí-lo e/ou
//// modificá-lo sob os termos da Licença Pública Geral GNU como
//// publicada pela Free Software Foundation, tanto a versão 3 da
//// Licença como (a seu critério) qualquer versão posterior.
////
//// Este programa é distribuído na esperança de que seja útil,
//// mas SEM NENHUMA GARANTIA; sem mesmo a garantia implícita de
//// COMERCIABILIDADE ou ADEQUAÇÃO A QUALQUER PROPÓSITO EM
//// PARTICULAR. Consulte a Licença Pública Geral GNU para mais
//// detalhes.
////
//// Você deve ter recebido uma cópia da Licença Pública Geral GNU
//// junto com este programa. Se não, veja <http://www.gnu.org/licenses/>.
////==============================================================================
//
///**
// * @file MalhaRandome.cpp
// * @brief Calcula as coordenadas dos nós para uma malha uniforme unidimensional.
// * 
// * @details
// * Este programa demonstra como gerar uma malha uniforme unidimensional para o método dos volumes finitos
// * utilizando a classe RandomGrid1D da biblioteca FVMaker.
// * 
// * A malha é definida pelos seguintes parâmetros:
// *  - Comprimento total do domínio.
// *  - Coordenada inicial do domínio.
// *  - Número de volumes (ou células) da malha.
// * 
// * O fluxo do programa é o seguinte:
// *  1. Configura os parâmetros de entrada e exibe-os formatadamente.
// *  2. Gera a malha uniforme utilizando a classe RandomGrid1D.
// *  3. Imprime as coordenadas dos nós resultantes.
// *    ```
// * 
// * @version 1.1
// * @date 2024-11-30
// * @author 
// *         João Flávio Vasconcellos
// * 
// * @copyright
// *         Copyright (c) 2025 João Flávio Vasconcellos.
// * @license GNU GPL v3
// */
//
//==============================================================================
//      Includes da Biblioteca FVMaker
//==============================================================================

#include <FVMAKER/Grid/Grid1D/RandomGrid1D.h>           ///< Definição da classe RandomGrid1D para geração da malha aleatoria
#include <FVMAKER/Grid/GridPattern/CellCentered.h>
#include <FVMAKER/Grid/GridPattern/FaceCentered.h>

using fvm::grd::CellCentered;
using fvm::grd::FaceCentered;
using fvm::grd::RandomGrid1D;

//==============================================================================
//      Função principal
//==============================================================================

/**
 * @brief Função principal que gera e exibe uma malha uniforme unidimensional.
 * 
 * A função define os parâmetros de configuração da malha, gera a malha utilizando a classe RandomGrid1D,
 * e imprime os dados de entrada e a malha resultante formatadamente na saída padrão.
 * 
 * @return int Código de saída do programa. Retorna EXIT_SUCCESS se a execução for bem-sucedida.
 */
int main() {
    
//==============================================================================
//      Dados de configuração da malha
//==============================================================================

    const Real LENGTH(1.0);    ///< Comprimento total do domínio
    const Real XINIT(-4.0);    ///< Coordenada inicial do domínio
    const int NVOL(10);        ///< Número de volumes da malha

//==============================================================================
//      Impressao dos dados da malha
//==============================================================================

    auto flags = std::cout.flags();
    std::cout << "Dados iniciais do Malha Randome\n";
    fvm::PrintLine(std::cout);
    std::cout << std::fixed << std::setprecision(3);
    std::cout << "Comprimento do domínio unidimensional: " << std::setw(10) << LENGTH << "\n";
    std::cout << "Coordenada inicial da malha:           " << std::setw(10) << XINIT << "\n";
    std::cout << "Número de volumes finitos:             " << std::setw(10) << NVOL << "\n";
    fvm::PrintLine(std::cout);

//==============================================================================
//      Geração e exibição da malha aleatoria usando VolumeCentrado
//==============================================================================

    RandomGrid1D<CellCentered> ug1D(NVOL, LENGTH, XINIT);
    std::cout << "\nImpressao da malha aleatorio do tipo Volume Centrado";
    std::cout << ug1D << "\n";

//==============================================================================
//      Geração e exibição da malha aleatoria usando FaceCentrada
//==============================================================================

    fvm::grd::RandomGrid1D<FaceCentered> ug2D(NVOL, LENGTH, XINIT);
    std::cout << "\nImpressao da malha aleatorio do tipo Face Centrada";
    std::cout << ug2D << "\n";
    
//==============================================================================
//     Fim do programa
//==============================================================================

    return EXIT_SUCCESS;
}
