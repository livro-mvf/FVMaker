// ============================================================================
// Arquivo: exercicio_23.cpp
// Projeto: FVMaker
// Versao: consulte <FVMaker/Core/Version.h>
// Descricao: Implementa uma malha uniforme 1D com contrato documentado.
// Autor: Joao Flavio Vieira de Vasconcellos
//
// SPDX-FileCopyrightText: 2.36 Joao Flavio Vieira de Vasconcellos
// SPDX-License-Identifier: BSD-3-Clause
// ============================================================================

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
        : xmin_{xmin},
          xmax_{xmax},
          numero_volumes_{numero_volumes},
          dx_{calcular_dx(xmin, xmax, numero_volumes)}
    {
    }

    [[nodiscard]] Real xmin() const noexcept { return xmin_; }
    [[nodiscard]] Real xmax() const noexcept { return xmax_; }
    [[nodiscard]] std::size_t numero_volumes() const noexcept { return numero_volumes_; }
    [[nodiscard]] Real dx() const noexcept { return dx_; }

    // Retorna o centro do volume de indice i.
    //
    // Escolhemos std::out_of_range, e nao apenas assert, porque a malha sera
    // usada como componente reutilizavel: um indice invalido precisa ser
    // diagnosticado tambem em execucoes otimizadas. Um assert poderia ser uma
    // sentinela adicional de desenvolvimento, mas nao e uma validacao da API.
    [[nodiscard]] Real centro(std::size_t indice) const
    {
        if (indice >= numero_volumes_) {
            throw std::out_of_range("Indice do volume fora do intervalo [0, N).");
        }

        return xmin_ + (static_cast<Real>(indice) + Real{0.5}) * dx_;
    }

    [[nodiscard]] Real face(std::size_t indice) const
    {
        if (indice > numero_volumes_) {
            throw std::out_of_range("Indice da face fora do intervalo [0, N].");
        }

        return xmin_ + static_cast<Real>(indice) * dx_;
    }

private:
    Real xmin_{};
    Real xmax_{};
    std::size_t numero_volumes_{};
    Real dx_{};

    static Real calcular_dx(Real xmin, Real xmax, std::size_t numero_volumes)
    {
        validar_parametros(xmin, xmax, numero_volumes);
        return (xmax - xmin) / static_cast<Real>(numero_volumes);
    }

    static void validar_parametros(Real xmin, Real xmax, std::size_t numero_volumes)
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
            throw std::invalid_argument("xmax deve ser estritamente maior que xmin.");
        }
    }
};

struct Placar {
    unsigned aprovados{};
    unsigned total{};
};

[[nodiscard]] bool aproximadamente_igual(Real a, Real b, Real tolerancia = Real{1.0e-12})
{
    return std::abs(a - b) <= tolerancia;
}

void registrar(Placar& placar, const std::string& descricao, bool passou)
{
    ++placar.total;
    if (passou) {
        ++placar.aprovados;
    }

    std::cout << (passou ? "[PASSOU] " : "[FALHOU] ") << descricao << '\n';
}

template <typename Excecao, typename Acao>
void testar_excecao(Placar& placar, const std::string& descricao, Acao acao)
{
    bool lancou = false;

    try {
        acao();
    } catch (const Excecao&) {
        lancou = true;
    }

    registrar(placar, descricao, lancou);
}

void imprimir_malha(const Malha1D& malha)
{
    std::cout << std::fixed << std::setprecision(6);
    std::cout << "Malha 1D uniforme\n";
    std::cout << "xmin = " << malha.xmin() << '\n';
    std::cout << "xmax = " << malha.xmax() << '\n';
    std::cout << "N    = " << malha.numero_volumes() << '\n';
    std::cout << "dx   = " << malha.dx() << "\n\n";

    std::cout << std::setw(6) << "i"
              << std::setw(14) << "face_i"
              << std::setw(14) << "centro_i"
              << std::setw(14) << "face_i+1" << '\n';

    for (std::size_t i = 0; i < malha.numero_volumes(); ++i) {
        std::cout << std::setw(6) << i
                  << std::setw(14) << malha.face(i)
                  << std::setw(14) << malha.centro(i)
                  << std::setw(14) << malha.face(i + 1) << '\n';
    }

    std::cout << '\n';
}

void executar_testes(Placar& placar)
{
    const Malha1D malha{Real{0.0}, Real{2.0}, 4};

    registrar(placar, "dx = (xmax - xmin) / N",
              aproximadamente_igual(malha.dx(), Real{0.5}));
    registrar(placar, "centro do primeiro volume",
              aproximadamente_igual(malha.centro(0), Real{0.25}));
    registrar(placar, "centro do ultimo volume",
              aproximadamente_igual(malha.centro(3), Real{1.75}));
    registrar(placar, "face direita do dominio",
              aproximadamente_igual(malha.face(4), Real{2.0}));

    testar_excecao<std::invalid_argument>(
        placar,
        "N = 0 e rejeitado no construtor",
        [] { Malha1D{Real{0.0}, Real{1.0}, 0}; }
    );

    testar_excecao<std::invalid_argument>(
        placar,
        "xmax <= xmin e rejeitado no construtor",
        [] { Malha1D{Real{1.0}, Real{1.0}, 4}; }
    );

    testar_excecao<std::out_of_range>(
        placar,
        "centro(N) e rejeitado",
        [&malha] { (void)malha.centro(4); }
    );

    testar_excecao<std::out_of_range>(
        placar,
        "face(N+1) e rejeitada",
        [&malha] { (void)malha.face(5); }
    );
}

} // namespace

int main()
{
    std::cout << "Exercicio Computacional 2.3\n";
    std::cout << "Implementacao da malha 1D\n";
    std::cout << "=========================\n\n";

    const Malha1D malha{Real{0.0}, Real{2.0}, 4};
    imprimir_malha(malha);

    Placar placar;
    executar_testes(placar);

    std::cout << "\nResumo: " << placar.aprovados << " de " << placar.total
              << " verificacoes aprovadas.\n";

    return (placar.aprovados == placar.total) ? 0 : 1;
}