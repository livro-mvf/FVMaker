// Exercicio 7.4 - influencia da frequencia da solucao.
#include <array>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <numbers>
#include <FVGridMaker/Core/StrongTypes.h>
#include <FVGridMaker/OneDimensional/Distribution1D/Uniform1D.h>
#include <FVMaker/Algebra/ErrorNorms.h>
#include <FVMaker/OneDimensional/Assembly/Assembler1D.h>
#include <FVMaker/OneDimensional/Equation/Equation1D.h>
#include <FVMaker/OneDimensional/System/AlgebraicResidual1D.h>
namespace {
using Real=fvm::Real;using Size=fvm::Size;
Real residuo(Size volumes,unsigned modo){
 const auto axis=fvgrid::uniform_axis_1d(fvgrid::NVol{volumes},fvgrid::Length{1.0},fvgrid::XInit{0.0});const fvm::GridView1D grid{axis};const Real k=modo*std::numbers::pi;
 const fvm::Equation1D eq{grid,fvm::Laplacian1D{},fvm::function_source_1d(grid,[k](Real x){return -k*k*std::cos(k*x);}),{fvm::neumann_1d(0.0),fvm::dirichlet_1d(std::cos(k))}};
 const auto a=fvm::assemble_steady_1d(eq);fvm::DenseVector phi{volumes};for(Size i=0;i<volumes;++i)phi[i]=std::cos(k*grid.centers()[i]);
 const auto r=fvm::algebraic_residual(a,phi);fvm::DenseVector interior{volumes-2};for(Size i=1;i+1<volumes;++i)interior[i-1]=r[i];return fvm::norm_rms(interior);
}
}
int main(){std::cout<<std::scientific<<std::setprecision(8);
 for(unsigned modo:std::array{1U,2U,4U,8U}){std::cout<<"\nn = "<<modo<<"\nN              dx             RMS       ordem\n";Real anterior{};
  for(Size n=16;n<=8192;n*=2){const Real r=residuo(n,modo);const Real p=anterior>0?std::log(anterior/r)/std::log(2.0):0.0;std::cout<<std::setw(6)<<n<<std::setw(18)<<1.0/n<<std::setw(18)<<r<<"  "<<std::setw(14)<<p<<'\n';anterior=r;}
 }
}
