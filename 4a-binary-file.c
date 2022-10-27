#include<stdio.h>
#include<stdlib.h>

typedef struct complex_data {
	int x,y,value;
} MyComplexData;
 
int main(int argc, char **argv)
{
	MyComplexData cdout;
	MyComplexData cdin;

	if( argc != 2 ) { 
		fprintf(stderr,"No file given as an argument!"); 
		fprintf(stderr,"Syntax:\n\n"); 
		fprintf(stderr,"\t%s {filename.bin}\n\n",argv[0]); 
		exit(-1); 
	}

	printf("Now writing the data ...\n");

	FILE *fp = fopen(argv[1],"w");
	if(!fp){
		fprintf(stderr,"Could not open file %s\n",argv[1]);
		exit(-1);
	}

	cdout.x=3; cdout.y=7; cdout.value=76;

	fwrite(&cdout,1,sizeof(MyComplexData),fp);
	fclose(fp);

	printf("Wrote: x=%d, y=%d, value=%d\n",cdout.x,cdout.y,cdout.value);

	printf("Now reading the data back ...\n");

	fp = fopen(argv[1],"r");
	if(!fp){
		fprintf(stderr,"Could not open file %s\n",argv[1]);
		exit(-1);
	}

	fread(&cdin,1,sizeof(MyComplexData),fp);
	fclose(fp);

	printf("Read: x=%d, y=%d, value=%d\n",cdin.x,cdin.y,cdin.value);

	return(0);
}
