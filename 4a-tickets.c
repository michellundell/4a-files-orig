/**
 * @file 4a-tickets.c
 * @author Julie Anne Cantillep
 * @brief A program that generates ticket for every passenger
 * @version 0.1
 * @date 2022-11-04
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* -- CREATE DATA STRUCTURES THAT DEFINES THE FLIGHT DATA -- */
typedef struct flight_reservation_node
{
    int flightnum;       // <- Flight no.
    char depart[20];     // <- Departure airport code
    char dest[20];       // <- Destination airport code
    char datestr[20];    // <- Date departure
    char timestr[20];    // <- Time departure
    int fclass;          // <- No. of First class rows
    int *farr;           // <- Array to first class. 1 == occupied, 0 == not occupied
    int bclass;          // <- No. of Business class rows
    int *barr;           // <- Array to business class. 1 == occupied, 0 == not occupied
    int eclass;          // <- No. of Economy class rows
    int *earr;           // <- Array to economy class. 1 == occupied, 0 == not occupied
    struct flight_reservation_node *next;   // <- A pointer to the next flight

} FlightReserveNode;

/* -- CREATE DATA STRUCTURE THAT DEFINES A BOOKING TICKET -- */
typedef struct book_reservation_node
{
    int booknum;         // <- Booking. no
    char datestr[15];    // <- Departure date
    char timestr[15];    // <- Departure time
    char depart[15];     // <- Departure airport
    char dest[15];       // <- Destination airport
    char clas[20];       // <- Seat class
    char fname[25];      // <- First name
    char lname[25];      // <- Last name
    struct book_reservation_node *next;   // <- A pointer to the next booking

}BookReserveNode;

/* -- FUNCTION DEFINITION -- */
FlightReserveNode *read_flights(const char *filename);
BookReserveNode *read_bookings(const char *filename);
int create_tickets(BookReserveNode *bookings, FlightReserveNode *flights);
int allocate_seat(BookReserveNode *booking, FlightReserveNode *flight, int *row, int *seat);
void print_ticket(BookReserveNode *brnp, FlightReserveNode *frnp, int seat, int row);

/**
 * @brief Main entry point of the program.
 * This creates a ticket for each passenger from FILE.
 * 
 * @param [in] argc Number of arguments
 * @param [in] argv Takes vector of arguments
 * @return returns .txt files
 */

int main(int argc, char const *argv[])
{
	/* -- POINTER TO STRUCTS -- */
	BookReserveNode *bookings;
	FlightReserveNode *flights;

	/* -- CALL STRUCT FUNCTION -- */
    bookings = read_bookings(argv[1]);
	flights = read_flights(argv[2]);

	if(flights && bookings)
	{
		create_tickets(bookings, flights);
	}
	else
	{
		fprintf(stderr, "Error\n");
		exit(-1);
	}

    return 0;

}

/**
 * @brief Function that takes data flight
 * 
 */
/* READ INDATA FLIGHTS FILE AND SAVE IT IN A FLIGHTS LIST */
FlightReserveNode *read_flights(const char *filename)
{
    FlightReserveNode fln, *head = NULL; // <- next is unknown so it's set to NULL
    FILE *fp = fopen(filename, "r");  // <- read flight list

    while(fscanf(fp, "%d, %[^,], %[^,], %[^,], %[^,], %d, %d, %d\n", &fln.flightnum, fln.depart, fln.dest, fln.datestr, fln.timestr, &fln.fclass, &fln.bclass, &fln.eclass) == 8)
    {
        FlightReserveNode *nn = malloc(sizeof(FlightReserveNode)); // <-- allocate memory size to node/struct that takes indata
        memcpy(nn, &fln, sizeof(FlightReserveNode));   // <- copy memory of fl data to nn
        // assigns each class with no. of seat == 7
        nn->farr = malloc(fln.fclass * sizeof(int) * 7);  // <- 7 is the number of seats in a row
        memset(nn->farr, 0, fln.fclass);  // replaces values of seats with 0 <- not occupied
        nn->barr = malloc(fln.bclass * sizeof(int) * 7);
        memset(nn->barr, 0, fln.bclass);
        nn->earr = malloc(fln.eclass * sizeof(int) * 7);
        memset(nn->earr, 0, fln.eclass);
        nn->next = head;   // <- points next to head
        head = nn;         // <- returns head to nn, which reads new data again.
    }

    fclose(fp);
    return(head);
}

/**
 * @brief function that takes booking data
 * 
 */
/* READ INDATA BOOKING FILE AND SAVE IT IN A BOOKING LIST */
BookReserveNode *read_bookings(const char *filename)
{
    BookReserveNode bok, *last = NULL; // <- pointing to next which is unknown so it's set to NULL
    FILE *fp = fopen(filename, "r"); // <- read booking list

    // read each line in file excluding comma and newline
    while(fscanf(fp, "%d, %[^,], %[^,], %[^,], %[^,], %[^,], %[^,], %[^'\n']\n", &bok.booknum, bok.datestr, bok.timestr, bok.depart, bok.dest, bok.clas, bok.fname, bok.lname) == 8)
    {
        BookReserveNode *bk = malloc(sizeof(BookReserveNode)); // <- allocate memory size for booking list
        memcpy(bk, &bok, sizeof(BookReserveNode)); // <- copy memory of data bok to bk
        bk->next = last;  // <- points next to last
        last = bk;       // <- returns last to bk, which reads new data again.

    }
	
    fclose(fp);
    return(last);

}
/**
 * @brief Function that creates ticket from bookings & flights.csv
 * 
 */

