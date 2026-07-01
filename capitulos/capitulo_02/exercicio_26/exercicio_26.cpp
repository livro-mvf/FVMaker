// ============================================================================
// Arquivo: exercicio_26.cpp
// Projeto: FVMaker
// Versao: consulte <FVMaker/Core/Version.h>
// Descricao: Esconde SoA/AoS atras da mesma interface publica de Malha1D.
// Autor: Joao Flavio Vieira de Vasconcellos
//
// SPDX-FileCopyrightText: 2026 Joao Flavio Vieira de Vasconcellos
// SPDX-License-Identifier: BSD-3-Clause
// ============================================================================

#include <cmath>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

using Real = double;

namespace {

void validar_malha(Real xmin, Real xmax, std::size_t numero_volumes)
{
    if (!std::isfinite(xmin) || !std::isfinite(xmax)) {
        throw std::invalid_argument("Os extremos da malha devem ser finitos.");
    }
    if (numero_volumes == 0) {
        throw std::invalid_argument("N deve ser maior que zero.");
    }
    if (!(xmax > xmin)) {
        throw std::invalid_argument("xmax deve ser maior que xmin.");
    }
}

[[nodiscard]] Real centro_uniforme(Real xmin, Real dx, std::size_t indice)
{
    return xmin + (static_cast<Real>(indice) + Real{0.5}) * dx;
}

// Implementacao SoA (structure of arrays).
// Centros, phi e fonte ficam em vetores separados. Essa decisao fica
// encapsulada na classe; quem usa a malha enxerga apenas a interface publica.
class Malha1DSoA
{
public:
    Malha1DSoA(Real xmin, Real xmax, std::size_t numero_volumes)
        : xmin_{xmin},
          xmax_{xmax},
          dx_{calcular_dx(xmin, xmax, numero_volumes)},
          centros_(numero_volumes),
          phi_(numero_volumes, Real{}),
          fonte_(numero_volumes, Real{})
    {
        for (std::size_t i = 0; i < numero_volumes; ++i) {
            centros_[i] = centro_uniforme(xmin_, dx_, i);
        }
    }

    [[nodiscard]] std::size_t numero_volumes() const noexcept { return centros_.size(); }
    [[nodiscard]] Real centro(std::size_t indice) const { verificar_indice(indice); return centros_[indice]; }
    [[nodiscard]] Real phi(std::size_t indice) const { verificar_indice(indice); return phi_[indice]; }
    [[nodiscard]] Real fonte(std::size_t indice) const { verificar_indice(indice); return fonte_[indice]; }

    void definir_phi(std::size_t indice, Real valor)
    {
        verificar_indice(indice);
        phi_[indice] = valor;
    }

    void definir_fonte(std::size_t indice, Real valor)
    {
        verificar_indice(indice);
        fonte_[indice] = valor;
    }

private:
    Real xmin_{};
    Real xmax_{};
    Real dx_{};
    std::vector<Real> centros_;
    std::vector<Real> phi_;
    std::vector<Real> fonte_;

    static Real calcular_dx(Real xmin, Real xmax, std::size_t numero_volumes)
    {
        validar_malha(xmin, xmax, numero_volumes);
        return (xmax - xmin) / static_cast<Real>(numero_volumes);
    }

    void verificar_indice(std::size_t indice) const
    {
        if (indice >= centros_.size()) {
            throw std::out_of_range("Indice fora do intervalo [0, N).");
        }
    }
};

// Implementacao AoS (array of structures).
// Cada volume guarda centro, phi e fonte juntos. A interface publica permanece
// igual a da versao SoA, entao o laco externo nao depende do arranjo interno.
class Malha1DAoS
{
public:
    Malha1DAoS(Real xmin, Real xmax, std::size_t numero_volumes)
        : xmin_{xmin},
          xmax_{xmax},
          dx_{calcular_dx(xmin, xmax, numero_volumes)},
          volumes_(numero_volumes)
    {
        for (std::size_t i = 0; i < numero_volumes; ++i) {
            volumes_[i].centro = centro_uniforme(xmin_, dx_, i);
        }
    }

