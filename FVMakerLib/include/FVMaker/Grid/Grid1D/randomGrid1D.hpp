#pragma once

//==============================================================================
// Includes da biblioteca padrão do C++
//==============================================================================
#include <chrono>
#include <random>


//==============================================================================
// Includes da biblioteca FVMaker
//==============================================================================

#include <FVMaker/Grid/GridPattern/abstractGridPattern.h>

GRID_NAMESPACE_OPEN
        
template<typename TypePattern>
RandomGrid1D<TypePattern> :: RandomGrid1D   (   const int&          _nVol
                                            ,   const Real&         _length
                                            ,   const Real&         _xIni
                                            ) 
                                            : AbstractGrid1D<TypePattern>(_nVol, _length, _xIni)
{
auto flag =    this->typePattern_->BuildMesh(this);

}
  
template<typename TypePattern>
std::unique_ptr<AbstractGrid1D<TypePattern>> RandomGrid1D<TypePattern>::Clone() const {
        return std::make_unique<RandomGrid1D<TypePattern>>(*this);
}

template<typename TypePattern>
bool RandomGrid1D<TypePattern> :: GeraFaces() {
    if (this->NVol() < 10000) return  GeraMalhaSequencial(&this->xFace_);
     return GeraMalhaParalelo(&this->xFace_);

}

template<typename TypePattern>
bool RandomGrid1D<TypePattern> :: GeraCentros() {
    if (this->NVol() < 10000) return  GeraMalhaSequencial(&this->xCentro_);
    return GeraMalhaParalelo(&this->xCentro_);     
}

template<typename TypePattern>
bool RandomGrid1D<TypePattern> :: GeraMalhaSequencial (VecReal* _coord) {

auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    std::vector<size_t> indices(_coord->size());
    std::iota(indices.begin(), indices.end(), 0); 
    std::uniform_real_distribution<Real> dist(this->XInit (), this->XInit () + this->Length());
    
    auto Sorteio = [&_coord, seed, &dist] (const size_t& i){
        std::mt19937 localGen(seed + i);
        (*_coord)[i] = dist(localGen);
    };
    
    std::for_each   (   std::execution::seq
                    ,   indices.begin()
                    ,   indices.end()
                    ,   Sorteio
                    );
    
    std::sort(std::execution::par, _coord->begin(), _coord->end());
    return true;
}

template<typename TypePattern>
bool RandomGrid1D<TypePattern> :: GeraMalhaParalelo (VecReal* _coord){
    
    if (this->NVol() > 100000) return GeraMalhaSIMD( _coord);
auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    std::vector<size_t> indices(this->NVol());
    std::iota(indices.begin(), indices.end(), 0);
    return true;
}

template<typename TypePattern>
bool RandomGrid1D<TypePattern> :: GeraMalhaSIMD (VecReal*){
    
auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();    return true;
    std::vector<size_t> indices(this->NVol());
    std::iota(indices.begin(), indices.end(), 0);
    return true;
}

GRID_NAMESPACE_CLOSE