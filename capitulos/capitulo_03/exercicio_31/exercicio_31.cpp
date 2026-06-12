//==============================================================================
// SPDX-FileCopyrightText: 2026 FVMaker Team
// SPDX-License-Identifier: MIT
//==============================================================================
// AVISO LEGAL / LEGAL DISCLAIMER
//
// [PT-BR]
// O código-fonte é fornecido sob a licença MIT, no estado em que se encontra
// ("as is"). Embora nos esforcemos para garantir o rigor matemático e a
// correção das implementações, a natureza da computação científica implica
// que inconsistências pontuais possam ocorrer.
//
// Caso identifique algum erro, comportamento inesperado, ou tenha sugestões de
// aprimoramento, seremos imensamente gratos se nos puder contactar através do
// e-mail livromvf@gmail.com. A sua contribuição é inestimável para o
// aperfeiçoamento contínuo deste livro e da sua biblioteca de apoio.
//
// [EN-GB]
// The source code is provided under the MIT Licence, on an "as is" basis.
// Whilst we endeavour to ensure mathematical rigour and correctness in the
// C++ implementations, the nature of scientific computing implies that
// occasional inconsistencies or errors may arise.
//
// Should you identify any bugs, unexpected behaviour, or have suggestions for
// improvement, we would be most grateful if you could reach out to us at
// livromvf@gmail.com. Your feedback is invaluable to the continuous refinement
// of this textbook and its accompanying library.
//==============================================================================

//==============================================================================
// Exercício Computacional 3.1
// Gerador de malhas aleatórias
//
// Este programa constrói uma malha aleatória unidimensional para o MDF.
//
// O programa recebe a coordenada inicial xI, o comprimento L > 0 e o número
// de nós N > 2. Em seguida, gera N - 2 posições aleatórias no interior do
// domínio (xI, xI + L), fixa os dois extremos, ordena os nós internos e
// imprime cada coordenada x_i e o respectivo espaçamento Delta x_i.
//
// Para completar o exercício, a malha gerada é usada nas fórmulas não
// uniformes da Seção 3.2. Como função de teste adotamos f(x) = exp(x), pois
// suas derivadas primeira e segunda são conhecidas exatamente. Com isso,
// calculamos os erros locais de f' e f'' em cada nó interno e observamos como
// eles se comportam quando os passos vizinhos saltam de forma abrupta.
//
// O código é autocontido e não depende da biblioteca em desenvolvimento.
//==============================================================================

//==============================================================================
// Bibliotecas padrão do C++
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

//==============================================================================
// Tipo real usado no programa
//==============================================================================

using Real = double;

//==============================================================================
// Estrutura de dados da malha
//==============================================================================

struct MalhaAleatoria1D {
    Real coordenada_inicial{};
    Real comprimento{};
    std::uint64_t semente{};
    std::vector<Real> nos;
};

//==============================================================================
// Conversão dos argumentos da linha de comando
//==============================================================================

