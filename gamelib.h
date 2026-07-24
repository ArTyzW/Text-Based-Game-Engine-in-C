#ifndef GAMELIB_H
#define GAMELIB_H
#include <stdbool.h>
#define MAX_GIOCATORI 4
#define MAX_SLOT_ZAINO 4
extern int fine;
enum Tipo_oggetto_iniziale {
    EMF,
    spirit_box,
    videocamera,
    calmanti,
    sale
};

enum Tipo_oggetto_zona {
    adrenalina,
    cento_dollari,
    coltello,
    nessun_oggetto
};

enum Tipo_zona {
    caravan,
    cucina,
    soggiorno,
    camera,
    bagno,
    garage,
    seminterrato
};

enum Tipo_prova {
    prova_EMF,
    prova_spirit_box,
    prova_videocamera,
    nessuna_prova
};

struct Zona_mappa {
    enum Tipo_zona zona;
    enum Tipo_oggetto_zona oggetto_zona;
    enum Tipo_prova prova;
    struct Zona_mappa* prossima_zona;
};

struct Giocatore {
    char nome_giocatore[50];
    unsigned char sanita_mentale;
    struct Zona_mappa* posizione;
    unsigned char zaino[4];  // Contiene i valori di Tipo_oggetto_iniziale, Tipo_oggetto_zona, Tipo_prova
};

void imposta_gioco();
void gioca();
void termina_gioco();
void inserisci_zona();
void cancella_zona();
void stampa_mappa();
void chiudi_mappa();
void torna_caravan(struct Giocatore *giocatore);
void stampa_giocatore(struct Giocatore *giocatore);
void stampa_zona(struct Giocatore *giocatore);
void avanza(struct Giocatore *giocatore);
void raccogli_prova(struct Giocatore *giocatore, struct Giocatore** giocatori, int numero_giocatori);
void raccogli_oggetto(struct Giocatore *giocatore);
void usa_oggetto(struct Giocatore *giocatore);
void passa(int* turno);
void uso_sale(struct Giocatore *giocatore);
void uso_calmanti(struct Giocatore *giocatore);
void uso_100_dollari(struct Giocatore *giocatore);
void uso_coltello(struct Giocatore *giocatore, struct Giocatore** giocatori, int numero_giocatori);
void uso_adrenalina(struct Giocatore *giocatore);
void decrementa_sanita_mentale(struct Giocatore *giocatore);
void dealloca_giocatori(struct Giocatore** giocatori, int numero_giocatori);
void dealloca_zone_mappa(struct Zona_mappa* pFirst);
void dealloca_tutto(struct Giocatore** giocatori, int numero_giocatori, struct Zona_mappa* pFirst);
int* ordine_casuale(void);
void stampa_menu();
#endif // GAMELIB_H
