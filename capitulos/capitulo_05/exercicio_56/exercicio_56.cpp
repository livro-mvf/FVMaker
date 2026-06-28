// ============================================================================
// Arquivo: exercicio_56.cpp
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Implementa exercicio 56 no contexto de capitulos / capitulo_05 / exercicio_56.
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
//     std::cout << std::fixed << std::setprecision(12);
//     std::cout << "Exercicio 5.6 - regressao do termo fonte\n\n";

//     const Real x_inicial = -1.0;
//     const Real comprimento = 2.0;

//     const fvgrid::Axis1D nao_uniforme =
//         capitulo_05::malha_volume_centrada_customizada(
//             capitulo_05::faces_potencia(8, comprimento, x_inicial, 1.8),
//             x_inicial,
//             comprimento
//         );

//     const fvgrid::Axis1D uniforme = fvgrid::Uniform1D::make(
//         fvgrid::NVol{8},
//         fvgrid::Length{comprimento},
//         fvgrid::XInit{x_inicial},
//         fvgrid::VolumeCentered1D{}
//     );

//     const Real c = 3.0;
//     const Real a = -2.0;
//     const Real b = 5.0;

//     unsigned aprovados{};
//     unsigned total{};

//     std::cout << "Malha nao uniforme volume centrada\n";
//     std::cout << "==================================\n";
//     std::cout << nao_uniforme << "\n\n";

//     ++total;
//     aprovados += testar_fonte(
//         "Fonte constante: f(x) = c",
//         nao_uniforme,
//         [c](Real) { return c; },
//         [c](Real xw, Real xe) { return c * (xe - xw); }
//     );

//     ++total;
//     aprovados += testar_fonte(
//         "Fonte linear: f(x) = ax + b",
//         nao_uniforme,
//         [a, b](Real x) { return a * x + b; },
//         [a, b](Real xw, Real xe) {
//             return Real{0.5} * a * (xe * xe - xw * xw) + b * (xe - xw);
//         }
//     );

//     std::cout << "Repeticao em malha uniforme\n";
//     std::cout << "===========================\n";

//     ++total;
//     aprovados += testar_fonte(
//         "Fonte constante, malha uniforme",
//         uniforme,
//         [c](Real) { return c; },
//         [c](Real xw, Real xe) { return c * (xe - xw); }
//     );

//     ++total;
//     aprovados += testar_fonte(
//         "Fonte linear, malha uniforme",
//         uniforme,
//         [a, b](Real x) { return a * x + b; },
//         [a, b](Real xw, Real xe) {
//             return Real{0.5} * a * (xe * xe - xw * xw) + b * (xe - xw);
//         }
//     );

//     return aprovados == total ? 0 : 1;
}
