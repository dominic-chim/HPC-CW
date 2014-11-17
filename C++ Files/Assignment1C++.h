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
 nx and ny are distribution values for x and y dimensions - limits propogation
***/
void update(double nx, double ny, double u1[][100], double u2[][100])
{
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
	int ix, iy;
	ifstream fp(fname);
	
	if (!fp)
	{
	  fprintf(stderr, "Can't open input file %s!\n",fname);
	  exit(1);
	}
	
	for(ix=0;ix<XDIM;ix++)
	{
		string line;
		getline(fp,line);
		stringstream src (line);
		
		for(iy=0;iy<YDIM;iy++)
		{
			string val;
			int temp=0;
			getline(src,val,',');
			stringstream end;
			end.clear();
			end.str("");
			end.str(val);
			end>>u1[ix][iy];			
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
	fp = fopen(ffname, "w");
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
    fclose(fp);
    printf(" %s\n",fname);
}