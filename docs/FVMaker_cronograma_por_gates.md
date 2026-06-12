# FVMaker - Cronograma por Gates

Este documento define o cronograma técnico da nova FVMaker, daqui em diante
chamada de FVM.

O avanço deve ocorrer por gates de qualidade, não por calendário.

Regra geral:

```text
Só avançamos para o próximo bloco quando o bloco anterior estiver compilando,
testado com GoogleTest, executável por CTest, documentado minimamente e com
exemplos funcionais quando a funcionalidade for pública.
```

## 1. Regras Formais de Avanço

```text
1. Nenhum bloco novo começa com build quebrado.
2. Nenhum bloco novo começa com teste quebrado.
3. Nenhum bloco novo começa com exemplo público quebrado.
4. Nenhum bloco novo começa com documentação essencial ausente.
5. Nenhuma funcionalidade pública entra sem exemplo.
6. Nenhum componente público entra sem teste.
7. Nenhum erro esperado entra sem código textual testado.
8. Nenhuma montagem numérica entra sem teste de coeficientes.
9. Nenhum solver entra sem caso com solução conhecida.
10. Nenhum termo diferencial entra sem teste de montagem.
11. Nenhuma condição de contorno entra sem teste isolado.
12. Nenhum esquema temporal entra sem teste de evolução simples.
13. Nenhum bloco de advecção entra sem teste de interpolação em faces.
14. Nenhum bloco não linear entra sem critério de convergência testado.
15. Nenhum output entra sem arquivo de referência simples.
16. Nenhum módulo periférico pode criar dependência reversa no núcleo.
17. Nenhuma categoria extensível deve ser implementada com enum central.
18. Toda classe que possa ser origem de erro deve expor ID próprio.
19. require() deve receber ErrorDescriptor + ID sempre que possível.
20. A FVMakerLib não deve depender de YAML, GoogleTest ou ferramentas de profiling.
21. Todo bloco ligado a conteúdo do livro deve produzir ou atualizar exercícios,
    resultados, tabelas ou figuras correspondentes quando aplicável.
22. Resultado destinado ao livro deve ser reprodutível e protegido por teste de
    regressão quando passar a ser publicado ou citado no texto.
```

Observações:

```text
GoogleTest é permitido apenas nos testes.
YAML é permitido apenas em exemplos ou aplicações externas.
perf, valgrind, callgrind, gprof e sanitizers são ferramentas de desenvolvimento.
A geometria de malha pertence à FVGridMaker, não à FVM.
```

## 2. Status Resumido

```text
Bloco 0  - Fundação do projeto e CMake                         CONCLUÍDO
Bloco 1  - Core e tipos fundamentais                           CONCLUÍDO
Bloco 2  - ErrorHandling adaptado da FVGridMaker                CONCLUÍDO
Bloco 3  - Integração mínima com FVGridMaker 1D                 CONCLUÍDO
Bloco 4  - Field1D e condições iniciais                         CONCLUÍDO
Bloco 5  - LinearSystem1D tridiagonal e TDMA                    CONCLUÍDO
Bloco 6  - Resíduo algébrico e normas de erro                   CONCLUÍDO
Bloco 6.1 - Funções escalares/vetoriais e suporte a MMS         CONCLUÍDO
Bloco 7  - Boundary1D e ghost volumes                           CONCLUÍDO
Bloco 8  - Termo fonte 1D e fontes linearizadas                 CONCLUÍDO
Bloco 9  - Laplacian1D: d2phi/dx2 = f(x)                        CONCLUÍDO
Bloco 10 - Solvers iterativos lineares 1D                       CONCLUÍDO
Bloco 11 - Coeficiente variável linear 1D                       CONCLUÍDO
Bloco 12 - Controle de solução e SolveResult                    PENDENTE
Bloco 13 - Problemas não lineares 1D                            PENDENTE
Bloco 14 - Fluxos, interpolação e advecção 1D                   PENDENTE
Bloco 15 - Controle transiente 1D e ddt                         PENDENTE
Bloco 16 - Esquemas temporais de primeira ordem 1D              PENDENTE
Bloco 17 - d2dt2 e equação da onda 1D                           PENDENTE
Bloco 18 - Integração mínima com FVGridMaker 2D                 PENDENTE
Bloco 19 - Field2D e Boundary2D                                 PENDENTE
Bloco 20 - LinearSystem2D esparso estruturado                   PENDENTE
Bloco 21 - Laplacian2D linear em regime permanente              PENDENTE
Bloco 22 - Solvers iterativos 2D                                PENDENTE
Bloco 23 - Problemas não lineares 2D                            PENDENTE
Bloco 24 - Controle transiente 2D                               PENDENTE
Bloco 25 - Equação da onda 2D                                   PENDENTE
Bloco 26 - Fluxos, interpolação e advecção 2D                   PENDENTE
Bloco 27 - Output de campos                                     PENDENTE
Bloco 28 - Exemplos externos com YAML                           PENDENTE
Bloco 29 - Performance/profiling                                PENDENTE
```

## 3. Blocos

## Bloco 0 - Fundação do Projeto e CMake

Objetivo: preparar a base da nova FVM sem ainda implementar física.

Entregas:

