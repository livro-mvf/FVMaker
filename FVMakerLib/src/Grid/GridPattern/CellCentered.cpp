
#include <FVMaker/Grid/GridPattern/CellCentered.h>
#include <FVMaker/Grid/Grid1D/Grid1D.h>

GRID_NAMESPACE_OPEN

std::shared_ptr<GridPattern> CellCentered::Clone() const {
        return std::make_shared<CellCentered>(*this);
}
        

std::string CellCentered::TipoPadraoMalha() const {
    return "Volume Centrado";
}


bool CellCentered::GenerateCoordinates(void* _var) {
    Grid1D<CellCentered>* obj = static_cast<Grid1D<CellCentered>*>(_var);  

    bool flag = obj->GeraFaces();
         flag = obj->CalculaCentros();
    return true;
};



GRID_NAMESPACE_CLOSE
