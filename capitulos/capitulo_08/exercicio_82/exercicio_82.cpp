// Exercicio 8.2 - verificacao por solucao fabricada.
#include <cmath>
#include <iomanip>
#include <iostream>
#include <numbers>
#include <FVGridMaker/Core/StrongTypes.h>
#include <FVGridMaker/OneDimensional/Distribution1D/Uniform1D.h>
#include <FVMaker/Algebra/ErrorNorms.h>
#include <FVMaker/OneDimensional/Assembly/Assembler1D.h>
#include <FVMaker/OneDimensional/Equation/Equation1D.h>
#include <FVMaker/OneDimensional/Solver/TDMA.h>
namespace {
using Real=fvm::Real;using Size=fvm::Size;
Real erro(Size n){
 const auto axis=fvgrid::uniform_axis_1d(fvgrid::NVol{n},fvgrid::Length{1.0},fvgrid::XInit{0.0});const fvm::GridView1D grid{axis};fvm::DenseVector gamma{n};
 for(Size i=0;i<n;++i)gamma[i]=1.0+grid.centers()[i]*grid.centers()[i];
 const auto faces=fvm::harmonic_field_coefficient_1d(grid,gamma);
 const auto rhs=[](Real x){return 2.0*std::numbers::pi*x*std::cos(std::numbers::pi*x)-(1.0+x*x)*std::numbers::pi*std::numbers::pi*std::sin(std::numbers::pi*x);};
 const fvm::Equation1D eq{grid,fvm::Laplacian1D{faces},fvm::function_source_1d(grid,rhs),{fvm::dirichlet_1d(0.0),fvm::dirichlet_1d(0.0)}};
 const auto result=fvm::TDMA::solve(fvm::assemble_steady_1d(eq));fvm::DenseVector e{n};for(Size i=0;i<n;++i)e[i]=result.solution[i]-std::sin(std::numbers::pi*grid.centers()[i]);return fvm::norm_rms(e);
}
}
int main(){std::cout<<std::scientific<<std::setprecision(8)<<"N              dx        erro RMS       ordem\n";Real anterior{};for(Size n=16;n<=4096;n*=2){const Real e=erro(n),p=anterior>0?std::log(anterior/e)/std::log(2.0):0.0;std::cout<<std::setw(6)<<n<<std::setw(16)<<1.0/n<<std::setw(16)<<e<<"  "<<std::setw(12)<<p<<'\n';anterior=e;}}