```text
estrutura nova de diretórios;
CMake modular;
opções BUILD_TESTS, BUILD_EXAMPLES, BUILD_BOOK e BUILD_PERFORMANCE;
integração futura com FVGridMaker preparada;
exemplo mínimo;
teste mínimo;
documentos FVMaker_requisitos.md e FVMaker_cronograma_por_gates.md.
```

Gate de saída:

```text
cmake -S . -B build passa;
cmake --build build passa;
ctest --test-dir build --output-on-failure passa quando BUILD_TESTS=ON;
run_ex_Minimal passa quando BUILD_EXAMPLES=ON.
docs passa quando BUILD_DOCUMENTATION=ON e Sphinx está disponível.
```

Status: concluído.

## Bloco 1 - Core e Tipos Fundamentais

Objetivo: criar a fundação tipada da FVM.

Entregas:

```text
Types.h;
StrongTypes.h;
Version.h;
ID.h;
Version.cc;
ID.cc;
tipos para Size, Index, Real;
tipos fortes iniciais para tolerância, iteração, tempo e passo de tempo;
testes de Core.
```

Gate de saída:

```text
run_tst_Types passa;
run_tst_StrongTypes passa;
run_tst_ID passa;
run_tst_Version passa;
ctest --output-on-failure passa.
```

Status: concluído.

Verificação executada:

```text
cmake -S . -B build;
cmake --build build -j2;
cmake -S . -B /tmp/fvmaker-codex-tests -DBUILD_BOOK=OFF -DBUILD_TESTS=ON -DFVM_TESTS_FETCH_GOOGLETEST=OFF;
cmake --build /tmp/fvmaker-codex-tests --target run_all_tests -j2;
ctest --test-dir /tmp/fvmaker-codex-tests --output-on-failure.
```

## Bloco 2 - ErrorHandling Adaptado da FVGridMaker

Objetivo: adaptar o sistema de erros da FVGridMaker para a FVM.

Entregas:

```text
ErrorCodes.h;
ErrorDescriptor.h;
ErrorCatalog.h;
ErrorRecord.h;
FVMException.h/.cc;
ThrowError.h/.cc;
exemplo Ex_ErrorHandling;
testes de ErrorCodes, ErrorCatalog, ErrorRecord, FVMException e ThrowError.
```

Decisões:

```text
códigos de erro são textuais;
não há enum central de erros;
não há exit(), abort() ou std::cerr como mecanismo de erro da biblioteca;
require() deve preferir ErrorDescriptor + ID;
erros externos podem usar descritores próprios.
```

Gate de saída:

```text
run_tst_ErrorHandling_ErrorCodes passa;
run_tst_ErrorHandling_ErrorCatalog passa;
run_tst_ErrorHandling_ErrorRecord passa;
run_tst_ErrorHandling_FVMException passa;
run_tst_ErrorHandling_ThrowError passa;
run_ex_ErrorHandling passa;
ctest --output-on-failure passa.
```

Status: concluído.

Verificação executada:

```text
cmake -S . -B /tmp/fvmaker-codex-tests -DBUILD_BOOK=OFF -DBUILD_TESTS=ON -DFVM_TESTS_FETCH_GOOGLETEST=OFF;
cmake --build /tmp/fvmaker-codex-tests --target run_all_tests -j2;
ctest --test-dir /tmp/fvmaker-codex-tests --output-on-failure;
cmake -S . -B /tmp/fvmaker-codex-examples -DBUILD_BOOK=OFF -DBUILD_EXAMPLES=ON;
cmake --build /tmp/fvmaker-codex-examples --target run_ex_ErrorHandling_ErrorHandling -j2;
cmake -S . -B build;
cmake --build build -j2.
```

## Bloco 3 - Integração Mínima com FVGridMaker 1D

Objetivo: consumir `fvgrid::Axis1D` sem gerar malhas dentro da FVM.

Entregas:

```text
Axis1DView ou GridView1D;
GridMetrics1D;
acesso a número de volumes, faces, centros, dx_faces e dx_centers;
validação de compatibilidade mínima;
exemplo consumindo fvgrid::Axis1D;
testes com malha uniforme e malha não uniforme.
```

Gate de saída:

```text
run_tst_Grid_GridView1D passa;
GridMetrics1D é testado junto com GridView1D;
run_ex_Grid_GridView1D passa;
ctest --output-on-failure passa.
```

Status: concluído.

Verificação executada:

```text
cmake -S . -B /tmp/fvmaker-codex-tests -DBUILD_BOOK=OFF -DBUILD_TESTS=ON -DFVM_TESTS_FETCH_GOOGLETEST=OFF;
cmake --build /tmp/fvmaker-codex-tests --target run_tst_Grid_GridView1D -j2;
cmake --build /tmp/fvmaker-codex-tests --target run_all_tests -j2;
ctest --test-dir /tmp/fvmaker-codex-tests --output-on-failure;
cmake -S . -B /tmp/fvmaker-codex-examples -DBUILD_BOOK=OFF -DBUILD_EXAMPLES=ON;
cmake --build /tmp/fvmaker-codex-examples --target run_ex_Grid_GridView1D -j2;
cmake -S . -B build;
cmake --build build -j2.
```

## Bloco 4 - Field1D e Condições Iniciais

Objetivo: criar campos escalares 1D associados a uma malha da FVGridMaker.

Entregas:

```text
Field1D;
FieldView1D;
armazenamento contíguo de valores nos volumes;
nome do campo;
inicialização uniforme;
inicialização por função da posição;
inicialização por vetor;
histórico temporal mínimo quando solicitado;
testes de tamanho, acesso e inicialização.
```

