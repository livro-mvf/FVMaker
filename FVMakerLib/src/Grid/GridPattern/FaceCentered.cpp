#include <FVMaker/Grid/GridPattern/FaceCentered.h>

GRID_NAMESPACE_OPEN

std::shared_ptr<GridPattern> FaceCentered::Clone() const {
        return std::make_shared<FaceCentered>(*this);
}

std::string FaceCentered::TipoPadraoMalha() const {
    return "Face Centereda";
}

bool FaceCentered::AvaliarCoordenadaPrincipal() const {
    // Implementação específica do padrão face-centrado.
    // Substituir pelo código real.
    return true;
}

bool FaceCentered::AvaliarCoordenadaAdicional() const {
    // Implementação específica do padrão face-centrado.
    // Substituir pelo código real.
    return true;
}

GRID_NAMESPACE_CLOSE
