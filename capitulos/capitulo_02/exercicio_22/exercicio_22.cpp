// ============================================================================
// Arquivo: exercicio_22.cpp
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Implementa exercicio 22 no contexto de capitulos / capitulo_02 / exercicio_22.
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

#include <cmath>
#include <iomanip>
#include <iostream>


using Real = double;

namespace {

class Malha1D
{
public:
    inline Malha1D(
        Real xmin,
        Real xmax,
        std::size_t numero_volumes
    );

    [[nodiscard]] inline Real xmin() const noexcept;
    [[nodiscard]] inline Real xmax() const noexcept;
    [[nodiscard]] inline std::size_t numero_volumes() const noexcept;
    [[nodiscard]] inline Real tamanho_volume() const noexcept;
    [[nodiscard]] inline Real centro(std::size_t indice) const;
    [[nodiscard]] inline Real face(std::size_t indice) const;

private:
    Real m_xmin;
    Real m_xmax;
    std::size_t m_numero_volumes;
    Real m_tamanho_volume;

    static inline void validar(
        Real xmin,
        Real xmax,
        std::size_t numero_volumes
    );
};

//==============================================================================
// Implementacao dos metodos da classe
//==============================================================================

inline Malha1D::Malha1D(
    Real xmin,
    Real xmax,
    std::size_t numero_volumes
)
{
    validar(xmin, xmax, numero_volumes);

    m_xmin = xmin;
    m_xmax = xmax;
    m_numero_volumes = numero_volumes;
    m_tamanho_volume = (xmax - xmin) / static_cast<Real>(numero_volumes);
}

[[nodiscard]] inline Real Malha1D::xmin() const noexcept
{
    return m_xmin;
}

[[nodiscard]] inline Real Malha1D::xmax() const noexcept
{
    return m_xmax;
}

[[nodiscard]] inline std::size_t Malha1D::numero_volumes() const noexcept
{
    return m_numero_volumes;
}

[[nodiscard]] inline Real Malha1D::tamanho_volume() const noexcept
{
    return m_tamanho_volume;
}

[[nodiscard]] inline Real Malha1D::centro(std::size_t indice) const
{
    if (indice >= m_numero_volumes) {
        throw std::out_of_range("Indice do volume fora do intervalo [0, N).");
    }

    return m_xmin
        + (static_cast<Real>(indice) + Real{0.5}) * m_tamanho_volume;
}

[[nodiscard]] inline Real Malha1D::face(std::size_t indice) const
{
    if (indice > m_numero_volumes) {
        throw std::out_of_range("Indice da face fora do intervalo [0, N].");
    }

    return m_xmin + static_cast<Real>(indice) * m_tamanho_volume;
}

inline void Malha1D::validar(
    Real xmin,
    Real xmax,
    std::size_t numero_volumes
)
{
    if (!std::isfinite(xmin)) {
        throw std::invalid_argument("xmin deve ser finito.");
    }

    if (!std::isfinite(xmax)) {
        throw std::invalid_argument("xmax deve ser finito.");
    }

    if (numero_volumes == 0) {
        throw std::invalid_argument(
            "O numero de volumes deve ser maior que zero."
        );
    }

    if (!(xmax > xmin)) {
        throw std::invalid_argument(
            "xmax deve ser estritamente maior que xmin."
        );
    }
}

//==============================================================================
// Funcoes auxiliares de teste
//==============================================================================

inline void imprimir_malha(const Malha1D& malha)
{
    std::cout << std::fixed << std::setprecision(6);

    std::cout << "Malha 1D uniforme\n";
    std::cout << "=================\n";
    std::cout << "xmin            = " << malha.xmin() << '\n';
    std::cout << "xmax            = " << malha.xmax() << '\n';
    std::cout << "N               = " << malha.numero_volumes() << '\n';
    std::cout << "dx              = " << malha.tamanho_volume() << '\n';
    std::cout << '\n';

    std::cout << std::setw(6) << "i"
              << std::setw(16) << "face_esq"
              << std::setw(16) << "centro"
              << std::setw(16) << "face_dir"
              << '\n';

    for (std::size_t i = 0; i < malha.numero_volumes(); ++i) {
        std::cout << std::setw(6) << i
                  << std::setw(16) << malha.face(i)
                  << std::setw(16) << malha.centro(i)
                  << std::setw(16) << malha.face(i + 1)
                  << '\n';
    }

    std::cout << '\n';
}

[[nodiscard]] inline bool aproximadamente_igual(
    Real a,
    Real b,
    Real tolerancia = Real{1e-12}
)
{
    return std::abs(a - b) <= tolerancia;
}

inline bool testar_valor(
    const std::string& descricao,
    Real obtido,
    Real esperado
)
{
    const bool passou = aproximadamente_igual(obtido, esperado);

    std::cout << (passou ? "[PASSOU] " : "[FALHOU] ")
              << descricao << '\n';

    if (!passou) {
        std::cout << std::fixed << std::setprecision(12)
                  << "  obtido   = " << obtido << '\n'
                  << "  esperado = " << esperado << '\n';
    }

    return passou;
}

inline bool testar_excecao_construcao(
    const std::string& descricao,
    Real xmin,
    Real xmax,
    std::size_t numero_volumes
)
{
    bool excecao_lancada = false;

    try {
        Malha1D malha(xmin, xmax, numero_volumes);
        (void)malha;
    } catch (const std::invalid_argument&) {
        excecao_lancada = true;
    } catch (...) {
        excecao_lancada = false;
    }

    std::cout << (excecao_lancada ? "[PASSOU] " : "[FALHOU] ")
              << descricao << '\n';

    if (!excecao_lancada) {
        std::cout
            << "  A construcao deveria ter lancado std::invalid_argument.\n";
    }

    return excecao_lancada;
}

inline bool testar_excecao_indice(
    const std::string& descricao,
    const Malha1D& malha,
    std::size_t indice_fora
)
{
    bool excecao_lancada = false;

    try {
        (void)malha.centro(indice_fora);
    } catch (const std::out_of_range&) {
        excecao_lancada = true;
    } catch (...) {
        excecao_lancada = false;
    }

    std::cout << (excecao_lancada ? "[PASSOU] " : "[FALHOU] ")
              << descricao << '\n';

    if (!excecao_lancada) {
        std::cout << "  A consulta deveria ter lancado std::out_of_range.\n";
    }

    return excecao_lancada;
}

//==============================================================================
// Mensagem final didatica
//==============================================================================

inline void imprimir_mensagem_final()
{
    const int size = 80;

    //==========================================================================
    // Aplicacoes e recomendacoes
    //==========================================================================

    std::cout << "\nAplicacoes e recomendacoes\n";
    std::cout << std::string(size, '-') << '\n';
    std::cout << "1. A classe Malha1D organiza os dados fundamentais ";
    std::cout << "da malha uniforme.\n";
    std::cout << "2. O encapsulamento evita acesso direto aos detalhes ";
    std::cout << "internos da malha.\n";
    std::cout << "3. A validacao no construtor impede a criacao de ";
    std::cout << "malhas inconsistentes.\n";
    std::cout << "4. As funcoes centro(i) e face(i) fornecem uma ";
    std::cout << "interface simples e segura.\n";
    std::cout << "5. Este modelo e adequado como base para ";
    std::cout << "discretizacoes em volumes finitos.\n";
    std::cout << std::string(size, '-') << '\n';

    //==========================================================================
    // Conceitos demonstrados
    //==========================================================================

    std::cout << "\nConceitos demonstrados\n";
    std::cout << std::string(size, '-') << '\n';
    std::cout << "1. Definicao de uma classe para representar uma ";
    std::cout << "malha unidimensional.\n";
    std::cout << "2. Uso de atributos privados e metodos publicos ";
    std::cout << "de consulta.\n";
    std::cout << "3. Calculo do tamanho uniforme de cada volume ";
    std::cout << "de controle.\n";
    std::cout << "4. Calculo das coordenadas das faces e dos centros ";
    std::cout << "dos volumes.\n";
    std::cout << "5. Lancamento de excecoes para parametros e ";
    std::cout << "indices invalidos.\n";
    std::cout << "6. Uso de testes simples para verificar valores ";
    std::cout << "esperados.\n";
    std::cout << std::string(size, '-') << '\n';
}

} // namespace

