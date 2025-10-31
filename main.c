#include <stdio.h>
#include "Team_B/1_player.h"
#include "Team_B/2_player.h"


int main() {
    int choice;
    printf("Tic Tac Toe Game\n");
    printf("1. One Player (vs AI)\n");
    printf("2. Two Player\n");
    printf("Enter choice: ");
    scanf("%d", &choice);

    if (choice == 1)
        playOnePlayer();
    else if (choice == 2)
        playTwoPlayer();
    else
        printf("Invalid choice.\n");

    return 0;
}
