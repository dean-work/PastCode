/***************************************************************************
 *
 *   File        : tasks.c
 *   Student Id  : 913634
 *   Name        : Dean Raguso	
 *
 ***************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <string.h>
#include "tasks.h"

//Constants
#define 	Q2A_OUTPUT			"2.3.csv"
#define		Q2B_OUTPUT			"out_shock.csv"
#define 	Q4_OUTPUT			"out_linalsys.csv"
#define 	Q5_OUTPUT			"out_interp.csv" 	

#define 	MEM_SIZE_GUESS		6
#define 	MEM_SIZE_INC		2
#define 	GAMMA				1.4
#define 	PI					acos(-1.0)
#define 	ACCURACY			pow(10,-8)          
#define 	THETA_DIV			0.5
#define 	NINETY_DEGREES		90
#define 	ZERO_DEGREES		0
#define 	ITERATION_THRESH 	1000000
#define 	MAX_GRAD			10000
#define 	MIN_GRAD			pow(10,-7)
#define 	MAX_F				10000
#define 	MIN_NUDGE			pow(10,-2)
                                                                  
#define 	FOUR_COLS			4

#define 	MEM1_SIZE_GUESS		50
#define 	MEM1_SIZE_INC		10

#define 	TIMELINE			0.2
#define 	TIME_INCREMENT		0.05

#define 	CENTRAL_SCHEME		1
#define 	FORWARD_SCHEME		0

#define 	ZERO				0.0			

//Type defs
typedef struct match_s{
	double 	value;
	int		status, end; //0 for none found, 1 for 1 found, -1 for reached end.
}match_t;

//Function Prototypes
double f(double B, double theta, double M, double gamma);
double fprime(double B, double theta, double M, double gamma);
double rads(double degrees);
double degs(double radians);
double nrhap(double B, double theta, double M, double gamma, double accuracy);
double sign(double x);
double left(double theta, double M);
double cot(double angle);
match_t cubic_spline(double *x, int start, int x_range, double value,
	double* A, double*B, double*C, double *D);
int is_between(double a, double b, double value);
void RK2(double*fn, double*fn_05, double*fn_1, double timestep, double c,
	int Nx, int state);
void wave_print(FILE* fp, double*f, int size);
void first_order_diff(double *dfndx, double *fn, int size, double step);
void second_order_diff(double *dfndx, double *fn, int size, double step);
void output_wave(FILE* fp, double*fn, double xstep, int size);
int whatever_is_bigger(int a, int b);

//----------------------------------------------------------------------------

void shockwave(const char* q2_file)
{
	//Extract the data.

		//Open File
		FILE* fp;
		fp = fopen(q2_file, "r");
		
		//Create data structures
		double Mach_a, theta, beta_l, beta_u, gamma, Mach_b;
		int Mach_c_size = 0, Mach_c_mem = MEM_SIZE_GUESS;
		double* Mach_c = (double*)malloc(sizeof(double)*Mach_c_mem);
		
		//Extract the data
		while(fgetc(fp) != '\n'){} //Cycle through sub-headings.
		fscanf(fp,"%lf,%lf,%lf,%lf,%lf\n",&Mach_a,&theta,&beta_l,&beta_u,
			&gamma);    
		fscanf(fp,"M\n%lf\nM\n", &Mach_b);	
		while(fscanf(fp, "%lf\n", &Mach_c[Mach_c_size])==1){
			Mach_c_size++;
			if(Mach_c_size == Mach_c_mem){ //If I've reached the memories end.
				Mach_c_mem += MEM_SIZE_INC;
				Mach_c = (double*)realloc(Mach_c, Mach_c_mem*sizeof(double));
			}
		}
	
	//Call function and print outputs, 2.3 a).
	double Bu=1, Bl=1;
	nrhap(rads(beta_l),rads(theta),Mach_a,GAMMA,ACCURACY);
	nrhap(rads(beta_u),rads(theta),Mach_a,GAMMA,ACCURACY);
	
	//PRINT TO OUTPUTS HERE
	
	//Call function and print outputs 2.3 b).
	
		//Create Output file
		FILE* fp1, *fp2;
		fp1 = fopen(Q2A_OUTPUT,"w");
		fp2 = fopen(Q2B_OUTPUT,"w");
	
	int i;
	double lguess;
	fprintf(fp1, "theta, Bl, Bu\n");
	
	for(i=0; (i<=NINETY_DEGREES)&&(Bl); i++){ 
		lguess = left(rads(i),Mach_b);
		Bl = nrhap(lguess,rads(i),Mach_b,GAMMA,ACCURACY);
		Bu = nrhap(rads(NINETY_DEGREES),rads(i),Mach_b,GAMMA,ACCURACY);
		
		fprintf(fp1, "%d,%0.6lf,%0.6lf\n", i, degs(Bl), degs(Bu));
		fflush(fp1);
	}
	
	//2.3 c)
	
		//Sub-headings
		fprintf(fp2,"M,theta,beta_lower,beta_upper\n");
		
		//Loop for all M values requested
		for(int m=0; m<Mach_c_size; m++){
			
			//Loop through every theta, finding the Bu Bl values
			for(i=0; (i<=NINETY_DEGREES); i++){ 

				lguess = left(rads(i),Mach_b);
				Bl = nrhap(lguess,rads(i),Mach_c[m],GAMMA,ACCURACY);
				Bu = nrhap(rads(NINETY_DEGREES),rads(i),Mach_c[m],
					GAMMA,ACCURACY);
				
				if((!Bl) || (!Bu)){
					break;	
				}
				
				fprintf(fp2, "%0.6lf,%1d,%0.6lf,%0.6lf\n",
					Mach_c[m], i, degs(Bl), degs(Bu));
	}
			
			
		}
	
	//Free and close everything.
	fclose(fp);                                                  
	fclose(fp1);
	fclose(fp2);
	free(Mach_c);
}

void linalgbsys(const char* q4_file)
{                                                       
    //Extract Data and create output file
    
    	//Create files
    	FILE *fp, *fp1;
    	fp = fopen(q4_file,"r");
    	fp1 = fopen(Q4_OUTPUT,"w");
    	
    	//Create data structures
    	int data_size = 0, data_mem = MEM_SIZE_GUESS;    
    	double *a, *b, *c, *Q, *x;
    	a = (double*)malloc(sizeof(double)*data_mem);
    	b = (double*)malloc(sizeof(double)*data_mem);
    	c = (double*)malloc(sizeof(double)*data_mem);
    	Q = (double*)malloc(sizeof(double)*data_mem);
    	x = (double*)malloc(sizeof(double)*data_mem);
    	
    	while(fgetc(fp) != '\n'){} //Cycle through subheadings
    	
    	//Extract the data
    	while(fscanf(fp,"%lf,%lf,%lf,%lf\n",&a[data_size],&b[data_size],
    		&c[data_size],&Q[data_size]) == FOUR_COLS){
    		
			data_size++;
			
			if(data_size == data_mem){ //If the data size might exceed mem.
				data_mem += MEM_SIZE_INC;
				a = (double*)realloc(a,sizeof(double)*data_mem);
				b = (double*)realloc(b,sizeof(double)*data_mem);
				c = (double*)realloc(c,sizeof(double)*data_mem);
				Q = (double*)realloc(Q,sizeof(double)*data_mem);
				x = (double*)realloc(x,sizeof(double)*data_mem);
			}
    	}
    	
   /* for(int i = 0; i<data_size; i++){
    	printf("%06lf,%06lf,%06lf,%06lf\n", a[i], b[i], c[i], Q[i]);	
    }	*/
    
	//Modify a and Q, as though guass elimination has occurred
	for(int i = 1; i<data_size; i++){
		a[i] = a[i] - (c[i]*b[i-1])/a[i-1];
		Q[i] = Q[i] - (c[i]*Q[i-1])/a[i-1];
	}
    
    //Obtain the solutions and place them into the x vector
	x[data_size-1] = Q[data_size-1]/a[data_size-1];
    for(int i = data_size-2; i>=0; i--){
    	x[i] = (Q[i] - b[i]*x[i+1]) / a[i]; 	
    }
    
    //Output the data
    for(int i = 0; i<data_size; i++){
    	fprintf(fp1,"%06lf\n", x[i]);	
    }
    
    //Close files and free memory
    fclose(fp);
    fclose(fp1);
    free(a);
    free(b);
    free(c);
    free(Q);
    free(x);
}

