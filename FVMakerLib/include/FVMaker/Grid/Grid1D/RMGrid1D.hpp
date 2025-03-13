

GRID_NAMESPACE_OPEN
        
template <typename T>
RMGrid1D<T> :: RMGrid1D    (   const Real&         _beta
                        ,   const int&          _nVol
                        ,   const Real&         _length
                        ,   const Real&         _xIni
                        ) 
                        :   beta_(_beta)
                        ,   AbstractGrid1D<T>(_nVol, _length, _xIni)
{
    
auto flag =    this->typePattern_->BuildMesh(this);
}

template<typename TypePattern>
std::unique_ptr<AbstractGrid1D<TypePattern>> RMGrid1D<TypePattern>::Clone() const {
        return std::make_unique<RMGrid1D<TypePattern>>(*this);
}

template<typename T>
bool RMGrid1D<T>::GeraFaces() {

    if (this->NVol() < 10000) return GeraMalhaSequencial(0.0, &this->xFace_);
    return GeraMalhaParalelo(0.0, &this->xFace_); 

}

template<typename T>
bool RMGrid1D<T>:: GeraCentros() {
    return false; 
}

template<typename T>
bool RMGrid1D<T>:: GeraMalhaSequencial (const Real& _xi, VecReal* _coord) {

const Real DX(1.0/ this->NVol());    
auto MalhaFronteira = [DX] (const Real& soma) { return soma + DX; };

    (*_coord)[0] = this->XInit();
    std::transform  (   _coord->begin() + 1
                    ,   _coord->end() - 1
                    ,   _coord->begin() 
                    ,   MalhaFronteira
                    );  
    return true;
}


template<typename T>
bool RMGrid1D<T> :: GeraMalhaParalelo (const Real& _xi, VecReal* _coord){
    
    if (this->NVol() > 100000) return GeraMalhaSIMD(_xi, _coord);
    
const Real DX(this->Length()/ this->NVol()); 

const Real xIniLocal = this->XInit();
auto Uniforme = [DX, xIniLocal] (const std::size_t& i) { return i * DX + xIniLocal;};

std::vector<std::size_t> indices(this->NVol() + 1);
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

template<typename T>
bool RMGrid1D<T> :: GeraMalhaSIMD (const Real& _xi, VecReal* _coord){
    
    
const Real DX(this->Length() / this->NVol()); 

const Real xIniLocal = this->XInit();
auto Uniforme = [DX, xIniLocal] (const std::size_t& i) { return i * DX + xIniLocal;};

std::vector<std::size_t> indices(this->NVol() + 1);
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

template<typename T>
Real RMGrid1D<T> :: Funcao (const Real& _eta) const {

const Real  auxi1 = (_eta - ALPHA_) / (1.0  - ALPHA_);
const Real  auxi2 = pow(this->auxiBeta_, auxi1);
const Real  nume  = this->beta_ + 2 * ALPHA_ - auxi2 * (this->beta_ - 2 * ALPHA_);
const Real  deno  = (2 * ALPHA_ + 1) * (auxi2 + 1);
    return (nume / deno)* this->Length() + this->XInit();
}
GRID_NAMESPACE_CLOSE