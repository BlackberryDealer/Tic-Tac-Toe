#include <stdio.h>
#include "Team_B/1_player.h"
#include "Team_B/2_player.h"

int main() {
    int choice;

    while (1) {
        printf("\nTic Tac Toe Game\n");
        printf("1. One Player (vs AI)\n");
        printf("2. Two Player\n");
        printf("3. Exit the game\n");
        printf("\nEnter choice: ");
        scanf("%d", &choice);

        if (choice == 1)
            playOnePlayer();
        else if (choice == 2)
            playTwoPlayer();
        else if (choice == 3) {
            printf("\nExiting the game.... Goodbye!\n\n");
            break;
        } else {
            printf("Invalid choice.\n");
        }
    }

    return 0;
}
