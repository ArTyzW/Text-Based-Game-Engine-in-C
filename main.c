#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gamelib.h"  

int main() {
    int scelta;
    char temp[100];
    do {
        printf("Menu di scelte:\n");
        printf("1) Imposta gioco\n");
        printf("2) Gioca\n");
        printf("3) Termina gioco\n");
        printf("Scelta: ");
        fgets(temp, sizeof(temp), stdin);
          scelta=atoi(temp);
           switch ( scelta) {
            case 1:
                imposta_gioco();
                break;
            case 2:
                    gioca();
                break;
            case 3:
                   termina_gioco();
                break;
            default:
                printf("Scelta non valida. Riprova.\n");
                break;
          }  
    } while ( fine );

    return 0;
}
