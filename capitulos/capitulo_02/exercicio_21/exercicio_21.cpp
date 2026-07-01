// ============================================================================
// Arquivo: exercicio_21.cpp
// Projeto: FVMaker
// Versao: consulte <FVMaker/Core/Version.h>
// Descricao: Usa validacoes e asserts como sentinelas na montagem de sistema.
// Autor: Joao Flavio Vieira de Vasconcellos
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


//==============================================================================
// Header c++
//==============================================================================
#include <cassert>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>

using Real = double;

namespace {

// Representa SistemaLinear1D, uma estrutura didatica usada para organizar a malha.
struct SistemaLinear1D
{
    std::vector<Real> oeste;
    std::vector<Real> diagonal;
    std::vector<Real> leste;
    std::vector<Real> termo_fonte;
};

// Representa Placar, um conjunto de dados usado neste exercicio.
struct Placar {
    unsigned aprovados{};
    unsigned total{};
};

// Registra no placar o resultado de uma verificacao.
void registrar(Placar& placar, const std::string& descricao, bool passou)
{
    ++placar.total;
    if (passou) {
        ++placar.aprovados;
    }

    std::cout << (passou ? "[PASSOU] " : "[FALHOU] ") << descricao << '\n';
}

// Imprime a explicacao didatica de uma verificacao.
void documentar_verificacao(
    const std::string& tipo,
    const std::string& protege,
    const std::string& falha_quando
)
{
    std::cout << "- " << tipo << "\n";
    std::cout << "  protege: " << protege << '\n';
    std::cout << "  falha quando: " << falha_quando << '\n';
}

// Valida os dados antes de seguir com o calculo.
void validar_entradas(
    const std::vector<Real>& dx,
    const std::vector<Real>& difusividade,
    const std::vector<Real>& fonte
)
{
    if (dx.empty()) {
        throw std::invalid_argument("A malha deve conter ao menos um volume.");
    }

    if (dx.size() != difusividade.size() || dx.size() != fonte.size()) {
        throw std::invalid_argument(
            "dx, difusividade e fonte devem ter o mesmo numero de volumes."
        );
    }

    for (std::size_t i = 0; i < dx.size(); ++i) {
        if (!std::isfinite(dx[i]) || dx[i] <= Real{0.0}) {
            throw std::invalid_argument("Todo dx deve ser finito e positivo.");
        }
        if (!std::isfinite(difusividade[i]) || difusividade[i] < Real{0.0}) {
            throw std::invalid_argument(
                "A difusividade deve ser finita e nao negativa."
            );
        }
        if (!std::isfinite(fonte[i])) {
            throw std::invalid_argument("O termo fonte deve ser finito.");
        }
    }
}

// Verifica uma propriedade numerica exigida pelo exercicio.
void verificar_invariante_diagonal(Real diagonal, std::size_t indice)
{
    if (!std::isfinite(diagonal)) {
        throw std::logic_error("A diagonal montada deve permanecer finita.");
    }

    if (std::abs(diagonal) <= Real{1.0e-14}) {
        throw std::logic_error(
            "A diagonal do volume " + std::to_string(indice) + " ficou nula."
        );
    }

    assert(std::isfinite(diagonal));
    assert(std::abs(diagonal) > Real{1.0e-14});
}

// Monta um sistema tridiagonal simples para ilustrar verificacoes preventivas.
//
// A rotina nao pretende ser um solver completo de volumes finitos; ela e uma
// bancada didatica. Os coeficientes oeste/leste sao proporcionais a
// difusividade/dx, e a diagonal deve ser a soma dos acoplamentos vizinhos.
// Entradas invalidas sao rejeitadas antes da montagem; invariantes internos sao
// checados apos cada linha do sistema ser formada.
[[nodiscard]] SistemaLinear1D montar_sistema_linear(
    const std::vector<Real>& dx,
    const std::vector<Real>& difusividade,
    const std::vector<Real>& fonte
)
{
    validar_entradas(dx, difusividade, fonte);

    const std::size_t n = dx.size();
    SistemaLinear1D sistema{
        std::vector<Real>(n, Real{}),
        std::vector<Real>(n, Real{}),
        std::vector<Real>(n, Real{}),
        std::vector<Real>(n, Real{})
    };

    for (std::size_t i = 0; i < n; ++i) {
        const Real oeste = (i > 0) ? difusividade[i - 1] / dx[i] : Real{};
        const Real leste = (i + 1 < n) ? difusividade[i] / dx[i] : Real{};
        const Real diagonal = oeste + leste;

        verificar_invariante_diagonal(diagonal, i);

        sistema.oeste[i] = oeste;
        sistema.leste[i] = leste;
        sistema.diagonal[i] = diagonal;
        sistema.termo_fonte[i] = fonte[i] * dx[i];
    }

    return sistema;
}

// Testa se uma acao lanca a excecao esperada.
template <typename Excecao, typename Acao>
[[nodiscard]] bool lanca(Acao acao)
{
    try {
        acao();
    } catch (const Excecao&) {
        return true;
    }
    return false;
}

// Compara valores reais dentro de uma tolerancia.
[[nodiscard]] bool aproximadamente_igual(Real a, Real b, Real tolerancia = Real{1.0e-12})
{
    return std::abs(a - b) <= tolerancia;
}

// Imprime os dados do exercicio em formato legivel.
void imprimir_sistema(const SistemaLinear1D& sistema)
{
    std::cout << std::fixed << std::setprecision(6);
    std::cout << std::setw(6) << "i"
              << std::setw(14) << "aW"
              << std::setw(14) << "aP"
              << std::setw(14) << "aE"
              << std::setw(14) << "b" << '\n';

    for (std::size_t i = 0; i < sistema.diagonal.size(); ++i) {
        std::cout << std::setw(6) << i
                  << std::setw(14) << sistema.oeste[i]
                  << std::setw(14) << sistema.diagonal[i]
                  << std::setw(14) << sistema.leste[i]
                  << std::setw(14) << sistema.termo_fonte[i] << '\n';
    }
}

// Executa o bloco principal de testes ou experimento.
void executar_testes(Placar& placar)
{
    const std::vector<Real> dx{Real{0.5}, Real{0.5}, Real{0.5}};
    const std::vector<Real> difusividade{Real{1.0}, Real{1.0}, Real{1.0}};
    const std::vector<Real> fonte{Real{2.0}, Real{4.0}, Real{6.0}};

    const SistemaLinear1D sistema = montar_sistema_linear(dx, difusividade, fonte);
    imprimir_sistema(sistema);
    std::cout << '\n';

    registrar(placar, "caso valido monta diagonal esperada",
              aproximadamente_igual(sistema.diagonal[1], Real{4.0}));

    registrar(placar, "tamanhos incompativeis sao recusados como entrada invalida",
              lanca<std::invalid_argument>([] {
                  (void)montar_sistema_linear({Real{0.5}, Real{0.5}}, {Real{1.0}}, {Real{1.0}, Real{1.0}});
              }));

    registrar(placar, "NaN nos dados e recusado antes da montagem",
              lanca<std::invalid_argument>([] {
                  (void)montar_sistema_linear({Real{0.5}}, {Real{1.0}}, {std::nan("")});
              }));

    registrar(placar, "diagonal nula e acusada como invariante interno quebrado",
              lanca<std::logic_error>([] {
                  (void)montar_sistema_linear(
                      {Real{0.5}, Real{0.5}, Real{0.5}},
                      {Real{0.0}, Real{0.0}, Real{0.0}},
                      {Real{1.0}, Real{1.0}, Real{1.0}}
                  );
              }));
}

} // namespace

