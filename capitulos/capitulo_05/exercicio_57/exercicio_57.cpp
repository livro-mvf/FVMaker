// //==============================================================================
// // SPDX-FileCopyrightText: 2026 FVMaker Team
// // SPDX-License-Identifier: MIT
// //==============================================================================
// // Exercicio Computacional 5.7
// // Consistencia vista pelo residuo normalizado
// //==============================================================================


// //==============================================================================
// // Header FVGridMaker
// //==============================================================================
// #include "../comum/mvf_capitulo_05.h"


// namespace {

// using Real = capitulo_05::Real;
// using Size = capitulo_05::Size;

// [[nodiscard]] Real solucao(Real x) {
//     return x - Real{0.5} * x * x;
// }

// [[nodiscard]] std::vector<Real> faces_suaves(Size n) {
//     constexpr Real pi = 3.141592653589793238462643383279502884;

//     std::vector<Real> faces(n + 1);
//     const Real amplitude = Real{0.12} / static_cast<Real>(n);

//     for (Size i = 0; i <= n; ++i) {
//         const Real s = static_cast<Real>(i) / static_cast<Real>(n);
//         faces[i] = s + amplitude * std::sin(Real{2} * pi * s);
//     }

//     faces.front() = 0.0;
//     faces.back() = 1.0;
//     return faces;
// }

// [[nodiscard]] Real residuo_integrado(
//     const fvgrid::Axis1D& eixo,
//     Size p
// ) {
//     const Real phi_w = solucao(eixo.centers()[p - 1]);
//     const Real phi_p = solucao(eixo.centers()[p]);
//     const Real phi_e = solucao(eixo.centers()[p + 1]);

//     const Real fluxo_e =
//         (phi_e - phi_p) / (eixo.centers()[p + 1] - eixo.centers()[p]);
//     const Real fluxo_w =
//         (phi_p - phi_w) / (eixo.centers()[p] - eixo.centers()[p - 1]);

//     return fluxo_e - fluxo_w + eixo.cell_lengths()[p];
// }

// [[nodiscard]] Real residuo_normalizado(
//     const fvgrid::Axis1D& eixo,
//     Size p
// ) {
//     return residuo_integrado(eixo, p) / eixo.cell_lengths()[p];
// }

// struct Residuos {
//     Real integrado_max{};
//     Real normalizado_max{};
// };

// [[nodiscard]] Residuos medir_residuos(const fvgrid::Axis1D& eixo) {
//     Residuos r{};

//     for (Size p = 1; p + 1 < eixo.num_cells(); ++p) {
//         r.integrado_max = std::max(
//             r.integrado_max,
//             std::abs(residuo_integrado(eixo, p))
//         );
//         r.normalizado_max = std::max(
//             r.normalizado_max,
//             std::abs(residuo_normalizado(eixo, p))
//         );
//     }

//     return r;
// }

// void imprimir_linha(
//     Size n,
//     const Residuos& face_centrada,
//     const Residuos& volume_suave,
//     const Residuos& volume_persistente
// ) {
//     std::cout << std::setw(4) << n
//               << std::setw(18) << face_centrada.normalizado_max
//               << std::setw(18) << volume_suave.normalizado_max
//               << std::setw(18) << volume_persistente.normalizado_max
//               << std::setw(18) << volume_persistente.integrado_max
//               << '\n';
// }

// }  // namespace

int main() {
//     std::cout << std::fixed << std::setprecision(12);
//     std::cout << "Exercicio 5.7 - residuo de truncamento normalizado\n\n";
//     std::cout << "N"
//               << std::setw(18) << "FC norm"
//               << std::setw(18) << "VC suave norm"
//               << std::setw(18) << "VC salto norm"
//               << std::setw(18) << "VC salto int" << '\n';

//     bool face_centrada_decai = true;
//     bool volume_suave_decai = true;
//     bool integrado_engana = true;

//     Real fc_anterior = 1.0e100;
//     Real vc_suave_anterior = 1.0e100;
//     Real integrado_anterior = 1.0e100;

//     for (Size n : {16u, 32u, 64u, 128u}) {
//         const fvgrid::Axis1D face_centrada =
//             capitulo_05::malha_face_centrada_customizada(
//                 capitulo_05::centros_potencia(n, 1.0, 0.0, 1.35),
//                 0.0,
//                 1.0
//             );

//         const fvgrid::Axis1D volume_suave =
//             capitulo_05::malha_volume_centrada_customizada(
//                 faces_suaves(n),
//                 0.0,
//                 1.0
//             );

//         const fvgrid::Axis1D volume_persistente =
//             capitulo_05::malha_volume_centrada_customizada(
//                 capitulo_05::faces_potencia(n, 1.0, 0.0, 1.7),
//                 0.0,
//                 1.0
//             );

//         const Residuos r_fc = medir_residuos(face_centrada);
//         const Residuos r_vc_suave = medir_residuos(volume_suave);
//         const Residuos r_vc_persistente = medir_residuos(volume_persistente);

//         imprimir_linha(n, r_fc, r_vc_suave, r_vc_persistente);

//         face_centrada_decai =
//             face_centrada_decai &&
//             r_fc.normalizado_max <= std::max(fc_anterior, Real{1.0e-10});
//         volume_suave_decai =
//             volume_suave_decai &&
//             r_vc_suave.normalizado_max <=
//                 std::max(vc_suave_anterior, Real{1.0e-10});
//         integrado_engana =
//             integrado_engana &&
//             r_vc_persistente.integrado_max <=
//                 std::max(integrado_anterior, Real{1.0e-10});

//         fc_anterior = r_fc.normalizado_max;
//         vc_suave_anterior = r_vc_suave.normalizado_max;
//         integrado_anterior = r_vc_persistente.integrado_max;
//     }

//     std::cout << '\n';

//     unsigned aprovados{};
//     unsigned total{};

//     ++total;
//     aprovados += capitulo_05::registrar(
//         "residuo normalizado da face centrada decai",
//         face_centrada_decai
//     );

//     ++total;
//     aprovados += capitulo_05::registrar(
//         "volume centrado decai quando a nao uniformidade local se atenua",
//         volume_suave_decai
//     );

//     ++total;
//     aprovados += capitulo_05::registrar(
//         "residuo integrado pode diminuir mesmo no caso persistente",
//         integrado_engana
//     );

//     std::cout << "\nA coluna 'VC salto int' e impressa apenas para mostrar "
//               << "por que o residuo integrado nao deve ser usado sozinho.\n";

//     return aprovados == total ? 0 : 1;
}
