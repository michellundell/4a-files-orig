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

/**
 * @brief 
 * 
 */
typedef struct flightsInfo {
	int flNum;
	char dep[5];
	char des[5];
	char date[12];
	char time[7];
	int fSeats;
	int bSeats;
	int eSeats;
} FInfo;

/**
 * @brief 
 * 
 */
typedef struct bookingInfo {
	int bNum;
	char date[12];
	char time[7];
	char dep[5];
	char des[5];
	char sClass[10];
	char fName[20];
	char surname[20];
} BInfo;

/**
 * @brief 
 * 
 */
typedef struct flightsListNode {
	int key;
	FInfo data;
	struct flightsListNode *next;
} FLNode;

/**
 * @brief 
 * 
 */
typedef struct bookingListNode {
	int key;
	BInfo data;
	struct bookingListNode *next;
} BLNode;

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
	int ret;
	int i = 0;
	FILE *fpF = fopen("flights.csv", "r");
	FLNode *newFNode = (FLNode *)malloc(sizeof(FLNode));

	while ((ret = fscanf(fpF, "%d,%[^,],%[^,],%[^,],%[^,],%d,%d,%d\n", &newFNode->data.flNum, newFNode->data.dep, newFNode->data.des,
						 newFNode->data.date, newFNode->data.time, &newFNode->data.fSeats, &newFNode->data.bSeats, &newFNode->data.eSeats)) > 0)
	{
		//printf("ret=%d\n", ret);
		//printf("%d,%s,%s,%s,%s,%d,%d,%d\n", newFNode->data.flNum, newFNode->data.dep, newFNode->data.des, newFNode->data.date,
		//		newFNode->data.time, newFNode->data.fSeats, newFNode->data.bSeats, newFNode->data.eSeats);
		newFNode->key = i++;
		newFNode->next = fList;
		fList = newFNode;
		newFNode = (FLNode *)malloc(sizeof(FLNode));
	}
	fclose(fpF);
	free(newFNode);
	newFNode = NULL;

	for (FLNode *itF = fList; itF != NULL; itF = itF->next)
	{
		printf("Node id: %d\n", itF->key);
	}
	printf("\n");

	FILE *fpB = fopen("bookings.csv", "r");
	BLNode *newBNode = (BLNode *)malloc(sizeof(BLNode));
	i = 0;
	while ((ret = fscanf(fpB, "%d,%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%s\n", &newBNode->data.bNum, newBNode->data.date, newBNode->data.time,
						 newBNode->data.dep, newBNode->data.des, newBNode->data.sClass, newBNode->data.fName, newBNode->data.surname)) > 0)
	{
		//printf("ret=%d\n", ret);
		//printf("%d,%s,%s,%s,%s,%s,%s,%s\n", newBNode->data.bNum, newBNode->data.date, newBNode->data.time,
		//				 newBNode->data.dep, newBNode->data.des, newBNode->data.sClass, newBNode->data.fName, newBNode->data.surname);
		newBNode->key = i++;
		newBNode->next = bList;
		bList = newBNode;
		newBNode = (BLNode *)malloc(sizeof(BLNode));
	}
	fclose(fpB);
	free(newBNode);
	newBNode = NULL;

	for (BLNode *itB = bList; itB != NULL; itB = itB->next)
	{
		printf("Node id: %d\n", itB->key);
	}
	printf("\n");

	FLNode *temp = NULL;
	while (fList != NULL)
	{
		temp = fList;
		fList = fList->next;
		free(temp);
		temp = NULL;
	}

	BLNode *tmp = NULL;
	while (bList != NULL)
	{
		tmp = bList;
		bList = bList->next;
		free(tmp);
		tmp = NULL;
	}
	
	return 0;
}
