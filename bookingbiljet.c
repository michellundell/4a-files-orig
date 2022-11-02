#include <stdio.h>
#include <string.h>
#include <stdlib.h>




typedef struct Flights
{
    int flight_number;
    char departure[20];
    char destination[20];
    char date[20];
    char time[20];
    int fseats;
    int *f_seats;
    int bseats;
    int *b_seats;
    int eseats;
    int *e_seats;
    struct Flights *next;
  
}FlightsNode;

typedef struct Bookings
{
    int booking_number;
    char dep_date[20];
    char dep_time[20];
    char departure[20];
    char destination[20];
    char class[20];
    char firstName[25];
    char lastName[25];
    struct Bookings *next;
}BookingsNode;

int allocate_seat(FlightsNode *flights, BookingsNode *bookings , int *row, int *seat);
void print_ticket(BookingsNode *booking_node, FlightsNode *flight_node, int seat, int row);

FlightsNode* readFlightInformations(const char *src_path)
{
    FlightsNode flight, *head = NULL;
     FILE *src = fopen(src_path, "r");

     if(src == NULL){

        printf("Error: Could not open file");
        exit(-1);
     }


     while(fscanf(src,"%d,%[^,],%[^,],%[^,],%[^,],%d,%d,%d\n",&flight.flight_number,flight.departure,flight.destination,flight.date,flight.time,&flight.fseats,&flight.bseats,&flight.eseats) == 8){
        FlightsNode *n = malloc(sizeof(FlightsNode));
        memcpy(n, &flight, sizeof(FlightsNode));
        n->f_seats = malloc(flight.fseats*sizeof(int)*7);
        memset(n->f_seats,0,flight.fseats);
        n->b_seats = malloc(flight.bseats*sizeof(int)*7);
        memset(n->b_seats,0,flight.bseats);
        n->e_seats = malloc(flight.eseats*sizeof(int)*7);
        memset(n->e_seats,0,flight.eseats);
        n->next = head;
        head = n;
     }

     return(head);
      
     }

BookingsNode* readBookingsInformations(const char *src_path)
{
    BookingsNode booking, *head=NULL;
    FILE *src = fopen (src_path, "r");

    if(src == NULL){

        printf("Error: Could not open file");
        exit(-1);
     }

    while(fscanf(src, "%d,%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,]\n", &booking.booking_number,booking.dep_date,booking.dep_time, booking.departure, booking.destination, booking.class, booking.firstName, booking.lastName) == 8){
        BookingsNode *m = malloc(sizeof(BookingsNode));
        memcpy(m, &booking , sizeof(BookingsNode));
        m->next = head;
        head = m;
    }
    return(head);
}

int createTickets (BookingsNode *bookings , FlightsNode *flights)
{
    int numbOfTickets = 0;
    fprintf(stdout,"Writing tickets: ");
    for(BookingsNode *booking_node = bookings ; booking_node != NULL ; booking_node = booking_node->next)
    {
       for(FlightsNode *flights_node = flights; flights_node != NULL; flights_node = flights_node->next){
        if(!strcmp(booking_node->departure,flights_node->departure) && !strcmp(booking_node->destination, flights_node->destination) && !strcmp(booking_node->dep_date, flights_node->date) && !strcmp(booking_node->dep_time, flights_node->time))
        {
            int row = 0 , seat = 0;
            if(allocate_seat(flights_node,booking_node, &row,&seat))
            {
                fprintf(stdout,"[ticket-%d.txt]", booking_node->booking_number);
                print_ticket(booking_node,flights_node,row,seat);
                numbOfTickets++;
            }

        }
    }

    fprintf(stdout, "Created %d tickets\n\n", numbOfTickets);
    return(numbOfTickets);


}

int allocate_seat(FlightsNode *flights, BookingsNode *bookings , int *row, int *seat)
{
    int mySeat = 0;
    int myRow = 0;

    if(strcmp("first", bookings->class) == 0)
    {
        for(int i=0 ; i < flights->fseats*7 ; i++)
        {
            if(flights->f_seats[i] == 0)
            {
                flights->f_seats[i] = 1;
                mySeat = i + 1;
                myRow =(int)i/7+1;
                break;
            }
        }
    }
    if(strcmp("business", bookings->class ) == 0)
    {
        for(int i =0; i< flights->bseats*7; i++)
        {
            if(flights->b_seats[i] == 0)
            {
                flights->b_seats[i] = 1;
                mySeat = i + flights->bseats*7+1;
                myRow = flights->bseats+(int)i/7+1;
                break;
            }
        }
    }
    if(strcmp("economy", bookings->class) == 0)
    {
        for (int i = 0; i < flights->eseats*7; i++)
        {
            if (flights->e_seats[i] == 0)
            {
                flights->e_seats[i] = 1;
                mySeat = i + flights->eseats*7+ flights->eseats*7 +1;
                myRow = flights->eseats + flights->eseats+(int)i/7+1;
                break;
            }
        }
    }
    if(myRow == 0 || mySeat == 0)
    {
        fprintf(stdout, "Did not find class  %s for booking on this plane \n", bookings->class);
        return(0);
    }

    *row = myRow;
    *seat = mySeat;
    return(1);
}

void print_ticket(BookingsNode *booking_node, FlightsNode *flight_node, int seat, int row)
{

    char myFile[255];
    sprintf(myFile , "ticket-%d.txt", booking_node->booking_number);
    FILE *f = fopen(myFile , "w");
    if (f){
        fprintf(f,"BOOKING: %d\n", booking_node->booking_number);
        fprintf(f, "FLIGHT: %d DEPARTURE: %s DESTINATION : %s %s %s\n", flight_node->flight_number, flight_node->departure , flight_node->destination , flight_node->date , flight_node->time);
        fprintf(f, "PASSAGER %s %s \n", booking_node -> firstName , booking_node -> lastName);
        fprintf(f, "ROW %d SEAT %d\n\n",row,seat);
        fprintf(f, "CLASS %s\n", booking_node->class);
        fclose(f);
    }
}


int main(int argc, char **argv)
{

    FlightsNode *flights= NULL;
    BookingsNode *bookings = NULL;

    flights = readFlightInformations(argv[1]);
    bookings = readBookingsInformations(argv[2]);
    
    if(flights && bookings)
    {

        createTickets(flights,bookings);
    }

    return 0;
}



