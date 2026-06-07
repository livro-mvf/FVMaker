//==============================================================================
// Name        : AbstractGrid1D.h
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.1
// Description : Definicao da classe AbstractGrid1D, que representa uma grade unidimensional
//               na biblioteca FVMaker.
//
// Copyright   : Copyright (C) 2025 Joao Flavio Vasconcellos
//               (jflavio at iprj.uerj.br)
//
// License     : Este programa e software livre: voce pode redistribui-lo e/ou
//               modifica-lo sob os termos da Licenca Publica Geral GNU, conforme
//               publicada pela Free Software Foundation, versao 3 da licenca,
//               ou (a seu criterio) qualquer versao posterior.
//
//               Este programa e distribuido na esperanca de que seja util,
//               mas SEM QUALQUER GARANTIA; sem mesmo a garantia implicita de
//               COMERCIABILIDADE ou ADEQUACAO A UM DETERMINADO PROPOSITO. Consulte
//               a Licenca Publica Geral GNU para mais detalhes.
//
//               Voce deve ter recebido uma copia da Licenca Publica Geral GNU
//               junto com este programa. Caso contrario, veja <https://www.gnu.org/licenses/>.
//==============================================================================

#pragma once

/**
 * @file AbstractGrid1D.h
 * @brief Classe base para grades unidimensionais
 * @ingroup Grid1D
 *
 * Define a interface comum para todas as grades unidimensionais na biblioteca FVMaker,
 * com suporte a operacoes paralelas via ParallelControl.
 */

//==============================================================================
// Includes
//==============================================================================
#include <memory>
#include <FVMaker/Grid/abstractGrid.h>
#include <FVMaker/Misc/type.h>
#include <FVMaker/Misc/namespace.h>
#include <FVMaker/Misc/parallelControl.h>

GRID_NAMESPACE_OPEN

/**
 * @class AbstractGrid1D
 * @brief Classe base abstrata para grades unidimensionais
 *
 * Fornece a interface comum para implementacao de grades 1D com suporte a:
 * - Diferentes padroes de discretizacao (CellCentered, FaceCentered)
 * - Operacoes paralelas controladas por ParallelControl
 * - Calculo automatico de distancias entre elementos
 *
 * @tparam TypePattern Tipo do padrao de discretizacao
 */
template<typename TypePattern>
class AbstractGrid1D : public AbstractGrid<TypePattern> {
    
//==============================================================================
// Construtores/Destrutora
//==============================================================================
public:
    
    /**
     * @brief Construtor padrao
     */
    AbstractGrid1D() noexcept = default;
    
    /**
     * @brief Construtor completo
     * @param _nvol Numero de volumes
     * @param _length Comprimento total da grade
     * @param _xIni Coordenada inicial (padrao = 0)
     */
    AbstractGrid1D(const int&, const Real&, const Real& = 0);
    
    /**
     * @brief Construtor de copia
     */
    AbstractGrid1D(const AbstractGrid1D&) noexcept = default;
    
    /**
     * @brief Destrutor virtual
     */
    virtual ~AbstractGrid1D() noexcept override = default;
    
    /**
     * @brief Construtor de movimento (deletado)
     */
    AbstractGrid1D(AbstractGrid1D&&) = delete;

//==============================================================================
// Operadores
//==============================================================================
public:
    
    /**
     * @brief Operador de atribuicao de copia (deletado)
     */
    AbstractGrid1D& operator=(const AbstractGrid1D&) = delete;
    
    /**
     * @brief Operador de atribuicao de movimento (deletado)
     */
    AbstractGrid1D& operator=(AbstractGrid1D&&) = delete;
    
    /**
     * @brief Operador de saida para impressao
     */
    template <typename U>
    friend std::ostream& operator<<(std::ostream&, const AbstractGrid1D<U>&);

//==============================================================================
// Interface Publica
//==============================================================================
public:
    
    /**
     * @brief Cria uma copia da grade
     * @return Ponteiro unico para a nova instancia
     */
    [[nodiscard]] virtual std::unique_ptr<AbstractGrid1D> Clone() const = 0;
    
