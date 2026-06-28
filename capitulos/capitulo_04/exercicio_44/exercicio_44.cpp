// ============================================================================
// Arquivo: exercicio_44.cpp
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Implementa exercicio 44 no contexto de capitulos / capitulo_04 / exercicio_44.
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
#include <cmath>
#include <iostream>
#include <random>
#include <string>
#include <string_view>
#include <vector>

//==============================================================================
// Header FVGridMaker
//==============================================================================
#include <FVGridMaker/OneDimensional/Distribution1D/Custom1D.h>
#include <FVGridMaker/Output/CSV/Axis1DCSVWriter.h>

namespace {

struct FacesPonderadasDosCentros1D final {
    using primary_coordinates = fvgrid::CenterCoordinates1D;

    [[nodiscard]] static constexpr std::string_view name() noexcept {
        return "FacesPonderadasDosCentros1D";
    }


    [[nodiscard]] static fvgrid::AxisGeometry1D complete_geometry(
        std::vector<fvgrid::Real> centers,
        fvgrid::Domain1D domain
    ) {
        const fvgrid::Size nvol = centers.size();

        std::vector<fvgrid::Real> faces(nvol + 1);
        faces.front() = domain.xmin();
        faces.back() = domain.xmax();

        for (fvgrid::Size i = 1; i < nvol; ++i) {
            faces[i] = 0.25 * centers[i - 1] + 0.75 * centers[i];
        }

        return fvgrid::AxisGeometry1D{
            .faces = std::move(faces),
            .centers = std::move(centers),
            .pattern_name = std::string{name()},
        };
    }
};

class MalhaSimetrica1D final {
public:
    MalhaSimetrica1D(
        fvgrid::Length comprimento,
        fvgrid::NVol numero_de_volumes,
        fvgrid::XInit x_inicial,
        fvgrid::Seed semente
    )
        : comprimento_{comprimento},
          numero_de_volumes_{numero_de_volumes},
          x_inicial_{x_inicial},
          semente_{semente} {}

    [[nodiscard]] fvgrid::Axis1D gerar() const {
        std::vector<fvgrid::Real> centros = gerar_centros();

        return fvgrid::Custom1D::make(
            fvgrid::Coordinates1D::centers(std::move(centros)),
            FacesPonderadasDosCentros1D{},
            fvgrid::Domain1D::from_length(x_inicial_, comprimento_)
        );
    }

private:
    [[nodiscard]] std::vector<fvgrid::Real> gerar_centros() const {
        const fvgrid::Size n = numero_de_volumes_.value();
        const fvgrid::Real xmin = x_inicial_.value();
        const fvgrid::Real xmax = xmin + comprimento_.value();
        const fvgrid::Real xmeio = 0.5 * (xmin + xmax);

        const fvgrid::Size metade = n / 2;

        std::mt19937_64 gerador{semente_.value()};
        std::uniform_real_distribution<fvgrid::Real> distribuicao{xmin, xmeio};

        std::vector<fvgrid::Real> centros_esquerda(metade);

        for (fvgrid::Size i = 0; i < metade; ++i) {
            centros_esquerda[i] = distribuicao(gerador);
        }

        std::sort(centros_esquerda.begin(), centros_esquerda.end());

        std::vector<fvgrid::Real> centros(n);

        for (fvgrid::Size i = 0; i < metade; ++i) {
            centros[i] = centros_esquerda[i];
        }

        if (n % 2 != 0) {
            centros[metade] = xmeio;
        }

        for (fvgrid::Size i = 0; i < metade; ++i) {
            centros[n - 1 - i] = 2.0 * xmeio - centros_esquerda[i];
        }

        return centros;
    }

