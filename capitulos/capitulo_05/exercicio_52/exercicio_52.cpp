// ============================================================================
// Arquivo: exercicio_52.cpp
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Implementa exercicio 52 no contexto de capitulos / capitulo_05 / exercicio_52.
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

int main() {
//     using capitulo_05::Real;
//     using capitulo_05::Size;

//     std::cout << std::fixed << std::setprecision(12);

//     const Real x_inicial = 0.0;
//     const Real comprimento = 5.0;
//     const Size n = 5;

//     const fvgrid::Axis1D eixo =
//         capitulo_05::malha_face_centrada_customizada(
//             capitulo_05::centros_potencia(n, comprimento, x_inicial, 1.45),
//             x_inicial,
//             comprimento
//         );

//     const auto fonte = [](Real x) { return x * x; };

//     const fvm::EquationContribution1D coeficientes =
//         capitulo_05::montar_coeficientes_poisson(
//             eixo,
//             fonte,
//             fvm::dirichlet_1d(1.0),
//             fvm::neumann_1d(0.0)
//         );

//     std::cout << "Exercicio 5.2 - coeficientes em malha nao uniforme\n\n";
//     std::cout << "Malha gerada pela FVGridMaker\n";
//     std::cout << "=============================\n";
//     std::cout << eixo << "\n\n";

//     std::cout << "Coeficientes montados sem mudar a rotina do Exercicio 5.1\n";
//     std::cout << "=========================================================\n";
//     std::cout << coeficientes << "\n\n";

//     bool internos_assimetricos = true;
//     bool ap_soma_interna = true;

//     for (Size p = 1; p + 1 < n; ++p) {
//         internos_assimetricos =
//             internos_assimetricos &&
//             !capitulo_05::perto(coeficientes.aw()[p], coeficientes.ae()[p]);

//         ap_soma_interna =
//             ap_soma_interna &&
//             capitulo_05::perto(
//                 coeficientes.ap()[p],
//                 coeficientes.aw()[p] + coeficientes.ae()[p]
//             );
//     }

//     unsigned aprovados{};
//     unsigned total{};

//     ++total;
//     aprovados += capitulo_05::registrar(
//         "A_W != A_E nos volumes internos",
//         internos_assimetricos
//     );

//     ++total;
//     aprovados += capitulo_05::registrar(
//         "A_P = A_W + A_E nos volumes internos",
//         ap_soma_interna
//     );

//     std::cout << "\nTroca deliberada de delta x_w por delta x_e\n";
//     std::cout << "==========================================\n";

//     bool erro_detectado = false;
//     const auto xC = eixo.centers();

//     for (Size p = 1; p + 1 < n; ++p) {
//         const Real aw_errado = Real{1} / (xC[p + 1] - xC[p]);

//         if (!capitulo_05::perto(aw_errado, coeficientes.aw()[p])) {
//             erro_detectado = true;
//             std::cout << "P = " << p
//                       << ": A_W correto = " << coeficientes.aw()[p]
//                       << ", A_W com distancia trocada = " << aw_errado
//                       << '\n';
//         }
//     }

//     ++total;
//     aprovados += capitulo_05::registrar(
//         "a malha nao uniforme acusa a troca de distancia",
//         erro_detectado
//     );

//     return aprovados == total ? 0 : 1;
}
