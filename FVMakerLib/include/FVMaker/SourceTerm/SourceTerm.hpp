#pragma once

//==============================================================================
// Includes da biblioteca padrão do C++
//==============================================================================
#include <algorithm>                // std::transform
#include <iomanip>                  // std::setw
#include <iostream>                 // std::cerr 
#include <iterator>               // std::ostream_iterator

//==============================================================================
// Includes da biblioteca FVMaker
//==============================================================================
#include <FVMaker/Error/FVMakerException.h>
#include <FVMaker/Misc/Misc.h>

FVMAKER_NAMESPACE_OPEN


template<typename T>
std::ostream& operator<<(std::ostream& _os, const SourceTerm<T>& _source) {
const int MYSIZE= LSIZE - 15;    
     _os << "\nImpressao do AbstractGrid1D\n";
     PrintLine(_os, MYSIZE);
     _os    << std::setw(5)     << "i" 
            << std::setw(20)    << "xCentro"
            << std::setw(20)    << "SP" 
            << std::setw(20)    << "SC"
            << "\n";
     PrintLine(_os, MYSIZE);
/*     
unsigned volume = 0;
auto     ptrdxCentro = std::begin(_grid1D.dxCentro_);
auto     ptrdxFace = std::begin(_grid1D.dxFace_);

auto Print = [&volume, &ptrdxCentro, &ptrdxFace](const auto& _xC, const auto& _xF) {
    std::stringstream ss;
    ss << std::setw(5) << volume 
       << std::scientific 
       << std::setw(20) << _xC
       << std::setw(20) << _xF
       << std::setw(20) << *ptrdxCentro
       << std::setw(20) << *ptrdxFace;
    volume++;ptrdxCentro++;ptrdxFace++;
    return ss.str();    
};

    std::transform  (   std::begin(_grid1D.xCentro_)
                    ,   std::end(_grid1D.xCentro_)
                    ,   std::begin(_grid1D.xFace_)
                    ,   std::ostream_iterator<std::string>(_os, "\n")
                    ,   Print
                    );

    
    _os     << std::setw(5) << volume  << std::scientific << std::setprecision(6)
            << std::setw(40) << *(std::prev(std::end(_grid1D.xFace_)))
            << std::setw(20) << *(std::prev(std::end(_grid1D.dxCentro_)))
            << "\n";
    
    _os << std::flush;
    PrintLine(_os, MYSIZE);
*/     
    return _os;
}


FVMAKER_NAMESPACE_CLOSE