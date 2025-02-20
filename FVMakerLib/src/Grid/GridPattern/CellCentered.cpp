#include <FVMaker/Grid/GridPattern/CellCentered.h>

GRID_NAMESPACE_OPEN

std::shared_ptr<GridPattern> CellCentered::Clone() const {
        return std::make_shared<CellCentered>(*this);
}
        

std::string CellCentered::TipoPadraoMalha() const {
    return "Volume Centrado";
}

bool CellCentered::AvaliarCoordenadaPrincipal() const {
    return true;
}

bool CellCentered::AvaliarCoordenadaAdicional() const {
    return true;
}

GRID_NAMESPACE_CLOSE
