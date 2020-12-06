/* The bisection method for finding a root of an equation.
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* Define your lower and upper bounds for bracket */

/* Define your convergence criterion */

/* Declare your functions. f will evaulate the function f(x) for supplied x while bisection will obtain mean and compare which half bracket to keep and which to discard.*/
double f(double x);

double bisection(double a, double b, double accuracy);

int
main(int argc, char *argv[]) {
	double x;
	
	double accuracy = pow(10,-8);
	x = bisection(2, 3, accuracy);
	
	printf("(main) x = %.10f, f(x) = %12.10f\n", x, f(x));
	return 0;
}

/* Define your functions here */
double
f(double x) {
	return x*x*x*x + 30*exp(-x/4) - 60;
}

int whichsideisclosertozero(double a, double b){
	if(sqrt(f(a)*f(a)) <= sqrt(f(b)*f(b))){ //Left is closer to 0
		return 1;	
	} else {
		return 0;
	}	
}

double
bisection(double a, double b, double accuracy) {
	double x = (a+b)/2;
	printf("%lf \n", x);
	double temp = f(x);
	
	if(sqrt(temp*temp) < accuracy){
		return x;	
	} else {
			if(whichsideisclosertozero((a+x)/2, (x+b)/2)){
				return bisection(a, x, accuracy);	
			} else {
				return bisection(x, b, accuracy);	
			}
	}
}

