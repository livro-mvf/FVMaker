// ============================================================================
// Arquivo: exercicio_24.cpp
// Projeto: FVMaker
// Versao: consulte <FVMaker/Core/Version.h>
// Descricao: Testes unitarios, de integracao e de regressao para Malha1D.
// Autor: Joao Flavio Vieira de Vasconcellos
//
// SPDX-FileCopyrightText: 2026 Joao Flavio Vieira de Vasconcellos
// SPDX-License-Identifier: BSD-3-Clause
// ============================================================================

#include <cmath>
#include <cstddef>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using Real = double;

namespace {

// Representa Malha1D, uma estrutura didatica usada para organizar a malha.
class Malha1D
{
public:
    // Constroi a malha e inicializa seus dados internos.
    Malha1D(Real xmin, Real xmax, std::size_t numero_volumes)
        : xmin_{xmin},
          xmax_{xmax},
          numero_volumes_{numero_volumes},
          dx_{calcular_dx(xmin, xmax, numero_volumes)}
    {
    }

    // Retorna a coordenada inicial da malha.
    [[nodiscard]] Real xmin() const noexcept { return xmin_; }
    // Retorna a coordenada final da malha.
    [[nodiscard]] Real xmax() const noexcept { return xmax_; }
    // Retorna o numero de volumes ou nos internos armazenados.
    [[nodiscard]] std::size_t numero_volumes() const noexcept { return numero_volumes_; }
    // Retorna o espacamento uniforme da malha.
    [[nodiscard]] Real dx() const noexcept { return dx_; }

    // Retorna a coordenada do centro associado ao indice.
    [[nodiscard]] Real centro(std::size_t indice) const
    {
        if (indice >= numero_volumes_) {
            throw std::out_of_range("Indice do volume fora do intervalo [0, N).");
        }
        return xmin_ + (static_cast<Real>(indice) + Real{0.5}) * dx_;
    }

private:
    Real xmin_{};
    Real xmax_{};
    std::size_t numero_volumes_{};
    Real dx_{};

    // Calcula o espacamento uniforme depois de validar a malha.
    static Real calcular_dx(Real xmin, Real xmax, std::size_t numero_volumes)
    {
        validar(xmin, xmax, numero_volumes);
        return (xmax - xmin) / static_cast<Real>(numero_volumes);
    }