void interp(const char* q5_file, const double xo)
{
    //Create files
    FILE *fp, *fp1;
    fp = fopen(q5_file,"r");
    fp1 = fopen(Q5_OUTPUT,"w");
    
    //Create data structures for the input
    int data_size = 0, data_mem = MEM1_SIZE_GUESS;
    double *x, *f;
    x = (double*)malloc(sizeof(double)*data_mem);
    f = (double*)malloc(sizeof(double)*data_mem);
    
    //Extract data
    while(fgetc(fp) != '\n');
    while(fscanf(fp,"%lf,%lf\n", &x[data_size], &f[data_size]) == 2){
    	
    	data_size++;
    	
    	if(data_size == data_mem){
    		data_mem += MEM1_SIZE_INC;
    		x = (double*)realloc(x,sizeof(double)*data_mem);
    		f = (double*)realloc(f,sizeof(double)*data_mem);
    	}
    }
    
    //Solve tri-diagonal matrix for c coefficients.
    
    	//Create data structures for splines
    	//There is only data_size-1 vector cells needed.
    	//Captials are spline coeff, lower case is for tri-diagonal matrix.
    	double *A, *B, *C, *D, *h, *Q, *a, *b, *c;
    	
    	A = (double*)calloc(data_size,sizeof(double));
    	B = (double*)calloc(data_size,sizeof(double));
    	D = (double*)calloc(data_size,sizeof(double));
    	h = (double*)calloc(data_size,sizeof(double));
    	
    	//C requires arrays for diagonal stuff
    	C = (double*)calloc(data_size,sizeof(double));
    	Q = (double*)calloc(data_size,sizeof(double));
    	a = (double*)calloc(data_size,sizeof(double));
    	b = (double*)calloc(data_size,sizeof(double));
    	c = (double*)calloc(data_size,sizeof(double));
    	   	
    	//Populate the h arrray
    	for(int i = 0; i<data_size-1; i++){
    		h[i] = x[i+1]-x[i];	
    	}
    	
    	//Populate the A coefficients
    	for(int i = 0; i<data_size; i++){
    		A[i] = f[i];
    	}
    	
    	//Populate the C coefficients---------------------------
    	
    	//Create the Q coefficients
    	Q[0] = 0; 
    	Q[data_size-1] = 0;
    	for(int i = 1; i < data_size-1; i++){
    		Q[i] = (3.0/h[i])*(A[i+1] - A[i]) + (3.0/h[i-1])*(A[i-1] - A[i]);
    	}
    	
    	//STEP 1 Create the diagonal vectors
    	a[0] = 1; 
    	a[data_size-2] = 2*(h[data_size-3] + h[data_size-2]);
    	a[data_size-1]= 1;
    	c[0] = h[0]; 
    	c[data_size-2] = 0; 
    	b[0] = 0; 
    	b[data_size-2] = h[data_size-2];
    	for(int i = 1; i < data_size-2; i++){
    		a[i] = 	2*(h[i-1] + h[i]);
    		b[i] = h[i];
    		c[i] = h[i];
    	}
    	
    	//STEP 2 for C coeff.
    	for(int i = 1; i<data_size; i++){
			a[i] = a[i] - (c[i]*b[i-1])/a[i-1];
			Q[i] = Q[i] - (c[i]*Q[i-1])/a[i-1];
		}
    
   		//STEP 3 for C coeff.
		C[data_size-2] = Q[data_size-2]/a[data_size-2]; //Something's occurring here
   		for(int i = data_size-3; i>= 0; i--){
    		C[i] = (Q[i] - b[i]*C[i+1])/a[i]; 	
    	}
    	
    	//Populate the B and D coefficients
    	for(int i = 0; i<data_size-1; i++){
    		B[i] = (1.0/h[i])*(A[i+1] - A[i]) - (h[i]/3.0)*(2.0*C[i] + C[i+1]);
    		D[i] = (C[i+1] - C[i])/(3.0*h[i]);
    	}

    //Get the values for x0 and output them, there may be many.
    match_t output = {0};
    int start = 0;
    
    while((output = cubic_spline(x, start, data_size, xo,
	A, B, C, D)).status == 1){
		
		fprintf(fp1,"%lf,%lf\n",xo,output.value);
		start = output.end;
	}
    
	
	
    //Free and close everything
    fclose(fp);
    fclose(fp1);
    free(x);
    free(f);
    free(A);
    free(B);
    free(C);
    free(D);
    free(h);
    free(Q);
    free(a);
    free(b);
    free(c);
}

