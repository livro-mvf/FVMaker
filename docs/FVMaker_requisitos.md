# FVMaker - Requisitos Funcionais e Não Funcionais

Este documento descreve os requisitos iniciais da nova FVMaker, daqui em
diante chamada de FVM.

A FVM deve ser uma biblioteca C++ para solução de problemas de volumes finitos
em malhas estruturadas, acompanhando os exemplos e a exposição didática do
livro.

## 1. Escopo

A FVM deve resolver problemas em:

```text
1D;
2D.
```

A FVM não deve resolver problemas 3D.

A FVM deve usar a FVGridMaker como biblioteca responsável por:

```text
geração de malhas estruturadas;
armazenamento geométrico da malha;
validação geométrica da malha;
métricas geométricas primárias;
malhas 1D;
malhas estruturadas 2D.
```

A FVM não deve implementar geração própria de malhas.

## 2. Requisitos Funcionais

## 2.1 Papel da FVM

A FVM deve tratar de:

```text
campos/variáveis definidos sobre malhas estruturadas;
funções analíticas escalares e vetoriais para MMS, fontes e coeficientes;
coeficientes físicos;
termos fonte;
condições de contorno;
termos diferenciais independentes;
montagem de equações discretizadas;
montagem de sistemas lineares;
solução de sistemas lineares;
controle de solução em regime permanente;
controle de solução transiente;
esquemas temporais de primeira e segunda ordem;
exemplos didáticos ligados ao livro;
programas de performance/profiling.
```

A FVM não deve tratar de:

```text
geração de malha;
malhas não estruturadas;
malhas 3D;
parsing YAML dentro da biblioteca;
visualização gráfica interna;
dependência obrigatória de bibliotecas externas de álgebra linear;
modelos CFD gerais no estilo OpenFOAM.
```

## 2.2 Integração com FVGridMaker

A FVM deve consumir objetos da FVGridMaker por meio de views/adapters leves.

Para 1D, a FVM deve aceitar malhas baseadas em:

```text
fvgrid::Axis1D
```

Para 2D, a FVM deve aceitar malhas baseadas em:

```text
fvgrid::StructuredGrid2D
```

As views/adapters da FVM devem fornecer apenas o acesso necessário para a
montagem dos operadores de volumes finitos:

```text
número de volumes;
faces;
centros;
espaçamentos;
áreas/volumes de controle;
indexação estruturada;
vizinhança estruturada.
```

## 2.3 Campos e Variáveis

A FVM deve permitir a criação de variáveis/campos associados a uma malha.

Exemplos conceituais:

```cpp
auto phi = fvm::field(axis, "phi");
auto u = fvm::field(grid2d, "u");
```

Um campo deve armazenar:

```text
nome;
referência/view da malha;
valores nos volumes;
estado atual;
histórico temporal quando necessário.
```

A FVM deve suportar campos escalares em 1D e 2D.

A FVM deve distinguir, ao menos internamente:

```text
valores armazenados nos volumes/células;
valores avaliados ou interpolados nas faces;
fluxos definidos nas faces.
```

A FVM deve suportar condições iniciais para campos transientes.

Condições iniciais devem poder ser:

```text
uniformes;
definidas por função da posição;
definidas por campo;
compatíveis com histórico temporal quando o esquema exigir.
```

## 2.3.1 Funções Analíticas e MMS

A FVM deve fornecer uma infraestrutura comum para transportar funções
analíticas usadas em campos, coeficientes, fontes, condições de contorno,
soluções manufaturadas e testes de erro de truncamento.

Tipos funcionais desejados:

```text
ScalarFunction1D;
ScalarFunction2D;
VectorFunction1D;
VectorFunction2D.
```

A FVM deve distinguir:

```text
conceitos/templates para aceitar lambdas, funções livres, functors e
std::function sem custo extra quando a função é apenas avaliada;
tipos armazenáveis baseados em std::function quando a função precisa ser
guardada em objetos como fonte, coeficiente, condição de contorno ou MMS.
```

Para o método das funções manufaturadas, a FVM deve permitir armazenar e
transportar, inicialmente em 1D:

```text
phi(x);
dphi/dx;
d2phi/dx2;
um conjunto extensível de coeficientes variáveis nomeados, como gamma(x),
sigma(x), beta(x) ou outros exigidos pela equação;
derivadas de cada coeficiente quando necessárias;
termos fonte derivados da solução manufaturada.
```

