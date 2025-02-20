#pragma once
#include <FVMaker/Error/FVMakerException.h>
#include <FVMaker/Misc/Misc.h>
#include <iostream>

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

GRID_NAMESPACE_CLOSE