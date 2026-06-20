//==============================================================================
// SPDX-FileCopyrightText: 2026 FVMaker Team
// SPDX-License-Identifier: MIT
//==============================================================================
// Exercicio Computacional 6.6 - O piso da tolerancia
//==============================================================================

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string_view>
#include <vector>

namespace {

template <class T>
struct Resultado {
    bool convergiu{};
    std::size_t iteracoes{};
    T correcao{};
    T erro{};
};

template <class T>
[[nodiscard]] Resultado<T> gauss_seidel_phi_um(
    std::size_t n,
    T eps,
    std::size_t max_iter
) {
    std::vector<T> phi(n, T{});

    for (std::size_t iter = 1; iter <= max_iter; ++iter) {
        T max_delta{};

        for (std::size_t i = 0; i < n; ++i) {
            const T antigo = phi[i];
            if (i == 0) {
                phi[i] = (T{2} + phi[1]) / T{3};
            } else if (i + 1 == n) {
                phi[i] = phi[i - 1];
            } else {
                phi[i] = (phi[i - 1] + phi[i + 1]) / T{2};
            }
            max_delta = std::max(max_delta, std::abs(phi[i] - antigo));
        }

        if (max_delta <= eps) {
            T erro{};
            for (T valor : phi) {
                erro = std::max(erro, std::abs(valor - T{1}));
            }
            return {true, iter, max_delta, erro};
        }
    }

    T erro{};
    for (T valor : phi) {
        erro = std::max(erro, std::abs(valor - T{1}));
    }
    return {false, max_iter, T{}, erro};
}

template <class T>
void executar(std::string_view nome, std::size_t n) {
    std::cout << nome << "  epsilon_maquina = "
              << std::numeric_limits<T>::epsilon() << '\n';
    std::cout << std::setw(14) << "epsilon"
              << std::setw(12) << "conv."
              << std::setw(14) << "iter"
              << std::setw(18) << "correcao"
              << std::setw(18) << "erro real" << '\n';

    const T epsilons[] = {
        T{1e-3}, T{1e-5}, T{1e-7}, T{1e-9},
        T{1e-11}, T{1e-13}, T{1e-15}, T{1e-17}
    };

    for (T eps : epsilons) {
        const Resultado<T> r = gauss_seidel_phi_um<T>(n, eps, 500000);
        std::cout << std::setw(14) << static_cast<double>(eps)
                  << std::setw(12) << (r.convergiu ? "sim" : "nao")
                  << std::setw(14) << r.iteracoes
                  << std::setw(18) << static_cast<double>(r.correcao)
                  << std::setw(18) << static_cast<double>(r.erro) << '\n';
    }
}

}  // namespace

int main() {
    std::cout << std::scientific << std::setprecision(6);
    std::cout << "Exercicio 6.6 - piso da tolerancia\n\n";

    constexpr std::size_t n = 80;
    executar<float>("float", n);
    std::cout << '\n';
    executar<double>("double", n);
}
