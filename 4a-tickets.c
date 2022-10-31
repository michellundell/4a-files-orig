/**
 * @file 4a-tickets.c
 * @author Joakim Englund (joakimenglund@protonmail.com)
 * @brief This program takes flight and booking information and 
 * create files with tickets.
 * @version 0.1
 * @date 2022-10-27
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief 
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
 * @brief 
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

FLNode* add_flights(const char *filename);
BLNode* add_bookings(const char *filename);
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
	FLNode *fList = NULL;
	BLNode *bList = NULL;

	fList = add_flights("flights.csv"); // argv[1]
	bList = add_bookings("bookings.csv"); // argv[2]

	for (FLNode *itF = fList; itF != NULL; itF = itF->next)
	{
		printf("Node id: %d\n", itF->flNum);
	}
	printf("\n");

	for (BLNode *itB = bList; itB != NULL; itB = itB->next)
	{
		printf("Node id: %d\n", itB->bNum);
	}
	printf("\n");

	deleteFList(fList);
	fList = NULL;
	deleteBList(bList);
	bList = NULL;
	
	return 0;
}

FLNode* add_flights(const char *filename)
{
	FILE *fp = fopen(filename, "r");
	FLNode newNode, *fList = NULL;

	while (fscanf(fp, "%d,%[^,],%[^,],%[^,],%[^,],%d,%d,%d\n", &newNode.flNum, newNode.dep, newNode.des,
				  newNode.date, newNode.time, &newNode.fRows, &newNode.bRows, &newNode.eRows) > 0)
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

BLNode* add_bookings(const char *filename)
{
	FILE *fp = fopen(filename, "r");
	BLNode newNode, *bList = NULL;

	while (fscanf(fp, "%d,%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%s\n", &newNode.bNum, newNode.date, newNode.time,
						 newNode.dep, newNode.des, newNode.sClass, newNode.fName, newNode.surname) > 0)
	{
		BLNode *newBNode = (BLNode *)malloc(sizeof(BLNode));
		memcpy(newBNode, &newNode, sizeof(BLNode));
		newBNode->next = bList;
		bList = newBNode;
	}
	fclose(fp);
	return bList;
}

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
