#pragma once

//==============================================================================
// Name        : FVMakerException.h
// Author      : João Flávio Vieira de Vasconcellos
// Version     : 1.1
// Description : Definição da classe FVMakerException, para lidar com 
//               as exceptions da biblioteca FVMaker.
//
// Copyright   : Copyright (C) 2025 João Flávio Vasconcellos
//               (jflavio at iprj.uerj.br)
//
// License     : Este programa é software livre: você pode redistribuí-lo e/ou
//               modificá-lo sob os termos da Licença Pública Geral GNU, conforme
//               publicada pela Free Software Foundation, versão 3 da licença,
//               ou (a seu critério) qualquer versão posterior.
//
//               Este programa é distribuído na esperança de que seja útil,
//               mas SEM QUALQUER GARANTIA; sem mesmo a garantia implícita de
//               COMERCIABILIDADE ou ADEQUAÇÃO A UM DETERMINADO PROPÓSITO. Consulte
//               a Licença Pública Geral GNU para mais detalhes.
//
//               Você deve ter recebido uma cópia da Licença Pública Geral GNU
//               junto com este programa. Caso contrário, veja <https://www.gnu.org/licenses/>.
//==============================================================================

//==============================================================================
// Includes da biblioteca padrão do C++
//==============================================================================

#include <exception>            // std::exception 
#include <string>               // std::string
#include <unordered_map>        // std::hash
#include <string_view>          // std::string_view
#include <source_location>      // std::source_location


//==============================================================================
// Includes da biblioteca FVMaker
//==============================================================================
#include <FVMaker/Misc/namespace.h>





// Enum com os códigos de erro
enum class ErrorCode {
    GridGenerationError,
    InvalidLength,
    InvalidNumbersOfVolumes,
    MemoryAllocationError,
    UnhandledException,
    VectorResizeError
};

// Especialização de std::hash para ErrorCode, permitindo seu uso como chave no unordered_map.
namespace std {
    template <>
    struct hash<ErrorCode> {
        std::size_t operator()(const ErrorCode& code) const noexcept {
            return std::hash<int>()(static_cast<int>(code));
        }
    };
}


FVMAKER_NAMESPACE_OPEN


// Declaração do catálogo de erros
extern const std::unordered_map<ErrorCode, std::string_view> errorCatalog;

// Função para obter a mensagem associada a um código de erro
std::string_view getErrorMessage(ErrorCode code);

// Classe de exceção que integra o catálogo e as informações de localização
class FVMakerException : public std::exception {
    
public:
    
    FVMakerException    (   const ErrorCode& code
                        ,   const std::source_location& loc = std::source_location::current()
                        );

    const char* what() const noexcept override;
    ErrorCode getCode() const noexcept;

private:
    
    ErrorCode       code;
    std::string     message;
    
};


        
FVMAKER_NAMESPACE_CLOSE
