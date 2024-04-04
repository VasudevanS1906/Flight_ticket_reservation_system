#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char flightNumber[10];
    char origin[50];
    char destination[50];
    int capacity;
    int reserved;
} Flight;

typedef struct {
    char name[50];
    int age;
    char gender;
} Passenger;

typedef union {
    char creditCardNumber[20];
    int cashAmount;
} Payment;

typedef struct FlightNode {
    Flight flight;
    struct FlightNode* next;
} FlightNode;

typedef struct PassengerNode {
    Passenger passenger;
    Payment payment;
    struct PassengerNode* next;
} PassengerNode;

typedef struct {
    PassengerNode* front;
    PassengerNode* rear;
} Queue;

typedef struct {
    PassengerNode* top;
} Stack;

void addFlight(FlightNode** head, Flight flight) {
    FlightNode* newFlight = (FlightNode*)malloc(sizeof(FlightNode));
    newFlight->flight = flight;
    newFlight->next = NULL;

    if (*head == NULL) {
        *head = newFlight;
    } else {
        FlightNode* temp = *head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newFlight;
    }
}

FlightNode* createFlight(char flightNumber[], char origin[], char destination[], int capacity);
void displayFlights(FlightNode* head);
int bookFlight(FlightNode* head, Queue* reservationQueue, Stack* cancellationStack);
void cancelReservation(Queue* reservationQueue, Stack* cancellationStack);
void viewReservations(Queue* reservationQueue);

int main() {
    FlightNode* flightList = NULL;
    Queue reservationQueue = {NULL, NULL};
    Stack cancellationStack = {NULL};


    Flight flight1 = {"AA123", "Chennai", "Coimbatore", 200, 0};
    Flight flight2 = {"UA456", "Chennai", "Salem", 150, 0};
    Flight flight3 = {"DL789", "Madurai", "Trichy", 180, 0};
    addFlight(&flightList, flight1);
    addFlight(&flightList, flight2);
    addFlight(&flightList, flight3);

    int choice;
    do {
        printf("\nAirline Reservation System\n");
        printf("1. Display Flights\n");
        printf("2. Book Flight\n");
        printf("3. Cancel Reservation\n");
        printf("4. View Reservations\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                displayFlights(flightList);
                break;
            case 2:
                bookFlight(flightList, &reservationQueue, &cancellationStack);
                break;
            case 3:
                cancelReservation(&reservationQueue, &cancellationStack);
                break;
            case 4:
                viewReservations(&reservationQueue);
                break;
            case 5:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice. Try again.\n");
        }
    } while (choice != 5);

    return 0;
}

FlightNode* createFlight(char flightNumber[], char origin[], char destination[], int capacity) {
    FlightNode* newFlight = (FlightNode*)malloc(sizeof(FlightNode));
    strcpy(newFlight->flight.flightNumber, flightNumber);
    strcpy(newFlight->flight.origin, origin);
    strcpy(newFlight->flight.destination, destination);
    newFlight->flight.capacity = capacity;
    newFlight->flight.reserved = 0;
    newFlight->next = NULL;
    return newFlight;
}

void displayFlights(FlightNode* head) {
    if (head == NULL) {
        printf("No flights available.\n");
        return;
    }

    printf("Available Flights:\n");
    FlightNode* temp = head;
    while (temp != NULL) {
        printf("Flight Number: %s\n", temp->flight.flightNumber);
        printf("Origin: %s\n", temp->flight.origin);
        printf("Destination: %s\n", temp->flight.destination);
        printf("Capacity: %d\n", temp->flight.capacity);
        printf("Reserved Seats: %d\n", temp->flight.reserved);
        printf("--------------------\n");
        temp = temp->next;
    }
}

int bookFlight(FlightNode* head, Queue* reservationQueue, Stack* cancellationStack) {
    char flightNumber[10];
    printf("Enter the flight number: ");
    scanf("%s", flightNumber);

    FlightNode* temp = head;
    while (temp != NULL) {
        if (strcmp(temp->flight.flightNumber, flightNumber) == 0) {
            if (temp->flight.capacity == temp->flight.reserved) {
                printf("Flight is fully booked.\n");
                return 0;
            }

            Passenger passenger;
            printf("Enter passenger name: ");
            scanf("%s", passenger.name);
            printf("Enter passenger age: ");
            scanf("%d", &passenger.age);
            printf("Enter passenger gender (M/F): ");
            scanf(" %c", &passenger.gender);

            Payment payment;
            char paymentMethod;
            printf("Enter payment method (C for credit card, S for cash): ");
            scanf(" %c", &paymentMethod);

            if (paymentMethod == 'C' || paymentMethod == 'c') {
                printf("Enter credit card number: ");
                scanf("%s", payment.creditCardNumber);
            } else if (paymentMethod == 'S' || paymentMethod == 's') {
                printf("Enter cash amount: ");
                scanf("%d", &payment.cashAmount);
            } else {
                printf("Invalid payment method.\n");
                return 0;
            }

            PassengerNode* newPassenger = (PassengerNode*)malloc(sizeof(PassengerNode));
            newPassenger->passenger = passenger;
            newPassenger->payment = payment;
            newPassenger->next = NULL;

            if (reservationQueue->front == NULL) {
                reservationQueue->front = newPassenger;
                reservationQueue->rear = newPassenger;
            } else {
                reservationQueue->rear->next = newPassenger;
                reservationQueue->rear = newPassenger;
            }

            temp->flight.reserved++;
            printf("Reservation successful!\n");
            return 1;
        }
        temp = temp->next;
    }

    printf("Flight not found.\n");
    return 0;
}

void cancelReservation(Queue* reservationQueue, Stack* cancellationStack) {
    if (reservationQueue->front == NULL) {
        printf("No reservations to cancel.\n");
        return;
    }

    PassengerNode* cancelledReservation = reservationQueue->front;
    reservationQueue->front = reservationQueue->front->next;

    if (reservationQueue->front == NULL) {
        reservationQueue->rear = NULL;
    }

    cancelledReservation->next = cancellationStack->top;
    cancellationStack->top = cancelledReservation;

    printf("Reservation cancelled successfully.\n");
    }

void viewReservations(Queue* reservationQueue) {
    if (reservationQueue->front == NULL) {
        printf("No reservations found.\n");
        return;
    }

    printf("Current Reservations:\n");
    PassengerNode* temp = reservationQueue->front;
    while (temp != NULL) {
        printf("Passenger Name: %s\n", temp->passenger.name);
        printf("Passenger Age: %d\n", temp->passenger.age);
        printf("Passenger Gender: %c\n", temp->passenger.gender);
        if (temp->payment.creditCardNumber[0] != '\0') {
            printf("Payment Method: Credit Card (%s)\n", temp->payment.creditCardNumber);
        } else {
            printf("Payment Method: Cash ($%d)\n", temp->payment.cashAmount);
        }
        printf("--------------------\n");
        temp = temp->next;
    }
}