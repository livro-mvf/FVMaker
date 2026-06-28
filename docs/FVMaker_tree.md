FVMaker - Tree do Projeto

Este documento define a árvore de referência da nova FVMaker, daqui em diante
chamada de FVM.

A FVM é uma biblioteca de volumes finitos para problemas 1D e 2D em malhas
estruturadas. A geração, o armazenamento geométrico e a validação primária das
malhas pertencem à FVGridMaker. A FVM consome essas malhas e implementa campos,
coeficientes, fontes, condições de contorno, termos diferenciais, montagem de
sistemas, solvers e controle de solução.

## 1. Princípios da árvore

A organização do projeto deve refletir estas decisões:

```text
1. A FVM resolve problemas de volumes finitos em 1D e 2D.
2. A FVM não implementa problemas 3D.
3. A FVM não gera malhas.
4. A geometria de malha vem da FVGridMaker.
5. Em 1D, a FVM consome fvgrid::Axis1D.
6. Em 2D, a FVM consome fvgrid::StructuredGrid2D.
7. Grid na FVM significa view/adaptação/acesso, não geração.
8. Componentes 1D e 2D devem ficar separados explicitamente.
9. Campos armazenam valores em volumes/células.
10. Valores em faces e fluxos de faces são avaliados por componentes próprios.
11. Termos diferenciais são independentes e combináveis em equações.
12. Assembly monta sistemas; Solver resolve sistemas.
13. Controle de solução fica separado da equação.
14. Output é periférico e não modifica campos, grids ou sistemas.
15. YAML é permitido apenas em exemplos ou aplicações externas.
16. GoogleTest aparece apenas nos testes.
17. Ferramentas de profiling aparecem apenas em programas auxiliares.
18. Nenhum módulo periférico deve criar dependência reversa no núcleo.
19. Nenhuma categoria extensível deve ser implementada com enum central.
20. Toda classe que possa ser origem de erro deve expor ID próprio.
```

Vocabulário preferencial:

```text
Field
Coefficient
Source
Boundary
Flux
Interpolation
Term
Equation
Assembler
LinearSystem
Solver
SteadyState
Transient
TimeScheme
SolveResult
Execution
Controller
Output
```

Vocabulário a evitar no núcleo da FVM:

```text
Mesh
Grid generation
3D
Runtime polymorphism para termos
YAML parser
External solver backend obrigatório
```

## 2. Tree recomendada

