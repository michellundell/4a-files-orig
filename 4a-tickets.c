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

	struct flights_list_node *next;

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
int allocate_seat(BookingListNode *bookings, FlightListNode *flight, int *row, int *seat);
int create_ticket (BookingListNode *bookings, FlightListNode *flight, int *row, int *seat);
void print_ticket(BookingListNode *bookings, FlightListNode *flight, int *seat, int *row);

int main(int argc, char const *argv[])
{
	FlightListNode *flights = NULL;
	BookingListNode *bookings = NULL;

	flights = readFlight(argv[1]);
	bookings = readBooking(argv[2]);

	if(flights && bookings)
	{
		create_tickets(flights, bookings);
	}

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
	FILE *filename = fopen(filename, "r");
	int create_ticket (BookingListNode *bookings, FlightListNode *flight, int *row, int *seat);
	

	while ((fscanf(filename, "%d, %[^,], %[^,], %[^,], %[^,], %d, %d, %d"), &flight.flightNo, flight.fDep, flight.fDest, flight.fDate, flight.fTime,
			&flight.numberFSeats, &flight.numberBSeats, &flight.numberESeats) == 8)
	{

		FlightListNode *newNode = malloc(sizeof(FlightListNode)); // Skapar en ny nod med storlek av structen FlightListNode
		memcpy(newNode, &flight, sizeof(FlightListNode));		  // Kopierar värdena i flight till newNode

		newNode->numberFSeats = malloc(flight.numberFSeats * 7); // Skapar en rad, 7st platser
		memset(newNode, 0, sizeof(int) * 7);					 // Sätter varje värde till 0

		newNode->numberBSeats = malloc(flight.numberBSeats * 7);
		memset(newNode, 0, sizeof(int) * 7);

		newNode->numberESeats = malloc(flight.numberESeats * 7);
		memset(newNode, 0, sizeof(int) * 7);

		newNode->next = head;
		head = newNode;
	}
}

BookingListNode *readBooking(const char *filename)
{
	BookingListNode bookings, *head;
	FILE *filename = fopen(filename, "r");

	while (fscanf(filename, "%d, %[^,], %[^,], %[^,], %[^,], %[^,], %[^,], %[^,], %[^,]", &bookings.bookingNo, bookings.date, bookings.time, bookings.dep,
				  bookings.dest, bookings.class, bookings.fname, bookings.lname) == 8)
	{
		BookingListNode *newNode = malloc(sizeof(BookingListNode));
		memcpy(newNode, &bookings, sizeof(BookingListNode));

		newNode->next = head;
		head = newNode;
	}
}

int allocate_seat(BookingListNode *bookings, FlightListNode *flight, int *row, int *seat)
{
	int seatNumber = 0;
	int rowNumber = 0;
	if (strcmp("first", bookings->class) == 0)
	{
		for (int i = 0; i < flight->fSeats; i++)
		{
			if (flight->fSeats[i] == 0)
			{
				flight->fSeats[i] = 1;
				seatNumber = i + 1;
				rowNumber = (int)i / 7 + 1;
				break;
			}
		}
	}

	if (strcmp("business", bookings->class) == 0)
	{
		for (int i = 0; i < flight->bSeats; i++)
		{
			if (flight->bSeats[i] == 0)
			{
				flight->bSeats[i] = 1;
				seatNumber = i + flight->numberFSeats * 7 + 1;
				rowNumber = (int)i / 7 + 1;
			}
		}
	}

	if (strcmp("economy", bookings->class) == 0)
	{
		for (int i = 0; i < flight->eSeats; i++)
		{
			if (flight->eSeats[i] == 0)
			{
				flight->eSeats[i] = 1;
				seatNumber = i + flight->numberBSeats + flight->numberFSeats * 7 + 1;
			}
		}
	}

	row = rowNumber;
	seat = seatNumber;

	return (1);
}

int create_ticket (BookingListNode *bookings, FlightListNode *flight, int *row, int *seat)
{
	int numberTickets = 0;
	fprintf(stdout, "Writing tickets: ");
	for(BookingListNode *blnp = bookings; blnp!=NULL; blnp = blnp->next)
	{
		for(FlightListNode *flnp = flight; flnp!=NULL; flnp = flnp->next)
		{
			if(!strcmp(blnp->dep, flnp->fDep) && !strcmp(blnp->dest, flnp->fDest) && !strcmp(blnp->time, flnp->fTime) && !strcmp(blnp->time, flnp->fTime))
			{
				int row=0, seat=0;
				if(allocate_seat(flnp,blnp,&row,&seat)){
					fprintf(stdout,"[ticket-%d.txt]",blnp->bookingNo);
					print_ticket(blnp,flnp,seat,row);
					numberTickets++;
				}

			}
		}
	}
}

void print_ticket(BookingListNode *bookings, FlightListNode *flight, int *seat, int *row)
{
	char filename[255];
	if(allocate_seat(bookings, flight, row, seat)==1)
	{
		sprintf(filename,"Ticket-{%d}", bookings->bookingNo);
		FILE *fp = fopen(filename, "w");
		if(fp)
		fprintf(fp, "Booking: %d\nFlight: %s, Departure: %s, Destination: %s, Date: %s, Time: %s\nClass: %s\nRow: %d, Seat: %d", bookings->bookingNo, flight->flightNo, bookings->dep, bookings->dest);
		fclose(fp);
	}

}