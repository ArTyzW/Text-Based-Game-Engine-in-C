#include "gamelib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>   
#include <stdbool.h>

struct Zona_mappa* pFirst = NULL;
struct Zona_mappa* pLast = NULL;
static int n_zone = 0;
static int mappa_creata;
static int numero_giocatori;
static int oggetti_prova_inizio[] = {0, 0, 0};
static int livello_difficolta = 3;
static int oggetti_prova_consegnati = 0;
static int sale_utilizzato = 0;
static int gioco_impostato = 0;
static int conta_morti = 0;
static int n_prove = 0;
static int fant = 0;
static int avanzato = 0; 
static int turno = 0; 
struct Giocatore** giocatori; 
static int oggetti_prova[3] = {0, 0, 0};
int fine = 1;
           
void imposta_gioco() {
    time_t t;
    srand((unsigned) time(&t));
    int conferma = 0;
    char temp[100];
    system("clear");
do {
    // Ask the user for the number of players
    printf("Inserisci il numero di giocatori (da 1 a 4): ");
    fgets(temp, sizeof(temp), stdin);          
    numero_giocatori = atoi(temp);
    
    if (numero_giocatori < 1 || numero_giocatori > 4) {
        printf("Numero giocatori non supportato. Riprova.\n");
        conferma = 1; // Invalid number, continue loop
    } else {
        conferma = 0; // Valid number, exit loop
    }
} while (conferma == 1);
 
    // Dynamic allocation for the array of pointers to Giocatore
    giocatori = (struct Giocatore**)malloc(numero_giocatori * sizeof(struct Giocatore*));

    if (giocatori == NULL) {
        printf("Errore nell'allocazione di memoria.\n");
    }
    system("clear");
    // Initialize players (dynamic allocation for each player)
    for (int i = 0; i < numero_giocatori; i++) {
        giocatori[i] = (struct Giocatore*)malloc(sizeof(struct Giocatore));
        if (giocatori[i] == NULL) {
            printf("Errore nell'allocazione di memoria per il giocatore %d.\n", i + 1);
        }
        printf("Inserisci il nome del giocatore %d: ", i + 1);
        scanf("%s", giocatori[i]->nome_giocatore); 
        giocatori[i]->sanita_mentale = 100;                                
        // Initialize backpack inventory slots to empty/no item
        for (int j = 0; j < MAX_SLOT_ZAINO; j++) {
            giocatori[i]->zaino[j] = 15;
        }  
    }
        system("clear");
        // Request difficulty level
        int a = 1;
        do {
        printf("Seleziona il livello di difficoltà:\n");
        printf("1. Dilettante\n");
        printf("2. Intermedio\n");
        printf("3. Incubo\n");
        char temp1[100];
        fgets(temp1, sizeof(temp1), stdin);
        livello_difficolta = atoi(temp1);
        if (a == 1){  // Workaround: Skip initial loop turn to handle fgets buffer flush issues
        system("clear");             
        a = 0;
        continue;
        }             
        if(livello_difficolta < 1 || livello_difficolta > 3) {
            printf("Scelta non valida. Riprova.\n");
        } 
      
    } while (livello_difficolta < 1 || livello_difficolta > 3);
     system("clear");   
    char temp1[100];    
     int numero_tipi_oggetti = 5;  // Set item types available      
    // Generate initial items
    for (int i = 0; i < numero_giocatori; i++) {
      // Generate a random index for available item types
      int oggetto_generato = rand() % numero_tipi_oggetti;

      // Assign generated item to the player
      giocatori[i]->zaino[0] = oggetto_generato;
       switch (oggetto_generato) {
    case 0:
        if(oggetti_prova_inizio[0] == 0){
          printf("%s hai ricevuto EMF\n", giocatori[i]->nome_giocatore);
          oggetti_prova_inizio[0] = 1;
          for(int k = 0; k < 3 ; k++){
          }
         break;
        }else{
         i--;
         break;
        }     
    case 1:  
       if(oggetti_prova_inizio[1] == 0){
         printf("%s hai ricevuto spirit_box\n", giocatori[i]->nome_giocatore);
         oggetti_prova_inizio[1] = 1;
        break;
        }else{
        i--;
        break;
        }
    case 2:
         if(oggetti_prova_inizio[2] == 0){
         printf("%s hai ricevuto videocamera \n", giocatori[i]->nome_giocatore);
         oggetti_prova_inizio[2] = 1;
         break;
        }else{
         i--;
         break;
        }        
    case 3:
        printf("%s hai ricevuto calmanti \n", giocatori[i]->nome_giocatore);
        break;
    case 4:
        printf("%s hai ricevuto sale\n", giocatori[i]->nome_giocatore);
        break;
    default:
        break;
       }
      
    }    
    
    // Check if at least one player has an evidence tool; otherwise assign one randomly
    int flag = 0;
    for (int i = 0; i < numero_giocatori; i++) {
       if(giocatori[i]->zaino[0] > 2){
         flag++;
       }
    }
     
    if(flag == numero_giocatori){
      int giocatore_con_prova = rand() % numero_giocatori;
      int oggetto_prova = rand() % 3;
      giocatori[giocatore_con_prova]->zaino[0] = oggetto_prova;
      switch (oggetto_prova) {
    case 0:
        printf("%s hai ricevuto EMF\n", giocatori[giocatore_con_prova]->nome_giocatore);
        oggetti_prova_inizio[0] = 1;
        break;       
    case 1:
       printf("%s hai ricevuto spirit_box\n", giocatori[giocatore_con_prova]->nome_giocatore);
        oggetti_prova_inizio[1] = 1;
        break;      
    case 2:      
        printf("%s hai ricevuto videocamera \n", giocatori[giocatore_con_prova]->nome_giocatore);
        oggetti_prova_inizio[2] = 1;
        break;
    default:
        break;
       }
     }
    
    // Loop allowing each player to select an item

    char *endptr;
    char temp3[100];
    int Flag;
    for (int i = 0; i < numero_giocatori; i++) {
        int scelta_oggetto;
        do {
      printf("Giocatore %d, è il tuo turno di scegliere un oggetto:\n", i + 1);   
      printf("Scegli un oggetto: 0 - EMF, 1 - spirit_box, 2 - videocamera, 3 - calmanti, 4 - sale\n"); // Item choices
      fgets(temp3, sizeof(temp3), stdin);
      size_t len = strlen(temp1);
      if (len > 0 && temp3[len - 1] == '\n') {
       temp3[len - 1] = '\0';
      }
      scelta_oggetto = strtol(temp3, &endptr, 10);
      if (*endptr == '\n' || *endptr == '\0') {
      Flag = 1;
      switch(scelta_oggetto){
     case 0: 
               if(oggetti_prova_inizio[0] == 1){
                i--;
                printf("oggetto prova gia assegnato. Riprova.\n"); // If evidence item 0 (EMF) is already assigned, repeat choice loop
               }else{
                 giocatori[i]->zaino[1] = scelta_oggetto;
                 oggetti_prova_inizio[scelta_oggetto] = 1;
                }
     
      break;
     case 1:
            if(oggetti_prova_inizio[1] == 1){
                i--;
                printf("oggetto prova gia assegnato. Riprova.\n"); // If evidence item 1 (spirit box) is already assigned, repeat choice loop
               }else{
                 giocatori[i]->zaino[1] = scelta_oggetto;
                 oggetti_prova_inizio[scelta_oggetto] = 1;
                }              
      break;
     case 2:         
            if(oggetti_prova_inizio[2] == 1){
                i--;
                printf("oggetto prova gia assegnato. Riprova.\n"); // If evidence item 2 (videocamera) is already assigned, repeat choice loop
               }else{
                 giocatori[i]->zaino[1] = scelta_oggetto;
                 oggetti_prova_inizio[scelta_oggetto] = 1;
                }
      break;
      case 3:
            giocatori[i]->zaino[1] = scelta_oggetto;
      break;
      case 4:
            giocatori[i]->zaino[1] = scelta_oggetto;
      break;    
     default:
            printf("Scelta dell'oggetto non valida. Scegli un numero tra 0 e 4.\n");
            i--; 
      break;
      
      }    
     } else {
            Flag = 0;
            printf("Errore: l'input non è un numero valido.\n");
        }
      }while(Flag != 1);
    } 
    system("clear");
      int scelta;
      char temp4[100];
     do{ 
        printf("Menu:\n");
        printf("1. Inserisci zona\n");
        printf("2. Cancella zona\n");
        printf("3. Stampa mappa\n");
        printf("4. Chiudi mappa\n");
        printf("Scelta: ");
        fgets(temp4, sizeof(temp4), stdin);
        scelta = atoi(temp4);      
        switch (scelta) {
            case 1:
                inserisci_zona();
                break;

            case 2:
                if (n_zone == 0) {
                    printf("Errore: crea prima la mappa.\n");
                    break;
                }
                cancella_zona();
                break;

            case 3:
                stampa_mappa();
                break;

            case 4:
                chiudi_mappa();
                break;               
            default:
                printf("Scelta non valida.\n");
                break;
        } 
     }while(scelta != 4);
    gioco_impostato = 1;
}


 void inserisci_zona() {
    // Allocate new zone dynamically
    struct Zona_mappa* nuova_zona = (struct Zona_mappa*)malloc(sizeof(struct Zona_mappa));
    time_t t;
    srand((unsigned) time(&t));
    if (nuova_zona == NULL) {
        printf("Errore nell'allocazione della memoria per la nuova zona\n");
        exit(EXIT_FAILURE);
    }
    int a = 0;
    // Randomly generate zone type
    a = rand() % 6; 
    nuova_zona->zona = a;
    // Randomly generate room item
    nuova_zona->oggetto_zona = (rand() % 4) + 5;
    
    // Randomly determine if evidence is present in the room
    if (rand() % 100 < 40) {
        nuova_zona->prova = (rand() % 3) + 10;
    } else {
        nuova_zona->prova = 13;
    }

    // Attach new zone to circular linked list
    if (pLast == NULL) {
        // Empty list: new zone becomes first and last node
        pFirst = nuova_zona;
        pLast = nuova_zona;
        nuova_zona->prossima_zona = nuova_zona; // Self-reference to maintain circular link
    } else {
        // Append new zone after the last zone
        nuova_zona->prossima_zona = pFirst; // Next pointer points to head
        pLast->prossima_zona = nuova_zona;  // Previous tail points to new node
        pLast = nuova_zona;                 // Update tail pointer
    }
    
    if (n_zone == 1) {
            for (int j = 0; j < numero_giocatori; j++){
            giocatori[j]->posizione = pFirst;
            }
    
    } 
    system("clear");
    printf("Zona aggiunta\n");
    n_zone++;
}

