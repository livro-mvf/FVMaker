// ============================================================================
// Arquivo: exercicio_72.cpp
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Implementa exercicio 72 no contexto de capitulos / capitulo_07 / exercicio_72.
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

#include <array>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <string_view>
template<class Real> void executar(std::string_view nome){
 constexpr std::array<unsigned long long,4> limites{100000ULL,1000000ULL,10000000ULL,100000000ULL};
 Real soma{};std::size_t proximo{};
 for(unsigned long long i=1;i<=limites.back();++i){soma+=Real{0.1};if(i!=limites[proximo])continue;
  const long double exata=static_cast<long double>(i)*0.1L;
  const long double erro=static_cast<long double>(soma)-exata;
  std::cout<<std::setw(14)<<nome<<std::setw(14)<<i<<std::setw(24)<<static_cast<long double>(soma)<<std::setw(22)<<erro<<std::setw(20)<<100.0L*std::abs(erro)/exata<<'\n';
  if(++proximo==limites.size())break;
 }
}
int main(){std::cout<<std::scientific<<std::setprecision(12)<<std::setw(14)<<"tipo"<<std::setw(14)<<"N"<<std::setw(24)<<"soma"<<std::setw(22)<<"erro"<<std::setw(20)<<"erro (%)"<<'\n';executar<float>("float");executar<double>("double");executar<long double>("long double");}
