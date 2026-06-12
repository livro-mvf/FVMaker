//==============================================================================
// Nome        : FVMaker.h
// Autor       : João Flávio Vieira de Vasconcellos
// Versão      : 1.1
// Descrição   : Declaração da classe FVMaker para organizar simulações de
//               volumes finitos.
//
// Direitos autorais : Copyright (C) 2024 João Flávio Vasconcellos
//                     (jflavio at iprj.uerj.br)
//
// Licença     : Este programa é software livre: você pode redistribuí-lo e/ou
//               modificá-lo sob os termos da Licença Pública Geral GNU como
//               publicada pela Free Software Foundation, tanto a versão 3
//               da licença, como (a seu critério) qualquer versão posterior.
//
//               Este programa é distribuído na esperança de que seja útil,
//               mas SEM NENHUMA GARANTIA; sem mesmo a garantia implícita de
//               COMERCIABILIDADE ou ADEQUAÇÃO A QUALQUER PROPÓSITO
//               EM PARTICULAR.
//               Consulte a Licença Pública Geral GNU para mais detalhes.
//
//               Você deve ter recebido uma cópia da Licença Pública Geral GNU
//               junto com este programa.
//               Se não, veja <https://www.gnu.org/licenses/>.
//==============================================================================

/**
 * @file FVMaker.h
 * @brief Cabeçalho para a classe principal da biblioteca FVMaker.
 *
 * Este arquivo define a classe FVMaker que é usada para gerenciar a criação
 * e manipulação de volumes finitos em malhas estruturadas.
 *
 * @version 1.1
 * @date 2024-11-30
 *
 * @author João
 *
 * @copyright Copyright (c) 2024
 * @license GNU GPL v3
 */

#ifndef FVMAKER_H
#define FVMAKER_H

//==============================================================================
// Includes da Biblioteca Padrão do C++
//==============================================================================

//==============================================================================
// Includes da Biblioteca FVMaker
//==============================================================================

#include <FVMaker/Misc/type.h>
#include <FVMaker/Algebra/DenseVector.h>
#include <FVMaker/Algebra/ErrorNorms.h>
#include <FVMaker/Core/Version.h>
#include <FVMaker/ErrorHandling/ErrorCatalog.h>
#include <FVMaker/ErrorHandling/ErrorCodes.h>
#include <FVMaker/ErrorHandling/ErrorRecord.h>
#include <FVMaker/ErrorHandling/FVMException.h>
#include <FVMaker/ErrorHandling/ThrowError.h>
#include <FVMaker/Functions/FunctionEvaluation1D.h>
#include <FVMaker/Functions/FunctionTypes.h>
#include <FVMaker/Functions/ManufacturedProblem1D.h>
#include <FVMaker/Functions/ManufacturedSolution1D.h>
#include <FVMaker/Functions/VariableCoefficient1D.h>
#include <FVMaker/OneDimensional/Assembly/Assembler1D.h>
#include <FVMaker/OneDimensional/Boundary/BoundaryCondition1D.h>
#include <FVMaker/OneDimensional/Boundary/GhostCells1D.h>
#include <FVMaker/OneDimensional/Boundary/GhostBoundary1D.h>
#include <FVMaker/OneDimensional/Coefficient/DiffusionCoefficient1D.h>
#include <FVMaker/OneDimensional/Field/Field1D.h>
#include <FVMaker/OneDimensional/Field/FieldView1D.h>
#include <FVMaker/OneDimensional/Equation/Equation1D.h>
#include <FVMaker/OneDimensional/Grid/GridMetrics1D.h>
#include <FVMaker/OneDimensional/Grid/GridView1D.h>
#include <FVMaker/OneDimensional/Solver/ConjugateGradient.h>
#include <FVMaker/OneDimensional/Solver/GaussSeidel.h>
#include <FVMaker/OneDimensional/Solver/Jacobi.h>
#include <FVMaker/OneDimensional/Operator/Laplacian1D.h>
#include <FVMaker/OneDimensional/Solver/TDMA.h>
#include <FVMaker/OneDimensional/Source/LinearizedSource1D.h>
#include <FVMaker/OneDimensional/System/AlgebraicResidual1D.h>
#include <FVMaker/OneDimensional/System/PentadiagonalSystem1D.h>
#include <FVMaker/OneDimensional/System/TridiagonalSystem1D.h>
#include <FVMaker/Solver/SolveResult.h>
#include <FVMaker/Solver/IterativeSolverOptions.h>

FVMAKER_NAMESPACE_OPEN

[[nodiscard]] const char* version() noexcept;

/**
 * @class FVMaker
 * @brief Classe para gerenciar a criação de volumes finitos em malhas
 * estruturadas.
 *
 * A classe FVMaker é utilizada para criar e manipular volumes finitos, que
 * são usados em simulações numéricas baseadas no método dos volumes finitos
 * (FVM).
 */
class FVMaker {

//==============================================================================
// Construtores/Destrutor
//==============================================================================

public:
    /**
     * @brief Construtor da classe FVMaker.
     *
     * Este construtor inicializa a classe FVMaker para começar a configurar
     * volumes finitos em uma malha.
     */
    FVMaker() noexcept = default;

    /**
     * @brief Construtor de cópia da classe FVMaker.
     *
     * Cria uma nova instância da classe FVMaker copiando outra instância.
     *
     * @param other Objeto FVMaker a ser copiado.
     */
    FVMaker(const FVMaker& other) noexcept = default;

    /**
     * @brief Construtor de movimento da classe FVMaker.
     *
     * Cria uma nova instância da classe FVMaker movendo outra instância.
     *
     * @param other Objeto FVMaker a ser movido.
     */
    FVMaker(FVMaker&& other) noexcept = default;

    /**
     * @brief Destrutor da classe FVMaker.
     *
     * Destroi a instância da classe FVMaker, liberando quaisquer recursos
     * utilizados.
     */
    ~FVMaker() noexcept = default;

//==============================================================================
// Sobrecarga de Operadores
//==============================================================================

public:
    /**
     * @brief Sobrecarga do operador de atribuição.
     *
     * Realiza a atribuição de um objeto FVMaker para outro.
     *
     * @param other Objeto FVMaker a ser copiado.
     * @return Referência para o objeto atual após a atribuição.
     */
    FVMaker& operator=(const FVMaker& other) noexcept = default;

    /**
     * @brief Sobrecarga do operador de atribuição por movimento.
     *
     * Realiza a atribuição de um objeto FVMaker para outro utilizando
     * movimento.
     *
     * @param other Objeto FVMaker a ser movido.
     * @return Referência para o objeto atual após a atribuição.
     */
    FVMaker& operator=(FVMaker&& other) noexcept = default;
};

FVMAKER_NAMESPACE_CLOSE

#endif // FVMAKER_H
