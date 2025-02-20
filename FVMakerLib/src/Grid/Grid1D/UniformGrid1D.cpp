#include <FVMaker/Grid/Grid1D/UniformGrid1D.h>
#include <algorithm>
#include <iostream>
#include <numeric>          // Para std::iota
#include <execution>        //  Necessário para std::execution

GRID_NAMESPACE_OPEN
        

UniformGrid1D :: UniformGrid1D (    const int&          _nVol
                                ,   const Real&         _lenght
                                ,   const Real&         _xIni
                                ) : Grid1D<CellCentered>(_nVol, _lenght, _xIni)
{
    try {
        if (!GeraMalha())  throw fvm::FVMakerException(ErrorCode::GridGenerationError);
    } catch (const fvm::FVMakerException& e) {
        std::cerr << "\n\n";
        PrintLine(std::cerr);
        std::cerr << "Exceção capturada: " << e.what() << "\n";
        PrintLine(std::cerr);
        std::cerr << "\n\n";
        exit(EXIT_FAILURE);
    }
}
        
std::unique_ptr<Grid1D<CellCentered>> UniformGrid1D::Clone() const {
        return std::make_unique<UniformGrid1D>(*this);
}

bool UniformGrid1D::GeraMalha() {
    if (nVol < 10000) {
            return GeraMalhaSsequencial();
        } else if (nVol < 100000) {
            return GeraMalhaParalelo();
        }; 
        
    std::cout << "🔥 Geração paralela com SIMD.\n";
    return GeraMalhaSIMD ();     
}

bool UniformGrid1D :: GeraMalhaSsequencial () {

const Real DX(lenght/ nVol);    
auto Uniforme = [DX] (const Real& soma) { return soma + DX; };

    xFace[0] = xIni;
    std::transform  (   std::begin(xFace)
                    ,   std::end(xFace) - 1
                    ,   std::begin(xFace) + 1
                    ,   Uniforme
                    );  
    
    return true;
}
bool UniformGrid1D :: GeraMalhaParalelo (){
    
const Real DX(lenght/ nVol); 

const Real xIniLocal = xIni;
auto Uniforme = [DX, xIniLocal] (const std::size_t& i) { return i * DX + xIniLocal;};

std::vector<std::size_t> indices(nVol + 1);
    std::iota   (   std::begin(indices)
                ,   std::end(indices)
                , 0
                ); 

    std::transform  (   std::execution::par
                    ,   std::begin(indices)
                    ,   std::end(indices)
                    ,   std::begin(xFace)
                    ,   Uniforme
                    );   
    return true;
}
bool UniformGrid1D :: GeraMalhaSIMD (){
    
    
const Real DX(lenght/ nVol); 

const Real xIniLocal = xIni;
auto Uniforme = [DX, xIniLocal] (const std::size_t& i) { return i * DX + xIniLocal;};

std::vector<std::size_t> indices(nVol + 1);
    std::iota   (   std::begin(indices)
                ,   std::end(indices)
                , 0
                ); 

    std::transform  (   std::execution::par_unseq
                    ,   std::begin(indices)
                    ,   std::end(indices)
                    ,   std::begin(xFace)
                    ,   Uniforme
                    );
    return true;
}

GRID_NAMESPACE_CLOSE