/* The Newton Raphson method for finding a root of an equation.
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* Define your guess, convergence criterion */


/* Declare your functions. f will evaluate f(x) while fprime will evaluate df/dx(x). newton will perform the iterative loop to estimate next value for x all the while comparing f(x) with the convergence criterion eps*/

double f(double x);

double fprime(double x);

double newtonraph(double x, double accuracy);

int
main(int argc, char *argv[]) {
double x = atof(argv[1]);
	x = newtonraph(x, pow(10,-4));
	
	printf("(main) x = %.10f, f(x) = %12.10f\n", x, f(x));
	return 0;
}

/* Define your functions*/
double f(double x){
	return x*x*x*x + 30*exp(-x/4) - 60;
}
double fprime(double x) {
	return 4*x*x*x - (30/4)*exp(-x/4);
}

double newtonraph(double x, double accuracy) {
	if(sqrt(f(x)*f(x))<accuracy){
		return x;	
	} else {
		//if(sqrt(newtonraph(x - (f(x)/fprime(x)), accuracy)*
			//newtonraph(x - (f(x)/fprime(x)), accuracy)) >= 
		return newtonraph(x - (f(x)/fprime(x)), accuracy);
	}
}