As derivadas manufaturadas devem ser fornecidas pelo usuário, exemplos ou
ferramentas externas. A FVMakerLib não deve depender de bibliotecas simbólicas.

## 2.4 Coeficientes

A FVM deve representar coeficientes físicos usados em operadores e equações.

Tipos funcionais desejados:

```text
coeficiente uniforme;
coeficiente definido por campo;
coeficiente definido por função;
coeficiente dependente do tempo;
coeficiente dependente da posição.
```

## 2.5 Termos Fonte

A FVM deve representar termos fonte de forma independente da equação.

Tipos funcionais desejados:

```text
fonte uniforme;
fonte definida por campo;
fonte definida por função;
fonte dependente do tempo;
fonte dependente da posição.
```

A FVM deve suportar fontes explícitas e fontes linearizadas.

A forma linearizada mínima deve ser compatível com a decomposição:

```text
S = S_C + S_P phi
```

Essa forma deve ser usada quando necessário em problemas não lineares e em
formulações de volumes finitos no estilo clássico.

## 2.6 Condições de Contorno

A FVM deve suportar condições de contorno em 1D e 2D.

Tipos mínimos:

```text
Dirichlet;
Neumann;
Robin.
```

As condições de contorno devem poder ser:

```text
constantes;
dependentes da posição;
dependentes do tempo.
```

Em 1D, a FVM deve representar:

```text
left;
right.
```

Em 2D, a FVM deve representar:

```text
left;
right;
bottom;
top.
```

Exemplo conceitual:

```cpp
auto bc = fvm::boundary_1d(
    fvm::dirichlet_left(1.0),
    fvm::dirichlet_right(0.0)
);
```

A FVM deve suportar estratégias distintas de aplicação de condições de
contorno, incluindo:

```text
volumes fictícios/ghost volumes;
modificação direta dos coeficientes da matriz;
modificação do lado direito;
tratamento específico para termos advectivos.
```

Em problemas 1D, o uso de ghost cells deve ser opcional. A configuração
padrão não deve criar ghost cells, mas a infraestrutura deve permitir até duas
ghost cells em cada lado do domínio quando o esquema numérico exigir.

Alguns esquemas 1D podem gerar acoplamento com vizinhos até `i-2` e `i+2`.
Por isso, a FVM deve suportar sistemas pentadiagonais como alternativa
explícita aos sistemas tridiagonais. O caminho padrão para os primeiros
problemas 1D continua sendo tridiagonal, resolvido por TDMA.

## 2.7 Termos Diferenciais Independentes

A FVM deve permitir que cada derivada ou termo físico seja representado de
forma independente e depois combinado para formar uma equação.

Termos mínimos desejados:

```text
ddt       - derivada temporal de primeira ordem;
d2dt2     - derivada temporal de segunda ordem;
laplacian - operador difusivo;
source    - termo fonte.
```

Termos futuros possíveis:

```text
gradient;
divergence;
convection/advection;
reaction.
```

A FVM deve representar fluxos de face quando necessário para termos
advectivos ou conservativos.

Conceitos funcionais associados:

```text
FaceFlux;
MassFlux;
VelocityField;
interpolação de valores nas faces.
```

Exemplo conceitual:

```cpp
auto equation =
    fvm::ddt(phi)
  - fvm::laplacian(gamma, phi)
  == fvm::source(q);
```

Para equação da onda:

```cpp
auto equation =
    fvm::d2dt2(u)
  - fvm::laplacian(c2, u)
  == fvm::source(q);
```

Cada termo deve ser capaz de contribuir para o sistema final sem depender dos
demais termos da equação.

## 2.8 Equações

A FVM deve permitir compor equações por soma/subtração de termos.

Equações mínimas desejadas:

```text
difusão em regime permanente;
difusão transiente;
equação da onda;
equações fonte-difusão 1D;
equações fonte-difusão 2D.
```

A FVM deve permitir que o usuário escreva equações em uma forma próxima da
notação matemática e da exposição do livro.

## 2.9 Montagem do Sistema

A FVM deve montar o sistema algébrico final a partir dos termos da equação.

O processo de montagem deve:

```text
receber uma equação;
percorrer os termos independentes;
somar contribuições de matriz;
somar contribuições de vetor fonte;
aplicar condições de contorno;
gerar um sistema linear final;
preservar a estrutura 1D ou 2D do problema.
```

A montagem deve ser separada da solução do sistema linear.

## 2.10 Sistemas Lineares

