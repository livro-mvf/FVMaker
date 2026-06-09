//==============================================================================
// SPDX-FileCopyrightText: 2026 Author Name
// SPDX-License-Identifier: MIT
//==============================================================================
// AVISO LEGAL / LEGAL DISCLAIMER
//
// [PT-BR]
// O código-fonte é fornecido sob a licença MIT, no estado em que se encontra
// ("as is"). Embora nos esforcemos para garantir o rigor matemático e a
// correção das implementações, a natureza da computação científica implica
// que inconsistências pontuais possam ocorrer. Caso identifique algum erro,
// comportamento inesperado, ou tenha sugestões de aprimoramento, seremos
// imensamente gratos se nos puder contactar através do e-mail
// livromvf@gmail.com. A sua contribuição é inestimável para o aperfeiçoamento
// contínuo deste livro e da sua biblioteca de apoio.
//
// [EN-GB]
// The source code is provided under the MIT Licence, on an "as is" basis.
// Whilst we endeavour to ensure mathematical rigour and correctness in the
// C++ implementations, the nature of scientific computing implies that
// occasional inconsistencies or errors may arise. Should you identify any
// bugs, unexpected behaviour, or have suggestions for improvement, we would
// be most grateful if you could reach out to us at livromvf@gmail.com.
// Your feedback is invaluable to the continuous refinement of this textbook
// and its accompanying library.
//==============================================================================

//==============================================================================
// Exercicio Computacional 2.2
// Implementacao da classe Malha1D uniforme
//
// Este programa implementa a classe Malha1D, que representa uma malha
// unidimensional uniforme seguindo o principio do encapsulamento.
//
// Dados do problema:
// xmin, coordenada inicial do dominio.
// xmax, coordenada final do dominio.
// N, numero de volumes de controle, com N > 0.
//
// A classe guarda os tres valores fundamentais e oferece, por meio de uma
// interface publica enxuta, consultas a esses valores, o calculo do tamanho
// de cada volume (∆x) e a coordenada do centro do i-esimo volume.
//
// O construtor valida as pre-condicoes e rejeita entradas inconsistentes
// (N <= 0 ou xmax <= xmin) lancando std::invalid_argument.
//
// O codigo e autocontido e nao depende da biblioteca em desenvolvimento.
//==============================================================================

//==============================================================================
// Bibliotecas padrao do C++
//==============================================================================

#include <cmath>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>

//==============================================================================
// Tipo real usado no programa
//==============================================================================

using Real = double;

//==============================================================================
// Classe Malha1D
//==============================================================================

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

[[nodiscard]] inline Real Malha1D::xmin() const noexcept { return m_xmin; }
[[nodiscard]] inline Real Malha1D::xmax() const noexcept { return m_xmax; }
[[nodiscard]] inline std::size_t Malha1D::numero_volumes() const noexcept { return m_numero_volumes; }
[[nodiscard]] inline Real Malha1D::tamanho_volume() const noexcept { return m_tamanho_volume; }

[[nodiscard]] inline Real Malha1D::centro(std::size_t indice) const
{
    if (indice >= m_numero_volumes) {
        throw std::out_of_range("Indice do volume fora do intervalo [0, N).");
    }
    return m_xmin + (static_cast<Real>(indice) + Real{0.5}) * m_tamanho_volume;
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
        throw std::invalid_argument("O numero de volumes deve ser maior que zero.");
    }
    if (!(xmax > xmin)) {
        throw std::invalid_argument("xmax deve ser estritamente maior que xmin.");
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
    std::cout << "∆x              = " << malha.tamanho_volume() << '\n';
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
    Real a, Real b, Real tolerancia = Real{1e-12}
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
    std::cout << (passou ? "[PASSOU] " : "[FALHOU] ") << descricao << '\n';
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

    std::cout << (excecao_lancada ? "[PASSOU] " : "[FALHOU] ") << descricao << '\n';
    if (!excecao_lancada) {
        std::cout << "  A construcao deveria ter lancado std::invalid_argument.\n";
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

    std::cout << (excecao_lancada ? "[PASSOU] " : "[FALHOU] ") << descricao << '\n';
    if (!excecao_lancada) {
        std::cout << "  A consulta deveria ter lancado std::out_of_range.\n";
    }
    return excecao_lancada;
}

//==============================================================================
// Programa principal
//==============================================================================

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
            if (testar_valor("xmin", malha.xmin(), 0.0)) ++testes_passaram;
            if (testar_valor("xmax", malha.xmax(), 1.0)) ++testes_passaram;
            if (testar_valor("N", static_cast<Real>(malha.numero_volumes()), 4.0)) ++testes_passaram;
            if (testar_valor("∆x", malha.tamanho_volume(), 0.25)) ++testes_passaram;
            if (testar_valor("centro(0)", malha.centro(0), 0.125)) ++testes_passaram;
            if (testar_valor("centro(3)", malha.centro(3), 0.875)) ++testes_passaram;
            if (testar_valor("face(4)", malha.face(4), 1.0)) ++testes_passaram;
        }
        std::cout << '\n';

        std::cout << "Teste 2: Malha com 5 volumes em [-2, 3]\n";
        std::cout << "----------------------------------------\n";
        {
            Malha1D malha(Real{-2.0}, Real{3.0}, 5);
            imprimir_malha(malha);

            testes_total += 4;
            if (testar_valor("∆x", malha.tamanho_volume(), 1.0)) ++testes_passaram;
            if (testar_valor("centro(0)", malha.centro(0), -1.5)) ++testes_passaram;
            if (testar_valor("centro(2)", malha.centro(2), 0.5)) ++testes_passaram;
            if (testar_valor("centro(4)", malha.centro(4), 2.5)) ++testes_passaram;
        }
        std::cout << '\n';

        std::cout << "Testes de validacao (pre-condicoes da construcao)\n";
        std::cout << "-------------------------------------------------\n";
        testes_total += 3;
        if (testar_excecao_construcao("N = 0 deve ser rejeitado", 0.0, 1.0, 0)) ++testes_passaram;
        if (testar_excecao_construcao("xmax < xmin deve ser rejeitado", 1.0, 0.0, 5)) ++testes_passaram;
        if (testar_excecao_construcao("xmax == xmin deve ser rejeitado", 1.0, 1.0, 5)) ++testes_passaram;
        std::cout << '\n';

        std::cout << "Testes de validacao (indices fora do intervalo)\n";
        std::cout << "-----------------------------------------------\n";
        {
            Malha1D malha(Real{0.0}, Real{1.0}, 4);
            testes_total += 2;
            if (testar_excecao_indice("centro(4) em malha de N=4 deve falhar", malha, 4)) ++testes_passaram;
            if (testar_excecao_indice("centro(100) deve falhar", malha, 100)) ++testes_passaram;
        }
        std::cout << '\n';

        std::cout << "Resumo\n";
        std::cout << "======\n";
        std::cout << "Testes aprovados: " << testes_passaram << " de " << testes_total << '\n';

        return (testes_passaram == testes_total) ? 0 : 1;

    } catch (const std::exception& erro) {
        std::cerr << "Erro inesperado: " << erro.what() << '\n';
        return 1;
    }
}