void cancella_zona() {
    if (pFirst == NULL) {
        // Empty list: nothing to delete
        printf("Nessuna zona da cancellare\n");
        return;
    }

    if (pFirst == pLast) {
        // Only one zone in the list
        free(pFirst);
        pFirst = NULL;
        pLast = NULL;
    } else {
        // Find node prior to last node
        struct Zona_mappa* zona_precedente = pFirst;
        while (zona_precedente->prossima_zona != pLast) {
            zona_precedente = zona_precedente->prossima_zona;
        }

        // Link node before last directly to head node to preserve circular structure
        zona_precedente->prossima_zona = pFirst;

        // Deallocate tail node
        free(pLast);

        // Update tail pointer
        pLast = zona_precedente;
    }
    system("clear");
    printf("Ultima zona cancellata\n");
    n_zone--;
}

void stampa_mappa() {
    if (pFirst == NULL) {
        printf("Nessuna zona presente nella mappa.\n");
        return;
    }

    struct Zona_mappa* current_zone = pFirst;
    int zone_counter = 1;

    printf("Mappa di gioco:\n");

    do {
        printf("Zona %d:\n", zone_counter);
        printf("   Tipo zona: ");
        switch (current_zone->zona) {
    case 0:
        printf("Cucina\n");
        break;
    case 1:
        printf("Soggiorno\n");
        break;
    case 2:
        printf("Camera\n");
        break;
    case 3:
        printf("Bagno\n");
        break;
    case 4:
        printf("Garage\n");
        break;
    case 5:
        printf("Seminterrato\n");
        break;
    default:
        printf("Errore");
        break;
       }

    int oggettoz = current_zone->oggetto_zona;
        
        printf("   Oggetto zona: ");
              
       switch (oggettoz) {
    case 5:
        printf("Nessun oggetto\n");
        break;
    case 6:
        printf("Adrenalina\n");
        break;
    case 7:
        printf("100 $\n");
        break;
    case 8:
        printf("Coltello\n");
        break;
    default:
        printf("Errore");
       }
        
int stampa_prova = current_zone->prova;
        printf("   Prova: ");
        switch (stampa_prova) {
    case 10:
        printf("EMF\n");
        break;
    case 11:
        printf("Spirit Box\n");
        break;
    case 12:
        printf("Videocamera\n");
        break;
    case 13:
        printf("Nessun oggetto prova\n");
        break;
    default:
        printf("Errore");
       }
        
        
        
        printf("\n");

        current_zone = current_zone->prossima_zona;
        zone_counter++;
    } while (current_zone != pFirst);
}


