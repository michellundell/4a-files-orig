/**
 * @file 4a-tickets.c
 * @author Daniel Örhill (daniel.orhill@studerande.movant.se)
 * @brief This program takes flight and booking information and
 * create files with tickets.
 * @version 2
 * @date 2022-10-28
 *
 * @copyright Copyright (c) 2022
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Declare struct for flight data
 *
 */
typedef struct flightData
{
    int flightN, fMax, bMax, eMax, fCurrent, bCurrent, eCurrent;
    char depart[4], arrive[4], date[11], time[6];
    struct flightData *next;
} flights;

/**
 * @brief Declare struct for booking data
 *
 */
typedef struct bookingData
{
    int bookingN;
    char date[11], time[6], depart[4], arrive[4], class[10], firstName[20], lastName[20];
    struct bookingData *next;
} bookings;

/**
 * @brief Declare function prototypes
 */
void sortedInsert(bookings **, bookings *);
void insertSort(bookings **);
void cancelFlights(flights **);
void seatingMap(flights *);
void printTicket(bookings *, flights *);

/**
 * main entry point of the program.
 * @param[in] argc Number of command line arguments.
 * @param[in] argv An array of pointers to null terminated arrays of characters (command line arguments).
 *            argv[1] should be a filename of the flight data file in csv format.
 *            argv[2] should be a filename of the booking data file in csv format.
 * @param[out] returns 0 on success.
 */
int main(int argc, char **argv)
{
    if (argc != 3)
    {
        fprintf(stderr, "No file given as an argument!\n");
        fprintf(stderr, "Syntax:");
        fprintf(stderr, "\t%s {filename} {filename}\n", argv[0]);
        fflush(stderr);
        exit(-1);
    }

    /**
     * @brief Open flight data file
     * exit program if file could not be opened
     */
    FILE *fp = fopen(argv[1], "r");

    if (!fp)
    {
        fprintf(stderr, "Could not open file %s", argv[1]);
        exit(-1);
    }

    /**
     * @brief Create head of flight list
     *
     */
    flights *flightList = (flights *)malloc(sizeof(flights));
    flightList->next = NULL;

    /**
     * @brief Scan in flight data into linked list
     *
     */
    while (fscanf(fp, "%d,%[^,],%[^,],%[^,],%[^,],%d,%d,%d\n", &flightList->flightN, flightList->depart, flightList->arrive, flightList->date, flightList->time, &flightList->fMax, &flightList->bMax, &flightList->eMax) == 8)
    {
        flightList->fCurrent = 1;
        flightList->bCurrent = flightList->fMax + 1;
        flightList->eCurrent = flightList->fMax + flightList->bMax + 1;
        flights *newFlight = (flights *)malloc(sizeof(flights));
        memset(newFlight, 0, sizeof(flights));
        newFlight->next = flightList;
        flightList = newFlight;
    }

    /**
     * @brief Close file and free unused memory
     *
     */
    fclose(fp);
    flights *tempF = flightList;
    flightList = flightList->next;
    free(tempF);

    /**
     * @brief Open booking data file
     * exit program if file could not be opened
     */
    fp = fopen(argv[2], "r");

    if (!fp)
    {
        fprintf(stderr, "Could not open file %s", argv[2]);
        exit(-1);
    }

    /**
     * @brief Create head of booking list
     *
     */
    bookings *bookingList = (bookings *)malloc(sizeof(bookings));
    bookingList->next = NULL;

    /**
     * @brief Scan in booking data into linked list
     *
     */
    while (fscanf(fp, "%d,%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^\n]\n", &bookingList->bookingN, bookingList->date, bookingList->time, bookingList->depart, bookingList->arrive, bookingList->class, bookingList->firstName, bookingList->lastName) == 8)
    {
        bookings *newBooking = (bookings *)malloc(sizeof(bookings));
        memset(newBooking, 0, sizeof(flights));
        newBooking->next = bookingList;
        bookingList = newBooking;
    }

    /**
     * @brief Close file and free unused memory
     *
     */
    fclose(fp);
    bookings *tempB = bookingList;
    bookingList = bookingList->next;
    free(tempB);

    fprintf(stdout, "\n\n");
    /**
     * @brief Sort booking list
     * @param[in] bookingList
     */
    insertSort(&bookingList);
    /**
     * @brief Print out tickets
     * @param[in] bookingList
     * @param[in] flightList
     */
    printTicket(bookingList, flightList);
    /**
     * @brief Cancel flights with no passengers
     * Remove such flights from flight linked list
     */
    cancelFlights(&flightList);
    /**
     * @brief Print out seating chart
     * 
     */
    seatingMap(flightList);
    return 0;
}

/**
 * @brief Define function to be used in another function to sort list
 * Code I found online
 * @param head Booking head linked list
 * @param newNode Booking current list to sort
 */
void sortedInsert(bookings **head, bookings *newNode)
{
    bookings dummy;
    bookings *current = &dummy;
    dummy.next = *head;

    while (current->next != NULL && current->next->bookingN < newNode->bookingN)
    {
        current = current->next;
    }

    newNode->next = current->next;
    current->next = newNode;
    *head = dummy.next;
}

/**
 * @brief Define function prototype to used to sort the booking list
 * Code I found online
 * @param head Booking head linked list
 */
void insertSort(bookings **head)
{
    bookings *result = NULL;
    bookings *current = *head;
    bookings *next;

    while (current != NULL)
    {
        next = current->next;

        sortedInsert(&result, current);
        current = next;
    }

    *head = result;
}