    fvgrid::Length comprimento_;
    fvgrid::NVol numero_de_volumes_;
    fvgrid::XInit x_inicial_;
    fvgrid::Seed semente_;
};

[[nodiscard]] bool registrar_teste(
    const std::string& descricao,
    bool passou
) {
    std::cout << (passou ? "[PASSOU] " : "[FALHOU] ")
              << descricao << '\n';
    return passou;
}

[[nodiscard]] bool centros_simetricos(
    const fvgrid::Axis1D& malha,
    fvgrid::Real tolerancia
) {
    const auto centros = malha.centers();
    const fvgrid::Real xmeio = 0.5 * (malha.xmin() + malha.xmax());

    bool passou = true;

    for (fvgrid::Size i = 0; i < centros.size(); ++i) {
        const fvgrid::Size j = centros.size() - 1 - i;

        if (i > j) {
            break;
        }

        const fvgrid::Real residuo =
            std::abs((centros[i] + centros[j]) - 2.0 * xmeio);

        if (residuo > tolerancia) {
            std::cout << "  centros[" << i << "] + centros[" << j
                      << "] difere de 2*xmeio por "
                      << residuo << '\n';
            passou = false;
        }
    }

    return passou;
}

[[nodiscard]] bool faces_ponderadas_corretas(
    const fvgrid::Axis1D& malha,
    fvgrid::Real tolerancia
) {
    const auto faces = malha.faces();
    const auto centros = malha.centers();

    bool passou = true;

    for (fvgrid::Size i = 1; i + 1 < faces.size(); ++i) {
        const fvgrid::Real face_esperada =
            0.25 * centros[i - 1] + 0.75 * centros[i];

        const fvgrid::Real residuo = std::abs(faces[i] - face_esperada);

        if (residuo > tolerancia) {
            std::cout << "  face[" << i
                      << "] nao obedece a regra 25/75; residuo = "
                      << residuo << '\n';
            passou = false;
        }
    }

    return passou;
}

void imprimir_razao_de_espacamentos(const fvgrid::Axis1D& malha) {
    const auto dx = malha.cell_lengths();
    const auto [min_it, max_it] = std::minmax_element(dx.begin(), dx.end());

    std::cout << "Dx_max/Dx_min = " << (*max_it / *min_it) << "\n\n";
}

[[nodiscard]] unsigned verificar_malha(
    const fvgrid::Axis1D& malha,
    fvgrid::Real tolerancia
) {
    unsigned aprovados{};
    unsigned total{};

    std::cout << "\nVerificacoes automaticas\n";
    std::cout << "========================\n";

    ++total;
    aprovados += registrar_teste(
        "centros simetricos em relacao ao ponto medio",
        centros_simetricos(malha, tolerancia)
    );

    ++total;
    aprovados += registrar_teste(
        "faces internas obedecem a regra 25/75",
        faces_ponderadas_corretas(malha, tolerancia)
    );

    std::cout << "\nResumo\n";
    std::cout << "======\n";
    std::cout << "Verificacoes aprovadas: "
              << aprovados << " de " << total << '\n';

    return (aprovados == total) ? 0u : 1u;
}

void imprimir_mensagem_final()
{
    std::cout << "\nAplicacoes e recomendacoes\n";
    std::cout << "--------------------------\n";
    std::cout << "1. A construcao especializada fica no exercicio, ";
    std::cout << "mantendo a biblioteca focada em componentes reutilizaveis.\n";
    std::cout << "2. A simetria dos centros pode ser usada para testar ";
    std::cout << "malhas nao uniformes com propriedades geometricas impostas.\n";
    std::cout << "3. A razao Dx_max/Dx_min ajuda a avaliar o grau de ";
    std::cout << "nao uniformidade antes de resolver uma equacao numerica.\n";
}

} // namespace

int main()
{
    try {
        const fvgrid::Length comprimento{1.0};
        const fvgrid::NVol numero_de_volumes{21};
        const fvgrid::XInit x_inicial{0.0};
        const fvgrid::Seed semente{2026};

        constexpr fvgrid::Real tolerancia = 1.0e-12;

        const MalhaSimetrica1D gerador{
            comprimento,
            numero_de_volumes,
            x_inicial,
            semente
        };

        const fvgrid::Axis1D malha = gerador.gerar();

        std::cout << std::fixed << std::setprecision(8);
        std::cout << "Exercicio Computacional 4.4\n";
        std::cout << "Malha simetrica definida pelo usuario\n";
        std::cout << "=====================================\n\n";

        std::cout << malha << '\n';
        imprimir_razao_de_espacamentos(malha);

        const unsigned status = verificar_malha(malha, tolerancia);
        fvgrid::Axis1DCSVWriter::write(
            malha,
            "exercicio_44_malha_simetrica.csv"
        );
        std::cout << "\nCSV gerado: exercicio_44_malha_simetrica.csv\n";
        imprimir_mensagem_final();

        return static_cast<int>(status);
    } catch (const std::exception& erro) {
        std::cerr << "Erro: " << erro.what() << '\n';
        return 1;
    }
}