/* CREATE TICKET FOR EACH LINE IN BOOKING LIST */
/* FOR EACH BOOKING IN THE BOOKING LIST */
int create_tickets(BookReserveNode *bookings, FlightReserveNode *flights)
{
    int num_tickets = 0; // <- variable to count no. of tickets
    fprintf(stdout, "Writing tickets:\n");
    /* ITERATE THROUGH BOOKING LIST */
    for(BookReserveNode *brnp = bookings; brnp != NULL; brnp = brnp->next)
    {
        /* ITERATE THROUGH FLIGHT LIST */
        for(FlightReserveNode *frnp = flights; frnp != NULL; frnp = frnp->next)
        {
            // checks if dep, dest, date & time don't match
            if(!strcmp(brnp->depart, frnp->depart) && !strcmp(brnp->dest, frnp->dest) && !strcmp(brnp->datestr, frnp->datestr) && !strcmp(brnp->timestr, frnp->timestr))
            {
                /*  ALLOCATE A SEAT AND PRINT THE TICKET */
                int row = 0, seat = 0;
                if(allocate_seat(brnp, frnp, &row, &seat)) // <- call seat assigning function
                {
                    fprintf(stdout, "[ticket-%d.txt]", brnp->booknum); 
					printf("\n");
                    print_ticket(brnp, frnp, seat, row); // <- function to print ticket
                    num_tickets++;  // appends the ticket for each iteration
                }

            }
        }

    }
	// print the total no. of tickets
    fprintf(stdout, "Created %d tickets\n\n", num_tickets); 
    return(num_tickets);

}
/**
 * @brief Function that allocates seat for each class.
 * First, Business & Economy class.
 */
/* ALLOCATE A SEAT AND MARK THE SEAT BOOKED INCL. ROW & SEAT NO. */
int allocate_seat(BookReserveNode *booking, FlightReserveNode *flight, int *row, int *seat)
{
    int sn = 0;   // <- points to *seat pointer as referral
    int rn = 0;   // <- points to *row pointer as referral
    if(strcmp("first", booking->clas) == 0)   // <- if there's first match in .txt file
    {
        for(int p = 0; p < flight->fclass * 7; p++)    // <- loop through sits
        {
            if(flight->farr[p] == 0)    // assign class to array
            {
                flight->farr[p] = 1;    
                sn = p + 1;
                rn = (int)p / 7 + 1;
                break;
            }
        }
    }
    if(strcmp("business", booking->clas) == 0)   // <- if there's business match in .txt file
    {
        for(int p = 0; p < flight->bclass * 7; p++)    // <- loop through sits
        {
            if(flight->barr[p] == 0)
            {
                flight->barr[p] = 1;
                sn = p + flight->fclass * 7 + 1;    // <- include first class so to not merge
                rn = flight->fclass + (int)p / 7 + 1;  
                break;
            }
        }
    }
    if(strcmp("economy", booking->clas) == 0)  // <- if there's economy match in .txt file
    {
        for(int p = 0; p < flight->eclass * 7; p++)  // <- loop through sits
        {
            if(flight->earr[p] == 0)
            {
                flight->earr[p] = 1;
                sn = p + flight->fclass * 7 + flight->bclass * 7 + 1; // <-include fclass & bclass to not merge
                rn = flight->fclass + flight->bclass + (int)p / 7 + 1; 
                break;
            }
        }
    }

    if(rn == 0 || sn == 0) 
    {
        fprintf(stdout, "did not find class %s for booking on this plane\n", booking->clas);
        return(0);
    }
    *row = rn;  // row no.
    *seat = sn; // seat no.
    return(1);
}

/**
 * @brief Function that prints the ticket out
 * 
 */
/* PRINT THE TICKET TO A TICKET FILE */
void print_ticket(BookReserveNode *brnp, FlightReserveNode *frnp, int seat, int row)
{
    char filename[20]; 
	sprintf(filename, "ticket-%d.txt", brnp->booknum);  // <- store ticket name in filename
	FILE *fp = fopen(filename,"w");  
	if(fp)
	{
		fprintf(fp, "BOOKING: %d\n", brnp->booknum);
		fprintf(fp, "FLIGHT: %d DEPARTURE: %s DESTINATION: %s %s %s\n", frnp->flightnum, frnp->depart, frnp->dest, frnp->datestr, frnp->timestr);
	    fprintf(fp, "PASSENGER: %s %s\n", brnp->fname, brnp->lname);
        fprintf(fp, "CLASS: %s\n", brnp->clas);
        fprintf(fp, "ROW: %d SEAT: %d\n", row, seat);
        fclose(fp);
	}

}




