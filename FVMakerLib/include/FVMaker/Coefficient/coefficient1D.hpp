#pragma once

//==============================================================================
// Includes da biblioteca padrão do C++
//==============================================================================

#include <iomanip>

//==============================================================================
// Incluições do FVMAKER
//==============================================================================
#include <FVMaker/Error/FVMakerException.h>
#include <FVMaker/Grid/abstractGrid.h>              ///< Definições da Grid; 
#include <FVMaker/Misc/misc.h>
using fvm::grd::AbstractGrid;


FVMAKER_NAMESPACE_OPEN

        
template<typename T>
std::ostream& operator<< (std::ostream& _os, const Coefficient1D<T>& _coeff1D) {

std::ios_base::fmtflags fflags = _os.flags();
const int MYSIZE= LSIZE - 35;    
     _os << "\nImpressao do Coefficient1D<T>\n";
     PrintLine(_os, MYSIZE);
     _os    << std::setw(5)     << "i" 
            << std::setw(20)    << "xCentro"
            << std::setw(20)    << "Gamma"
            << "\n";
     PrintLine(_os, MYSIZE);

    const std::size_t N = _coeff1D.NVol();

    std::vector<std::size_t> indices(N);
    std::iota(indices.begin(), indices.end(), 0);

auto xCentro = _coeff1D.Grid().GetCentre();
auto Print = [&](const std::size_t& i)
{
    std::stringstream ss;
    ss << std::setw(5) << i
       << std::scientific 
       << std::setw(20) << xCentro[i]
       << std::setw(20) << _coeff1D.coeff_[i];
    return ss.str(); 
};

    std::transform  (   std::begin(indices)
                    ,   std::end(indices)
                    ,   std::ostream_iterator<std::string>(_os, "\n")
                    ,   Print
                    );


    _os << std::flush;
    PrintLine(_os, MYSIZE);
    _os.flags(fflags);

    return _os;
};


FVMAKER_NAMESPACE_CLOSE
