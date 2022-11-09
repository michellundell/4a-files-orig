/*bookingnumber,date,time,departure,destination,seatclass,firstname,surname*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct bInfo
{
    int bookN, fSeatsN, bSeatsN, eSeatsN, bTypeN; // tipe of flight booked
    char date[30], time[20], depa[20], desti[20], seatC[20], fName[30], sName[30];
    struct bInfo *nextB;
};

struct fInfo
{
    int fNum, fSeats, bSeats, eSeats, typeN; // type of flight
    char depa[20], desti[20], time[20], date[20];
    struct fInfo *nextF;
};

struct tycket
{
    int bNum, fNum, row, seatN;
    char depa[10], desti[10], date[20], time[10], fName[30], sName[30], seatC[15];
    struct tycket *nextT;
};

struct fInfo *iFly(char *text);
struct bInfo *iBooking(char *text);
void bType(struct fInfo *, struct bInfo *);
void pTyckets(struct fInfo *, struct bInfo *);
void deleteF(struct fInfo *, struct bInfo *);

// main
int main()
{

    struct fInfo *head = iFly("fInfo.txt");        // create a linked list of fligh
    struct bInfo *headB = iBooking("booking.txt"); // create a linked lis of bookings

    bType(head, headB);    // funktion that add booking type
    pTyckets(head, headB); // funktion that print the tyckets

    deleteF(head, headB);

    return 0;
}

// functions
struct fInfo *iFly(char *fileName)
{

    struct fInfo fF, *head = NULL;
    fF.typeN = 0;

    FILE *f = fopen(fileName, "r");
    if (!f)
    {
        fprintf(stderr, "Could not open file %s\n", fileName);
        exit(-1);
    }

    // read the information in a file 'f'
    while (fscanf(f, "%d,%[^,],%[^,],%[^,],%[^,],%d,%d,%d,\n", &fF.fNum, fF.depa, fF.desti, fF.date, fF.time, &fF.fSeats, &fF.bSeats, &fF.eSeats) == 8)
    {
        fF.typeN++; // element type av flight that is not in the file
        struct fInfo *nF = malloc(sizeof(struct fInfo));
        memcpy(nF, &fF, sizeof(struct fInfo));
        nF->nextF = head;
        head = nF;
    }

    fclose(f);
    return (head);
}

struct bInfo *iBooking(char *fileName)
{
    struct bInfo fB, *headB = NULL;

    FILE *fP = fopen(fileName, "r");
    if (!fP)
    {
        fprintf(stderr, "Could not open file %s\n", fileName);
        exit(-1);
    }

    while (fscanf(fP, "%d,%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%s", &fB.bookN, fB.date, fB.time, fB.depa, fB.desti, fB.seatC, fB.fName, fB.sName) == 8)
    {
        fB.bSeatsN = 0;
        fB.eSeatsN = 0;
        fB.fSeatsN = 0;
        struct bInfo *nB = malloc(sizeof(struct bInfo));
        memcpy(nB, &fB, sizeof(struct bInfo));
        nB->nextB = headB;
        headB = nB;
    }

    fclose(fP);
    return (headB);
}

// booking type
void bType(struct fInfo *head, struct bInfo *headB)
{
    struct fInfo *pH;
    struct bInfo *pHb;
    int i = 0;
    int totalF = 0;
    int totalE = 0;
    int totalB = 0;

    for (pHb = headB; pHb != NULL; pHb = pHb->nextB)
    {

        for (pH = head; pH != NULL; pH = pH->nextF)
        {

            if (strcmp(pHb->time, pH->time) == 0 && strcmp(pHb->depa, pH->depa) == 0 && strcmp(pHb->desti, pH->desti) == 0)
            {
                pHb->bTypeN = pH->typeN;
            }
        }

        if (strcmp(pHb->seatC, "first") == 0)
        {
            totalF++;
            pHb->fSeatsN = pHb->fSeatsN + totalF;
        }
        else if (strcmp(pHb->seatC, "economy") == 0)
        {
            totalE++;
            pHb->eSeatsN = pHb->eSeatsN + totalE;
        }
        else if (strcmp(pHb->seatC, "business") == 0)
        {
            totalB++;
            pHb->bSeatsN = pHb->bSeatsN + totalB;
        }
    }
}

void pTyckets(struct fInfo *head, struct bInfo *headB)
{
    struct fInfo *pH;
    struct bInfo *pHb;
    FILE *pF;
    char fileN[50];
    int row, seatsT;
    for (pHb = headB; pHb != NULL; pHb = pHb->nextB)
    {

        for (pH = head; pH != NULL; pH = pH->nextF)
        {

            if (strcmp(pHb->time, pH->time) == 0 && strcmp(pHb->depa, pH->depa) == 0 && strcmp(pHb->desti, pH->desti) == 0)
            {
                // defene type of seat
                if (pHb->bSeatsN != 0)
                {
                    seatsT = pHb->bSeatsN;
                }
                else if (pHb->eSeatsN != 0)
                {
                    seatsT = pHb->eSeatsN;
                }
                else if (pHb->fSeatsN != 0)
                {
                    seatsT = pHb->fSeatsN;
                }

                // defene row
                row = (seatsT + 6) / 7;
                sprintf(fileN, "ticket-%d.txt", pHb->bookN);
                pF = fopen(fileN, "w");
                fprintf(pF, "BOOKING: %d\nFLIGHT:%d DEPARTURE: %s DESTINATION: %s %s %s\nPASSENGER: %s %s\nCLASS: %s\nROW: %d SEAT: %d\n******************\n", pHb->bookN, pH->fNum, pH->depa, pH->desti, pH->date, pH->time, pHb->fName, pHb->sName, pHb->seatC, row, seatsT);
                fclose(pF);
            }
        }
    }
}

void deleteF(struct fInfo *head, struct bInfo *headB)
{
    struct fInfo *pH;
    struct bInfo *pHb;
    int control;
    FILE *pF;

    for (pH = head; pH != NULL; pH = pH->nextF)
    {
        control = 0;
        for (pHb = headB; pHb != NULL; pHb = pHb->nextB)
        {

            if (pH->typeN == pHb->bTypeN)
            {
                control++;
            }
        }
        if (control == 0)
        {
            pF = fopen("cancelled-flight.txt", "a");
            fprintf(pF, "The flight number %d has been canceleted\n", pH->typeN);
            fclose(pF);

            free(pH);
        }
    }
}
