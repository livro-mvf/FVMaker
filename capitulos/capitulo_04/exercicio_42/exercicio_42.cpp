//==============================================================================
// SPDX-FileCopyrightText: 2026 FVMaker Team
// SPDX-License-Identifier: MIT
//==============================================================================
// Exercício Computacional 4.2
// Malha aleatória
//==============================================================================


//==============================================================================
// Header c++
//==============================================================================
#include <algorithm>
#include <iostream>

//==============================================================================
// Header FVGridMaker
//==============================================================================
#include <FVGridMaker/OneDimensional/Distribution1D/Random1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/VolumeCentered1D.h>
#include <FVGridMaker/Output/CSV/Axis1DCSVWriter.h>


namespace {

using Real = fvgrid::Real;

constexpr Real kTol = 1.0e-12;

[[nodiscard]] bool perto(Real a, Real b) {
    return std::abs(a - b) <=
           kTol * std::max<Real>({Real{1}, std::abs(a), std::abs(b)});
}

[[nodiscard]] Real soma(std::span<const Real> valores) {
    return std::accumulate(valores.begin(), valores.end(), Real{});
}

[[nodiscard]] bool registrar(const std::string& texto, bool passou) {
    std::cout << (passou ? "[PASSOU] " : "[FALHOU] ") << texto << '\n';
    return passou;
}

[[nodiscard]] bool faces_estritamente_crescentes(std::span<const Real> xF) {
    for (std::size_t p = 0; p + 1 < xF.size(); ++p) {
        if (!(xF[p + 1] > xF[p])) {
            return false;
        }
    }

    return true;
}

[[nodiscard]] bool comprimentos_positivos(std::span<const Real> Dx) {
    return std::all_of(
        Dx.begin(),
        Dx.end(),
        [](Real dx) {
            return dx > Real{};
        }
    );
}

[[nodiscard]] bool centros_sao_pontos_medios(
    std::span<const Real> xF,
    std::span<const Real> xC
) {
    for (std::size_t p = 0; p < xC.size(); ++p) {
        const Real esperado = Real{0.5} * (xF[p] + xF[p + 1]);

        if (!perto(xC[p], esperado)) {
            return false;
        }
    }

    return true;
}

void imprimir_resumo(
    unsigned aprovados,
    unsigned total,
    std::span<const Real> Dx,
    Real comprimento
) {
    std::cout << "\nResumo\n";
    std::cout << "======\n";
    std::cout << "Verificações aprovadas: "
              << aprovados << " de " << total << '\n';
    std::cout << "Soma dos comprimentos Dx: "
              << soma(Dx) << '\n';
    std::cout << "Erro na soma: "
              << std::abs(soma(Dx) - comprimento) << '\n';
}

void imprimir_observacao_final() {
    std::cout << "\nObservação\n";
    std::cout << "==========\n";
    std::cout << "Como a malha é aleatória, as coordenadas mudam quando ";
    std::cout << "a semente muda.\n";
    std::cout << "O gabarito do gerador não é uma lista fixa de números, ";
    std::cout << "mas sim a preservação do domínio, a ordem estrita das ";
    std::cout << "faces e a positividade dos comprimentos dos volumes.\n";
}

void imprimir_mensagem_final() {
    constexpr int size = 80;

    std::cout << "\nAplicações e recomendações\n";
    std::cout << std::string(size, '=') << '\n';
    std::cout << "1. Use a malha aleatória como teste de estresse para ";
    std::cout << "fórmulas em malhas não uniformes.\n";
    std::cout << "2. Fixe a semente quando precisar comparar resultados ";
    std::cout << "entre execuções.\n";
    std::cout << "3. Verifique sempre se as faces permanecem ordenadas e ";
    std::cout << "se todos os comprimentos Dx[P] são positivos.\n";
    std::cout << "4. Confirme a soma dos volumes antes de usar a malha em ";
    std::cout << "um sistema de equações.\n";
    std::cout << "5. Use os vetores xF, xC, Dx e dx como entrada natural ";
    std::cout << "das discretizações do MVF.\n";
    std::cout << std::string(size, '=') << '\n';

    std::cout << "\nConceitos demonstrados\n";
    std::cout << std::string(size, '=') << '\n';
    std::cout << "1. Construção de uma malha não uniforme por faces ";
    std::cout << "internas aleatórias.\n";
    std::cout << "2. Fixação das faces externas em xI e xI + L.\n";
    std::cout << "3. Arranjo volume centrado, com centros calculados por ";
    std::cout << "xC[P] = (xF[P] + xF[P+1]) / 2.\n";
    std::cout << "4. Cálculo automático de Dx e dx pela Axis1D da ";
    std::cout << "FVGridMaker.\n";
    std::cout << "5. Validação geométrica como gabarito de um gerador ";
    std::cout << "aleatório.\n";
    std::cout << std::string(size, '=') << '\n';
}

}  // namespace

