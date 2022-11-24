/**
 * @file 4a-tickets.c
 * @author Joakim Englund (joakimenglund@protonmail.com)
 * @brief This program takes flight and booking information and 
 * create files with tickets.
 * @version 1.0
 * @date 2022-10-27
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief FLNode is a node in a linked list containing information about flights.
 * 
 */
typedef struct flightsInfoNode {
	int flNum;
	char dep[5];
	char des[5];
	char date[12];
	char time[7];
	int fRows;
	char *fSeatFlags;
	int bRows;
	char *bSeatFlags;
	int eRows;
	char *eSeatFlags;
	struct flightsInfoNode *next;
} FLNode;

/**
 * @brief BLNode is a node in a linked list containing information about bookings.
 * 
 */
typedef struct bookingInfoNode {
	int bNum;
	char date[12];
	char time[7];
	char dep[5];
	char des[5];
	char sClass[10];
	char fName[20];
	char surname[20];
	struct bookingInfoNode *next;
} BLNode;

FLNode* addFlights(const char *filename);
BLNode* addBookings(const char *filename);
void createTickets(FLNode *fList, BLNode *bList);
int allocateSeat(FLNode *flight, BLNode *booking, int *row, int *seat);
void createTicket(FLNode *flight, BLNode *booking, int row, int seat);
void deleteFList(FLNode *head);
void deleteBList(BLNode *head);

/**
 * main entry point of the program.
 * @param[in] argc Number of command line arguments.
 * @param[in] argv An array of pointers to null terminated arrays of characters (command line arguments).
 *            argv[1] should be a filename of the flightdata file in csv format.
 *            argv[2] should be a filename of the booking file in csv format.
 * @param[out] returns 0 on success.
*/
int main(int argc, char **argv)
{
	// Create the linked lists.
	FLNode *fList = NULL;
	BLNode *bList = NULL;

	// Add elements to the linked lists and create tickets.
	fList = addFlights(argv[1]);
	bList = addBookings(argv[2]);
	// Create tickets if both lists were created properly.
	if (fList && bList)
	{
		createTickets(fList, bList);
	}

	// Clean up properly.
	deleteFList(fList);
	fList = NULL;
	deleteBList(bList);
	bList = NULL;
	
	return 0;
}

/**
 * @brief This function reads data about flights from the file given as a
 * parameter, adds it to a linked list, and returns the linked list.
 * 
 * @param filename Name of file to read data from.
 * @return FLNode* Returns a pointer to a linked list on success.
 */
FLNode* addFlights(const char *filename)
{
	FILE *fp = fopen(filename, "r");
	if (!fp)
	{
		fprintf(stderr, "Error! Could not open the file: %s\n", filename);
		return NULL;
	}

	FLNode newNode, *fList = NULL;

	// Add nodes to the list while fscanf() reads 8 items from the file.
	while (fscanf(fp, "%d,%[^,],%[^,],%[^,],%[^,],%d,%d,%d\n", &newNode.flNum, newNode.dep, newNode.des,
				  newNode.date, newNode.time, &newNode.fRows, &newNode.bRows, &newNode.eRows) == 8)
	{
		FLNode *newFNode = (FLNode *)malloc(sizeof(FLNode));
		memcpy(newFNode, &newNode, sizeof(FLNode));
		newFNode->fSeatFlags = malloc(newNode.fRows*sizeof(char)*7);
		memset(newFNode->fSeatFlags, 0, newNode.fRows*sizeof(char)*7);
		newFNode->bSeatFlags = malloc(newNode.bRows*sizeof(char)*7);
		memset(newFNode->bSeatFlags, 0, newNode.bRows*sizeof(char)*7);
		newFNode->eSeatFlags = malloc(newNode.eRows*sizeof(char)*7);
		memset(newFNode->eSeatFlags, 0, newNode.eRows*sizeof(char)*7);
		newFNode->next = fList;
		fList = newFNode;
	}
	fclose(fp);
	return fList;
}

/**
 * @brief This function reads data about bookings from the file given as a
 * parameter, adds it to a linked list, and returns the linked list.
 * 
 * @param filename Name of file to read data from.
 * @return BLNode* Returns a pointer to a linked list on success.
 */
BLNode* addBookings(const char *filename)
{
	FILE *fp = fopen(filename, "r");
	if (!fp)
	{
		fprintf(stderr, "Error! Could not open the file: %s\n", filename);
		return NULL;
	}

	BLNode newNode, *bList = NULL;

	// Add nodes to the list while fscanf() reads 8 items from the file.
	while (fscanf(fp, "%d,%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%s\n", &newNode.bNum, newNode.date, newNode.time,
						 newNode.dep, newNode.des, newNode.sClass, newNode.fName, newNode.surname) == 8)
	{
		BLNode *newBNode = (BLNode *)malloc(sizeof(BLNode));
		memcpy(newBNode, &newNode, sizeof(BLNode));
		newBNode->next = bList;
		bList = newBNode;
	}
	fclose(fp);
	return bList;
}

/**
 * @brief This function contains the logic to match bookings to flights and use
 * that information to create tickets.
 * 
 * @param fList A linked list containing information about flights.
 * @param bList a linked list containing information about bookings.
 */
