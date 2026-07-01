// ============================================================================
// Arquivo: exercicio_32.cpp
// Projeto: FVMaker
// Descricao: Monta o sistema linear da equacao de Poisson 1D em malha uniforme.
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
#include <cmath>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <vector>

using Real = double;

namespace {

// Enumera os tipos de condicao de contorno usados no capitulo.
enum class TipoContorno
{
    Dirichlet,
    Neumann,
    Robin
};

// Guarda a forma alpha*phi + beta*dphi/dx = gamma de uma fronteira.
struct CondicaoContorno
{
    TipoContorno tipo{};
    Real alpha{};
    Real beta{};
    Real gamma{};
};

// Reune os dados fisicos e geometricos necessarios para montar o sistema.
struct ProblemaPoisson
{
    std::size_t numero_nos{};
    Real x_inicial{};
    Real x_final{};
    CondicaoContorno esquerda{};
    CondicaoContorno direita{};
};

// Armazena a matriz dos coeficientes, o vetor independente e os nos usados.
struct SistemaLinear
{
    std::vector<std::vector<Real>> matriz;
    std::vector<Real> vetor;
    std::vector<Real> nos;
};

// Cria uma condicao de Dirichlet phi = valor.
[[nodiscard]] CondicaoContorno dirichlet(Real valor)
{
    return CondicaoContorno{TipoContorno::Dirichlet, Real{1.0}, Real{0.0}, valor};
}

// Cria uma condicao de Neumann dphi/dx = valor.
[[nodiscard]] CondicaoContorno neumann(Real valor)
{
    return CondicaoContorno{TipoContorno::Neumann, Real{0.0}, Real{1.0}, valor};
}

// Cria uma condicao de Robin alpha*phi + beta*dphi/dx = gamma.
[[nodiscard]] CondicaoContorno robin(Real alpha, Real beta, Real gamma)
{
    return CondicaoContorno{TipoContorno::Robin, alpha, beta, gamma};
}

// Retorna um nome curto para imprimir o tipo de condicao de contorno.
[[nodiscard]] std::string nome(TipoContorno tipo)
{
    switch (tipo) {
    case TipoContorno::Dirichlet: return "Dirichlet";
    case TipoContorno::Neumann: return "Neumann";
    case TipoContorno::Robin: return "Robin";
    }
    return "desconhecida";
}

// Verifica se a malha e as condicoes de contorno fazem sentido numerico.
void validar_problema(const ProblemaPoisson& problema)
{
    if (problema.numero_nos < 3) {
        throw std::invalid_argument("A montagem exige pelo menos tres nos.");
    }
    if (!std::isfinite(problema.x_inicial) || !std::isfinite(problema.x_final)) {
        throw std::invalid_argument("Os extremos do dominio devem ser finitos.");
    }
    if (!(problema.x_final > problema.x_inicial)) {
        throw std::invalid_argument("O dominio deve satisfazer x_final > x_inicial.");
    }
}

// Calcula as coordenadas de uma malha uniforme nodal.
[[nodiscard]] std::vector<Real> gerar_nos_uniformes(
    Real x_inicial,
    Real x_final,
    std::size_t numero_nos
)
{
    const Real dx = (x_final - x_inicial) / static_cast<Real>(numero_nos - 1);
    std::vector<Real> nos(numero_nos);

    for (std::size_t i = 0; i < numero_nos; ++i) {
        nos[i] = x_inicial + static_cast<Real>(i) * dx;
    }

    nos.back() = x_final;
    return nos;
}

// Preenche a linha esquerda usando diferenca progressiva para a derivada.
void aplicar_contorno_esquerdo(
    SistemaLinear& sistema,
    const CondicaoContorno& contorno,
    Real dx
)
{
    sistema.matriz[0][0] = contorno.alpha - contorno.beta / dx;
    sistema.matriz[0][1] = contorno.beta / dx;
    sistema.vetor[0] = contorno.gamma;
}

// Preenche a linha direita usando diferenca regressiva para a derivada.
void aplicar_contorno_direito(
    SistemaLinear& sistema,
    const CondicaoContorno& contorno,
    Real dx
)
{
    const std::size_t n = sistema.matriz.size();
    sistema.matriz[n - 1][n - 2] = -contorno.beta / dx;
    sistema.matriz[n - 1][n - 1] = contorno.alpha + contorno.beta / dx;
    sistema.vetor[n - 1] = contorno.gamma;
}

// Monta o sistema discreto para phi'' = termo_fonte(x).
template <typename Fonte>
[[nodiscard]] SistemaLinear montar_sistema_poisson(
    const ProblemaPoisson& problema,
    Fonte termo_fonte
)
{
    validar_problema(problema);

    const std::size_t n = problema.numero_nos;
    SistemaLinear sistema{
        std::vector<std::vector<Real>>(n, std::vector<Real>(n, Real{})),
        std::vector<Real>(n, Real{}),
        gerar_nos_uniformes(problema.x_inicial, problema.x_final, n)
    };

    const Real dx = sistema.nos[1] - sistema.nos[0];

    aplicar_contorno_esquerdo(sistema, problema.esquerda, dx);

    for (std::size_t i = 1; i + 1 < n; ++i) {
        sistema.matriz[i][i - 1] = Real{1.0};
        sistema.matriz[i][i] = Real{-2.0};
        sistema.matriz[i][i + 1] = Real{1.0};
        sistema.vetor[i] = termo_fonte(sistema.nos[i]) * dx * dx;
    }

    aplicar_contorno_direito(sistema, problema.direita, dx);
    return sistema;
}

// Imprime a malha para conectar os indices da matriz aos pontos do dominio.
void imprimir_nos(const std::vector<Real>& nos)
{
    std::cout << "Nos da malha\n";
    for (std::size_t i = 0; i < nos.size(); ++i) {
        std::cout << "x[" << i << "] = " << nos[i] << '\n';
    }
    std::cout << '\n';
}

// Imprime a matriz dos coeficientes em formato tabular simples.
void imprimir_matriz(const SistemaLinear& sistema)
{
    std::cout << "Matriz A\n";
    for (const auto& linha : sistema.matriz) {
        for (Real valor : linha) {
            std::cout << std::setw(10) << valor;
        }
        std::cout << '\n';
    }
    std::cout << '\n';
}

// Imprime o vetor independente associado ao sistema linear.
void imprimir_vetor(const SistemaLinear& sistema)
{
    std::cout << "Vetor b\n";
    for (std::size_t i = 0; i < sistema.vetor.size(); ++i) {
        std::cout << "b[" << i << "] = " << sistema.vetor[i] << '\n';
    }
    std::cout << '\n';
}

// Compara dois escalares com uma tolerancia absoluta pequena.
[[nodiscard]] bool proximo(Real a, Real b, Real tolerancia = Real{1.0e-12})
{
    return std::abs(a - b) <= tolerancia;
}

// Confere se a montagem de Dirichlet duplo reproduz a matriz esperada no papel.
[[nodiscard]] bool conferir_dirichlet_duplo(const SistemaLinear& sistema)
{
    const std::vector<std::vector<Real>> matriz_esperada{
        { 1,  0,  0,  0,  0, 0},
        { 1, -2,  1,  0,  0, 0},
        { 0,  1, -2,  1,  0, 0},
        { 0,  0,  1, -2,  1, 0},
        { 0,  0,  0,  1, -2, 1},
        { 0,  0,  0,  0,  0, 1}
    };
    const std::vector<Real> vetor_esperado{0, -0.08, -0.08, -0.08, -0.08, 0};

    for (std::size_t i = 0; i < matriz_esperada.size(); ++i) {
        for (std::size_t j = 0; j < matriz_esperada[i].size(); ++j) {
            if (!proximo(sistema.matriz[i][j], matriz_esperada[i][j])) {
                return false;
            }
        }
        if (!proximo(sistema.vetor[i], vetor_esperado[i])) {
            return false;
        }
    }

    return true;
}

} // namespace

