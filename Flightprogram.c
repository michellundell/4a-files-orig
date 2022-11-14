
/* READ INDATA FLIGHTS FILE AND SAVE IT IN A FLIGHTS LIST.
READ INDATA BOOKING FILE AND SAVE IT IN A BOOKING LIST.
FOR EACH BOOKING IN THE BOOKING LIST
    FIND THE FLIGHT THE BOOKING IS ON
    THEN ALLOCATE A SEAT 
    MARK THE SEAT BOOKED
    THEN PRINT THE TICKET TO A TICKET FILE*/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>



typedef struct flight_list_node {
int flightnumber;     /* The flight number */
char departurecode[20]; /*Departure airport code*/
char des [20];         /* Destination airport code*/
char datestr [20]; /* Date of departure */ 
char timestr [20]; /*departure time */
int nfs;  /*Number of first clas rows*/
int *fs;        /* An array of fllags of if a seat is taken or not */ 
int nbs;  /*Number of buissnes class rows*/
int *bs;         /* An array of flags of a seat is taken or not */ 
int nes;   /*Number of economy class rows*/
int *es;     /* An array with flasg of if a seat is taken or not */
struct flight_list_node *next;   /* !< A pointer to the next flight information */
} FlightListNode;


typedef struct booking_list_node {
int booking;  /* The booking number */
char datestr[15]; /* The departure date */ 
char timestr [15]; /*The departure time */ 
char dep [10];   /*The departure airport*/
char des [10];  /* The destination airport*/
char class [20]; /*The seat class*/
char firstname [25]; /* Firstname*/
char lastname [25];   /*Lastname*/
struct booking_list_node *next; /* A pointer to the next booking*/
} BookingListNode;

FlightListNode *read_flights(const char  *filename);
BookingListNode *read_booking (const char *filename);

int create_tickets (BookingListNode *booking, FlightListNode *flights);
int allocate_seat(FlightListNode *flight, BookingListNode *booking, int *row, int *seat);
void print_ticket (BookingListNode *blnp, FlightListNode *flnp,int seat, int row);



FlightListNode *read_flights (const char *filename)
{
FlightListNode *head=NULL, fln; 

FILE *fp = fopen ("flights.csv","r");
if (!fp)
{
fprintf (stderr, "Error! Could not open the file: %s\n", filename);
return NULL;
}


while ( fscanf(fp, "%d, %[^,],%[^,],%[^,],%[^,],%d %d %d\n",&fln.flightnumber,fln.departurecode, fln.des, fln.datestr,fln.timestr, &fln.nfs,&fln.nbs,&fln.nes) == 8) {

FlightListNode *newnod = malloc (sizeof(FlightListNode));
memcpy (newnod,&fln,sizeof(FlightListNode));
newnod -> fs = malloc (fln.nfs*sizeof(int)*7); /* 7 seats in a row */
memset (newnod ->fs, 0,fln.nfs*7); 
newnod ->bs = malloc (fln.nbs*sizeof(int)*7);  /* 7 seast in a row */
memset (newnod ->bs, 0, fln.nbs*7);
newnod -> es = malloc (fln.nes*sizeof(int)*7); /* 7 seats in a row */
memset (newnod->es,0,fln.nes*7);
newnod ->next = head; 
head =newnod;
 }
return (head);

}

