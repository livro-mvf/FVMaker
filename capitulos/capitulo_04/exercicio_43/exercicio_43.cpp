//==============================================================================
// SPDX-FileCopyrightText: 2026 FVMaker Team
// SPDX-License-Identifier: MIT
//==============================================================================
// Exercício Computacional 4.3
// Refinamento nas fronteiras de Roberts
//==============================================================================

//==============================================================================
// Header c++
//==============================================================================
#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>

//==============================================================================
// Header FVGridMaker
//==============================================================================
#include <FVGridMaker/OneDimensional/Distribution1D/Roberts1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/VolumeCentered1D.h>
#include <FVGridMaker/Output/CSV/Axis1DCSVWriter.h>


namespace {

using Real = fvgrid::Real;

constexpr Real kTol = 1.0e-12;

struct Caso {
    std::string nome;
    Real beta;
};

[[nodiscard]] bool perto(Real a, Real b, Real tol = kTol) {
    return std::abs(a - b) <=
           tol * std::max<Real>({Real{1}, std::abs(a), std::abs(b)});
}

[[nodiscard]] fvgrid::Axis1D gerar_malha_roberts(
    std::size_t n,
    Real comprimento,
    Real beta
) {
    // -------------------------------------------------------------------------
    // PONTO CENTRAL DO EXERCÍCIO
    // Aqui a malha de Roberts é gerada pela FVGridMaker.
    //
    // Roberts1D usa alpha = 1/2 internamente e recebe beta como parâmetro.
    // VolumeCentered1D indica que as coordenadas primárias calculadas pela
    // formulação de Roberts são faces da malha.
    // -------------------------------------------------------------------------
    return fvgrid::Roberts1D::make(
        fvgrid::NVol{n},
        fvgrid::Length{comprimento},
        fvgrid::XInit{0.0},
        fvgrid::Beta{beta},
        fvgrid::VolumeCentered1D{}
    );
}

[[nodiscard]] Real razao_max_min(std::span<const Real> Dx) {
    const auto [menor, maior] = std::minmax_element(Dx.begin(), Dx.end());
    return *maior / *menor;
}

[[nodiscard]] Real soma(std::span<const Real> valores) {
    return std::accumulate(valores.begin(), valores.end(), Real{});
}

[[nodiscard]] bool simetria_faces(const fvgrid::Axis1D& eixo) {
    const Real comprimento = eixo.length();
    const std::size_t n = eixo.num_faces() - 1;

    for (std::size_t i = 0; i <= n; ++i) {
        if (!perto(eixo.faces()[i], comprimento - eixo.faces()[n - i], 1.0e-11)) {
            return false;
        }
    }

    return true;
}

[[nodiscard]] bool registrar(const std::string& texto, bool passou) {
    std::cout << (passou ? "[PASSOU] " : "[FALHOU] ") << texto << '\n';
    return passou;
}

void exportar_razao(std::size_t n, Real comprimento) {
    std::ofstream arquivo{"exercicio_43_roberts_razao.csv"};

    if (!arquivo) {
        throw std::runtime_error("Não foi possível criar exercicio_43_roberts_razao.csv.");
    }

    arquivo << "beta,razao_Dxmax_Dxmin\n";

    constexpr std::size_t amostras = 80;
    constexpr Real beta_min = 1.1;
    constexpr Real beta_max = 20.0;

    for (std::size_t k = 0; k < amostras; ++k) {
        const Real t = static_cast<Real>(k) / static_cast<Real>(amostras - 1);
        const Real beta = beta_min + t * (beta_max - beta_min);
        const fvgrid::Axis1D eixo = gerar_malha_roberts(n, comprimento, beta);

        arquivo << std::setprecision(17) << beta << ','
                << razao_max_min(eixo.cell_lengths()) << '\n';
    }
}

void exportar_malhas(
    std::size_t n,
    Real comprimento,
    const std::array<Caso, 3>& casos
) {
    for (const Caso& caso : casos) {
        const fvgrid::Axis1D eixo = gerar_malha_roberts(n, comprimento, caso.beta);
        const std::string nome_arquivo =
            "exercicio_43_roberts_" + caso.nome + ".csv";

        fvgrid::Axis1DCSVWriter::write(eixo, nome_arquivo);
    }
}

void explicar_alpha_um() {
    std::cout << "\nPor que alpha = 1 não pode ser usado?\n";
    std::cout << "=====================================\n";
    std::cout << "Na equação de Roberts aparece o expoente ";
    std::cout << "(eta - alpha) / (1 - alpha).\n";
    std::cout << "Se alpha = 1, o denominador 1 - alpha é zero; ";
    std::cout << "a transformação fica singular e deixa de definir ";
    std::cout << "uma malha válida por essa fórmula.\n";
    std::cout << "Por isso a FVGridMaker usa alpha = 1/2 nesta distribuição.\n";
}

void imprimir_mensagem_final() {
    constexpr int size = 80;

    std::cout << "\nAplicações e recomendações\n";
    std::cout << std::string(size, '=') << '\n';
    std::cout << "1. Use beta próximo de 1 quando quiser forte concentração ";
    std::cout << "de volumes junto às fronteiras.\n";
    std::cout << "2. Use beta grande quando quiser uma malha próxima da uniforme.\n";
    std::cout << "3. Verifique a razão Dx_max/Dx_min antes de usar a malha ";
    std::cout << "em uma simulação.\n";
    std::cout << "4. Confirme a simetria quando o problema físico também ";
    std::cout << "for simétrico nas fronteiras.\n";
    std::cout << "5. Use os CSVs gerados para visualizar a concentração ";
    std::cout << "produzida por diferentes valores de beta.\n";
    std::cout << std::string(size, '=') << '\n';

    std::cout << "\nConceitos demonstrados\n";
    std::cout << std::string(size, '=') << '\n';
    std::cout << "1. Geração de malhas analíticas pela distribuição Roberts1D.\n";
    std::cout << "2. Controle de refinamento por meio do parâmetro beta.\n";
    std::cout << "3. Uso de alpha = 1/2 para obter uma distribuição simétrica.\n";
    std::cout << "4. Cálculo de uma métrica global de não uniformidade ";
    std::cout << "por Dx_max/Dx_min.\n";
    std::cout << "5. Validação numérica da simetria xF[i] = L - xF[N-i].\n";
    std::cout << std::string(size, '=') << '\n';
}

}  // namespace