int main()
{
    try {
        std::cout << "Exercicio Computacional 2.2\n";
        std::cout << "Implementacao da Malha1D uniforme\n";
        std::cout << "==================================\n\n";

        unsigned testes_passaram = 0;
        unsigned testes_total = 0;

        std::cout << "Teste 1: Malha com 4 volumes em [0, 1]\n";
        std::cout << "---------------------------------------\n";
        {
            Malha1D malha(Real{0.0}, Real{1.0}, 4);
            imprimir_malha(malha);

            testes_total += 7;

            if (testar_valor("xmin", malha.xmin(), 0.0)) {
                ++testes_passaram;
            }

            if (testar_valor("xmax", malha.xmax(), 1.0)) {
                ++testes_passaram;
            }

            if (
                testar_valor(
                    "N",
                    static_cast<Real>(malha.numero_volumes()),
                    4.0
                )
            ) {
                ++testes_passaram;
            }

            if (testar_valor("dx", malha.tamanho_volume(), 0.25)) {
                ++testes_passaram;
            }

            if (testar_valor("centro(0)", malha.centro(0), 0.125)) {
                ++testes_passaram;
            }

            if (testar_valor("centro(3)", malha.centro(3), 0.875)) {
                ++testes_passaram;
            }

            if (testar_valor("face(4)", malha.face(4), 1.0)) {
                ++testes_passaram;
            }
        }

        std::cout << '\n';

        std::cout << "Teste 2: Malha com 5 volumes em [-2, 3]\n";
        std::cout << "----------------------------------------\n";
        {
            Malha1D malha(Real{-2.0}, Real{3.0}, 5);
            imprimir_malha(malha);

            testes_total += 4;

            if (testar_valor("dx", malha.tamanho_volume(), 1.0)) {
                ++testes_passaram;
            }

            if (testar_valor("centro(0)", malha.centro(0), -1.5)) {
                ++testes_passaram;
            }

            if (testar_valor("centro(2)", malha.centro(2), 0.5)) {
                ++testes_passaram;
            }

            if (testar_valor("centro(4)", malha.centro(4), 2.5)) {
                ++testes_passaram;
            }
        }

        std::cout << '\n';

        std::cout << "Testes de validacao (pre-condicoes da construcao)\n";
        std::cout << "-------------------------------------------------\n";

        testes_total += 3;

        if (
            testar_excecao_construcao(
                "N = 0 deve ser rejeitado",
                0.0,
                1.0,
                0
            )
        ) {
            ++testes_passaram;
        }

        if (
            testar_excecao_construcao(
                "xmax < xmin deve ser rejeitado",
                1.0,
                0.0,
                5
            )
        ) {
            ++testes_passaram;
        }

        if (
            testar_excecao_construcao(
                "xmax == xmin deve ser rejeitado",
                1.0,
                1.0,
                5
            )
        ) {
            ++testes_passaram;
        }

        std::cout << '\n';

        std::cout << "Testes de validacao (indices fora do intervalo)\n";
        std::cout << "-----------------------------------------------\n";
        {
            Malha1D malha(Real{0.0}, Real{1.0}, 4);

            testes_total += 2;

            if (
                testar_excecao_indice(
                    "centro(4) em malha de N=4 deve falhar",
                    malha,
                    4
                )
            ) {
                ++testes_passaram;
            }

            if (
                testar_excecao_indice(
                    "centro(100) deve falhar",
                    malha,
                    100
                )
            ) {
                ++testes_passaram;
            }
        }

        std::cout << '\n';

        std::cout << "Resumo\n";
        std::cout << "======\n";
        std::cout << "Testes aprovados: " << testes_passaram
                  << " de " << testes_total << '\n';

        imprimir_mensagem_final();

        return (testes_passaram == testes_total) ? 0 : 1;

    } catch (const std::exception& erro) {
        std::cerr << "Erro inesperado: " << erro.what() << '\n';
        return 1;
    }
}
