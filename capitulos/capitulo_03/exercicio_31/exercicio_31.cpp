//==============================================================================
// SPDX-FileCopyrightText: 2026 FVMaker Team
// SPDX-License-Identifier: MIT
//==============================================================================
// Exercício Computacional 3.1
// Gerador de malhas aleatórias
//
// O programa recebe xI, L > 0 e N > 2; gera N - 2 posições aleatórias no
// interior de (xI, xI + L); fixa os extremos; ordena os nós; imprime x_i e
// Delta x_i; e verifica automaticamente as propriedades que servem como
// gabarito para uma malha aleatória válida.
//==============================================================================

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <limits>
#include <numeric>
#include <random>
#include <stdexcept>
#include <string>
#include <vector>

using Real = double;

struct MalhaAleatoria1D {
    Real x_inicial{};
    Real comprimento{};
    std::uint64_t semente{};
    std::vector<Real> nos;
};

[[nodiscard]] Real converter_real(
    const std::string& texto,
    const std::string& nome
) {
    std::size_t posicao{};
    const Real valor = std::stod(texto, &posicao);

    if (posicao != texto.size()) {
        throw std::invalid_argument(
            "O argumento '" + nome + "' contém caracteres inválidos."
        );
    }

    if (!std::isfinite(valor)) {
        throw std::invalid_argument(
            "O argumento '" + nome + "' deve ser finito."
        );
    }

    return valor;
}

[[nodiscard]] std::size_t converter_tamanho(
    const std::string& texto,
    const std::string& nome
) {
    std::size_t posicao{};
    const unsigned long valor = std::stoul(texto, &posicao);

    if (posicao != texto.size()) {
        throw std::invalid_argument(
            "O argumento '" + nome + "' contém caracteres inválidos."
        );
    }

    return static_cast<std::size_t>(valor);
}

[[nodiscard]] std::uint64_t converter_semente(
    const std::string& texto,
    const std::string& nome
) {
    std::size_t posicao{};
    const unsigned long long valor = std::stoull(texto, &posicao);

    if (posicao != texto.size()) {
        throw std::invalid_argument(
            "O argumento '" + nome + "' contém caracteres inválidos."
        );
    }

    return static_cast<std::uint64_t>(valor);
}

void validar_parametros(
    Real x_inicial,
    Real comprimento,
    std::size_t numero_nos
) {
    if (!std::isfinite(x_inicial)) {
        throw std::invalid_argument("xI deve ser finito.");
    }

    if (!std::isfinite(comprimento) || !(comprimento > Real{})) {
        throw std::invalid_argument("L deve ser finito e maior que zero.");
    }

    if (numero_nos <= 2) {
        throw std::invalid_argument("N deve ser maior que 2.");
    }

    if (!(x_inicial + comprimento > x_inicial)) {
        throw std::invalid_argument(
            "xI + L deve ser maior que xI na aritmética usada."
        );
    }
}

[[nodiscard]] MalhaAleatoria1D gerar_malha_aleatoria(
    Real x_inicial,
    Real comprimento,
    std::size_t numero_nos,
    std::uint64_t semente
) {
    validar_parametros(x_inicial, comprimento, numero_nos);

    const Real x_final = x_inicial + comprimento;
    const Real menor_interno = std::nextafter(x_inicial, x_final);
    const Real maior_interno = std::nextafter(x_final, x_inicial);

    if (!(maior_interno > menor_interno)) {
        throw std::invalid_argument(
            "O domínio não possui intervalo interno representável."
        );
    }

    std::mt19937_64 gerador{semente};
    std::uniform_real_distribution<Real> distribuicao{
        menor_interno,
        maior_interno
    };

    std::vector<Real> nos;
    nos.reserve(numero_nos);
    nos.push_back(x_inicial);

    for (std::size_t i = 0; i < numero_nos - 2; ++i) {
        nos.push_back(distribuicao(gerador));
    }

    nos.push_back(x_final);
    std::sort(nos.begin(), nos.end());

    return MalhaAleatoria1D{x_inicial, comprimento, semente, nos};
}

[[nodiscard]] std::vector<Real> calcular_espacamentos(
    const std::vector<Real>& nos
) {
    std::vector<Real> espacamentos;
    espacamentos.reserve(nos.size() - 1);

    for (std::size_t i = 1; i < nos.size(); ++i) {
        espacamentos.push_back(nos[i] - nos[i - 1]);
    }

    return espacamentos;
}

[[nodiscard]] Real soma(const std::vector<Real>& valores) {
    return std::accumulate(valores.begin(), valores.end(), Real{});
}

[[nodiscard]] bool aproximadamente_igual(
    Real a,
    Real b,
    Real tolerancia
) {
    return std::abs(a - b) <= tolerancia;
}

[[nodiscard]] bool nos_estritamente_crescentes(
    const std::vector<Real>& nos
) {
    for (std::size_t i = 1; i < nos.size(); ++i) {
        if (!(nos[i - 1] < nos[i])) {
            return false;
        }
    }

    return true;
}

