/**
 * filename mc
 * author Elev User  (ahmed.absharibrahim@studerande.movant.se)
 * brief A program that generates tickets based on CSV data.
 * version 0.1
 * date 2022-11-07
 *
 * copyright Copyright (c) 2022
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/**
 * @brief flight_list node containing all variables from flight.csv file
 *
 */
typedef struct flight_list
{
    int flightNumm;            /* !< The flight number */
    char de[20];      /* !< Departure airport code */
    char dest[20];        /* !< Destination airport code */
    char Date[20];      /* !< Date departure*/
    char Hour[20];      /* !< Time departure */
    int firstClass;           /* !< Number of First class rows */
    int *fs;                  /* !< An array with flags of if a seat is taken or not. 1==occupied, 0==not occupied */
    int businessClass;        /* !< Number of Business class rows */
    int *bc;                  /* !< An array with flags of if a seat is taken or not. 1==occupied, 0==not occupied */
    int economyClass;         /* !< Number of Economy class rows */
    int *ec;                  /* !< An array with flags of if a seat is taken or not. 1==occupied, 0==not occupied */
    struct flight_list *next; /* !< A pointer to the next flight information */
} flightList;

/**
 * @brief booking_list containing all variables from bookingsfile.csv file
 *
 */
typedef struct booking_list
{
    int bookingNumm;            /* !< The booking number */
    char Date[15];       /* !< The departure date */
    char Hour[15];       /* !< The departure time */
    char de[10];       /* !< The departure airport */
    char dest[10];         /* !< The destination airport */
    char seatClass[20];        /* !< The seat class */
    char fName[25];        /* !< Firstname */
    char lName[25];         /* !< Lastname */
    struct booking_list *next; /* !< a pointer to the next booking */
} bookingList;

flightList *read_flights(const char *filename);
bookingList *read_bookings(const char *filename);
int create_tickets(bookingList *bookings, flightList *flights);
int allocate_seat(flightList *flight, bookingList *bookingNumm, int *row, int *seat);
void print_ticket(bookingList *blnp, flightList *flnp, int seat, int row);

/**
 * @brief Function to scan flights.csv file and store all variables
 *
 * @param filename
 * @return flightList*
 */
flightList *read_flights(const char *filename)
{
    flightList fln, *head = NULL;
    FILE *fp = fopen(filename, "r");

    while (fscanf(fp, "%d, %[^,], %[^,], %[^,], %[^,], %d, %d, %d\n", &fln.flightNumm, fln.de, fln.dest, fln.Date, fln.Hour, &fln.firstClass, &fln.businessClass, &fln.economyClass) == 8)
    {
        flightList *nn = malloc(sizeof(flightList));
        memcpy(nn, &fln, sizeof(flightList));
        nn->fs = malloc(fln.firstClass * sizeof(int) * 7); /** 7 is the number of seats in a row */
        memset(nn->fs, 0, fln.firstClass * 7);
        nn->bc = malloc(fln.businessClass * sizeof(int) * 7); /** 7 is the number of seats in a row */
        memset(nn->bc, 0, fln.businessClass * 7);
        nn->ec = malloc(fln.economyClass * sizeof(int) * 7); /** 7 is the number of seats in a row */
        memset(nn->ec, 0, fln.economyClass * 7);
        nn->next = head;
        head = nn;
    }
    fclose(fp);
    return (head);
}

/**
 * @brief Function to scan bookingsfile.csv and store all variables
 *
 * @param filename
 * @return bookingList*
 */
bookingList *read_bookings(const char *filename)
{
    bookingList fln, *head = NULL;
    FILE *fp = fopen(filename, "r");

    while (fscanf(fp, "%d, %[^,], %[^,], %[^,], %[^,], %[^,], %[^,], %[^\n]\n", &fln.bookingNumm, fln.Date, fln.Hour, fln.de, fln.dest, fln.seatClass, fln.fName, fln.lName) == 8)
    {
        bookingList *nn = malloc(sizeof(bookingList));
        memcpy(nn, &fln, sizeof(flightList));
        nn->next = head;
        head = nn;
    }
    fclose(fp);
    return (head);
}

/**
 * @brief Create a tickets object
 *
 * @param bookings
 * @param flights
 * @return int
 */

