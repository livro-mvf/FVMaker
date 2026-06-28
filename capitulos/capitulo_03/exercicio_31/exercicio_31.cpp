// ============================================================================
// Arquivo: exercicio_31.cpp
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Implementa exercicio 31 no contexto de capitulos / capitulo_03 / exercicio_31.
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

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <random>

using Real = double;

namespace {

struct Parametros
{
    Real x_inicial{0.0};
    Real comprimento{1.0};
    std::size_t numero_nos{12};
    std::uint64_t semente{2026};
};

struct MalhaAleatoria1D
{
    Real x_inicial{};
    Real comprimento{};
    std::uint64_t semente{};
    std::vector<Real> nos;
};

struct ResultadoDerivada
{
    std::size_t indice{};
    Real x{};
    Real dx_esquerda{};
    Real dx_direita{};
    Real valor{};
    Real aproximada{};
    Real exata{};
    Real erro{};
    Real indicador_erro{};
};

[[nodiscard]] Real converter_real(
    const std::string& texto,
    const std::string& nome
)
{
    std::size_t posicao{};
    const Real valor = std::stod(texto, &posicao);

    if (posicao != texto.size()) {
        throw std::invalid_argument(
            "O argumento '" + nome + "' contem caracteres invalidos."
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
)
{
    std::size_t posicao{};
    const unsigned long valor = std::stoul(texto, &posicao);

    if (posicao != texto.size()) {
        throw std::invalid_argument(
            "O argumento '" + nome + "' contem caracteres invalidos."
        );
    }

    return static_cast<std::size_t>(valor);
}

[[nodiscard]] std::uint64_t converter_semente(
    const std::string& texto,
    const std::string& nome
)
{
    std::size_t posicao{};
    const unsigned long long valor = std::stoull(texto, &posicao);

    if (posicao != texto.size()) {
        throw std::invalid_argument(
            "O argumento '" + nome + "' contem caracteres invalidos."
        );
    }

    return static_cast<std::uint64_t>(valor);
}

void validar_parametros(
    Real x_inicial,
    Real comprimento,
    std::size_t numero_nos
)
{
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
            "xI + L deve ser maior que xI na aritmetica usada."
        );
    }
}

[[nodiscard]] MalhaAleatoria1D gerar_malha_aleatoria(
    Real x_inicial,
    Real comprimento,
    std::size_t numero_nos,
    std::uint64_t semente
)
{
    validar_parametros(x_inicial, comprimento, numero_nos);

    const Real x_final = x_inicial + comprimento;
    const Real menor_interno = std::nextafter(x_inicial, x_final);
    const Real maior_interno = std::nextafter(x_final, x_inicial);

    if (!(maior_interno > menor_interno)) {
        throw std::invalid_argument(
            "O dominio nao possui intervalo interno representavel."
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

[[nodiscard]] MalhaAleatoria1D gerar_malha_uniforme(
    Real x_inicial,
    Real comprimento,
    std::size_t numero_nos
)
{
    validar_parametros(x_inicial, comprimento, numero_nos);

    const Real passo =
        comprimento / static_cast<Real>(numero_nos - 1);

    std::vector<Real> nos;
    nos.reserve(numero_nos);

    for (std::size_t i = 0; i < numero_nos; ++i) {
        nos.push_back(x_inicial + static_cast<Real>(i) * passo);
    }

    nos.back() = x_inicial + comprimento;

    return MalhaAleatoria1D{x_inicial, comprimento, 0u, nos};
}

[[nodiscard]] std::vector<Real> calcular_espacamentos(
    const std::vector<Real>& nos
)
{
    std::vector<Real> espacamentos;
    espacamentos.reserve(nos.size() - 1);

    for (std::size_t i = 1; i < nos.size(); ++i) {
        espacamentos.push_back(nos[i] - nos[i - 1]);
    }

    return espacamentos;
}

[[nodiscard]] Real soma(const std::vector<Real>& valores)
{
    return std::accumulate(valores.begin(), valores.end(), Real{});
}

[[nodiscard]] bool aproximadamente_igual(
    Real a,
    Real b,
    Real tolerancia
)
{
    return std::abs(a - b) <= tolerancia;
}

[[nodiscard]] bool nos_estritamente_crescentes(
    const std::vector<Real>& nos
)
{
    for (std::size_t i = 1; i < nos.size(); ++i) {
        if (!(nos[i - 1] < nos[i])) {
            return false;
        }
    }

    return true;
}

[[nodiscard]] bool espacamentos_positivos(
    const std::vector<Real>& espacamentos
)
{
    return std::all_of(
        espacamentos.begin(),
        espacamentos.end(),
        [](Real dx) {
            return dx > Real{};
        }
    );
}

[[nodiscard]] std::vector<Real> avaliar_exponencial(
    const std::vector<Real>& nos
)
{
    std::vector<Real> valores;
    valores.reserve(nos.size());

    for (const Real x : nos) {
        valores.push_back(std::exp(x));
    }

    return valores;
}

[[nodiscard]] Real segunda_derivada_nao_uniforme(
    const std::vector<Real>& nos,
    const std::vector<Real>& valores,
    std::size_t i
)
{
    const Real dx_esquerda = nos[i] - nos[i - 1];
    const Real dx_direita = nos[i + 1] - nos[i];

    const Real inclinacao_esquerda =
        (valores[i] - valores[i - 1]) / dx_esquerda;

    const Real inclinacao_direita =
        (valores[i + 1] - valores[i]) / dx_direita;

    return Real{2.0}
        * (inclinacao_direita - inclinacao_esquerda)
        / (dx_esquerda + dx_direita);
}

[[nodiscard]] std::vector<ResultadoDerivada> calcular_estudo_derivada(
    const std::vector<Real>& nos
)
{
    const std::vector<Real> valores = avaliar_exponencial(nos);

    std::vector<ResultadoDerivada> resultados;
    resultados.reserve(nos.size() - 2);

    for (std::size_t i = 1; i + 1 < nos.size(); ++i) {
        const Real dx_esquerda = nos[i] - nos[i - 1];
        const Real dx_direita = nos[i + 1] - nos[i];
        const Real aproximada =
            segunda_derivada_nao_uniforme(nos, valores, i);
        const Real exata = std::exp(nos[i]);
        const Real erro = std::abs(aproximada - exata);
        const Real indicador =
            std::abs(dx_esquerda - dx_direita) * std::abs(exata);

        resultados.push_back(
            ResultadoDerivada{
                i,
                nos[i],
                dx_esquerda,
                dx_direita,
                valores[i],
                aproximada,
                exata,
                erro,
                indicador
            }
        );
    }

    return resultados;
}

[[nodiscard]] Real maior_erro(
    const std::vector<ResultadoDerivada>& resultados
)
{
    Real maior = Real{};

    for (const ResultadoDerivada& resultado : resultados) {
        maior = std::max(maior, resultado.erro);
    }

    return maior;
}

[[nodiscard]] Real erro_medio(
    const std::vector<ResultadoDerivada>& resultados
)
{
    if (resultados.empty()) {
        return Real{};
    }

    Real total = Real{};

    for (const ResultadoDerivada& resultado : resultados) {
        total += resultado.erro;
    }

    return total / static_cast<Real>(resultados.size());
}

[[nodiscard]] std::size_t indice_maior_erro(
    const std::vector<ResultadoDerivada>& resultados
)
{
    return static_cast<std::size_t>(
        std::max_element(
            resultados.begin(),
            resultados.end(),
            [](const ResultadoDerivada& a, const ResultadoDerivada& b) {
                return a.erro < b.erro;
            }
        ) - resultados.begin()
    );
}

void imprimir_malha(const MalhaAleatoria1D& malha)
{
    const std::vector<Real> espacamentos =
        calcular_espacamentos(malha.nos);

    std::cout << "Malha aleatoria\n";
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

void imprimir_tabela_derivada(
    const std::string& titulo,
    const std::vector<ResultadoDerivada>& resultados
)
{
    std::cout << '\n' << titulo << '\n';
    std::cout << std::string(titulo.size(), '=') << '\n';
    std::cout << std::setw(6) << "i"
              << std::setw(18) << "x_i"
              << std::setw(18) << "dx_i"
              << std::setw(18) << "dx_{i+1}"
              << std::setw(18) << "f(x_i)"
              << std::setw(18) << "f'' aprox."
              << std::setw(18) << "f'' exata"
              << std::setw(18) << "erro abs."
              << std::setw(18) << "indicador" << '\n';

    for (const ResultadoDerivada& resultado : resultados) {
        std::cout << std::setw(6) << resultado.indice
                  << std::setw(18) << resultado.x
                  << std::setw(18) << resultado.dx_esquerda
                  << std::setw(18) << resultado.dx_direita
                  << std::setw(18) << resultado.valor
                  << std::setw(18) << resultado.aproximada
                  << std::setw(18) << resultado.exata
                  << std::setw(18) << resultado.erro
                  << std::setw(18) << resultado.indicador_erro << '\n';
    }
}

void imprimir_estudo_derivada(
    const MalhaAleatoria1D& malha_aleatoria,
    const MalhaAleatoria1D& malha_uniforme
)
{
    const std::vector<ResultadoDerivada> resultados_aleatorios =
        calcular_estudo_derivada(malha_aleatoria.nos);

    const std::vector<ResultadoDerivada> resultados_uniformes =
        calcular_estudo_derivada(malha_uniforme.nos);

    imprimir_tabela_derivada(
        "Estimativa de f'' na malha aleatoria",
        resultados_aleatorios
    );

    imprimir_tabela_derivada(
        "Estimativa de f'' na malha uniforme",
        resultados_uniformes
    );

    const std::size_t pos_maior_erro_aleatorio =
        indice_maior_erro(resultados_aleatorios);

    const ResultadoDerivada& pior =
        resultados_aleatorios[pos_maior_erro_aleatorio];

    const Real menor_dx = std::min(pior.dx_esquerda, pior.dx_direita);
    const Real maior_dx = std::max(pior.dx_esquerda, pior.dx_direita);

    std::cout << "\nComparacao dos erros\n";
    std::cout << "====================\n";
    std::cout << std::setw(6) << "i"
              << std::setw(18) << "erro aleat."
              << std::setw(18) << "erro unif."
              << std::setw(18) << "razao"
              << std::setw(18) << "indicador" << '\n';

    for (std::size_t k = 0; k < resultados_aleatorios.size(); ++k) {
        const Real erro_uniforme = resultados_uniformes[k].erro;
        const Real razao =
            (erro_uniforme > Real{})
                ? resultados_aleatorios[k].erro / erro_uniforme
                : std::numeric_limits<Real>::infinity();

        std::cout << std::setw(6) << resultados_aleatorios[k].indice
                  << std::setw(18) << resultados_aleatorios[k].erro
                  << std::setw(18) << erro_uniforme
                  << std::setw(18) << razao
                  << std::setw(18)
                  << resultados_aleatorios[k].indicador_erro << '\n';
    }

    const Real maior_erro_aleatorio = maior_erro(resultados_aleatorios);
    const Real maior_erro_uniforme = maior_erro(resultados_uniformes);
    const Real erro_medio_aleatorio = erro_medio(resultados_aleatorios);
    const Real erro_medio_uniforme = erro_medio(resultados_uniformes);

    std::cout << "\nResumo comparativo\n";
    std::cout << "==================\n";
    std::cout << "maior erro aleatorio       = "
              << maior_erro_aleatorio << '\n';
    std::cout << "maior erro uniforme        = "
              << maior_erro_uniforme << '\n';
    std::cout << "razao dos maiores erros    = "
              << (maior_erro_aleatorio / maior_erro_uniforme) << '\n';
    std::cout << "erro medio aleatorio       = "
              << erro_medio_aleatorio << '\n';
    std::cout << "erro medio uniforme        = "
              << erro_medio_uniforme << '\n';
    std::cout << "razao dos erros medios     = "
              << (erro_medio_aleatorio / erro_medio_uniforme) << '\n';

    std::cout << "\nMaior erro local na malha aleatoria\n";
    std::cout << "===================================\n";
    std::cout << "i              = " << pior.indice << '\n';
    std::cout << "erro absoluto  = " << pior.erro << '\n';
    std::cout << "indicador      = " << pior.indicador_erro << '\n';
    std::cout << "dx_i           = " << pior.dx_esquerda << '\n';
    std::cout << "dx_{i+1}       = " << pior.dx_direita << '\n';
    std::cout << "salto local    = " << (maior_dx / menor_dx) << '\n';
}

[[nodiscard]] bool registrar_teste(
    const std::string& descricao,
    bool passou
)
{
    std::cout << (passou ? "[PASSOU] " : "[FALHOU] ")
              << descricao << '\n';
    return passou;
}

[[nodiscard]] unsigned verificar_malha(const MalhaAleatoria1D& malha)
{
    constexpr Real tolerancia = 1.0e-12;

    const Real x_final = malha.x_inicial + malha.comprimento;
    const std::vector<Real> espacamentos =
        calcular_espacamentos(malha.nos);

    unsigned aprovados{};
    unsigned total{};

    std::cout << "\nVerificacoes automaticas\n";
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
    std::cout << "Verificacoes aprovadas: "
              << aprovados << " de " << total << '\n';
    std::cout << "Soma dos espacamentos: "
              << soma(espacamentos) << '\n';
    std::cout << "Erro na soma: "
              << std::abs(soma(espacamentos) - malha.comprimento) << '\n';

    return (aprovados == total) ? 0u : 1u;
}

void imprimir_mensagem_final()
{
    std::cout << "\nAplicacoes e recomendacoes\n";
    std::cout << "--------------------------\n";
    std::cout << "1. Os valores numericos mudam quando a semente muda. ";
    std::cout << "As propriedades verificadas acima devem permanecer ";
    std::cout << "verdadeiras em toda execucao valida.\n";
    std::cout << "2. Os nos e os Delta x_i impressos aqui alimentam ";
    std::cout << "as formulas nao uniformes da Secao 3.2.\n";
    std::cout << "3. A tabela de f'' mostra o erro ponto a ponto para ";
    std::cout << "f(x) = exp(x), cuja segunda derivada exata tambem ";
    std::cout << "e exp(x).\n";
    std::cout << "4. A malha uniforme com o mesmo N serve de referencia ";
    std::cout << "para medir o preco da irregularidade.\n";
    std::cout << "5. O indicador |dx_i - dx_{i+1}| |f'''(x_i)| ajuda ";
    std::cout << "a localizar onde o erro da malha aleatoria tende ";
    std::cout << "a crescer.\n";
}

void imprimir_uso(const char* nome_programa)
{
    std::cout << "Uso:\n";
    std::cout << "  " << nome_programa << '\n';
    std::cout << "  " << nome_programa << " xI L N [semente]\n\n";
    std::cout << "Exemplo:\n";
    std::cout << "  " << nome_programa << " 0.0 1.0 12 2026\n";
}

[[nodiscard]] Parametros ler_parametros(int argc, char* argv[])
{
    Parametros parametros;

    if (!(argc == 1 || argc == 4 || argc == 5)) {
        imprimir_uso(argv[0]);
        throw std::invalid_argument("Numero invalido de argumentos.");
    }

    if (argc >= 4) {
        parametros.x_inicial = converter_real(argv[1], "xI");
        parametros.comprimento = converter_real(argv[2], "L");
        parametros.numero_nos = converter_tamanho(argv[3], "N");
    }

    if (argc == 5) {
        parametros.semente = converter_semente(argv[4], "semente");
    }

    return parametros;
}

} // namespace

int main(int argc, char* argv[])
{
    try {
        const Parametros parametros = ler_parametros(argc, argv);

        std::cout << std::fixed << std::setprecision(12);
        std::cout << "Exercicio Computacional 3.1\n";
        std::cout << "Gerador de malhas aleatorias\n";
        std::cout << "============================\n\n";

        const MalhaAleatoria1D malha = gerar_malha_aleatoria(
            parametros.x_inicial,
            parametros.comprimento,
            parametros.numero_nos,
            parametros.semente
        );

        const MalhaAleatoria1D malha_uniforme = gerar_malha_uniforme(
            parametros.x_inicial,
            parametros.comprimento,
            parametros.numero_nos
        );

        imprimir_malha(malha);
        const unsigned status = verificar_malha(malha);
        imprimir_estudo_derivada(malha, malha_uniforme);
        imprimir_mensagem_final();

        return static_cast<int>(status);
    } catch (const std::exception& erro) {
        std::cerr << "Erro: " << erro.what() << '\n';
        return 1;
    }
}
