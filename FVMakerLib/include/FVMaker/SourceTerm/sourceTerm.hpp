// ============================================================================
// Arquivo: sourceTerm.hpp
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Declara source Term no contexto de SourceTerm.
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
#include <algorithm>                // std::transform
#include <iomanip>                  // std::setw
#include <iostream>                 // std::cerr 
#include <iterator>               // std::ostream_iterator

//==============================================================================
// Includes da biblioteca FVMaker
//==============================================================================
#include <FVMaker/Error/FVMakerException.h>
#include <FVMaker/Misc/misc.h>

FVMAKER_NAMESPACE_OPEN


template<typename T>
std::ostream& operator<<    (  std::ostream& _os
                            ,   const SourceTerm<T>& _source
                            ) 
{
std::ios_base::fmtflags fflags = _os.flags();
const int MYSIZE= LSIZE - 15;    
     _os << "\nImpressao do SourceTerm\n";
     PrintLine(_os, MYSIZE);
     _os    << std::setw(5)     << "i" 
            << std::setw(20)    << "xCentro"
            << std::setw(20)    << "SP" 
            << std::setw(20)    << "SC"
            << "\n";
     PrintLine(_os, MYSIZE);

    const std::size_t N = _source.NVol();

    std::vector<std::size_t> indices(N);
    std::iota(indices.begin(), indices.end(), 0);

auto xCentro = _source.Grid().GetCentre();

auto Print = [&](const std::size_t& i)
{
    std::stringstream ss;
    ss << std::setw(5) << i
       << std::scientific 
       << std::setw(20) << xCentro[i]
       << std::setw(20) << _source.vecSource_[i].first
       << std::setw(20) << _source.vecSource_[i].second;
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

template<typename T>
bool SourceTerm<T> :: EvaluateSourceFunction () {

    try {
        if (this->sourceFunction_ == nullptr) {
            throw fvm::FVMakerException(ErrorCode::UndefiniedFunction);
        }

    } catch (const fvm::FVMakerException& e) {
        std::cerr << "\n\n";
        PrintLine(std::cerr);
        std::cerr << "Exceção capturada: " << e.what() << "\n";
        PrintLine(std::cerr);
        std::cerr << "\n\n";
        exit(EXIT_FAILURE);
    }

    const std::size_t N = this->grid_.NVol();
    std::vector<std::size_t> indices(N);
    std::iota(indices.begin(), indices.end(), 0);
    
const auto& xc    = this->grid_.GetCentre();  // vetor de centros

auto CalculaFuncao = [&] (const size_t& i) {
    return this->sourceFunction_->Fx(xc[i]);
};

    std::transform  (   std::begin(indices)
                    ,   std::end(indices)
                    ,   std::begin(this->vecSource_)
                    ,   CalculaFuncao
                    );
    return true;
}


FVMAKER_NAMESPACE_CLOSE