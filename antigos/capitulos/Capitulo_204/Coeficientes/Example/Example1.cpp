// ============================================================================
// Arquivo: Example1.cpp
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Implementa Example 1 no contexto de antigos / capitulos / Capitulo_204 / Coeficientes / Example.
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

#include <FVMaker/BoundaryCondition/boundaryCondition.h>        ///< Definição da classe Dirichlet para as condições de contorno
#include <FVMaker/BoundaryCondition/dirichlet.h>        ///< Definição da classe Dirichlet para as condições de contorno
#include <FVMaker/BoundaryCondition/neumann.h>          ///< Definição da classe Neumann para as condições de contorno
#include <FVMaker/Coefficient/coefficient1D.h>          ///< Definição da classe Coefficient para os coeficientes unidimensional da equação diferencial
#include <FVMaker/Equation/diffusion.h>                 ///< Definição da classe Diffusion para o termo de difusao
#include <FVMaker/Grid/Grid1D/uniformGrid1D.h>          ///< Definição da classe uniformGrid1D para geração da malha uniforme


//==============================================================================
//      Função principal
//==============================================================================

/**
 * @brief Função principal que gera e exibe uma malha uniforme unidimensional.
 * 
 * A função define os parâmetros de configuração da malha, gera a malha utilizando a classe uniformGrid1D,
 * e imprime os dados de entrada e a malha resultante formatadamente na saída padrão.
 * 
 * @return int Código de saída do programa. Retorna EXIT_SUCCESS se a execução for bem-sucedida.
 */
int main() {
    
//==============================================================================
//      Dados de configuração da malha
//==============================================================================

    const Real length(5.0);    ///< Comprimento total do domínio
    const Real xInit(0.0);    ///< Coordenada inicial do domínio
    const int nVol(5);        ///< Número de volumes da malha

//==============================================================================
//      Impressao dos dados da malha
//==============================================================================

    auto flags = std::cout.flags();
    std::cout << "Dados iniciais do Malha\n";
    fvm::PrintLine(std::cout);
    std::cout << std::fixed << std::setprecision(3);
    std::cout << "Comprimento do domínio unidimensional: " << std::setw(10) << length << "\n";
    std::cout << "Coordenada inicial da malha:           " << std::setw(10) << xInit << "\n";
    std::cout << "Número de volumes finitos:             " << std::setw(10) << nVol << "\n";
    fvm::PrintLine(std::cout);

//==============================================================================
//      Geração e exibição da malha uniforme
//==============================================================================

    fvm::grd::uniformGrid1D ug1D(nVol, length, xInit);
    std::cout << ug1D << "\n";

//==============================================================================
//      Definicao das condições de contorno
//==============================================================================

    fvm::Dirichlet      cc_w(1);
    fvm::Neumann        cc_e(0);
 
    fvm::BoundaryConditions<fvm::grd::uniformGrid1D>       bc1D = makeBC<fvm::grd::uniformGrid1D>(cc_w, cc_e);

//==============================================================================
//      Definicao das condições de contorno
//==============================================================================
    
    fvm::Coefficient1D<fvm::grd::uniformGrid1D>          coeff(ug1D);
    std::cout <<  coeff << "\n\n";


//==============================================================================
//      Definição da função
//==============================================================================

    
fvm::Function<fvm::grd::uniformGrid1D>        fx(ug1D);       
auto TermoFonte = [] (const Real& _x) -> fvm::PairData { 
    return fvm::PairData(_x * _x, 0);
};

    fx.setFunction(TermoFonte);
    

    
//==============================================================================
//      Definicao dos coeficientes do sistema de equações lineares
//==============================================================================
    
    fvm::Diffusion<fvm::grd::uniformGrid1D>    eq_diff(ug1D, coeff, bc1D, &fx);
    bool flag = eq_diff.ComputeCoefficient();
    
    
    std::cout << eq_diff << "\n\n";
    
//==============================================================================
//     Fim do programa
//==============================================================================

    return EXIT_SUCCESS;
}