Gate de saída:

```text
run_tst_Field_Field1D passa;
run_ex_Field_Field1D passa;
ctest --output-on-failure passa.
```

Status: concluído.

Verificação executada:

```text
cmake -S . -B /tmp/fvmaker-codex-tests -DBUILD_BOOK=OFF -DBUILD_TESTS=ON -DFVM_TESTS_FETCH_GOOGLETEST=OFF;
cmake --build /tmp/fvmaker-codex-tests --target run_tst_Field_Field1D -j2;
cmake --build /tmp/fvmaker-codex-tests --target run_all_tests -j2;
ctest --test-dir /tmp/fvmaker-codex-tests --output-on-failure;
cmake -S . -B /tmp/fvmaker-codex-examples -DBUILD_BOOK=OFF -DBUILD_EXAMPLES=ON;
cmake --build /tmp/fvmaker-codex-examples --target run_ex_Field_Field1D -j2;
cmake -S . -B build;
cmake --build build -j2.
```

## Bloco 5 - LinearSystem1D Tridiagonal e TDMA

Objetivo: criar a infraestrutura algébrica mínima para o primeiro problema 1D.

Entregas:

```text
TridiagonalSystem1D;
DenseVector;
TDMA;
SolveResult mínimo para solver linear;
testes com sistemas conhecidos;
teste de erro para pivô inválido ou dimensão incompatível.
```

Gate de saída:

```text
run_tst_System_TridiagonalSystem1D passa;
run_tst_Solver_TDMA passa;
run_ex_Solver_TDMA passa;
ctest --output-on-failure passa.
```

Status: concluído.

Verificação executada:

```text
cmake -S . -B /tmp/fvmaker-codex-tests -DBUILD_BOOK=OFF -DBUILD_TESTS=ON -DFVM_TESTS_FETCH_GOOGLETEST=OFF;
cmake --build /tmp/fvmaker-codex-tests --target run_tst_System_TridiagonalSystem1D run_tst_Solver_TDMA -j2;
cmake --build /tmp/fvmaker-codex-tests --target run_all_tests -j2;
ctest --test-dir /tmp/fvmaker-codex-tests --output-on-failure;
cmake -S . -B /tmp/fvmaker-codex-examples -DBUILD_BOOK=OFF -DBUILD_EXAMPLES=ON;
cmake --build /tmp/fvmaker-codex-examples --target run_ex_Solver_TDMA -j2;
cmake -S . -B build;
cmake --build build -j2.
```

## Bloco 6 - Resíduo Algébrico e Normas de Erro

Objetivo: calcular o erro/resíduo algébrico dos sistemas montados.

Definição:

```text
erro = A X - B
```

O erro de truncamento usado nos testes numéricos deve ser medido pela norma
desse erro.

Entregas:

```text
função para calcular A X - B em sistemas 1D;
norma infinito;
norma L2 discreta;
norma L1 discreta quando útil;
integração com SolveResult;
testes com sistemas pequenos conhecidos;
exemplo simples exibindo a norma do erro.
```

Gate de saída:

```text
run_tst_System_AlgebraicResidual1D passa;
run_tst_Algebra_ErrorNorms passa;
run_ex_System_ResidualNorm1D passa;
ctest --output-on-failure passa.
```

Status: concluído.

Verificação executada:

```text
cmake -S . -B /tmp/fvmaker-codex-tests -DBUILD_BOOK=OFF -DBUILD_TESTS=ON -DFVM_TESTS_FETCH_GOOGLETEST=OFF;
cmake --build /tmp/fvmaker-codex-tests --target run_tst_Algebra_ErrorNorms run_tst_System_AlgebraicResidual1D run_tst_Solver_TDMA -j2;
cmake --build /tmp/fvmaker-codex-tests --target run_all_tests -j2;
ctest --test-dir /tmp/fvmaker-codex-tests --output-on-failure;
cmake -S . -B /tmp/fvmaker-codex-examples -DBUILD_BOOK=OFF -DBUILD_EXAMPLES=ON;
cmake --build /tmp/fvmaker-codex-examples --target run_ex_System_ResidualNorm1D -j2;
cmake -S . -B build;
cmake --build build -j2.
```

## Bloco 6.1 - Funções Escalares/Vetoriais e Suporte a MMS

Objetivo: criar a infraestrutura comum para transportar funções analíticas,
coeficientes variáveis, soluções manufaturadas, fontes e condições de contorno
por toda a FVM.

Motivação:

```text
O método das funções manufaturadas exige que solução exata, derivadas,
coeficientes variáveis e termos fonte sejam avaliáveis de forma consistente em
centros, faces, fronteiras e instantes de tempo.
```

Entregas:

```text
conceito ScalarFunction1D para funções Real(Real);
conceito ScalarFunction2D para funções Real(Real, Real);
conceito VectorFunction1D para funções vetoriais em 1D;
conceito VectorFunction2D para funções vetoriais em 2D;
tipos armazenáveis StoredScalarFunction1D e StoredScalarFunction2D;
tipos armazenáveis StoredVectorFunction1D e StoredVectorFunction2D;
helpers para avaliar função escalar nos centros de uma GridView1D;
helpers para avaliar função escalar nas faces de uma GridView1D;
estrutura inicial ManufacturedSolution1D;
estrutura inicial VariableCoefficient1D;
estrutura inicial ManufacturedProblem1D com múltiplos coeficientes nomeados;
testes com função livre, lambda, functor e std::function;
exemplo simples de MMS 1D apenas avaliando phi, dphi/dx, d2phi/dx2 e f(x).
```

