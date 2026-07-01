// ============================================================================
// Arquivo: exercicio_22.cpp
// Projeto: FVMaker
// Versao: consulte <FVMaker/Core/Version.h>
// Descricao: Refatora uma rotina de media movel escrita de forma opaca.
// Autor: Joao Flavio Vieira de Vasconcellos
//
// SPDX-FileCopyrightText: 2026 Joao Flavio Vieira de Vasconcellos
// SPDX-License-Identifier: BSD-3-Clause
// ============================================================================

#include <cmath>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

using Real = double;

namespace {

// Versao propositalmente ruim, mantida apenas como referencia didatica.
// Ela reproduz o estilo do enunciado: nome opaco, argumento copiado,
// dependencia global e nenhuma protecao para casos limite.
int largura_global_legada = 3;

[[nodiscard]] std::vector<Real> mm(std::vector<Real> d)
{
    std::vector<Real> o;

    for (int i = 0; i <= static_cast<int>(d.size()) - largura_global_legada; ++i) {
        Real s = Real{};
        for (int j = 0; j < largura_global_legada; ++j) {
            s += d[static_cast<std::size_t>(i + j)];
        }
        o.push_back(s / static_cast<Real>(largura_global_legada));
    }

    return o;
}

// Calcula a media movel aritmetica de uma sequencia.
//
// Pre-condicoes verificadas em tempo de execucao:
//   1. o vetor de dados deve conter ao menos uma amostra;
//   2. a largura da janela deve ser maior que zero;
//   3. a largura da janela nao pode exceder o numero de amostras;
//   4. todas as amostras devem ser finitas.
//
// A rotina lanca std::invalid_argument para entradas invalidas porque esses
// erros pertencem a quem chama a funcao. Diferentemente de assert, essa
// validacao continua ativa em compilacoes otimizadas, preservando a
// reprodutibilidade e evitando resultados silenciosamente sem sentido.
[[nodiscard]] std::vector<Real> media_movel(
    const std::vector<Real>& dados,
    std::size_t largura_janela
)
{
    if (dados.empty()) {
        throw std::invalid_argument("A media movel exige ao menos uma amostra.");
    }

    if (largura_janela == 0) {
        throw std::invalid_argument("A largura da janela deve ser maior que zero.");
    }

    if (largura_janela > dados.size()) {
        throw std::invalid_argument(
            "A largura da janela nao pode ser maior que o numero de amostras."
        );
    }

    for (Real valor : dados) {
        if (!std::isfinite(valor)) {
            throw std::invalid_argument("Todas as amostras devem ser finitas.");
        }
    }

    std::vector<Real> medias;
    medias.reserve(dados.size() - largura_janela + 1);

    Real soma_janela = Real{};
    for (std::size_t i = 0; i < largura_janela; ++i) {
        soma_janela += dados[i];
    }

    medias.push_back(soma_janela / static_cast<Real>(largura_janela));

    for (std::size_t inicio = 1; inicio + largura_janela <= dados.size(); ++inicio) {
        soma_janela -= dados[inicio - 1];
        soma_janela += dados[inicio + largura_janela - 1];
        medias.push_back(soma_janela / static_cast<Real>(largura_janela));
    }

    return medias;
}

struct Placar {
    unsigned aprovados{};
    unsigned total{};
};

void registrar(Placar& placar, const std::string& descricao, bool passou)
{
    ++placar.total;
    if (passou) {
        ++placar.aprovados;
    }

    std::cout << (passou ? "[PASSOU] " : "[FALHOU] ") << descricao << '\n';
}

[[nodiscard]] bool aproximadamente_igual(Real a, Real b, Real tolerancia = Real{1.0e-12})
{
    return std::abs(a - b) <= tolerancia;
}

[[nodiscard]] bool vetores_proximos(
    const std::vector<Real>& obtido,
    const std::vector<Real>& esperado,
    Real tolerancia = Real{1.0e-12}
)
{
    if (obtido.size() != esperado.size()) {
        return false;
    }

    for (std::size_t i = 0; i < obtido.size(); ++i) {
        if (!aproximadamente_igual(obtido[i], esperado[i], tolerancia)) {
            return false;
        }
    }

    return true;
}

void testar_vetor(
    Placar& placar,
    const std::string& descricao,
    const std::vector<Real>& obtido,
    const std::vector<Real>& esperado
)
{
    const bool passou = vetores_proximos(obtido, esperado);
    registrar(placar, descricao, passou);

    if (!passou) {
        std::cout << std::setprecision(16);
        std::cout << "  obtido  :";
        for (Real valor : obtido) {
            std::cout << ' ' << valor;
        }
        std::cout << "\n  esperado:";
        for (Real valor : esperado) {
            std::cout << ' ' << valor;
        }
        std::cout << '\n';
    }
}

template <typename Acao>
void testar_excecao(Placar& placar, const std::string& descricao, Acao acao)
{
    bool lancou = false;

    try {
        acao();
    } catch (const std::invalid_argument&) {
        lancou = true;
    }

    registrar(placar, descricao, lancou);
}

void imprimir_vetor(const std::string& nome, const std::vector<Real>& valores)
{
    std::cout << nome << ':';
    std::cout << std::fixed << std::setprecision(6);
    for (Real valor : valores) {
        std::cout << ' ' << valor;
    }
    std::cout << '\n';
}

} // namespace

int main()
{
    std::cout << "Exercicio Computacional 2.2\n";
    std::cout << "Refatoracao de uma funcao ilegivel\n";
    std::cout << "==================================\n\n";

    const std::vector<Real> dados{Real{1.0}, Real{2.0}, Real{4.0}, Real{7.0}, Real{11.0}};
    const std::vector<Real> esperado{
        Real{7.0} / Real{3.0},
        Real{13.0} / Real{3.0},
        Real{22.0} / Real{3.0}
    };

    largura_global_legada = 3;
    const std::vector<Real> legado = mm(dados);
    const std::vector<Real> refatorado = media_movel(dados, 3);

    imprimir_vetor("dados", dados);
    imprimir_vetor("media movel, janela 3", refatorado);
    std::cout << '\n';

    Placar placar;

    testar_vetor(
        placar,
        "a rotina refatorada reproduz a media movel esperada",
        refatorado,
        esperado
    );

    testar_vetor(
        placar,
        "a rotina refatorada coincide com a versao original nos casos validos",
        refatorado,
        legado
    );

    testar_vetor(
        placar,
        "janela de largura 1 devolve os proprios dados",
        media_movel(dados, 1),
        dados
    );

    testar_excecao(
        placar,
        "vetor vazio e rejeitado",
        [] { (void)media_movel({}, 2); }
    );

    testar_excecao(
        placar,
        "janela nula e rejeitada",
        [&dados] { (void)media_movel(dados, 0); }
    );

    testar_excecao(
        placar,
        "janela maior que o vetor e rejeitada",
        [&dados] { (void)media_movel(dados, dados.size() + 1); }
    );

    testar_excecao(
        placar,
        "amostra NaN e rejeitada antes do calculo",
        [] { (void)media_movel({Real{1.0}, std::nan("")}, 2); }
    );

    std::cout << "\nResumo: " << placar.aprovados << " de " << placar.total
              << " verificacoes aprovadas.\n";

    std::cout << "\nObservacao academica\n";
    std::cout << "--------------------\n";
    std::cout << "A mudanca principal nao e cosmetica: nomes, argumentos explicitos,\n";
    std::cout << "validacao e documentacao tornam a rotina reconhecivel e segura para\n";
    std::cout << "ser reutilizada em outro experimento numerico.\n";

    return (placar.aprovados == placar.total) ? 0 : 1;
}