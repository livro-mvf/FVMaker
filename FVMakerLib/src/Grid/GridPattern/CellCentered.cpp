#include <FVMaker/Grid/GridPattern/CellCentered.h>
//#include <FVMaker/Grid/Grid1D/Grid1D.h>

GRID_NAMESPACE_OPEN

std::shared_ptr<AbstractGridPattern> CellCentered::Clone() const {
    return std::make_shared<CellCentered>(*this);
}

std::string CellCentered::TipoPadraoMalha() const {
    return "Volume Centrado";
}

GRID_NAMESPACE_CLOSE
