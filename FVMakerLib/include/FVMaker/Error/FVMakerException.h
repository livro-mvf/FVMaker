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

#pragma once

/**
 * @file FVMakerException.h
 * @brief Definicao da classe de excecoes personalizadas para a biblioteca FVMaker.
 * @author Joao Flavio Vieira de Vasconcellos
 * @version 1.3
 * @date 2025-05-20
 * 
 * @copyright Copyright (C) 2025 Joao Flavio Vasconcellos
 * @license GNU GPLv3
 */

//==============================================================================
// Includes da biblioteca padrão do C++
//==============================================================================
#include <exception>
#include <string>
#include <unordered_map>
#include <string_view>
#include <source_location>

//==============================================================================
// Includes da biblioteca FVMaker
//==============================================================================
#include <FVMaker/Misc/namespace.h>

/**
 * @enum ErrorCode
 * @brief Codigos de erro utilizados pelas excecoes da biblioteca FVMaker.
 * 
 * A lista completa de códigos de erro e suas mensagens está definida no arquivo 
 * FVMakerException.cpp. Novos códigos devem ser adicionados tanto no enum quanto 
 * no catálogo de erros.
 */
enum class ErrorCode {
    GridGenerationError,          ///< Erro generico na geracao da malha
    InvalidGridDimension,         ///< Dimensao da grid invalida ou nao definida
    InvalidBeta,                  ///< Valor de beta invalido (<= 1.0)
    InvalidLength,                ///< Comprimento do dominio invalido
    InvalidNumbersOfVolumes,      ///< Numero de volumes da malha invalido
    InvalidOption,                ///< Opcao invalida ou nao liberada
    MemoryAllocationError,        ///< Falha na alocacao de memoria
    UndefiniedCoefficient,        ///< Coeficiente nao definido
    UndefiniedFunction,           ///< Funcao nao definida
    UndefiniedGrid,               ///< Malha nao definida
    UnhandledException,           ///< Excecao nao tratada
    UnorderedCenterCoordinates,   ///< Coordenadas dos centros nao ordenadas
    UnorderedFaceCoordinates,     ///< Coordenadas das faces nao ordenadas
    VectorResizeError,            ///< Erro no redimensionamento de vetor
    // Adicione novos códigos aqui
    ExampleNewError1,             ///< Exemplo de novo codigo de erro 1
    ExampleNewError2              ///< Exemplo de novo codigo de erro 2
};

// Especialização de std::hash para ErrorCode
namespace std {
    template <>
    struct hash<ErrorCode> {
        std::size_t operator()(const ErrorCode& code) const noexcept {
            return static_cast<std::size_t>(code);
        }
    };
}

FVMAKER_NAMESPACE_OPEN

/**
 * @brief Catalogo de mensagens de erro usando unordered_map.
 * 
 * Mapeia cada ErrorCode para sua respectiva mensagem descritiva.
 * A inicializacao é feita no arquivo FVMakerException.cpp.
 */
extern const std::unordered_map<ErrorCode, std::string_view> errorCatalog;

/**
 * @brief Obtem a mensagem de erro associada a um codigo.
 * 
 * @param code Codigo de erro a ser pesquisado.
 * @return std::string_view Mensagem de erro correspondente ou "Erro desconhecido" se nao encontrado.
 * 
 * @note Complexidade: O(1) em média, O(n) no pior caso (improvável com bons hashes)
 */
std::string_view getErrorMessage(ErrorCode code) noexcept;

/**
 * @class FVMakerException
 * @brief Classe base para todas as excecoes lancadas pela biblioteca FVMaker.
 */
class FVMakerException : public std::exception {
public:
    /**
     * @brief Construtor da excecao.
     * 
     * @param code Codigo de erro associado a excecao.
     * @param loc Localizacao no codigo fonte (preenchido automaticamente).
     */
    FVMakerException(ErrorCode code, 
                    const std::source_location& loc = std::source_location::current()) noexcept;
    
    /**
     * @brief Retorna a mensagem de erro formatada.
     * 
     * @return const char* Mensagem de erro no formato C.
     */
    const char* what() const noexcept override;
    
    /**
     * @brief Obtem o codigo de erro associado a excecao.
     * 
     * @return ErrorCode Codigo de erro que originou a excecao.
     */
    ErrorCode getCode() const noexcept;

private:
    ErrorCode code_;             ///< Codigo de erro armazenado
    std::string message_;        ///< Mensagem de erro formatada
};

FVMAKER_NAMESPACE_CLOSE