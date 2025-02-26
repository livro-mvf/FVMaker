//==============================================================================
// Includes da biblioteca FVMaker
//==============================================================================
#include <FVMaker/Grid/Grid1D/UniformGrid1D.h>

GRID_NAMESPACE_OPEN
        

UniformGrid1D :: UniformGrid1D (    const int&          _nVol
                                ,   const Real&         _length
                                ,   const Real&         _xIni
                                ) : Grid1D<CellCentered>(_nVol, _length, _xIni)
{
    
auto flag =    this->typePattern->BuildMesh(this);
}
        
std::unique_ptr<Grid1D<CellCentered>> UniformGrid1D::Clone() const {
        return std::make_unique<UniformGrid1D>(*this);
}

bool UniformGrid1D::GeraFaces() {

    if (nVol < 10000) return GeraMalhaSequencial(&this->xFace);
    return GeraMalhaParalelo(&this->xFace); 

}

bool UniformGrid1D:: GeraCentros() {
    return false; 
}

bool UniformGrid1D :: GeraMalhaSequencial (VecReal* _coord) {

const Real DX(length/ nVol);    
auto Uniforme = [DX] (const Real& soma) { return soma + DX; };

    (*_coord)[0] = xIni;
    std::transform  (   _coord->begin()
                    ,   _coord->end() - 1
                    ,   _coord->begin() + 1
                    ,   Uniforme
                    );  
    return true;
}
bool UniformGrid1D :: GeraMalhaParalelo (VecReal* _coord){
    
    if (nVol > 100000) return GeraMalhaSIMD(_coord);
    
const Real DX(length/ nVol); 

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
                    ,   _coord->begin()
                    ,   Uniforme
                    );   
    return true;
}
bool UniformGrid1D :: GeraMalhaSIMD (VecReal* _coord){
    
    
const Real DX(length / nVol); 

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
                    ,   _coord->begin()
                    ,   Uniforme
                    );
    return true;
}

GRID_NAMESPACE_CLOSE