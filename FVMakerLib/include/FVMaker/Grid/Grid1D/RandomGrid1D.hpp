//
////============================================================================== 
////   includes  
////============================================================================== 
//
//
//
//#include <iostream>
//#include <chrono>               // std::chrono
//#include <random>               // std::uniform_real_distribution
//
////============================================================================== 
////   includes  do biblioteca
////============================================================================== 
//
//#include <MVF++/Grid/Grid1D/RandomGrid1D.h>
//
//MVF_NAMESPACE_OPEN
//        
//
//template <typename TypePattern>
//RandomGrid1D<TypePattern> ::     RandomGrid1D   (   const int&                 _nvol
//                                                ,   const Real&                _lenght
//                                                ,   const Real&                _xi
//                                                ) 
//                                                :   CartesianGrid1D<TypePattern>    (    _nvol
//                                                                                    ,   _xi
//                                                                                    ,   _lenght + _xi
//                                                                                    )
//{
//}
//
//
//template <typename TypePattern>
//bool RandomGrid1D<TypePattern>  :: MainCoordinate (VecReal& _phi)
//{
// 
//auto seed(std::chrono::system_clock::now().time_since_epoch().count());    
//
//std::default_random_engine                       generator(seed);
//
//std::uniform_real_distribution<Real>             distribution   ( this->StartX()
//                                                                , this->EndX()
//                                                                );  
//auto GeraNumero = [&distribution, &generator] () {return distribution(generator);};
//    
//auto     ptrPhi = std::begin(_phi);
//    
//    switch (str2int(this->GridPatternType().c_str())) {
//        case str2int("Cell Centered") : 
//            std::cout << "Passou por Cell Centered \n";
//            *ptrPhi    =  this->StartX();
//            std::generate_n (   ptrPhi + 1
//                            ,   this->NVol()
//                            ,   GeraNumero
//                            );       
//            *(ptrPhi + this->NVol())    =  this->EndX();
//            break;
//        case str2int("Face Centered"): 
//            std::cout << "Passou por Face Centered \n";
//            std::generate_n (   ptrPhi
//                            ,   this->NVol()
//                            ,   GeraNumero
//                            ); 
//            break;
//        default:
//            std::cout << "Invalid option" << std::endl;
//            abort();
//    }
//    
//    std::sort   ( std::begin(_phi), std::end(_phi));
//
//    return true;
//
//}
//
//MVF_NAMESPACE_CLOSE
