// ============================================================================
// Arquivo: programa_31.cpp
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Implementa programa 31 no contexto de antigos / capitulos / Capitulo_103a / Malhas / MalhaUniforme / Programa_31.
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

#include <FVMaker/Grid/Grid1D/uniformGrid1D.h>
#include <FVMaker/Grid/GridAnalysis/gridReport.h>

int main() {

    const Real length(1.0);
    const Real xInit(-4.0);
    const int  nVol(10);

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

    fvm::grd::uniformGrid1D ug1D(nVol, length, xInit);
    std::cout << ug1D << "\n";

    const auto& xCentro = ug1D.GetCentre();
    std::cout << "\nCoordenadas do centro da malha\n";
    for (auto xC : xCentro) {
        std::cout << xC << "\n";
    }

//==============================================================================
//  Análise da malha com os analisadores desenvolvidos
//==============================================================================

fvm::grd::GridReport<fvm::grd::uniformGrid1D> relatorio(ug1D);
relatorio.analyse();        // roda todos os analisadores 1D
std::cout << relatorio;     // mostra o resultado agregado

/*
    std::cout << "\n";
    fvm::grd::CellSizeAnalyser1D<fvm::grd::uniformGrid1D> analyser_dx(ug1D);
    analyser_dx.analyse();
    std::cout << analyser_dx << "\n\n";

    fvm::grd::SmoothnessAnalyser1D<fvm::grd::uniformGrid1D> analyser_smooth(ug1D);
    analyser_smooth.analyse();
    std::cout << analyser_smooth << "\n\n";

    fvm::grd::UniformityAnalyser1D<fvm::grd::uniformGrid1D> analyser_uniform(ug1D);
    analyser_uniform.analyse();
    std::cout << analyser_uniform << "\n";


    fvm::grd::SpacingRatioAnalyser1D<fvm::grd::uniformGrid1D> analyser_ratio(ug1D);
    analyser_ratio.analyse();
    std::cout << analyser_ratio << std::endl;
*/

    return EXIT_SUCCESS;
}
