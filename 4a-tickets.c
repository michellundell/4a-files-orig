#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

typedef struct bookings
{
	int *bookingNo, *flight, *fSeats, *bSeats, *eSeats;
	char date[30], time[20];
	char dep[20], des[20], class[20], firstName[20], surName[20];

	struct bookings *next;

} bookings;

int main(int argc, char **argv)
{

	char buf[20];
	bookings *list = NULL;
	list = (bookings *)malloc(sizeof(bookings));
	list->bookingNo = 0;
	list->flight = 0;
	list->fSeats = 0;
	list->bSeats = 0;
	list->eSeats = 0;
	list->next = NULL;

	fprintf(stdout,"argc:%d\n", argc); // for test

	if (argc != 3)
	{
		printf("%d", argc);
		fprintf(stderr, "No file given as an argument!\n");
		fprintf(stderr, "Syntax:\n");
		fprintf(stderr, "\t%s {filename}\n", argv[0]);
		fflush(stderr);
		exit(-1);
	}

	fprintf(stdout, "Reading text file (csv)\n");
	fflush(stdout);

	FILE *fp1 = fopen(argv[1], "r");
	FILE *fp2 = fopen(argv[2], "r");

	fprintf(stdout,"e");
	fflush(stdout);
	if (!fp1)
	{
		fprintf(stderr, "Could not open file %s\n", argv[1]);
		exit(-1);
	}
	if (!fp2)
	{
		fprintf(stderr, "Could not open file %s\n", argv[2]);
		exit(-1);
	}

	fprintf(stdout, "woo");
	
	char c=fgetc(fp1), k=fgetc(fp2);

	while(c!=EOF && k!=EOF)
	{
		for (c; c!='\n'; c++)
		{
			fscanf(fp1, "%d,%*[^,],%*[^,],%*[^,],%*[^,],%[^,],%[^,],%[^\n]", list->bookingNo, list->class, list->firstName, list->surName);
		}
		for(k; k!='\n'; k++);
		{
			fscanf(fp2, "%d,%[^,],%[^,],%[^,],%[^,],%d,%d,%d", list->flight, list->dep, list->des, list->date, list->time, list->fSeats, list->bSeats, list->eSeats);
		}

		sprintf(buf, "Ticket-{%ls}.txt", list->bookingNo);
		FILE *fnew=fopen(buf, "w+");
		bookings *newLine = (bookings *)malloc(sizeof(bookings));

		for(newLine = list; newLine!=NULL; newLine=newLine->next)
		{
			fprintf(fnew, "BOOKING: {%ls}\nFLIGHT: {%ls} DEPARTURE: {%s} DESTINATION: {%s} {%s} {%s}\nPASSENGER {%s} {%s}\nCLASS: {%s}", list->bookingNo, list->flight, list->dep, list->des, list->date, list->time, list->firstName, list->surName, list->class);
		}
		free(list);
		fclose(fnew);
	}


	fclose(fp1);
	fclose(fp2);
	return 0;
}
