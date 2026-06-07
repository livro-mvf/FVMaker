#pragma once

//==============================================================================
// Includes da biblioteca padrão do C++
//==============================================================================


//==============================================================================
// Incluições do FVMAKER
//==============================================================================

//==============================================================================
// Incluições do FVMAKER
//==============================================================================

#include <FVMaker/Grid/abstractGrid.h>     // Definições da Grid; 

FVMAKER_NAMESPACE_OPEN

template<typename T>
std::ostream& operator<< (std::ostream& _os, const Equation<T>& _equation) {

std::ios_base::fmtflags fflags = _os.flags();
const int MYSIZE= LSIZE + 5;    

     _os << "\nImpressao do Equation\n";
     PrintLine(_os, MYSIZE);
     _os    << std::setw(5)     << "i" 
            << std::setw(12)    << "AP"
            << std::setw(12)    << "AW"
            << std::setw(12)    << "AE"
            << "\n";
     PrintLine(_os, MYSIZE);

    const std::size_t N = _equation.NVol();

    std::vector<std::size_t> indices(N);
    std::iota(indices.begin(), indices.end(), 0);

auto Print = [&](const std::size_t& i) {
    std::stringstream ss;
    ss << std::setw(5) << i
       << std::scientific
       << std::setprecision(4)
       << std::setw(12) << _equation.aP_[i]
       << std::setw(12) << _equation.aW_[i]
       << std::setw(12) << _equation.aE_[i];
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
}

FVMAKER_NAMESPACE_CLOSE