void chiudi_mappa() {
    mappa_creata = 1;
    system("clear");
    printf("Creazione della mappa terminata.\n");
    gioco_impostato = 1;
    if(gioco_impostato == 1){
    
    printf("Gioco impostato.\n");
    }
}


void gioca(){
    system("clear");
    int scelta;
    time_t t;
    srand((unsigned) time(&t));
    if(gioco_impostato == 0){
      printf("Devi prima impostare il gioco\n");
      stampa_menu();
      return;
    }
    int* giocatore_corrente;
    // Turn execution loop
    while (n_prove != 3 && conta_morti != numero_giocatori) {
         printf("   Turno numero %d  \n ", turno);
         printf("Debug: Condizioni del ciclo, n_prove: %d, conta_morti: %d\n", n_prove, conta_morti);
        // Each player takes turn sequentially                
            giocatore_corrente = ordine_casuale();
            
            if (giocatori[*giocatore_corrente]->sanita_mentale != 0) {
                // Execute active player actions
                do{
                     printf("   Azione da compiere:  \n ");
                     printf("1. Torna al caravan\n");
                     printf("2. Stampa valori del giocatore\n");
                     printf("3. Stampa i valori della zona\n");
                     printf("4. Avanza nella zona successiva\n");
                     printf("5. Raccogli prova\n");
                     printf("6. Raccogli oggetto\n");
                     printf("7. Usa oggetto\n");
                     printf("8. Passa il turno\n");
                     char temp[100];
                     fgets(temp, sizeof(temp), stdin);
                     scelta = atoi(temp);                  
                 switch (scelta) {
             case 1:
             if(fant == 0){
                torna_caravan(giocatori[*giocatore_corrente]);
                }else{
                printf("Il fantasma non ti permette di andare al caravan\n");
                }
                break;
             case 2:
              if(giocatori[*giocatore_corrente]->sanita_mentale != 0){
                  stampa_giocatore(giocatori[*giocatore_corrente]);
                }else{
                printf("Sei morto, unica opzione possibile: 8. Passa\n");
                }
                
                break;
             case 3:
                if(giocatori[*giocatore_corrente]->sanita_mentale != 0){
                  stampa_zona(giocatori[*giocatore_corrente]);
                }else{
                printf("Sei morto, unica opzione possibile: 8. Passa\n");
                }
                
                break;
            case 4:
                if(giocatori[*giocatore_corrente]->sanita_mentale != 0){
                 avanza(giocatori[*giocatore_corrente]);
                }else{
                printf("Sei morto, unica opzione possibile: 8. Passa\n");
                }
              
               break;
            case 5:
               if(giocatori[*giocatore_corrente]->sanita_mentale != 0){
                  raccogli_prova(giocatori[*giocatore_corrente] , giocatori, numero_giocatori);
                }else{
                printf("Sei morto, unica opzione possibile: 8. Passa\n");
                }
               
               break;
            case 6:
               if(giocatori[*giocatore_corrente]->sanita_mentale != 0){
                  raccogli_oggetto(giocatori[*giocatore_corrente]);
                }else{
                printf("Sei morto, unica opzione possibile: 8. Passa\n");
                }
               
               break;
            case 7:
               if(giocatori[*giocatore_corrente]->sanita_mentale != 0){
                  usa_oggetto(giocatori[*giocatore_corrente]);
                }else{
                printf("Sei morto, unica opzione possibile: 8. Passa\n");
                }
             
               break;
           case 8:
               passa(&turno);
               free(giocatore_corrente);
               break;
            default:
               printf("Errore");
       }
                }while(scelta != 8);
                
                
            }
        fant = 0;
        } 
     if(conta_morti == numero_giocatori){
       printf("La partita è finita: Avete perso! \n");
     }else if(n_prove == 3){
       printf("La partita è finita: avete vinto \n");
     }
     
     gioco_impostato = 0;
     dealloca_tutto(giocatori ,numero_giocatori,pFirst);

     
     
     
}