A FVM deve possuir uma representação própria mínima de sistemas lineares.

O sistema linear deve conter:

```text
matriz;
vetor do lado direito;
vetor de incógnitas;
informações de dimensão;
informações de estrutura quando úteis.
```

Para 1D, a FVM deve permitir solvers especializados para sistemas
tridiagonais quando aplicável.

Para 2D, a FVM deve permitir sistemas esparsos estruturados.

A FVM deve permitir calcular o erro/resíduo algébrico associado ao sistema:

```text
erro = A X - B
```

O erro de truncamento usado nos testes numéricos deve poder ser medido pela
norma desse erro.

Normas mínimas desejadas:

```text
norma infinito;
norma L2 discreta;
norma L1 discreta quando útil.
```

## 2.11 Solvers

A FVM deve fornecer solvers internos básicos, sem depender de bibliotecas
externas de álgebra linear.

Solvers iniciais desejados:

```text
Thomas1D para sistemas tridiagonais;
Gauss-Seidel;
Jacobi;
Conjugate Gradient para sistemas simétricos positivos definidos.
```

## 2.12 Controle de Solução

A FVM deve separar a equação do controle do processo de solução.

Controles mínimos:

```text
SteadyState;
Transient.
```

`SteadyState` deve controlar:

```text
tolerância;
número máximo de iterações;
critério de convergência.
```

Critérios de convergência devem poder considerar:

```text
norma absoluta do resíduo;
norma relativa do resíduo;
norma da correção;
variação entre iterações;
falha por divergência;
número máximo de iterações.
```

`Transient` deve controlar:

```text
tempo inicial;
tempo final;
passo de tempo;
número máximo de passos;
política de armazenamento de histórico.
```

Para problemas transientes, a FVM deve controlar também:

```text
condições iniciais;
restrições de estabilidade quando aplicáveis;
número de CFL em problemas advectivos ou hiperbólicos;
passo de tempo fixo;
passo de tempo adaptativo futuro.
```

Para problemas iterativos e não lineares, a FVM deve suportar:

```text
relaxação de campos;
relaxação de equações;
controle de estabilidade da iteração;
relato estruturado de não convergência.
```

Exemplos conceituais:

```cpp
auto result = fvm::solve(equation, fvm::SteadyState{});
```

```cpp
auto result = fvm::solve(
    equation,
    fvm::Transient{.dt = 0.001, .t_final = 1.0},
    fvm::CrankNicolson{}
);
```

## 2.13 Esquemas Temporais

A FVM deve suportar esquemas temporais extensíveis.

Esquemas nativos iniciais:

```text
ExplicitEuler;
ImplicitEuler;
CrankNicolson;
BDF2;
CentralDifference2;
Newmark.
```

A FVM deve suportar:

```text
derivada temporal de primeira ordem;
derivada temporal de segunda ordem;
esquemas de passo único;
esquemas multipasso;
esquemas com histórico;
esquemas explícitos;
esquemas implícitos.
```

Novos esquemas devem poder ser adicionados por tipos externos e traits/concepts,
sem modificar o núcleo da biblioteca.

## 2.14 Equação da Onda

A FVM deve resolver a equação da onda em 1D e 2D.

A equação da onda deve suportar:

```text
campo principal;
velocidade de onda ou coeficiente equivalente;
condições iniciais;
velocidade inicial;
condições de contorno;
fonte opcional;
esquemas temporais de segunda ordem.
```

## 2.15 Resultados

A FVM deve retornar resultados estruturados.

Um resultado de solução deve conter:

```text
estado final;
número de iterações ou passos;
tempo final atingido;
indicador de convergência;
norma/resíduo final;
norma do erro A X - B quando aplicável;
mensagem diagnóstica quando aplicável.
```

A FVM deve permitir saída simples de campos e soluções para inspeção.

Formatos iniciais desejados:

```text
CSV para campos 1D;
CSV para campos 2D;
VTK para campos 2D quando a FVGridMaker fornecer a geometria necessária.
```

Output de campos deve ser periférico e não deve modificar o estado da
solução.

## 2.16 API Pública

A API pública da FVM deve ser simples para usuários que não são especialistas
em C++.

O usuário deve poder montar problemas com chamadas curtas e próximas da
notação matemática.

Exemplo conceitual completo 1D:

