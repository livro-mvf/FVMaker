// ============================================================================
// Arquivo: tst_Variable1DVTKWriter.cc
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Testa tst Variable 1 DVTKWriter no contexto de Output.
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

#include <sstream>
#include <string>
#include <vector>

#include <FVGridMaker/Core/StrongTypes.h>
#include <FVGridMaker/OneDimensional/Distribution1D/Uniform1D.h>

#include <FVMaker/OneDimensional/Grid/GridView1D.h>
#include <FVMaker/OneDimensional/Variable/Variable1D.h>
#include <FVMaker/Output/VTK/Variable1DVTKWriter.h>

#include <gtest/gtest.h>

namespace fvm {

TEST(Variable1DVTKWriter, WritesLegacyRectilinearGridWithVariableStates) {
    static const fvgrid::Axis1D axis = fvgrid::uniform_axis_1d(
        fvgrid::NVol{3},
        fvgrid::Length{1.0},
        fvgrid::XInit{0.0}
    );
    const GridView1D grid{axis};
    Variable1D variable{grid, "phi", std::vector<Real>{1.0, 2.0, 3.0}};
    variable.assign_analytic([](Real) {
        return 1.0;
    });

    std::ostringstream output;
    Variable1DVTKWriter::write(variable, output);

    const std::string text = output.str();
    EXPECT_NE(text.find("DATASET RECTILINEAR_GRID"), std::string::npos);
    EXPECT_NE(text.find("DIMENSIONS 4 2 2"), std::string::npos);
    EXPECT_NE(text.find("CELL_DATA 3"), std::string::npos);
    EXPECT_NE(text.find("SCALARS phi double 1"), std::string::npos);
    EXPECT_NE(text.find("SCALARS phi_ant double 1"), std::string::npos);
    EXPECT_NE(text.find("SCALARS phi_iter double 1"), std::string::npos);
    EXPECT_NE(text.find("SCALARS phi_analit double 1"), std::string::npos);
}

}  // namespace fvm
