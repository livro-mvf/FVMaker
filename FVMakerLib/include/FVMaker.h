// ============================================================================
// Arquivo: FVMaker.h
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Declara a interface agregadora principal do FVMaker.
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
#include <FVMaker/OneDimensional/Advection/Advection1D.h>
#include <FVMaker/OneDimensional/Advection/FaceFlux1D.h>
#include <FVMaker/OneDimensional/Advection/Interpolation1D.h>
#include <FVMaker/OneDimensional/Boundary/BoundaryCondition1D.h>
#include <FVMaker/OneDimensional/Boundary/GhostCells1D.h>
#include <FVMaker/OneDimensional/Boundary/GhostBoundary1D.h>
#include <FVMaker/OneDimensional/Coefficient/DiffusionCoefficient1D.h>
#include <FVMaker/OneDimensional/Coefficient/NonlinearCoefficient1D.h>
#include <FVMaker/OneDimensional/Field/Field1D.h>
#include <FVMaker/OneDimensional/Field/FieldView1D.h>
#include <FVMaker/OneDimensional/Equation/Equation1D.h>
#include <FVMaker/OneDimensional/Grid/GridMetrics1D.h>
#include <FVMaker/OneDimensional/Grid/GridView1D.h>
#include <FVMaker/OneDimensional/Solver/BiCG.h>
#include <FVMaker/OneDimensional/Solver/BiCGSTAB.h>
#include <FVMaker/OneDimensional/Solver/ConjugateGradient.h>
#include <FVMaker/OneDimensional/Solver/GaussSeidel.h>
#include <FVMaker/OneDimensional/Solver/Jacobi.h>
#include <FVMaker/OneDimensional/Solver/LinearSolverOptions1D.h>
#include <FVMaker/OneDimensional/Solver/NonlinearSolve1D.h>
#include <FVMaker/OneDimensional/Solver/SolveController1D.h>
#include <FVMaker/OneDimensional/Operator/DiffusionOperator1D.h>
#include <FVMaker/OneDimensional/Operator/Laplacian1D.h>
#include <FVMaker/OneDimensional/Solver/TDMA.h>
#include <FVMaker/OneDimensional/Source/LinearizedSource1D.h>
#include <FVMaker/OneDimensional/System/AlgebraicResidual1D.h>
#include <FVMaker/OneDimensional/System/PentadiagonalSystem1D.h>
#include <FVMaker/OneDimensional/System/TridiagonalSystem1D.h>
#include <FVMaker/OneDimensional/Term/Terms1D.h>
#include <FVMaker/OneDimensional/Variable/Variable1D.h>
#include <FVMaker/Output/VTK/Variable1DVTKWriter.h>
#include <FVMaker/Roadmap/FakeBlocks.h>
#include <FVMaker/Solver/SolveResult.h>
#include <FVMaker/Solver/IterativeSolverOptions.h>
#include <FVMaker/Solver/PicardIteration.h>
#include <FVMaker/Solver/SteadyState.h>
#include <FVMaker/Solver/StopCriteria.h>

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
