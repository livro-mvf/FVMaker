//==============================================================================
// Includes da biblioteca FVMaker
//==============================================================================
#include <FVMaker/Error/FVMakerException.h>
#include <FVMaker/Misc/Misc.h>

//==============================================================================
// Includes da biblioteca padrão do C++
//==============================================================================
#include <format>


FVMAKER_NAMESPACE_OPEN
        
// Definição do catálogo de erros usando unordered_map
const std::unordered_map<ErrorCode, std::string_view> errorCatalog{
    { ErrorCode::GridGenerationError,           "Erro nao definido na geracao da malha." },
    { ErrorCode::InvalidGridDimension,          "Dimensao da Grid nao definida." },            
    { ErrorCode::InvalidLength,                 "Comprimento do dominio invalido." },
    { ErrorCode::InvalidNumbersOfVolumes,       "Numero de volumes da malha invalido." },
    { ErrorCode::MemoryAllocationError,         "Erro ao alocar memória (std::bad_alloc)." },
    { ErrorCode::UndefiniedCoefficient,         "Coeficiente nao definido." },
    { ErrorCode::UndefiniedFunction,            "Funcao nao definida." },
    { ErrorCode::UndefiniedGrid,                "Malha nao definida." },
    { ErrorCode::UnhandledException,            "Excecao nao tratada." },            
    { ErrorCode::UnorderedCenterCoordinates,    "As coordenadas dos centros não estão ordenadas." },
    { ErrorCode::UnorderedFaceCoordinates,      "As coordenadas das faces não estão ordenadas." },
    { ErrorCode::VectorResizeError,             "Erro ao redimensionar vetor (std::length_error)." }
};

std::string_view getErrorMessage(ErrorCode code) {
    auto it = errorCatalog.find(code);
    if (it != errorCatalog.end())
        return it->second;
    return "Erro desconhecido.";
}

FVMakerException::FVMakerException  (   const ErrorCode&                code
                                    ,   const std::source_location&     loc
                                    )
                                    : code(code)
{
    // Monta a mensagem completa com informações do local
    message = std::format("{}\nArquivo: {}\nLinha: {}\nFunção: {}",
                          getErrorMessage(code),
                          loc.file_name(),
                          loc.line(),
                          loc.function_name());
}

const char* FVMakerException::what() const noexcept {
    return message.c_str();
}

ErrorCode FVMakerException::getCode() const noexcept {
    return code;
}

FVMAKER_NAMESPACE_CLOSE