```cpp
auto axis = fvgrid::Uniform1D::make(
    fvgrid::NVol{100},
    fvgrid::Length{1.0},
    fvgrid::XInit{0.0}
);

auto phi = fvm::field(axis, "phi");
auto gamma = fvm::coefficient(axis, 1.0);
auto q = fvm::source(axis, 0.0);

auto bc = fvm::boundary_1d(
    fvm::dirichlet_left(1.0),
    fvm::dirichlet_right(0.0)
);

auto equation =
    -fvm::laplacian(gamma, phi)
    == fvm::source(q);

auto result = fvm::solve(equation, bc, fvm::SteadyState{});
```

## 2.17 Exemplos do Livro

A FVM deve acompanhar o livro por meio de exemplos claros e pequenos.

Os exemplos do livro devem demonstrar:

```text
criação de malha com FVGridMaker;
criação de campos;
definição de coeficientes;
definição de fontes;
definição de condições de contorno;
montagem de equações;
solução em regime permanente;
solução transiente;
equação da onda;
interpretação dos resultados.
```

Os exemplos podem usar YAML como configuração externa, mas a biblioteca FVM
não deve depender de YAML.

## 2.18 Sequência de Problemas do Livro

O desenvolvimento funcional da FVM deve acompanhar a sequência didática dos
problemas apresentados no livro.

Essa sequência deve começar por problemas 1D simples, avançar para
coeficientes variáveis, não linearidade, advecção/interpolação e transiente,
e depois repetir a progressão essencial em 2D.

## 2.18.1 Problemas 1D em Regime Permanente

O primeiro problema-base deve ser:

```text
d2phi/dx2 = f(x)
```

Esse problema deve ser usado para implementar e testar:

```text
montagem 1D básica;
operador laplaciano 1D;
termo fonte;
solvers lineares;
condições de contorno;
volumes fictícios/ghost volumes;
comparação com soluções analíticas simples.
```

Esse bloco deve incluir verificações de conservação local e global quando
aplicável.

Em seguida, a FVM deve tratar problemas lineares com coeficiente variável:

```text
d/dx(gamma(x) dphi/dx) = f(x)
```

Esse bloco deve testar:

```text
coeficientes dependentes da posição;
interpolação de coeficientes nas faces;
montagem linear com coeficientes variáveis;
validação contra casos manufaturados.
```

Depois, a FVM deve tratar problemas 1D com coeficientes variáveis e não
lineares.

Esse bloco deve testar diferentes caminhos para problemas não lineares:

```text
iteração de Picard;
Newton ou Newton aproximado;
atualização explícita de coeficientes;
controle de convergência não linear;
critérios de parada;
relato de não convergência.
```

Esse bloco deve testar também:

```text
fontes linearizadas;
relaxação de campos;
relaxação de equações.
```

## 2.18.2 Problemas 1D com Advecção

Após os problemas 1D não lineares, a FVM deve incluir problemas 1D com
advecção.

Esse bloco deve introduzir e comparar funções de interpolação/esquemas para
valores nas faces.

Esquemas iniciais desejados:

```text
upwind;
central difference;
power-law;
exponential;
QUICK ou outro esquema de maior ordem quando adequado ao livro.
```

Esse bloco deve testar:

```text
termo advectivo 1D;
fluxo nas faces;
campos em volumes e valores interpolados nas faces;
estabilidade numérica;
efeito do número de Peclet;
efeito do número de CFL em problemas transientes advectivos;
comparação entre esquemas de interpolação;
tratamento de condições de contorno em problemas advectivos.
```

## 2.18.3 Problemas 1D Transientes

A FVM deve tratar problemas 1D transientes.

Esse bloco deve testar:

```text
ddt(phi);
condições iniciais;
controle Transient;
armazenamento de histórico;
esquemas temporais de primeira ordem;
esquemas temporais de segunda ordem;
restrições de estabilidade temporal;
problemas parabólicos 1D;
equação da onda 1D com d2dt2(phi).
```

## 2.18.4 Problemas 2D em Regime Permanente

Depois da base 1D, a FVM deve tratar problemas 2D em regime permanente
linear.

Esse bloco deve testar:

```text
campos 2D;
indexação estruturada 2D;
laplaciano 2D;
fontes 2D;
condições de contorno left/right/bottom/top;
montagem esparsa estruturada;
solvers iterativos;
verificação de conservação local e global;
soluções manufaturadas para testes de ordem.
```

Em seguida, a FVM deve tratar problemas 2D em regime permanente não linear.

Esse bloco deve testar:

