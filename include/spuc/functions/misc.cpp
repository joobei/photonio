// 
#include <stdlib.h>
#include <math.h>
// Exclusive or the bits in x together. 
// N is the number of bits in x.
namespace SPUC {
bool reduce(long x, long n)
{
	bool c=0;
	for (int i =0;i<n;i++) {
		c ^= (x & 0x01);
		x >>= 1;
	}
	return(c);
}
double erfc1(double x) {
  const double a1 = 0.254829592;
  const double a2 = -0.284496736;
  const double a3 = 1.421413741;
  const double a4 = -1.453152027;
  const double a5 = 1.0601405429;
  const double p = .3275911;       
  double t,rf;
  t = 1./(1.+p*x);
  rf = (a1*t+a2*t*t+a3*t*t*t+a4*t*t*t*t+a5*t*t*t*t*t)*exp(-x*x);
  return(rf); 
}
double erf1(double x) { return(1-erfc1(x)); }
long round(long in, long bits)
{
  double scale = 1.0/(double)(1 << bits);
  return((long)floor((double)(scale*in)+0.5));
}
long saturate(long in, long bits)
{
  long low_mask = ((1<<(bits-1)) - 1);
  if (labs(in) > low_mask) return((in>0) ? low_mask : ~low_mask);
  else return(in);
}  	
/* +++Date last modified: 05-Jul-1997 */
/*
** quicksort.c -- quicksort integer array
** public domain by Raymond Gardner     12/91
*/
void swap(int *a, int *b)
{
      register int t;
      t = *a;
      *a = *b;
      *b = t;
}

void quicksort(int* v, unsigned n)
{
  unsigned i, j, ln, rn;
  while (n > 1) {
	swap(&v[0], &v[n/2]);
	for (i = 0, j = n; ; ) {
	  do
		--j;
	  while (v[j] > v[0]);
	  do
		++i;
	  while (i < j && v[i] < v[0]);
	  if (i >= j) break;
	  swap(&v[i], &v[j]);
	}
	swap(&v[j], &v[0]);
	ln = j;
	rn = n - ++j;
	if (ln < rn) {
	  quicksort(v, ln);
	  v += j;
	  n = rn;
	} else {
	  quicksort(v + j, rn);
	  n = ln;
	}
  }
}

} // namespace SPUC 