    [[nodiscard]] std::size_t numero_volumes() const noexcept { return volumes_.size(); }
    [[nodiscard]] Real centro(std::size_t indice) const { verificar_indice(indice); return volumes_[indice].centro; }
    [[nodiscard]] Real phi(std::size_t indice) const { verificar_indice(indice); return volumes_[indice].phi; }
    [[nodiscard]] Real fonte(std::size_t indice) const { verificar_indice(indice); return volumes_[indice].fonte; }

    void definir_phi(std::size_t indice, Real valor)
    {
        verificar_indice(indice);
        volumes_[indice].phi = valor;
    }

    void definir_fonte(std::size_t indice, Real valor)
    {
        verificar_indice(indice);
        volumes_[indice].fonte = valor;
    }

private:
    struct Volume
    {
        Real centro{};
        Real phi{};
        Real fonte{};
    };

    Real xmin_{};
    Real xmax_{};
    Real dx_{};
    std::vector<Volume> volumes_;

    static Real calcular_dx(Real xmin, Real xmax, std::size_t numero_volumes)
    {
        validar_malha(xmin, xmax, numero_volumes);
        return (xmax - xmin) / static_cast<Real>(numero_volumes);
    }

    void verificar_indice(std::size_t indice) const
    {
        if (indice >= volumes_.size()) {
            throw std::out_of_range("Indice fora do intervalo [0, N).");
        }
    }
};

template <typename Malha>
void inicializar_campos(Malha& malha)
{
    for (std::size_t i = 0; i < malha.numero_volumes(); ++i) {
        const Real x = malha.centro(i);
        malha.definir_phi(i, std::sin(x));
        malha.definir_fonte(i, Real{1.0} + x * x);
    }
}

template <typename Malha>
void atualizar_phi_pela_interface(Malha& malha, Real dt)
{
    for (std::size_t i = 0; i < malha.numero_volumes(); ++i) {
        malha.definir_phi(i, malha.phi(i) + dt * malha.fonte(i));
    }
}

template <typename Malha>
[[nodiscard]] Real soma_phi(const Malha& malha)
{
    Real soma{};
    for (std::size_t i = 0; i < malha.numero_volumes(); ++i) {
        soma += malha.phi(i);
    }
    return soma;
}

template <typename Malha>
void executar_experimento(Malha& malha, Real dt)
{
    inicializar_campos(malha);
    atualizar_phi_pela_interface(malha, dt);
}

[[nodiscard]] bool aproximadamente_igual(Real a, Real b, Real tolerancia = Real{1.0e-12})
{
    return std::abs(a - b) <= tolerancia;
}

} // namespace

int main()
{
    std::cout << "Exercicio Computacional 2.6\n";
    std::cout << "DOD escondido por dentro da classe\n";
    std::cout << "==================================\n\n";

    constexpr Real xmin = Real{0.0};
    constexpr Real xmax = Real{1.0};
    constexpr std::size_t n = 8;
    constexpr Real dt = Real{0.05};

    Malha1DSoA malha_soa{xmin, xmax, n};
    Malha1DAoS malha_aos{xmin, xmax, n};

    executar_experimento(malha_soa, dt);
    executar_experimento(malha_aos, dt);

    const Real soma_soa = soma_phi(malha_soa);
    const Real soma_aos = soma_phi(malha_aos);
    const bool somas_iguais = aproximadamente_igual(soma_soa, soma_aos);

    std::cout << std::fixed << std::setprecision(12);
    std::cout << "soma phi SoA = " << soma_soa << '\n';
    std::cout << "soma phi AoS = " << soma_aos << '\n';
    std::cout << (somas_iguais ? "[PASSOU] " : "[FALHOU] ")
              << "o mesmo laco externo funciona para SoA e AoS\n\n";

    std::cout << "Fronteira de projeto\n";
    std::cout << "--------------------\n";
    std::cout << "A classe decide o arranjo interno de memoria. Quem chama depende apenas\n";
    std::cout << "da interface publica: consultar centro, phi e fonte, e atribuir novos\n";
    std::cout << "valores. Essa fronteira permite otimizar por dentro sem quebrar o codigo\n";
    std::cout << "que executa o experimento numerico.\n";

    return somas_iguais ? 0 : 1;
}