//==============================================================================
// Nome        : UniformGrid1D.cpp
// Autor       : Joao Flavio Vieira de Vasconcellos
// Versao      : 2.0
// Descricao   : Implementacao da classe UniformGrid1D
//
// Este programa e software livre: voce pode redistribui-lo e/ou
// modifica-lo sob os termos da Licenca Publica Geral GNU, versao 3
// ou qualquer versao posterior.
//
// Este programa e distribuido na esperanca de que seja util,
// mas SEM QUALQUER GARANTIA; sem mesmo a garantia implicita de
// COMERCIABILIDADE ou ADEQUACAO A UM DETERMINADO PROPOSITO.
// Consulte a Licenca Publica Geral GNU para mais detalhes.
//
// Voce deve ter recebido uma copia da Licenca Publica Geral GNU
// junto com este programa. Se nao, veja <https://www.gnu.org/licenses/>.
//==============================================================================


//==============================================================================
// Includes da FVMaker
//==============================================================================

#include <FVMaker/Grid/Grid1D/UniformGrid1D.h>
#include <FVMaker/Misc/ParallelControl.h>


GRID_NAMESPACE_OPEN

//==============================================================================
// Construtores
//==============================================================================

UniformGrid1D::UniformGrid1D    (   const int&      _nVol, 
                                    const Real&     _length, 
                                    const Real&     _xIni)
                    : AbstractGrid1D<CellCentered>(_nVol, _length, _xIni) 
{
    this->typePattern_ = std::make_shared<CellCentered>();
    auto flag = this->typePattern_->BuildMesh(this);
}

UniformGrid1D::UniformGrid1D(const UniformGrid1D& _copia) noexcept
    : AbstractGrid1D<CellCentered>(_copia) 
{
    this->typePattern_ = std::make_shared<CellCentered>(*_copia.typePattern_);
}

//==============================================================================
// Metodos Publicos
//==============================================================================

std::unique_ptr<AbstractGrid1D<CellCentered>> UniformGrid1D::Clone() const 
{
    return std::make_unique<UniformGrid1D>(*this);
}

bool UniformGrid1D::GeraFaces() 
{
    GeraCoordenadas(&this->xFace_, 0.0);
    return true;
}

bool UniformGrid1D::GeraCentros() 
{
    GeraCoordenadas(&this->xCentro_, 0.5);
    return true;
}

//==============================================================================
// Implementacao dos Metodos Privados
//==============================================================================

void UniformGrid1D::GeraCoordenadas(VecReal* _coords, const Real& _offset) 
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