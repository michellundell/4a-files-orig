#include "module.h"
#include <string.h>
#include <stdlib.h>

// READ INDATA FLIGHTS FILE AND SAVE IT IN A FLIGHTS LIST
FlightListNode *read_flights(const char *filename)
{
    FlightListNode fln, *head = NULL;
    FILE *fp = fopen(filename, "r");
    while (fscanf(fp, "%d,%[^,],%[^,],%[^,],%[^,],%d,%d,%d\n", &fln.flightno, fln.dep, fln.des, fln.datestr, fln.timestr, &fln.nfs, &fln.nbs, &fln.nes) == 8)
    {
        fprintf(stdout, "%d,%s,%s,%s,%s,%d,%d,%d\n", fln.flightno, fln.dep, fln.des, fln.datestr, fln.timestr, fln.nfs, fln.nbs, fln.nes);

        FlightListNode *nn = malloc(sizeof(FlightListNode));
        memcpy(nn, &fln, sizeof(FlightListNode));
        nn->fs = malloc(fln.nfs * sizeof(int) * 7); /** 7 is the number of seats in a row */
        memset(nn->fs, 0, fln.nfs * 7);
        nn->bs = malloc(fln.nbs * sizeof(int) * 7); /** 7 is the number of seats in a row */
        memset(nn->bs, 0, fln.nbs * 7);
        nn->es = malloc(fln.nes * sizeof(int) * 7); /** 7 is the number of seats in a row */
        memset(nn->es, 0, fln.nes * 7);
        nn->next = head;
        head = nn;
    }
    fclose(fp);
    return (head);
}

// READ INDATA BOOKING FILE AND SAVE IT IN A BOOKING LIST.

BookingListNode *read_bookings(const char *filename)
{
    BookingListNode fln, *head = NULL;
    FILE *fp = fopen(filename, "r");

    while ((fscanf(fp, "%d,%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,\n]", &fln.booking, fln.datestr, fln.timestr, fln.dep, fln.des, fln.class, fln.fname, fln.lname)) == 8)
    {
        fprintf(stdout, "%d,%s,%s,%s,%s,%s,%s,%s\n", fln.booking, fln.datestr, fln.timestr, fln.dep, fln.des, fln.class, fln.fname, fln.lname);
        BookingListNode *bb = malloc(sizeof(BookingListNode)); // New node of size BookingListNode size
        memcpy(bb, &fln, sizeof(BookingListNode));
        bb->next = head; // swap address of head and next node
        head = bb;
    }
    fclose(fp);
    return (head);
}

// FOR EACH BOOKING IN THE BOOKING LIST

int create_tickets(BookingListNode *bookings, FlightListNode *flights)
{
    int num_tickets = 0;
    fprintf(stdout, "Writing tickets: ");

    for (BookingListNode *blnp = bookings; blnp != NULL; blnp = blnp->next)
    {
        for (FlightListNode *flnp = flights; flnp != NULL; flnp = flnp->next)
        {
            /* find the flight for this booking */
            if (!strcmp(blnp->datestr, flnp->datestr) && !strcmp(blnp->timestr, flnp->timestr) && !strcmp(blnp->dep, flnp->dep) && !strcmp(blnp->des, flnp->des))
            {
                int row = 0, seat = 0;
                if (allocate_seat(flnp, blnp, &row, &seat))
                {
                    fprintf(stdout, "[ticket-%d.txt]\n", blnp->booking);
                    print_ticket(blnp, flnp, seat, row);
                    num_tickets++;
                }
            }
        }
    }
    return (num_tickets);
}

//## #Hint 8, example of realizing THEN ALLOCATE A SEAT &&MARK THE SEAT BOOKED

int allocate_seat(FlightListNode *flight, BookingListNode *booking, int *row, int *seat)
{
    int sn = 0;
    int rn = 0;
    if (strcmp("first", booking->class) == 0)
    {
        for (int p = 0; p < flight->nfs * 7; p++)
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
    if (strcmp("business", booking->class) == 0)
    {
        for (int p = 0; p < flight->nbs * 7; p++)
        {
            if (flight->bs[p] == 0)
            {
                flight->bs[p] = 1;
                sn = p + flight->nfs * 7 + 1; /* humans usually start counting at 1 */
                rn = flight->nfs + (int)p / 7 + 1;
                break;
            }
        }
    }
    if (strcmp("economy", booking->class) == 0)
    {
        for (int p = 0; p < flight->nes * 7; p++)
        {
            if (flight->es[p] == 0)
            {
                flight->es[p] = 1;
                sn = p + flight->nfs * 7 + flight->nbs * 7 + 1; /* humans usually start counting at 1 */
                rn = flight->nfs + flight->nbs + (int)p / 7 + 1;
                break;
            }
        }
    }
    if (rn == 0 || sn == 0)
    {
        fprintf(stdout, "did not find class \"%s\" for booking on this plane\n", booking->class);
        return (0);
    }
    *row = rn;
    *seat = sn;
    return (1);
}

// ## #Hint 9, example of realizing PRINT THE TICKET TO A TICKET FILE

void print_ticket(BookingListNode *blnp, FlightListNode *flnp, int seat, int row)

{
    char filename[255];
    sprintf(filename, "ticket-%d.txt", blnp->booking);
    FILE *fp = fopen(filename, "w");
    if (fp)
    {
        fprintf(fp, "BOOKING:%d\n", blnp->booking);
        fprintf(fp, "FLIGHT:%d DEPARTURE:%s DESTINATION: %s %s %s\n", flnp->flightno, flnp->dep, flnp->des, flnp->datestr, flnp->timestr);
        fprintf(fp, "PASSENGER %s %s\n", blnp->fname, blnp->lname);
        fprintf(fp, "CLASS: %s\n", blnp->class);
        fprintf(fp, "ROW %d SEAT %d\n\n", row, seat);
        fclose(fp);
    }
}

//## #1. Cancel all flights that does not have any passengers.
void cancel_flights(BookingListNode *bookings, FlightListNode *flights)
{
}