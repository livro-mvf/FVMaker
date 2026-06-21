// Exercicio 8.3 - coeficiente que varia continuamente.
#include <algorithm>
#include <array>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <numbers>
#include <string>
#include <FVGridMaker/Core/StrongTypes.h>
#include <FVGridMaker/OneDimensional/Distribution1D/Uniform1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/FaceCentered1D.h>
#include <FVMaker/Algebra/ErrorNorms.h>
#include <FVMaker/OneDimensional/Assembly/Assembler1D.h>
#include <FVMaker/OneDimensional/Equation/Equation1D.h>
#include <FVMaker/OneDimensional/Solver/TDMA.h>
namespace {
using Real=fvm::Real;using Size=fvm::Size;
fvm::DenseVector resolver(const fvm::GridView1D& grid,Real c,unsigned modo,bool harmonica){
 fvm::DenseVector gamma{grid.num_volumes()};for(Size i=0;i<grid.num_volumes();++i){const Real y=(grid.centers()[i]-grid.xmin())/grid.length();gamma[i]=1.0+c*std::pow(std::sin(modo*std::numbers::pi*y),2);}
 const auto faces=harmonica?fvm::harmonic_field_coefficient_1d(grid,gamma):fvm::arithmetic_field_coefficient_1d(grid,gamma);
 const fvm::Equation1D eq{grid,fvm::Laplacian1D{faces},fvm::zero_source_1d(grid.num_volumes()),{fvm::dirichlet_1d(0.0),fvm::dirichlet_1d(1.0)}};
 return fvm::TDMA::solve(fvm::assemble_steady_1d(eq)).solution;
}
}
int main(int argc,char** argv){const Real L=argc>1?std::stod(argv[1]):1.0;const Real x0=argc>2?std::stod(argv[2]):0.0;const Size nvol=argc>3?std::stoull(argv[3]):200;
 const auto axis=fvgrid::Uniform1D::make(fvgrid::NVol{nvol},fvgrid::Length{L},fvgrid::XInit{x0},fvgrid::FaceCentered1D{});const fvm::GridView1D grid{axis};
 std::cout<<std::scientific<<std::setprecision(8)<<"L="<<L<<" xI="<<x0<<" N="<<nvol<<" padrao="<<grid.pattern_name()<<"\nC0       n      phi_meio(harm)   diferenca max arit-harm\n";
 for(Real c:std::array{0.0,10.0,100.0})for(unsigned modo:std::array{1U,4U,8U}){const auto h=resolver(grid,c,modo,true),a=resolver(grid,c,modo,false);std::cout<<std::setw(8)<<c<<std::setw(5)<<modo<<std::setw(20)<<h[nvol/2]<<std::setw(24)<<fvm::norm_infinity(a-h)<<'\n';}
}
