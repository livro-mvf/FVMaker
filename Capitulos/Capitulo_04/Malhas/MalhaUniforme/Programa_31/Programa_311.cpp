//==============================================================================
// Nome        : Programa_31.cpp
// Versão      : 1.2 (com análise de malha)
//==============================================================================

#include <FVMAKER/Grid/Grid1D/UniformGrid1D.h>
#include <FVMAKER/Grid/GridAnalysis/CellSizeAnalyser1D.h>
#include <FVMAKER/Grid/GridAnalysis/SmoothnessAnalyser1D.h>
#include <FVMAKER/Grid/GridAnalysis/UniformityAnalyser1D.h>
#include <FVMaker/Grid/GridAnalysis/SpacingRatioAnalyser1D.h>


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

    fvm::grd::UniformGrid1D ug1D(nVol, length, xInit);
    std::cout << ug1D << "\n";

    const auto& xCentro = ug1D.CentreCoordinate();
    std::cout << "\nCoordenadas do centro da malha\n";
    for (auto xC : xCentro) {
        std::cout << xC << "\n";
    }

    //==============================================================================
    //  Análise da malha com os analisadores desenvolvidos
    //==============================================================================

    std::cout << "\n";
    fvm::grd::CellSizeAnalyser1D<fvm::grd::UniformGrid1D> analyser_dx(ug1D);
    analyser_dx.analyse();
    std::cout << analyser_dx << "\n\n";

    fvm::grd::SmoothnessAnalyser1D<fvm::grd::UniformGrid1D> analyser_smooth(ug1D);
    analyser_smooth.analyse();
    std::cout << analyser_smooth << "\n\n";

    fvm::grd::UniformityAnalyser1D<fvm::grd::UniformGrid1D> analyser_uniform(ug1D);
    analyser_uniform.analyse();
    std::cout << analyser_uniform << "\n";


    fvm::grd::SpacingRatioAnalyser1D<fvm::grd::UniformGrid1D> analyser_ratio(ug1D);
    analyser_ratio.analyse();
    std::cout << analyser_ratio << std::endl;


    return EXIT_SUCCESS;
}