```text
FVMaker/
├── CMakeLists.txt
├── README.md
├── LICENSE
├── .gitignore
├── .clang-format
│
├── cmake/
│   ├── ConfigCompiler.cmake
│   ├── ConfigDependencies.cmake
│   ├── ConfigDocs.cmake
│   ├── ConfigExamples.cmake
│   ├── ConfigLivro.cmake
│   ├── ConfigOptions.cmake
│   ├── ConfigPerformance.cmake
│   ├── ConfigTargets.cmake
│   └── ConfigTests.cmake
│
├── docs/
│   ├── conf.py
│   ├── index.rst
│   ├── FVMaker_requisitos.md
│   ├── FVMaker_cronograma_por_gates.md
│   ├── FVMaker_tree.md
│   └── _static/
│
├── FVMakerLib/
│   ├── include/
│   │   └── FVMaker/
│   │       ├── FVMaker.h
│   │       │
│   │       ├── Core/
│   │       │   ├── ID.h
│   │       │   ├── StrongTypes.h
│   │       │   ├── Types.h
│   │       │   └── Version.h
│   │       │
│   │       ├── Error/
│   │       │   ├── ErrorCatalog.h
│   │       │   ├── ErrorCodes.h
│   │       │   ├── ErrorDescriptor.h
│   │       │   ├── ErrorRecord.h
│   │       │   ├── FVMException.h
│   │       │   └── ThrowError.h
│   │       │
│   │       ├── Common/
│   │       │   ├── DenseVector.h
│   │       │   ├── ErrorNorms.h
│   │       │   ├── Residual.h
│   │       │   └── Storage.h
│   │       │
│   │       ├── OneDimensional/
│   │       │   ├── Grid/
│   │       │   │   ├── Axis1DView.h
│   │       │   │   └── GridMetrics1D.h
│   │       │   │
│   │       │   ├── Field/
│   │       │   │   ├── Field1D.h
│   │       │   │   ├── FieldView1D.h
│   │       │   │   └── InitialCondition1D.h
│   │       │   │
│   │       │   ├── Coefficient/
│   │       │   │   ├── Coefficient1D.h
│   │       │   │   ├── FieldCoefficient1D.h
│   │       │   │   ├── FunctionCoefficient1D.h
│   │       │   │   └── UniformCoefficient1D.h
│   │       │   │
│   │       │   ├── Source/
│   │       │   │   ├── FieldSource1D.h
│   │       │   │   ├── FunctionSource1D.h
│   │       │   │   ├── LinearizedSource1D.h
│   │       │   │   ├── Source1D.h
│   │       │   │   └── UniformSource1D.h
│   │       │   │
│   │       │   ├── Boundary/
│   │       │   │   ├── BoundarySet1D.h
│   │       │   │   ├── BoundarySide1D.h
│   │       │   │   ├── Dirichlet1D.h
│   │       │   │   ├── GhostBoundary1D.h
│   │       │   │   ├── Neumann1D.h
│   │       │   │   └── Robin1D.h
│   │       │   │
│   │       │   ├── Flux/
│   │       │   │   ├── FaceFlux1D.h
│   │       │   │   └── MassFlux1D.h
│   │       │   │
│   │       │   ├── Interpolation/
│   │       │   │   ├── CentralDifference1D.h
│   │       │   │   ├── Exponential1D.h
│   │       │   │   ├── Interpolation1D.h
│   │       │   │   ├── PowerLaw1D.h
│   │       │   │   ├── QUICK1D.h
│   │       │   │   └── Upwind1D.h
│   │       │   │
│   │       │   ├── Term/
│   │       │   │   ├── Advection1D.h
│   │       │   │   ├── D2dt2_1D.h
│   │       │   │   ├── Ddt1D.h
│   │       │   │   ├── Laplacian1D.h
│   │       │   │   └── SourceTerm1D.h
│   │       │   │
│   │       │   ├── Equation/
│   │       │   │   ├── Equation1D.h
│   │       │   │   ├── Diffusion1D.h
│   │       │   │   └── Wave1D.h
│   │       │   │
│   │       │   ├── System/
│   │       │   │   └── TridiagonalSystem1D.h
│   │       │   │
│   │       │   ├── Assembly/
│   │       │   │   ├── Assembler1D.h
│   │       │   │   └── BoundaryAssembler1D.h
│   │       │   │
│   │       │   └── Solver/
│   │       │       ├── ConjugateGradient1D.h
│   │       │       ├── GaussSeidel1D.h
│   │       │       ├── Jacobi1D.h
│   │       │       └── Thomas1D.h
│   │       │
│   │       ├── TwoDimensional/
│   │       │   ├── Grid/
│   │       │   │   ├── GridMetrics2D.h
│   │       │   │   └── StructuredGrid2DView.h
│   │       │   │
│   │       │   ├── Field/
│   │       │   │   ├── Field2D.h
│   │       │   │   ├── FieldView2D.h
│   │       │   │   └── InitialCondition2D.h
│   │       │   │
│   │       │   ├── Coefficient/
│   │       │   ├── Source/
│   │       │   ├── Boundary/
│   │       │   ├── Flux/
│   │       │   ├── Interpolation/
│   │       │   ├── Term/
│   │       │   ├── Equation/
│   │       │   ├── System/
│   │       │   ├── Assembly/
│   │       │   └── Solver/
│   │       │
│   │       ├── Time/
│   │       │   ├── BDF2.h
│   │       │   ├── CentralDifference2.h
│   │       │   ├── CrankNicolson.h
│   │       │   ├── ExplicitEuler.h
│   │       │   ├── ImplicitEuler.h
│   │       │   ├── Newmark.h
│   │       │   ├── TimeSchemeTraits.h
│   │       │   ├── TimeState.h
│   │       │   └── Transient.h
│   │       │
│   │       ├── Execution/
│   │       │   ├── Solve.h
│   │       │   ├── SolveResult.h
│   │       │   ├── SteadyState.h
│   │       │   └── SolveController.h
│   │       │
│   │       └── Output/
│   │           ├── CSVField1DWriter.h
│   │           ├── CSVField2DWriter.h
│   │           └── VTKField2DWriter.h
│   │
│   └── src/
│       └── FVMaker/
│           ├── Core/
│           ├── Error/
│           ├── OneDimensional/
│           ├── TwoDimensional/
│           ├── Time/
│           ├── Execution/
│           └── Output/
│
├── tests/
│   ├── Core/
│   ├── Error/
│   ├── Common/
│   ├── OneDimensional/
│   ├── TwoDimensional/
│   ├── Time/
│   ├── Execution/
│   └── Output/
│
├── examples/
│   ├── ex_Minimal.cpp
│   ├── ex_ErrorHandling.cpp
│   ├── OneDimensional/
│   └── TwoDimensional/
│
├── capitulos/
│   └── <exemplos do livro>
│
├── benchmarks/
│   └── <programas bm_*.cpp>
│
└── profiling/
    └── <programas prof_*.cpp>
```