```text
coeficientes 2D variáveis;
coeficientes 2D não lineares;
iteração não linear;
controle de convergência;
atualização de campos auxiliares.
```

## 2.18.5 Problemas 2D Transientes

A FVM deve tratar problemas 2D transientes.

Esse bloco deve testar:

```text
ddt(phi) em 2D;
d2dt2(phi) em 2D;
controle de passo de tempo;
histórico temporal 2D;
equação da onda 2D;
acoplamento entre montagem espacial e esquema temporal.
```

## 2.18.6 Problemas 2D com Advecção

Após os problemas 2D transientes, a FVM deve incluir problemas 2D com
advecção.

Esse bloco deve testar:

```text
termo advectivo 2D;
fluxos nas faces x e y;
esquemas de interpolação em 2D;
efeito do número de Peclet;
efeito do número de CFL em problemas transientes advectivos;
tratamento de condições de contorno advectivas;
comparação entre soluções 2D com diferentes esquemas.
```

## 3. Requisitos Não Funcionais

## 3.1 Linguagem e Plataforma

A FVM deve usar:

```text
C++20;
CMake;
GoogleTest apenas para testes.
```

O código público e os comentários de API devem ser escritos em inglês.

Discussões, documentação de planejamento e material do livro podem ser em
português.

## 3.2 Dependências Externas

A FVMakerLib não deve depender de bibliotecas externas.

Exceções permitidas:

```text
FVGridMaker como dependência de malha;
GoogleTest apenas para testes;
YAML apenas em exemplos ou aplicações externas;
ferramentas de profiling apenas no desenvolvimento.
```

A FVMakerLib não deve:

```text
incluir headers YAML;
linkar com yaml-cpp;
incluir headers GoogleTest;
linkar com GoogleTest;
depender de bibliotecas externas de álgebra linear;
depender de bibliotecas externas de solvers.
```

## 3.3 Arquitetura

A FVM deve evitar herança na arquitetura principal.

A FVM deve preferir:

```text
composição;
tipos concretos;
funções livres;
templates;
concepts;
traits;
views/adapters leves.
```

A FVM deve usar DOD onde houver dados numéricos intensivos:

```text
campos;
coeficientes;
fontes;
resíduos;
matrizes;
vetores;
operadores;
loops sobre volumes/faces.
```

SOLID deve ser usado como guia de organização, principalmente:

```text
responsabilidade única;
baixo acoplamento;
interfaces públicas estáveis;
componentes pequenos;
testabilidade.
```

## 3.4 Flexibilidade e Simplicidade de Uso

A FVM deve ser flexível internamente e simples externamente.

A complexidade de:

```text
templates;
traits;
armazenamento;
otimizações;
montagem;
histórico temporal;
integração com FVGridMaker;
solvers;
profiling.
```

deve ficar encapsulada na biblioteca.

O programa escrito pelo usuário deve ser curto, legível e próximo da notação
matemática usada no livro.

## 3.5 Desempenho

A FVM deve priorizar tempo de execução.

Maior tempo de compilação é aceitável quando isso melhorar o desempenho de
execução.

A FVM deve evitar polimorfismo dinâmico em caminhos críticos, como:

```text
montagem de termos;
loops sobre volumes/faces;
avaliação de coeficientes;
aplicação de condições de contorno;
montagem de sistemas;
solvers lineares;
avanço temporal.
```

## 3.6 Sistema de Erros

A FVM deve adaptar o sistema de erros da FVGridMaker.

O sistema de erros deve usar:

```text
códigos textuais;
descritores de erro;
registro completo do erro;
exceção base da FVM;
funções throw_error() e require();
ID textual da fonte do erro;
source_location quando possível.
```

A FVM não deve usar:

```text
enum central de erros;
enum central de classes;
exit();
abort();
impressão direta em std::cerr como mecanismo de erro da biblioteca.
```

Erros devem ser reportados por exceções estruturadas. O usuário ou exemplo
decide como apresentar o diagnóstico.

## 3.7 Testes

Cada classe ou componente público novo deve ter testes com GoogleTest.

Todo bloco funcional deve ser fechado com:

```text
build completo;
testes unitários;
CTest;
exemplo público quando houver API pública.
```

Os testes devem verificar:

```text
invariantes;
casos válidos;
casos inválidos;
mensagens/códigos de erro esperados;
resultados numéricos simples;
soluções analíticas e manufaturadas;
ordem de convergência quando aplicável;
conservação local e global quando aplicável;
norma do erro A X - B;
regressões de montagem;
regressões de solvers.
```

