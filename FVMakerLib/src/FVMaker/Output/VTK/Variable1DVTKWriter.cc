// ----------------------------------------------------------------------------
// File: Variable1DVTKWriter.cc
// Project: FVMaker
// Description: Implements a legacy VTK writer for 1D variables.
// ----------------------------------------------------------------------------

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
