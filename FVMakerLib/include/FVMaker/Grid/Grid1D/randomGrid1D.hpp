// ============================================================================
// Arquivo: randomGrid1D.hpp
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Declara random Grid 1 D no contexto de Grid / Grid1D.
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
        
// Realiza a operacao random grid1 d definida por esta interface.
template<typename TypePattern>
RandomGrid1D<TypePattern> :: RandomGrid1D   (   const int&          _nVol
                                            ,   const Real&         _length
                                            ,   const Real&         _xIni
                                            ) 
                                            : AbstractGrid1D<TypePattern>(_nVol, _length, _xIni)
{
auto flag =    this->typePattern_->BuildMesh(this);

}
  
// Realiza a operacao clone definida por esta interface.
template<typename TypePattern>
std::unique_ptr<AbstractGrid1D<TypePattern>> RandomGrid1D<TypePattern>::Clone() const {
        return std::make_unique<RandomGrid1D<TypePattern>>(*this);
}

// Realiza a operacao gera faces definida por esta interface.
template<typename TypePattern>
bool RandomGrid1D<TypePattern> :: GeraFaces() {
    if (this->NVol() < 10000) return  GeraMalhaSequencial(&this->xFace_);
     return GeraMalhaParalelo(&this->xFace_);

}

// Realiza a operacao gera centros definida por esta interface.
template<typename TypePattern>
bool RandomGrid1D<TypePattern> :: GeraCentros() {
    if (this->NVol() < 10000) return  GeraMalhaSequencial(&this->xCentro_);
    return GeraMalhaParalelo(&this->xCentro_);     
}

// Realiza a operacao gera malha sequencial definida por esta interface.
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

// Realiza a operacao gera malha paralelo definida por esta interface.
template<typename TypePattern>
bool RandomGrid1D<TypePattern> :: GeraMalhaParalelo (VecReal* _coord){
    
    if (this->NVol() > 100000) return GeraMalhaSIMD( _coord);
auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    std::vector<size_t> indices(this->NVol());
    std::iota(indices.begin(), indices.end(), 0);
    return true;
}

// Realiza a operacao gera malha simd definida por esta interface.
template<typename TypePattern>
bool RandomGrid1D<TypePattern> :: GeraMalhaSIMD (VecReal*){
    
auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();    return true;
    std::vector<size_t> indices(this->NVol());
    std::iota(indices.begin(), indices.end(), 0);
    return true;
}

GRID_NAMESPACE_CLOSE