int main() {
    std::cout << std::fixed << std::setprecision(12);

    constexpr std::size_t n = 50;
    constexpr Real comprimento = 1.0;
    constexpr Real alpha = 0.5;

    const std::array<Caso, 3> casos{{
        {"proximo_de_1", 1.1},
        {"intermediario", 4.0},
        {"beta_grande", 20.0}
    }};

    std::cout << "Exercício 4.3 - refinamento nas fronteiras de Roberts\n";
    std::cout << "L = " << comprimento
              << ", N = " << n
              << ", alpha = " << alpha
              << ", beta em [1.1, 20]\n\n";

    bool todos_simetria = true;
    bool todos_soma = true;

    std::cout << "Casos representativos\n";
    std::cout << "=====================\n";
    for (const Caso& caso : casos) {
        const fvgrid::Axis1D eixo = gerar_malha_roberts(n, comprimento, caso.beta);

        const bool simetria = simetria_faces(eixo);
        const bool soma_ok = perto(soma(eixo.cell_lengths()), comprimento);

        todos_simetria = todos_simetria && simetria;
        todos_soma = todos_soma && soma_ok;

        std::cout << caso.nome << " (beta = " << caso.beta << ")\n";
        std::cout << "  Dx_max/Dx_min = " << razao_max_min(eixo.cell_lengths()) << '\n';
        std::cout << "  sum(Dx)       = " << soma(eixo.cell_lengths()) << '\n';
        std::cout << "  simetria      = " << (simetria ? "sim" : "não") << "\n\n";
    }

    exportar_razao(n, comprimento);
    exportar_malhas(n, comprimento, casos);

    std::cout << "Verificações automáticas\n";
    std::cout << "========================\n";
    unsigned aprovados{};
    unsigned total{};
    ++total; aprovados += registrar("sum Dx[P] = L nos casos representativos", todos_soma);
    ++total; aprovados += registrar("xF[i] = L - xF[N-i] nos casos representativos", todos_simetria);

    std::cout << "\nResumo\n";
    std::cout << "======\n";
    std::cout << "Verificações aprovadas: " << aprovados << " de " << total << '\n';
    std::cout << "CSVs gerados:\n";
    std::cout << "  exercicio_43_roberts_razao.csv\n";
    std::cout << "  exercicio_43_roberts_proximo_de_1.csv\n";
    std::cout << "  exercicio_43_roberts_intermediario.csv\n";
    std::cout << "  exercicio_43_roberts_beta_grande.csv\n";

    explicar_alpha_um();
    imprimir_mensagem_final();

    return aprovados == total ? 0 : 1;
}
