#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;
#include <vector.h>
#include <matrix.h>

using namespace SPUC;
int main(int argv, char* argc[]) {

	Matrix<double> P;
	Vector<double> k;
	Vector<double> w, u, ut, x;
	double ialpha=0;
	Matrix<double> d;
	double y=0;

    double e = y-dot(w,u);
    w = w + e*k;
    d = k*(ut*P);
    P = ialpha*(P - d);
	return(1);
}
