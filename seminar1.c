#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct Masina {
    int id;
    float capacitateC;
    int nrLocuri;
    char normaPoluare;
    char* marca;
};

struct Masina initializare(int id, int nrLocuri, const char* marca, float capacitateC, char normaPoluare) {
    struct Masina m;
    m.id = id;
    m.nrLocuri = nrLocuri;
    m.capacitateC = capacitateC;
    m.normaPoluare = normaPoluare;
    m.marca = (char*)malloc(strlen(marca) + 1);
    strcpy_s(m.marca, strlen(marca) + 1, marca);
    return m;

}

void afisare(struct Masina m)
{
    printf("ID: %d\n", m.id);
    printf("Marca: %s\n", m.marca);
    printf("Numar locuri: %d\n", m.nrLocuri);
    printf("Capacitate cilindrica: %f\n", m.capacitateC);
    printf("Norma poluare: Euro %c\n\n", m.normaPoluare);
}

void modifica_nrLocuri(struct Masina* m, int nrNou)
{
    if (nrNou > 0)
        m->nrLocuri = nrNou;
}


void dezalocare(struct Masina* m) {
    free(m->marca);
    m->marca = NULL;
}

int main() {
    struct Masina masina = initializare(1, 5, "Dacia", 1.5, '4');
    afisare(masina);
    modifica_nrLocuri(&masina, 400);
    printf("--------------------------------------------------");
    afisare(masina);
    dezalocare(&masina);
    afisare(masina);
    printf("%d \n", sizeof(char*));
    printf("%d", sizeof(masina));

    return 0;

}