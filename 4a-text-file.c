#include<stdio.h>
#include<stdlib.h>

int main(int argc, char **argv)
{
	int i=1,i10=10,i100=100,i1000=1000;

	if( argc != 2 ) { 
		fprintf(stderr,"No file given as an argument!"); 
		fprintf(stderr,"Syntax:\n"); 
		fprintf(stderr,"\t%s {filename}\n",argv[0]); 
		fflush(stderr);
		exit(-1); 
	}

	fprintf(stdout,"Writing text file (csv)\n");

	FILE *fp = fopen(argv[1],"w");
	if( !fp ) { 
		fprintf(stderr,"Could not open file %s\n",argv[1]);
		exit(-1); 
	}
	fprintf(fp,"%d;%d;%d;%d\n",i,i10,i100,i1000);
	fclose(fp);

	fprintf(stdout,"Wrote: %d, %d, %d, %d\n",i,i10,i100,i1000);

	i=0,i10=0,i100=0,i1000=0;

	fprintf(stdout,"Reading text file (csv)\n");

	fp = fopen(argv[1],"r");
	if( !fp ) { 
		fprintf(stderr,"Could not open file %s\n",argv[1]);
		fflush(stderr);
		exit(-1);
	}

	fscanf(fp,"%d;%d;%d;%d\n",&i,&i10,&i100,&i1000);
	fprintf(stdout,"Read: %d, %d, %d, %d\n",i,i10,i100,i1000);
	fclose(fp);
	
	return(0);
}
