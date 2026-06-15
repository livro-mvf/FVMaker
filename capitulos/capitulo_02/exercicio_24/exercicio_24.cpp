// SPDX-FileCopyrightText: 2026 FVMaker Team
// SPDX-License-Identifier: MIT
//
// Exercicio Computacional 2.4
// Titulo: Plano minimo de testes para a malha 1D.
//
// Objetivo:
//   Retomar a classe Malha1D e escrever testes manuais pequenos, legiveis e
//   capazes de apanhar erros comuns de implementacao.
//
// Modelo numerico:
//   Dominio [xmin, xmax] dividido em N volumes uniformes, com centros em
//   xmin + (i + 1/2) dx.
//
// Verificacoes:
//   O programa testa uma malha valida, rejeicao de parametros invalidos e
//   acesso a indice fora do intervalo permitido.

#include <cmath>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>

using Real = double;

namespace {

class Malha1D
{
public:
    Malha1D(Real xmin, Real xmax, std::size_t numero_volumes)
        : m_xmin{xmin},
          m_xmax{xmax},
          m_numero_volumes{numero_volumes}
    {
        validar(xmin, xmax, numero_volumes);
        m_dx = (m_xmax - m_xmin) / static_cast<Real>(m_numero_volumes);
    }

    [[nodiscard]] Real xmin() const noexcept
    {
        return m_xmin;
    }

    [[nodiscard]] Real xmax() const noexcept
    {
        return m_xmax;
    }

    [[nodiscard]] std::size_t numero_volumes() const noexcept
    {
        return m_numero_volumes;
    }

    [[nodiscard]] Real dx() const noexcept
    {
        return m_dx;
    }

    [[nodiscard]] Real centro(std::size_t indice) const
    {
        if (indice >= m_numero_volumes) {
            throw std::out_of_range("Indice do volume fora do intervalo [0, N).");
        }

        return m_xmin + (static_cast<Real>(indice) + Real{0.5}) * m_dx;
    }

private:
    Real m_xmin{};
    Real m_xmax{};
    std::size_t m_numero_volumes{};
    Real m_dx{};

    static void validar(Real xmin, Real xmax, std::size_t numero_volumes)
    {
        if (!std::isfinite(xmin)) {
            throw std::invalid_argument("xmin deve ser finito.");
        }

        if (!std::isfinite(xmax)) {
            throw std::invalid_argument("xmax deve ser finito.");
        }

        if (numero_volumes == 0) {
            throw std::invalid_argument("N deve ser maior que zero.");
        }

        if (!(xmax > xmin)) {
            throw std::invalid_argument("xmax deve ser maior que xmin.");
        }
    }
};

[[nodiscard]] bool aproximadamente_igual(
    Real a,
    Real b,
    Real tolerancia = Real{1.0e-12}
)
{
    return std::abs(a - b) <= tolerancia;
}

void imprimir_resultado(
    const std::string& nome,
    bool passou,
    const std::string& erro_que_apana
)
{
    std::cout << (passou ? "[PASSOU] " : "[FALHOU] ") << nome << '\n';
    std::cout << "         apanharia: " << erro_que_apana << '\n';
}

[[nodiscard]] bool testar_malha_valida()
{
    const Malha1D malha{Real{0.0}, Real{1.0}, 4};

    const bool passou =
        aproximadamente_igual(malha.dx(), Real{0.25})
        && aproximadamente_igual(malha.centro(0), Real{0.125})
        && aproximadamente_igual(malha.centro(1), Real{0.375})
        && aproximadamente_igual(malha.centro(2), Real{0.625})
        && aproximadamente_igual(malha.centro(3), Real{0.875});

    imprimir_resultado(
        "malha valida em [0, 1] com N = 4",
        passou,
        "formula errada para dx ou para o centro do volume"
    );

    return passou;
}

[[nodiscard]] bool testar_rejeicao_n_zero()
{
    bool lancou = false;

    try {
        const Malha1D malha{Real{0.0}, Real{1.0}, 0};
        (void)malha;
    } catch (const std::invalid_argument&) {
        lancou = true;
    }

    imprimir_resultado(
        "N = 0 deve ser rejeitado",
        lancou,
        "divisao por zero ou criacao de uma malha sem volumes"
    );

    return lancou;
}

[[nodiscard]] bool testar_rejeicao_intervalo_invalido()
{
    bool rejeitou_intervalo_nulo = false;
    bool rejeitou_intervalo_invertido = false;

    try {
        const Malha1D malha{Real{1.0}, Real{1.0}, 4};
        (void)malha;
    } catch (const std::invalid_argument&) {
        rejeitou_intervalo_nulo = true;
    }

    try {
        const Malha1D malha{Real{2.0}, Real{1.0}, 4};
        (void)malha;
    } catch (const std::invalid_argument&) {
        rejeitou_intervalo_invertido = true;
    }

    const bool passou = rejeitou_intervalo_nulo && rejeitou_intervalo_invertido;

    imprimir_resultado(
        "xmax <= xmin deve ser rejeitado",
        passou,
        "malha com comprimento nulo ou negativo"
    );

    return passou;
}

[[nodiscard]] bool testar_indice_fora_do_intervalo()
{
    bool lancou = false;

    try {
        const Malha1D malha{Real{0.0}, Real{1.0}, 4};
        (void)malha.centro(4);
    } catch (const std::out_of_range&) {
        lancou = true;
    }

    imprimir_resultado(
        "centro(N) deve ser rejeitado",
        lancou,
        "acesso fora do intervalo valido [0, N)"
    );

    return lancou;
}

} // namespace

int main()
{
    try {
        std::cout << "Exercicio Computacional 2.4\n";
        std::cout << "Plano minimo de testes para a Malha1D\n";
        std::cout << "=====================================\n\n";

        unsigned testes_aprovados = 0;
        unsigned testes_total = 0;

        ++testes_total;
        testes_aprovados += testar_malha_valida();

        ++testes_total;
        testes_aprovados += testar_rejeicao_n_zero();

        ++testes_total;
        testes_aprovados += testar_rejeicao_intervalo_invalido();

        ++testes_total;
        testes_aprovados += testar_indice_fora_do_intervalo();

        std::cout << "\nResumo\n";
        std::cout << "======\n";
        std::cout << "Testes aprovados: "
                  << testes_aprovados << " de " << testes_total << '\n';

        std::cout << "\nAplicacoes e recomendacoes\n";
        std::cout << "--------------------------\n";
        std::cout << "1. Cada teste declara qual erro de implementacao ele apanha.\n";
        std::cout << "2. Os testes manuais sao suficientes para proteger o contrato minimo.\n";
        std::cout << "3. Um framework de testes pode reaproveitar exatamente os mesmos casos.\n";

        return (testes_aprovados == testes_total) ? 0 : 1;
    } catch (const std::exception& erro) {
        std::cerr << "Erro inesperado: " << erro.what() << '\n';
        return 1;
    }
}