Decisões:

```text
ScalarFunction1D e ScalarFunction2D devem ser concepts para uso eficiente em
código template;
os tipos Stored* devem usar std::function apenas quando a função precisar ser
armazenada;
VectorFunction1D existe mesmo que, em vários problemas 1D, o vetor tenha apenas
uma componente efetiva;
MMS deve permitir vários coeficientes nomeados, pois cada equação diferencial
pode exigir um conjunto diferente de propriedades e derivadas;
nenhum parser externo, YAML ou biblioteca simbólica entra na FVMakerLib;
derivadas manufaturadas serão fornecidas pelo usuário ou pelos exemplos, não
calculadas simbolicamente pela biblioteca.
```

Gate de saída:

```text
run_tst_Functions_Functions1D passa;
run_tst_Functions_Functions2D passa;
run_tst_Functions_ManufacturedSolution1D passa;
run_ex_Functions_ManufacturedFunctions1D passa;
ctest --output-on-failure passa.
```

Status: concluído.

Verificação executada:

```text
cmake -S . -B /tmp/fvmaker-codex-tests -DBUILD_BOOK=OFF -DBUILD_TESTS=ON -DFVM_TESTS_FETCH_GOOGLETEST=OFF;
cmake --build /tmp/fvmaker-codex-tests --target run_tst_Functions_Functions1D run_tst_Functions_Functions2D run_tst_Functions_ManufacturedSolution1D -j2;
cmake --build /tmp/fvmaker-codex-tests --target run_all_tests -j2;
ctest --test-dir /tmp/fvmaker-codex-tests --output-on-failure;
cmake -S . -B /tmp/fvmaker-codex-examples -DBUILD_BOOK=OFF -DBUILD_EXAMPLES=ON;
cmake --build /tmp/fvmaker-codex-examples --target run_ex_Functions_ManufacturedFunctions1D -j2;
cmake -S . -B build;
cmake --build build -j2.
```

## Bloco 7 - Boundary1D e Ghost Volumes

Objetivo: implementar condições de contorno 1D e as estratégias básicas de aplicação.

Entregas:

```text
BoundarySide1D;
BoundaryCondition1D na forma alpha phi + beta phi' = gamma;
atalhos Dirichlet, Neumann e Robin como casos particulares;
BoundarySet1D;
GhostCells1D com ate 2 ghost cells por lado, fora da opcao default;
PentadiagonalSystem1D para esquemas 1D com acoplamento ate i+-2;
estratégia por ghost volumes;
estratégia por modificação dos coeficientes;
testes isolados de cada condição de contorno;
exemplo de comparação entre estratégias quando aplicável.
```

Gate de saída:

```text
run_tst_Boundary_GhostCells1D passa;
run_tst_System_PentadiagonalSystem1D passa;
run_tst_Boundary1D passa;
run_tst_GhostBoundary1D passa;
run_ex_System_PentadiagonalAndGhostCells1D passa;
run_ex_Boundary1D passa;
ctest --output-on-failure passa.
```

Status: concluído.

Progresso concluido:

```text
GhostCells1D representa 0, 1 ou 2 ghost cells por lado;
o default de GhostCells1D e zero ghost cells;
BoundaryCondition1D representa a forma geral alpha phi + beta phi' = gamma;
BoundarySet1D reserva espaco explicito para lado esquerdo e direito;
PentadiagonalSystem1D foi criado como alternativa explicita ao sistema
tridiagonal, sem substituir TridiagonalSystem1D + TDMA como caminho default;
algebraic_residual calcula A X - B para sistemas tridiagonais e
pentadiagonais.
GhostBoundary1D fornece a linearizacao do primeiro ghost cell na forma
phi_g = c + m phi_P, que permite modificar coeficientes e lado direito em
montagens futuras.
ex_Poisson1DCoefficients mostra os coeficientes de d2phi/dx2 = f(x) em uma
malha 1D criada pela FVGridMaker, usando BoundarySet1D e ghost cells.
```

Verificação executada:

```text
cmake -S . -B /tmp/fvmaker-codex-tests -DBUILD_BOOK=OFF -DBUILD_TESTS=ON -DFVM_TESTS_FETCH_GOOGLETEST=OFF;
cmake --build /tmp/fvmaker-codex-tests --target run_tst_Boundary_Boundary1D run_tst_Boundary_GhostBoundary1D run_tst_Boundary_GhostCells1D run_tst_System_PentadiagonalSystem1D -j2;
cmake --build /tmp/fvmaker-codex-tests --target run_all_tests -j2;
ctest --test-dir /tmp/fvmaker-codex-tests --output-on-failure;
cmake -S . -B /tmp/fvmaker-codex-examples -DBUILD_BOOK=OFF -DBUILD_EXAMPLES=ON;
cmake --build /tmp/fvmaker-codex-examples --target run_ex_Boundary_Boundary1D run_ex_System_PentadiagonalAndGhostCells1D run_ex_Equation_Poisson1DCoefficients -j2.
```

## Bloco 8 - Termo Fonte 1D e Fontes Linearizadas

