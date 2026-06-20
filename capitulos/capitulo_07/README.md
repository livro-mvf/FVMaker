# Capitulo 7 - Sobre Erros Numericos

Implementacoes dos cinco exercicios computacionais da referencia do capitulo.

- 7.1: regra do trapezio composta; autocontido.
- 7.2: acumulacao de 0.1 em float, double e long double; autocontido.
- 7.3: FVGridMaker para malhas uniforme/customizada e FVMaker para equacao, contornos, montagem, residuo e RMS.
- 7.4: FVGridMaker e FVMaker para o teste de frequencias n=1,2,4,8.
- 7.5: solucao manufaturada montada e verificada com FVGridMaker/FVMaker.

Nos testes de ordem de 7.3--7.5, a norma usada para estimar a ordem e calculada
nos volumes internos. As equacoes de fronteira devem ser verificadas
separadamente, como recomenda o proprio capitulo; inclui-las na mesma RMS faz
a aproximacao de contorno dominar a ordem local.
