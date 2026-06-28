// ============================================================================
// Arquivo: parallelControl.h
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Declara parallel Control no contexto de Misc.
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

/**
 * @file ParallelControl.h
 * @brief Controle de politicas de execucao paralela e thresholds automaticos
 * @defgroup Paralelismo Controle de Execucao Paralela
 * @ingroup Misc
 *
 * Gerencia politicas de execucao (sequencial, paralela, vetorizada) e thresholds
 * automaticos baseados em hardware para operacoes na biblioteca FVMaker.
 *
 * @author Joao Flavio Vasconcellos
 * @version 2.0
 * @date 2025-05-20
 * @copyright Copyright (c) 2026, João Flávio Vieira de Vasconcellos
 */

//==============================================================================
// Includes do C++
//==============================================================================
#include <algorithm>
#include <execution>
#include <thread>
#include <utility>

//==============================================================================
// Includes da FVMaker
//==============================================================================
#include <FVMaker/Misc/configure.h>

FVMAKER_NAMESPACE_OPEN

/**
 * @class ParallelControl
 * @brief Controle centralizado de execucao paralela
 * @details Gerencia politicas de execucao e thresholds automaticos para operacoes
 *          em colecoes, com valores padrao otimizados "de fabrica".
 * 
 * @ingroup Paralelismo
 */
class ParallelControl {
public:
    /**
     * @enum ExecutionPolicy
     * @brief Politicas de execucao disponiveis
     */
    enum class ExecutionPolicy {
        SEQUENTIAL,     ///< Execucao sequencial tradicional
        PARALLEL,       ///< Execucao paralela com threads
        PARALLEL_UNSEQ, ///< Execucao paralela com vetorizacao
        GPU             ///< Reservado para implementacao futura
    };

    /**
     * @enum ThresholdPolicy
     * @brief Modos de determinacao do threshold
     */
    enum class ThresholdPolicy {
        FIXED,          ///< Usa valor definido (padrao de fabrica ou customizado)
        AUTO            ///< Calcula automaticamente baseado no hardware
    };

    //==========================================================================
    // Interface Publica
    //==========================================================================

    /**
     * @brief Aplica uma funcao a um range de elementos
     * @tparam InputIt Tipo do iterador de entrada
     * @tparam UnaryFunction Tipo da funcao unaria
     * @param first Inicio do range
     * @param last Fim do range
     * @param f Funcao a ser aplicada
     */
    template<typename InputIt, typename UnaryFunction>
    static void for_each(InputIt first, InputIt last, UnaryFunction f) {
        #ifdef ENABLE_PARALLEL_EXECUTION
        if (current_exec_policy() == ExecutionPolicy::PARALLEL) {
            std::for_each(std::execution::par, first, last, f);
            return;
        }
        else if (current_exec_policy() == ExecutionPolicy::PARALLEL_UNSEQ) {
            std::for_each(std::execution::par_unseq, first, last, f);
            return;
        }
        #endif
        std::for_each(first, last, f);
    }

    /**
     * @brief Define a politica de execucao atual
     * @param policy Nova politica a ser utilizada
     */
    static void set_execution_policy(ExecutionPolicy policy) noexcept {
        current_exec_policy() = policy;
    }

    /**
     * @brief Define a politica de threshold atual
     * @param policy Nova politica (FIXED ou AUTO)
     * @param fixed_value Valor customizado para FIXED (opcional)
     */
    static void set_threshold_policy(ThresholdPolicy policy, 
                                   size_t fixed_value = 0) noexcept {
        if (policy == ThresholdPolicy::FIXED && fixed_value > 0) {
            current_threshold() = fixed_value;
        }
        current_threshold_policy() = policy;
    }

    /**
     * @brief Retorna o threshold efetivo atual
     * @return Threshold para paralelismo automatico
     */
    static size_t parallel_threshold() noexcept {
        if (current_threshold_policy() == ThresholdPolicy::AUTO) {
            return calculate_auto_threshold();
        }
        return current_threshold();
    }

    /**
     * @brief Valor padrao de fabrica para o threshold
     * @return 10000 (valor calibrado para maioria dos casos)
     */
    static constexpr size_t factory_threshold() noexcept {
        return 10000;
    }

private:
    //==========================================================================
    // Implementacao Interna
    //==========================================================================

    // Variaveis de estado (thread-safe)
    static ExecutionPolicy& current_exec_policy() noexcept {
        static ExecutionPolicy policy = ExecutionPolicy::PARALLEL;
        return policy;
    }

    static ThresholdPolicy& current_threshold_policy() noexcept {
        static ThresholdPolicy policy = ThresholdPolicy::FIXED;
        return policy;
    }

    static size_t& current_threshold() noexcept {
        static size_t threshold = factory_threshold();
        return threshold;
    }

    // Calculo automatico do threshold
    static size_t calculate_auto_threshold() noexcept {
        static const size_t cached_value = [](){
            const size_t cores = std::max(1u, std::thread::hardware_concurrency());
            const size_t cache_per_core = 32768; // 32KB cache L1 tipico
            const size_t element_size = sizeof(Real);
            return std::max(factory_threshold(), 
                          (cores * cache_per_core) / element_size);
        }();
        return cached_value;
    }
};

FVMAKER_NAMESPACE_CLOSE