void waveeqn(const char* q6_file)
{
	//Open files
	FILE *fp, *fp1, *fp_forward, *fp_central;
	fp = fopen(q6_file,"r");
	fp1 = fopen("out_waveeqn.csv","w");
	fp_forward = fopen("out_waveeqn_1U.csv", "w");
	fp_central = fopen("out_waveeqn_2C.csv", "w");
	
	//These files are fo the time-levels output.
	FILE *fp005, *fp01, *fp015, *fp02, *fp005b, *fp01b, *fp015b, *fp02b;
	fp005 = fopen("f005.csv", "w");
	fp01 = fopen("f01.csv", "w");
	fp015 = fopen("f015.csv", "w");
	fp02 = fopen("f02.csv", "w");
	fp005b = fopen("f005b.csv", "w");
	fp01b = fopen("f01b.csv", "w");
	fp015b = fopen("f015b.csv", "w");
	fp02b = fopen("f02b.csv", "w");
	
	fprintf(fp005, "x,f\n");
	fprintf(fp01, "x,f\n");
	fprintf(fp015, "x,f\n");
	fprintf(fp02, "x,f\n");
	fprintf(fp005b, "x,f\n");
	fprintf(fp01b, "x,f\n");
	fprintf(fp015b, "x,f\n");
	fprintf(fp02b, "x,f\n");
	
	
	//Extract the values from the file.
	double c, CFL, out_iter;
	int Nx;
	while(fgetc(fp) != '\n'){}
	fscanf(fp, "%lf,%d,%lf,%lf", &c, &Nx, &CFL, &out_iter);
	int Size = Nx + 1;
	double xstep = ((double)1/Nx);
	
	//Create the required data structures
	double *fn, *fn_1, *fn_05;
	fn = (double*)calloc(Size,sizeof(double));
	fn_1 = (double*)calloc(Size,sizeof(double));
	fn_05 = (double*)calloc(Size,sizeof(double));
	
	//Write in IC - For the forward scheme
	for(int i = 0; i<Size; i++){
		if((xstep)*(double)i < 0.125){
			fn[i] = 0;	
		} else if ((0.125 <= (xstep)*(double)i) && ( 0.375 >= xstep*
			(double)i)) {
			fn[i] = 0.5*(1 - cos(8*PI*(xstep*(double)i - 0.125)));
		} else {
			fn[i] = 0;	
		}
	}
	
	//Prints x axis for the sake of graphing, not part of assingment.
	double timestep = xstep*CFL/c;
	double time_iterations = whatever_is_bigger(out_iter,(int)(1.0/timestep));
	
	for(int i = 0; i<Size; i++){
		fprintf(fp1, "%lf,", (double)i*xstep);	
	}
	fprintf(fp1,"\n");
	
	int s1=1, s2=1, s3=1, s4=1, s5 = 1, s6 = 1, s7=1, s8=1;
	
	//Loop for each timestep, and output for the forward scheme.
	for(int j = 0; j <= time_iterations; j++){
		
		if(((j*timestep - 0.05) > 0) && s1){
			s1=0;
			for(int i = 0; i< Nx + 1; i++){
				fprintf(fp005, "%0.6lf,%0.6lf\n", xstep*(double)i, fn[i]); 	
			}
		} else if(((j*timestep - 0.1) > 0) && s2){
			s2=0;
			for(int i = 0; i< Nx + 1; i++){
				fprintf(fp01, "%0.6lf,%0.6lf\n", xstep*(double)i, fn[i]); 	
			}
		} else if(((j*timestep - 0.15) > 0) && s3){
			s3=0;
			for(int i = 0; i< Nx + 1; i++){
				fprintf(fp015, "%0.6lf,%0.6lf\n", xstep*(double)i, fn[i]); 	
			}
		} else if(((j*timestep - 0.2) > 0) && s4){
			s4=0;
			for(int i = 0; i< Nx + 1; i++){
				fprintf(fp02, "%0.6lf,%0.6lf\n", xstep*(double)i, fn[i]); 	
			}
		}
		
		//Printing the kth iteration.
		if(j == out_iter){
			output_wave(fp_forward, fn, xstep, Nx+1);	
		}
		
		//RK2 Routine
		RK2(fn, fn_05, fn_1, timestep, c, Nx, FORWARD_SCHEME);
				
		//Copy fn_1 back to fn
		for(int i = 0; i< Nx + 1; i++){
			fn[i] = fn_1[i];	
		}
	}
	
	//Write in IC - For the central scheme
	for(int i = 0; i<Size; i++){
		if((xstep)*(double)i < 0.125){
			fn[i] = 0;	
		} else if ((0.125 <= (xstep)*(double)i) && ( 0.375 >= xstep*
			(double)i)) {
			fn[i] = 0.5*(1 - cos(8*PI*(xstep*(double)i - 0.125)));
		} else {
			fn[i] = 0;	
		}
	}
	
	//Loop for each timestep, and output for the central scheme.
	for(int j = 0; j <= time_iterations; j++){
		
		if(((j*timestep - 0.05) > 0) && s5){
			s1=0;
			for(int i = 0; i< Nx + 1; i++){
				fprintf(fp005b, "%0.6lf,%0.6lf\n", xstep*(double)i, fn[i]); 	
			}
		} else if(((j*timestep - 0.1) > 0) && s6){
			s2=0;
			for(int i = 0; i< Nx + 1; i++){
				fprintf(fp01b, "%0.6lf,%0.6lf\n", xstep*(double)i, fn[i]); 	
			}
		} else if(((j*timestep - 0.15) > 0) && s7){
			s3=0;
			for(int i = 0; i< Nx + 1; i++){
				fprintf(fp015b, "%0.6lf,%0.6lf\n", xstep*(double)i, fn[i]); 	
			}
		} else if(((j*timestep - 0.2) > 0) && s8){
			s4=0;
			for(int i = 0; i< Nx + 1; i++){
				fprintf(fp02b, "%0.6lf,%0.6lf\n", xstep*(double)i, fn[i]); 	
			}
		}
		
		//Printing the kth iteration.
		if(j == out_iter){
			output_wave(fp_central, fn, xstep, Nx+1);	
		}
		
		//RK2 Routine
		RK2(fn, fn_05, fn_1, timestep, c, Nx, CENTRAL_SCHEME);
				
		//Copy fn_1 back to fn
		for(int i = 0; i< Nx + 1; i++){
			fn[i] = fn_1[i];	
		}
	}
	
	
	
	//Close and Free everything
	fclose(fp);
	fclose(fp1);
	fclose(fp_forward);
	fclose(fp_central);
	
	free(fn);
	free(fn_1);
	free(fn_05);
	
	fclose(fp005);
	fclose(fp01);
	fclose(fp015);
	fclose(fp02);
	fclose(fp005b);
	fclose(fp01b);
	fclose(fp015b);
	fclose(fp02b);
}

