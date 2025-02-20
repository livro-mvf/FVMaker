#pragma once

//==============================================================================
// Includes da biblioteca padrão do C++
//==============================================================================
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <iterator>             // std::ostream_iterator
#include <sstream>

//==============================================================================
// Includes da biblioteca FVMaker
//==============================================================================
#include <FVMaker/Error/FVMakerException.h>
#include <FVMaker/Misc/Misc.h>


GRID_NAMESPACE_OPEN

template <typename T>
Grid1D<T> :: Grid1D     (   const int&          _nvol
                        ,   const Real&         _lenght
                        ,   const Real&         _xIni
                        ) 
                        :   nVol(_nvol)
                        ,   lenght(_lenght)
                        ,   xIni(_xIni)
{
    try {
        if (nVol < 2)  throw fvm::FVMakerException(ErrorCode::InvalidNumbersOfVolumes);
        if (lenght <= 0)  throw fvm::FVMakerException(ErrorCode::InvalidLenght);
        InitVector ();
        
    } catch (const fvm::FVMakerException& e) {
        std::cerr << "\n\n";
        PrintLine(std::cerr);
        std::cerr << "Exceção capturada: " << e.what() << "\n";
        PrintLine(std::cerr);
        std::cerr << "\n\n";
        exit(EXIT_FAILURE);
    }
    
    typePattern = std::make_shared<T>();
}

template <typename T>
void Grid1D<T> :: InitVector () {
    
    try {    
        xFace.reserve(nVol + 1);  
        xFace.resize(nVol + 1, 0.0);

        dxCentro.reserve(nVol + 1);  
        dxCentro.resize(nVol + 1, 0.0);

        xCentro.reserve(nVol);  
        xCentro.resize(nVol, 0.0);

        dxFace.reserve(nVol);  
        dxFace.resize(nVol, 0.0);  
    } catch (const std::bad_alloc&) {
        throw fvm::FVMakerException(ErrorCode::MemoryAllocationError);
    } catch (const std::length_error&) {
        throw fvm::FVMakerException(ErrorCode::VectorResizeError);
    } catch (const std::exception& e) {
        throw fvm::FVMakerException(ErrorCode::UnhandledException);
    }
    
}

template<typename T>
std::ostream& operator<<(std::ostream& _os, const Grid1D<T>& _grid1D) {
const int MYSIZE= LSIZE + 5;    
     _os << "\nImpressao do Grid1D\n";
     PrintLine(_os, MYSIZE);
     _os    << std::setw(5)     << "i" 
            << std::setw(20)    << "xCentro"
            << std::setw(20)    << "xFace" 
            << std::setw(20)    << "dxCentro"
            << std::setw(20)    << "dxFace" 
            << "\n";
     PrintLine(_os, MYSIZE);
     
unsigned volume = 0;
auto     ptrdxCentro = std::begin(_grid1D.dxCentro);
auto     ptrdxFace = std::begin(_grid1D.dxFace);

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

    std::transform  (   std::begin(_grid1D.xCentro)
                    ,   std::end(_grid1D.xCentro)
                    ,   std::begin(_grid1D.xFace)
                    ,   std::ostream_iterator<std::string>(_os, "\n")
                    ,   Print
                    );

    
    _os     << std::setw(5) << volume  << std::scientific << std::setprecision(6)
            << std::setw(40) << *(std::prev(std::end(_grid1D.xFace)))
            << std::setw(20) << *(std::prev(std::end(_grid1D.dxCentro)))
            << "\n";
    
    _os << std::flush;


     PrintLine(_os, MYSIZE);
     
    return _os;
}

GRID_NAMESPACE_CLOSE