//==============================================================================
// SPDX-FileCopyrightText: 2026 FVMaker Team
// SPDX-License-Identifier: MIT
//==============================================================================
// Utilidades didaticas para os exercicios computacionais do Capitulo 5.
//==============================================================================

#pragma once


#include <algorithm>
#include <iomanip>
#include <iostream>
#include <string_view>
#include <vector>

#include <FVGridMaker/OneDimensional/Axis1D/Axis1D.h>
#include <FVMaker/OneDimensional/Boundary/BoundaryCondition1D.h>
#include <FVMaker/OneDimensional/System/EquationContribution1D.h>


namespace capitulo_05 {

using Real = fvm::Real;
using Size = fvm::Size;

constexpr Real kTol = 1.0e-11;

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

inline void imprimir_vetores(const fvm::EquationContribution1D& coeficientes) {
    std::cout << "P               A_W             A_E             A_P             B_P\n";
    std::cout << "--------------------------------------------------------------------\n";

    for (Size p = 0; p < coeficientes.size(); ++p) {
        std::cout << std::setw(2) << p
                  << ' '
                  << std::setw(14) << coeficientes.aw()[p]
                  << ' '
                  << std::setw(14) << coeficientes.ae()[p]
                  << ' '
                  << std::setw(14) << coeficientes.ap()[p]
                  << ' '
                  << std::setw(14) << coeficientes.bp()[p] << '\n';
    }
}

inline void imprimir_matriz(
    const std::vector<std::vector<Real>>& matriz,
    std::span<const Real> rhs
) {
    for (Size i = 0; i < matriz.size(); ++i) {
        std::cout << "[ ";

        for (Real valor : matriz[i]) {
            std::cout << std::setw(10) << valor << ' ';
        }

        std::cout << "]  = " << rhs[i] << '\n';
    }
}

[[nodiscard]] inline bool conferir_matriz(
    const std::vector<std::vector<Real>>& obtida,
    const std::vector<std::vector<Real>>& esperada
) {
    if (obtida.size() != esperada.size()) {
        return false;
    }

    for (Size i = 0; i < obtida.size(); ++i) {
        if (obtida[i].size() != esperada[i].size()) {
            return false;
        }

        for (Size j = 0; j < obtida[i].size(); ++j) {
            if (!perto(obtida[i][j], esperada[i][j])) {
                return false;
            }
        }
    }

    return true;
}

[[nodiscard]] inline bool conferir_vetor(
    std::span<const Real> obtido,
    std::span<const Real> esperado
) {
    if (obtido.size() != esperado.size()) {
        return false;
    }

    for (Size i = 0; i < obtido.size(); ++i) {
        if (!perto(obtido[i], esperado[i])) {
            return false;
        }
    }

    return true;
}

}  // namespace capitulo_05