## 3.8 Exemplos

Cada funcionalidade pública relevante deve ter exemplo executável.

Exemplos devem ser:

```text
curtos;
didáticos;
determinísticos;
adequados ao livro;
independentes de detalhes internos da biblioteca.
```

YAML pode ser usado em exemplos, desde que fique fora da FVMakerLib.

## 3.9 Performance e Profiling

A FVM deve possuir programas específicos para análise de desempenho.

Esses programas devem ser separados de:

```text
testes unitários;
exemplos didáticos;
capítulos do livro.
```

Ferramentas externas permitidas no desenvolvimento:

```text
perf;
valgrind;
callgrind;
gprof;
sanitizers.
```

Essas ferramentas não devem ser dependências da FVMakerLib.

Os programas de performance devem permitir avaliar:

```text
montagem de termos;
montagem de sistemas;
solvers lineares;
avanço temporal;
uso de memória;
problemas grandes 1D;
problemas grandes 2D.
```

## 3.10 Organização 1D e 2D

A FVM deve separar explicitamente os componentes 1D e 2D na organização do
código e dos testes.

Traits e templates podem ser usados internamente para reduzir duplicação, mas
a estrutura do projeto deve deixar claro quais componentes pertencem a 1D e
quais pertencem a 2D.

## 3.11 Convenções de Programação

A FVM deve seguir o padrão de programação adotado na FVGridMaker.

Arquivos fonte devem usar o cabeçalho padrão:

```cpp
// ----------------------------------------------------------------------------
// File: <FileName>
// Project: FVMaker
// Version: 0.1.0
// Description: <short description>
// Author: FVMaker Team
// License: GPL-3.0-or-later
// ----------------------------------------------------------------------------
```

Os includes devem ser agrupados nesta ordem:

```cpp
// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// FVMaker includes
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// External library includes
// ----------------------------------------------------------------------------
```

Convenções de nomes:

```text
Tipos/classes/structs/concepts  : PascalCase
Funções e métodos               : snake_case
Variáveis locais                : snake_case
Parâmetros de funções           : snake_case
Membros privados                : snake_case_
Constantes globais constexpr    : kPascalCase
Namespaces                      : snake_case curto, preferencialmente fvm
Arquivos de teste               : tst_<Component>.cc
Arquivos de exemplo             : ex_<Example>.cc
```

Exemplo:

```cpp
class Field1D final {
public:
    [[nodiscard]] Size size() const noexcept;
    [[nodiscard]] std::span<const Real> values() const noexcept;

private:
    std::vector<Real> values_;
    std::string name_;
};
```

Regras adicionais:

```text
usar #pragma once em headers;
usar [[nodiscard]] em funções cujo retorno não deve ser ignorado;
usar noexcept em funções que garantidamente não lançam exceção;
preferir const para variáveis locais que não mudam;
preferir std::span para exposição não proprietária de arrays;
preferir std::vector para armazenamento contíguo;
evitar abreviações opacas em nomes públicos;
evitar includes comentados;
evitar using namespace em headers;
evitar macros para identidade, erros ou APIs públicas.
```

Identidade de classes:

```cpp
[[nodiscard]] static constexpr ID id() noexcept;
[[nodiscard]] static constexpr std::string_view class_name() noexcept;
[[nodiscard]] static constexpr std::string_view class_id() noexcept;
```

Exemplo de `class_id`:

```text
fvm.1d.field.Field1D
fvm.1d.system.TridiagonalSystem1D
fvm.1d.term.Laplacian1D
fvm.time.Transient
```

## 3.12 Gates de Desenvolvimento

O avanço da FVM deve ocorrer por blocos pequenos.

Nenhum bloco novo deve começar se o bloco anterior estiver com:

```text
build quebrado;
teste quebrado;
exemplo público quebrado;
erro esperado sem teste;
API pública sem exemplo;
componente público sem teste.
```

## 4. Vocabulário Inicial

Vocabulário preferencial:

```text
Field;
Coefficient;
Source;
Boundary;
Term;
Equation;
Assembler;
LinearSystem;
Solver;
SteadyState;
Transient;
TimeScheme;
SolveResult;
Execution;
Controller.
```

Vocabulário a evitar no núcleo da FVM:

```text
Mesh;
Grid generation;
3D;
Runtime polymorphism para termos;
YAML parser;
External solver backend obrigatório.
```
