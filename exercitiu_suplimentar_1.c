#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct MelodieSimfonica {
    int id;
    char* titlu;
    char* compozitor;
    int anCompozitie;
    float* durataPeParagrafe;
    int nrParagrafe;
};

struct MelodieSimfonica citesteMelodieSimfonica() {
    struct MelodieSimfonica melodie;
    char buffer[100];
    
    printf("Introduceti ID-ul melodiei: ");
    scanf("%d", &melodie.id);
    getchar();
    
    printf("Introduceti titlul melodiei: ");
    fgets(buffer, 100, stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    
    melodie.titlu = (char*)malloc(strlen(buffer) + 1);
    strcpy(melodie.titlu, buffer);
    
    printf("Introduceti numele compozitorului: ");
    fgets(buffer, 100, stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    
    melodie.compozitor = (char*)malloc(strlen(buffer) + 1);
    strcpy(melodie.compozitor, buffer);
    
    printf("Introduceti anul compozitiei: ");
    scanf("%d", &melodie.anCompozitie);
    
    printf("Introduceti numarul de paragrafe: ");
    scanf("%d",&melodie.nrParagrafe);
    
    melodie.durataPeParagrafe = (float*)malloc(melodie.nrParagrafe * sizeof(float));
    
    for(int i = 0; i < melodie.nrParagrafe; i++) {
        printf("Introduceti durata pentru paragraful %d (minute): ", i + 1);
        scanf("%f",&melodie.durataPeParagrafe[i]);
    }
    
    return melodie;
}

float calculeazaDurataTotala(struct MelodieSimfonica melodie) {
    float durataTotala = 0;
    for(int i=0; i<melodie.nrParagrafe;i++) {
        durataTotala+=melodie.durataPeParagrafe[i];
    }
    return durataTotala;
}

void modificaTitlu(struct MelodieSimfonica* melodie, char* titluNou) {
    free(melodie->titlu);
    melodie->titlu = (char*)malloc(strlen(titluNou)+ 1);
    strcpy(melodie->titlu,titluNou);
}

void afiseazaMelodieSimfonica(struct MelodieSimfonica melodie) {
    printf("\nInformatii Melodie Simfonica:\n");
    printf("ID: %d\n", melodie.id);
    printf("Titlu: %s\n", melodie.titlu);
    printf("Compozitor: %s\n", melodie.compozitor);
    printf("An compozitie: %d\n", melodie.anCompozitie);
    printf("Numar paragrafe: %d\n",melodie.nrParagrafe);
    printf("Durate pe paragrafe:\n");
    
    for (int i = 0; i<melodie.nrParagrafe; i++) {
        printf("  - Paragraful %d: %.2f minute\n", i+1, melodie.durataPeParagrafe[i]);
    }
    
    printf("Durata totala: %.2f minute\n",calculeazaDurataTotala(melodie));
}

void elibereazaMemorie(struct MelodieSimfonica* melodie) {
    free(melodie->titlu);
    free(melodie->compozitor);
    free(melodie->durataPeParagrafe);
}

int main() {
    struct MelodieSimfonica melodie = citesteMelodieSimfonica();
    
    afiseazaMelodieSimfonica(melodie);
    
    char titluNou[100];
    printf("\nIntroduceti noul titlu al melodiei: ");
    getchar();
    fgets(titluNou, 100, stdin);
    titluNou[strcspn(titluNou, "\n")] = '\0';
    
    modificaTitlu(&melodie, titluNou);
    
    printf("\nDupa modificarea titlului:\n");
    afiseazaMelodieSimfonica(melodie);
    
    elibereazaMemorie(&melodie);
    
    return 0;
}