[[nodiscard]] bool espacamentos_positivos(
    const std::vector<Real>& espacamentos
) {
    return std::all_of(
        espacamentos.begin(),
        espacamentos.end(),
        [](Real dx) {
            return dx > Real{};
        }
    );
}

void imprimir_malha(const MalhaAleatoria1D& malha) {
    const std::vector<Real> espacamentos =
        calcular_espacamentos(malha.nos);

    std::cout << "Malha aleatória\n";
    std::cout << "===============\n";
    std::cout << "xI      = " << malha.x_inicial << '\n';
    std::cout << "L       = " << malha.comprimento << '\n';
    std::cout << "N       = " << malha.nos.size() << '\n';
    std::cout << "semente = " << malha.semente << "\n\n";

    std::cout << std::setw(6) << "i"
              << std::setw(24) << "x_i"
              << std::setw(24) << "Delta x_i" << '\n';

    std::cout << std::setw(6) << 0
              << std::setw(24) << malha.nos.front()
              << std::setw(24) << "--" << '\n';

    for (std::size_t i = 1; i < malha.nos.size(); ++i) {
        std::cout << std::setw(6) << i
                  << std::setw(24) << malha.nos[i]
                  << std::setw(24) << espacamentos[i - 1] << '\n';
    }
}

[[nodiscard]] bool registrar_teste(
    const std::string& descricao,
    bool passou
) {
    std::cout << (passou ? "[PASSOU] " : "[FALHOU] ")
              << descricao << '\n';
    return passou;
}

[[nodiscard]] unsigned verificar_malha(const MalhaAleatoria1D& malha) {
    constexpr Real tolerancia = 1.0e-12;

    const Real x_final = malha.x_inicial + malha.comprimento;
    const std::vector<Real> espacamentos =
        calcular_espacamentos(malha.nos);

    unsigned aprovados{};
    unsigned total{};

    std::cout << "\nVerificações automáticas\n";
    std::cout << "========================\n";

    ++total;
    aprovados += registrar_teste(
        "x_0 = xI",
        malha.nos.front() == malha.x_inicial
    );

    ++total;
    aprovados += registrar_teste(
        "x_{N-1} = xI + L",
        malha.nos.back() == x_final
    );

    ++total;
    aprovados += registrar_teste(
        "x_i < x_{i+1}",
        nos_estritamente_crescentes(malha.nos)
    );

    ++total;
    aprovados += registrar_teste(
        "Delta x_i > 0",
        espacamentos_positivos(espacamentos)
    );

    ++total;
    aprovados += registrar_teste(
        "sum Delta x_i = L",
        aproximadamente_igual(
            soma(espacamentos),
            malha.comprimento,
            tolerancia
        )
    );

    std::cout << "\nResumo\n";
    std::cout << "======\n";
    std::cout << "Verificações aprovadas: "
              << aprovados << " de " << total << '\n';
    std::cout << "Soma dos espaçamentos: "
              << soma(espacamentos) << '\n';
    std::cout << "Erro na soma: "
              << std::abs(soma(espacamentos) - malha.comprimento) << '\n';

    return (aprovados == total) ? 0u : 1u;
}

void imprimir_observacao_final() {
    std::cout << "\nObservação\n";
    std::cout << "==========\n";
    std::cout << "Os valores numéricos mudam quando a semente muda. ";
    std::cout << "As propriedades verificadas acima devem permanecer ";
    std::cout << "verdadeiras em toda execução válida.\n";
    std::cout << "Use os nós e os Delta x_i impressos aqui para alimentar ";
    std::cout << "as fórmulas não uniformes da Seção 3.2.\n";
}

void imprimir_uso(const char* nome_programa) {
    std::cout << "Uso:\n";
    std::cout << "  " << nome_programa << '\n';
    std::cout << "  " << nome_programa << " xI L N [semente]\n\n";
    std::cout << "Exemplo:\n";
    std::cout << "  " << nome_programa << " 0.0 1.0 12 2026\n";
}

int main(int argc, char* argv[]) {
    try {
        Real x_inicial = 0.0;
        Real comprimento = 1.0;
        std::size_t numero_nos = 12;
        std::uint64_t semente = 2026;

        if (!(argc == 1 || argc == 4 || argc == 5)) {
            imprimir_uso(argv[0]);
            return 1;
        }

        if (argc >= 4) {
            x_inicial = converter_real(argv[1], "xI");
            comprimento = converter_real(argv[2], "L");
            numero_nos = converter_tamanho(argv[3], "N");
        }

        if (argc == 5) {
            semente = converter_semente(argv[4], "semente");
        }

        std::cout << std::fixed << std::setprecision(12);
        std::cout << "Exercício Computacional 3.1\n";
        std::cout << "Gerador de malhas aleatórias\n";
        std::cout << "============================\n\n";

        const MalhaAleatoria1D malha = gerar_malha_aleatoria(
            x_inicial,
            comprimento,
            numero_nos,
            semente
        );

        imprimir_malha(malha);
        const unsigned status = verificar_malha(malha);
        imprimir_observacao_final();

        return static_cast<int>(status);
    } catch (const std::exception& erro) {
        std::cerr << "Erro: " << erro.what() << '\n';
        return 1;
    }
}
