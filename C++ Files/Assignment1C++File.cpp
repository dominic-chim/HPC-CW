/* authors Dominic chim & Michael Waterworth
HPC Assignment 1 C Part */

#include <stdio.h>
#include <stdlib.h>
#include <cmath>
//set dimensions of array

#include <fstream>
//#include <sstream>
#include <stdio.h>
#include <iostream>
using namespace std;

#define XDIM 100
#define YDIM 100


struct Params
{
	double cx,cy;  //grid points
	int nts; // total number of timesteps to be computed
}params;



/***
*  update: computes new values for timestep t+delta_t
 nx and ny are distribution values for x and y dimensions - limits propogation
***/
void update(double nx, double ny, double u1[][100], double u2[][100])
{

	cout << "U2: " << u2[10][10] << endl;
    int ix, iy;

	/* copy boundary conditions from old_u to new_u */
    for (ix = 0; ix <= XDIM-1; ix++) //Top and bottom rows
    {
    	u1[XDIM-1][ix] = u2[XDIM-1][ix];
    	u1[0][ix] = u2[0][ix];
    }
    for (iy = 0; iy <= YDIM-1; iy++) //Down both edges
    {
    	u1[iy][0] = u2[iy][0];
    	u1[iy][YDIM-1] = u2[iy][YDIM-1];
    }

 	for(ix = 1;ix < XDIM - 1; ++ix)
	{
		for(iy = 1;iy < YDIM - 1; ++iy)
		{
			u1[ix][iy] = // Ux,y,t
			u2[ix][iy] // Ux,y,t−1
			+ nx * // + Cx *
				(u2[ix+1][iy] + u2[ix-1][iy] - 2 * u2[ix][iy]) //(Ux+1,y,t−1 + Ux−1,y,t−1 − 2  Ux,y,t−1)
			+ ny * // + Cy *
				(u2[ix][iy+1] + // (Ux,y+1,t−1 + 
				u2[ix][iy-1] - 2 * u2[ix][iy]); // Ux,y−1,t−1 − 2 * Ux,y,t−1)
		}
	}
}
 
/***
*  initdata: initializes old_u with data read from file specified at command line
***/
void initdata(double u1[XDIM][YDIM], char* fname)
{
	int i=0,j=0;
	FILE *fp = fopen(fname,"r");
	if (!fp)
	{
	  fprintf(stderr, "Can't open input file %s!\n",fname);
	  exit(1);
	}
	
	char *line, *temp,*data;
	char *record;
	char buffer[2048];
	double val;
	
	while((line=fgets(buffer,sizeof(buffer),fp))!=NULL)
   	{
		//cout<<line<<"\n\n\n";
		record = strtok(line,",");
		while(record != NULL)
		{
			if(j >= XDIM){//Break for last comma
				break;
			}
			//cout << i << " " << j << endl;
			//printf("record : %s\n",record) ;    //here you can put the record into the array as per your requirement.
			u1[i][j] = atoi(record);
			//cout << atof(record) << endl;
			record = strtok(NULL,",");
			//printf("%g \n", u1[i][j]);
			j++;
   		}
		j = 0;
		i++;
   }

  //  	for(int p = 1; p < XDIM; p++){
  //  		for(int q = 1; q < XDIM; q++){
  //  			cout << u1[p][q] << endl;
		// }
  //  	}
}


/***
*  printdata: generates a .csv file with data contained in parameter double* u1 
***/
void prtdata(int nx, int ny, int ts, double *u1, char* fname)
{
    int ix, iy;
	FILE *fp;
	char ffname[100];
	sprintf(ffname,"output/%s%d.csv",fname,ts);
	fp = fopen(ffname, "w");
	if(fp!=NULL)
	{
		for (iy = ny-1; iy >= 0; iy--) {
			for (ix = 0; ix <= nx-1; ix++) {
				fprintf(fp, "%8.3f,", *(u1+ix*ny+iy));
				if (ix != nx-1) {
					fprintf(fp, " ");
				}
				else {
					fprintf(fp, "\n");
				}
			}
		}
	}
	fflush(fp);
    fclose(fp);
    printf(" %s\n",ffname);
}




int main(int argc, char *argv[])
{
    //./temp_dist2D 0.1 0.1 50 initial_data.csv

    /* Read in grid sampling paameters and timesteps and initialize struct Params */
	Params para;
	para.cx=strtod(argv[1],NULL);
	para.cy=strtod(argv[2], NULL);
	para.nts=strtod(argv[3], NULL);

	/* Read grid dimension dim and allocate old_u and new_u */
	double old_u[XDIM][YDIM];
    double new_u[XDIM][YDIM];
    printf("Using [%d][%d] grid.\n", XDIM, YDIM);

    int it = 0; // Ts iterator
    char *fname= argv[4];
    char nname[] = "final_data_";
    
    printf("Starting serial version of 2D temperature distribution example...\n");
 
    /* Initialize grid from input file */
    printf("Initializing grid from input file: %s\n",fname);
    initdata(old_u, fname);
    cout << ": " << old_u[10][10] << endl;


    // Iterate over all timesteps and create output files
    printf("Iterating over %d time steps...\n", para.nts);
    while(it < para.nts){
		
        if(it % 2 == 1){//if timestep is odd 
            update(para.cx, para.cy, old_u, new_u);
			prtdata(XDIM, YDIM, it, *old_u, nname);
        } else {// if timestep is even 
            update(para.cx, para.cy, new_u, old_u);
			prtdata(XDIM, YDIM, it, *new_u, nname);
        }
       it++;
    }
    printf("Done. Created output file: %d\n", it);

	getchar();
	return 0;
}
 


