//==============================================================================
// Nome        : uniformGrid1D.h
// Autor       : Joao Flavio Vieira de Vasconcellos
// Versao      : 2.0
// Descricao   : Classe para geracao de malha uniforme 1D
//
// Este programa e software livre: voce pode redistribui-lo e/ou
// modifica-lo sob os termos da Licenca Publica Geral GNU, versao 3
// ou qualquer versao posterior.
//
// Este programa e distribuido na esperanca de que seja util,
// mas SEM QUALQUER GARANTIA; sem mesmo a garantia implicita de
// COMERCIABILIDADE ou ADEQUACAO A UM DETERMINADO PROPOSITO.
// Consulte a Licenca Publica Geral GNU para mais detalhes.
//
// Voce deve ter recebido uma copia da Licenca Publica Geral GNU
// junto com este programa. Se nao, veja <https://www.gnu.org/licenses/>.
//==============================================================================

#pragma once

/**
 * @file uniformGrid1D.h
 * @brief Classe para geracao de malha uniforme unidimensional
 * @ingroup Grid1D
 *
 * Implementa uma malha 1D com espacamento constante entre volumes,
 * utilizando o ParallelControl para gerenciamento de execucao paralela.
 *
 * @author Joao Flavio Vasconcellos
 * @version 2.0
 * @date 2025-05-20
 * @copyright GNU General Public License v3.0
 */

//==============================================================================
// Includes da FVMaker
//==============================================================================

#include <FVMaker/Grid/Grid1D/abstractGrid1D.h>
#include <FVMaker/Grid/GridPattern/cellCentered.h>


GRID_NAMESPACE_OPEN

/**
 * @class uniformGrid1D
 * @brief Malha uniforme unidimensional
 *
 * Gera uma malha com espacamento constante entre volumes, utilizando
 * politicas de execucao paralela configuraveis via ParallelControl.
 *
 * @tparam CellCentered Padrao de discretizacao centrado nas celulas
 */
class uniformGrid1D : public AbstractGrid1D<CellCentered> {
public:
    //==========================================================================
    // Tipo Publico
    //==========================================================================
    using DataType = Real;  ///< Tipo base para calculos numericos

    //==========================================================================
    // Construtores/Destrutores
    //==========================================================================
    
    /**
     * @brief Construtor padrao
     */
    uniformGrid1D() noexcept = default;
    
    /**
     * @brief Construtor completo
     * @param nVol Numero de volumes
     * @param length Comprimento total da malha
     * @param xIni Coordenada inicial (padrao = 0.0)
     */
    uniformGrid1D(const int& nVol, const Real& length, const Real& xIni = 0.0);
    
    /**
     * @brief Construtor de copia
     */
    uniformGrid1D(const uniformGrid1D&) noexcept;
    
    /**
     * @brief Destrutor
     */
    virtual ~uniformGrid1D() noexcept override = default;
    
    /**
     * @brief Construtor de movimento deletado
     */
    uniformGrid1D(uniformGrid1D&&) = delete;
    
    //==========================================================================
    // Operadores
    //==========================================================================
    
    /**
     * @brief Operador de atribuicao de copia deletado
     */
    uniformGrid1D& operator=(const uniformGrid1D&) = delete;
    
    /**
     * @brief Operador de atribuicao de movimento deletado
     */
    uniformGrid1D& operator=(uniformGrid1D&&) = delete;
    
    //==========================================================================
    // Interface Publica
    //==========================================================================
    
    /**
     * @brief Cria uma copia da malha
     * @return Ponteiro unico para a copia
     */
    [[nodiscard]] virtual std::unique_ptr<AbstractGrid1D<CellCentered>> Clone() const override;
    
    /**
     * @brief Gera as coordenadas das faces
     * @return true se gerado com sucesso
     *
     * Utiliza a politica de execucao configurada no ParallelControl
     */
    [[nodiscard]] virtual bool GeraFaces() override;
    
    /**
     * @brief Gera as coordenadas dos centros
     * @return true se gerado com sucesso
     *
     * Utiliza a politica de execucao configurada no ParallelControl
     */
    [[nodiscard]] virtual bool GeraCentros() override;
    
private:
    
    //==========================================================================
    // Implementacao
    //==========================================================================
    
    /**
     * @brief Gera as coordenadas usando a politica atual
     * @param _coords Vetor de coordenadas a ser preenchido
     * @param _offset Offset para posicionamento (0.0 para faces, 0.5 para centros)
     */
    void GeraCoordenadas(VecReal* _coords, const Real& _offset);
};

GRID_NAMESPACE_CLOSE

//#include <FVMaker/Grid/Grid1D/uniformGrid1D.hpp>