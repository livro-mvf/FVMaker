// ----------------------------------------------------------------------------
// File: ex_MontaSistemaCapitulo05.cpp
// Project: FVMaker
// Description: Exercise 5.3 - assemble, but do not solve, the Chapter 5 system.
// ----------------------------------------------------------------------------
//
// Problem from Chapter 5:
//
//     d2(phi)/dx2 + x^2 = 0,  0 <= x <= 5,
//     phi(0) = 1,
//     dphi/dx(5) = 0.
//
// The mesh is generated with FVGridMaker. FVMaker then consumes the mesh through
// GridView1D and assembles the equation into a TridiagonalSystem1D. The final
// printout uses the book convention
//
//     AP[i] phi[i] = AW[i] phi[i - 1] + AE[i] phi[i + 1] + BP[i].
//
// The system is intentionally not solved here.
// ----------------------------------------------------------------------------

#include <array>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>

#include <FVGridMaker/Core/StrongTypes.h>
#include <FVGridMaker/OneDimensional/Distribution1D/Uniform1D.h>

#include <FVMaker/Core/Types.h>
#include <FVMaker/OneDimensional/Boundary/BoundaryCondition1D.h>
#include <FVMaker/OneDimensional/Grid/GridView1D.h>
#include <FVMaker/OneDimensional/Source/LinearizedSource1D.h>
#include <FVMaker/OneDimensional/System/TridiagonalSystem1D.h>
#include <FVMaker/OneDimensional/Term/Terms1D.h>

namespace {

struct VetoresLivro final {
    std::vector<fvm::Real> aw;
    std::vector<fvm::Real> ap;
    std::vector<fvm::Real> ae;
    std::vector<fvm::Real> bp;
};

[[nodiscard]] fvm::Real fonte_do_livro(fvm::Real x) {
    return x * x;
}

[[nodiscard]] VetoresLivro copiar_da_contribuicao(
    const fvm::EquationContribution1D& contribution
) {
    const fvm::Size n = contribution.size();

    VetoresLivro vetores{
        .aw = std::vector<fvm::Real>(n, fvm::Real{}),
        .ap = std::vector<fvm::Real>(n, fvm::Real{}),
        .ae = std::vector<fvm::Real>(n, fvm::Real{}),
        .bp = std::vector<fvm::Real>(n, fvm::Real{}),
    };

    for (fvm::Size i = 0; i < n; ++i) {
        vetores.aw[i] = contribution.aw()[i];
        vetores.ap[i] = contribution.ap()[i];
        vetores.ae[i] = contribution.ae()[i];
        vetores.bp[i] = contribution.bp()[i];
    }

    return vetores;
}

void imprimir_malha(const fvm::GridView1D& grid) {
    std::cout << "Malha gerada com FVGridMaker\n";
    std::cout << "===========================\n";
    std::cout << "padrao  : " << grid.pattern_name() << '\n';
    std::cout << "volumes : " << grid.num_volumes() << '\n';
    std::cout << "faces   : " << grid.num_faces() << '\n';
    std::cout << "xmin    : " << grid.xmin() << '\n';
    std::cout << "xmax    : " << grid.xmax() << "\n\n";

    std::cout << std::setw(5) << "i"
              << std::setw(14) << "xF[i]"
              << std::setw(14) << "xC[i]"
              << std::setw(14) << "Dx[i]"
              << '\n';

    for (fvm::Size i = 0; i < grid.num_faces(); ++i) {
        std::cout << std::setw(5) << i
                  << std::setw(14) << grid.faces()[i];

        if (i < grid.num_volumes()) {
            std::cout << std::setw(14) << grid.centers()[i]
                      << std::setw(14) << grid.cell_lengths()[i];
        }

        std::cout << '\n';
    }
}

void imprimir_vetores(const VetoresLivro& vetores) {
    std::cout << "\nVetores do sistema na convencao do livro\n";
    std::cout << "=======================================\n";
    std::cout << std::setw(5) << "i"
              << std::setw(12) << "AW"
              << std::setw(12) << "AP"
              << std::setw(12) << "AE"
              << std::setw(12) << "BP"
              << '\n';

    for (fvm::Size i = 0; i < vetores.ap.size(); ++i) {
        std::cout << std::setw(5) << i
                  << std::setw(12) << vetores.aw[i]
                  << std::setw(12) << vetores.ap[i]
                  << std::setw(12) << vetores.ae[i]
                  << std::setw(12) << vetores.bp[i]
                  << '\n';
    }
}

void imprimir_matriz(const VetoresLivro& vetores) {
    std::cout << "\nMatriz equivalente A phi = B\n";
    std::cout << "===========================\n";

    const fvm::Size n = vetores.ap.size();

    for (fvm::Size i = 0; i < n; ++i) {
        std::cout << "[";

        for (fvm::Size j = 0; j < n; ++j) {
            fvm::Real value = fvm::Real{};

            if (j == i) {
                value = vetores.ap[i];
            } else if (i > 0 && j + 1 == i) {
                value = -vetores.aw[i];
            } else if (j == i + 1) {
                value = -vetores.ae[i];
            }

            std::cout << std::setw(12) << value;
        }

        std::cout << " ] [phi" << i << "] = "
                  << std::setw(8) << vetores.bp[i] << '\n';
    }
}

[[nodiscard]] bool conferir_com_eq_565(const VetoresLivro& vetores) {
    constexpr std::array<fvm::Real, 5> aw_ref{0.0, 1.0, 1.0, 1.0, 1.0};
    constexpr std::array<fvm::Real, 5> ap_ref{3.0, 2.0, 2.0, 2.0, 1.0};
    constexpr std::array<fvm::Real, 5> ae_ref{1.0, 1.0, 1.0, 1.0, 0.0};
    constexpr std::array<fvm::Real, 5> bp_ref{2.25, 2.25, 6.25, 12.25, 20.25};

    constexpr fvm::Real tolerance = 1.0e-12;

    bool ok = true;

    for (fvm::Size i = 0; i < vetores.ap.size(); ++i) {
        ok = ok && std::abs(vetores.aw[i] - aw_ref[i]) < tolerance;
        ok = ok && std::abs(vetores.ap[i] - ap_ref[i]) < tolerance;
        ok = ok && std::abs(vetores.ae[i] - ae_ref[i]) < tolerance;
        ok = ok && std::abs(vetores.bp[i] - bp_ref[i]) < tolerance;
    }

    return ok;
}

}  // namespace