//---------------------Functions--------------------------------------------
//f(B) function, (2). Generalized as f(B, theta, M,gamma).
double f(double B, double theta, double M, double gamma){
	double sum1, sum2;
	sum1 = 2*(1/tan(B))*(M*M*sin(B)*sin(B) - 1)/(M*M*(gamma + cos(2*B)) + 2);
	sum2 = -tan(theta);
	
	return sum1 + sum2;
}

//fprime function.
double fprime(double B, double theta, double M, double gamma){

	return (4*pow(M,2)*cos(B)*cot(B)*sin(B))/((gamma + cos(2*B))*pow(M,2) + 2) 
		-(2*(pow(cot(B),2) + 1)*(pow(M,2)*pow(sin(B),2) - 1))/
		((gamma + cos(2*B))*pow(M,2) + 2) + (4*pow(M,2)*sin(2*B)*cot(B)*
		(pow(M,2)*pow(sin(B),2) - 1))/
		pow(pow(M,2)*(gamma + cos(2*B)) + 2,2);
}

//Newton Rhapson Function
double nrhap(double B, double theta, double M, double gamma, double accuracy){
	//Assumes inputs are in radians.
	
	double out = B;
	double tmp_prime, tmp;
	int count = 0;
	
	while((sqrt(f(out,theta,M,gamma)*f(out,theta,M,gamma)) > accuracy) &&
		(count < ITERATION_THRESH)){
		count++;
		
		//The current point.
		tmp = f(out,theta,M,gamma); //tmp f value
		tmp_prime = fprime(out,theta,M,gamma);
		
		//The next point.
		out = out - (tmp/tmp_prime); //new_out
		
		//printf("%lf - %0.6lf - %lf\n", out, tmp, tmp_prime);
	      
		if((out<0)||(out>PI/2)){ //If this occurs, no solution.
			out = 0;
			break;
		}
	}
	
	if(count == ITERATION_THRESH){
		return 0;	//beta should never be 0.
	}
	
	return out;
}
                                                          
