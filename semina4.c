#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraMasina {
    int id;
    int nrUsi;
    float pret;
    char* model;
    char* numeSofer;
    unsigned char serie;
};
typedef struct StructuraMasina Masina;

struct Nod {
    Masina info;
    struct Nod* urm;
};
typedef struct Nod Nod;

Masina citireMasinaDinFisier(FILE* file) {
    char buffer[100];
    char sep[3] = ",\n";
    fgets(buffer, 100, file);
    char* aux;
    Masina m1;
    
    aux = strtok(buffer, sep);
    m1.id = atoi(aux);
    
    m1.nrUsi = atoi(strtok(NULL, sep));
    m1.pret = atof(strtok(NULL, sep));
    
    aux = strtok(NULL, sep);
    m1.model = malloc(strlen(aux) + 1);
    strcpy_s(m1.model, strlen(aux) + 1, aux);

    aux = strtok(NULL, sep);
    m1.numeSofer = malloc(strlen(aux) + 1);
    strcpy_s(m1.numeSofer, strlen(aux) + 1, aux);

    m1.serie = *strtok(NULL, sep);
    
    return m1;
}

void afisareMasina(Masina masina) {
    printf("Id: %d\n", masina.id);
    printf("Nr. usi : %d\n", masina.nrUsi);
    printf("Pret: %.2f\n", masina.pret);
    printf("Model: %s\n", masina.model);
    printf("Nume sofer: %s\n", masina.numeSofer);
    printf("Serie: %c\n\n", masina.serie);
}

void afisareListaMasini(Nod* cap) {
    while (cap) {
        afisareMasina(cap->info);
        cap = cap->urm;
    }
}

void adaugaMasinaInLista(Nod** cap, Masina masinaNoua) {
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->info = masinaNoua;
    nou->urm = NULL;
    
    if ((*cap) == NULL) {
        *cap = nou;
    } else {
        Nod* p = *cap;
        while (p->urm) {
            p = p->urm;
        }
        p->urm = nou;
    }
}

void adaugaLaInceputInLista(Nod** cap, Masina masinaNoua) {
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->info = masinaNoua;
    nou->urm = *cap;
    *cap = nou;
}

Nod* citireListaMasiniDinFisier(const char* numeFisier) {
    FILE* f = fopen(numeFisier, "r");
    Nod* lista = NULL;
    
    while (!feof(f)) {
        adaugaLaInceputInLista(&lista, citireMasinaDinFisier(f));
    }
    
    fclose(f);
    return lista;
}

void dezalocareListaMasini(Nod** cap) {
    while ((*cap) != NULL) {
        Nod* aux = *cap;
        *cap = (*cap)->urm;
        free(aux->info.numeSofer);
        free(aux->info.model);
        free(aux);
    }
}

float calculeazaPretMediu(Nod* lista) {
    if (!lista)
        return 0;
    
    int nr = 0;
    float sum = 0;
    
    while (lista) {
        sum += lista->info.pret;
        nr++;
        lista = lista->urm;
    }
    
    return sum / nr;
}

void stergeMasiniDinSeria(Nod** lista, char serieCautata) {
    if (!lista || !(*lista))
        return;
        
    while (*lista && (*lista)->info.serie == serieCautata) {
        Nod* temp = *lista;
        *lista = (*lista)->urm;
        free(temp->info.model);
        free(temp->info.numeSofer);
        free(temp);
    }
    
    if (!(*lista))
        return;
        
    Nod* anterior = *lista;
    Nod* curent = (*lista)->urm;
    
    while (curent) {
        if (curent->info.serie == serieCautata) {
            anterior->urm = curent->urm;
            free(curent->info.model);
            free(curent->info.numeSofer);
            free(curent);
            curent = anterior->urm;
        } else {
            anterior = curent;
            curent = curent->urm;
        }
    }
}

float calculeazaPretulMasinilorUnuiSofer(Nod* lista, const char* numeSofer) {
    float sum = 0;
    
    while (lista) {
        if (strcmp(numeSofer, lista->info.numeSofer) == 0)
            sum += lista->info.pret;
        lista = lista->urm;
    }

    return sum;
}

char* getCeaMaiScumpaMasina(Nod* lista) {
    float pretMax = 0;
    char* modelScump = NULL;
    
    while (lista) {
        if (lista->info.pret > pretMax) {
            pretMax = lista->info.pret;
            modelScump = lista->info.model;
        }
        lista = lista->urm;
    }
    
    if (pretMax > 0) {
        char* nou = malloc(strlen(modelScump) + 1);
        strcpy_s(nou, strlen(modelScump) + 1, modelScump);
        return nou;
    }
    
    return NULL;
}

int main() {
    Nod* cap = NULL;
    cap = citireListaMasiniDinFisier("masini.txt");
    
    afisareListaMasini(cap);
    printf("Pret mediu: %.2f\n", calculeazaPretMediu(cap));
    printf("Pret total masini Ionescu: %.2f\n", calculeazaPretulMasinilorUnuiSofer(cap, "Ionescu"));
    
    char* modelScump = getCeaMaiScumpaMasina(cap);
    printf("Cea mai scumpa masina: %s\n", modelScump);
    free(modelScump);
    
    dezalocareListaMasini(&cap);

    return 0;
}