BookingListNode *read_bookings(const char *filename) {
BookingListNode bln,*head=NULL;
FILE *fp = fopen ("bookings.csv","r");
while ( fscanf(fp,"%d, %[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,]\n"& bln.booking,bln.datestr,bln.timestr,bln.dep,bln.des,bln.class,bln.firstname,bln.lastname)==8){
BookingListNode *newnod = malloc (sizeof(BookingListNode));
newnod -> next = NULL;
memcpy (newnod,&bln, sizeof(BookingListNode));
newnod ->next = head;
head =newnod;
}
return (head);

}
int allocate_seat[FlightListNode *flight BookingListNode *booking int *row,int *seat]
{
int sn = 0;
int rn = 0;
if (strcmp("first", booking->class) == 0) {
      for (int p=0;p <flight->nfs*7;p++) {
        if (flight ->fs[p] == 0) {
        flight ->fs[p] = 1;
        sn = p+1;
        rn = (int) p/7+1;
        break;
        }
      }
}
if (strcmp("buisness", booking -> class)==0 ) {
    for (int p=0; p<flight->nbs*7;p++) {
    if (flight ->bs[p] == 0) {
        flight ->bs [p] =1;
        sn = p+flight ->nfs*7+1;
        rn = flight -> nfs +(int) p/7+1;
        break;
       }
    }
}
if (strcmp ("economy",booking -> class) == 0 ) {
    for (int p=0;p<flight->nes*7; p++) {
        if (flight -> es [p] === 0 ) {
     flight ->es [p] = 1; 
     sn = p + flight -> nfs*7+flight->nbs*7+1;
     rn = flight -> nfs+(int)p/7+1
     break;
        }

    }

}
if ( rn == 0 || sn == 0) {
   fprintf (stout, "did not find class\"%s\" for booking on this plane\n", booking -> class);
   return (0);
}
*row = rn;
*seat =sn;
return (1);
}


int main (int argc, char **argv);
{
if (argc == 3)
}
 FlightListNode *flights = read_flights (argv[1]);
 BookingListNode *bookings = read_bookings (argv[2]);
 int num_processed = create_tickets (bookings, flights);
{ 
else 
}
printf ("Error!\n");
return (-1); 
{


int create_tickets (BookingListNode *bookings, FlightListNode *flights)
{

 int num_tickets = 0;
 fprintf (stdout, "Printing tickets: ");
 for (BookingListNode *blnp = bookings; blnp !=NULL; blnp=blnp->next)
{
if (!strcmp(blnp->departure, flp->departure) && !strcmp (blnp->date,flp->date) && !strcmp
{

int row =0, seat =0;
if (allocate_seat(flp,blp,&row,&seat)
{
 fprintf (stdout, "[ticket-%d.txt]"), blnp ->booking);
 print_ticket(blnp,flnp,seat,row);
 num_tickets++;  
}

}
fprintf (stdout, "Created %d tickets\n\n", num_tickets);
return (num_tickets);
}

for (FlightListNode *flnp = flights; flnp !=NULL; flnp=flnp->next) 
{
if (!strcmp (blnp->dep, flnp->dep) && !strcmp (blnp->des,flnp->des) && !strcmp(blnp->datestr) && !strcmp (blnp->timestr,flnp->timestr))
{

for (FlightListNode *flnp =flights; flnp !=NULL; flnp =flnp->next)
{
if !strcmp (blnp->dep,flnp->dep) &&
   !strcmp (blnp->des,flnp->des) &&
   !strcmp (blnp->datestr,flnp->datestr) &&
   !strcmp (blnp->timestr,flnp->timestr) 
} 

void print_ticket (BookingListNode *blnp,FlightListNode *flnp, int seat, int row)
{

  char filename [255];
  sprintf (filename,"ticket-%dtxt",blnp->booking);
  FILE *fp =fopen (filename, "w");
  if ( fp ) {
   fprintf (fp, "BOOKING:%d\n",blnp->booking);
   fprintf (fp, "FLIGHT:%d" DEPARTURE:%s DESTINATION: %s %s %s\n" flnp -> flightnumber, flnp ->departurecode, flnp->des,flnp-> datestr, flnp ->timestr");
   fprintf (fp, "PASSENGER %s %s\n", blnp->firstname, blnp->lastname);
   frptintf (fp, "CLASS: %s\n", blnp -> class);
   fprintf (fp, "ROW" %d "SEAT %d\n\n", row,seat);
   fclose (fp);    
 }
}