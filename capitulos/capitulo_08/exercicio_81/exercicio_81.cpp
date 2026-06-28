// ============================================================================
// Arquivo: exercicio_81.cpp
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Implementa exercicio 81 no contexto de capitulos / capitulo_08 / exercicio_81.
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
#include <vector>
#include <FVGridMaker/OneDimensional/Distribution1D/Custom1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/Coordinates1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/VolumeCentered1D.h>
#include <FVMaker/OneDimensional/Coefficient/DiffusionCoefficient1D.h>
int main(){
 const auto axis=fvgrid::Custom1D::make(fvgrid::Coordinates1D::faces({0.0,0.2,1.0}),fvgrid::VolumeCentered1D{});const fvm::GridView1D grid{axis};const fvm::DenseVector gamma{{1.0,10.0}};
 const auto arit=fvm::arithmetic_field_coefficient_1d(grid,gamma);const auto harm=fvm::harmonic_field_coefficient_1d(grid,gamma);
 const auto inventada=fvm::interpolated_field_coefficient_1d(grid,gamma,[](fvm::FaceInterpolationData1D d){return std::sqrt(d.west_value*d.east_value);});
 std::cout<<std::fixed<<std::setprecision(10)<<"Centros: "<<grid.centers()[0]<<", "<<grid.centers()[1]<<"  face interna: "<<grid.faces()[1]<<"\n";
 std::cout<<"face        aritmetica       harmonica       geometrica(usuario)\n";
 for(fvm::Size f=0;f<grid.num_faces();++f)std::cout<<std::setw(4)<<f<<std::setw(18)<<arit.face_value(f)<<std::setw(18)<<harm.face_value(f)<<std::setw(24)<<inventada.face_value(f)<<'\n';
}
