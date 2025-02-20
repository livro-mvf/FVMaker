//==============================================================================
// Name        : Misc.h
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.0
// Description : Arquivo com definições que serao utilizadas
//               pela biblioteca
//
// Copyright   : Copyright (C) <2025>  Joao Flavio Vasconcellos 
//                                      (jflavio at iprj.uerj.br)
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
//==============================================================================

#pragma once

//==============================================================================
// Includes da GridVoronoi++
//==============================================================================

//#include <FVMaker/Misc/type.h>
#include <FVMaker/Misc/Configure.h>

FVMAKER_NAMESPACE_OPEN

//============================================================================
// include das funcoes que nao sao de classe alguma
//============================================================================

void            PrintLine(std::ostream&, const unsigned& = LSIZE);

[[nodiscard]]
bool            string2bool (std::string);

[[nodiscard]]
std::string getCurrentWorkingDirectory();

[[nodiscard]]
std::string getFullPath(const std::string&);

[[nodiscard]]
std::string RenameFile(const std::string&, const std::string&);

//============================================================================
// Funcoes importantes
//============================================================================

//! Funcao utilizada para converter string em inteiros.

constexpr unsigned int str2int(const char* _str, const int& _h = 0)
{
    return !_str[_h] ? 5381 : (str2int(_str, _h+1) * 33) ^ _str[_h];
}

FVMAKER_NAMESPACE_CLOSE
