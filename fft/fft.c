#include "fft.h"
//funçaõ implementada por Determinant https://github.com/Determinant
//https://gist.github.com/Determinant/db7889995f08fe982418
#define comp_mul_self(c, c2) \
do { \
    double ca = c->a; \
    c->a = ca * c2->a - c->b * c2->b; \
    c->b = c->b * c2->a + ca * c2->b; \
} while (0)
Comp comp_euler(double x) {
    Comp res;
    res.a = cos(x);
    res.b = sin(x);
    return res;
}
void fft(const Comp *sig, Comp *f, int s, int n, int inv) { //sig = input signal, f = output frequency, s = ? , n =length, inv = bool inverse
    int i, hn = n >> 1;//n>>1 = n/2
    Comp ep = comp_euler((inv ? PI : -PI) / (double)hn), ei;
    Comp *pi = &ei, *pp = &ep;
    if (!hn) *f = *sig;
    else
    {
        fft(sig, f, s << 1, hn, inv);
        fft(sig + s, f + hn, s << 1, hn, inv); //s<<1 = s*2
        pi->a = 1;
        pi->b = 0;
        for (i = 0; i < hn; i++)
        {
            Comp even = f[i], *pe = f + i, *po = pe + hn;
            comp_mul_self(po, pi);
            pe->a += po->a;
            pe->b += po->b;
            po->a = even.a - po->a;
            po->b = even.b - po->b;
            comp_mul_self(pi, pp);
        }
    }
}