//==============================================================================
// Nome        : GridAnalyser.h
// Autor       : João Flávio Vieira de Vasconcellos
// Versão      : 1.0
// Descrição   : Classe base para análise de malhas estruturadas na biblioteca
//               FVMaker. Permite extensão por meio de herança e exige que todas
//               as classes derivadas implementem análise e saída.
// Licença     : GNU GPL v3
//==============================================================================

#pragma once

// ==============================================================================
// INCLUDES - C++
// ==============================================================================
#include <iostream>

// ==============================================================================
// INCLUDES - FVMaker
// ==============================================================================
#include <FVMaker/Misc/Namespace.h>

GRID_NAMESPACE_OPEN

/**
 * @brief Classe base abstrata para análises de malhas.
 * 
 * Esta classe define a interface para todas as classes de análise de malhas.
 * Toda classe derivada que produz resultados deverá implementar o método
 * `analyse()` e o método `print()`, que permite a saída formatada com `operator<<`.
 * 
 * @tparam GridType Tipo da malha a ser analisada (por exemplo: UniformGrid1D).
 */
template <typename GridType>
class GridAnalyser {
public:
    /**
     * @brief Construtor que recebe uma referência constante à malha.
     * 
     * @param grid Referência à malha a ser analisada.
     */
    explicit GridAnalyser(const GridType& grid) : grid_(grid) {}

    /**
     * @brief Método abstrato que executa a análise.
     * 
     * Cada classe derivada deve implementar sua lógica específica de análise.
     */
    virtual void analyse() = 0;

    /**
     * @brief Método abstrato para impressão formatada.
     * 
     * Este método é chamado pelo operador `<<` para apresentar os resultados.
     * 
     * @param os Stream de saída (como std::cout).
     */
    virtual void print(std::ostream& os) const = 0;

    /**
     * @brief Destrutor virtual padrão.
     */
    virtual ~GridAnalyser() = default;

    /**
     * @brief Sobrecarga do operador `<<` para qualquer classe derivada.
     * 
     * Permite usar `std::cout << object` diretamente para imprimir o resultado.
     * 
     * @param os Stream de saída.
     * @param analyser Referência ao analisador a ser impresso.
     * @return std::ostream& Stream modificado.
     */
    friend std::ostream& operator<<(std::ostream& os, const GridAnalyser& analyser) {
        analyser.print(os);
        return os;
    }

protected:
    const GridType& grid_;  ///< Referência constante à malha analisada
};

GRID_NAMESPACE_CLOSE
