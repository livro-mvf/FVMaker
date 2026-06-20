// Exercicio 7.1 - convergencia da regra do trapezio composta.
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <numbers>
template<class F> double trapezio(F f,double a,double b,std::size_t n){const double h=(b-a)/n;double s=0.5*(f(a)+f(b));for(std::size_t i=1;i<n;++i)s+=f(a+h*i);return h*s;}
int main(){const auto f=[](double x){return std::cos(std::numbers::pi*x/2.0);};constexpr double exata=2.0/std::numbers::pi;std::ofstream csv{"exercicio_71_convergencia.csv"};csv<<"N,dx,integral,erro,ordem\n";std::cout<<std::scientific<<std::setprecision(10)<<"N              dx        integral            erro      ordem\n";double anterior{};for(std::size_t n=4;n<=(std::size_t{1}<<22);n*=2){const double dx=1.0/n;const double valor=trapezio(f,0.0,1.0,n);const double erro=std::abs(valor-exata);const double p=erro>0&&anterior>0?std::log(anterior/erro)/std::log(2.0):0.0;std::cout<<std::setw(8)<<n<<std::setw(18)<<dx<<std::setw(18)<<valor<<std::setw(18)<<erro<<"  "<<std::setw(14)<<p<<'\n';csv<<n<<','<<dx<<','<<valor<<','<<erro<<','<<p<<'\n';anterior=erro;}std::cout<<"\nDados log-log: exercicio_71_convergencia.csv\n";}
