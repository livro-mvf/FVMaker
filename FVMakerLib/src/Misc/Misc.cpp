//==============================================================================
//  Includes c++
//==============================================================================

#include <algorithm>        // std::generate_n
#include <iostream>         // std::iostream
#include <iterator>         // std::ostream_iterator
#include <climits>
#include <sstream>          // std::stringstream
#include <unistd.h>

//==============================================================================
//  Includes da GridVoronoi++
//==============================================================================

#include <FVMAKER/Misc/Misc.h>



FVMAKER_NAMESPACE_OPEN


void 
PrintLine   (   std::ostream&           _os
            ,   const unsigned&         _lsize
            ) 
{

const char ASCIINUMBER(0x3D);

auto PrintAscII = [] () {return ASCIINUMBER;};


    std::generate_n     (   std::ostream_iterator<char> ( _os, "" )
                        ,   _lsize
                        ,   PrintAscII
                        );

    _os << "\n";

}



bool string2bool (std::string str) {
    
    
    std::transform  (   std::begin(str)
                    ,   std::end(str)
                    ,   std::begin(str)
                    ,   ::tolower
                    );
    
    std::istringstream is(str);
    bool b;
    is >> std::boolalpha >> b;
    return b;
}



// Função para obter o diretório de trabalho atual
std::string getCurrentWorkingDirectory() {
    char temp[PATH_MAX];
    if (getcwd(temp, PATH_MAX) != nullptr) {
        return std::string(temp);
    } else {
        perror("getcwd() error");
        return std::string();
    }
}

// Função para obter o caminho completo do arquivo
std::string getFullPath (   const std::string& filename) {
    std::string cwd = getCurrentWorkingDirectory();
    if (!cwd.empty()) {
        return cwd + "/" + filename;
    } else {
        return filename;
    }
}

std::string RenameFile  (   const std::string& _filename
                        ,   const std::string& _newFiletype
                        ) {
   // Encontra a posição do último ponto no nome do arquivo
    size_t lastDot = _filename.find_last_of(".");
    
    if (lastDot != std::string::npos) {
        // Extrai o tipo de arquivo atual
        std::string filetype = _filename.substr(lastDot + 1);
        
        // Verifica se o tipo de arquivo atual é diferente do novo tipo
        if (filetype != _newFiletype) {
            // Constrói o novo nome do arquivo
            std::string newFilename = _filename.substr(0, lastDot) + "." + _newFiletype;
            return newFilename;
        } else {
            // O arquivo já possui o tipo especificado, então retorna o nome original
            return _filename;
        }
    } else {
        // Se o nome do arquivo não contém uma extensão, simplesmente adicionamos o novo tipo de arquivo
        std::string newFilename = _filename + "." + _newFiletype;
        return newFilename;
    }
}



FVMAKER_NAMESPACE_CLOSE
