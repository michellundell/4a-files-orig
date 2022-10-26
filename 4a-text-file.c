#include<stdio.h>
#include<stdlib.h>
int main(int argc, char **argv)
{
	int i=1,i10=10,i100=100,i1000=1000;
	if( argc != 2 ) { fprintf(stderr,"No file given as an argument!"); exit(-1); }
	FILE *fp = fopen(argv[1],"w");
	if( !fp ) { 
		fprintf(stderr,"Could not open file %s\n",argv[1]); exit(-1); 
	}
	fprintf(stdout,"Writing text file (csv)\n");
	fprintf(fp,"%d;%d;%d;%d\n",i,i10,i100,i1000);
	fprintf(stdout,"Wrote: %d, %d, %d, %d\n",i,i10,i100,i1000);
	fclose(fp);

	i=0,i10=0,i100=0,i1000=0;
	fp = fopen(argv[1],"r");
	if( !fp ) { 
		fprintf(stderr,"Could not open file %s\n",argv[1]); exit(-1); 
	}

	fprintf(stdout,"Reading text file (csv)\n");
	fscanf(fp,"%d;%d;%d;%d\n",&i,&i10,&i100,&i1000);
	fprintf(stdout,"Read: %d, %d, %d, %d\n",i,i10,i100,i1000);
	fclose(fp);
	
	return(0);
}
