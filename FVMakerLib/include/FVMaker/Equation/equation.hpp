// ============================================================================
// Arquivo: equation.hpp
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Declara equation no contexto de Equation.
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


//==============================================================================
// Incluições do FVMAKER
//==============================================================================

//==============================================================================
// Incluições do FVMAKER
//==============================================================================

#include <FVMaker/Grid/abstractGrid.h>     // Definições da Grid; 

FVMAKER_NAMESPACE_OPEN

// Escreve uma representacao textual do objeto no fluxo de saida.
template<typename T>
std::ostream& operator<< (std::ostream& _os, const Equation<T>& _equation) {

std::ios_base::fmtflags fflags = _os.flags();
const int MYSIZE= LSIZE + 5;    

     _os << "\nImpressao do Equation\n";
     PrintLine(_os, MYSIZE);
     _os    << std::setw(5)     << "i" 
            << std::setw(12)    << "AP"
            << std::setw(12)    << "AW"
            << std::setw(12)    << "AE"
            << "\n";
     PrintLine(_os, MYSIZE);

    const std::size_t N = _equation.NVol();

    std::vector<std::size_t> indices(N);
    std::iota(indices.begin(), indices.end(), 0);

auto Print = [&](const std::size_t& i) {
    std::stringstream ss;
    ss << std::setw(5) << i
       << std::scientific
       << std::setprecision(4)
       << std::setw(12) << _equation.aP_[i]
       << std::setw(12) << _equation.aW_[i]
       << std::setw(12) << _equation.aE_[i];
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
}

FVMAKER_NAMESPACE_CLOSE