Objetivo: representar fontes 1D independentes da equação.

Entregas:

```text
LinearizedSource1D com S = S_C + S_P phi;
fonte uniforme;
fonte por vetor;
fonte por função f(x);
fonte por função f(x,t);
S_P opcional, com default zero;
testes de avaliação em volumes;
testes de contribuição para vetor e diagonal.
```

Gate de saída:

```text
run_tst_Source1D passa;
run_tst_LinearizedSource1D passa;
run_ex_Source1D passa;
ctest --output-on-failure passa.
```

Status: concluído.

Decisões:

```text
O termo fonte 1D é representado por LinearizedSource1D;
cada volume possui dois coeficientes: S_C e S_P;
a avaliação local segue B_P = S_C + S_P phi_P;
S_P é opcional e seu default é zero;
fontes podem ser uniformes, vetoriais, f(x) ou f(x,t);
FunctionTypes já prevê ScalarFunction2DTime para fontes futuras f(x,y,t);
ao aplicar a fonte em A phi = b, S_C soma no lado direito e S_P é movido para
a diagonal com sinal negativo: a_P <- a_P - S_P.
```

Verificação executada:

```text
cmake -S . -B /tmp/fvmaker-codex-tests -DBUILD_BOOK=OFF -DBUILD_TESTS=ON -DFVM_TESTS_FETCH_GOOGLETEST=OFF;
cmake --build /tmp/fvmaker-codex-tests --target run_tst_Source_LinearizedSource1D run_tst_Functions_Functions1D run_tst_Functions_Functions2D -j2;
cmake --build /tmp/fvmaker-codex-tests --target run_all_tests -j2;
ctest --test-dir /tmp/fvmaker-codex-tests --output-on-failure;
cmake -S . -B /tmp/fvmaker-codex-examples -DBUILD_BOOK=OFF -DBUILD_EXAMPLES=ON;
cmake --build /tmp/fvmaker-codex-examples --target run_ex_Source_Source1D -j2.
```

## Bloco 9 - Laplacian1D: d2phi/dx2 = f(x)

Objetivo: resolver o primeiro problema-base do livro.

Problema:

```text
d2phi/dx2 = f(x)
```

Entregas:

```text
Laplacian1D para coeficiente constante;
Equation1D mínima;
Assembler1D;
aplicação de Boundary1D;
montagem de sistema tridiagonal;
exemplo didático do primeiro problema;
testes com solução analítica simples;
teste de conservação local/global quando aplicável.
medida da norma de A X - B.
```

Gate de saída:

```text
run_tst_Laplacian1D passa;
run_tst_Equation1D passa;
run_tst_Assembler1D passa;
run_ex_Poisson1D passa;
ctest --output-on-failure passa.
```

Status: concluído.

Decisões:

```text
Laplacian1D representa o operador difusivo 1D com coeficiente constante;
Equation1D agrupa GridView1D, Laplacian1D, LinearizedSource1D e BoundarySet1D;
Assembler1D monta o TridiagonalSystem1D para d2phi/dx2 = f(x);
as condições de contorno entram pela forma alpha phi + beta phi' = gamma;
ghost cells são linearizados como phi_g = c + m phi_P;
fontes linearizadas entram como rhs += S_C e diagonal -= S_P;
TDMA resolve o sistema montado;
SolveResult fornece residual A X - B e norma infinito.
```

Verificação executada:

```text
cmake -S . -B /tmp/fvmaker-codex-tests -DBUILD_BOOK=OFF -DBUILD_TESTS=ON -DFVM_TESTS_FETCH_GOOGLETEST=OFF;
cmake --build /tmp/fvmaker-codex-tests --target run_tst_Operator_Laplacian1D run_tst_Equation_Equation1D run_tst_Assembly_Assembler1D -j2;
cmake --build /tmp/fvmaker-codex-tests --target run_all_tests -j2;
ctest --test-dir /tmp/fvmaker-codex-tests --output-on-failure;
cmake -S . -B /tmp/fvmaker-codex-examples -DBUILD_BOOK=OFF -DBUILD_EXAMPLES=ON;
cmake --build /tmp/fvmaker-codex-examples --target run_ex_Equation_Poisson1DCoefficients run_ex_Equation_Poisson1D -j2.
```

## Bloco 10 - Solvers Iterativos Lineares 1D

Objetivo: comparar solvers no problema-base 1D.

Entregas:

```text
Jacobi;
GaussSeidel;
ConjugateGradient quando aplicável;
controle de tolerância;
controle de máximo de iterações;
relato de convergência;
testes com sistemas conhecidos;
exemplo comparando TDMA e solvers iterativos.
```

Gate de saída:

```text
run_tst_Jacobi passa;
run_tst_GaussSeidel passa;
run_tst_ConjugateGradient passa quando aplicável;
run_ex_LinearSolvers1D passa;
ctest --output-on-failure passa.
```

Status: concluído.

Decisões:

```text
Jacobi, GaussSeidel e ConjugateGradient resolvem TridiagonalSystem1D;
IterativeSolverOptions centraliza tolerância e máximo de iterações;
falha por não convergência retorna SolveResult com converged=false;
erros de configuração, diagonal nula ou denominador singular lançam FVMException;
TDMA permanece o solver direto/default para sistemas tridiagonais;
ConjugateGradient é usado para sistemas SPD quando aplicável.
```

Verificação executada:

