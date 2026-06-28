// ============================================================================
// Arquivo: exercicio_58.cpp
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Implementa exercicio 58 no contexto de capitulos / capitulo_05 / exercicio_58.
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

//     std::cout << std::fixed << std::setprecision(12);

//     const fvgrid::Axis1D eixo = fvgrid::Uniform1D::make(
//         fvgrid::NVol{3},
//         fvgrid::Length{1.0},
//         fvgrid::XInit{0.0},
//         fvgrid::FaceCentered1D{}
//     );

//     const fvm::EquationContribution1D coeficientes =
//         capitulo_05::montar_coeficientes_poisson(
//             eixo,
//             [](Real) { return Real{1}; },
//             fvm::dirichlet_1d(0.0),
//             fvm::neumann_1d(0.0)
//         );

//     const fvm::TridiagonalSystem1D sistema =
//         fvm::to_tridiagonal_system(coeficientes);
//     const std::vector<std::vector<Real>> matriz =
//         capitulo_05::matriz_densa(coeficientes);

//     std::cout << "Exercicio 5.8 - problema proprio\n\n";
//     std::cout << "Malha uniforme face centrada de 3 volumes\n";
//     std::cout << "=========================================\n";
//     std::cout << eixo << "\n\n";

//     std::cout << "Coeficientes\n";
//     std::cout << "============\n";
//     std::cout << coeficientes << '\n';

//     std::cout << "\nMatriz reconstruida\n";
//     std::cout << "===================\n";
//     capitulo_05::imprimir_matriz(matriz, sistema.rhs().values());

//     const std::vector<std::vector<Real>> matriz_esperada{
//         {9.0, -3.0, 0.0},
//         {-3.0, 6.0, -3.0},
//         {0.0, -3.0, 3.0}
//     };

//     const std::array<Real, 3> rhs_esperado{
//         Real{1} / Real{3},
//         Real{1} / Real{3},
//         Real{1} / Real{3}
//     };

//     unsigned aprovados{};
//     unsigned total{};

//     ++total;
//     aprovados += capitulo_05::registrar(
//         "matriz coincide com a montagem manual do problema pequeno",
//         capitulo_05::conferir_matriz(matriz, matriz_esperada)
//     );

//     ++total;
//     aprovados += capitulo_05::registrar(
//         "lado direito coincide com a montagem manual",
//         capitulo_05::conferir_vetor(sistema.rhs().values(), rhs_esperado)
//     );

//     std::cout << "\nA resolucao do sistema fica para o capitulo 6.\n";

//     return aprovados == total ? 0 : 1;
}
