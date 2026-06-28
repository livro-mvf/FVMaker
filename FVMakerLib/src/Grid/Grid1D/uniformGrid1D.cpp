// ============================================================================
// Arquivo: uniformGrid1D.cpp
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Implementa uniform Grid 1 D no contexto de Grid / Grid1D.
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
// #include <FVMaker/Misc/parallelControl.h>


GRID_NAMESPACE_OPEN

//==============================================================================
// Construtores
//==============================================================================

uniformGrid1D::uniformGrid1D    (   const int&      _nVol, 
                                    const Real&     _length, 
                                    const Real&     _xIni)
                    : AbstractGrid1D<CellCentered>(_nVol, _length, _xIni) 
{
    this->typePattern_ = std::make_shared<CellCentered>();
    auto flag = this->typePattern_->BuildMesh(this);
}

uniformGrid1D::uniformGrid1D(const uniformGrid1D& _copia) noexcept
    : AbstractGrid1D<CellCentered>(_copia) 
{
    this->typePattern_ = std::make_shared<CellCentered>(*_copia.typePattern_);
}

//==============================================================================
// Metodos Publicos
//==============================================================================

std::unique_ptr<AbstractGrid1D<CellCentered>> uniformGrid1D::Clone() const 
{
    return std::make_unique<uniformGrid1D>(*this);
}

bool uniformGrid1D::GeraFaces() 
{
    GeraCoordenadas(&this->xFace_, 0.0);
    return true;
}

bool uniformGrid1D::GeraCentros() 
{
    GeraCoordenadas(&this->xCentro_, 0.5);
    return true;
}

//==============================================================================
// Implementacao dos Metodos Privados
//==============================================================================

void uniformGrid1D::GeraCoordenadas(VecReal* _coords, const Real& _offset) 
{
    const Real dx = this->Length() / this->NVol();
    const Real x0 = this->XInit();
    
    // Preenche as coordenadas base
    _coords->resize(this->NVol() + (_offset == 0.0 ? 1 : 0));
    std::iota(_coords->begin(), _coords->end(), 0.0);
    
    // Aplica transformacao usando a politica do ParallelControl

    auto GeraCoordenadas =  [=](Real& _val) {
                _val = x0 + (_val + _offset) * dx;
    };

    ParallelControl::for_each   (   _coords->begin(), 
                                    _coords->end(),
                                    GeraCoordenadas);
    
    // Calcula distancias se for uma operacao de faces
    if (!this->CalculaDistancias()) {
        throw std::runtime_error("Falha ao calcular distancias");
    }
}

GRID_NAMESPACE_CLOSE