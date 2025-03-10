#pragma once

//==============================================================================
// Includes da biblioteca padrão do C++
//==============================================================================

#include <iomanip>

//==============================================================================
// Incluições do FVMAKER
//==============================================================================
#include <FVMaker/Error/FVMakerException.h>
#include <FVMaker/Grid/AbstractGrid.h>              ///< Definições da Grid; 
#include <FVMaker/Misc/Misc.h>
using fvm::grd::AbstractGrid;


FVMAKER_NAMESPACE_OPEN

        
template<typename T>
std::ostream& operator<< (std::ostream& _os, const Coefficient1D<T>& _coeff1D) {
const int MYSIZE= LSIZE - 35;    
     _os << "\nImpressao do Coefficient1D<T>\n";
     PrintLine(_os, MYSIZE);
     _os    << std::setw(5)     << "i" 
            << std::setw(20)    << "xCentro"
            << std::setw(20)    << "Gamma"
            << "\n";
     PrintLine(_os, MYSIZE);
     
unsigned volume = 0;
auto     ptrGrid = _coeff1D.Grid();
const auto&     ptrCentre = ptrGrid->CentreCoordinate();
auto     ptrCoeff  = _coeff1D.VectorCoeff ();

auto Print = [&volume](const auto& _xC, const auto& _xF) {
    std::stringstream ss;
    ss << std::setw(5) << volume 
       << std::scientific 
       << std::setw(20) << _xC
       << std::setw(20) << _xF;
    volume++;
    return ss.str();    
};
 
    std::transform  (   ptrCentre.begin()
                    ,   ptrCentre.end()
                    ,   ptrCoeff->begin()
                    ,   std::ostream_iterator<std::string>(_os, "\n")
                    ,   Print
                    );

    _os << std::flush;
    PrintLine(_os, MYSIZE);
     
    return _os;
};


FVMAKER_NAMESPACE_CLOSE