int main() {
    const fvgrid::Axis1D axis = fvgrid::uniform_axis_1d(
        fvgrid::NVol{5},
        fvgrid::Length{5.0},
        fvgrid::XInit{0.0}
    );

    const fvm::GridView1D grid{axis};

    const fvm::DiffusionTerm1D termo_difusao{1.0};
    const fvm::SourceTerm1D termo_fonte{
        fvm::function_source_1d(grid, fonte_do_livro)
    };
    const fvm::BoundaryTerm1D termo_contorno{
        fvm::BoundarySet1D{
            fvm::BoundaryCondition1D::dirichlet(1.0),
            fvm::BoundaryCondition1D::neumann(0.0)
        },
        1.0
    };

    const fvm::EquationContribution1D contribuicao =
        fvm::assemble(termo_difusao, grid)
      + fvm::assemble(termo_fonte, grid)
      + fvm::assemble(termo_contorno, grid);

    const fvm::TridiagonalSystem1D system =
        fvm::assemble_equation(contribuicao);
    (void)system;

    const VetoresLivro vetores = copiar_da_contribuicao(contribuicao);

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "Exercicio Computacional 5.3\n";
    std::cout << "Montagem do sistema do Exemplo 5.2, sem resolver\n\n";

    imprimir_malha(grid);
    imprimir_vetores(vetores);
    imprimir_matriz(vetores);

    const bool confere = conferir_com_eq_565(vetores);

    std::cout << "\nConferencia com a Eq. (5.65): "
              << (confere ? "ok" : "falhou")
              << '\n';

    return confere ? 0 : 1;
}
