// Exercicio 7.5 - teste por solucao manufaturada.
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
Real phi(Real x){return 1.0+x*x+0.2*std::sin(2.0*std::numbers::pi*x);}
Real segunda(Real x){return 2.0-0.8*std::numbers::pi*std::numbers::pi*std::sin(2.0*std::numbers::pi*x);}
Real residuo(Size n){
 const auto axis=fvgrid::uniform_axis_1d(fvgrid::NVol{n},fvgrid::Length{1.0},fvgrid::XInit{0.0});const fvm::GridView1D grid{axis};
 const fvm::Equation1D eq{grid,fvm::Laplacian1D{},fvm::function_source_1d(grid,[](Real x){return segunda(x);}),{fvm::dirichlet_1d(phi(0.0)),fvm::dirichlet_1d(phi(1.0))}};
 const auto a=fvm::assemble_steady_1d(eq);fvm::DenseVector exata{n};for(Size i=0;i<n;++i)exata[i]=phi(grid.centers()[i]);
 const auto r=fvm::algebraic_residual(a,exata);fvm::DenseVector interior{n-2};for(Size i=1;i+1<n;++i)interior[i-1]=r[i];return fvm::norm_rms(interior);
}
}
int main(){std::cout<<"Solucao manufaturada: phi=1+x^2+0.2 sin(2 pi x)\nEquacao: phi''=2-0.8 pi^2 sin(2 pi x)\nContornos: phi(0)=1, phi(1)=2\n\n";std::cout<<std::scientific<<std::setprecision(8)<<"N              dx             RMS       ordem\n";Real anterior{};for(Size n=16;n<=8192;n*=2){const Real r=residuo(n);const Real p=anterior>0?std::log(anterior/r)/std::log(2.0):0.0;std::cout<<std::setw(6)<<n<<std::setw(18)<<1.0/n<<std::setw(18)<<r<<"  "<<std::setw(14)<<p<<'\n';anterior=r;}}
