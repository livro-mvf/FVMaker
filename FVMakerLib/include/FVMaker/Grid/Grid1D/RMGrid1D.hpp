

GRID_NAMESPACE_OPEN
        
template <typename T>
RMGrid1D<T> :: RMGrid1D (   const Real&         _beta
                        ,   const int&          _nVol
                        ,   const Real&         _length
                        ,   const Real&         _xIni
                        ) 
                        :   beta_(_beta)
                        ,   AbstractGrid1D<T>(_nVol, _length, _xIni)
{
    auxiBeta_ = (beta_ + 1) / (beta_ - 1);
auto flag =    this->typePattern_->BuildMesh(this);
}

template<typename TypePattern>
std::unique_ptr<AbstractGrid1D<TypePattern>> RMGrid1D<TypePattern>::Clone() const {
        return std::make_unique<RMGrid1D<TypePattern>>(*this);
}

template<typename T>
bool RMGrid1D<T>::GeraFaces() {

const Real DX(1.0 / this->NVol());    
auto Uniforme = [DX] (const Real& _soma) {return _soma + DX; };

    this->xFace_[0] = 0.0;
    std::transform  (   this->xFace_.begin()
                    ,   this->xFace_.end() - 1
                    ,   this->xFace_.begin() + 1
                    ,   Uniforme
                    );          
    if (this->NVol() < 10000) return GeraMalhaSequencial(&this->xFace_);
    return GeraMalhaParalelo(&this->xFace_); 

}

template<typename T>
bool RMGrid1D<T>:: GeraCentros() {
    return false; 
}

template<typename T>
bool RMGrid1D<T>:: GeraMalhaSequencial (VecReal* _coord) {

const Real DX(1.0/ this->NVol());    
auto MalhaFronteira = [&] (const Real& _x) {return this->Funcao(_x); };

    std::transform  (   _coord->begin()
                    ,   _coord->end()
                    ,   _coord->begin() 
                    ,   MalhaFronteira
                    );  
    return true;
}


template<typename T>
bool RMGrid1D<T> :: GeraMalhaParalelo (VecReal* _coord){
    
    if (this->NVol() > 100000) return GeraMalhaSIMD(_coord);
    
const Real DX(1.0/ this->NVol());    
auto MalhaFronteira = [&] (const Real& _x) {return this->Funcao(_x); };

    std::transform  (   std::execution::par
                    ,   _coord->begin()
                    ,   _coord->end()
                    ,   _coord->begin() 
                    ,   MalhaFronteira
                    );    
    
    return true;
}

template<typename T>
bool RMGrid1D<T> :: GeraMalhaSIMD (VecReal* _coord){
    
    
const Real DX(1.0/ this->NVol());    
auto MalhaFronteira = [&] (const Real& _x) {return this->Funcao(_x); };

    std::transform  (   std::execution::par_unseq
                    ,   _coord->begin()
                    ,   _coord->end()
                    ,   _coord->begin() 
                    ,   MalhaFronteira
                    );    
        return true;
}

template<typename T>
Real RMGrid1D<T> :: Funcao (const Real& _eta) const {

const Real  expoente = (_eta - ALPHA_) / (1.0  - ALPHA_);
const Real  auxi     = pow(this->auxiBeta_, expoente);
const Real  nume     = this->beta_ * (auxi - 1) + 2.0 * ALPHA_ * (auxi + 1);
const Real  deno  = (2 * ALPHA_ + 1) * (1 + auxi);
    return (nume / deno)* this->Length() + this->XInit();
}
GRID_NAMESPACE_CLOSE