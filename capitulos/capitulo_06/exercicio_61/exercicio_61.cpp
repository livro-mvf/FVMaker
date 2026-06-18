//==============================================================================
// SPDX-FileCopyrightText: 2026 FVMaker Team
// SPDX-License-Identifier: MIT
//==============================================================================
// Exercicio Computacional 6.1
// Montar os coeficientes
//==============================================================================


//==============================================================================
// Header FVGridMaker
//==============================================================================
#include "../comum/mvf_capitulo_06.h"



namespace {

using Real = capitulo_06::Real;
using Size = capitulo_06::Size;

struct DadosEntrada {
    Real comprimento{5.0};
    Size n{5};
    Real x_inicial{0.0};
    Real alpha_w{1.0};
    Real beta_w{0.0};
    Real gamma_w{1.0};
    Real alpha_e{0.0};
    Real beta_e{1.0};
    Real gamma_e{0.0};
};

[[nodiscard]] DadosEntrada ler_entrada(int argc, char** argv) {
    DadosEntrada dados{};

    if (argc == 10) {
        dados.comprimento = std::atof(argv[1]);
        dados.n = static_cast<Size>(std::strtoull(argv[2], nullptr, 10));
        dados.x_inicial = std::atof(argv[3]);
        dados.alpha_w = std::atof(argv[4]);
        dados.beta_w = std::atof(argv[5]);
        dados.gamma_w = std::atof(argv[6]);
        dados.alpha_e = std::atof(argv[7]);
        dados.beta_e = std::atof(argv[8]);
        dados.gamma_e = std::atof(argv[9]);
    }

    return dados;
}

}  // namespace

int main(int argc, char** argv) {
    std::cout << std::fixed << std::setprecision(12);

    const DadosEntrada dados = ler_entrada(argc, argv);

    const fvgrid::Axis1D eixo = capitulo_06::malha_uniforme_exemplo(
        dados.n,
        dados.comprimento,
        dados.x_inicial
    );

    const fvm::BoundaryCondition1D esquerda = fvm::robin_1d(
        dados.alpha_w,
        dados.beta_w,
        dados.gamma_w
    );
    const fvm::BoundaryCondition1D direita = fvm::robin_1d(
        dados.alpha_e,
        dados.beta_e,
        dados.gamma_e
    );

    const fvm::EquationContribution1D coeficientes =
        capitulo_06::montar_coeficientes_poisson(
            eixo,
            [](Real x) { return x * x; },
            esquerda,
            direita
        );

    const fvm::TridiagonalSystem1D sistema =
        fvm::to_tridiagonal_system(coeficientes);
    const std::vector<std::vector<Real>> matriz =
        capitulo_06::matriz_densa(coeficientes);

    std::cout << "Exercicio 6.1 - montar os coeficientes\n\n";
    std::cout << "Uso opcional: exercicio_61 L N xI aw bw gw ae be ge\n\n";
    std::cout << "Malha gerada pela FVGridMaker\n";
    std::cout << "=============================\n";
    std::cout << eixo << "\n\n";

    std::cout << "Coeficientes impressos pela FVMaker\n";
    std::cout << "===================================\n";
    std::cout << coeficientes << "\n\n";

    std::cout << "Matriz reconstruida\n";
    std::cout << "===================\n";
    capitulo_06::imprimir_matriz(matriz, sistema.rhs().values());

    unsigned aprovados{};
    unsigned total{};

    if (dados.n == 5 && capitulo_06::perto(dados.comprimento, 5.0)
        && capitulo_06::perto(dados.x_inicial, 0.0)) {
        const std::vector<std::vector<Real>> matriz_esperada{
            {3.0, -1.0, 0.0, 0.0, 0.0},
            {-1.0, 2.0, -1.0, 0.0, 0.0},
            {0.0, -1.0, 2.0, -1.0, 0.0},
            {0.0, 0.0, -1.0, 2.0, -1.0},
            {0.0, 0.0, 0.0, -1.0, 1.0}
        };

        bool matriz_ok = true;
        for (Size i = 0; i < matriz.size(); ++i) {
            for (Size j = 0; j < matriz[i].size(); ++j) {
                matriz_ok = matriz_ok &&
                            capitulo_06::perto(matriz[i][j], matriz_esperada[i][j]);
            }
        }

        ++total;
        aprovados += capitulo_06::registrar(
            "matriz reproduz a Eq. 05:exeSEL_1",
            matriz_ok
        );
    }

    return aprovados == total ? 0 : 1;
}
