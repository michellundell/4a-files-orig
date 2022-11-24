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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CMD_ARGUMENTS 3U
#define FLIGHTS 1U
#define BOOKINGS 2U
#define TICKET "ticket-"
#define TXT_FILES 20

int main(int argc, char **argv)
{
	// Create feilds flightnumber,departure,destination,date,time,fseats,bseats,eseats

	char dep[20], des[20], da[20], ti[20];
	int fl_num, fs, bs, es, ret;
	if (argc != CMD_ARGUMENTS)
	{
		(void)printf("There are no data files given on the Command Prompt\n");
		(void)printf("You must give two data file names to run the program\n");
		(void)printf("Program will exit now....\n");
		exit(0);
	}
	for (int i = 0; i <= argc; i++)
	{
		printf("%s\n", argv[i]);
	}
	// Read flight data from flights.csv printout to terminal

	FILE *fp = fopen(argv[FLIGHTS], "r");
	while (fscanf(fp, "%d,%[^,],%[^,],%[^,],%[^,],%d,%d,%d\n", &fl_num, dep, des, da, ti, &fs, &bs, &es) == 8)
	//(ret = fscanf(fp, "%d,%[^,],%[^,],%[^,],%[^,],%d,%d,%d\n", &fl_num, dep, des, da, ti, &fs, &bs, &es)) > 0)
	{
		// fprintf(stdout, "ret=%d\n", ret);
		fprintf(stdout, "%d,%s,%s,%s,%s,%d,%d,%d\n", fl_num, dep, des, da, ti, fs, bs, es);
	}
	fclose(fp);

	// Create fields to read bookings.csv
	//	bookingnumber, date, time, departure, destination, seatclass, firstname, surname
	int bnumber;
	char date[20], time[20], departure[20], destination[20], seatclass[20], firstname[20], surname[20];
	char ticket[TXT_FILES];
	//  Read booking data from bookings.csv and printout to terminal
	(void)printf("Reading bookings.csv file now...\n");

	FILE *tp; // file pointer to create ticket files
	FILE *bp = fopen(argv[BOOKINGS], "r");
	while (fscanf(bp, "%d,%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^\n]\n", &bnumber, date, time, departure, destination, seatclass, firstname, surname) == 8)
	{
		fprintf(stdout, "%d,%s,%s,%s,%s,%s,%s,%s\n", bnumber, date, time, departure, destination, seatclass, firstname, surname);
		// create ticket text file
		sprintf(ticket, "ticket-%d.txt", bnumber);
		tp = fopen(ticket, "w");
		if (!tp)
		{
			(void)printf("File could not be created\n");
		}
		fprintf(tp, "BOOKING:%i\nFLIGHT:%i DEPARTURE:%s DESITNATION:%s\n%s %s\nPASSENGER %s %s\nCLASS: %s",
				bnumber, bnumber, departure, destination, date, time, firstname, surname, seatclass);
		fclose(tp); // Close the file.
	}

	return 0;
}

// Helper functions
void print_ticket() {}