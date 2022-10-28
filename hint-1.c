
/*
304,GOT,CPH,2022-10-27,06:30,28,70,112
304,GOT,CPH,2022-10-27,07:30,28,70,112
304,GOT,CPH,2022-10-27,08:30,28,70,112
304,GOT,CPH,2022-10-27,09:30,28,70,112
306,CPH,LHR,2022-10-27,06:30,14,28,70
308,GOT,LGW,2022-10-27,06:30,28,70,112
310,GOT,ARN,2022-10-27,06:30,14,28,70
310,GOT,ARN,2022-10-27,07:30,14,28,70
310,GOT,ARN,2022-10-27,08:30,14,28,70
312,STO,HEL,2022-10-27,06:30,28,70,112
312,STO,HEL,2022-10-27,07:30,28,70,112
312,STO,HEL,2022-10-27,08:30,28,70,112
*/

#include<stdio.h>

int main()
{
	char dep[20],des[20],da[20],ti[20];
	int f,fs,bs,es,ret;
	FILE *fp = fopen("flights.csv","r");
	while( (ret = fscanf(fp,"%d,%[^,],%[^,],%[^,],%[^,],%d,%d,%d\n",&f,dep,des,da,ti,&fs,&bs,&es)) > 0 ) {
		fprintf(stdout,"ret=%d\n",ret);
		fprintf(stdout,"%d,%s,%s,%s,%s,%d,%d,%d\n",f,dep,des,da,ti,fs,bs,es);
	}
	return(0);
}
