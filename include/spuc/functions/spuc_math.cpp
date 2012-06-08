// 
#include <spuc_math.h>
//#include <math.h>
// Exclusive or the bits in x together. 
// N is the number of bits in x.
namespace SPUC {
double sqrt(double x) {	return(::sqrt(x));}
double cos(double x)  {	return(::cos(x));}
complex<double> sqrt(complex<double> x) {
  double mag = sqrt(sqrt(magsq(x)));
  double ang = 0.5*arg(x); // ambiguity
  return(polar(mag,ang));
}
complex<double> sqrt(complex<long> x) {
  double mag = sqrt(sqrt(magsq(x)));
  double ang = 0.5*arg(x); // ambiguity
  return(polar(mag,ang));
}
// function for hyperbolic inverse cosine of x
double coshin(double x) { return(log(x+sqrt(x*x-1.))); }
// function for inverse cosine of x
double arccos(double x) {
  if (x == 0) return(PI/2.0);
  if (x < 0) return(atan(sqrt(1.-x*x)/x));
  else return(atan(sqrt(1.-x*x)/x) +  PI);
}
// function for hyperbolic cosine of x
double cosh(double x) { return((::exp(x)+::exp(-x))/2.); }
double sinh(double x) { return((::exp(x)-::exp(-x))/2.); }

complex<double> cos(complex<double> x) { 
  double cosh(double x);
  double cos(double x);
  double r = cosh(x.imag())*cos(x.real());
  double i = sinh(x.imag())*sin(x.real());
  return(complex<double>(r,i));
}
} // namespace SPUC 
