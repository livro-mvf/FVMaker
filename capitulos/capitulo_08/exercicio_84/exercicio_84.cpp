// ============================================================================
// Arquivo: exercicio_84.cpp
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Implementa exercicio 84 no contexto de capitulos / capitulo_08 / exercicio_84.
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
#include <string_view>
#include <FVGridMaker/Core/StrongTypes.h>
#include <FVGridMaker/OneDimensional/Distribution1D/Uniform1D.h>
#include <FVMaker/Algebra/ErrorNorms.h>
#include <FVMaker/OneDimensional/Assembly/Assembler1D.h>
#include <FVMaker/OneDimensional/Equation/Equation1D.h>
#include <FVMaker/OneDimensional/Solver/TDMA.h>
namespace {
using Real=fvm::Real;using Size=fvm::Size;
struct Resultado{Real gamma_face;Real erro_rms;Real fluxo;};
Resultado executar(Real interface,bool harmonica){
 constexpr Size n=40;constexpr Real g1=1.0,g2=100.0;
 const auto axis=fvgrid::uniform_axis_1d(fvgrid::NVol{n},fvgrid::Length{1.0},fvgrid::XInit{0.0});const fvm::GridView1D grid{axis};fvm::DenseVector gamma{n};
 for(Size i=0;i<n;++i)gamma[i]=grid.centers()[i]<interface?g1:g2;
 const auto faces=harmonica?fvm::harmonic_field_coefficient_1d(grid,gamma):fvm::arithmetic_field_coefficient_1d(grid,gamma);
 const fvm::Equation1D eq{grid,fvm::Laplacian1D{faces},fvm::zero_source_1d(n),{fvm::dirichlet_1d(0.0),fvm::dirichlet_1d(1.0)}};
 const auto sol=fvm::TDMA::solve(fvm::assemble_steady_1d(eq)).solution;const Real resistencia=interface/g1+(1.0-interface)/g2,fluxo=1.0/resistencia;fvm::DenseVector erro{n};
 for(Size i=0;i<n;++i){const Real x=grid.centers()[i];const Real exata=x<=interface?fluxo*x/g1:fluxo*(interface/g1+(x-interface)/g2);erro[i]=sol[i]-exata;}
 Size face=1;for(Size f=2;f+1<grid.num_faces();++f)if(std::abs(grid.faces()[f]-interface)<std::abs(grid.faces()[face]-interface))face=f;
 const Real fluxo_num=faces.face_value(face)*(sol[face]-sol[face-1])/(grid.centers()[face]-grid.centers()[face-1]);
 return {faces.face_value(face),fvm::norm_rms(erro),fluxo_num};
}
void caso(Real interface){std::cout<<"\nInterface x="<<interface<<(std::abs(interface-0.5)<1e-12?" (alinhada a face)":" (corta um volume)")<<"\nmetodo          gamma_face       erro RMS          fluxo\n";for(bool h:{true,false}){const auto r=executar(interface,h);std::cout<<std::setw(12)<<(h?"harmonica":"aritmetica")<<std::setw(18)<<r.gamma_face<<std::setw(18)<<r.erro_rms<<std::setw(18)<<r.fluxo<<'\n';}}
}
int main(){std::cout<<std::scientific<<std::setprecision(8);caso(0.5);caso(0.53);}
