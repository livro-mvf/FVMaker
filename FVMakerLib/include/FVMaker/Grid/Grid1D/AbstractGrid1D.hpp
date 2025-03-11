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

GRID_NAMESPACE_OPEN

template <typename T>
AbstractGrid1D<T> :: AbstractGrid1D     (   const int&          _nvol
                        ,   const Real&         _length
                        ,   const Real&         _xIni
                        ) 
                        :   nVol_(_nvol)
                        ,   length_(_length)
                        ,   xIni_(_xIni)
{
    try {
        if (nVol_ < 2)  throw fvm::FVMakerException(ErrorCode::InvalidNumbersOfVolumes);
        if (length_ <= 0)  throw fvm::FVMakerException(ErrorCode::InvalidLength);
        InitVector ();
        
    } catch (const fvm::FVMakerException& e) {
        std::cerr << "\n\n";
        PrintLine(std::cerr);
        std::cerr << "Exceção capturada: " << e.what() << "\n";
        PrintLine(std::cerr);
        std::cerr << "\n\n";
        exit(EXIT_FAILURE);
    }
    
    typePattern_ = std::make_shared<T>();
}

template <typename T>
void AbstractGrid1D<T> :: InitVector () {
    
    try {    
        xFace_.reserve(nVol_ + 1);  
        xFace_.resize(nVol_ + 1, 0.0);

        dxCentro_.reserve(nVol_ + 1);  
        dxCentro_.resize(nVol_ + 1, 0.0);

        xCentro_.reserve(nVol_);  
        xCentro_.resize(nVol_, 0.0);

        dxFace_.reserve(nVol_);  
        dxFace_.resize(nVol_, 0.0);  
    } catch (const std::bad_alloc&) {
        throw fvm::FVMakerException(ErrorCode::MemoryAllocationError);
    } catch (const std::length_error&) {
        throw fvm::FVMakerException(ErrorCode::VectorResizeError);
    } catch (const std::exception& e) {
        throw fvm::FVMakerException(ErrorCode::UnhandledException);
    }
    
}

template<typename T>
std::ostream& operator<<(std::ostream& _os, const AbstractGrid1D<T>& _grid1D) {
const int MYSIZE= LSIZE + 5;    
     _os << "\nImpressao do AbstractGrid1D\n";
     PrintLine(_os, MYSIZE);
     _os    << std::setw(5)     << "i" 
            << std::setw(20)    << "xCentro"
            << std::setw(20)    << "xFace" 
            << std::setw(20)    << "dxCentro"
            << std::setw(20)    << "dxFace" 
            << "\n";
     PrintLine(_os, MYSIZE);
     
    const std::size_t N = _grid1D.NVol();

    std::vector<std::size_t> indices(N > 3 ? N : 0);
    std::iota(indices.begin(), indices.end(), 0);
 
const auto xFace = _grid1D.FaceCoordinate();

//    std::cout << "Aqio   " << xFace[0] << "\n\n";

auto Print = [&](const std::size_t& i)
{
    std::stringstream ss;
    ss << std::setw(5) << i;
 //      << std::setw(20) << _grid1D.xCentro_[i];
 //      << std::setw(20) << _grid1D.xFace_[i];
    return ss.str(); 
};

    std::transform  (   std::execution::par
                    ,   std::begin(indices)
                    ,   std::end(indices)
                    ,   std::ostream_iterator<std::string>(_os, "\n")
                    ,   Print
                    );

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

template <typename T>
bool AbstractGrid1D<T> :: CalculaCentros(const Real& _offset) {
    
    if (!fvm::OrdemCrescente(xFace_)) {
        std::cout << "As coordenadas das faces não estão ordenadas\n";
        exit(EXIT_FAILURE);
    }
    
    auto Media = [_offset] (const Real& _x, const Real& _y) {return _y + _offset * (_x - _y);};
    std::transform  (   std::execution::par
                    ,   xFace_.begin()
                    ,   xFace_.end() - 1
                    ,   xFace_.begin() + 1
                    ,   xCentro_.begin()
                    ,   Media
                    );    
    
    return true;
}

template <typename T>
bool AbstractGrid1D<T> :: CalculaFaces(const Real& _offset) {
    
    xFace_[0]=  this->XInit();
    xFace_[this->NVol()] = this->XInit() + this->Length();    
    
    if (!fvm::OrdemCrescente(xCentro_)) {
        std::cout << "As coordenadas dos centros não estão ordenadas\n";
        exit(EXIT_FAILURE);
    }
    
    auto Media = [_offset] (const Real& _x, const Real& _y) {return _y + _offset * (_x - _y);};
    std::transform  (   std::execution::par
                    ,   xCentro_.begin()
                    ,   xCentro_.end() - 1
                    ,   xCentro_.begin() + 1
                    ,   xFace_.begin() + 1
                    ,   Media
                    );    
    
    return true;
}


template <typename T>
bool AbstractGrid1D<T> :: CalculaDistancias() {
    

    auto Distancia = [] (const Real& _x, const Real& _y) {return _x - _y;};
    std::transform  (   std::execution::par
                    ,   xFace_.begin() + 1
                    ,   xFace_.end()
                    ,   xFace_.begin()
                    ,   dxFace_.begin()
                    ,   Distancia
                    );    

    std::transform  (   std::execution::par
                    ,   xCentro_.begin() + 1
                    ,   xCentro_.end()
                    ,   xCentro_.begin()
                    ,   dxCentro_.begin() + 1
                    ,   Distancia
                    );    
    dxCentro_[0]    = xCentro_[0] - xFace_[0];
    dxCentro_[nVol_] = xFace_[nVol_] - xCentro_[nVol_ - 1];
    
    return true;
}

GRID_NAMESPACE_CLOSE