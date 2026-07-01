// ============================================================================
// Arquivo: coefficient1D.hpp
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Declara coefficient 1 D no contexto de Coefficient.
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

#pragma once

//==============================================================================
// Includes da biblioteca padrão do C++
//==============================================================================

#include <iomanip>

//==============================================================================
// Incluições do FVMAKER
//==============================================================================
#include <FVMaker/Error/FVMakerException.h>
#include <FVMaker/Grid/abstractGrid.h>              ///< Definições da Grid; 
#include <FVMaker/Misc/misc.h>
using fvm::grd::AbstractGrid;


FVMAKER_NAMESPACE_OPEN

        
// Escreve uma representacao textual do objeto no fluxo de saida.
template<typename T>
std::ostream& operator<< (std::ostream& _os, const Coefficient1D<T>& _coeff1D) {

std::ios_base::fmtflags fflags = _os.flags();
const int MYSIZE= LSIZE - 35;    
     _os << "\nImpressao do Coefficient1D<T>\n";
     PrintLine(_os, MYSIZE);
     _os    << std::setw(5)     << "i" 
            << std::setw(20)    << "xCentro"
            << std::setw(20)    << "Gamma"
            << "\n";
     PrintLine(_os, MYSIZE);

    const std::size_t N = _coeff1D.NVol();

    std::vector<std::size_t> indices(N);
    std::iota(indices.begin(), indices.end(), 0);

auto xCentro = _coeff1D.Grid().GetCentre();
auto Print = [&](const std::size_t& i)
{
    std::stringstream ss;
    ss << std::setw(5) << i
       << std::scientific 
       << std::setw(20) << xCentro[i]
       << std::setw(20) << _coeff1D.coeff_[i];
    return ss.str(); 
};

    std::transform  (   std::begin(indices)
                    ,   std::end(indices)
                    ,   std::ostream_iterator<std::string>(_os, "\n")
                    ,   Print
                    );


    _os << std::flush;
    PrintLine(_os, MYSIZE);
    _os.flags(fflags);

    return _os;
};


FVMAKER_NAMESPACE_CLOSE
