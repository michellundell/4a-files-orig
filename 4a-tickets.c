/**
 * @brief This program takes flight and booking information and 
 * create files with tickets.
 *
 * @file 4a-tickets.c
 * @author Michel
*/


/**
 * main entry point of the program.
 * @param[in] argc Number of command line arguments.
 * @param[in] argv An array of pointers to null terminated arrays of characters (command line arguments).
 *            argv[1] should be a filename of the flightdata file in csv format.
 *            argv[2] should be a filename of the booking file in csv format.
 * @param[out] returns 0 on success.
*/

#include<stdio.h>
#include<stdlib.h>
#include <string.h>




struct reservation_struct
{ 
  int flight_number;
  char departure_airport[5];
  char arrival_airport[5];
  char departure_time[17];
  char departure_date[17];
  int frows;
  int *fseats;
  int brows;
  int *bseats;
  int erows;
  int *eseats;

  struct reservation_struct *next;
};
/*
        char dep[20],des[20],da[20],ti[20];
        int f,fs,bs,es,ret;
        FILE *fp = fopen("flights.csv","r");
        while( (ret = fscanf(fp,"%d,%[^,],%[^,],%[^,],%[^,],%d,%d,%d\n",&f,dep,des,da,ti,&fs,&bs,&es)) > 0 ) {
        fprintf(stdout,"ret=%d\n",ret);
        fprintf(stdout,"%d,%s,%s,%s,%s,%d,%d,%d\n",f,dep,des,da,ti,fs,bs,es);
*/



int
main (int argc, char **argv)
{
  char airport1[100];
  char airport2[100];
  struct reservation_struct *head = NULL;




  fprintf (stdout, "Writing text file  (csv)\n");



  FILE *stdmovant = fopen ("flights.csv", "r");
  if (!stdmovant)
    {
      fprintf (stderr, "Could not open file %s\n", argv[1]);



      return (-1);
    }




  printf ("Enter airport code: ");
  fgets (airport1, sizeof (airport1), stdin);
  airport1[strlen (airport1) - 1] = '\0';



  printf ("Enter another airport code: ");
  fgets (airport2, sizeof (airport2), stdin);
  airport2[strlen (airport2) - 1] = '\0';



  struct reservation_struct fln;
  while (fscanf
	 (stdmovant, "%d,%[^,],%[^,],%[^,],%[^,],%d,%d,%d\n", &fln.flight_number,
	  fln.departure_airport, fln.arrival_airport, fln.departure_date,
	  fln.departure_time, &fln.frows, &fln.brows, &fln.erows) == 8)
    {
      struct reservation_struct *nn =
	malloc (sizeof (struct reservation_struct));
      memcpy (nn, &fln, sizeof (struct reservation_struct));
      nn->fseats = malloc (fln.frows * sizeof (int) * 7);
						      /** 7 is the number of seats in a row */
      memset (nn->fseats, 0, fln.frows * 7);
      nn->bseats = malloc (fln.brows * sizeof (int) * 7);
						/** 7 is the number of seats in a row */
      memset (nn->bseats, 0, fln.brows *7);
      nn->eseats = malloc (fln.erows * sizeof (int) * 7);
						/** 7 is the number of seats in a row */
      memset (nn->eseats, 0, fln.erows *7);
      nn->next = head;
      head = nn;
    }



  return 0;

}