void torna_caravan(struct Giocatore *giocatore){
  int prova_trovata;
  int conta = 0;
  printf("Entrata Caravan\n");
      for (int i = 0; i < 4; i++) {
       if(giocatore->zaino[i] >= 10 && giocatore->zaino[i] < 15){
          prova_trovata = giocatore->zaino[i];
          for (int j = 0; j < 3; j++){
            if(prova_trovata == (j+10)){                                              
              oggetti_prova[j] = 1; 
              oggetti_prova_inizio[j] = 1;
              oggetti_prova_consegnati++;
              giocatore->zaino[i] = 15;
            switch (prova_trovata) {
                  case 10:
                      printf("Prova EMF Depositata\n");
                      n_prove++;
                      conta++;
                      break;
                  case 11:
                      printf("Prova Spirit box Depositata\n");
                      n_prove++;
                      conta++;
                      break;
                  case 12:
                      printf("Prova Videocamera Depositata\n");
                      n_prove++;
                      conta++;
                      break;
                  default:
                     printf("Errore");
                      }
            }          
          } 
       }
     }
     if(conta == 0){
       printf("Nessuna prova trovata\n");
       }
   
    for(int i = 0; i < 4; i++){ // Replace deposited evidence item with new default if all initial evidence tools are assigned
        for(int j = 0; j < 3; j++){
          if(giocatore->zaino[i] == 15 && oggetti_prova_inizio[j] == 0){
             giocatore->zaino[i] = j;
             oggetti_prova_inizio[j] = 1;
          }
        } 
    }
   
    giocatore->posizione = pFirst;
    decrementa_sanita_mentale(giocatore);
    passa(&turno);
}
 
void stampa_giocatore(struct Giocatore *giocatore) {
    printf("Nome: %s\n", giocatore->nome_giocatore);
    printf("Sanità Mentale: %u\n", giocatore->sanita_mentale);
     switch (giocatore->posizione->zona) {
    case 0:
        printf("Cucina\n");
        break;
    case 1:
        printf("Soggiorno\n");
        break;
    case 2:
        printf("Camera\n");
        break;
    case 3:
        printf("Bagno\n");
        break;
    case 4:
        printf("Garage\n");
        break;
    case 5:
        printf("Seminterrato\n");
        break;
    default:
        printf("Errore");
        break;
       }
    
    printf("Zaino: [ ");
    for (int i = 0; i < 4; i++) {
    
    switch (giocatore->zaino[i]) {
    case 0:
        printf(" EMF ");
        break;
    case 1:
        printf(" spirit_box ");
        break;
    case 2:
        printf(" videocamera ");
        break;
    case 3:
        printf(" calmanti ");
        break;
    case 4:
        printf(" sale ");
        break;
    case 6:
        printf("Adrenalina\n");
        break;
    case 7:
        printf("100 $\n");
        break;
    case 8:
        printf("Coltello\n");
        break;
    case 10:
        printf("Prova:spirit box\n");
        break;
    case 11:
        printf("Prova:EMF\n");
        break; 
    case 12:
        printf("Prova:videocamera\n");
        break;       
    case 15:
        printf(" Vuoto ");
        break;      
    default:
        printf("Errore");
       }
    }
    printf("]\n");
}



