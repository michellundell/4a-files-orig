/**
 * @brief This program takes flight and booking information and 
 * create files with tickets.
 *
 * @file 4a-tickets.c
 * @author Michel
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

/**
 * main entry point of the program.
 * @param[in] argc Number of command line arguments.
 * @param[in] argv An array of pointers to null terminated arrays of characters (command line arguments).
 *            argv[1] should be a filename of the flightdata file in csv format.
 *            argv[2] should be a filename of the booking file in csv format.
 * @param[out] returns 0 on success.
*/

/* NOTE!! Please change the variables merge, 
 * current_line, row & seat in order 
 * to generate new .txt file for each passenger 
*/
int main(int argc, char **argv)
{
	FILE *fp1, *fp2, *merge;
	// to store characters from the file
	char dt[20], ti[20], dep[20], des[20], cl[20], fn[20], ln[20],
	depa[20], dest[20], date[20], time[20]; 
	// to store integers from the file
	int b, ret, ret1, fNum, fSeat, bSeat, eSeat, line = 0, line1 = 0;
	bool keep_reading = true, keep_reading1 = true;  

	// store 2 arguments in the terminal otherwise error
	if(argc < 3)
	{
		fprintf(stderr,"Error! You didn't initialize a filename.");
		exit(-1);
	}

	fp1 = fopen(argv[1], "r");
	fp2 = fopen(argv[2], "r");
	// ENTER FILE.TXT HERE EXAMPLE: TICKET-####.TXT <- CHECK BOOKINGS.CSV/FLIGHTS.CSV FOR REFERRAL
	merge = fopen("ENTER A FILE NAME HERE AND END IT WITH .TXT!", "w");


	if(!fp1 || !fp2 || !merge)
	{
		fprintf(stderr,"[ERROR] No such file in directory.\n");
		exit(-1);
	}
	// keeps track of lines in file
	int current_line = 10;

	do
	{
		fscanf(fp1, " %d, %[^,], %[^,], %[^,], %[^,], %[^,], %[^,], %[^'\n']\n", &b, dt, ti, dep, des, cl, fn, ln);
		line++;
		if(fp1 == '\0')
		{
			keep_reading = false;
			fprintf(stderr, "Couldn't find line %d.\n", line);
			exit(-1);
		}
		else if(current_line == line) /* if line matches the current line you want*/
		{
			keep_reading = false; 
			fprintf(merge, "BOOKING NR: #%d\nPASSENGER: %s %s\nCLASS: %s\n", b, fn, ln, cl);
			
		}
			
	}while(keep_reading);

	/* CHANGE CURRENT LINE ACCORDING TO FLIGHTS.CSV */
	int current_line2 = 6;

	/* MANUAL CHANGE FOR ROW & SEAT */ 
	/* !! MAKE SURE THE SEATS DON'T GET DUPLICATED. READ SEATS.TXT !! */
	int row = 6, seat = 38;

	do
	{
		fscanf(fp2, "%d, %[^,], %[^,], %[^,], %[^,], %d, %d, %d\n", &fNum, depa, dest, date, time, &fSeat, &bSeat, &eSeat);
		line1++;

		if(fp2 == '\0')
		{
			keep_reading1 = false;
			fprintf(stderr, "Couldn't find line %d.\n", line1);
			exit(-1);
		}
		else if(current_line2 == line1)
		{
			keep_reading1 = false;
			fprintf(merge, "FLIGHT: %d DEPARTURE: %s DESTINATION: %s %s %s\n", fNum, depa, dest, date, time);
			/* !! MANUALLY CHANGE THE VALUES FOR ROW & SEAT !! */
			fprintf(merge, "ROW: %d SEAT: %d", row, seat);
		}

	}while(keep_reading1);
	
	fclose(fp1);
	fclose(fp2);
	fclose(merge);

	return 0;
}
