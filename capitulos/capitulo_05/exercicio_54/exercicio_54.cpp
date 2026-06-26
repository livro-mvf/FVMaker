// //==============================================================================
// // SPDX-FileCopyrightText: 2026 FVMaker Team
// // SPDX-License-Identifier: MIT
// //==============================================================================
// // Exercicio Computacional 5.4
// // Condicoes de contorno em uma rotina unica
// //==============================================================================

// //==============================================================================
// // Header FVGridMaker
// //==============================================================================
// #include "../comum/mvf_capitulo_05.h"


// namespace {

// using Real = capitulo_05::Real;

// bool testar(
//     std::string_view nome,
//     fvm::BoundarySide1D lado,
//     const fvm::BoundaryCondition1D& condicao,
//     Real x_contorno,
//     Real distancia,
//     Real termo_fonte,
//     Real coeficiente_vizinho,
//     capitulo_05::CoeficientesContorno esperado
// ) {
//     const capitulo_05::CoeficientesContorno c =
//         capitulo_05::coeficientes_contorno(
//             lado,
//             condicao,
//             x_contorno,
//             distancia,
//             termo_fonte,
//             coeficiente_vizinho
//         );

//     fvm::EquationContribution1D coeficientes{1};
//     coeficientes.aw()[0] = c.aw;
//     coeficientes.ae()[0] = c.ae;
//     coeficientes.ap()[0] = c.ap;
//     coeficientes.bp()[0] = c.bp;

//     std::cout << nome << '\n';
//     std::cout << "  alpha = " << condicao.alpha(x_contorno)
//               << ", beta = " << condicao.beta(x_contorno)
//               << ", gamma = " << condicao.gamma(x_contorno) << '\n';
//     std::cout << coeficientes << "\n\n";

//     return capitulo_05::perto(c.aw, esperado.aw) &&
//            capitulo_05::perto(c.ae, esperado.ae) &&
//            capitulo_05::perto(c.ap, esperado.ap) &&
//            capitulo_05::perto(c.bp, esperado.bp);
// }

// }  // namespace

int main() {
//     std::cout << std::fixed << std::setprecision(12);
//     std::cout << "Exercicio 5.4 - contornos por uma rotina unica\n\n";

//     unsigned aprovados{};
//     unsigned total{};

//     ++total;
//     aprovados += testar(
//         "Dirichlet a esquerda: phi(x_I) = 3",
//         fvm::BoundarySide1D::left,
//         fvm::dirichlet_1d(3.0),
//         0.0,
//         0.25,
//         2.0,
//         2.0,
//         capitulo_05::CoeficientesContorno{0.0, 2.0, 6.0, 14.0}
//     );

//     ++total;
//     aprovados += testar(
//         "Neumann a direita: phi'(x_I + L) = 2",
//         fvm::BoundarySide1D::right,
//         fvm::neumann_1d(2.0),
//         1.0,
//         0.25,
//         0.5,
//         2.0,
//         capitulo_05::CoeficientesContorno{2.0, 0.0, 2.0, 2.5}
//     );

//     ++total;
//     aprovados += testar(
//         "Robin a esquerda: 2 phi(x_I) + 3 phi'(x_I) = 5",
//         fvm::BoundarySide1D::left,
//         fvm::robin_1d(2.0, 3.0, 5.0),
//         0.0,
//         0.2,
//         0.0,
//         2.5,
//         capitulo_05::CoeficientesContorno{
//             0.0,
//             2.5,
//             2.5 + 2.0 / (2.0 * 0.2 - 3.0),
//             5.0 / (2.0 * 0.2 - 3.0)
//         }
//     );

//     static_cast<void>(
//         capitulo_05::registrar(
//             "uma unica rotina reproduz Dirichlet, Neumann e Robin",
//             aprovados == total
//         )
//     );

//     return aprovados == total ? 0 : 1;
}