```text
cmake -S . -B /tmp/fvmaker-codex-tests -DBUILD_BOOK=OFF -DBUILD_TESTS=ON -DFVM_TESTS_FETCH_GOOGLETEST=OFF;
cmake --build /tmp/fvmaker-codex-tests --target run_tst_Solver_IterativeSolvers1D -j2;
cmake --build /tmp/fvmaker-codex-tests --target run_all_tests -j2;
ctest --test-dir /tmp/fvmaker-codex-tests --output-on-failure;
cmake -S . -B /tmp/fvmaker-codex-examples -DBUILD_BOOK=OFF -DBUILD_EXAMPLES=ON;
cmake --build /tmp/fvmaker-codex-examples --target run_ex_Solver_LinearSolvers1D -j2;
cmake -S . -B build;
cmake --build build -j2.
```

## Bloco 11 - Coeficiente Variável Linear 1D

Objetivo: resolver o segundo problema-base do livro.

Problema:

```text
d/dx(gamma(x) dphi/dx) = f(x)
```

Entregas:

```text
Coefficient1D;
UniformCoefficient1D;
FunctionCoefficient1D;
FieldCoefficient1D;
interpolação de coeficientes nas faces;
Laplacian1D com coeficiente variável;
testes com soluções manufaturadas;
teste de ordem de convergência quando aplicável.
```

Gate de saída:

```text
run_tst_Coefficient1D passa;
run_tst_FaceCoefficientInterpolation1D passa;
run_tst_VariableLaplacian1D passa;
run_ex_VariableCoefficient1D passa;
ctest --output-on-failure passa.
```

Status: concluído.

Decisões:

```text
o coeficiente difusivo 1D é armazenado nas faces dos volumes de controle;
DiffusionCoefficient1D representa os valores de gamma nas faces;
uniform_coefficient_1d cria coeficiente constante em todas as faces;
function_coefficient_1d avalia gamma(x) diretamente nas faces da GridView1D;
field_coefficient_1d interpola valores de centros para faces por média
aritmética como default;
harmonic_field_coefficient_1d fica disponível para casos com salto de
coeficiente;
Laplacian1D continua aceitando coeficiente constante e passa a aceitar
DiffusionCoefficient1D para montar d/dx(gamma(x) dphi/dx);
Assembler1D usa gamma_w e gamma_e por face, preservando o sistema
tridiagonal.
```

Verificação executada:

```text
cmake -S . -B /tmp/fvmaker-codex-tests -DBUILD_BOOK=OFF -DBUILD_TESTS=ON -DFVM_TESTS_FETCH_GOOGLETEST=OFF;
cmake --build /tmp/fvmaker-codex-tests --target run_tst_Coefficient_DiffusionCoefficient1D run_tst_Operator_Laplacian1D run_tst_Assembly_Assembler1D -j2;
cmake --build /tmp/fvmaker-codex-tests --target run_all_tests -j2;
ctest --test-dir /tmp/fvmaker-codex-tests --output-on-failure;
cmake -S . -B /tmp/fvmaker-codex-examples -DBUILD_BOOK=OFF -DBUILD_EXAMPLES=ON;
cmake --build /tmp/fvmaker-codex-examples --target run_ex_Equation_VariableCoefficient1D -j2;
cmake -S . -B build;
cmake --build build -j2.
```

## Bloco 12 - Controle de Solução e SolveResult

Objetivo: separar equação, solver e controle do processo de solução.

Entregas:

```text
SteadyState;
SolveResult completo;
SolveController ou função solve();
critérios de convergência absolutos e relativos;
norma do resíduo;
norma de A X - B;
norma da correção;
falha por não convergência;
exemplo de uso público simplificado.
```

Gate de saída:

```text
run_tst_SteadyState passa;
run_tst_SolveResult passa;
run_tst_SolveController passa;
run_ex_SolveSteady1D passa;
ctest --output-on-failure passa.
```

Status: pendente.

## Bloco 13 - Problemas Não Lineares 1D

Objetivo: resolver problemas 1D com coeficientes variáveis e não lineares.

Entregas:

```text
NonlinearCoefficient1D;
PicardIteration;
Newton ou Newton aproximado se adequado;
relaxação de campos;
relaxação de equações;
controle de convergência não linear;
fontes linearizadas em problema não linear;
exemplo didático não linear 1D.
```

Gate de saída:

```text
run_tst_NonlinearCoefficient1D passa;
run_tst_PicardIteration passa;
run_tst_UnderRelaxation passa;
run_ex_Nonlinear1D passa;
ctest --output-on-failure passa.
```

Status: pendente.

## Bloco 14 - Fluxos, Interpolação e Advecção 1D

Objetivo: implementar problemas 1D com advecção e comparar funções de interpolação.

Entregas:

```text
FaceFlux1D;
MassFlux1D;
Interpolation1D;
Upwind1D;
CentralDifference1D;
PowerLaw1D;
Exponential1D;
QUICK1D quando adequado;
Advection1D;
testes de valores de face;
testes de número de Peclet;
exemplo comparando esquemas.
```

Gate de saída:

```text
run_tst_FaceFlux1D passa;
run_tst_Interpolation1D passa;
run_tst_Advection1D passa;
run_ex_Advection1D passa;
ctest --output-on-failure passa.
```

Status: pendente.

## Bloco 15 - Controle Transiente 1D e ddt

