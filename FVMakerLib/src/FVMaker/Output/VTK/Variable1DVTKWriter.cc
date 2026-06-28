// ============================================================================
// Arquivo: Variable1DVTKWriter.cc
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Implementa Variable 1 DVTKWriter no contexto de Output / VTK.
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

#include <fstream>
#include <ostream>

#include <FVMaker/ErrorHandling/ErrorCatalog.h>
#include <FVMaker/ErrorHandling/ThrowError.h>
#include <FVMaker/Output/VTK/Variable1DVTKWriter.h>

namespace fvm {

namespace {

void write_scalar(
    std::ostream& output,
    std::string_view name,
    const DenseVector& values
) {
    output << "SCALARS " << name << " double 1\n";
    output << "LOOKUP_TABLE default\n";

    for (Real value : values.values()) {
        output << value << '\n';
    }
}

}  // namespace

void Variable1DVTKWriter::write(
    const Variable1D& variable,
    std::ostream& output
) {
    require(
        output.good(),
        error_catalog::kInvalidArgument,
        Variable1DVTKWriter::id()
    );

    const GridView1D& grid = variable.grid();

    output << "# vtk DataFile Version 3.0\n";
    output << "FVMaker Variable1D: " << variable.name() << '\n';
    output << "ASCII\n";
    output << "DATASET RECTILINEAR_GRID\n";
    output << "DIMENSIONS " << grid.num_faces() << " 2 2\n";

    output << "X_COORDINATES " << grid.num_faces() << " double\n";
    for (Real x : grid.faces()) {
        output << x << '\n';
    }

    output << "Y_COORDINATES 2 double\n";
    output << "0\n1\n";
    output << "Z_COORDINATES 2 double\n";
    output << "0\n1\n";

    output << "CELL_DATA " << grid.num_volumes() << '\n';
    write_scalar(output, "phi", variable.phi());
    write_scalar(output, "phi_ant", variable.phi_ant());
    write_scalar(output, "phi_iter", variable.phi_iter());
    write_scalar(output, "phi_analit", variable.phi_analit());
}

void Variable1DVTKWriter::write(
    const Variable1D& variable,
    const std::filesystem::path& filepath
) {
    std::ofstream output{filepath};
    require(
        output.good(),
        error_catalog::kInvalidArgument,
        Variable1DVTKWriter::id()
    );

    write(variable, output);
}

}  // namespace fvm
