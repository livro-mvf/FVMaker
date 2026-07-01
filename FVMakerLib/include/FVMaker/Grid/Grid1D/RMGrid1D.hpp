// ============================================================================
// Arquivo: RMGrid1D.hpp
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Declara RMGrid 1 D no contexto de Grid / Grid1D.
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

GRID_NAMESPACE_OPEN
        
// Realiza a operacao rmgrid1 d definida por esta interface.
template <typename T>
RMGrid1D<T> :: RMGrid1D (   const Real&         _beta
                        ,   const int&          _nVol
                        ,   const Real&         _length
                        ,   const Real&         _xIni
                        ) 
                        :   beta_(_beta)
                        ,   AbstractGrid1D<T>(_nVol, _length, _xIni)
{
    

    try {
        if (beta_ <=  1.0)  throw fvm::FVMakerException(ErrorCode::InvalidBeta);
        
    } catch (const fvm::FVMakerException& e) {
        std::cerr << "\n\n";
        PrintLine(std::cerr);
        std::cerr << "Exceção capturada: " << e.what() << "\n";
        PrintLine(std::cerr);
        std::cerr << "\n\n";
        exit(EXIT_FAILURE);
    }
        
    auxiBeta_ = (beta_ + 1) / (beta_ - 1);
auto flag =    this->typePattern_->BuildMesh(this);
}

// Realiza a operacao clone definida por esta interface.
template<typename TypePattern>
std::unique_ptr<AbstractGrid1D<TypePattern>> RMGrid1D<TypePattern>::Clone() const {
        return std::make_unique<RMGrid1D<TypePattern>>(*this);
}

// Realiza a operacao gera faces definida por esta interface.
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

// Realiza a operacao gera centros definida por esta interface.
template<typename T>
bool RMGrid1D<T>:: GeraCentros() {
const Real DX(1.0 / this->NVol());    
auto Uniforme = [DX] (const Real& _soma) {return _soma + DX; };

    this->xCentro_[0] = 0.5 * DX;
    std::transform  (   this->xCentro_.begin()
                    ,   this->xCentro_.end() - 1
                    ,   this->xCentro_.begin() + 1
                    ,   Uniforme
                    );          
    if (this->NVol() < 10000) return GeraMalhaSequencial(&this->xCentro_);
    return GeraMalhaParalelo(&this->xCentro_); 
    return true; 
}

// Realiza a operacao gera malha sequencial definida por esta interface.
template<typename T>
bool RMGrid1D<T>:: GeraMalhaSequencial (VecReal* _coord) {

 
auto MalhaFronteira = [&] (const Real& _x) {return this->Funcao(_x); };

//for (auto xx : *_coord) {
//    std::cout << xx << "\n";
//}

    std::transform  (   _coord->begin()
                    ,   _coord->end()
                    ,   _coord->begin() 
                    ,   MalhaFronteira
                    );  
    return true;
}


// Realiza a operacao gera malha paralelo definida por esta interface.
template<typename T>
bool RMGrid1D<T> :: GeraMalhaParalelo (VecReal* _coord){
    
    if (this->NVol() > 100000) return GeraMalhaSIMD(_coord);
    
   
auto MalhaFronteira = [&] (const Real& _x) {return this->Funcao(_x); };

    std::transform  (   std::execution::par
                    ,   _coord->begin()
                    ,   _coord->end()
                    ,   _coord->begin() 
                    ,   MalhaFronteira
                    );    
    
    return true;
}

// Realiza a operacao gera malha simd definida por esta interface.
template<typename T>
bool RMGrid1D<T> :: GeraMalhaSIMD (VecReal* _coord){
    
    
    
auto MalhaFronteira = [&] (const Real& _x) {return this->Funcao(_x); };

    std::transform  (   std::execution::par_unseq
                    ,   _coord->begin()
                    ,   _coord->end()
                    ,   _coord->begin() 
                    ,   MalhaFronteira
                    );    
        return true;
}

// Realiza a operacao funcao definida por esta interface.
template<typename T>
Real RMGrid1D<T> :: Funcao (const Real& _eta) const {

const Real  expoente = (_eta - ALPHA_) / (1.0  - ALPHA_);
const Real  auxi     = pow(this->auxiBeta_, expoente);
const Real  nume     = this->beta_ * (auxi - 1) + 2.0 * ALPHA_ * (auxi + 1);
const Real  deno  = (2 * ALPHA_ + 1) * (1 + auxi);
    return (nume / deno)* this->Length() + this->XInit();
}
GRID_NAMESPACE_CLOSE