/**
 * @brief Define function to cancel flights with no passengers
 *
 * @param fList Flights list head
 */
void cancelFlights(flights **fList)
{
    flights dummy;
    flights *current = &dummy;
    dummy.next = *fList;

    FILE *cancel = fopen("cancelled-flights.txt", "w");

    if (!cancel)
    {
        fprintf(stderr, "Could not open cancelled flights file");
    }

    for (flights *nodeF = &dummy; nodeF != NULL; nodeF = nodeF->next)
    {
        if (nodeF->next->fCurrent == 1 && nodeF->next->bCurrent == nodeF->next->fMax + 1 && nodeF->next->eCurrent == nodeF->next->fMax + nodeF->next->bMax + 1)
        {
            fprintf(cancel, "%d,%s,%s,%s,%s,%d,%d,%d is cancelled\n", nodeF->next->flightN, nodeF->next->depart, nodeF->next->arrive, nodeF->next->date, nodeF->next->time, nodeF->next->fMax, nodeF->next->bMax, nodeF->next->eMax);
            flights *temp = nodeF->next->next;
            nodeF->next = temp;
        }
    }

    fclose(cancel);
}

/**
 * @brief Define function to print out a txt file with seat chart of flights that are not cancelled
 * 
 * @param fList Flight List head
 */
void seatingMap(flights *fList)
{
    FILE *seatMap = fopen("seating-report.txt", "w");

    for (flights *nodeF = fList; nodeF != NULL; nodeF = nodeF->next)
    {
        int row = 1;
        int count = 0;
        fprintf(seatMap, "Flight %d, Departure %s, Destination %s, Date %s, Time %s\n", nodeF->flightN, nodeF->depart, nodeF->arrive, nodeF->date, nodeF->time);
        fprintf(seatMap, "first class\n");
        for (int seat = 1; seat <= nodeF->fMax; seat++)
        {
            if (seat > 7 * row)
            {
                row++;
                fprintf(seatMap, "\n");
                count = 0;
            }

            if (seat < nodeF->fCurrent)
            {
                fprintf(seatMap,"[1]");
            }
            else
            {
                fprintf(seatMap,"[0]");
            }
            count++;
            if (count == 2 || count == 5)
            {
                fprintf(seatMap," ");
            }
        }
        fprintf(seatMap,"\n");
        fprintf(seatMap, "bussiness class");
        for (int seat = nodeF->fMax+1; seat <= nodeF->bMax; seat++)
        {
            if (seat > 7 * row)
            {
                row++;
                fprintf(seatMap, "\n");
                count = 0;
            }

            if (seat < nodeF->bCurrent)
            {
                fprintf(seatMap,"[1]");
            }
            else
            {
                fprintf(seatMap,"[0]");
            }
            count++;
            if (count == 2 || count == 5)
            {
                fprintf(seatMap," ");
            }
        }
        fprintf(seatMap,"\n");
        fprintf(seatMap, "economy class");
        for (int seat = nodeF->bMax+1; seat <= nodeF->eMax; seat++)
        {
            if (seat > 7 * row)
            {
                row++;
                fprintf(seatMap, "\n");
                count = 0;
            }

            if (seat < nodeF->eCurrent)
            {
                fprintf(seatMap,"[1]");
            }
            else
            {
                fprintf(seatMap,"[0]");
            }
            count++;
            if (count == 2 || count == 5)
            {
                fprintf(seatMap," ");
            }
        }
        fprintf(seatMap,"\n\n");
    }
    fclose(seatMap);
}

/**
 * @brief Define function to be used to print out tickets into txt files
 *
 * @param bList Booking head linked list
 * @param fList Flight head linked list
 */
void printTicket(bookings *bList, flights *fList)
{
    for (bookings *nodeB = bList; nodeB != NULL; nodeB = nodeB->next)
    {
        for (flights *nodeF = fList; nodeF != NULL; nodeF = nodeF->next)
        {
            if (!strcmp(nodeB->date, nodeF->date) && !strcmp(nodeB->time, nodeF->time) && !strcmp(nodeB->depart, nodeF->depart) && !strcmp(nodeB->arrive, nodeF->arrive))
            {

                char filename[20];
                sprintf(filename, "ticket-%d.txt", nodeB->bookingN);
                int seat;
                int row = 1;
                FILE *ticket = fopen(filename, "w");

                if (!ticket)
                {
                    fprintf(stderr, "Could not open ticket file");
                }

                if (strcmp(nodeB->class, "first") == 0)
                {
                    seat = nodeF->fCurrent;
                    nodeF->fCurrent++;
                }
                else if (strcmp(nodeB->class, "business") == 0)
                {
                    seat = nodeF->bCurrent;
                    nodeF->bCurrent++;
                }
                else if (strcmp(nodeB->class, "economy") == 0)
                {
                    seat = nodeF->eCurrent;
                    nodeF->eCurrent++;
                }

                while (seat > 7 * row)
                {
                    row++;
                }

                fprintf(ticket, "BOOKING: %d\nFLIGHT: %d DEPARTURE: %s DESTINATION: %s %s %s\nPASSENGER: %s %s\nCLASS: %s\nROW: %d SEAT: %d", nodeB->bookingN, nodeF->flightN, nodeB->depart, nodeB->arrive, nodeB->date, nodeB->time, nodeB->firstName, nodeB->lastName, nodeB->class, row, seat);
                fclose(ticket);
            }
        }
    }
}