void stampa_zona(struct Giocatore *giocatore) {
    printf("Zona: ");
    switch (giocatore->posizione->zona) {
    case 0:
        printf("Cucina\n");
        break;
    case 1:
        printf("Soggiorno\n");
        break;
    case 2:
        printf("Camera\n");
        break;
    case 3:
        printf("Bagno\n");
        break;
    case 4:
        printf("Garage\n");
        break;
    case 5:
        printf("Seminterrato\n");
        break;
    default:
        printf("Errore");
        break;
       }
    printf("Oggetto zona: ");
    int oggetto_zon = giocatore->posizione->oggetto_zona;
    switch (oggetto_zon) {
    case 5:
        printf("Nessun oggetto\n");
        break;
    case 6:
        printf("Adrenalina\n");
        break;
    case 7:
        printf("100 $\n");
        break;
    case 8:
        printf("Coltello\n");
        break;
    default:
        printf("Errore");
       }
    
    printf("   Prova: ");
    int provaa = giocatore->posizione->prova;
        switch (provaa) {
    case 10:
        printf("EMF\n");
        break;
    case 11:
        printf("Spirit Box\n");
        break;
    case 12:
        printf("Videocamera\n");
        break;
    case 13:
        printf("Nessun oggetto prova\n");
        break;
    default:
        printf("Errore");
       }
}




void avanza(struct Giocatore *giocatore) {
    // Check if player has already moved during this turn
    if (avanzato == 1) {
        printf("Hai già avanzato in questo turno.\n");
        return; // Player moved already, exit function
    }else{
        // Move player to the next zone node
        giocatore->posizione = giocatore->posizione->prossima_zona;
        avanzato = 1;
    }
    // Randomize evidence type for the current zone
    int probabilita = rand() % 100;
    if (probabilita < 40) {
        giocatore->posizione->prova = 13;
    } else if (probabilita < 60) {
        giocatore->posizione->prova = 10;
    } else if (probabilita < 80) {
        giocatore->posizione->prova = 11;
    } else {
        giocatore->posizione->prova = 12;
    }

    // Mark player move phase as complete for this turn
    decrementa_sanita_mentale(giocatore);
}


