// ============================================================================
// Arquivo: misc.cpp
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Implementa misc no contexto de Misc.
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

#include <algorithm>        ///< Para std::generate_n, std::transform
#include <iostream>         ///< Para std::iostream
#include <iterator>         ///< Para std::ostream_iterator
#include <climits>          ///< Para PATH_MAX
#include <sstream>          ///< Para std::stringstream
#include <unistd.h>         ///< Para getcwd()

//==============================================================================
// Includes da FVMaker
//==============================================================================
#include <FVMaker/Misc/misc.h>

FVMAKER_NAMESPACE_OPEN

//==============================================================================
// Implementacao das funcoes
//==============================================================================

/**
 * @brief Imprime uma linha de caracteres no stream de saida
 * @param _os Stream de saida onde a linha sera impressa
 * @param _lsize Comprimento da linha a ser impressa
 * @details Usa o caractere '=' (ASCII 0x3D) para formar a linha
 */
void PrintLine(std::ostream& _os, const unsigned& _lsize) 
{
    const char ASCIINUMBER(0x3D);
    auto PrintAscII = []() { return ASCIINUMBER; };

    std::generate_n(std::ostream_iterator<char>(_os, ""), _lsize, PrintAscII);
    _os << "\n";
}

/**
 * @brief Converte uma string em valor booleano
 * @param str String a ser convertida
 * @return Valor booleano correspondente
 * @details Aceita "true"/"false", "1"/"0" (case-insensitive)
 * @throws Nenhuma excecao explicita
 */
bool string2bool(std::string str) 
{
    std::transform(std::begin(str), std::end(str), std::begin(str), ::tolower);
    std::istringstream is(str);
    bool b;
    is >> std::boolalpha >> b;
    return b;
}

/**
 * @brief Obtem o diretorio de trabalho atual
 * @return String com o caminho completo do diretorio atual
 * @throws Nenhuma excecao explicita, mas retorna string vazia em caso de erro
 */
std::string getCurrentWorkingDirectory() 
{
    char temp[PATH_MAX];
    if (getcwd(temp, PATH_MAX) != nullptr) {
        return std::string(temp);
    } else {
        perror("getcwd() error");
        return std::string();
    }
}

/**
 * @brief Retorna o caminho absoluto de um arquivo
 * @param filename Nome do arquivo
 * @return Caminho absoluto completo
 * @throws Nenhuma excecao explicita
 */
std::string getFullPath(const std::string& filename) 
{
    std::string cwd = getCurrentWorkingDirectory();
    if (!cwd.empty()) {
        return cwd + "/" + filename;
    }
    return filename;
}

/**
 * @brief Renomeia a extensao de um arquivo
 * @param _filename Nome original do arquivo
 * @param _newFiletype Nova extensao desejada
 * @return Nome do arquivo com a nova extensao
 * @details Mantem o nome base e substitui apenas a extensao
 * @throws Nenhuma excecao explicita
 */
std::string RenameFile(const std::string& _filename, const std::string& _newFiletype) 
{
    size_t lastDot = _filename.find_last_of(".");
    
    if (lastDot != std::string::npos) {
        std::string filetype = _filename.substr(lastDot + 1);
        if (filetype != _newFiletype) {
            return _filename.substr(0, lastDot) + "." + _newFiletype;
        }
        return _filename;
    }
    return _filename + "." + _newFiletype;
}

FVMAKER_NAMESPACE_CLOSE