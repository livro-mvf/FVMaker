//==============================================================================
// SPDX-FileCopyrightText: 2026 FVMaker Team
// SPDX-License-Identifier: MIT
//==============================================================================
// Utilidades didaticas para os exercicios computacionais do Capitulo 6.
//==============================================================================

#pragma once

//==============================================================================
// Header c++
//==============================================================================
#include <cmath>
#include <iostream>
#include <chrono>

//==============================================================================
// Header FVGridMaker
//==============================================================================
#include <FVGridMaker/OneDimensional/Axis1D/Axis1D.h>
#include <FVGridMaker/OneDimensional/Distribution1D/Uniform1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/FaceCentered1D.h>
#include <FVMaker/Algebra/ErrorNorms.h>
#include <FVMaker/OneDimensional/Boundary/BoundaryCondition1D.h>
#include <FVMaker/OneDimensional/System/EquationContribution1D.h>

namespace capitulo_06 {

using Real = fvm::Real;
using Size = fvm::Size;

constexpr Real kTol = 1.0e-10;

[[nodiscard]] inline bool perto(Real a, Real b, Real tol = kTol) {
    return std::abs(a - b) <=
           tol * std::max<Real>({Real{1}, std::abs(a), std::abs(b)});
}

[[nodiscard]] inline bool registrar(std::string_view texto, bool passou) {
    std::cout << (passou ? "[PASSOU] " : "[FALHOU] ") << texto << '\n';
    return passou;
}

template <class Function>
[[nodiscard]] inline Real fonte_um_ponto(
    const fvgrid::Axis1D& eixo,
    Size p,
    Function&& f
) {
    return f(eixo.centers()[p]) * eixo.cell_lengths()[p];
}

struct CoeficientesContorno {
    Real aw{};
    Real ae{};
    Real ap{};
    Real bp{};
};

[[nodiscard]] inline CoeficientesContorno coeficientes_contorno(
    fvm::BoundarySide1D lado,
    const fvm::BoundaryCondition1D& condicao,
    Real x_contorno,
    Real distancia_contorno,
    Real termo_fonte,
    Real coeficiente_vizinho
) {
    const Real alpha = condicao.alpha(x_contorno);
    const Real beta = condicao.beta(x_contorno);
    const Real gamma = condicao.gamma(x_contorno);

    if (lado == fvm::BoundarySide1D::left) {
        const Real denom = alpha * distancia_contorno - beta;
        return CoeficientesContorno{
            Real{},
            coeficiente_vizinho,
            coeficiente_vizinho + alpha / denom,
            termo_fonte + gamma / denom
        };
    }

    const Real denom = alpha * distancia_contorno + beta;
    return CoeficientesContorno{
        coeficiente_vizinho,
        Real{},
        coeficiente_vizinho + alpha / denom,
        termo_fonte + gamma / denom
    };
}

template <class Function>
[[nodiscard]] inline fvm::EquationContribution1D montar_coeficientes_poisson(
    const fvgrid::Axis1D& eixo,
    Function&& f,
    const fvm::BoundaryCondition1D& esquerda,
    const fvm::BoundaryCondition1D& direita
) {
    const Size n = eixo.num_cells();
    fvm::EquationContribution1D coeficientes{n};

    const auto xF = eixo.faces();
    const auto xC = eixo.centers();

    for (Size p = 1; p + 1 < n; ++p) {
        const Real aw = Real{1} / (xC[p] - xC[p - 1]);
        const Real ae = Real{1} / (xC[p + 1] - xC[p]);

        coeficientes.aw()[p] = aw;
        coeficientes.ae()[p] = ae;
        coeficientes.ap()[p] = aw + ae;
        coeficientes.bp()[p] = fonte_um_ponto(eixo, p, f);
    }

    {
        const Real d = xC[0] - xF[0];
        const Real ae = Real{1} / (xC[1] - xC[0]);
        const CoeficientesContorno c = coeficientes_contorno(
            fvm::BoundarySide1D::left,
            esquerda,
            xF[0],
            d,
            fonte_um_ponto(eixo, 0, f),
            ae
        );

        coeficientes.aw()[0] = c.aw;
        coeficientes.ae()[0] = c.ae;
        coeficientes.ap()[0] = c.ap;
        coeficientes.bp()[0] = c.bp;
    }

    {
        const Size p = n - 1;
        const Real d = xF[n] - xC[p];
        const Real aw = Real{1} / (xC[p] - xC[p - 1]);
        const CoeficientesContorno c = coeficientes_contorno(
            fvm::BoundarySide1D::right,
            direita,
            xF[n],
            d,
            fonte_um_ponto(eixo, p, f),
            aw
        );

        coeficientes.aw()[p] = c.aw;
        coeficientes.ae()[p] = c.ae;
        coeficientes.ap()[p] = c.ap;
        coeficientes.bp()[p] = c.bp;
    }

    return coeficientes;
}

[[nodiscard]] inline fvgrid::Axis1D malha_uniforme_exemplo(
    Size n = 5,
    Real comprimento = 5.0,
    Real x_inicial = 0.0
) {
    return fvgrid::Uniform1D::make(
        fvgrid::NVol{n},
        fvgrid::Length{comprimento},
        fvgrid::XInit{x_inicial},
        fvgrid::FaceCentered1D{}
    );
}

[[nodiscard]] inline fvm::EquationContribution1D coeficientes_exemplo(
    Size n = 5,
    Real comprimento = 5.0,
    Real x_inicial = 0.0
) {
    const fvgrid::Axis1D eixo =
        malha_uniforme_exemplo(n, comprimento, x_inicial);

    return montar_coeficientes_poisson(
        eixo,
        [](Real x) { return x * x; },
        fvm::dirichlet_1d(1.0),
        fvm::neumann_1d(0.0)
    );
}

[[nodiscard]] inline fvm::EquationContribution1D coeficientes_phi_um(
    Size n = 5
) {
    fvm::EquationContribution1D coeficientes{n};

    for (Size i = 0; i < n; ++i) {
        coeficientes.aw()[i] = i > 0 ? Real{1} : Real{};
        coeficientes.ae()[i] = i + 1 < n ? Real{1} : Real{};
        coeficientes.ap()[i] = coeficientes.aw()[i] + coeficientes.ae()[i];
        coeficientes.bp()[i] = Real{};
    }

    coeficientes.ap()[0] = Real{3};
    coeficientes.bp()[0] = Real{2};

    return coeficientes;
}

[[nodiscard]] inline fvm::DenseVector vetor_constante(Size n, Real valor) {
    return fvm::DenseVector{n, valor};
}

[[nodiscard]] inline std::vector<std::vector<Real>> matriz_densa(
    const fvm::EquationContribution1D& coeficientes
) {
    const Size n = coeficientes.size();
    std::vector<std::vector<Real>> matriz(n, std::vector<Real>(n, Real{}));

    for (Size p = 0; p < n; ++p) {
        matriz[p][p] = coeficientes.ap()[p];

        if (p > 0) {
            matriz[p][p - 1] = -coeficientes.aw()[p];
        }

        if (p + 1 < n) {
            matriz[p][p + 1] = -coeficientes.ae()[p];
        }
    }

    return matriz;
}

inline void imprimir_matriz(
    const std::vector<std::vector<Real>>& matriz,
    std::span<const Real> rhs
) {
    for (Size i = 0; i < matriz.size(); ++i) {
        std::cout << "[ ";

        for (Real valor : matriz[i]) {
            std::cout << std::setw(14) << valor << ' ';
        }

        std::cout << "]  = " << rhs[i] << '\n';
    }
}

struct TDMATrace {
    fvm::DenseVector t;
    fvm::DenseVector q;
    fvm::DenseVector phi;
};

[[nodiscard]] inline TDMATrace tdma_com_rastro(
    const fvm::EquationContribution1D& coeficientes
) {
    const Size n = coeficientes.size();
    const auto aw = coeficientes.aw().values();
    const auto ap = coeficientes.ap().values();
    const auto ae = coeficientes.ae().values();
    const auto bp = coeficientes.bp().values();

    TDMATrace rastro{
        .t = fvm::DenseVector{n},
        .q = fvm::DenseVector{n},
        .phi = fvm::DenseVector{n}
    };

    rastro.t[0] = ae[0] / ap[0];
    rastro.q[0] = bp[0] / ap[0];

    for (Size i = 1; i < n; ++i) {
        const Real aux = Real{1} / (ap[i] - aw[i] * rastro.t[i - 1]);
        rastro.t[i] = ae[i] * aux;
        rastro.q[i] = (bp[i] + aw[i] * rastro.q[i - 1]) * aux;
    }

    rastro.phi[n - 1] = rastro.q[n - 1];

    for (Size reverse_index = n - 1; reverse_index > 0; --reverse_index) {
        const Size i = reverse_index - 1;
        rastro.phi[i] = rastro.t[i] * rastro.phi[i + 1] + rastro.q[i];
    }

    return rastro;
}

inline void imprimir_vetor(std::string_view nome, const fvm::DenseVector& v) {
    std::cout << nome << '\n';
    for (Size i = 0; i < v.size(); ++i) {
        std::cout << std::setw(6) << i
                  << std::setw(18) << v[i] << '\n';
    }
}

[[nodiscard]] inline fvm::DenseVector solucao_analitica(
    const fvgrid::Axis1D& eixo
) {
    fvm::DenseVector valores{eixo.num_cells()};

    for (Size i = 0; i < eixo.num_cells(); ++i) {
        const Real x = eixo.centers()[i];
        valores[i] = -std::pow(x, Real{4}) / Real{12}
                    + Real{125} * x / Real{3}
                    + Real{1};
    }

    return valores;
}

[[nodiscard]] inline Real erro_infinito(
    const fvm::DenseVector& numerica,
    const fvm::DenseVector& analitica
) {
    fvm::DenseVector erro = numerica - analitica;
    return fvm::norm_infinity(erro);
}

[[nodiscard]] inline Real rms(const fvm::DenseVector& v) {
    if (v.empty()) {
        return Real{};
    }

    Real soma{};
    for (Real valor : v.values()) {
        soma += valor * valor;
    }

    return std::sqrt(soma / static_cast<Real>(v.size()));
}

[[nodiscard]] inline Real residuo_rms(
    const fvm::TridiagonalSystem1D& sistema,
    const fvm::DenseVector& solucao
) {
    return rms(sistema * solucao - sistema.rhs());
}

template <class Function>
[[nodiscard]] inline Real menor_tempo_de(
    int repeticoes,
    Function&& f
) {
    Real melhor = std::numeric_limits<Real>::max();

    for (int i = 0; i < repeticoes; ++i) {
        const auto inicio = std::chrono::steady_clock::now();
        f();
        const auto fim = std::chrono::steady_clock::now();
        const std::chrono::duration<Real> duracao = fim - inicio;
        melhor = std::min(melhor, duracao.count());
    }

    return melhor;
}

}  // namespace capitulo_06