    // Valida os dados antes de seguir com o calculo.
    static void validar(Real xmin, Real xmax, std::size_t numero_volumes)
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
};

// Representa Placar, um conjunto de dados usado neste exercicio.
struct Placar {
    unsigned aprovados{};
    unsigned total{};
};

// Compara valores reais dentro de uma tolerancia.
[[nodiscard]] bool aproximadamente_igual(Real a, Real b, Real tolerancia = Real{1.0e-12})
{
    return std::abs(a - b) <= tolerancia;
}

// Registra no placar o resultado de uma verificacao.
void registrar(
    Placar& placar,
    const std::string& nome,
    bool passou,
    const std::string& erro_que_apanha
)
{
    ++placar.total;
    if (passou) {
        ++placar.aprovados;
    }

    std::cout << (passou ? "[PASSOU] " : "[FALHOU] ") << nome << '\n';
    std::cout << "         apanharia: " << erro_que_apanha << '\n';
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

// Avalia a solucao analitica usada no teste de integracao.
[[nodiscard]] Real solucao_analitica(Real x)
{
    return Real{1.0} + x + x * x;
}

// Avalia uma funcao conhecida nos pontos da malha.
[[nodiscard]] std::vector<Real> avaliar_solucao(const Malha1D& malha)
{
    std::vector<Real> valores(malha.numero_volumes());

    for (std::size_t i = 0; i < malha.numero_volumes(); ++i) {
        valores[i] = solucao_analitica(malha.centro(i));
    }

    return valores;
}

// Compara vetores numericos dentro de uma tolerancia.
[[nodiscard]] bool comparar_vetores(
    const std::vector<Real>& obtido,
    const std::vector<Real>& esperado,
    Real tolerancia
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

// Gera a saida textual fixa usada no teste de regressao.
[[nodiscard]] std::string saida_integracao(const Malha1D& malha)
{
    std::ostringstream saida;
    saida << std::fixed << std::setprecision(12);
    saida << "# referencia exercicio_24\n";
    saida << "N " << malha.numero_volumes() << '\n';
    saida << "xmin " << malha.xmin() << '\n';
    saida << "xmax " << malha.xmax() << '\n';
    saida << "dx " << malha.dx() << '\n';
    saida << "i centro u\n";

    for (std::size_t i = 0; i < malha.numero_volumes(); ++i) {
        const Real centro = malha.centro(i);
        saida << i << ' ' << centro << ' ' << solucao_analitica(centro) << '\n';
    }

    return saida.str();
}

// Fornece a referencia usada no teste de regressao.
[[nodiscard]] const std::string& referencia_regressao()
{
    static const std::string referencia =
        "# referencia exercicio_24\n"
        "N 4\n"
        "xmin 0.000000000000\n"
        "xmax 1.000000000000\n"
        "dx 0.250000000000\n"
        "i centro u\n"
        "0 0.125000000000 1.140625000000\n"
        "1 0.375000000000 1.515625000000\n"
        "2 0.625000000000 2.015625000000\n"
        "3 0.875000000000 2.640625000000\n";

    return referencia;
}

// Grava a saida de referencia ou o resultado do experimento.
void gravar_referencia(const std::string& nome_arquivo)
{
    std::ofstream arquivo{nome_arquivo};
    if (!arquivo) {
        throw std::runtime_error("Nao foi possivel criar " + nome_arquivo);
    }
    arquivo << referencia_regressao();
}

// Le dados externos usados pelo exercicio.
[[nodiscard]] std::string ler_arquivo(const std::string& nome_arquivo)
{
    std::ifstream arquivo{nome_arquivo};
    if (!arquivo) {
        throw std::runtime_error("Nao foi possivel ler " + nome_arquivo);
    }

    std::ostringstream conteudo;
    conteudo << arquivo.rdbuf();
    return conteudo.str();
}

// Executa o bloco principal de testes ou experimento.
void executar_testes_unitarios(Placar& placar)
{
    std::cout << "Testes unitarios\n";
    std::cout << "----------------\n";

    const Malha1D malha{Real{0.0}, Real{1.0}, 4};

    registrar(placar, "dx esperado", aproximadamente_igual(malha.dx(), Real{0.25}),
              "formula errada para o espacamento");
    registrar(placar, "centro do primeiro volume", aproximadamente_igual(malha.centro(0), Real{0.125}),
              "deslocamento incorreto de meio volume");
    registrar(placar, "centro do ultimo volume", aproximadamente_igual(malha.centro(3), Real{0.875}),
              "erro de indice no fim da malha");
    registrar(placar, "N = 0 rejeitado", lanca<std::invalid_argument>([] { Malha1D{Real{0.0}, Real{1.0}, 0}; }),
              "divisao por zero no calculo de dx");
    registrar(placar, "xmax <= xmin rejeitado", lanca<std::invalid_argument>([] { Malha1D{Real{1.0}, Real{1.0}, 4}; }),
              "malha com comprimento nulo ou negativo");
    registrar(placar, "indice fora do intervalo rejeitado", lanca<std::out_of_range>([&malha] { (void)malha.centro(4); }),
              "acesso fora do intervalo valido [0, N)");

    std::cout << '\n';
}

// Executa o bloco principal de testes ou experimento.
void executar_teste_integracao(Placar& placar)
{
    std::cout << "Teste de integracao\n";
    std::cout << "-------------------\n";

    const Malha1D malha{Real{0.0}, Real{1.0}, 4};
    const std::vector<Real> obtido = avaliar_solucao(malha);
    const std::vector<Real> esperado{
        Real{1.140625}, Real{1.515625}, Real{2.015625}, Real{2.640625}
    };

    registrar(placar, "u(x) = 1 + x + x^2 avaliada nos centros",
              comparar_vetores(obtido, esperado, Real{1.0e-12}),
              "incompatibilidade entre a malha e uma rotina numerica que usa seus centros");

    std::cout << "         garante: a interface da malha compoe corretamente com uma\n";
    std::cout << "                  avaliacao externa de solucao analitica.\n\n";
}

// Executa o bloco principal de testes ou experimento.
void executar_teste_regressao(Placar& placar)
{
    std::cout << "Teste de regressao\n";
    std::cout << "------------------\n";

    const std::string nome_arquivo = "exercicio_24_referencia.txt";
    gravar_referencia(nome_arquivo);

    const Malha1D malha{Real{0.0}, Real{1.0}, 4};
    const std::string referencia = ler_arquivo(nome_arquivo);
    const std::string atual = saida_integracao(malha);

    registrar(placar, "saida atual coincide com a referencia salva",
              atual == referencia,
              "mudanca acidental de valores, precisao textual ou formato de saida");

    std::cout << "         detecta: uma alteracao futura que continue compilando, mas\n";
    std::cout << "                  mude a saida usada como base de comparacao.\n\n";
}

} // namespace

// Executa o roteiro completo do exercicio.
int main()
{
    std::cout << "Exercicio Computacional 2.4\n";
    std::cout << "Testes unitarios, de integracao e de regressao\n";
    std::cout << "================================================\n\n";

    Placar placar;
    executar_testes_unitarios(placar);
    executar_teste_integracao(placar);
    executar_teste_regressao(placar);

    std::cout << "Resumo: " << placar.aprovados << " de " << placar.total
              << " verificacoes aprovadas.\n";

    return (placar.aprovados == placar.total) ? 0 : 1;
}