## 3. Core

`Core/Types.h`

Define aliases fundamentais:

```text
Real
Index
Size
Int8/16/32/64
UInt8/16/32/64
```

`Core/StrongTypes.h`

Define tipos fortes para parâmetros recorrentes da FVM:

```text
Tolerance
MaxIterations
Time
TimeStep
FinalTime
InitialTime
RelaxationFactor
```

Esses tipos devem evitar troca acidental de argumentos em APIs públicas e em
controles de solução.

`Core/ID.h`

Define identidade imutável de classe/componente:

```text
module
class_name
class_id
```

Regra:

```text
Toda classe da FVM deve ter identidade própria quando puder ser origem de erro.
Nem toda função precisa de identidade própria.
Função/método específico é rastreado por std::source_location.
```

Exemplos:

```text
fvm.1d.field.Field1D
fvm.1d.term.Laplacian1D
fvm.1d.system.TridiagonalSystem1D
fvm.time.Transient
```

## 4. Error

O sistema de erros usa códigos e descritores textuais, sem `enum` central.

Componentes:

```text
ErrorCodes.h        -> códigos textuais estáveis
ErrorDescriptor.h   -> code + message + category
ErrorCatalog.h      -> catálogo interno de descritores FVM
ErrorRecord.h       -> registro completo do erro
FVMException.h      -> exceção com ErrorRecord
ThrowError.h        -> throw_error() e require()
```

Forma preferencial de uso interno:

```cpp
require(
    condition,
    error_catalog::kInvalidArgument,
    Field1D::id()
);
```

Forma explícita para erros externos ou mensagens contextuais:

```cpp
require(
    condition,
    "USER.CODE.INVALID_VALUE",
    "Context-specific diagnostic message.",
    "UserCategory",
    UserClass::id()
);
```

## 5. OneDimensional

`Grid/Axis1DView`

Adapta `fvgrid::Axis1D` para uso interno da FVM. Não gera malha.

Deve expor:

```text
num_cells();
num_faces();
faces();
centers();
dx_faces();
dx_centers();
cell_lengths();
```

`Field/Field1D`

Armazena valores escalares em volumes/células 1D.

Deve expor leitura por `std::span<const Real>` e controlar histórico temporal
quando necessário.

`System/TridiagonalSystem1D`

Representa sistemas tridiagonais oriundos dos primeiros problemas 1D.

Deve permitir calcular:

```text
erro = A X - B
```

e normas desse erro.

`Term/Laplacian1D`

Monta contribuições para:

```text
d2phi/dx2 = f(x)
d/dx(gamma(x) dphi/dx) = f(x)
```

`Boundary`

Contém condições de contorno e estratégias de aplicação:

```text
Dirichlet1D
Neumann1D
Robin1D
GhostBoundary1D
```

## 6. TwoDimensional

`Grid/StructuredGrid2DView`

Adapta `fvgrid::StructuredGrid2D` para uso interno da FVM. Não gera malha.

Deve expor:

```text
num_cells_x();
num_cells_y();
linear_cell_index(i, j);
x_center(i);
y_center(j);
x_cell_length(i);
y_cell_length(j);
cell_area(i, j);
```

Os demais módulos 2D repetem a organização 1D, com implementações próprias
para campos, contornos, termos, sistemas, montagem e solvers 2D.

## 7. Time e Execution

`Time/`

Contém esquemas temporais e traits:

```text
ExplicitEuler
ImplicitEuler
CrankNicolson
BDF2
CentralDifference2
Newmark
TimeSchemeTraits
```

Novos esquemas devem poder ser adicionados por tipos externos e traits/concepts,
sem herança e sem enum central.

`Execution/`

Contém o controle do processo de solução:

```text
SteadyState
Transient
SolveController
SolveResult
solve()
```

A equação diz o que resolver. O controle de solução diz como resolver.

## 8. Output, Examples, Tests e Livro

`Output/`

É periférico. Lê campos/sistemas/resultados e escreve arquivos. Não modifica
estado.

`examples/`

Contém exemplos técnicos pequenos e executáveis.

`capitulos/`

Contém programas e exercícios ligados ao texto do livro.

`tests/`

Contém testes GoogleTest. Cada componente público novo deve ter teste.

`benchmarks/` e `profiling/`

Contêm programas específicos para análise de desempenho. Ferramentas como
`perf`, `valgrind`, `callgrind` e `gprof` não são dependências da FVMakerLib.

## 9. Convenções de programação

Source files devem usar o cabeçalho padrão:

```cpp
// ============================================================================
// Arquivo: <FileName>
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: <descrição curta em português>
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
```

Include sections devem seguir esta ordem:

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