// Executa o roteiro completo do exercicio.
int main()
{
    std::cout << "Exercicio Computacional 2.1\n";
    std::cout << "Assercoes como sentinelas\n";
    std::cout << "=========================\n\n";

    std::cout << "Verificacoes documentadas\n";
    std::cout << "-------------------------\n";
    documentar_verificacao(
        "validacao de entrada",
        "os vetores representam a mesma malha e podem ser combinados indice a indice",
        "dx, difusividade e fonte chegam com tamanhos diferentes"
    );
    documentar_verificacao(
        "validacao de entrada",
        "a montagem nao recebe NaN, infinito ou volume de tamanho nao positivo",
        "um dado externo viola as hipoteses fisicas ou numericas da rotina"
    );
    documentar_verificacao(
        "invariante interno",
        "a diagonal principal do sistema permanece finita e nao nula",
        "a montagem gera uma linha sem acoplamento suficiente para resolver o sistema"
    );
    std::cout << '\n';

    Placar placar;
    executar_testes(placar);

    std::cout << "\nResumo: " << placar.aprovados << " de " << placar.total
              << " verificacoes aprovadas.\n";

    std::cout << "\nPor que assert nao substitui validacao de usuario\n";
    std::cout << "------------------------------------------------\n";
    std::cout << "assert e uma sentinela de desenvolvimento: ajuda a revelar invariantes\n";
    std::cout << "quebrados durante a depuracao, mas pode desaparecer com NDEBUG. Dados\n";
    std::cout << "vindos de quem usa o programa precisam de validacao explicita por excecao\n";
    std::cout << "ou outro mecanismo que continue ativo na versao otimizada.\n";

    return (placar.aprovados == placar.total) ? 0 : 1;
}