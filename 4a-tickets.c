/**
 * @file 4a-tickets.c
 * @author Daniel Örhill (daniel.orhill@studerande.movant.se)
 * @brief This program takes flight and booking information and
 * create files with tickets.
 * @version 1
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
 * @brief Declare function to be used in another function to sort list
 * 
 * @param head Booking head linked list
 * @param newNode Booking current list to sort
 */
void sortedInsert(bookings **head, bookings *newNode);

/**
 * @brief Declare function prototype to used to sort the booking list
 * 
 * @param head Booking head linked list
 */
void insertSort(bookings **head);

/**
 * @brief Declare function protoype to be used to print out tickets into txt files
 * 
 * @param bList Booking head linked list
 * @param fList Flight head linked list
 */
void printTicket(bookings *bList, flights *fList);

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
     * @brief Create pointer to flight linked list
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
        flightList->eCurrent = flightList->bMax + 1;
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
     * @brief Create pointer to booking linked list
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
    insertSort(&bookingList);
    printTicket(bookingList, flightList);

    return 0;
}

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

void printTicket(bookings *bList, flights *fList)
{
    for (bookings *nodeB = bList; nodeB != NULL; nodeB = nodeB->next)
    {
        for (flights *nodeF = fList; nodeF != NULL; nodeF = nodeF->next)
        {
            if (strcmp(nodeB->date, nodeF->date) == 0)
            {
                if (strcmp(nodeB->time, nodeF->time) == 0)
                {
                    if (strcmp(nodeB->depart, nodeF->depart) == 0)
                    {
                        if (strcmp(nodeB->arrive, nodeF->arrive) == 0)
                        {
                            char filename[20];
                            sprintf(filename, "ticket-%d.txt", nodeB->bookingN);
                            int seat;
                            int row = 1;
                            FILE *ticket = fopen(filename, "w");
                            if (!ticket)
                            {
                                fprintf(stderr, "fopen");
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
        }
    }
}