int main() {
    std::cout << std::fixed << std::setprecision(12);

    constexpr Real x_inicial = 0.0;
    constexpr Real comprimento = 1.0;
    constexpr std::size_t n = 20;
    constexpr std::uint64_t semente = 20260402;
    constexpr Real espacamento_minimo = 1.0e-4;

    // -------------------------------------------------------------------------
    // PONTO CENTRAL DO EXERCÍCIO
    // Aqui a malha aleatória é efetivamente gerada pela FVGridMaker.
    //
    // Random1D, com VolumeCentered1D, gera N-1 faces internas aleatórias,
    // fixa as faces externas em xI e xI + L, ordena as faces e completa a
    // Axis1D com xC, Dx e dx.
    // -------------------------------------------------------------------------
    const fvgrid::Axis1D eixo = fvgrid::Random1D::make(
        fvgrid::NVol{n},
        fvgrid::Length{comprimento},
        fvgrid::XInit{x_inicial},
        fvgrid::Seed{semente},
        fvgrid::MinSpacing{espacamento_minimo},
        fvgrid::VolumeCentered1D{}
    );

    const auto xF = eixo.faces();
    const auto xC = eixo.centers();
    const auto Dx = eixo.cell_lengths();
    const auto dx = eixo.dx_centers();

    std::cout << "Exercício 4.2 - malha aleatória\n";
    std::cout << "xI = " << x_inicial
              << ", L = " << comprimento
              << ", N = " << n
              << ", semente = " << semente << "\n\n";

    std::cout << "Dados da malha gerados pela FVGridMaker\n";
    std::cout << "=======================================\n";
    std::cout << eixo << "\n\n";

    std::cout << "Verificações automáticas\n";
    std::cout << "========================\n";

    unsigned aprovados{};
    unsigned total{};
    ++total; aprovados += registrar("dimensão de xF é N + 1", xF.size() == n + 1);
    ++total; aprovados += registrar("dimensão de xC é N", xC.size() == n);
    ++total; aprovados += registrar("dimensão de Dx é N", Dx.size() == n);
    ++total; aprovados += registrar("dimensão de dx é N + 1", dx.size() == n + 1);
    ++total; aprovados += registrar("xF[0] = xI", perto(xF.front(), x_inicial));
    ++total; aprovados += registrar("xF[N] = xI + L", perto(xF.back(), x_inicial + comprimento));
    ++total; aprovados += registrar("xF[P+1] > xF[P]", faces_estritamente_crescentes(xF));
    ++total; aprovados += registrar("Dx[P] > 0", comprimentos_positivos(Dx));
    ++total; aprovados += registrar("sum Dx[P] = L", perto(soma(Dx), comprimento));
    ++total; aprovados += registrar(
        "xC[P] = (xF[P] + xF[P+1]) / 2",
        centros_sao_pontos_medios(xF, xC)
    );

    imprimir_resumo(aprovados, total, Dx, comprimento);
    fvgrid::Axis1DCSVWriter::write(eixo, "exercicio_42_malha_aleatoria.csv");
    std::cout << "CSV gerado: exercicio_42_malha_aleatoria.csv\n";
    imprimir_observacao_final();
    imprimir_mensagem_final();

    return aprovados == total ? 0 : 1;
}
