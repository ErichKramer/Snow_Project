#include <fcntl.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>


//const double m_pi  =  3.14159265358979323846;
//use C constant M_PI instead since math.h is included


extern int size;   // matrix dimension; nx and ny
//potentially reduce for performance speedup


const int animate  = 1; 	// animate flag, 1 = animate, 0 = only last frame
//this should be a CL option

const int skiprate = 100;   // 1 = log all frames, log every nth frame
const int debug    = 0;		// whether to dump phi to stdout


//specifies level of detail in output
//double phase_tol = .99;



/*
 //format header for compatibilitiy
	char head[128] = "Source USAVG.0\n"
				"Time 0 ms\nExport Particle_Data\n"
				"Data x y z sxx syy szz sxy sxz syz mat mass\n"
				"Format text\n"
				"EndHeader\n";
//write head, exit if bad


 
 * */



/*Function to log to a file for python reading*/
void log_python(double* data, int fd)
{
    
	char buffer[100];
	const char* bPoint = buffer;

	char* str = malloc(10 * sizeof(char));
	

	for (int y = 0; y < size; y++)
	{
		for (int x = 0; x < size; x++)
		{
            
			//this set of output is used for python drawing, do not modify
			sprintf(str, "%2.4f", data[x+y*size]);//write vals to string
			strcat(str, " ");
			if (write(fd, str, sizeof(char) * strlen(str)) == -1)
			{
				perror("write to file: ");
				exit(EXIT_FAILURE);
			}
			//write string, error handle

		}
	}
	//loop closed writing for python, again do not modify
	sprintf(str, "\n");
	if (write(fd, str, sizeof(char) * strlen(str)) == -1)
    {
		perror("write to file: ");
		exit(EXIT_FAILURE);
    }
	free(str);//fix leak
    
}

void write_file2D(int fd, double* geom, int lsize){

    char buffer[100];
    char* bPoint = buffer;


    for( int y = 0; y < lsize; y++){
        for( int x = 0; x< lsize; x++){
            if(geom[x + y*lsize]){
                     
                sprintf(bPoint, "%f\t%f\t%f\t0\t0\t0\t0\t0\t0\t0\t0\n", 
                            (float)x/10, (float)y/10, geom[x + y*lsize]);

                    if(write(fd, bPoint, sizeof(char) * strlen(bPoint)) ==-1){
                        perror("Write to file: ");
                        exit(EXIT_FAILURE);
                    }

                sprintf(bPoint, "%f\t%f\t%f\t0\t0\t0\t0\t0\t0\t0\t0\n", 
                            (float)x/10, (float)y/10, -geom[x + y*lsize]);

                    if(write(fd, bPoint, sizeof(char) * strlen(bPoint)) ==-1){
                        perror("Write to file: ");
                        exit(EXIT_FAILURE);
                    }
            }

        }

    }

}


void write_file3D(int fd,  double* geom, int lsize ){


    printf("fd inside write3D: %d\n", fd);

    char buffer[100];
    char* bPoint = buffer;

    int x = lsize;
    int y = lsize;
    int z = lsize;

    for(int i = 0; i < z; i++){
        for( int j = 0; j< y; j++){
            for( int k = 0; k < x; k++){
                
                if( geom[lsize*lsize*i + lsize*j + k] == 1){
                    sprintf(bPoint, "%f\t%f\t%f\t0\t0\t0\t0\t0\t0\t0\t0\n", 
                            (float)k/10, (float)j/10, (float)i/10);
                    if(write(fd, bPoint, sizeof(char) * strlen(bPoint)) ==-1){
                        perror("Write to file: ");
                        exit(EXIT_FAILURE);
                    }
                    
                } 
            }
        }
    }
}




double* data_log();

/*int main(){
 
    
    double* s1 = data_log(1);

    free(s1);
    return 0;
}*/




