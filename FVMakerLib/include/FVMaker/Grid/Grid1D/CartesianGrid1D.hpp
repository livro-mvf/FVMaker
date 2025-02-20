////==============================================================================
//// Includes C++
////==============================================================================
//
//#include <cmath>        // fabs
//#include <iomanip>      // std::setw
//#include <iostream>     // std:iostream
//#include <iterator>     // std::ostream_iterator
//
////==============================================================================
//// Includes da biblioteca
////==============================================================================
//
//#include <MVF++/Grid/Grid1D/CartesianGrid1D.h>                  // CartesianGrid1D  
//#include <MVF++/Exception/MVFException.h>
//#include <MVF++/Misc/Misc.h>
//
//MVF_NAMESPACE_OPEN
//
////==============================================================================
//// Construtora
////==============================================================================
//
//#undef __FUNCT__
//#define __FUNCT__ "CartesianGrid1D<T>::CartesianGrid1D(const int&, const Real&, const Real&, const GRIDPATTERN&)"
//template<typename T>
//CartesianGrid1D<T>::CartesianGrid1D(const int& _nvol, const Real& _startX, const Real& _endX)
//    : nVol(_nvol), startX(_startX), endX(_endX) {
//    SourceInfo auxiSource;
//
//    try {
//        if (endX <= startX) {
//            auxiSource = SourceInfo(__FILE__, __LINE__ - 1, __FUNCT__);
//            throw MVFException(auxiSource, MVFException::BAD_DOMAIN_LENGTH);
//        }
//
//        if (nVol < 1) {
//            auxiSource = SourceInfo(__FILE__, __LINE__ - 1, __FUNCT__);
//            throw MVFException(auxiSource, MVFException::BAD_NUMBER_OF_VOLUMES);
//        }
//    } catch (const MVFException& e) {
//        MVFLogger::getInstance("error.log").log(e.what(), auxiSource);
//        std::cerr << "Exce��o capturada: " << e.what() << std::endl;
//        abort();
//    }
//
//    centreX.resize(_nvol);
//    faceX.resize(_nvol + 1);
//    centreDX.resize(_nvol + 1);
//    faceDX.resize(_nvol);
//
//    try {
//        if (centreX.size() != _nvol || faceDX.size() != _nvol || faceX.size() != _nvol + 1 ||
//            centreDX.size() != _nvol + 1) {
//            auxiSource = SourceInfo(__FILE__, __LINE__ - 1, __FUNCT__);
//            throw MVFException(auxiSource, MVFException::GRID_1D_PROBLEM);
//        }
//    } catch (const MVFException& e) {
//        MVFLogger::getInstance("error.log").log(e.what(), auxiSource);
//        std::cerr << "Exce��o capturada: " << e.what() << std::endl;
//        abort();
//    }
//}
//
//#undef __FUNCT__
//#define __FUNCT__ "bool CartesianGrid1D<T>::EvaluateCentreDx()"
//template<typename T>
//bool CartesianGrid1D<T>::EvaluateCentreDx() {
//    auto ptrCentreX = std::begin(CentreX());
//    auto ptrFaceX = std::begin(FaceX());
//    auto ptrCentreDX = std::begin(centreDX);
//    auto ptrFaceDX = std::begin(faceDX);
//    auto Diff = [](const auto& _xw, const auto& _xe) { return _xe - _xw; };
//    *ptrCentreDX = *ptrCentreX - *ptrFaceX;
//    std::transform(ptrCentreX, std::end(CentreX()) - 1, ptrCentreX + 1, ptrCentreDX + 1, Diff);
//    *(std::end(centreDX) - 1) = *(std::end(FaceX()) - 1) - *(std::end(CentreX()) - 1);
//    return true;
//}
//
//#undef __FUNCT__
//#define __FUNCT__ "bool CartesianGrid1D<T>::EvaluateFaceDx()"
//template<typename T>
//bool CartesianGrid1D<T>::EvaluateFaceDx() {
//    auto ptrCentreX = std::begin(CentreX());
//    auto ptrFaceX = std::begin(FaceX());
//    auto ptrCentreDX = std::begin(centreDX);
//    auto ptrFaceDX = std::begin(faceDX);
//    auto Diff = [](const auto& _xw, const auto& _xe) { return _xe - _xw; };
//    std::transform(ptrFaceX, std::end(FaceX()) - 1, ptrFaceX + 1, ptrFaceDX, Diff);
//    return true;
//}
//
//#undef __FUNCT__
//#define __FUNCT__ "bool CartesianGrid1D<T>::EvaluateCoordinates()"
//template<typename T>
//bool CartesianGrid1D<T>::EvaluateCoordinates() {
//    SourceInfo auxiSource;
//
//    try {
//        if (centreX.empty() || faceDX.empty() || faceX.empty() || centreDX.empty()) {
//            auxiSource = SourceInfo(__FILE__, __LINE__ - 1, __FUNCT__);
//            throw MVFException(auxiSource, MVFException::GRID_1D_PROBLEM);
//        }
//    } catch (const MVFException& e) {
//        MVFLogger::getInstance("error.log").log(e.what(), auxiSource);
//        std::cerr << "Exce��o capturada: " << e.what() << std::endl;
//        abort();
//    }
//
//    switch (str2int(GridPatternType().c_str())) {
//        case str2int("Cell Centered"):
////            MainCoordinate(faceX);
//            break;
//
//        case str2int("Face Centered"):
////            MainCoordinate(centreX);
//            break;
//
//        default:
//            std::cout << "Op��o inv�lida" << std::endl;
//            abort();
//    }
//
//    return true;
//}
//
////==============================================================================
//// Sobrecarga do operador <<
////==============================================================================
//
//template<typename T>
//std::ostream& operator<<(std::ostream& os, const CartesianGrid1D<T>& grid) {
//    std::_Ios_Fmtflags original_format = os.flags();
//
//    try {
//        if (grid.empty()) {
//            throw MVFException(SourceInfo(__FILE__, __LINE__ - 1, __FUNCTION__), MVFException::UNDEFINED_GRID);
//        }
//    } catch (const MVFException& e) {
//        MVFLogger::getInstance("error.log").log(e.what(), e.GetExceptionSource());
//        std::cerr << "Exce��o capturada: " << e.what() << std::endl;
//    }
//
//    const char ASCIINUMBER = 0x55;
//    PrintLine(os, ASCIINUMBER);
//
//    os << "Impress�o:" << grid.className() << "\t\tTipo: " << grid.GridPatternType() << "\n";
//    PrintLine(os, ASCIINUMBER);
//    os << "\n";
//
//    const Real MINIMO = 1e-6;
//    auto PrintNumber = [&MINIMO](const Real& x) {
//        std::stringstream ss;
//        if (std::fabs(x) < MINIMO) {
//            ss << std::setprecision(2) << std::scientific << x;
//        } else {
//            ss << std::setprecision(6) << std::fixed << x;
//        }
//        return ss.str();
//    };
//
//    os << std::setw(25) << "XCentre" << std::setw(20) << "dXCentre" << std::setw(20) << "XFace" << std::setw(20)
//       << "dXFace" << std::endl;
//
//    int no = 0;
//    auto ptr_dxC = std::begin(grid.centreDX);
//    auto ptr_dxF = std::begin(grid.faceDX);
//
//    auto PrintTable = [&no, &ptr_dxC, &ptr_dxF, PrintNumber](const auto& xC, const auto& xF) {
//        std::stringstream ss;
//        ss << std::setw(5) << no++ << std::setw(20) << PrintNumber(xC) << std::setw(20) << PrintNumber(*ptr_dxC++)
//           << std::setw(20) << PrintNumber(xF) << std::setw(20) << PrintNumber(*ptr_dxF++);
//        return ss.str();
//    };
//
//    std::transform(std::begin(grid.CentreX()), std::end(grid.CentreX()), std::begin(grid.FaceX()),
//                   std::ostream_iterator<std::string>(os, "\n"), PrintTable);
//
//    os << std::setw(5) << no << std::setw(40) << PrintNumber(*ptr_dxC++) << std::setw(20)
//       << PrintNumber(*(--std::end(grid.FaceX())));
//
//    os.flags(original_format);
//
//    return os;
//}
//
//MVF_NAMESPACE_CLOSE
//
//
//
//
//////==============================================================================
//////   include c++
//////==============================================================================
////
////#include <cmath>        // fabs
////#include <iomanip>      // std::setw
////#include <iostream>     // std:iostream
////#include <iterator>     // std::ostream_iterator
//////
//////==============================================================================
//////   include da biblioteca
//////==============================================================================
////
////
////#include <MVF++/Grid/Grid1D/CartesianGrid1D.h>                  // CartesianGrid1D  
////#include <MVF++/Exception/MVFException.h>                       // MVFException
////#include <MVF++/Misc/Misc.h>                                    // PrintLine
////
////MVF_NAMESPACE_OPEN
////
////
////
//////==============================================================================
//////   construtora
//////==============================================================================
//////
////
////#undef __FUNCT__
////#define __FUNCT__ "CartesianGrid1D<T>  ::CartesianGrid1D (const int&, const Real&, const Real&, const GRIDPATTERN& )"
////template<typename T>
////CartesianGrid1D<T> ::   CartesianGrid1D    (   const int&                           _nvol           // Numero de volumes
////                                        ,   const Real&                             _startX         // Comprimento do dominio
////                                        ,   const Real&                             _endX           // Coordenada inicial
////                                        )  
////                                        :   nVol(_nvol)
////                                        ,   startX(_startX)
////                                        ,   endX(_endX)
////{
////  SourceInfo   auxiSource;
////    
////   try {
////       
////       if (endX <= startX) {
////            auxiSource = SourceInfo(__FILE__, __LINE__ - 1, __FUNCT__);
////            throw MVFException(auxiSource, MVFException::BAD_DOMAIN_LENGTH);
////       }
////
////       if (nVol < 1) {
////            auxiSource = SourceInfo(__FILE__, __LINE__ - 1, __FUNCT__);
////            throw MVFException(auxiSource, MVFException::BAD_NUMBER_OF_VOLUMES);
////       }
////    } catch (const MVFException& e) {
////        MVFLogger::getInstance("error.log").log(e.what(), auxiSource);
////        std::cerr << "Exce��o capturada: " << e.what() << std::endl;
////        abort();
////    }
////  
////    centreX.resize(_nvol);
////    faceX.resize(_nvol+1);
////    centreDX.resize(_nvol + 1);
////    faceDX.resize(_nvol);
////
////    try {
////       
////       if (centreX.size() != _nvol) {
////            auxiSource = SourceInfo(__FILE__, __LINE__ - 1, __FUNCT__);
////            throw MVFException(auxiSource, MVFException::CENTRE_GRID_1D_PROBLEM);
////       }
////
////       if (faceDX.size() != _nvol) {
////            auxiSource = SourceInfo(__FILE__, __LINE__ - 1, __FUNCT__);
////            throw MVFException(auxiSource, MVFException::FACEDX_GRID_1D_PROBLEM);
////       }
////
////       if (faceX.size() != _nvol + 1) {
////            auxiSource = SourceInfo(__FILE__, __LINE__ - 1, __FUNCT__);
////            throw MVFException(auxiSource, MVFException::FACE_GRID_1D_PROBLEM);
////       }
////       if (centreDX.size() != _nvol + 1) {
////            auxiSource = SourceInfo(__FILE__, __LINE__ - 1, __FUNCT__);
////            throw MVFException(auxiSource, MVFException::CENTREDX_GRID_1D_PROBLEM);
////       }
////
////    } catch (const MVFException& e) {
////        MVFLogger::getInstance("error.log").log(e.what(), auxiSource);
////        std::cerr << "Exce��o capturada: " << e.what() << std::endl;
////        abort();
////    }
////    
////
////}
////
////
////#undef __FUNCT__
////#define __FUNCT__ " bool CartesianGrid1D<T> :: EvaluateCentreDx () "
////    template<typename T>
//// bool CartesianGrid1D<T> :: EvaluateCentreDx () {
////        
////auto    ptrCentreX  = std::begin(CentreX());
////auto    ptrFaceX    = std::begin(FaceX());
////auto    ptrCentreDX = std::begin(centreDX);
////auto    ptrFaceDX   = std::begin(faceDX);
////auto Diff = [] (const auto& _xw,  const auto& _xe) { return _xe - _xw; };
////    *ptrCentreDX = *ptrCentreX - *ptrFaceX;
////    std::transform  (   ptrCentreX
////                    ,   std::end(CentreX()) - 1
////                    ,   ptrCentreX + 1
////                    ,   ptrCentreDX + 1
////                    ,   Diff
////                    );       
////    *(std::end(centreDX) - 1) = *(std::end(FaceX()) - 1) - *(std::end(CentreX()) - 1);        
////        return true;
////    }   
////    
////    
////#undef __FUNCT__
////#define __FUNCT__ "bool CartesianGrid1D<T> :: EvaluateFaceDx ()"    
////     template<typename T>
//// bool CartesianGrid1D<T> :: EvaluateFaceDx () {
////         
////auto    ptrCentreX  = std::begin(CentreX());
////auto    ptrFaceX    = std::begin(FaceX());
////auto    ptrCentreDX = std::begin(centreDX);
////auto    ptrFaceDX   = std::begin(faceDX);
////
////auto Diff = [] (const auto& _xw,  const auto& _xe) { return _xe - _xw; };
////    std::transform  (   ptrFaceX
////                    ,   std::end(FaceX()) - 1
////                    ,   ptrFaceX + 1
////                    ,   ptrFaceDX
////                    ,   Diff
////                    );  
////        return true;
////    }   
////
////    
////    
////
////#undef __FUNCT__
////#define __FUNCT__ "bool CartesianGrid1D<T> :: EvaluateCoordinates () "
////template<typename T>
////bool CartesianGrid1D<T> :: EvaluateCoordinates () {
////
////  SourceInfo   auxiSource;
////  
////    try {
////       
////       if (centreX.empty()) {
////            auxiSource = SourceInfo(__FILE__, __LINE__ - 1, __FUNCT__);
////            throw MVFException(auxiSource, MVFException::CENTRE_GRID_1D_PROBLEM);
////       }
////
////       if (faceDX.empty()) {
////            auxiSource = SourceInfo(__FILE__, __LINE__ - 1, __FUNCT__);
////            throw MVFException(auxiSource, MVFException::FACEDX_GRID_1D_PROBLEM);
////       }
////
////       if (faceX.empty()) {
////            auxiSource = SourceInfo(__FILE__, __LINE__ - 1, __FUNCT__);
////            throw MVFException(auxiSource, MVFException::FACE_GRID_1D_PROBLEM);
////       }
////       if (centreDX.empty()) {
////            auxiSource = SourceInfo(__FILE__, __LINE__ - 1, __FUNCT__);
////            throw MVFException(auxiSource, MVFException::CENTREDX_GRID_1D_PROBLEM);
////       }
////
////    } catch (const MVFException& e) {
////        MVFLogger::getInstance("error.log").log(e.what(), auxiSource);
////        std::cerr << "Exce��o capturada: " << e.what() << std::endl;
////        abort();
////    }
////
////    switch (str2int(GridPatternType().c_str())) {
////        case str2int("Cell Centered") : 
////            MainCoordinate (faceX);
////            break;
////            
////        case str2int("Face Centered"): 
////            MainCoordinate (centreX);
////            break;
////        
////        default:
////            std::cout << "Invalid option" << std::endl;
////            abort();
////    }
////
//////auto  flag = EvaluateOtherCoordinate();
//////     flag = EvaluateFaceDx ();
//////     flag = EvaluateCentreDx ();
////     return true;
////     
//// }
////
////
//////==============================================================================
//////   sobrecarga do operador <<
//////==============================================================================
////
////
////template<typename T>
////std::ostream& operator << (std::ostream& os, const CartesianGrid1D<T>& grid)
////{
////    std::_Ios_Fmtflags original_format = os.flags(); // Guarda o formato original
////
////    try {
////        if (grid.empty()) {
////            throw MVFException(SourceInfo(__FILE__, __LINE__ - 1, __FUNCTION__), MVFException::UNDEFINIED_GRID);
////        }
////    } catch (const MVFException& e) {
////        MVFLogger::getInstance("error.log").log(e.what(), e.GetExceptionSource());
////        std::cerr << "Exceção capturada: " << e.what() << std::endl;
////    }    
////
////    const char ASCIINUMBER = 0x55;
////    PrintLine(os, ASCIINUMBER);
////
////    os << "Impressao:" << grid.className() << "\t\tTipo: " << grid.GridPatternType() << "\n";
////    PrintLine(os, ASCIINUMBER);
////    os << "\n";
////
////    const Real MINIMO = 1e-6;
////    auto PrintNumber = [&MINIMO](const Real& x) {
////        std::stringstream ss;
////        if (std::fabs(x) < MINIMO) {
////            ss << std::setprecision(2) << std::scientific << x;     
////        } else {
////            ss << std::setprecision(6) << std::fixed << x;          
////        }
////        return ss.str();
////    };        
////
////    os << std::setw(25) << "XCentre"
////       << std::setw(20) << "dXCentre"
////       << std::setw(20) << "XFace"
////       << std::setw(20) << "dXFace" << std::endl;
////
////    UInt no = 0;
////    auto ptr_dxC = std::begin(grid.centreDX);
////    auto ptr_dxF = std::begin(grid.faceDX);
////
////    auto PrintTable = [&no, &ptr_dxC, &ptr_dxF, PrintNumber](const auto& xC, const auto& xF) {
////        std::stringstream ss;
////        ss << std::setw(5) << no++
////           << std::setw(20) << PrintNumber(xC)
////           << std::setw(20) << PrintNumber(*ptr_dxC++)
////           << std::setw(20) << PrintNumber(xF)
////           << std::setw(20) << PrintNumber(*ptr_dxF++);
////        return ss.str();
////    };
////
////    std::transform(std::begin(grid.CentreX()), std::end(grid.CentreX()), std::begin(grid.FaceX()),
////                   std::ostream_iterator<std::string>(os, "\n"), PrintTable);
////
////    os << std::setw(5) << no
////       << std::setw(40) << PrintNumber(*ptr_dxC++)
////       << std::setw(20) << PrintNumber(*(--std::end(grid.FaceX())));
////
////    os.flags(original_format); // Restaura o formato original
////
////    return os;
////}
////
////
////MVF_NAMESPACE_CLOSE