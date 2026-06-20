# Revisao dos exercicios computacionais do Capitulo 6

Referencia: bloco `No computador` do LaTeX fornecido em 20 de junho de 2026.

## Mapeamento

| Programa | Label no LaTeX | Titulo de referencia | Situacao |
|---|---|---|---|
| 6.1 | `exc_06:coeficientes` | Montar os coeficientes | alinhado |
| 6.2 | `exc_06:tdma` | Implementar o TDMA | alinhado usando o TDMA da biblioteca |
| 6.3 | `exc_06:tabelas-iterativos` | Os dois metodos iterativos, lado a lado | corrigido |
| 6.4 | `exc_06:tempo-iteracoes` | Iteracoes nao sao tempo | corrigido |
| 6.5 | `exc_06:parada-prog` | Como o criterio de parada muda a resposta | alinhado; campanha completa opcional |
| 6.6 | `exc_06:piso-epsilon` | O piso da tolerancia | alinhado |
| 6.7 | `exc_06:estimativa-inicial` | O efeito da estimativa inicial | alinhado, com ressalva matematica |
| 6.8 | `exc_06:dominancia-fraca` | Quando a dominancia apenas fraca ainda converge | corrigido |

Nao foi encontrada renumeracao entre os oito blocos de programacao: a ordem e os
titulos correspondem a 6.1--6.8.

## Item que mudou de secao

`caso_extra_convergencia` nao e um nono exercicio computacional. Ele gera as
tabelas dos exemplos convergente/divergente e corresponde ao exercicio
`exc_06:converge-diverge`, localizado no bloco de exercicios conceituais antes
de `No computador`. Deve permanecer como caso auxiliar, sem receber numero
6.9.

## Correcoes feitas

- 6.1 e 6.2 passaram a imprimir os sinais na convencao do livro.
- 6.3 deixou de comparar BiCG/BiCGSTAB e passou a reproduzir Jacobi e as
  varreduras crescente, decrescente e hibrida de Gauss-Seidel.
- 6.4 passou a comparar somente Jacobi e Gauss-Seidel, repetir a medicao tres
  vezes e registrar ambiente de compilacao. `--full` seleciona os tamanhos do
  enunciado.
- 6.5 oferece `--full` para 256 volumes; o modo padrao e curto o bastante para
  integracao continua.
- 6.7 passou a usar a norma L2 e a definicao exata `||R||/||R0||`.
- 6.8 passou a imprimir o historico inicial do residuo do sistema nao dominante.

## Inconsistencia no exercicio 6.7

Para este sistema linear, os chutes constantes 0, 0.9 e 0.99 satisfazem

    e0(0.9)  = 0.1  e0(0)
    e0(0.99) = 0.01 e0(0)

Como a iteracao e linear, os residuos em cada iteracao conservam a mesma
proporcionalidade. Dividir por `||R0||` cancela esse fator; os tres casos
atingem a tolerancia relativa na mesma iteracao, salvo efeitos de arredondamento.
Logo, esses chutes nao demonstram a frase de que uma estimativa melhor pode
exigir mais iteracoes. O programa agora evidencia esse resultado.

A afirmacao geral pode ocorrer quando os erros iniciais nao sao simples
multiplos entre si ou excitam modos diferentes da matriz de iteracao. Para
demonstrar isso, o enunciado precisaria escolher estimativas com formas
espaciais diferentes, e nao apenas tres constantes ao longo da reta entre zero e
a solucao exata.

## Localizacao dos links

Os links do LaTeX apontam para o repositorio `FVGridMaker`, mas os programas
estao atualmente em `FVMaker/capitulos/capitulo_06`. As URLs do manuscrito
devem ser atualizadas caso essa seja a organizacao definitiva.