void raccogli_prova(struct Giocatore *giocatore, struct Giocatore** giocatori, int numero_giocatori) {
    // Check if player already carries an evidence item
    int conta = 0;
    for(int i = 0; i < 4; i++){
      if (giocatore->zaino[i] >= 10 && giocatore->zaino[i] < 13){
       if (giocatore->zaino[i] != nessuna_prova) {
        printf("%s ha già una prova nel suo zaino.\n", giocatore->nome_giocatore);
        return;
       }
      }
    }

    // Verify if player carries required tool matching room evidence
    for(int i = 0; i < 4; i++){
    int prova = giocatore->posizione->prova;
      if ((giocatore->zaino[i] + 10) == prova){
        conta++;
            switch (giocatore->zaino[i]) {
               case 0:
                printf("%s ha raccolto una prova: EMF\n", giocatore->nome_giocatore );
                giocatore->zaino[i] += 10;
                break;
               case 1:
                printf("%s ha raccolto una prova: Spirit Box\n", giocatore->nome_giocatore );
                giocatore->zaino[i] += 10;
                break;
               case 2:
                printf("%s ha raccolto una prova: Videocamera\n", giocatore->nome_giocatore );
                giocatore->zaino[i] += 10;
                break;
               default:
                printf("%s non ha l'oggetto corrispondente per raccogliere la prova. 2\n", giocatore->nome_giocatore);
                break;
           }
           
       }
    }
    if(conta == 0){printf("%s non ha l'oggetto corrispondente per raccogliere la prova. 1\n", giocatore->nome_giocatore);}

    // Generate random value for ghost appearance check
    int probabilita_apparizione = rand() % 100;

    // Determine ghost encounter chance based on game difficulty
    int probabilita_minima = 0;
    int incremento_probabilita = 0;
    
    switch (livello_difficolta) {
        case 1:
            probabilita_minima = 10;
            incremento_probabilita = 20;
            break;
        case 2:
            probabilita_minima = 20;
            incremento_probabilita = 30;
            break;
        case 3:
            probabilita_minima = 30;
            incremento_probabilita = 40;
            break;
    }

    // Check ghost spawn trigger
    if (probabilita_apparizione < probabilita_minima + oggetti_prova_consegnati * incremento_probabilita) { 
        printf("Il fantasma è apparso!\n");
        fant = 1;
        // Decrease player sanity points based on difficulty level
        if(sale_utilizzato == 0){
        switch (livello_difficolta) {
            case 1:
            giocatore->sanita_mentale -= 10;
            printf("%s hai subito una diminuzione della sanità!\n", giocatore->nome_giocatore);
            if (giocatore->sanita_mentale == 0){
                   printf("%s è morto.\n", giocatore->nome_giocatore);
                   conta_morti++;
            }else if(giocatore->sanita_mentale > 140){
                  giocatore->sanita_mentale = 0;
                  printf("%s è morto.\n", giocatore->nome_giocatore);
                  conta_morti++;
             }
            for (int i = 0; i < numero_giocatori; i++) {
              if (giocatori[i] != giocatore && giocatori[i]->posizione == giocatore->posizione) {
                giocatori[i]->sanita_mentale -= 10;
                printf(" Diminuzione sanita mentale: %s!\n", giocatori[i]->nome_giocatore);
                if (giocatori[i]->sanita_mentale == 0){
                   printf("%s è morto.\n", giocatori[i]->nome_giocatore);
                   conta_morti++;
                }else if(giocatore->sanita_mentale > 140){
                  giocatori[i]->sanita_mentale = 0;
                  printf("%s è morto.\n", giocatori[i]->nome_giocatore);
                  conta_morti++;
                }
                
              }
            }
                break;
            case 2:
            giocatore->sanita_mentale -= 20;
            printf("%s hai subito una diminuzione della sanità mentale !\n", giocatore->nome_giocatore);
            if (giocatore->sanita_mentale == 0){
                   printf("%s è morto.\n", giocatore->nome_giocatore);
                   conta_morti++;
            }else if(giocatore->sanita_mentale > 140){
                  giocatore->sanita_mentale = 0;
                  printf("%s è morto.\n", giocatore->nome_giocatore);
                  conta_morti++;
             }
            for (int i = 0; i < numero_giocatori; i++) {
              if (giocatori[i] != giocatore && giocatori[i]->posizione == giocatore->posizione) {
                giocatori[i]->sanita_mentale -= 20;
                printf(" Diminuzione sanita mentale: %s !\n", giocatori[i]->nome_giocatore);
                if (giocatori[i]->sanita_mentale == 0){
                   printf("%s è morto.\n", giocatori[i]->nome_giocatore);
                   conta_morti++;
                }else if(giocatore->sanita_mentale > 140){
                  giocatori[i]->sanita_mentale = 0;
                  printf("%s è morto.\n", giocatori[i]->nome_giocatore);
                  conta_morti++;
                }
                
              }
            }
                break;
            case 3:
            giocatore->sanita_mentale -= 30;
            printf("%s hai subito una diminuzione della sanità mentale!\n", giocatore->nome_giocatore);
            if (giocatore->sanita_mentale == 0){
                   printf("%s è morto.\n", giocatore->nome_giocatore);
                   conta_morti++;
            }else if(giocatore->sanita_mentale > 140){
                  giocatore->sanita_mentale = 0;
                  printf("%s è morto.\n", giocatore->nome_giocatore);
                  conta_morti++;
             }
            for (int i = 0; i < numero_giocatori; i++) {
              if (giocatori[i] != giocatore && giocatori[i]->posizione == giocatore->posizione) {
                giocatori[i]->sanita_mentale -= 30;
                printf(" Diminuzione sanita mentale: %s!\n", giocatori[i]->nome_giocatore);
                if (giocatori[i]->sanita_mentale == 0){
                   printf("%s è morto.\n", giocatori[i]->nome_giocatore);
                   conta_morti++;
                }else if(giocatore->sanita_mentale > 140){
                  giocatori[i]->sanita_mentale = 0;
                  printf("%s è morto.\n", giocatori[i]->nome_giocatore);
                  conta_morti++;
                }
                
              }
            }
                break;
        }

        printf("%s ha perso punti di sanità mentale. Sanità mentale attuale: %d\n", giocatore->nome_giocatore, giocatore->sanita_mentale);
    }else{
       printf("%s è protetto dal sale\n", giocatore->nome_giocatore);
      }
  }
  decrementa_sanita_mentale(giocatore);
}
 
     


void raccogli_oggetto(struct Giocatore *giocatore) {
   if (giocatore->posizione->oggetto_zona != 5) {
    int spazio_libero = -1; // Initialize to -1 to flag filled backpack status
    for (int i = 0; i < 4; i++) {
        if (giocatore->zaino[i] == 15) {
            // Found empty slot in inventory
            spazio_libero = i;
            break;
        }
    }

    if (spazio_libero != -1) {
        // Space available: pick up item located in current zone
        int oggetto_da_raccogliere = giocatore->posizione->oggetto_zona;

        // Assign object to open inventory slot
        giocatore->zaino[spazio_libero] = oggetto_da_raccogliere;
        printf("%s ha raccolto un oggetto dalla zona!\n", giocatore->nome_giocatore);
    } else {
        // Backpack is full: cannot take item
        printf("%s ha lo zaino pieno e non può raccogliere altro!\n", giocatore->nome_giocatore);
    }
    
    }else{printf("Nessun oggetto presente in questa zona");}
    
    decrementa_sanita_mentale(giocatore);  
}



void uso_sale(struct Giocatore *giocatore) {
int conta = 0;
    // Check if player has salt item in inventory
    for (int i = 0; i < 4; i++){
      if (giocatore->zaino[i] == 4) {
       conta++;
        printf("%s usa il sale per proteggersi dal fantasma!\n", giocatore->nome_giocatore);
        // Salt prevents sanity reduction from ghost encounters
        sale_utilizzato = 1;
        giocatore->zaino[i] = 15; // 15 represents empty item slot
        break;
        
      }
    }
       if (conta == 0){ 
       printf("%s non hai sale!\n", giocatore->nome_giocatore);
       }
}

