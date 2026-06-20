//==============================================================================
// Exercicio Computacional 6.1 - Montagem dos coeficientes
//==============================================================================

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <string_view>
#include <vector>

#include <FVGridMaker/Core/StrongTypes.h>
#include <FVGridMaker/OneDimensional/Distribution1D/Uniform1D.h>
#include <FVMaker/OneDimensional/Assembly/Assembler1D.h>
#include <FVMaker/OneDimensional/Equation/Equation1D.h>

namespace {
using Real = fvm::Real;
using Size = fvm::Size;

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
    DadosEntrada d{};
    if (argc == 10) {
        d.comprimento = std::atof(argv[1]);
        d.n = static_cast<Size>(std::strtoull(argv[2], nullptr, 10));
        d.x_inicial = std::atof(argv[3]);
        d.alpha_w = std::atof(argv[4]);
        d.beta_w = std::atof(argv[5]);
        d.gamma_w = std::atof(argv[6]);
        d.alpha_e = std::atof(argv[7]);
        d.beta_e = std::atof(argv[8]);
        d.gamma_e = std::atof(argv[9]);
    }
    return d;
}

[[nodiscard]] bool perto(Real a, Real b, Real tol = 1.0e-10) {
    return std::abs(a - b)
        <= tol * std::max({Real{1}, std::abs(a), std::abs(b)});
}

void imprimir_sistema(const fvm::TridiagonalSystem1D& sistema) {
    std::cout << std::setw(6) << "P"
              << std::setw(16) << "A_W"
              << std::setw(16) << "A_P"
              << std::setw(16) << "A_E"
              << std::setw(16) << "B_P" << '\n';
    for (Size p = 0; p < sistema.size(); ++p) {
        const Real aw = p > 0 ? sistema.lower()[p - 1] : 0.0;
        const Real ae = p + 1 < sistema.size() ? sistema.upper()[p] : 0.0;
        std::cout << std::setw(6) << p
                  << std::setw(16) << aw
                  << std::setw(16) << -sistema.diagonal()[p]
                  << std::setw(16) << ae
                  << std::setw(16) << -sistema.rhs()[p] << '\n';
    }
}

[[nodiscard]] std::vector<std::vector<Real>> matriz_densa(
    const fvm::TridiagonalSystem1D& sistema
) {
    std::vector<std::vector<Real>> matriz(
        sistema.size(),
        std::vector<Real>(sistema.size())
    );
    for (Size i = 0; i < sistema.size(); ++i) {
        matriz[i][i] = -sistema.diagonal()[i];
        if (i > 0) matriz[i][i - 1] = -sistema.lower()[i - 1];
        if (i + 1 < sistema.size()) matriz[i][i + 1] = -sistema.upper()[i];
    }
    return matriz;
}
}  // namespace

int main(int argc, char** argv) {
    std::cout << std::fixed << std::setprecision(12);
    const DadosEntrada dados = ler_entrada(argc, argv);

    const fvgrid::Axis1D eixo = fvgrid::uniform_axis_1d(
        fvgrid::NVol{dados.n},
        fvgrid::Length{dados.comprimento},
        fvgrid::XInit{dados.x_inicial}
    );
    const fvm::GridView1D malha{eixo};

    const fvm::BoundaryCondition1D esquerda = fvm::robin_1d(
        dados.alpha_w, dados.beta_w, dados.gamma_w
    );
    const fvm::BoundaryCondition1D direita = fvm::robin_1d(
        dados.alpha_e, dados.beta_e, dados.gamma_e
    );
    const fvm::BoundarySet1D contornos{esquerda, direita};

    const fvm::Equation1D equacao{
        malha,
        fvm::Laplacian1D{},
        fvm::function_source_1d(malha, [](Real x) { return -x * x; }),
        contornos
    };
    const fvm::TridiagonalSystem1D sistema = fvm::assemble_steady_1d(equacao);

    std::cout << "Exercicio 6.1 - montar os coeficientes\n\n";
    std::cout << "Uso opcional: exercicio_61 L N xI aw bw gw ae be ge\n\n";
    std::cout << "1. Malha gerada pela FVGridMaker\n" << eixo << "\n\n";
    std::cout << "2. Condicoes de contorno\n"
              << "oeste: alpha=" << dados.alpha_w << ", beta=" << dados.beta_w
              << ", gamma=" << dados.gamma_w << '\n'
              << "leste: alpha=" << dados.alpha_e << ", beta=" << dados.beta_e
              << ", gamma=" << dados.gamma_e << "\n\n";
    std::cout << "3. Coeficientes montados pela FVMaker\n";
    imprimir_sistema(sistema);

    if (dados.n != 5 || !perto(dados.comprimento, 5.0)
        || !perto(dados.x_inicial, 0.0)) {
        return 0;
    }

    const std::vector<std::vector<Real>> esperada{
        {3.0, -1.0, 0.0, 0.0, 0.0},
        {-1.0, 2.0, -1.0, 0.0, 0.0},
        {0.0, -1.0, 2.0, -1.0, 0.0},
        {0.0, 0.0, -1.0, 2.0, -1.0},
        {0.0, 0.0, 0.0, -1.0, 1.0}
    };
    const auto obtida = matriz_densa(sistema);
    bool ok = true;
    for (Size i = 0; i < obtida.size(); ++i) {
        for (Size j = 0; j < obtida.size(); ++j) {
            ok = ok && perto(obtida[i][j], esperada[i][j]);
        }
    }
    std::cout << (ok ? "[PASSOU] " : "[FALHOU] ")
              << "matriz reproduz a equacao do livro\n";
    return ok ? 0 : 1;
}