    /**
     * @brief Gera as coordenadas das faces
     * @return true se bem-sucedido
     *
     * @note Utiliza ParallelControl para operacoes paralelas
     */
    [[nodiscard]] virtual bool GeraFaces() {return false;};
    
    /**
     * @brief Gera as coordenadas dos centros
     * @return true se bem-sucedido
     *
     * @note Utiliza ParallelControl para operacoes paralelas
     */
    [[nodiscard]] virtual bool GeraCentros() {return false;};
    
    /**
     * @brief Verifica se a grade esta vazia
     * @return true se vazia
     */
    [[nodiscard]] virtual bool empty() const override {return xFace_.empty() || xCentro_.empty();};

//==============================================================================
// Operacoes da Grade
//==============================================================================
public:
        
    /**
     * @brief Inicializa os vetores de armazenamento
     */
    void InitVector();
    
    /**
     * @brief Calcula as coordenadas dos centros
     * @param _offset Offset para posicionamento
     * @return true se bem-sucedido
     *
     * @note Utiliza ParallelControl para operacoes paralelas
     */
    [[nodiscard]] bool CalculaCentros(const Real&);
    
    /**
     * @brief Calcula as coordenadas das faces
     * @param _offset Offset para posicionamento
     * @return true se bem-sucedido
     *
     * @note Utiliza ParallelControl para operacoes paralelas
     */
    [[nodiscard]] bool CalculaFaces(const Real&);
    
    /**
     * @brief Calcula as distancias entre elementos
     * @return true se bem-sucedido
     *
     * @note Utiliza ParallelControl para operacoes paralelas
     */
    [[nodiscard]] bool CalculaDistancias();

//==============================================================================
// Acesso aos Dados
//==============================================================================
public:
        
    [[nodiscard]] inline size_t size() const {return nVol_;};
    [[nodiscard]] inline size_t NVol() const {return nVol_;};
    [[nodiscard]] inline Real Length() const {return length_;};
    [[nodiscard]] inline Real XInit() const {return xIni_;};
    [[nodiscard]] inline Real XEnd() const {return xIni_ + length_;};

    [[nodiscard]] inline const VecReal& GetFace() const {return xFace_;};
    [[nodiscard]] inline const VecReal& GetCentre() const {return xCentro_;};
    [[nodiscard]] inline const VecReal& GetDFace() const {return dxFace_;};
    [[nodiscard]] inline const VecReal& GetDCentre() const {return dxCentro_;};
    
    [[nodiscard]] inline VecReal* AddressxFace() {return &xFace_;};
    [[nodiscard]] inline const VecReal* AddressxFace() const {return &xFace_;};

//==============================================================================
// Dados da Classe
//==============================================================================
protected:
    VecReal xFace_;             ///< Coordenadas das faces
    VecReal xCentro_;           ///< Coordenadas dos centros
    VecReal dxFace_;            ///< Distancias entre faces
    VecReal dxCentro_;          ///< Distancias entre centros
    Real xIni_ = 0;             ///< Coordenada inicial
    Real length_ = 0;           ///< Comprimento total
    int nVol_ = 0;              ///< Numero de volumes
    std::shared_ptr<TypePattern> typePattern_ = nullptr; ///< Padrao de discretizacao
};

//==============================================================================
// Tipos de Ponteiros
//==============================================================================
template<typename TypePattern>
using SharedGrid1D = std::shared_ptr<AbstractGrid1D<TypePattern>>;

template<typename TypePattern>
using SharedConstGrid1D = std::shared_ptr<AbstractGrid1D<TypePattern> const>;

template<typename TypePattern>
using UniqueGrid1D = std::unique_ptr<AbstractGrid1D<TypePattern>>;

template<typename TypePattern>
using UniqueConstGrid1D = std::unique_ptr<AbstractGrid1D<TypePattern> const>;

GRID_NAMESPACE_CLOSE

#include <FVMaker/Grid/Grid1D/abstractGrid1D.hpp>