//Degrees to Radians
double rads(double degrees){
	return PI*degrees/180.0;	                                      
}

//Radians to Degrees
double degs(double radians){
	return radians*180.0/PI;	
}

//Returns the sign of a double
double sign(double x){
	if(x > 0){
		return 1.0;	
	} else {
		return -1.0;	
	}
}

//Left guess for newton rhapson, takes radians returns radians.
double left(double theta, double M){
	if(theta<rads(50)){
		return theta + asin(1/M);	
	} else {
		return theta;
	}
}

//Cotangent function
double cot(double angle){
	return cos(angle)/sin(angle);	
}

//Cubic Spline Interpretter
//Climbs up function from left to right, stops at first x range match.
match_t cubic_spline(double *x, int start, int x_range, double value,
	double* A, double*B, double*C, double *D){
	
	match_t match;
	match.value = 0;
	match.status = 0;
	match.end = start; //This is the last point the function got to.

	for(int i = start; i<x_range-1; i++){
		
		if(is_between(x[i],x[i+1],value)){
			
			//printf("yes\n");
			match.end = i+1;
			match.status = 1; //1 found.
			
			printf("%lf %lf %lf %lf %lf\n", A[i], B[i], C[i], D[i], value); 
			double tmp = value - x[i];
			match.value = A[i] + B[i]*tmp + C[i]*tmp*tmp + 
				D[i]*tmp*tmp*tmp;
				
			break;
		}
		
		//printf("no\n");
		
		match.end = i;
		match.status = -1; //Reached end.
	}
	
	return match;
}