void uso_calmanti(struct Giocatore *giocatore){
    int conta = 0;  
    for (int j = 0; j < 4; j++){
     if(giocatore->zaino[j] == 3){
     conta++;
     printf("%s usa i calmanti e si sente meglio!\n", giocatore->nome_giocatore);
     // Boost player sanity points by 40
    giocatore->sanita_mentale += 40;

    // Enforce sanity upper bound limit
    if (giocatore->sanita_mentale > 100 && giocatore->sanita_mentale < 141) {
        giocatore->sanita_mentale = 100;
    }
     giocatore->zaino[j] = 15; // 15 represents empty item slot
     break;
     }
   }
    if (conta == 0){ 
       printf("%s non hai calmanti!\n", giocatore->nome_giocatore);
       }
}

void uso_100_dollari(struct Giocatore *giocatore){
int conta = 0;
  for (int i = 0; i < 4; i++){
     if (giocatore->zaino[i] == 7) {
     conta++;
    }
  }
    if (conta == 0){
      printf("%s non ha abbastanza dollari per effettuare questa operazione.\n", giocatore->nome_giocatore);
      return;
    }else{
     printf("%s usa 100 dollari per ottenere un oggetto.\n", giocatore->nome_giocatore);
    }
    
    // Generate random binary choice (0 or 1) for reward item
    int scelta = rand() % 2;
  for (int i = 0; i < 4; i++){
    if (giocatore->zaino[i] == 7) {
      if (scelta == 0) {
          printf("Hai ricevuto dei calmanti!\n");
          giocatore->zaino[i] = 3; // Assign painkillers item (value 3)
      } else {
          printf("Hai ricevuto del sale!\n");
          giocatore->zaino[i] = 4; // Assign salt item (value 4)
      }
    }
  }
}




void uso_coltello(struct Giocatore *giocatore, struct Giocatore** giocatori, int numero_giocatori) {
  int conta = 0;
 
  for (int j = 0; j < 4; j++){
   if(giocatore->zaino[j] == 8){
   conta++;
    giocatore->zaino[j] = 15;
    // Check if active player sanity is below 30 threshold
    if (giocatore->sanita_mentale < 30) {
        // Retrieve current zone position node
        struct Zona_mappa* posizione_giocatore = giocatore->posizione;

        // Check for other players in same zone
        for (int i = 0; i < numero_giocatori; i++) {
            if (giocatori[i] != giocatore && giocatori[i]->posizione == posizione_giocatore) {
                // Kill target player
                printf("%s usa il coltello e uccide %s nella zona %d!\n", giocatore->nome_giocatore, giocatori[i]->nome_giocatore, posizione_giocatore->zona);

                // Zero out target player's sanity
                giocatori[i]->sanita_mentale = 0;
                conta_morti++;

                // Remove deceased player position reference
                giocatori[i]->posizione = NULL;
            }
            
        }
        break;
    } else {
        // Player sanity remains too high to activate knife
        printf("%s non usa il coltello perché la sua sanità mentale è superiore a 30.\n", giocatore->nome_giocatore);
        break;
    }
  }
 }
 if (conta == 0){ 
       printf("%s non hai Coltello!\n", giocatore->nome_giocatore);
       }
}

void uso_adrenalina(struct Giocatore *giocatore) {
    // Invoke avanza() function to move an extra step
    for (int j = 0; j < 4; j++){
     if(giocatore->zaino[j] == 6){
     giocatore->zaino[j] = nessun_oggetto;
     }
    }
    printf("%s usa l'adrenalina e avanza una volta in più!\n", giocatore->nome_giocatore);
    avanzato = 0;
    avanza(giocatore);
    
}

void usa_oggetto(struct Giocatore *giocatore) {
    int scelta;
    // Print list of usable items
    do{ 
    printf("Scegli l'oggetto da utilizzare:\n");
    printf("1. Sale\n");
    printf("2. Calmanti\n");
    printf("3. 100 Dollari\n");
    printf("4. Coltello\n");
    printf("5. Adrenalina\n");
    printf("6. indietro\n");
    char temp[100];
    fgets(temp, sizeof(temp), stdin);
    scelta = atoi(temp);
    switch (scelta) {
        case 1:
            // Use salt item
            int i = 0;
             for (int j = 0; j < 4; j++){
              if(giocatore->zaino[j] == 4){
               uso_sale(giocatore);
               decrementa_sanita_mentale(giocatore);
               break;
              }else{i++;};
             }
             if(i == 4){
               printf("Non possiedi questo oggetto");
               }
            
            break;
        case 2:
            // Use painkillers
            int z = 0;
             for (int j = 0; j < 4; j++){
              if(giocatore->zaino[j] == 3){
               uso_calmanti(giocatore);
               decrementa_sanita_mentale(giocatore);
               break;
               }else{z++;};
             }
             if(z == 4){
               printf("Non possiedi questo oggetto");
               }
            break;
        case 3:
            // Use 100 dollars
            int k = 0;
            for (int j = 0; j < 4; j++){
              if(giocatore->zaino[j] == 7){
               uso_100_dollari(giocatore);
               decrementa_sanita_mentale(giocatore);
               break;
               }else{k++;};
              
             }
             if(k == 4){
               printf("Non possiedi questo oggetto");
               }
            break;
        case 4:
            // Use knife
            int n = 0;
            for (int j = 0; j < 4; j++){
              if(giocatore->zaino[j] == 8){
               uso_coltello(giocatore, giocatori, numero_giocatori);
               decrementa_sanita_mentale(giocatore);
               break;
               }else{n++;};
             }
             if(n == 4){
               printf("Non possiedi questo oggetto");
               }
            break;
        case 5:
            // Use adrenaline
            int m = 0;
            for (int j = 0; j < 4; j++){
              if(giocatore->zaino[j] == 6){
               uso_adrenalina(giocatore);
               decrementa_sanita_mentale(giocatore);
               break;
               }else{m++;};
             }
             if(m == 4){
               printf("Non possiedi questo oggetto");
               }
            break;
        case 6:
         break;
          
        default:
            printf("Scelta non valida.\n");
            break;
    }
   } while(scelta != 6);
}