int create_tickets(bookingList *bookings, flightList *flights)
{
    int num_tickets = 0;
    fprintf(stdout, "Writing tickets:\n ");
    for (bookingList *blnp = bookings; blnp != NULL; blnp = blnp->next)
    {
        for (flightList *flnp = flights; flnp != NULL; flnp = flnp->next)
        {
            if (!strcmp(blnp->de, flnp->de) && !strcmp(blnp->dest, flnp->dest) && !strcmp(blnp->Date, flnp->Date) && !strcmp(blnp->Hour, flnp->Hour))
            {
                int row = 0, seat = 0;
                if (allocate_seat(flnp, blnp, &row, &seat))
                {
                    fprintf(stdout, "[ticket-%d.txt]", blnp->bookingNumm);
                    print_ticket(blnp, flnp, seat, row);
                    num_tickets++;
                }
            }
        }
    }
    fprintf(stdout, "\nCreated  %d tickets\n\n", num_tickets);
    return (num_tickets);
};

/**
 * @brief Function to loop throught all seat class and allocate seat
 * @param flight
 * @param bookingNum
 * @param row
 * @param seat
 * @return int
 */
int allocate_seat(flightList *flight, bookingList *bookingNumm, int *row, int *seat)
{
    int sn = 0;
    int rn = 0;
    if (strcmp("first", bookingNumm->seatClass) == 0)
    {
        for (int p = 0; p < flight->firstClass * 7; p++)
        {
            if (flight->fs[p] == 0)
            {
                flight->fs[p] = 1;
                sn = p + 1;
                rn = (int)p / 7 + 1;
                break;
            }
        }
    }
    if (strcmp("business", bookingNumm->seatClass) == 0)
    {
        for (int p = 0; p < flight->businessClass * 7; p++)
        {
            if (flight->bc[p] == 0)
            {
                flight->bc[p] = 1;
                sn = p + flight->firstClass * 7 + 1;
                rn = flight->firstClass + (int)p / 7 + 1;
                break;
            }
        }
    }
    if (strcmp("economy", bookingNumm->seatClass) == 0)
    {
        for (int p = 0; p < flight->economyClass * 7; p++)
        {
            if (flight->ec[p] == 0)
            {
                flight->ec[p] = 1;
                sn = p + flight->firstClass * 7 + flight->businessClass * 7 + 1;
                rn = flight->firstClass + flight->businessClass + (int)p / 7 + 1;
                break;
            }
        }
    }
    if (rn == 0 || sn == 0)
    {
        fprintf(stdout, "did not find class \"%s\" for bookingNum on this plane\n", bookingNumm->seatClass);
        return (0);
    }
    *row = rn;
    *seat = sn;
    return (1);
}
/**
 * @brief Function to output tickets to seperate .txt files
 *
 * @param blnp
 * @param flnp
 * @param seat
 * @param row
 */

void print_ticket(bookingList *blnp, flightList *flnp, int seat, int row)
{
    char filename[255];
    sprintf(filename, "ticket-%d.txt", blnp->bookingNumm);
    FILE *fp = fopen(filename, "w");
    if (fp)
    {
        fprintf(fp, "BOOKING:%d\n", blnp->bookingNumm);
        fprintf(fp, "FLIGHT:%d deARTURE:%s destTINATION: %s %s %s\n", flnp->flightNumm, flnp->de, flnp->dest, flnp->Date, flnp->Hour);
        fprintf(fp, "PASSENGER %s %s\n", blnp->fName, blnp->lName);
        fprintf(fp, "CLASS: %s\n", blnp->seatClass);
        fprintf(fp, "ROW %d SEAT %d\n\n", row, seat);
        fclose(fp);
    }
}

/**
 * main entry point of the program.
 * @param[in] argc Number of command line arguments.
 * @param[in] argv An array of pointers to null terminated arrays of characters (command line arguments).
 * argv[1] should be a filename of the flightdata file in csv format.
 * argv[2] should be a filename of the booking file in csv format.
 * @param[out] returns 0 on success.
 */
int main(int argc, char *argv[])
{
    if (argc == 3)
    {
        bookingList *bookings;
        flightList *flights;
        bookings = read_bookings(argv[1]);
        flights = read_flights(argv[2]);

        int makeTicket = create_tickets(bookings, flights);
        return (makeTicket);
    }
    else
    {
        return (-1);
    }
    return 0;
}
