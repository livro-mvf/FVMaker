// //==============================================================================
// // SPDX-FileCopyrightText: 2026 FVMaker Team
// // SPDX-License-Identifier: MIT
// //==============================================================================
// // Exercicio Computacional 5.6
// // Fonte constante e fonte linear como testes de regressao
// //==============================================================================


// //==============================================================================
// // Header FVGridMaker
// //==============================================================================
// #include "../comum/mvf_capitulo_05.h"

// #include <FVGridMaker/OneDimensional/Distribution1D/Uniform1D.h>
// #include <FVGridMaker/OneDimensional/GridPattern1D/VolumeCentered1D.h>


// namespace {

// using Real = capitulo_05::Real;
// using Size = capitulo_05::Size;

// template <class Function, class Integral>
// bool testar_fonte(
//     std::string_view nome,
//     const fvgrid::Axis1D& eixo,
//     Function&& f,
//     Integral&& integral
// ) {
//     bool passou = true;

//     std::cout << nome << '\n';
//     std::cout << "P              B_P          integral          erro\n";

//     for (Size p = 0; p < eixo.num_cells(); ++p) {
//         const Real aproximado = capitulo_05::fonte_um_ponto(eixo, p, f);
//         const Real exato = integral(eixo.faces()[p], eixo.faces()[p + 1]);
//         const Real erro = std::abs(aproximado - exato);

//         passou = passou && capitulo_05::perto(aproximado, exato);

//         std::cout << std::setw(2) << p
//                   << std::setw(16) << aproximado
//                   << std::setw(16) << exato
//                   << std::setw(16) << erro << '\n';
//     }

//     static_cast<void>(
//         capitulo_05::registrar(
//             "aproximacao de um ponto reproduz a integral",
//             passou
//         )
//     );
//     std::cout << '\n';

//     return passou;
// }

// }  // namespace

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
