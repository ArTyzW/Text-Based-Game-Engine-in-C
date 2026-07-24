#ifndef GAMELIB_H
#define GAMELIB_H

#include <stdbool.h>

#define MAX_GIOCATORI 4
#define MAX_SLOT_ZAINO 4

extern int fine;

// Enum representing starting items assigned or chosen by players
enum Tipo_oggetto_iniziale {
    EMF,
    spirit_box,
    videocamera,
    calmanti,
    sale
};

// Enum representing items that can be found in map zones
enum Tipo_oggetto_zona {
    adrenalina,
    cento_dollari,
    coltello,
    nessun_oggetto
};

// Enum representing the types of rooms/zones on the map
enum Tipo_zona {
    caravan,
    cucina,
    soggiorno,
    camera,
    bagno,
    garage,
    seminterrato
};

// Enum representing collectible ghost evidence items
enum Tipo_prova {
    prova_EMF,
    prova_spirit_box,
    prova_videocamera,
    nessuna_prova
};

// Node structure for the circular linked list representing the map
struct Zona_mappa {
    enum Tipo_zona zona;
    enum Tipo_oggetto_zona oggetto_zona;
    enum Tipo_prova prova;
    struct Zona_mappa* prossima_zona;
};

// Structure representing a player's state
struct Giocatore {
    char nome_giocatore[50];
    unsigned char sanita_mentale;
    struct Zona_mappa* posizione;
    unsigned char zaino[4];  // Contains values of Tipo_oggetto_iniziale, Tipo_oggetto_zona, Tipo_prova
};

// Main game flow management functions
void imposta_gioco();
void gioca();
void termina_gioco();

// Map creation and handling functions
void inserisci_zona();
void cancella_zona();
void stampa_mappa();
void chiudi_mappa();

// Player action functions during turns
void torna_caravan(struct Giocatore *giocatore);
void stampa_giocatore(struct Giocatore *giocatore);
void stampa_zona(struct Giocatore *giocatore);
void avanza(struct Giocatore *giocatore);
void raccogli_prova(struct Giocatore *giocatore, struct Giocatore** giocatori, int numero_giocatori);
void raccogli_oggetto(struct Giocatore *giocatore);
void usa_oggetto(struct Giocatore *giocatore);
void passa(int* turno);

// Item usage functions
void uso_sale(struct Giocatore *giocatore);
void uso_calmanti(struct Giocatore *giocatore);
void uso_100_dollari(struct Giocatore *giocatore);
void uso_coltello(struct Giocatore *giocatore, struct Giocatore** giocatori, int numero_giocatori);
void uso_adrenalina(struct Giocatore *giocatore);

// Helper and mechanics functions
void decrementa_sanita_mentale(struct Giocatore *giocatore);
void dealloca_giocatori(struct Giocatore** giocatori, int numero_giocatori);
void dealloca_zone_mappa(struct Zona_mappa* pFirst);
void dealloca_tutto(struct Giocatore** giocatori, int numero_giocatori, struct Zona_mappa* pFirst);
int* ordine_casuale(void);
void stampa_menu();

#endif // GAMELIB_H