Objetivo: implementar problemas 1D transientes de primeira ordem no tempo.

Entregas:

```text
Transient;
TimeState1D;
TimeHistory1D;
Ddt1D;
condições iniciais transientes;
controle de passo de tempo;
CFL quando aplicável;
exemplo transiente 1D simples.
```

Gate de saída:

```text
run_tst_Transient passa;
run_tst_TimeState1D passa;
run_tst_Ddt1D passa;
run_ex_Transient1D passa;
ctest --output-on-failure passa.
```

Status: pendente.

## Bloco 16 - Esquemas Temporais de Primeira Ordem 1D

Objetivo: permitir escolher esquemas temporais de primeira ordem.

Entregas:

```text
TimeSchemeTraits;
ExplicitEuler;
ImplicitEuler;
CrankNicolson;
BDF2 se usado como esquema de primeira derivada;
compatibilidade entre esquema e equação;
testes de evolução com solução conhecida.
```

Gate de saída:

```text
run_tst_TimeSchemeTraits passa;
run_tst_ExplicitEuler passa;
run_tst_ImplicitEuler passa;
run_tst_CrankNicolson passa;
run_tst_BDF2 passa quando implementado;
run_ex_TimeSchemes1D passa;
ctest --output-on-failure passa.
```

Status: pendente.

## Bloco 17 - d2dt2 e Equação da Onda 1D

Objetivo: resolver a equação da onda 1D.

Entregas:

```text
D2dt2_1D;
TimeState1D com velocidade ou histórico suficiente;
CentralDifference2;
Newmark;
condição inicial de campo;
condição inicial de velocidade;
Wave1D ou equação composta equivalente;
testes com solução analítica;
exemplo de onda 1D.
```

Gate de saída:

```text
run_tst_D2dt2_1D passa;
run_tst_CentralDifference2 passa;
run_tst_Newmark passa;
run_tst_Wave1D passa;
run_ex_Wave1D passa;
ctest --output-on-failure passa.
```

Status: pendente.

## Bloco 18 - Integração Mínima com FVGridMaker 2D

Objetivo: consumir `fvgrid::StructuredGrid2D`.

Entregas:

```text
StructuredGrid2DView ou GridView2D;
GridMetrics2D;
indexação linear 2D;
acesso a centros, faces, dx, dy e áreas;
vizinhança estruturada;
testes com grid retangular simples.
```

Gate de saída:

```text
run_tst_GridView2D passa;
run_tst_GridMetrics2D passa;
run_ex_GridView2D passa;
ctest --output-on-failure passa.
```

Status: pendente.

## Bloco 19 - Field2D e Boundary2D

Objetivo: criar campos e condições de contorno 2D.

Entregas:

```text
Field2D;
FieldView2D;
BoundarySide2D;
BoundarySet2D;
Dirichlet2D;
Neumann2D;
Robin2D;
contornos left, right, bottom, top;
testes de indexação e aplicação de contorno.
```

Gate de saída:

```text
run_tst_Field2D passa;
run_tst_Boundary2D passa;
run_ex_FieldBoundary2D passa;
ctest --output-on-failure passa.
```

Status: pendente.

## Bloco 20 - LinearSystem2D Esparso Estruturado

Objetivo: representar sistemas 2D sem depender de bibliotecas externas.

Entregas:

```text
SparseStructuredMatrix2D;
Stencil5Point ou estrutura equivalente;
DenseVector;
montagem e acesso a coeficientes;
testes de dimensão e padrão de vizinhança.
```

Gate de saída:

```text
run_tst_SparseStructuredMatrix2D passa;
run_tst_Stencil5Point passa;
run_ex_LinearSystem2D passa;
ctest --output-on-failure passa.
```

Status: pendente.

## Bloco 21 - Laplacian2D Linear em Regime Permanente

Objetivo: resolver problemas fonte-difusão 2D lineares em regime permanente.

Entregas:

```text
Laplacian2D;
Source2D;
Equation2D;
Assembler2D;
aplicação de Boundary2D;
teste com solução manufaturada;
teste de conservação local/global quando aplicável;
medida da norma de A X - B;
exemplo didático 2D linear.
```

Gate de saída:

```text
run_tst_Laplacian2D passa;
run_tst_Assembler2D passa;
run_tst_Poisson2D passa;
run_ex_Poisson2D passa;
ctest --output-on-failure passa.
```

Status: pendente.

## Bloco 22 - Solvers Iterativos 2D

Objetivo: resolver sistemas 2D com métodos iterativos internos.

Entregas:

```text
Jacobi2D;
GaussSeidel2D;
ConjugateGradient2D quando aplicável;
controle de convergência;
testes com sistemas pequenos conhecidos;
exemplo comparativo.
```

Gate de saída:

```text
run_tst_Jacobi2D passa;
run_tst_GaussSeidel2D passa;
run_tst_ConjugateGradient2D passa quando aplicável;
run_ex_LinearSolvers2D passa;
ctest --output-on-failure passa.
```

Status: pendente.

## Bloco 23 - Problemas Não Lineares 2D

Objetivo: resolver problemas 2D em regime permanente não linear.

Entregas:

```text
Coefficient2D variável;
NonlinearCoefficient2D;
PicardIteration2D;
Newton aproximado se adequado;
relaxação 2D;
controle de convergência não linear;
exemplo didático não linear 2D.
```

