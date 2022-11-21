/**
 * @brief This program takes flight and booking information and 
 * create files with tickets.
 *
 * @file 4a-tickets.c
 * @author Hussein
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>




typedef struct flight_list 
{
	int flightNumber; 
	int fSeatRow;
	int *fSeat;
	int bSeatRow;
	int *bSeat;
	int eSeatRow;
	int *eSeat;
	char departure[20];
	char destination[20];
	char date[20];
	char time[20];
	struct flight_list *next;
	
}flightList;

typedef struct booking_list
{
	int booking;
	char departure[20];
	char destination[20];
	char date[20];
	char time[20];
	char seatClass[20];
	char firstName[20];
	char lastName[20];
	struct booking_list *next;
	
}bookingList;

flightList* read_flights(const char *filename);
bookingList* read_booking(const char *filename);

void print_ticket(bookingList *blp, flightList *flp, int seat, int row);
int create_tickets(bookingList *booking, flightList *flights);
int allocate_seat(flightList *flight,bookingList *booking,int *row, int *seat);


flightList* read_flights(const char *filename)
{
		flightList fl, *head=NULL;

	FILE *fp = fopen(filename,"r");

	if (!fp)
	{
		fprintf(stderr, "Error! Could not open the file: %s\n", filename);
		return NULL;
	}
	while (fscanf(fp,"%d,%[^,],%[^,],%[^,],%[^,],%d,%d,%d\n", &fl.flightNumber, fl.departure, fl.destination, fl.date, fl.time, &fl.fSeatRow, &fl.bSeatRow, &fl.eSeatRow) == 8)
	{
		flightList *newnod = malloc(sizeof(flightList));
		memcpy(newnod, &fl,sizeof(flightList));

		newnod->fSeat = malloc(fl.fSeatRow*sizeof(int)*7);
		memset(newnod->fSeat,0,fl.fSeatRow);

		newnod->bSeat = malloc(fl.bSeatRow*sizeof(int)*7);
		memset(newnod->bSeat,0,fl.bSeatRow);

		newnod->eSeat = malloc(fl.eSeatRow*sizeof(int)*7);
		memset(newnod->eSeat,0,fl.eSeatRow);

		newnod->next = head;
		head= newnod;
	}

	return(head);
	
}

bookingList *read_booking(const char *filename)
{
	bookingList bl, *head=NULL;
	FILE *fp = fopen(filename,"r");
	if (!fp)
	
	{
		fprintf(stderr, "Error! Could not open the file: %s\n", filename);
		return NULL;
	}

	

	while (fscanf(fp,"%d,%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,'\n']\n", &bl.booking, bl.date, bl.time, bl.departure, bl.destination, bl.seatClass, bl.firstName, bl.lastName)==8)
	{
	bookingList *newNod = malloc(sizeof(bookingList));
	memcpy(newNod, &bl,sizeof(bookingList));
	newNod->next = head;
	head= newNod;
	}

	return(head);
}




int main(int argc, char **argv)

{
	if(argc == 3)
	{
		flightList *flights = read_flights(argv[1]);
		bookingList *bookings = read_booking(argv[2]);
		int num_processed = create_tickets(bookings,flights);
		
	}
	else
	{
       printf("error");
		return(-1);
	}

	return 0;
}


int create_tickets(bookingList *booking, flightList *flights)
{

	int numTickets = 0;
	fprintf(stdout,"Writning tickets: ");
	for (bookingList *blp = booking; blp != NULL; blp=blp->next)
	{
		
		for (flightList *flp = flights; flp != NULL; flp=flp->next)
		{

			if(!strcmp(blp->departure,flp->departure) && !strcmp(blp->destination,flp->destination) && !strcmp(blp->date,flp->date) && !strcmp(blp->time,flp->time))
			{
				int row = 0, seat = 0;
				if (allocate_seat(flp,blp,&row,&seat))
				{
					fprintf(stdout,"[ticket-%d.txt]", blp->booking);
					print_ticket(blp,flp,seat,row);
					numTickets++;
				}
				
			}

		}
		
	}
	
	fprintf(stdout,"Created  %d tickets\n\n",numTickets);
	return(numTickets);

}



int allocate_seat(flightList *flight,bookingList *booking,int *row, int *seat)
{
	int sn = 0;
	int rn = 0;
	if (strcmp("first",booking->seatClass) == 0)
	{
		for (int p = 0; p<flight->fSeatRow * 7; p++)
		{
			if (flight->fSeat[p] == 0)
			{
				flight->fSeat[p] = 1;
				sn= p+1;
				rn=(int)p/7+1;
				break;
			}
		}
		
	}

	if (strcmp("business",booking->seatClass) == 0)
	{
		for (int p = 0; p < flight->bSeatRow * 7; p++)
		{
			if (flight->bSeat[p] == 0)
			{
				flight->bSeat[p] = 1;
				sn= p+flight->fSeatRow * 7 + 1;
				rn=flight->fSeatRow + (int)(p / 7) + 1;
				break;
			}
		}
	
	}
	if (strcmp("economy",booking->seatClass) == 0)
	{
		for (int p = 0; p < flight->eSeatRow * 7; p++)
		{
			if (flight->eSeat[p] == 0)
			{
				flight->eSeat[p] = 1;
				sn = p+flight->fSeatRow * 7 + flight->bSeatRow * 7 + 1;
				rn = flight->fSeatRow + flight->bSeatRow +(int) (p / 7) + 1;
				break;

			}
		}
	
	}
	if (rn == 0 || sn == 0)
	{
		fprintf(stdout,"didn't find class \"%s\" for booking on this plane\n",booking->seatClass);
		return (0);
	}
	*row = rn;
	*seat = sn;
	return (1);	
}



	void print_ticket(bookingList *blp, flightList *flp, int seat, int row)
	{
		char filename[255];
		sprintf(filename,"ticket-%d.txt", blp->booking);
		FILE *fp = fopen(filename, "w");
		if (fp)
		{
			fprintf(fp,"BOOKING:%d\n",blp->booking);
			fprintf(fp,"FLIGHT:%d DEPARTURE:%s DESTINATION: %s %s %s\n",flp->flightNumber,flp->departure,flp->destination,flp->date,flp->time);
			fprintf(fp,"PASSENGER %s %s\n",blp->firstName,blp->lastName);
			fprintf(fp,"CLASS: %s\n",blp->seatClass);
			fprintf(fp,"ROW %d SEAT %d\n\n",row,seat);
			
		}

	}

