#ifndef __FVMAKER_TYPE_H__
#define __FVMAKER_TYPE_H__

//==============================================================================
// Nome        : type.h
// Autor       : João Flávio Vieira de Vasconcellos
// Versão      : 1.0
// Descrição   : Arquivo de cabeçalho contendo definições de tipos comuns usados 
//               na biblioteca FVMaker. Define tipos geométricos e numéricos 
//               utilizados em cálculos de geometria computacional e cálculos 
//               numéricos, incluindo ponteiros inteligentes para gerenciamento 
//               de memória.
//==============================================================================

/**
 * @file type.h
 * @brief Define tipos comuns geométricos e numéricos usados na biblioteca FVMaker.
 * 
 * Este arquivo define tipos chave usados em geometria computacional e cálculos 
 * numéricos. Tipos como pontos, segmentos, vetores e polígonos em 2D e 3D são 
 * definidos aqui, juntamente com ponteiros inteligentes para o gerenciamento 
 * de memória.
 * 
 * @ingroup misc
 * @version 1.0
 * @data 2024
 * 
 * Licenciado sob a Licença Pública Geral GNU, versão 3.
 */

//==============================================================================
// Includes do C++
//==============================================================================

#include <list>
#include <memory>
#include <vector>

//==============================================================================
// Incluições do FVMAKER
//==============================================================================

#include <FVMAKER/Misc/namespace.h>

FVMAKER_NAMESPACE_OPEN
FVMAKER_NAMESPACE_CLOSE

// Definições de tipos comuns

/// Tipo Real utilizado para cálculos (precisão dupla).
using Real = double;

/// Lista de inteiros.
using LstInt = std::list<int>;

/// Vetor de inteiros.
using VecInt = std::vector<int>;

/// Lista de números reais.
using LstReal = std::list<Real>;

/// Vetor de números reais.
using VecReal = std::vector<Real>;

// Ponteiros inteligentes para vetores de números reais
using PtrVecReal = std::shared_ptr<VecReal>;        ///< Ponteiro compartilhado para um vetor de números reais.
using PtrConstVecReal = std::shared_ptr<const VecReal>; ///< Ponteiro compartilhado para um vetor de números reais constante.

#endif // __FVMAKER_TYPE_H__
