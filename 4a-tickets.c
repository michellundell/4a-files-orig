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
/**
 * main entry point of the program.
 * @param[in] argc Number of command line arguments.
 * @param[in] argv An array of pointers to null terminated arrays of characters (command line arguments).
 *            argv[1] should be a filename of the flightdata file in csv format.
 *            argv[2] should be a filename of the booking file in csv format.
 * @param[out] returns 0 on success.
 */

typedef struct flights
{
	int flightN, fMax, bMax, eMax;
	char depart[4], arrive[4], date[11], time[6];
	struct flights *next;
} flights;

typedef struct bookings
{
	int bookingN;
	char date[11], time[6], depart[4], arrive[4], class[10], firstName[20], lastName[20];
	struct bookings *next;
} bookings;

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		fprintf(stderr, "No file given as an argument!");
		fprintf(stderr, "Syntax:\n");
		fprintf(stderr, "\t%s {filename} {filename}\n", argv[0]);
		fflush(stderr);
		exit(-1);
	}

	FILE *fp = fopen(argv[1], "r");

	if (!fp)
	{
		fprintf(stderr, "Could not open file %s", argv[1]);
		exit(-1);
	}

	flights *flightList = malloc(sizeof(flights));
	flightList->next = NULL;

	while (fscanf(fp, "%d,%[^,],%[^,],%[^,],%[^,],%d,%d,%d\n", &flightList->flightN, flightList->depart, flightList->arrive, flightList->date, flightList->time, &flightList->fMax, &flightList->bMax, &flightList->eMax) == 8)
	{
		flights *newFlight = malloc(sizeof(flights));
		memset(newFlight, 0, sizeof(flights));
		newFlight->next = flightList;
		flightList = newFlight;
	}

	flights *tempF = flightList;
	flightList = flightList->next;
	free(tempF);

	for (flights *nodeP = flightList; nodeP != NULL; nodeP = nodeP->next)
	{
		fprintf(stdout, "%d,%s,%s,%s,%s,%d,%d,%d\n", nodeP->flightN, nodeP->depart, nodeP->arrive, nodeP->date, nodeP->time, nodeP->fMax, nodeP->bMax, nodeP->eMax);
	}

	fclose(fp);

	fp = fopen(argv[2], "r");

	if (!fp)
	{
		fprintf(stderr, "Could not open file %s", argv[2]);
		exit(-1);
	}

	bookings *bookingList = malloc(sizeof(bookings));
	bookingList->next = NULL;

	while (fscanf(fp, "%d,%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^\n]\n", &bookingList->bookingN, bookingList->date, bookingList->time, bookingList->depart, bookingList->arrive, bookingList->class, bookingList->firstName, bookingList->lastName) == 8)
	{
		bookings *newBooking = malloc(sizeof(bookings));
		memset(newBooking,0,sizeof(bookings));
		newBooking->next = bookingList;
		bookingList = newBooking;
	}

	bookings *tempB = bookingList;
	bookingList = bookingList->next;
	free(tempB);

	for (bookings *nodeP = bookingList; nodeP != NULL; nodeP = nodeP->next)
	{
		fprintf(stdout, "%d,%s,%s,%s,%s,%d,%d,%d\n", nodeP->bookingN, nodeP->date, nodeP->time, nodeP->depart, nodeP->arrive, nodeP->class, nodeP->firstName, nodeP->lastName);
	}

	fclose(fp);

	return 0;
}
