#include <FVMaker/Grid/GridPattern/FaceCentered.h>
#include <FVMaker/Grid/Grid1D/Grid1D.h>

GRID_NAMESPACE_OPEN

std::shared_ptr<GridPattern> FaceCentered::Clone() const {
    return std::make_shared<FaceCentered>(*this);
}

std::string FaceCentered::TipoPadraoMalha() const {
    return "Face Centrada";
}



GRID_NAMESPACE_CLOSE
