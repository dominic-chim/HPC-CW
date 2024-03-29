#include <fstream>
//#include <sstream>
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