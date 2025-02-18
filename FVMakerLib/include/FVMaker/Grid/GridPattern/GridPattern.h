#ifndef __FVMAKER_GRIDPATTERN_H__
#define __FVMAKER_GRIDPATTERN_H__

//==============================================================================
// Nome        : GridPattern.h
// Autor       : João Flávio Vieira de Vasconcellos
// Versão      : 1.0
// Descrição   : Classe base para os padrões de geração de malha. Define a interface
//               para os padrões de malha, como Face-Centered, Volume-Centered, etc.
//==============================================================================

//==============================================================================
// Includes do C++
//==============================================================================

#include <memory>

//==============================================================================
// FVMAKER includes
//==============================================================================

#include <FVMaker/Misc/namespace.h>
#include <FVMaker/Misc/type.h>

//==============================================================================
// Abertura do namespace FVMaker::Grid
//==============================================================================

GRID_NAMESPACE_OPEN

/**
 * @class GridPattern
 * @brief Classe base para os padrões de geração de malhas.
 * 
 * A classe `GridPattern` define a interface para a geração de coordenadas de faces e centros.
 * Cada padrão (como `FaceCenteredPattern` ou `VolumeCenteredPattern`) deve implementar a 
 * lógica para gerar as coordenadas das faces e centros usando a função `generate`.
 */
class GridPattern {
    
//==============================================================================
// Construtores / destrutora
//==============================================================================
        
public:
    
     /**
     * @brief Construtora default da classe GridPattern.
     */
    GridPattern() noexcept = default;

     /**
     * @brief Construtor de cópia da classe GridPattern.
     * 
     * @param other Objeto a ser copiado.
     */
    GridPattern(const GridPattern&) noexcept = default;
    
     /**
     * @brief Destrutora da classe GridPattern.
     */
    virtual ~GridPattern() noexcept = default;

    /**
     * @brief Função puramente virtual para gerar as coordenadas das faces e centros.
     * As classes derivadas devem implementar essa função para a lógica específica.
     * @param input Coordenadas iniciais fornecidas pelo usuário.
     * @param faces Ponteiro para armazenar as coordenadas das faces.
     * @param centers Ponteiro para armazenar as coordenadas dos centros.
     */
    virtual void generate   ( const PtrVecReal& input
                            , PtrVecReal& faces
                            , PtrVecReal& centers
                            ) const = 0;
};

GRID_NAMESPACE_CLOSE

#endif // __FVMAKER_GRIDPATTERN_H__
