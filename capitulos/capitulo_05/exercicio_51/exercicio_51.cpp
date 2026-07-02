// ============================================================================
// Arquivo: exercicio_51.cpp
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Implementa exercicio 51 no contexto de capitulos / capitulo_05 / exercicio_51.
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

//==============================================================================
// Header c++
//==============================================================================
#include <array>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <span>
#include <string>
#include <vector>

//==============================================================================
// Header FVGridMaker
//==============================================================================
#include <FVGridMaker/OneDimensional/Distribution1D/Uniform1D.h>

//==============================================================================
// Header FVMaker
//==============================================================================
#include <FVMaker/Core/Types.h>
#include <FVMaker/OneDimensional/Assembly/Assembler1D.h>
#include <FVMaker/OneDimensional/Boundary/BoundaryCondition1D.h>
#include <FVMaker/OneDimensional/Equation/Equation1D.h>

namespace {

using Real = fvm::Real;
using Size = fvm::Size;

[[nodiscard]] bool perto(Real a, Real b, Real tol = 1.0e-10) {
    return std::abs(a - b) <= tol * std::max<Real>({Real{1}, std::abs(a), std::abs(b)});
}

[[nodiscard]] bool registrar(const std::string& texto, bool passou) {
    std::cout << (passou ? "[PASSOU] " : "[FALHOU] ") << texto << '\n';
    return passou;
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

[[nodiscard]] std::vector<Real> lado_direito(const fvm::TridiagonalSystem1D& sistema) {
    std::vector<Real> bp(sistema.size());

    for (Size i = 0; i < sistema.size(); ++i) {
        bp[i] = -sistema.rhs()[i];
    }

    return bp;
}

void imprimir_matriz(
    const std::vector<std::vector<Real>>& matriz,
    const std::vector<Real>& bp
) {
    for (Size i = 0; i < matriz.size(); ++i) {
        std::cout << "[";
        for (Real valor : matriz[i]) {
            std::cout << std::setw(16) << valor;
        }
        std::cout << " ] [phi" << i << "] = " << std::setw(16) << bp[i] << '\n';
    }
}

[[nodiscard]] bool conferir_matriz(
    const std::vector<std::vector<Real>>& obtida,
    const std::vector<std::vector<Real>>& esperada
) {
    bool ok = true;
    for (Size i = 0; i < obtida.size(); ++i) {
        for (Size j = 0; j < obtida[i].size(); ++j) {
            ok = ok && perto(obtida[i][j], esperada[i][j]);
        }
    }
    return ok;
}

[[nodiscard]] bool conferir_vetor(
    const std::vector<Real>& obtido,
    std::span<const Real> esperado
) {
    bool ok = true;
    for (Size i = 0; i < obtido.size(); ++i) {
        ok = ok && perto(obtido[i], esperado[i]);
    }
    return ok;
}

}  // namespace

int main() {
    std::cout << std::fixed << std::setprecision(12);

//==============================================================================
// Geração e Impressao da malha
//==============================================================================

    const fvgrid::Axis1D eixo =
        fvgrid::Uniform1D::make(
            fvgrid::NVol{5},
            fvgrid::Length{5.0},
            fvgrid::XInit{0.0}
        );

    const fvm::GridView1D malha{eixo};

    std::cout << "Exercicio 5.1 - malha do Exemplo 5.2\n\n";
    std::cout << eixo << "\n\n";

//==============================================================================
// Definição das condições de contorno
//==============================================================================

    const fvm::BoundaryCondition1D esquerda = fvm::dirichlet_1d(1.0);
    const fvm::BoundaryCondition1D direita = fvm::neumann_1d(0.0);
    const fvm::BoundarySet1D contornos{esquerda, direita};

    std::cout << "Exercicio 5.1 - condicoes de contorno do Exemplo 5.2\n\n";
    std::cout << "Condicao de contorno esquerda: " << esquerda << '\n';
    std::cout << "Condicao de contorno direita: " << direita << '\n';

//==============================================================================
// Definição da função fonte
//==============================================================================

    // f(x) é o termo fonte do Exemplo 5.2: d2phi/dx2 = f(x), com f(x) = x^2.
    const auto fonte = [](Real x) { return x * x; };

//==============================================================================
// Montagem do sistema pela FVMaker
//==============================================================================

    // fvm::Equation1D monta -laplacian(phi) == source(q), então para
    // reproduzir d2phi/dx2 = f(x) o termo fonte armazenado é -f(x). O sinal
    // é desfeito de volta pelo operator<< da FVMaker (TridiagonalSystem1D e
    // Equation1D), que devolve B_P na convenção do livro (positivo, igual a
    // f(x) integrado no volume).
    const fvm::Equation1D equacao{
        malha,
        fvm::Laplacian1D{},
        fvm::function_source_1d(malha, [&fonte](Real x) { return -fonte(x); }),
        contornos
    };

    std::cout << "\nExercicio 5.1 - coeficientes do Exemplo 5.2\n\n";
    std::cout << equacao << '\n';

    const fvm::TridiagonalSystem1D sistema = fvm::assemble_steady_1d(equacao);

//==============================================================================
// Verificação contra a Eq. do Exemplo 5.2
//==============================================================================

    const std::vector<std::vector<Real>> matriz = matriz_densa(sistema);
    const std::vector<Real> bp = lado_direito(sistema);

    const std::vector<std::vector<Real>> matriz_esperada{
        {3.0, -1.0, 0.0, 0.0, 0.0},
        {-1.0, 2.0, -1.0, 0.0, 0.0},
        {0.0, -1.0, 2.0, -1.0, 0.0},
        {0.0, 0.0, -1.0, 2.0, -1.0},
        {0.0, 0.0, 0.0, -1.0, 1.0}
    };

    const std::array<Real, 5> rhs_esperado{
        2.25,
        2.25,
        6.25,
        12.25,
        20.25
    };

    std::cout << "\nMatriz reconstruida a partir dos vetores\n";
    std::cout << "=======================================\n";
    imprimir_matriz(matriz, bp);

    unsigned aprovados{};
    unsigned total{};

    ++total;
    aprovados += registrar(
        "matriz coincide com a Eq. do exemplo",
        conferir_matriz(matriz, matriz_esperada)
    );

    ++total;
    aprovados += registrar(
        "lado direito coincide com a Eq. do exemplo",
        conferir_vetor(bp, std::span<const Real>{rhs_esperado})
    );

    bool tridiagonal = true;
    for (Size i = 0; i < matriz.size(); ++i) {
        for (Size j = 0; j < matriz[i].size(); ++j) {
            const bool nas_tres_diagonais =
                i == j || (i > 0 && j == i - 1) || (j == i + 1);

            if (!nas_tres_diagonais && !perto(matriz[i][j], 0.0)) {
                tridiagonal = false;
            }
        }
    }

    ++total;
    aprovados += registrar(
        "entradas fora das tres diagonais sao zero",
        tridiagonal
    );

    std::cout << "\nDiagonais nao nulas: inferior (i,i-1), principal (i,i), "
              << "superior (i,i+1)\n";

    return aprovados == total ? 0 : 1;
}