double* gen_crystal(int flag)//log progression for python
{
	double dx 	 = 0.03; //
	double dy 	 = 0.03; //
	double dt 	 = 0.0003; //s
	//.0003 dt is best for speed
	double t_final = 0.3; 		// total simulation time (increments of dt)
  
	double k 		 = 1.6; // Latent heat of fusion
	double tau 	     = 0.0003; 	//phase field relaxation time
	double delta_bar = 0.01; 	// Average thickness of layer (?)

	double mu 		  = 0.05; 	// Strength of anisotropy .02 default
	double anisotropy = 6.0;  		//6.0 default
	double alpha 	  = .9; 		// n-shifting coefficient .9 def
	double gamma 		  = 10.0; 	// 10.0 default
	double TM 		  = 1.0;  	// 1.0 Default Both T0 and TM are used once.
	double T0 		  = 0.0;  	// 0 default
	double theta_0 	  = 1.57; 		// 1.57 default
  
	int r 			    = 3; 	// initial condition radius

	double eccentricity = 1.0; // Playing w/ elliptical initial conditions 1 = circle
		//changing eccentricity slows it in all cases 


	/*
		create many 2D arrays of size-by-size
	*/  
    double t 			   	= 0;
	int arrsize 		   	= size*size;
	double* delta 		   	= malloc(sizeof(double) * arrsize);
	double* delta_dt 	   	= malloc(sizeof(double) * arrsize);
	double* u 			   	= malloc(sizeof(double) * arrsize);
	double* phi 		   	= malloc(sizeof(double) * arrsize);
	double* dphi_dx 	   	= malloc(sizeof(double) * arrsize);
	double* dphi_dy 	   	= malloc(sizeof(double) * arrsize);
	double* lap_u 		   	= malloc(sizeof(double) * arrsize);
	double* lap_phi 	   	= malloc(sizeof(double) * arrsize);
	double* phi_new 	   	= malloc(sizeof(double) * arrsize);
	double* u_new 		   	= malloc(sizeof(double) * arrsize);
	double* phi_grad_angle 	= malloc(sizeof(double) * arrsize);
	double* height		   	= malloc(sizeof(double) * arrsize);
	
	for (int y = 0; y<size; y++)
	{
		for (int x = 0; x<size; x++)
		{
			u[x+y*size] 	   = -1;       // u0 = (T0-TM) / (TM-T0) = -1
			delta[x+y*size]	   = 0;
			delta_dt[x+y*size] = 0;
			phi[x+y*size] 	   = 0;
			dphi_dx[x+y*size]  = 0;
			dphi_dy[x+y*size]  = 0;
			lap_u[x+y*size]    = 0;
			lap_phi[x+y*size]  = 0;
			phi_new[x+y*size]  = 0;
			u_new[x+y*size]	   = 0;
			phi_grad_angle[x+y*size] = 0;


			//set the nuclei
			if (eccentricity*(x-size/2.0)*(x-size/2.0) + (y-size/2.0)*(y-size/2.0) < r*r)
				phi[x+y*size] = 1.0; //fills all but large x and y values with 1.0
      
		}
	}

	//make data.txt and write
	int fd;

    //enclose all 
    if(flag){
        if ((fd = open("data.txt", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IWOTH | S_IROTH)) == -1)
        {
            perror("open failed: ");
            exit(EXIT_FAILURE);
        }

        //first frame no growth
        
        log_python(phi, fd);
    }


	int ym, yp, xm, xp;
	double ddelta2_dx, ddelta2_dy, term1, term2, term3, n;
	for (t=0; t<t_final; t+=dt)
	{
		//set information print for each 100th step
		if ((int)(t/dt) % (int)(t_final/(dt*10)) == 0)
			printf("step %g of %g\n", t/dt, t_final/dt);
		for (int y = 0; y<size; y++)
		{
			for (int x = 0; x<size; x++)
			{
				//periodic boundary condition
				//x plus and x minus, y plus and y minus
				//used in checking the 3x3 quad
				ym = y-1;
				yp = y+1;
				xm = x-1;
				xp = x+1;
	
				if (ym==-1)   ym = size-1;
				if (xm==-1)   xm = size-1;
				if (yp==size) yp = 0;
				if (xp==size) xp = 0;

				//calculate the gradient
				dphi_dx[x+y*size] = (phi[xp+y*size] - phi[xm+y*size]) / dx;
				dphi_dy[x+y*size] = (phi[x+yp*size] - phi[x+ym*size]) / dy;
				
				//laplacian calc
				lap_phi[x+y*size] = ((phi[xm+ym*size]  +
							phi[xm+y*size]   +
							phi[xm+yp*size]  +
							phi[x+ym*size]   +
							phi[x+yp*size]   +
							phi[xp+ym*size]  +
							phi[xp+y*size]   +
							phi[xp+yp*size]) - 8.0*phi[x+y*size]) / (3.0*dx*dy);
	
				lap_u[x+y*size] = ((u[xm+ym*size]  +
							u[xm+y*size]   +
							u[xm+yp*size]  +
							u[x+ym*size]   +
							u[x+yp*size]   +
							u[xp+ym*size]  +
							u[xp+y*size]   +
							u[xp+yp*size]) - 8.0*u[x+y*size]) / (3.0*dx*dx);

				//
				phi_grad_angle[x+y*size] = atan2(dphi_dy[x+y*size], dphi_dx[x+y*size]);
				delta[x+y*size] = delta_bar*(1.0 + mu * cos(anisotropy*(theta_0 - phi_grad_angle[x+y*size])));
				delta_dt[x+y*size] = -delta_bar*anisotropy*mu*sin(anisotropy*phi_grad_angle[x+y*size]);
				ddelta2_dx = (delta[xp+y*size]*delta[xp+y*size] - delta[xm+y*size]*delta[xm+y*size]) / (2*dx);
				ddelta2_dy = (delta[x+yp*size]*delta[x+yp*size] - delta[x+ym*size]*delta[x+ym*size]) / (2*dy);
				term1 = -(delta[xp+y*size]*delta_dt[xp+y*size]*dphi_dy[xp+y*size]
						- delta[xm+y*size]*delta_dt[xm+y*size]*dphi_dy[xm+y*size]) / (2*dx);
				term2 = (delta[x+yp*size]*delta_dt[x+yp*size]*dphi_dx[x+yp*size]
						- delta[x+ym*size]*delta_dt[x+ym*size]*dphi_dx[x+ym*size]) / (2*dy);
				term3 = ddelta2_dx*dphi_dx[x+y*size] + ddelta2_dy*dphi_dy[x+y*size];
				n = alpha/M_PI * atan(-gamma*(TM-T0)*u[x+y*size]);
				phi_new[x+y*size] = phi[x+y*size] + (term1 + term2 + term3 +
									delta[x+y*size]*delta[x+y*size]*lap_phi[x+y*size] +
									phi[x+y*size]*(1.0-phi[x+y*size])*(phi[x+y*size] - 0.5 + n)) * dt / tau;
	
				u_new[x+y*size] = u[x+y*size] + lap_u[x+y*size]*dt + k*(phi_new[x+y*size] - phi[x+y*size]);
			}
		}
    
		for (int y = 0; y<size; y++)
		{
			for (int x = 0; x<size; x++)
			{
				if (debug) printf("%2.4g ", lap_u[x+y*size]*dt);
				phi[x+y*size] = phi_new[x+y*size];
				u[x+y*size] = u_new[x+y*size];
			}
			if (debug) printf("\n");
		}
		if (debug) printf("\n");

		if (flag && (int)(t/dt) % skiprate == 0){ 
			log_python(phi, fd);
		}

	}
    if (flag){
	    log_python(phi, fd);
    }
	close(fd);
	free(delta);
	free(delta_dt);
	free(u);

//final phi exists last here

	free(dphi_dx);
	free(dphi_dy);
	free(lap_u);
	free(lap_phi);
	free(phi_new);
	free(u_new);
	free(phi_grad_angle);
	free(height);

    printf("Center of phi:, %f\n", phi[size/2 + size/2*size]);

    if(phi[size/2 + size/2*size] > 1){
        printf("Center exists?\n");
    }

    return phi;
}