void createTickets(FLNode *fList, BLNode *bList)
{
	for (BLNode *BLIt = bList; BLIt != NULL; BLIt = BLIt->next)
	{
		int foundMatch = 0;
		for (FLNode *FLIt = fList; FLIt != NULL; FLIt = FLIt->next)
		{
			// If we find a matching flight for the booking..
			if (!strcmp(FLIt->dep, BLIt->dep) && !strcmp(FLIt->des, BLIt->des) && !strcmp(FLIt->date, BLIt->date) && !strcmp(FLIt->time, BLIt->time))
			{
				int row = 0, seat = 0;
				// Try to allocate a seat.
				if (allocateSeat(FLIt, BLIt, &row, &seat))
				{
					// If successful, create a ticket and flag that we got a match.
					createTicket(FLIt, BLIt, row, seat);
					foundMatch = 1;
				}
				else
				{
					// If unsuccessful, print an error message.
					fprintf(stderr, "Error! Could not find a seat in %s class for booking #%d\n", BLIt->sClass, BLIt->bNum);
				}
				break; // Break out of the flights-list loop when we find a match with a booking.
			}
		}
		if (!foundMatch)
		{
			// If we reach this part, there is no matching flight for the current booking.
			fprintf(stderr, "Error! Could not find a matching flight for booking #%d\n", BLIt->bNum);
		}
	}
}

/**
 * @brief 
 * 
 * @param flight A linked list containing information about flights. Only uses
 * the first element in the list.
 * @param booking A linked list containing information about bookings. Only uses
 * the first element in the list.
 * @param row A reference to an integer variable used to store what row is
 * booked. Used as a return value.
 * @param seat A reference to an integer variable used to store what seat is
 * booked. Used as a return value.
 * @return int Returns 0 on unsuccessful allocating of a seat, and a positive
 * integer if successful.
 */
int allocateSeat(FLNode *flight, BLNode *booking, int *row, int *seat)
{
	if (!strcmp(booking->sClass, "first"))
	{
		// Try to allocate a seat in first class
		for (int i = 0; i < flight->fRows * 7; i++)
		{
			if (flight->fSeatFlags[i] == 0)
			{
				flight->fSeatFlags[i] = 1;
				*row = (int)(i / 7) + 1;
				*seat = i + 1;
				break; // break out of the loop when we are successful.
			}
		}
	}
	else if (!strcmp(booking->sClass, "business"))
	{
		// Try to allocate a seat in business class
		for (int i = 0; i < flight->bRows * 7; i++)
		{
			if (flight->bSeatFlags[i] == 0)
			{
				flight->bSeatFlags[i] = 1;
				*row = flight->fRows + (int)(i / 7) + 1;
				*seat = flight->fRows * 7 + i + 1;
				break; // break out of the loop when we are successful.
			}
		}
	}
	else
	{
		// Try to allocate a seat in economy class
		for (int i = 0; i < flight->eRows * 7; i++)
		{
			if (flight->eSeatFlags[i] == 0)
			{
				flight->eSeatFlags[i] = 1;
				*row = flight->bRows + flight->fRows + (int)(i / 7) + 1;
				*seat = flight->bRows * 7 + flight->fRows * 7 + i + 1;
				break; // break out of the loop when we are successful.
			}
		}
	}

	return *seat; // seat will be 0 if a seat wasn't assigned, which is all we care about.
}

/**
 * @brief This function creates a ticket in form of a file.
 * 
 * @param flight A linked list containing information about flights. Only uses
 * the first element in the list.
 * @param booking A linked list containing information about bookings. Only uses
 * the first element in the list.
 * @param row An integer containing what row the ticket is booked for.
 * @param seat An integer containing what seat the ticket is booked for.
 */
void createTicket(FLNode *flight, BLNode *booking, int row, int seat)
{
	char filename[20];
	sprintf(filename, "ticket-%d.txt", booking->bNum);
	FILE *fp = fopen(filename,"w");

	if(fp) {
		fprintf(fp,"BOOKING: %d\n", booking->bNum);
		fprintf(fp,"FLIGHT: %d DEPARTURE: %s DESTINATION: %s %s %s\n", flight->flNum, flight->dep, flight->des, flight->date, flight->time);
		fprintf(fp,"PASSENGER %s %s\n", booking->fName, booking->surname);
		fprintf(fp,"CLASS: %s\n", booking->sClass);
		fprintf(fp,"ROW %d SEAT %d\n", row, seat);
		fclose(fp);
	}
	else
	{
		fprintf(stderr, "Error! Could not create ticket with bookingnumber %d\n", booking->bNum);
	}
}

/**
 * @brief This function calls free() on every block of memory that was allocated
 * with malloc in a linked list of the FLNode type.
 * 
 * @param head Pointer to the linked list we want to delete properly.
 */
void deleteFList(FLNode *head)
{
	FLNode *temp = NULL;
	while (head != NULL)
	{
		temp = head;
		head = head->next;
		free(temp->fSeatFlags);
		free(temp->bSeatFlags);
		free(temp->eSeatFlags);
		free(temp);
	}
}

/**
 * @brief This function calls free() on every block of memory that was allocated
 * with malloc in a linked list of the BLNode type.
 * 
 * @param head Pointer to the linked list we want to delete properly.
 */
void deleteBList(BLNode *head)
{
	BLNode *temp = NULL;
	while (head != NULL)
	{
		temp = head;
		head = head->next;
		free(temp);
	}
}