void decrementa_sanita_mentale(struct Giocatore *giocatore) {
    int probabilita = rand() % 100 + 1;  // Generate random integer between 1 and 100
    
    if (probabilita <= 20) {
        giocatore->sanita_mentale -= 15;
        printf("%s ha subito una diminuzione di 15 punti alla sanità mentale!\n", giocatore->nome_giocatore);
    }
    if (giocatore->sanita_mentale == 0){
      printf("%s è morto.\n", giocatore->nome_giocatore);
      conta_morti++;
    }else if(giocatore->sanita_mentale > 140){
      giocatore->sanita_mentale = 0;
      printf("%s è morto.\n", giocatore->nome_giocatore);
      conta_morti++;
    }
}

void passa(int *turno) {
       (*turno)++;
       avanzato = 0;
}


void dealloca_giocatori(struct Giocatore** giocatori, int numero_giocatori) {
    for (int i = 0; i < numero_giocatori; i++) {
        free(giocatori[i]);
    }
    free(giocatori);
}

void dealloca_zone_mappa(struct Zona_mappa* pFirst) {
    if (pFirst == NULL) {
        return;
    }

    struct Zona_mappa* current_zone = pFirst;
    struct Zona_mappa* next_zone;

    do {
        next_zone = current_zone->prossima_zona;
        free(current_zone);
        current_zone = next_zone;
    } while (current_zone != pFirst);
}

void dealloca_tutto(struct Giocatore** giocatori, int numero_giocatori, struct Zona_mappa* pFirst) {
    dealloca_giocatori(giocatori, numero_giocatori);
    dealloca_zone_mappa(pFirst);
}





void termina_gioco(){
   system("clear");
   int play = 0;
   while (play != 2){
        printf("vuoi rigiocare? \n" );
        printf("1) si \n" );
        printf("2) no \n" );
        char temp[100];
        fgets(temp, sizeof(temp), stdin);
        play = atoi(temp);
        switch (play) {
            case 1:
                system("clear");
                
                
                 break;
            case 2:
                printf("Arrivederci \n" ); 
                //fine();
                fine = 0;
                break;  
            default:
                printf("Scelta non valida. Riprova.\n");
                break;
        }
    }
    
}

void stampa_menu(){
int scelta;

    do {
        printf("Menu di scelte:\n");
        printf("1) Imposta gioco\n");
        printf("2) Gioca\n");
        printf("3) Termina gioco\n");
        printf("Scelta: ");
        
        char temp[100];
        fgets(temp, sizeof(temp), stdin);
        scelta = atoi(temp);

        switch (scelta) {
            case 1:
                imposta_gioco();
                break;
            case 2:
                    gioca();
                break;
            case 3:
                   printf("Partita terminata, Arrivederci.\n");
                break;
            default:
                printf("Scelta non valida. Riprova.\n");
                break;
        }
    } while (scelta != 3);
}


// Function to randomize play sequence order for players at the end of every turn
 int* ordine_casuale(void) {

    int* ordine_giocatori = (int*)malloc(numero_giocatori * sizeof(int));
    bool array_ordine[numero_giocatori];

    for (int i = 0; i < numero_giocatori; i++) {// Initialize array values to true for valid player counts
        ordine_giocatori[i] = i; // Sequential array setup: 0-1-2-3 (e.g., 4-player game)
        array_ordine[i] = true;  // Mark all player slots as valid
    }

    srand(time(NULL));

    int* nuovo_ordine = (int*)malloc(numero_giocatori * sizeof(int));
    for (int i = 0; i < numero_giocatori; i++) {
        int ordine;

        do {
            ordine = rand() % numero_giocatori;
        } while (!array_ordine[ordine]); // Obtain randomized turn order

        array_ordine[ordine] = false; // Mark picked index false to prevent duplicate selection
        nuovo_ordine[i] = ordine_giocatori[ordine];
    }

    free(ordine_giocatori);

    return nuovo_ordine;
}
