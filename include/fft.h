#include <math.h>
//funçaõ implementada por Determinant https://github.com/Determinant
//https://gist.github.com/Determinant/db7889995f08fe982418
#define SQR(x) ((x) * (x))
typedef struct Comp {
    /* comp of the form: a + bi */
    double a, b;
} Comp;

#define PI 3.141592653589793

Comp comp_euler(double x);
void fft(const Comp *sig, Comp *f, int s, int n, int inv);
