
//==============================================================================
// Nome        : Programa_31.cpp
// Autor       : Joao Flavio Vieira de Vasconcellos
// Versão      : 1.1
// Descrição   : Programa que calcula as coordenadas dos nós para uma malha
//               uniforme unidimensional para volumes finitos
// Livro       : Código 3.1 do livro
//               Solução Numérica de Equações Diferenciais - 
//               O Método de Volumes Finitos
// Testado     : gcc 10.2 usando c++20 em 12 / mar / 2025 
//
// Direitos autorais : Copyright (C) <2025> Joao Flavio Vasconcellos 
//                    (jflavio at iprj.uerj.br)
//
// Este programa é software livre: você pode redistribuí-lo e/ou
// modificá-lo sob os termos da Licença Pública Geral GNU como
// publicada pela Free Software Foundation, tanto a versão 3 da
// Licença como (a seu critério) qualquer versão posterior.
//
// Este programa é distribuído na esperança de que seja útil,
// mas SEM NENHUMA GARANTIA; sem mesmo a garantia implícita de
// COMERCIABILIDADE ou ADEQUAÇÃO A QUALQUER PROPÓSITO EM
// PARTICULAR. Consulte a Licença Pública Geral GNU para mais
// detalhes.
//
// Você deve ter recebido uma cópia da Licença Pública Geral GNU
// junto com este programa. Se não, veja <http://www.gnu.org/licenses/>.
//==============================================================================

/**
 * @file MalhaUniforme.cpp
 * @brief Calcula as coordenadas dos nós para uma malha unidimensional
 *        uniforme.
 * 
 * @details Este programa demonstra como gerar uma malha unidimensional
 *          uniforme para o método dos volumes finitos, utilizando a
 *          classe UniformGrid1D da biblioteca FVMaker.
 * 
 *          A malha é definida pelos seguintes parâmetros:
 *           - Comprimento total do domínio.
 *           - Coordenada inicial do domínio.
 *           - Número de volumes (ou células) da malha.
 * 
 *          O fluxo do programa é o seguinte:
 *           1. Configura os parâmetros de entrada e os exibe formatadamente.
 *           2. Gera a malha utilizando a classe UniformGrid1D.
 *           3. Imprime as coordenadas dos nós resultantes.
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
//  Inclusões da biblioteca FVMaker
//==============================================================================

/// Definição da classe UniformGrid1D para geração da malha uniforme
#include <FVMAKER/Grid/Grid1D/UniformGrid1D.h>

//==============================================================================
//  Função principal
//==============================================================================

/**
 * @brief Função principal que gera e exibe uma malha unidimensional uniforme.
 * 
 * A função define os parâmetros de configuração da malha, gera a malha
 * utilizando a classe UniformGrid1D e imprime os dados de entrada, bem
 * como a malha resultante, formatadamente na saída padrão.
 * 
 * @return int Código de saída do programa. Retorna EXIT_SUCCESS se a
 *             execução for bem-sucedida.
 */
int main() {

    //==============================================================================
    //  Dados de configuração da malha
    //==============================================================================

    const Real length(1.0);      ///< Comprimento total do domínio
    const Real xInit(-4.0);      ///< Coordenada inicial do domínio
    const int  nVol(10);         ///< Número de volumes da malha

    //==============================================================================
    //  Impressão dos dados da malha
    //==============================================================================

    auto flags = std::cout.flags();
    std::cout << "Dados iniciais da malha uniforme\n";
    fvm::PrintLine(std::cout);
    std::cout << std::fixed << std::setprecision(3);
    std::cout << "Comprimento do domínio unidimensional: " 
              << std::setw(10) << length << "\n";
    std::cout << "Coordenada inicial da malha:           " 
              << std::setw(10) << xInit << "\n";
    std::cout << "Número de volumes finitos:             " 
              << std::setw(10) << nVol << "\n";
    fvm::PrintLine(std::cout);

    //==============================================================================
    //  Geração e exibição da malha uniforme
    //==============================================================================

    fvm::grd::UniformGrid1D ug1D(nVol, length, xInit);
    std::cout << ug1D << "\n";

    //==============================================================================
    //  Acesso e impressão dos vetores da malha
    //==============================================================================

    const auto& xCentro = ug1D.CentreCoordinate();

        std::cout << "\nCoordenadas do centro da malha\n";
        for (auto xC : xCentro)
        {
            std::cout << xC << "\n";
        }


    //==============================================================================
    //  Fim do programa
    //==============================================================================

    return EXIT_SUCCESS;
}
