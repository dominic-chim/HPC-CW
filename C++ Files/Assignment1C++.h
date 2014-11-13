#include <fstream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <iostream>
using namespace std;

#define XDIM 100
#define YDIM 100

/***
*  update: computes new values for timestep t+delta_t
***/
void update(int nx, int ny, double u1[][100], double u2[][100])
{
    int ix, iy;
	/* copy boundary conditions from old_u to new_u */
    for (ix = 0; ix <= XDIM-1; ix++) //Top and bottom rows
    {
    	u1[nx-1][ix] = u2[nx-1][ix];
    	u1[0][ix] = u2[0][ix];
        // TO BE COMPLETED
    }
    for (iy = 0; iy <= YDIM-1; iy++) //Down both edges
    {
    	u1[iy][0] = u2[iy][0];
    	u1[iy][nx-1] = u2[iy][nx-1];
        // TO BE COMPLETED 
    }

 	
 	//cout << u1 << endl;

 	for(ix = 1;ix < nx - 1; ++ix)
	{
		for(iy = 1;iy < ny - 1; ++iy)
		{
			//Test using Cx & Cy = 0.1 - Needs to come from command line
			u1[ix][iy] = // Ux,y,t
			u2[ix][iy] // Ux,y,t−1
			+ nx * // + Cx *
				(u2[ix+1][iy] + u2[ix-1][iy] - 2 * u2[ix][iy])
			+ ny * // + Cy *
				(u2[ix][iy+1] + // (Ux,y+1,t−1 + 
				u2[ix][iy-1] - 2 * u2[ix][iy]); // Ux,y−1,t−1 − 2 * Ux,y,t−1)
		}
	}

 	//Ux,y,t = Ux,y,t−1 + Cx * (Ux+1,y,t−1 + Ux−1,y,t−1 − 2  Ux,y,t−1) + Cy * (Ux,y+1,t−1 + Ux,y−1,t−1 − 2 * Ux,y,t−1)
        /* TO BE COMPLETED */
 
}
 
/***
*  initdata: initializes old_u with data read from file specified at command line
***/
void initdata(double u1[XDIM][YDIM], char* fname)
{
/* TODO*/

	int ix, iy;
	printf("\n\n\n");
	ifstream fp(fname);
	
	if (fp == NULL)
	{
	  fprintf(stderr, "Can't open input file %s!\n",fname);
	  exit(1);
	}


	
	for(ix=0;ix<YDIM;ix++)
	{
		string line;
		getline(fp,line);
		stringstream src (line);
		
		for(iy=0;iy<XDIM;iy++)
		{
			string val;
			int temp=0;
			getline(src,val,',');
			stringstream end;
			end.clear();
			end.str("");
			end.str(val);
			end>>u1[ix][iy];
			//u1[ix][iy]=temp;
			//printf("%d \n",(int)u1[ix][iy]);  //FINALLY GOT THIS WORKING, NOW IM DEAD TIRED !!!! STUPID FLOAT PRECISION
			//cout<<"ul["<<ix<<"]["<<iy<<"] is "<<u1[ix][iy]<<'\n';
			
		}
		
	}
}


/***
*  printdata: generates a .csv file with data contained in parameter double* u1 
***/
 
void prtdata(int nx, int ny, int ts, double *u1, char* fname)
{
    int ix, iy;
	FILE *fp;
	char* ffname = fname;
	sprintf(fname,"%s%d.csv",fname,ts);
    for (iy = ny-1; iy >= 0; iy--) {
    fp = fopen(ffname, "w");
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
    fclose(fp);
    printf(" %s\n",fname);
}