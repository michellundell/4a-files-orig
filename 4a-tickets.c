
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief This program takes flight and booking information and
 * @brief This program takes flight and booking information and
 * create files with tickets.
 *
 * @file 4a-tickets.c
 * @author Jesper Rehnfors
 */

/**
 * main entry point of the program.
@@ -14,9 +17,91 @@
 *            argv[1] should be a filename of the flightdata file in csv format.
 *            argv[2] should be a filename of the booking file in csv format.
 * @param[out] returns 0 on success.
*/

typedef struct flight_list_node
{
	int flightNo;	  // Flight number.
	char fDep[5];	  // Flight Departure.
	char fDest[5];	  // Flight Destination.
	char fDate[10];	  // Flight date.
	char fTime[6];	  // Flight departure time.
	int numberFSeats; // Number of first class seats.
	int numberBSeats; // Number of business class seats.
	int numberESeats; // Number of economy seats.
	int *fSeats;	  // Flag for first class seats.
	int *bSeats;	  // Flag for business class seats.
	int *eSeats;	  // Flag for economy class seats.

	struct flight_list_node *next;

} FlightListNode;

typedef struct bookings_list_node
{
	int bookingNo;	// Booking Number.
	char date[10];	// Departure date.
	char time[6];	// Departure time.
	char dep[5];	// Departure.
	char dest[5];	// Destination.
	char class[10]; // Seat class.
	char fname[20]; // Traveler first name.
	char lname[20]; // Traveler last name.

	struct bookings_list_node *next;

} BookingListNode;

FlightListNode *readFlight(const char *filename);
BookingListNode *readBooking(const char *filename);
int allocate_seat(FlightListNode *flight, BookingListNode *bookings, int *row, int *seat);
int create_ticket(FlightListNode *flight, BookingListNode *bookings);
void print_ticket(BookingListNode *blnp, FlightListNode *flnp, int seat, int row);

int main(int argc, char const *argv[])
{
	FlightListNode *flight = readFlight("flights.csv");
	BookingListNode *bookings = readBooking("bookings.csv");
	create_ticket(flight, bookings);

	return 0;
}

/**
 * @brief A function which reads a file and saves it's info in a list.
 *
 * @param filename
 * @return FlightListNode*
 */

FlightListNode *readFlight(const char *filename)
{
	FlightListNode flight, *head = NULL;
	FILE *fp = fopen(filename, "r");

	while (fscanf(fp, "%d, %[^,], %[^,], %[^,], %[^,], %d, %d, %d", &flight.flightNo, flight.fDep, flight.fDest, flight.fDate, flight.fTime, &flight.numberFSeats, &flight.numberBSeats, &flight.numberESeats) == 8)
	{

		FlightListNode *newNode = malloc(sizeof(FlightListNode)); // Skapar en ny nod med storlek av structen FlightListNode
		memcpy(newNode, &flight, sizeof(FlightListNode));		  // Kopierar värdena i flight till newNode

		newNode->fSeats = malloc(flight.numberFSeats * sizeof(int) * 7); // Skapar en rad, 7st platser
		memset(newNode->fSeats, 0, flight.numberFSeats);			 // Sätter varje värde till 0

		newNode->bSeats = malloc(flight.numberBSeats * sizeof(int) * 7);
		memset(newNode->bSeats, 0, flight.numberBSeats);

		newNode->eSeats = malloc(flight.numberESeats * sizeof(int) * 7);
		memset(newNode->eSeats, 0, flight.numberESeats);

		newNode->next = head;
		head = newNode;
	}
	fclose(fp);
	return (head);
}

BookingListNode *readBooking(const char *filename)
{
	BookingListNode bookings, *head = NULL;
	FILE *fp = fopen(filename, "r");

	while (fscanf(fp, "%d, %[^,], %[^,], %[^,], %[^,], %[^,], %[^,], %[^\n]", &bookings.bookingNo, bookings.date, bookings.time, bookings.dep, bookings.dest, bookings.class, bookings.fname, bookings.lname) == 8)
	{
		BookingListNode *newNode = malloc(sizeof(BookingListNode));
		memcpy(newNode, &bookings, sizeof(BookingListNode));

		newNode->next = head;
		head = newNode;
	}
	fclose(fp);
	return (head);
}

int allocate_seat(FlightListNode *flight, BookingListNode *bookings, int *row, int *seat)
{
	int seatNumber = 0;
	int rowNumber = 0;
	if (strcmp("first", bookings->class) == 0)
	{
		for (int p = 0; p < flight->numberFSeats*7; p++)
		{
			if (flight->fSeats[p] == 0)
			{
				flight->fSeats[p] = 1;
				seatNumber = p + 1;
				rowNumber = (int)p / 7 + 1;
				break;
			}
		}
	}

	if (strcmp("business", bookings->class) == 0)
	{
		for (int p = 0; p < flight->numberBSeats*7; p++)
		{
			if (flight->bSeats[p] == 0)
			{
				flight->bSeats[p] = 1;
				seatNumber = p + flight->numberFSeats * 7 + 1;
				rowNumber = flight->numberFSeats+(int)p / 7 + 1;
				break;
			}
		}
	}

	if (strcmp("economy", bookings->class) == 0)
	{
		for (int p = 0; p < flight->numberESeats*7; p++)
		{
			if (flight->eSeats[p] == 0)
			{
				flight->eSeats[p] = 1;
				seatNumber = p + flight->numberFSeats * 7 + flight->numberBSeats * 7 + 1;
				rowNumber = flight->numberFSeats + flight->numberBSeats+(int)p / 7 + 1;
				break;
			}
		}
	}

	if (rowNumber == 0 || seatNumber == 0)
	{
		fprintf(stdout,"Could not find class %s on this plane.", bookings->class);
	}

	*row = rowNumber;
	*seat = seatNumber;

	return (1);
}

int create_ticket(FlightListNode *flight, BookingListNode *bookings)
{
	int numberTickets = 0;
	fprintf(stdout, "Writing tickets: \n");
	for (BookingListNode *blnp = bookings; blnp != NULL; blnp = blnp->next)
	{
		for (FlightListNode *flnp = flight; flnp != NULL; flnp = flnp->next)
		{
			if (!strcmp(blnp->dep, flnp->fDep) && !strcmp(blnp->dest, flnp->fDest) && !strcmp(blnp->date, flnp->fDate) && !strcmp(blnp->time, flnp->fTime))
			{
				int row = 0, seat = 0;
				if (allocate_seat(flnp, blnp, &row, &seat))
				{
					fprintf(stdout, "[ticket-%d.txt]\n", blnp->bookingNo);
					print_ticket(blnp, flnp, seat, row);
					numberTickets++;
				}
			}
		}
	}

	fprintf(stdout,"Created %d tickets\n", numberTickets);
	return (numberTickets);
}

void print_ticket(BookingListNode *blnp, FlightListNode *flnp, int seat, int row)
{
	char filename[255];
	sprintf(filename, "Ticket-{%d}", blnp->bookingNo);
	FILE *fp = fopen(filename, "w");
	if (fp)
	{
		fprintf(fp, "Booking: %d\nFlight: %d, Departure: %s, Destination: %s, Date: %s, Time: %s\nPassenger: %s %s\nClass: %s\nRow: %d, Seat: %d", blnp->bookingNo, flnp->flightNo, blnp->dep, blnp->dest, blnp->date, blnp->time, blnp->fname, blnp->lname, blnp->class, row, seat);
		fclose(fp);
	}
}