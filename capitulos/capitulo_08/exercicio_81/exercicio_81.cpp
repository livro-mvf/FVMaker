// Exercicio 8.1 - coeficientes nas faces.
#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>
#include <FVGridMaker/OneDimensional/Distribution1D/Custom1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/Coordinates1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/VolumeCentered1D.h>
#include <FVMaker/OneDimensional/Coefficient/DiffusionCoefficient1D.h>
int main(){
 const auto axis=fvgrid::Custom1D::make(fvgrid::Coordinates1D::faces({0.0,0.2,1.0}),fvgrid::VolumeCentered1D{});const fvm::GridView1D grid{axis};const fvm::DenseVector gamma{{1.0,10.0}};
 const auto arit=fvm::arithmetic_field_coefficient_1d(grid,gamma);const auto harm=fvm::harmonic_field_coefficient_1d(grid,gamma);
 const auto inventada=fvm::interpolated_field_coefficient_1d(grid,gamma,[](fvm::FaceInterpolationData1D d){return std::sqrt(d.west_value*d.east_value);});
 std::cout<<std::fixed<<std::setprecision(10)<<"Centros: "<<grid.centers()[0]<<", "<<grid.centers()[1]<<"  face interna: "<<grid.faces()[1]<<"\n";
 std::cout<<"face        aritmetica       harmonica       geometrica(usuario)\n";
 for(fvm::Size f=0;f<grid.num_faces();++f)std::cout<<std::setw(4)<<f<<std::setw(18)<<arit.face_value(f)<<std::setw(18)<<harm.face_value(f)<<std::setw(24)<<inventada.face_value(f)<<'\n';
}
