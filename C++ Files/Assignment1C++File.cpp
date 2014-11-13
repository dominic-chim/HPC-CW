/* authors Dominic chim & Michael Waterworth
HPC Assignment 1 C Part */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>

#include "Assignment1C++.h"
//set dimensions of array





struct Params
{
	double cx,cy;  //grid points
	int nts; // total number of timesteps to be computed
}params;


int main(int argc, char *argv[])
{
    //Put parameters in to Params Struct
    //./temp_dist2D 0.1 0.1 50 initial_data.csv
   
	Params para;
	para.cx=strtod(argv[1],NULL);
	para.cy=strtod(argv[2], NULL);
	para.nts=strtod(argv[3], NULL);

	
	    double old_u[XDIM][YDIM];
    double new_u[XDIM][YDIM];
    //params para = params{argv[1],argv[2],argv[3]};
    int ix, iy, iz, it, ts; /* iterators */
    char *fname= argv[4];
    char nname[] = "data_";
    
    int nts = 100;
    it = 0;


    printf("Starting serial version of 2D temperature distribution example...\n");
 
    /* Read in grid dimension dim and allocate old_u and new_u */
	
    printf("Using [%d][%d] grid.\n", XDIM, YDIM);
	
    /* Read in grid sampling paameters and timesteps and initialize struct Params */
 
 
    /* Initialize grid from input file */
    printf("Initializing grid from input file: %s",fname);
 
    initdata(old_u, fname);


	//copy initialized grid to new grid,
    for(ix=0;ix<XDIM;++ix)
    {
        for(iy=0;iy<YDIM;++iy)
        {
           new_u[ix][iy]= old_u[ix][iy];
			//cout << old_u[ix][iy]<<'\n';
        }
    }
 
    // Iterate over all timesteps and create final output file (e.g. timestep = nts) 
    printf("Iterating over %d time steps...\n", para.nts);
    while(it < para.nts){
        if(it % 2 == 1){//if timestep is even 
            update(para.cx,para.cy, old_u, new_u);
        } else {// if timestep is odd 
            update(para.cx,para.cy, new_u, old_u);
        }
        it++;
    }
    printf("Done. Created output file: %d\n", it);

    prtdata(XDIM, YDIM, it, *new_u, nname);

	getchar();
	return 0;
}
 


