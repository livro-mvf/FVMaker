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

#include <FVMaker/Grid/AbstractGrid.h>     // Definições da Grid; 

FVMAKER_NAMESPACE_OPEN

template<typename T>
std::ostream& operator<< (std::ostream& _os, const Equation<T>& _equation) {
const int MYSIZE= LSIZE + 5;    
     _os << "\nImpressao do Equation\n";
     PrintLine(_os, MYSIZE);
     _os    << std::setw(5)     << "i" 
            << std::setw(12)    << "AP"
            << std::setw(12)    << "AW"
            << std::setw(12)    << "AE"
            << "\n";
     PrintLine(_os, MYSIZE);
     
unsigned volume = 0;

auto ptrAW = std::begin(_equation.aW_);
auto ptrAE = std::begin(_equation.aE_);

auto Print = [&volume, &ptrAW, &ptrAE](const auto& _aP) {
    std::stringstream ss;
    ss << std::setw(5) << volume 
       << std::scientific
       << std::setprecision(4)
       << std::setw(12) << _aP
       << std::setw(12) << *ptrAW
       << std::setw(12) << *ptrAE;
//       << std::setw(20) << _xF
//       << std::setw(20) << *ptrdxCentro
//       << std::setw(20) << *ptrdxFace;
   volume++; ++ptrAW; ++ptrAE;
    return ss.str();    
};

    std::transform  (   std::begin(_equation.aP_)
                    ,   std::end(_equation.aP_)
                    ,   std::ostream_iterator<std::string>(_os, "\n")
                    ,   Print
                    );

    PrintLine(_os, MYSIZE);
     
    return _os;
}

FVMAKER_NAMESPACE_CLOSE
