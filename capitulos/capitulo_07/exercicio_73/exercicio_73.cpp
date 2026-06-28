// ============================================================================
// Arquivo: exercicio_73.cpp
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Implementa exercicio 73 no contexto de capitulos / capitulo_07 / exercicio_73.
// Autor: João Flávio Vieira de Vasconcellos
//
// SPDX-FileCopyrightText: 2026 João Flávio Vieira de Vasconcellos
// SPDX-License-Identifier: BSD-3-Clause
//
// Este arquivo faz parte do FVMaker.
//
// Licença: BSD 3-Clause.
// É permitido usar, copiar, modificar e redistribuir este arquivo, em código-fonte
// ou forma binária, com ou sem modificações, desde que sejam preservados os avisos
// de copyright, esta identificação de licença e as condições descritas no arquivo
// LICENSE.md.
//
// O nome do autor, de colaboradores ou de instituições associadas ao projeto não
// pode ser usado para endossar ou promover produtos derivados sem autorização
// prévia por escrito.
//
// Este software é fornecido sem garantias de qualquer natureza. Consulte o arquivo
// LICENSE.md, na raiz do repositório, para o texto completo da licença.
// ============================================================================

#include <cmath>
#include <iomanip>
#include <iostream>
#include <numbers>
#include <vector>
#include <FVGridMaker/Core/StrongTypes.h>
#include <FVGridMaker/OneDimensional/Distribution1D/Custom1D.h>
#include <FVGridMaker/OneDimensional/Distribution1D/Uniform1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/Coordinates1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/VolumeCentered1D.h>
#include <FVMaker/Algebra/ErrorNorms.h>
#include <FVMaker/OneDimensional/Assembly/Assembler1D.h>
#include <FVMaker/OneDimensional/Equation/Equation1D.h>
#include <FVMaker/OneDimensional/System/AlgebraicResidual1D.h>
namespace {
using Real=fvm::Real;using Size=fvm::Size;
fvgrid::Axis1D eixo(Size n,bool perturbada){
 if(!perturbada)return fvgrid::uniform_axis_1d(fvgrid::NVol{n},fvgrid::Length{1.0},fvgrid::XInit{0.0});
 std::vector<fvgrid::Real> faces(n+1);const Real h=1.0/static_cast<Real>(n);
 for(Size i=0;i<=n;++i){const Real x=i*h;faces[i]=x+(i==0||i==n?0.0:0.12*h*std::sin(2.0*std::numbers::pi*x));}
 return fvgrid::Custom1D::make(fvgrid::Coordinates1D::faces(std::move(faces)),fvgrid::VolumeCentered1D{});
}
Real residuo(Size n,bool perturbada){
 const auto axis=eixo(n,perturbada);const fvm::GridView1D grid{axis};
 const fvm::Equation1D eq{grid,fvm::Laplacian1D{},fvm::function_source_1d(grid,[](Real x){return -std::numbers::pi*std::numbers::pi*std::cos(std::numbers::pi*x);}),{fvm::neumann_1d(0.0),fvm::dirichlet_1d(0.0)}};
 const auto a=fvm::assemble_steady_1d(eq);fvm::DenseVector phi{n};
 for(Size i=0;i<n;++i)phi[i]=1.0+std::cos(std::numbers::pi*grid.centers()[i]);
 const auto r=fvm::algebraic_residual(a,phi);fvm::DenseVector interior{n-2};for(Size i=1;i+1<n;++i)interior[i-1]=r[i];return fvm::norm_rms(interior);
}
void serie(bool perturbada){Real anterior{};std::cout<<(perturbada?"\nMalha perturbada\n":"Malha uniforme\n")<<"N              dx             RMS       ordem\n";
 for(Size n=16;n<=4096;n*=2){const Real r=residuo(n,perturbada);const Real p=anterior>0?std::log(anterior/r)/std::log(2.0):0.0;std::cout<<std::setw(6)<<n<<std::setw(18)<<1.0/n<<std::setw(18)<<r<<"  "<<std::setw(14)<<p<<'\n';anterior=r;}
}
}
int main(){std::cout<<std::scientific<<std::setprecision(8);serie(false);serie(true);}
