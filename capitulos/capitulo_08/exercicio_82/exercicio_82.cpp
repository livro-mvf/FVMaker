// ============================================================================
// Arquivo: exercicio_82.cpp
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Implementa exercicio 82 no contexto de capitulos / capitulo_08 / exercicio_82.
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
