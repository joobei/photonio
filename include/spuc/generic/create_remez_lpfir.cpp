#include <math.h>
#include <spuc.h>
#include <complex.h>
#include <fir.h>
#include <remez_fir.h>
namespace SPUC {
//! \brief calculates the coefficients for lowpass FIR based on Remez constraints
//! \ingroup fir
void create_remez_lpfir(fir<double>& remezfir, double* edge, double* fx, double* wtx) {
  
  long nfilt = remezfir.num_taps;
  remez_fir Remz;
  double* fir_coef;
  fir_coef = Remz.remez(nfilt,2,edge,fx,wtx,1);
  for (int i=0;i<nfilt;i++) remezfir.settap(i,fir_coef[i]);
}
}