int is_between(double a, double b, double value){
	if(	(value >= a) && (value < b)){
		return 1;	
	} else if ((value >= b) && (value < a)) {
		return 1;	
	} else {
		return 0;	
	}
}	

void RK2(double*fn, double*fn_05, double*fn_1, double timestep, double c,
	int Nx, int state){
	//State is the option to use the central scheme or the forward scheme.
	
	//Create variables and data structures. 
	double xstep = (double)1/Nx;
	double *dfndx, *dfn_05dx;
	int size = Nx+1; //size of the arrays required.
	
	dfndx = (double*)malloc(sizeof(double)*(size));	
	dfn_05dx = (double*)malloc(sizeof(double)*(size));	
	
	//Calculate the Spatial derrivative of fn.
	if(state){
		second_order_diff(dfndx, fn, size, xstep);
	} else {
		first_order_diff(dfndx, fn, size, xstep);
	}
	
	//Calculate fn_05
	for(int i = 0; i*xstep<=1; i++){
		fn_05[i] = fn[i] - c*timestep*dfndx[i];
	}
	
	//Calculate the Spatial derrivative of fn_05.
	if(state){
		second_order_diff(dfn_05dx, fn_05, size, xstep);
	} else {
		first_order_diff(dfn_05dx, fn_05, size, xstep);
	}
	
	//Calculate fn_1
	for(int i = 0; i*xstep<=1; i++){
		fn_1[i] = fn[i] - c*0.5*timestep*(dfndx[i] +
			dfn_05dx[i]);
	}
	
	//Free everything                 
	free(dfndx);
	free(dfn_05dx);
	
}

void first_order_diff(double *dfndx, double *fn, int size, double step){
	//Upwind Scheme
	dfndx[0] = (fn[1]-fn[0])/(double)step;
	
	for(int i = 1; i<size; i++){
		dfndx[i] = (fn[i]-fn[i-1])/(double)step;
	}
}

void second_order_diff(double *dfndx, double *fn, int size, double step){
	//Central Scheme
	dfndx[0] = (fn[1]-fn[0])/(double)step;
	dfndx[size-1] = (fn[size-1]-fn[size-2])/(double)step;
	
	for(int i = 1; i<size-1; i++){
		dfndx[i] = (fn[i+1]-fn[i-1])/(2*(double)step);                
	}
}

void wave_print(FILE* fp, double*f, int size){
	fprintf(fp, "%lf", f[0]);	
	for(int i = 1; i<size; i++){
		fprintf(fp, ",%lf", f[i]);	
	}
	fprintf(fp,"\n");	
}

void output_wave(FILE* fp, double*fn, double xstep, int size){
	fprintf(fp,"x,f(x)\n");
	for(int i = 0; i<size; i++){
		fprintf(fp,"%0.6lf,%0.6lf\n", (double)i*xstep, fn[i])	;
	}
}

int whatever_is_bigger(int a, int b){
	if (a>b){
		return a;
	} else {
		return b;	
	}
}