// Executa o roteiro completo do exercicio.
int main()
{
    std::cout << "Exercicio Computacional 3.2\n";
    std::cout << "Montando o sistema de Poisson em codigo\n";
    std::cout << "=======================================\n\n";

    const ProblemaPoisson problema{
        6,
        Real{0.0},
        Real{1.0},
        dirichlet(Real{0.0}),
        dirichlet(Real{0.0})
    };

    const auto fonte = [](Real) { return Real{-2.0}; };
    const SistemaLinear sistema = montar_sistema_poisson(problema, fonte);

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "Contorno esquerdo: " << nome(problema.esquerda.tipo) << '\n';
    std::cout << "Contorno direito : " << nome(problema.direita.tipo) << "\n\n";

    imprimir_nos(sistema.nos);
    imprimir_matriz(sistema);
    imprimir_vetor(sistema);

    const bool passou = conferir_dirichlet_duplo(sistema);
    std::cout << (passou ? "[PASSOU] " : "[FALHOU] ")
              << "matriz e vetor coincidem com o caso de Dirichlet duplo\n";

    // Demonstra que os construtores de Neumann e Robin tambem produzem a forma generica.
    const ProblemaPoisson exemplo_misto{5, Real{0.0}, Real{1.0}, neumann(Real{1.0}), robin(Real{2.0}, Real{1.0}, Real{3.0})};
    (void)montar_sistema_poisson(exemplo_misto, fonte);

    return passou ? 0 : 1;
}