# Capitulo 8 - Equacao Linear com Coeficiente Variavel

Exercicios computacionais da referencia do capitulo:

- 8.1 compara interpolacao aritmetica e harmonica ponderadas e demonstra uma politica criada pelo usuario;
- 8.2 verifica a implementacao com phi=sin(pi x), Gamma=1+x^2 e recupera segunda ordem;
- 8.3 resolve Gamma=1+C0 sin^2(n pi x/L) em malha FaceCentered1D e compara as medias;
- 8.4 resolve dois materiais como um unico campo, compara interface alinhada e volume atravessando a interface.

A FVMaker agora oferece interpolacao geometrica ponderada e a funcao
interpolated_field_coefficient_1d, que aceita qualquer politica compativel com
FaceInterpolationData1D.
