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
                    : data_{_nVol, _length, _xIni} 
{
    auto flag = typePattern_.BuildMesh(this);
    (void)flag;
}

uniformGrid1D::uniformGrid1D(const uniformGrid1D& _copia) noexcept
    : data_{_copia.data_},
      typePattern_{_copia.typePattern_} 
{
     
}

//==============================================================================
// Metodos Publicos
//==============================================================================

std::unique_ptr<uniformGrid1D> uniformGrid1D::Clone() const 
{
    return std::make_unique<uniformGrid1D>(*this);
}

bool uniformGrid1D::GeraFaces() 
{
    GeraCoordenadas(AddressxFace(), 0.0);
    return true;
}

bool uniformGrid1D::GeraCentros() 
{
    GeraCoordenadas(AddressxCentro(), 0.5);
    return true;
}

//==============================================================================
// Implementacao dos Metodos Privados
//==============================================================================

void uniformGrid1D::GeraCoordenadas(VecReal* _coords, const Real& _offset) 
{
    const Real dx = Length() / NVol();
    const Real x0 = XInit();
    
    // Preenche as coordenadas base
    _coords->resize(NVol() + (_offset == 0.0 ? 1 : 0));
    std::iota(_coords->begin(), _coords->end(), 0.0);
    
    // Aplica transformacao usando a politica do ParallelControl

    auto GeraCoordenadas =  [=](Real& _val) {
                _val = x0 + (_val + _offset) * dx;
    };

    ParallelControl::for_each   (   _coords->begin(), 
                                    _coords->end(),
                                    GeraCoordenadas);
    
    // Calcula distancias se for uma operacao de faces
    if (!CalculaDistancias()) {
        throw std::runtime_error("Falha ao calcular distancias");
    }
}

GRID_NAMESPACE_CLOSE