Gate de saída:

```text
run_tst_NonlinearCoefficient2D passa;
run_tst_NonlinearSolve2D passa;
run_ex_Nonlinear2D passa;
ctest --output-on-failure passa.
```

Status: pendente.

## Bloco 24 - Controle Transiente 2D

Objetivo: resolver problemas 2D transientes de primeira ordem no tempo.

Entregas:

```text
TimeState2D;
TimeHistory2D;
Ddt2D;
condições iniciais 2D;
controle de passo de tempo;
exemplo transiente 2D simples.
```

Gate de saída:

```text
run_tst_TimeState2D passa;
run_tst_Ddt2D passa;
run_ex_Transient2D passa;
ctest --output-on-failure passa.
```

Status: pendente.

## Bloco 25 - Equação da Onda 2D

Objetivo: resolver a equação da onda 2D.

Entregas:

```text
D2dt2_2D;
TimeState2D com velocidade ou histórico suficiente;
CentralDifference2 em 2D;
Newmark em 2D;
Wave2D ou equação composta equivalente;
testes com solução manufaturada;
exemplo de onda 2D.
```

Gate de saída:

```text
run_tst_D2dt2_2D passa;
run_tst_Wave2D passa;
run_ex_Wave2D passa;
ctest --output-on-failure passa.
```

Status: pendente.

## Bloco 26 - Fluxos, Interpolação e Advecção 2D

Objetivo: implementar problemas 2D com advecção.

Entregas:

```text
FaceFlux2D;
MassFlux2D;
VelocityField2D;
Interpolation2D;
Upwind2D;
CentralDifference2D;
PowerLaw2D;
Exponential2D;
Advection2D;
testes de fluxos nas faces x e y;
testes de Peclet e CFL quando aplicável;
exemplo comparando esquemas 2D.
```

Gate de saída:

```text
run_tst_FaceFlux2D passa;
run_tst_Interpolation2D passa;
run_tst_Advection2D passa;
run_ex_Advection2D passa;
ctest --output-on-failure passa.
```

Status: pendente.

## Bloco 27 - Output de Campos

Objetivo: exportar resultados para inspeção e exemplos do livro.

Entregas:

```text
CSVField1DWriter;
CSVField2DWriter;
VTKField2DWriter quando a geometria da FVGridMaker permitir;
arquivos de referência pequenos;
exemplos de saída.
```

Gate de saída:

```text
run_tst_CSVField1DWriter passa;
run_tst_CSVField2DWriter passa;
run_tst_VTKField2DWriter passa quando implementado;
run_ex_OutputFields passa;
ctest --output-on-failure passa.
```

Status: pendente.

## Bloco 28 - Exemplos Externos com YAML

Objetivo: demonstrar configuração externa sem contaminar a FVMakerLib.

Entregas:

```text
exemplo YAML fora da FVMakerLib;
conversão de YAML para chamadas públicas da FVM;
documentação deixando claro que YAML não é dependência da biblioteca.
```

Gate de saída:

```text
run_ex_YamlCase passa quando dependência externa estiver disponível no ambiente;
build da FVMakerLib continua sem YAML;
ctest --output-on-failure passa.
```

Status: pendente.

## Bloco 29 - Performance/Profiling

Objetivo: medir desempenho sem transformar ferramentas em dependência da biblioteca.

Entregas:

```text
benchmarks para montagem 1D;
benchmarks para solver 1D;
benchmarks para montagem 2D;
benchmarks para solver 2D;
benchmarks para avanço temporal;
targets CMake para perf stat, callgrind e gprof quando disponíveis;
documento FVMaker_performance.md.
```

Gate de saída:

```text
BUILD_PERFORMANCE=ON configura sem erro;
run_all_performance passa;
perf/callgrind/gprof targets existem quando ferramentas estão disponíveis;
resultados básicos documentados.
```

Status: pendente.

## 4. Trilha Contínua - Exercícios e Resultados para o Livro

Objetivo: elaborar, testar e estabilizar exercícios e resultados do livro à
medida que a FVM avança.

Esta trilha não é um bloco final. Ela acompanha os blocos funcionais.

Sempre que um bloco implementar uma capacidade usada em capítulo do livro,
devem ser produzidos ou atualizados os materiais correspondentes.

Entregas:

```text
lista incremental de exercícios por capítulo;
programas de referência para cada exercício computacional já suportado;
soluções analíticas quando existirem;
soluções manufaturadas quando adequado;
arquivos de entrada externos quando usados;
resultados numéricos reprodutíveis;
tabelas em formato simples;
dados para figuras do livro;
testes de regressão para resultados publicados;
normas do erro A X - B para resultados selecionados;
documentação do procedimento usado para gerar cada resultado.
```

Regras:

```text
resultado usado no livro deve ser reprodutível;
exercício computacional deve ter caso de referência;
alteração futura da biblioteca não pode modificar silenciosamente resultado publicado;
tabelas e figuras devem indicar a versão/configuração usada;
quando houver solução analítica, o erro deve ser reportado;
quando houver estudo de convergência, a ordem observada deve ser registrada.
```

Gate contínuo:

```text
exercícios do bloco atual compilam;
exercícios do bloco atual executam;
resultados usados no texto são reproduzíveis;
testes de regressão dos resultados já publicados passam;
documentação de reprodução do capítulo afetado está atualizada.
```

Status: trilha contínua.
