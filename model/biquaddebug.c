#include <math.h>
#include <stdio.h>

int main()
{
#define P(x, exp) double x = (exp); printf("%s = %12.6f\n", #x, x) 

	P(Fs, 48000);
	P(f0, 400);
	P(Q, 0.7);
	P(dBgain, 0);
	
	P(A, pow(10, (dBgain/40)));
	P(G, sqrt(dBgain));

	P(w0, 2*M_PI*f0/Fs);

	P(sinw0, sin(w0));
	P(cosw0, cos(w0));

	P(alpha, sinw0 / (2*Q));

	P(b0, (1 - cosw0) / 2);
	P(b1, 1 - cosw0);
	P(b2, (1 - cosw0) / 2);
	P(a0, 1 + alpha);
	P(a1, -2 * cosw0);
	P(a2, 1 - alpha);

	P(x0, b0/a0);
	P(x1, b1/a0);
	P(x2, b2/a0);
	P(y1, -a1/a0);
	P(y2, -a2/a0);
}
