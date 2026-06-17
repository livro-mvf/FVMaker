//==============================================================================
// SPDX-FileCopyrightText: 2026 FVMaker Team
// SPDX-License-Identifier: MIT
//==============================================================================
// Exercício Computacional 4.10
// Malha uniforme: as duas garantias
//==============================================================================

#include <FVGridMaker/OneDimensional/Distribution1D/Uniform1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/VolumeCentered1D.h>

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <stdexcept>
#include <string>

namespace {

using Real = fvgrid::Real;

constexpr Real kTol = 1.0e-12;

[[nodiscard]] Real converter_real(const std::string& texto, const std::string& nome) {
    std::size_t posicao{};
    const Real valor = std::stod(texto, &posicao);

    if (posicao != texto.size() || !std::isfinite(valor)) {
        throw std::invalid_argument(nome + " deve ser um número real finito.");
    }

    return valor;
}

[[nodiscard]] std::size_t converter_tamanho(const std::string& texto, const std::string& nome) {
    std::size_t posicao{};
    const auto valor = std::stoull(texto, &posicao);

    if (posicao != texto.size()) {
        throw std::invalid_argument(nome + " deve ser inteiro positivo.");
    }

    return static_cast<std::size_t>(valor);
}

[[nodiscard]] bool perto(Real a, Real b) {
    return std::abs(a - b) <= kTol * std::max<Real>({Real{1}, std::abs(a), std::abs(b)});
}

[[nodiscard]] Real soma(std::span<const Real> valores) {
    return std::accumulate(valores.begin(), valores.end(), Real{});
}

[[nodiscard]] bool registrar(const std::string& texto, bool passou) {
    std::cout << (passou ? "[PASSOU] " : "[FALHOU] ") << texto << '\n';
    return passou;
}

void validar(Real comprimento, std::size_t n) {
    if (!(comprimento > Real{}) || !std::isfinite(comprimento)) {
        throw std::invalid_argument("L deve ser finito e maior que zero.");
    }

    if (n == 0) {
        throw std::invalid_argument("N deve ser maior que zero.");
    }
}

void exportar_csv(const fvgrid::Axis1D& eixo) {
    std::ofstream arquivo{"exercicio_410_malha_uniforme.csv"};

    if (!arquivo) {
        throw std::runtime_error("Não foi possível criar exercicio_410_malha_uniforme.csv.");
    }

    arquivo << "i,xF_esquerda,xC,xF_direita,dxP\n";

    for (std::size_t i = 0; i < eixo.num_cells(); ++i) {
        arquivo << i << ',' << std::setprecision(17)
                << eixo.faces()[i] << ','
                << eixo.centers()[i] << ','
                << eixo.faces()[i + 1] << ','
                << eixo.cell_lengths()[i] << '\n';
    }
}

}  // namespace

int main(int argc, char** argv) {
    try {
        Real x_inicial = 0.0;
        Real comprimento = 1.0;
        std::size_t n = 50;

        if (argc != 1 && argc != 4) {
            throw std::invalid_argument("Uso: exercicio_410 [xI L N]");
        }

        if (argc == 4) {
            x_inicial = converter_real(argv[1], "xI");
            comprimento = converter_real(argv[2], "L");
            n = converter_tamanho(argv[3], "N");
        }

        validar(comprimento, n);

        const fvgrid::Axis1D eixo = fvgrid::Uniform1D::make(
            fvgrid::NVol{n},
            fvgrid::Length{comprimento},
            fvgrid::XInit{x_inicial},
            fvgrid::VolumeCentered1D{}
        );

        const Real dx_referencia = comprimento / static_cast<Real>(n);

        bool dx_constante = true;
        bool dx_positivo = true;

        for (const Real dx : eixo.cell_lengths()) {
            dx_constante = dx_constante && perto(dx, dx_referencia);
            dx_positivo = dx_positivo && dx > Real{};
        }

        std::cout << "Exercício 4.10 - malha uniforme\n";
        std::cout << "xI = " << x_inicial << ", L = " << comprimento
                  << ", N = " << n << "\n\n";

        std::cout << std::setw(6) << "i"
                  << std::setw(20) << "xF_i"
                  << std::setw(20) << "xC_i"
                  << std::setw(20) << "xF_{i+1}"
                  << std::setw(20) << "Delta xP_i" << '\n';

        for (std::size_t i = 0; i < eixo.num_cells(); ++i) {
            std::cout << std::setw(6) << i
                      << std::setw(20) << std::setprecision(12) << eixo.faces()[i]
                      << std::setw(20) << eixo.centers()[i]
                      << std::setw(20) << eixo.faces()[i + 1]
                      << std::setw(20) << eixo.cell_lengths()[i] << '\n';
        }

        std::cout << "\nVerificações automáticas\n";
        std::cout << "========================\n";

        unsigned aprovados{};
        unsigned total{};
        ++total; aprovados += registrar("Delta xP_i é constante", dx_constante);
        ++total; aprovados += registrar("Delta xP_i > 0", dx_positivo);
        ++total; aprovados += registrar("sum Delta xP_i = L", perto(soma(eixo.cell_lengths()), comprimento));

        std::cout << "\nDelta xP esperado = " << std::setprecision(17) << dx_referencia << '\n';
        std::cout << "sum Delta xP      = " << soma(eixo.cell_lengths()) << '\n';

        exportar_csv(eixo);
        std::cout << "CSV gerado: exercicio_410_malha_uniforme.csv\n";

        return aprovados == total ? 0 : 1;
    } catch (const std::exception& erro) {
        std::cerr << "Erro: " << erro.what() << '\n';
        return 1;
    }
}
