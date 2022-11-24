/**
 * @file main.c
 * @author Syed A G Kazmi (kazmisag@gmail.com)
 * @brief A program for a flight booking/ticketing system
 * @version 0.1
 * @date 2022-11-05
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "module.h"

int main(int argc, char **argv)
{
    if (argc == 3)
    {
        BookingListNode *bookings;
        FlightListNode *flights;
        int tickets = 0;
        flights = read_flights(FLIGHTS);

        bookings = read_bookings(BOOKINGS);

        fprintf(stdout, "Created  %d tickets\n\n", create_tickets(bookings, flights));
    }
    else
    {
        (void)printf("Error...\n");
        (void)printf("Please enter correct data files\n");
        exit(1);
    }

    return 0;
}

/* // argv[1] == BOOKINGS ? read_bookings(BOOKINGS) : read_flights(FLIGHTS);
    if (argv[1] == BOOKINGS && argv[2] == FLIGHTS)
    {
        read_bookings(BOOKINGS);
        read_bookings(FLIGHTS);
    }
    else if (argv[1] == FLIGHTS && argv[2] == BOOKINGS)
    {

    }
    else
        (void)printf("Please enter correct data file names");
        */