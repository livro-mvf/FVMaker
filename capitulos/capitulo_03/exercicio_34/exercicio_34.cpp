// ============================================================================
// Arquivo: exercicio_34.cpp
// Projeto: FVMaker
// Descricao: Mede a perda de ordem da derivada segunda em malha nao uniforme.
// Autor: Joao Flavio Vieira de Vasconcellos
// SPDX-License-Identifier: BSD-3-Clause
// ============================================================================

#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

using Real = double;

namespace {

// Identifica o modo usado para escolher os passos vizinhos do ponto central.
enum class ModoRefinamento
{
    PassosSuaves,
    SaltoFixo
};

// Guarda uma linha da tabela de erro para uma malha local nao uniforme.
struct LinhaNaoUniforme
{
    Real dx_esquerda{};
    Real dx_direita{};
    Real razao_passos{};
    Real aproximacao{};
    Real erro{};
    Real razao_erros{};
    Real ordem_observada{};
    Real fator_salto{};
};

// Avalia a funcao usada no experimento de ordem.
[[nodiscard]] Real funcao(Real x)
{
    return std::exp(x);
}

// Avalia a derivada segunda exata da funcao escolhida.
[[nodiscard]] Real derivada_segunda_exata(Real x)
{
    return std::exp(x);
}

// Aproxima f'' em uma malha local nao uniforme com passos esquerdo e direito.
[[nodiscard]] Real segunda_nao_uniforme(Real x, Real dx_esquerda, Real dx_direita)
{
    const Real f_esquerda = funcao(x - dx_esquerda);
    const Real f_centro = funcao(x);
    const Real f_direita = funcao(x + dx_direita);

    return Real{2.0} / (dx_esquerda + dx_direita)
        * ((f_direita - f_centro) / dx_direita
           - (f_centro - f_esquerda) / dx_esquerda);
}

// Calcula a ordem observada quando o passo e reduzido pela metade.
[[nodiscard]] Real ordem_por_razao(Real razao)
{
    if (!(razao > Real{0.0})) {
        return std::numeric_limits<Real>::quiet_NaN();
    }
    return std::log(razao) / std::log(Real{2.0});
}

// Define o passo direito a partir do esquerdo e do modo de refinamento.
[[nodiscard]] Real passo_direito(ModoRefinamento modo, Real dx_esquerda)
{
    if (modo == ModoRefinamento::PassosSuaves) {
        return dx_esquerda * (Real{1.0} + dx_esquerda);
    }
    return Real{2.0} * dx_esquerda;
}

// Constroi a tabela de erro para uma sequencia de passos esquerdos.
[[nodiscard]] std::vector<LinhaNaoUniforme> medir_ordem_nao_uniforme(
    ModoRefinamento modo,
    Real x,
    const std::vector<Real>& passos_esquerda
)
{
    std::vector<LinhaNaoUniforme> tabela;
    tabela.reserve(passos_esquerda.size());

    Real erro_anterior = Real{};
    for (Real dx_esquerda : passos_esquerda) {
        const Real dx_direita = passo_direito(modo, dx_esquerda);
        const Real aproximacao = segunda_nao_uniforme(x, dx_esquerda, dx_direita);
        const Real erro = std::abs(aproximacao - derivada_segunda_exata(x));
        const Real razao_erros = tabela.empty() ? Real{} : erro_anterior / erro;
        const Real ordem = tabela.empty() ? Real{} : ordem_por_razao(razao_erros);
        const Real fator_salto = std::abs(dx_esquerda - dx_direita);

        tabela.push_back(LinhaNaoUniforme{
            dx_esquerda,
            dx_direita,
            dx_direita / dx_esquerda,
            aproximacao,
            erro,
            razao_erros,
            ordem,
            fator_salto
        });

        erro_anterior = erro;
    }

    return tabela;
}

// Imprime uma tabela comparando passo, erro, razao de erros e ordem observada.
void imprimir_tabela(const std::string& titulo, const std::vector<LinhaNaoUniforme>& tabela)
{
    std::cout << titulo << '\n';
    std::cout << std::setw(16) << "dx_i"
              << std::setw(16) << "dx_i+1"
              << std::setw(16) << "razao dx"
              << std::setw(18) << "erro"
              << std::setw(18) << "razao err"
              << std::setw(18) << "ordem"
              << std::setw(20) << "|dx_i-dx_i+1|" << '\n';

    for (const LinhaNaoUniforme& linha : tabela) {
        std::cout << std::setw(16) << linha.dx_esquerda
                  << std::setw(16) << linha.dx_direita
                  << std::setw(16) << linha.razao_passos
                  << std::setw(18) << linha.erro;
        if (linha.razao_erros == Real{}) {
            std::cout << std::setw(18) << "--" << std::setw(18) << "--";
        } else {
            std::cout << std::setw(18) << linha.razao_erros
                      << std::setw(18) << linha.ordem_observada;
        }
        std::cout << std::setw(20) << linha.fator_salto << '\n';
    }
    std::cout << '\n';
}

// Verifica se um valor esta dentro de uma faixa fechada.
[[nodiscard]] bool dentro(Real valor, Real minimo, Real maximo)
{
    return valor >= minimo && valor <= maximo;
}

} // namespace

// Executa o roteiro completo do exercicio.
int main()
{
    std::cout << "Exercicio Computacional 3.4\n";
    std::cout << "A ordem que despenca em malha nao uniforme\n";
    std::cout << "=========================================\n\n";

    const Real x = Real{0.4};
    const std::vector<Real> passos{0.2, 0.1, 0.05, 0.025, 0.0125, 0.00625};

    std::cout << std::scientific << std::setprecision(8);

    const auto suave = medir_ordem_nao_uniforme(ModoRefinamento::PassosSuaves, x, passos);
    const auto salto = medir_ordem_nao_uniforme(ModoRefinamento::SaltoFixo, x, passos);

    imprimir_tabela("Passos suaves: dx_{i+1} = dx_i * (1 + dx_i)", suave);
    imprimir_tabela("Salto fixo: dx_{i+1} = 2 * dx_i", salto);

    const Real ordem_suave = suave.back().ordem_observada;
    const Real ordem_salto = salto.back().ordem_observada;
    const bool suave_ok = dentro(ordem_suave, Real{1.8}, Real{2.2});
    const bool salto_ok = dentro(ordem_salto, Real{0.8}, Real{1.2});

    std::cout << (suave_ok ? "[PASSOU] " : "[FALHOU] ")
              << "passos suaves convergem para segunda ordem\n";
    std::cout << (salto_ok ? "[PASSOU] " : "[FALHOU] ")
              << "salto fixo mostra queda para primeira ordem\n\n";

    std::cout << "Interpretacao\n";
    std::cout << "-------------\n";
    std::cout << "No caso suave, |dx_i - dx_{i+1}| e proporcional a dx_i^2,\n";
    std::cout << "entao o termo que envolve a terceira derivada tambem cai como segunda ordem.\n";
    std::cout << "No salto fixo, |dx_i - dx_{i+1}| continua proporcional a dx_i,\n";
    std::cout << "e esse fator decide a queda para primeira ordem.\n";

    return (suave_ok && salto_ok) ? 0 : 1;
}