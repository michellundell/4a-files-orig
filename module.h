#ifndef MODULE_H
#define MODULE_H
#include <stdio.h>

// Data files macros
#define BOOKINGS "bookings.csv"
#define FLIGHTS "flights.csv"

// create data structures that map the indata
typedef struct flight_list_node
{
    int flightno;                  /* !< The flight number */
    char dep[20];                  /* !< Departure airport code */
    char des[20];                  /* !< Destination airport code */
    char datestr[20];              /* !< Date departure*/
    char timestr[20];              /* !< time departure*/
    int nfs;                       /* !< Number of First class rows */
    int *fs;                       /* !< An array with flags of if
                 a seat is taken or not. 1==occupied, 0==not occupied */
    int nbs;                       /* !< Number of Business class rows */
    int *bs;                       /* !< An array with flags of if a seat is taken or not. 1==occupied, 0==not occupied */
    int nes;                       /* !< Number of Economy class rows */
    int *es;                       /* !< An array with flags of if a seat is taken or not. 1==occupied, 0==not occupied */
    struct flight_list_node *next; /* !< A pointer to the next flight information */
} FlightListNode;

typedef struct booking_list_node
{
    int booking;                    /* !< The booking number */
    char datestr[15];               /* !< The departure date */
    char timestr[15];               /* !< The departure time */
    char dep[10];                   /* !< The departure airport */
    char des[10];                   /* !< The destination airport */
    char class[20];                 /* !< The seat class */
    char fname[25];                 /* !< Firstname */
    char lname[25];                 /* !< Lastname */
    struct booking_list_node *next; /* !< a pointer to the next booking */
} BookingListNode;

FlightListNode *read_flights(const char *filename);

BookingListNode *read_bookings(const char *filename);

int create_tickets(BookingListNode *bookings, FlightListNode *flights);

int allocate_seat(FlightListNode *flight, BookingListNode *booking, int *row, int *seat);

void print_ticket(BookingListNode *blnp, FlightListNode *flnp, int seat, int row);

void cancel_flights(BookingListNode *bookings, FlightListNode *flights);

#endif