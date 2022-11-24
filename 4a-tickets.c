/**
 * filename: 4a-tickets.c
 * user: Ali karimi
 * Email: ali_karimi5025@outlook.com
 * @brief this program generates tickets based on CSV data.
 * @version 0.1
 * date 2022-11-24
 *
 * copyright  (c) 2022
 *
 * /

 /**
  * flight node containing all variables from flight.csv
  *
  */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct flight_list
{
    int flightNum;            /* !< the flight number */
    char dep[20];             /* !< Departure airport code */
    char des[20];             /* !< Destination airport code */
    char date_dep[20];        /* !< Date departure */
    char time_dep[20];        /* !< Time departure */
    int num_firstclass;       /* !< Number of first class rows */
    int *fs;                  /* !< An array with flags of if a seat is taken or not. 1==occupied, 0==not occupied */
    int num_businessClass;    /* !< Number of Business class rows */
    int *bs;                  /* !< An array with flags of if a seat is taken or not. 1==occupied, 0==not occupied */
    int num_economyClass;     /* !< Number of Economy class rows */
    int *es;                  /* !< An array with flags of if a seat is taken or not. 1==occupied, 0==not occupied */
    struct flight_list *next; /* !< A pointer to the next flight information */

} flightList;
typedef struct booking_list
{
    int bookingNum;            /* !< The booking number */
    char flight_Date[15];      /* !< The departure date */
    char flight_time[15];      /* !< The departure time */
    char dep[10];              /* !< The departure airport */
    char des[10];              /* !< The destination airport */
    char seatClass[20];        /* !< The seat class */
    char firstName[25];        /* !< Firstname */
    char lastName[25];         /* !< Lastname */
    struct booking_list *next; /* !< a pointer to the next booking */
} bookingList;

/* !< Declaration av Functions */
flightList *read_flights(const char *filename);
bookingList *read_bookings(const char *filename);
int create_tickets(bookingList *bookings, flightList *flights);
int allocate_seat(flightList *flight, bookingList *bookingNum, int *row, int *seat);
void print_ticket(bookingList *blnp, flightList *flnp, int seat, int row);

/*
  This Function to scan flights.csv file and store all variables in link list
 */
flightList *read_flights(const char *filename)
{
    flightList fln;
    flightList *head = NULL;
    FILE *fp = fopen(filename, "r");

    while (fscanf(fp, "%d, %[^,], %[^,], %[^,], %[^,], %d, %d, %d\n", &fln.flightNum, fln.dep, fln.des, fln.date_dep, fln.time_dep, &fln.num_firstclass, &fln.num_businessClass, &fln.num_economyClass) == 8)
    {
        flightList *nn = malloc(sizeof(flightList));
        memcpy(nn, &fln, sizeof(flightList));
        nn->fs = malloc(fln.num_firstclass * sizeof(int) * 7); /** 7 seats in a row in firstclass */
        memset(nn->fs, 0, fln.num_firstclass * 7);
        nn->bs = malloc(fln.num_businessClass * sizeof(int) * 7); /** 7 seats in a row in businessclass */
        memset(nn->bs, 0, fln.num_businessClass * 7);
        nn->es = malloc(fln.num_economyClass * sizeof(int) * 7); /** 7 seats in a row in economyclass */
        memset(nn->es, 0, fln.num_economyClass * 7);
        nn->next = head;
        head = nn;
    }
    fclose(fp);
    return (head);
}
/*this funktion is reading booking file and adds it to link list*/
bookingList *read_bookings(const char *filename)
{
    bookingList fln;
    bookingList *head = NULL;
    FILE *fp = fopen(filename, "r");
    while (fscanf(fp, "%d, %[^,], %[^,], %[^,], %[^,], %[^,], %[^,], %[^\n]\n)", &fln.bookingNum, fln.flight_Date, fln.flight_time, fln.dep, fln.des, fln.seatClass, fln.firstName, fln.lastName) == 8)
    {
        bookingList *nn = malloc(sizeof(bookingList));
        memcpy(nn, &fln, sizeof(bookingList));
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
    fprintf(stdout, "writing tickets:\n ");
    for (bookingList *blnp = bookings; blnp != NULL; blnp = blnp->next)
    {
        for (flightList *flnp = flights; flnp != NULL; flnp = flnp->next)
        {
            if (!strcmp(blnp->dep, flnp->dep) && !strcmp(blnp->des, flnp->des) && !strcmp(blnp->flight_Date, flnp->date_dep) && !strcmp(blnp->flight_time, flnp->time_dep))
            {
                /* allocate seat and mark it booked */

                int row = 0;
                int seat = 0;
                if (allocate_seat(flnp, blnp, &row, &seat))
                {
                    fprintf(stdout, "[ticket-%d.txt]", blnp->bookingNum);
                    print_ticket(blnp, flnp, seat, row);
                    num_tickets++;
                }
            }
        }
    }
    fprintf(stdout, "\nCreated %d tickets\n\n", num_tickets);
    return (num_tickets);
}
/**
 * @brief Function to loop throught all seat class and allocate seat
 * @param flight
 * @param bookingNum
 * @param row
 * @param seat
 * @return int
 */
int allocate_seat(flightList *flight, bookingList *bookingNum, int *row, int *seat)
{
    int sn = 0;
    int rn = 0;
    if (strcmp("first", bookingNum->seatClass) == 0)
    {
        for (int p = 0; p < flight->num_firstclass * 7; p++)
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
    if (strcmp("business", bookingNum->seatClass) == 0)
    {
        for (int p = 0; p < flight->num_businessClass * 7; p++)
        {
            if (flight->bs[p] == 0)
            {
                flight->bs[p] = 1;
                sn = p + flight->num_firstclass * 7 + 1; /* humans usually start counting at 1 */
                rn = flight->num_firstclass + (int)p / 7 + 1;
                break;
            }
        }
    }
    if (strcmp("economy", bookingNum->seatClass) == 0)
    {
        for (int p = 0; p < flight->num_economyClass * 7; p++)
        {
            if (flight->es[p] == 0)
            {
                flight->es[p] = 1;
                sn = p + flight->num_firstclass * 7 + flight->num_businessClass * 7 + 1; /* humans usually start counting at 1 */
                rn = flight->num_firstclass + flight->num_businessClass + (int)p / 7 + 1;
                break;
            }
        }
    }
    if (rn == 0 || sn == 0)
    {
        fprintf(stdout, "did not find class\"%s\" for bookingNum on this plane.\n", bookingNum->seatClass);
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
    sprintf(filename, "ticket-%d.txt", blnp->bookingNum);
    FILE *fp = fopen(filename, "w");
    if (fp)
    {
        fprintf(fp, "BOOKING:%d\n", blnp->bookingNum);
        fprintf(fp, "FLIGHT:%d DEPARTURE:%s DESTINATION: %s %s %s\n", flnp->flightNum, flnp->dep, flnp->des, flnp->date_dep, flnp->time_dep);
        fprintf(fp, "PASSENGER: %s %s\n", blnp->firstName, blnp->lastName);
        fprintf(fp, "CLASS: %s\n", blnp->seatClass);
        fprintf(fp, "ROW %d SEAT %d\n\n", row, seat);
        fclose(fp);
    }
}

/**
 * main entry point of the program.
 * @param[in] argc Number of command line arguments.
 * @param[in] argv An array of pointers to null terminated arrays of characters (command line arguments).
 *            argv[1] should be a filename of the flightdata file in csv format.
 *            argv[2] should be a filename of the booking file in csv format.
 * @param[out] returns 0 on success.
 */
int main(int argc, char *argv[])
{
    if (argc = 3)
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
