//==============================================================================
// SPDX-FileCopyrightText: 2026 FVMaker Team
// SPDX-License-Identifier: MIT
//==============================================================================
// Exercício Computacional 4.1
// Primeira malha
//==============================================================================

#include <FVGridMaker/OneDimensional/Distribution1D/Uniform1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/VolumeCentered1D.h>

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <string>

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

[[nodiscard]] bool dx_constante(std::span<const Real> Dx) {
    return std::all_of(
        Dx.begin(),
        Dx.end(),
        [referencia = Dx.front()](Real dx) {
            return perto(dx, referencia);
        }
    );
}

[[nodiscard]] bool centros_batem_com_formula(
    std::span<const Real> xC,
    Real x_inicial,
    Real delta_x
) {
    for (std::size_t p = 0; p < xC.size(); ++p) {
        const Real esperado =
            x_inicial + (static_cast<Real>(p) + Real{0.5}) * delta_x;

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
    std::cout << "Em uma malha uniforme, todos os comprimentos Dx[P] ";
    std::cout << "coincidem com Delta x = L/N.\n";
    std::cout << "Os centros calculados pela FVGridMaker também coincidem ";
    std::cout << "com a fórmula direta xC[P] = xI + (P + 1/2) Delta x.\n";
}

void imprimir_mensagem_final() {
    constexpr int size = 80;

    std::cout << "\nAplicações e recomendações\n";
    std::cout << std::string(size, '=') << '\n';
    std::cout << "1. Use a FVGridMaker para construir a malha e recuperar ";
    std::cout << "faces, centros e distâncias já consistentes.\n";
    std::cout << "2. Em uma malha uniforme, confirme sempre que todos os ";
    std::cout << "valores de Dx[P] são iguais a Delta x = L/N.\n";
    std::cout << "3. Verifique a conservação geométrica básica pela soma ";
    std::cout << "dos comprimentos: sum Dx[P] = L.\n";
    std::cout << "4. Compare os centros da biblioteca com a fórmula direta ";
    std::cout << "para entender a posição de cada volume.\n";
    std::cout << "5. Use este caso simples como referência antes de estudar ";
    std::cout << "malhas não uniformes.\n";
    std::cout << std::string(size, '=') << '\n';

    std::cout << "\nConceitos demonstrados\n";
    std::cout << std::string(size, '=') << '\n';
    std::cout << "1. Geração de uma malha uniforme unidimensional no ";
    std::cout << "intervalo [xI, xI + L].\n";
    std::cout << "2. Diferença entre faces xF, centros xC, comprimentos ";
    std::cout << "de volumes Dx e distâncias centro-face/centro-centro dx.\n";
    std::cout << "3. Uso de Axis1D como objeto que concentra a geometria ";
    std::cout << "necessária para o método dos volumes finitos.\n";
    std::cout << "4. Relação entre a fórmula analítica dos centros e os ";
    std::cout << "valores produzidos pela biblioteca.\n";
    std::cout << "5. Validação automática como parte do próprio exemplo ";
    std::cout << "computacional.\n";
    std::cout << std::string(size, '=') << '\n';
}

}  // namespace

int main() {
    std::cout << std::fixed << std::setprecision(12);

    constexpr Real x_inicial = 0.0;
    constexpr Real comprimento = 1.0;
    constexpr std::size_t n = 20;
    constexpr Real delta_x = comprimento / static_cast<Real>(n);

    // -------------------------------------------------------------------------
    // PONTO CENTRAL DO EXERCÍCIO
    // Aqui a primeira malha é efetivamente gerada pela FVGridMaker.
    //
    // Uniform1D define espaçamentos iguais.
    // VolumeCentered1D informa que as faces são as coordenadas primárias e que
    // os centros devem ser calculados pela biblioteca.
    // -------------------------------------------------------------------------
    const fvgrid::Axis1D eixo = fvgrid::Uniform1D::make(
        fvgrid::NVol{n},
        fvgrid::Length{comprimento},
        fvgrid::XInit{x_inicial},
        fvgrid::VolumeCentered1D{}
    );

    const auto xF = eixo.faces();
    const auto xC = eixo.centers();
    const auto Dx = eixo.cell_lengths();
    const auto dx = eixo.dx_centers();

    std::cout << "Exercício 4.1 - primeira malha\n";
    std::cout << "xI = " << x_inicial
              << ", L = " << comprimento
              << ", N = " << n
              << ", Delta x = " << delta_x << "\n\n";

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
    ++total; aprovados += registrar("todos os valores de Dx[P] são iguais", dx_constante(Dx));
    ++total; aprovados += registrar("sum Dx[P] = L", perto(soma(Dx), comprimento));
    ++total; aprovados += registrar(
        "xC[P] = xI + (P + 1/2) Delta x",
        centros_batem_com_formula(xC, x_inicial, delta_x)
    );

    imprimir_resumo(aprovados, total, Dx, comprimento);
    imprimir_observacao_final();
    imprimir_mensagem_final();

    return aprovados == total ? 0 : 1;
}