[[nodiscard]] inline Real converter_real(
    const std::string& texto,
    const std::string& nome
)
{
    std::size_t posicao = 0;
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

[[nodiscard]] inline std::size_t converter_tamanho(
    const std::string& texto,
    const std::string& nome
)
{
    std::size_t posicao = 0;
    const unsigned long valor = std::stoul(texto, &posicao);

    if (posicao != texto.size()) {
        throw std::invalid_argument(
            "O argumento '" + nome + "' contém caracteres inválidos."
        );
    }

    return static_cast<std::size_t>(valor);
}

[[nodiscard]] inline std::uint64_t converter_semente(
    const std::string& texto,
    const std::string& nome
)
{
    std::size_t posicao = 0;
    const unsigned long long valor = std::stoull(texto, &posicao);

    if (posicao != texto.size()) {
        throw std::invalid_argument(
            "O argumento '" + nome + "' contém caracteres inválidos."
        );
    }

    return static_cast<std::uint64_t>(valor);
}

//==============================================================================
// Validação dos parâmetros da malha
//==============================================================================

inline void validar_parametros_malha(
    Real coordenada_inicial,
    Real comprimento,
    std::size_t numero_nos
)
{
    if (!std::isfinite(coordenada_inicial)) {
        throw std::invalid_argument("A coordenada inicial xI deve ser finita.");
    }

    if (!std::isfinite(comprimento)) {
        throw std::invalid_argument("O comprimento L deve ser finito.");
    }

    if (!(comprimento > Real{0.0})) {
        throw std::invalid_argument("O comprimento L deve ser maior que zero.");
    }

    if (numero_nos <= 2) {
        throw std::invalid_argument("O número de nós N deve ser maior que 2.");
    }

    const Real coordenada_final = coordenada_inicial + comprimento;

    if (!(coordenada_final > coordenada_inicial)) {
        throw std::invalid_argument(
            "A soma xI + L deve produzir uma coordenada final maior que xI."
        );
    }
}

//==============================================================================
// Geração da malha aleatória
//==============================================================================

[[nodiscard]] inline MalhaAleatoria1D gerar_malha_aleatoria(
    Real coordenada_inicial,
    Real comprimento,
    std::size_t numero_nos,
    std::uint64_t semente
)
{
    validar_parametros_malha(
        coordenada_inicial,
        comprimento,
        numero_nos
    );

    const Real coordenada_final = coordenada_inicial + comprimento;

    const Real limite_inferior =
        std::nextafter(coordenada_inicial, coordenada_final);

    const Real limite_superior =
        std::nextafter(coordenada_final, coordenada_inicial);

    if (!(limite_superior > limite_inferior)) {
        throw std::invalid_argument(
            "Não há intervalo interno suficiente para gerar a malha."
        );
    }

    std::mt19937_64 gerador(semente);
    std::uniform_real_distribution<Real> distribuicao(
        limite_inferior,
        limite_superior
    );

    std::vector<Real> nos;
    nos.reserve(numero_nos);
    nos.push_back(coordenada_inicial);

    for (std::size_t i = 0; i < numero_nos - 2; ++i) {
        nos.push_back(distribuicao(gerador));
    }

    nos.push_back(coordenada_final);

    std::sort(nos.begin() + 1, nos.end() - 1);

    for (std::size_t i = 1; i < nos.size(); ++i) {
        if (!(nos[i] > nos[i - 1])) {
            throw std::runtime_error(
                "A malha gerada não é estritamente crescente. "
                "Tente outra semente."
            );
        }
    }

    return MalhaAleatoria1D{
        coordenada_inicial,
        comprimento,
        semente,
        nos
    };
}

//==============================================================================
// Espaçamentos e estatísticas da malha
//==============================================================================

[[nodiscard]] inline std::vector<Real> calcular_espacamentos(
    const MalhaAleatoria1D& malha
)
{
    std::vector<Real> espacamentos;
    espacamentos.reserve(malha.nos.size() - 1);

    for (std::size_t i = 1; i < malha.nos.size(); ++i) {
        espacamentos.push_back(malha.nos[i] - malha.nos[i - 1]);
    }

    return espacamentos;
}

[[nodiscard]] inline Real soma(const std::vector<Real>& valores)
{
    return std::accumulate(valores.begin(), valores.end(), Real{0.0});
}

[[nodiscard]] inline Real minimo(const std::vector<Real>& valores)
{
    return *std::min_element(valores.begin(), valores.end());
}

[[nodiscard]] inline Real maximo(const std::vector<Real>& valores)
{
    return *std::max_element(valores.begin(), valores.end());
}

[[nodiscard]] inline Real media(const std::vector<Real>& valores)
{
    return soma(valores) / static_cast<Real>(valores.size());
}

[[nodiscard]] inline bool estritamente_crescente(
    const std::vector<Real>& valores
)
{
    for (std::size_t i = 1; i < valores.size(); ++i) {
        if (!(valores[i] > valores[i - 1])) {
            return false;
        }
    }

    return true;
}

//==============================================================================
// Fórmulas de diferenças finitas em malha não uniforme
//==============================================================================

[[nodiscard]] inline Real derivada_primeira_nao_uniforme(
    Real f_esquerda,
    Real f_centro,
    Real f_direita,
    Real passo_esquerda,
    Real passo_direita
)
{
    const Real soma_passos = passo_esquerda + passo_direita;

    const Real coeficiente_esquerda =
        -passo_direita / (soma_passos * passo_esquerda);

    const Real coeficiente_centro =
        (passo_direita - passo_esquerda)
        / (passo_direita * passo_esquerda);

    const Real coeficiente_direita =
        passo_esquerda / (soma_passos * passo_direita);

    return coeficiente_esquerda * f_esquerda
         + coeficiente_centro * f_centro
         + coeficiente_direita * f_direita;
}

[[nodiscard]] inline Real derivada_segunda_nao_uniforme(
    Real f_esquerda,
    Real f_centro,
    Real f_direita,
    Real passo_esquerda,
    Real passo_direita
)
{
    const Real soma_passos = passo_esquerda + passo_direita;

    return Real{2.0} * f_direita / (passo_direita * soma_passos)
         + Real{2.0} * f_esquerda / (passo_esquerda * soma_passos)
         - Real{2.0} * f_centro / (passo_esquerda * passo_direita);
}

//==============================================================================
// Impressão da malha e das estatísticas
//==============================================================================

inline void imprimir_malha(const MalhaAleatoria1D& malha)
{
    const auto espacamentos = calcular_espacamentos(malha);

    std::cout << "Nós da malha aleatória\n";
    std::cout << "======================\n";
    std::cout << std::setw(6) << "i"
              << std::setw(24) << "x_i"
              << std::setw(24) << "Delta x_i" << '\n';

    std::cout << std::setw(6) << 0
              << std::setw(24) << malha.nos[0]
              << std::setw(24) << "--" << '\n';

    for (std::size_t i = 1; i < malha.nos.size(); ++i) {
        std::cout << std::setw(6) << i
                  << std::setw(24) << malha.nos[i]
                  << std::setw(24) << espacamentos[i - 1] << '\n';
    }
}

inline void imprimir_estatisticas(
    const MalhaAleatoria1D& malha
)
{
    const auto espacamentos = calcular_espacamentos(malha);
    const Real dx_min = minimo(espacamentos);
    const Real dx_max = maximo(espacamentos);
    const Real dx_medio = media(espacamentos);
    const Real soma_dx = soma(espacamentos);

    std::cout << "\nEstatísticas da malha\n";
    std::cout << "=====================\n";
    std::cout << "Coordenada inicial xI         = "
              << malha.coordenada_inicial << '\n';
    std::cout << "Comprimento L                 = "
              << malha.comprimento << '\n';
    std::cout << "Semente                       = "
              << malha.semente << '\n';
    std::cout << "Número de nós                 = "
              << malha.nos.size() << '\n';
    std::cout << "Número de intervalos          = "
              << espacamentos.size() << '\n';
    std::cout << "Menor espaçamento             = "
              << dx_min << '\n';
    std::cout << "Maior espaçamento             = "
              << dx_max << '\n';
    std::cout << "Espaçamento médio             = "
              << dx_medio << '\n';
    std::cout << "Razão dx_max/dx_min           = "
              << dx_max / dx_min << '\n';
    std::cout << "Soma dos espaçamentos         = "
              << soma_dx << '\n';
    std::cout << "Erro na soma dos espaçamentos = "
              << std::abs(soma_dx - malha.comprimento) << '\n';
}

//==============================================================================
// Análise do erro das fórmulas não uniformes
//==============================================================================

inline void imprimir_analise_erro(const MalhaAleatoria1D& malha)
{
    const auto funcao = [](Real x) {
        return std::exp(x);
    };

    const auto derivada_exata = [](Real x) {
        return std::exp(x);
    };

    Real maior_erro_primeira = Real{0.0};
    Real maior_erro_segunda = Real{0.0};
    Real maior_salto_local = Real{0.0};

    std::cout << "\nAnálise do erro com f(x) = exp(x)\n";
    std::cout << "=================================\n";
    std::cout << "Para f(x) = exp(x), temos f'(x) = exp(x) e ";
    std::cout << "f''(x) = exp(x).\n";
    std::cout << "Os erros abaixo são calculados nos nós internos.\n\n";

    std::cout << std::setw(6) << "i"
              << std::setw(18) << "x_i"
              << std::setw(18) << "dx_esq"
              << std::setw(18) << "dx_dir"
              << std::setw(18) << "|dx_dir-dx_esq|"
              << std::setw(18) << "erro f'"
              << std::setw(18) << "erro f''" << '\n';

    for (std::size_t i = 1; i + 1 < malha.nos.size(); ++i) {
        const Real x_esquerda = malha.nos[i - 1];
        const Real x_centro = malha.nos[i];
        const Real x_direita = malha.nos[i + 1];

        const Real passo_esquerda = x_centro - x_esquerda;
        const Real passo_direita = x_direita - x_centro;
        const Real salto_local = std::abs(passo_direita - passo_esquerda);

        const Real f_esquerda = funcao(x_esquerda);
        const Real f_centro = funcao(x_centro);
        const Real f_direita = funcao(x_direita);

        const Real aproximacao_primeira =
            derivada_primeira_nao_uniforme(
                f_esquerda,
                f_centro,
                f_direita,
                passo_esquerda,
                passo_direita
            );

        const Real aproximacao_segunda =
            derivada_segunda_nao_uniforme(
                f_esquerda,
                f_centro,
                f_direita,
                passo_esquerda,
                passo_direita
            );

        const Real valor_exato = derivada_exata(x_centro);
        const Real erro_primeira =
            std::abs(aproximacao_primeira - valor_exato);
        const Real erro_segunda =
            std::abs(aproximacao_segunda - valor_exato);

        maior_erro_primeira = std::max(maior_erro_primeira, erro_primeira);
        maior_erro_segunda = std::max(maior_erro_segunda, erro_segunda);
        maior_salto_local = std::max(maior_salto_local, salto_local);

        std::cout << std::setw(6) << i
                  << std::setw(18) << x_centro
                  << std::setw(18) << passo_esquerda
                  << std::setw(18) << passo_direita
                  << std::setw(18) << salto_local
                  << std::setw(18) << erro_primeira
                  << std::setw(18) << erro_segunda << '\n';
    }

    std::cout << "\nResumo da análise do erro\n";
    std::cout << "=========================\n";
    std::cout << "Maior salto local entre passos = "
              << maior_salto_local << '\n';
    std::cout << "Maior erro em f'               = "
              << maior_erro_primeira << '\n';
    std::cout << "Maior erro em f''              = "
              << maior_erro_segunda << '\n';
}

//==============================================================================
// Funções auxiliares de teste
//==============================================================================

[[nodiscard]] inline bool aproximadamente_igual(
    Real a,
    Real b,
    Real tolerancia
)
{
    return std::abs(a - b) <= tolerancia;
}

inline bool testar_valor(
    const std::string& descricao,
    Real obtido,
    Real esperado,
    Real tolerancia
)
{
    const bool passou = aproximadamente_igual(obtido, esperado, tolerancia);

    std::cout << (passou ? "[PASSOU] " : "[FALHOU] ")
              << descricao << '\n';

    if (!passou) {
        std::cout << std::fixed << std::setprecision(16)
                  << "  obtido     = " << obtido << '\n'
                  << "  esperado   = " << esperado << '\n'
                  << "  tolerancia = " << tolerancia << '\n';
    }

    return passou;
}

inline bool testar_condicao(
    const std::string& descricao,
    bool condicao
)
{
    std::cout << (condicao ? "[PASSOU] " : "[FALHOU] ")
              << descricao << '\n';

    return condicao;
}

inline bool testar_excecao(
    const std::string& descricao,
    Real coordenada_inicial,
    Real comprimento,
    std::size_t numero_nos
)
{
    bool excecao_lancada = false;

    try {
        (void)gerar_malha_aleatoria(
            coordenada_inicial,
            comprimento,
            numero_nos,
            2026
        );
    } catch (const std::invalid_argument&) {
        excecao_lancada = true;
    } catch (...) {
        excecao_lancada = false;
    }

    std::cout << (excecao_lancada ? "[PASSOU] " : "[FALHOU] ")
              << descricao << '\n';

    if (!excecao_lancada) {
        std::cout << "  A chamada deveria ter lançado std::invalid_argument.\n";
    }

    return excecao_lancada;
}

inline unsigned executar_testes()
{
    unsigned testes_passaram = 0;
    unsigned testes_total = 0;

    std::cout << "\nTestes internos\n";
    std::cout << "===============\n";

    const MalhaAleatoria1D malha =
        gerar_malha_aleatoria(Real{0.0}, Real{1.0}, 10, 2026);

    const auto espacamentos = calcular_espacamentos(malha);

    ++testes_total;
    if (
        testar_condicao(
            "A malha gerada é estritamente crescente",
            estritamente_crescente(malha.nos)
        )
    ) {
        ++testes_passaram;
    }

    ++testes_total;
    if (
        testar_valor(
            "O primeiro nó coincide com xI",
            malha.nos.front(),
            Real{0.0},
            Real{0.0}
        )
    ) {
        ++testes_passaram;
    }

    ++testes_total;
    if (
        testar_valor(
            "O último nó coincide com xI + L",
            malha.nos.back(),
            Real{1.0},
            Real{0.0}
        )
    ) {
        ++testes_passaram;
    }

    ++testes_total;
    if (
        testar_valor(
            "A soma dos espaçamentos recupera L",
            soma(espacamentos),
            Real{1.0},
            Real{1e-15}
        )
    ) {
        ++testes_passaram;
    }

    const Real x_esquerda = Real{0.0};
    const Real x_centro = Real{0.4};
    const Real x_direita = Real{1.0};
    const Real passo_esquerda = x_centro - x_esquerda;
    const Real passo_direita = x_direita - x_centro;

    const auto linear = [](Real x) {
        return Real{3.0} * x + Real{2.0};
    };

    const Real derivada_linear =
        derivada_primeira_nao_uniforme(
            linear(x_esquerda),
            linear(x_centro),
            linear(x_direita),
            passo_esquerda,
            passo_direita
        );

    ++testes_total;
    if (
        testar_valor(
            "A fórmula não uniforme de f' é exata para função linear",
            derivada_linear,
            Real{3.0},
            Real{1e-14}
        )
    ) {
        ++testes_passaram;
    }

    const auto quadratica = [](Real x) {
        return x * x;
    };

    const Real derivada_segunda_quadratica =
        derivada_segunda_nao_uniforme(
            quadratica(x_esquerda),
            quadratica(x_centro),
            quadratica(x_direita),
            passo_esquerda,
            passo_direita
        );

    ++testes_total;
    if (
        testar_valor(
            "A fórmula não uniforme de f'' é exata para f(x) = x^2",
            derivada_segunda_quadratica,
            Real{2.0},
            Real{1e-14}
        )
    ) {
        ++testes_passaram;
    }

    testes_total += 3;

    if (
        testar_excecao(
            "N = 2 deve ser rejeitado",
            Real{0.0},
            Real{1.0},
            2
        )
    ) {
        ++testes_passaram;
    }

    if (
        testar_excecao(
            "L = 0 deve ser rejeitado",
            Real{0.0},
            Real{0.0},
            8
        )
    ) {
        ++testes_passaram;
    }

    if (
        testar_excecao(
            "L < 0 deve ser rejeitado",
            Real{0.0},
            Real{-1.0},
            8
        )
    ) {
        ++testes_passaram;
    }

    std::cout << "\nResumo dos testes\n";
    std::cout << "=================\n";
    std::cout << "Testes aprovados: " << testes_passaram
              << " de " << testes_total << '\n';

    return (testes_passaram == testes_total) ? 0u : 1u;
}

//==============================================================================
// Mensagem final didática
//==============================================================================

inline void imprimir_mensagem_final()
{
    constexpr int size = 80;

    std::cout << "\nAplicações e recomendações\n";
    std::cout << std::string(size, '=') << '\n';
    std::cout << "1. A ordenação dos nós internos garante uma malha válida, ";
    std::cout << "com x_i < x_{i+1}.\n";
    std::cout << "2. A aleatoriedade não é removida: ela aparece nos saltos ";
    std::cout << "entre espaçamentos consecutivos.\n";
    std::cout << "3. A semente deve ser registrada para tornar a malha ";
    std::cout << "reprodutível.\n";
    std::cout << "4. Os extremos xI e xI + L são fixados explicitamente.\n";
    std::cout << "5. Os erros das derivadas devem ser avaliados apenas nos ";
    std::cout << "nós internos.\n";
    std::cout << std::string(size, '=') << '\n';

    std::cout << "\nConceitos demonstrados\n";
    std::cout << std::string(size, '=') << '\n';
    std::cout << "1. Geração de N - 2 nós aleatórios no interior do domínio.\n";
    std::cout << "2. Cálculo dos espaçamentos Delta x_i a partir dos nós.\n";
    std::cout << "3. Aplicação das fórmulas não uniformes de primeira e ";
    std::cout << "segunda derivadas.\n";
    std::cout << "4. Comparação com derivadas exatas usando f(x) = exp(x).\n";
    std::cout << "5. Observação do efeito de saltos abruptos nos passos ";
    std::cout << "vizinhos.\n";
    std::cout << std::string(size, '=') << '\n';
}

//==============================================================================
// Instruções de uso
//==============================================================================

inline void imprimir_uso(const char* nome_programa)
{
    std::cout << "Uso:\n";
    std::cout << "  " << nome_programa << "\n";
    std::cout << "  " << nome_programa << " xI L N [semente]\n\n";
    std::cout << "Exemplo:\n";
    std::cout << "  " << nome_programa << " 0.0 1.0 12 2026\n";
}

//==============================================================================
// Programa principal
//==============================================================================

int main(int argc, char* argv[])
{
    try {
        Real coordenada_inicial = Real{0.0};
        Real comprimento = Real{1.0};
        std::size_t numero_nos = 12;
        std::uint64_t semente = 2026;

        if (!(argc == 1 || argc == 4 || argc == 5)) {
            imprimir_uso(argv[0]);
            return 1;
        }

        if (argc >= 4) {
            coordenada_inicial = converter_real(argv[1], "xI");
            comprimento = converter_real(argv[2], "L");
            numero_nos = converter_tamanho(argv[3], "N");
        }

        if (argc == 5) {
            semente = converter_semente(argv[4], "semente");
        }

        std::cout << std::fixed << std::setprecision(12);
        std::cout << "Exercício Computacional 3.1\n";
        std::cout << "Gerador de malhas aleatórias\n";
        std::cout << "================================\n\n";

        const MalhaAleatoria1D malha =
            gerar_malha_aleatoria(
                coordenada_inicial,
                comprimento,
                numero_nos,
                semente
            );

        imprimir_malha(malha);
        imprimir_estatisticas(malha);
        imprimir_analise_erro(malha);

        const unsigned status_testes = executar_testes();

        imprimir_mensagem_final();

        return (status_testes == 0u) ? 0 : 1;

    } catch (const std::exception& erro) {
        std::cerr << "Erro inesperado: " << erro.what() << '\n';
        return 1;
    }
}