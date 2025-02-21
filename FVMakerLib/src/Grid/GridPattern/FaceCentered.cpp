#include <FVMaker/Grid/GridPattern/FaceCentered.h>

GRID_NAMESPACE_OPEN

std::shared_ptr<GridPattern> FaceCentered::Clone() const {
        return std::make_shared<FaceCentered>(*this);
}

std::string FaceCentered::TipoPadraoMalha() const {
    return "Face Centereda";
}

GRID_NAMESPACE_CLOSE
