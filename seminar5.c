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

typedef struct Nod {
	Masina info;
	struct Nod* prev;
	struct Nod* next;
} Nod;

typedef struct Lista {
	Nod* prim;
	Nod* ultim;
} Lista;

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
	strcpy(m1.model, aux);

	aux = strtok(NULL, sep);
	m1.numeSofer = malloc(strlen(aux) + 1);
	strcpy(m1.numeSofer, aux);

	m1.serie = *strtok(NULL, sep);
	return m1;
}

void afisareMasina(Masina masina) {
	printf("Id: %d\n", masina.id);
	printf("Nr. usi: %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

void afisareListaMasini(Lista list) {
	Nod* p = list.prim;
	while (p) {
		afisareMasina(p->info);
		p = p->next;
	}
}

void adaugaMasinaInLista(Lista* list, Masina masinaNoua) {
	Nod* p = (Nod*)malloc(sizeof(Nod));
	p->info = masinaNoua;
	p->prev = list->ultim;
	p->next = NULL;
	if (list->ultim) {
		list->ultim->next = p;
	}
	else {
		list->prim = p;
	}
	list->ultim = p;
}

void adaugaLaInceputInLista(Lista* list, Masina masinaNoua) {
	Nod* p = (Nod*)malloc(sizeof(Nod));
	p->info = masinaNoua;
	p->prev = NULL;
	p->next = list->prim;
	if (list->prim) {
		list->prim->prev = p;
	}
	else {
		list->ultim = p;
	}
	list->prim = p;
}

Lista citireLDMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	Lista list;
	list.prim = NULL;
	list.ultim = NULL;
	if (!f) {
		printf("Eroare la deschiderea fisierului!\n");
		return list;
	}
	while (!feof(f)) {
		Masina masina = citireMasinaDinFisier(f);
		adaugaMasinaInLista(&list, masina);
	}
	fclose(f);
	return list;
}

void dezalocareLDMasini(Lista* list) {
	Nod* p = list->prim;
	while (p) {
		Nod* aux = p;
		p = p->next;
		free(aux->info.model);
		free(aux->info.numeSofer);
		free(aux);
	}
	list->prim = NULL;
	list->ultim = NULL;
}

float calculeazaPretMediu(Lista list) {
	if (!list.prim) return 0;
	float suma = 0;
	int count = 0;
	Nod* p = list.prim;
	while (p) {
		suma += p->info.pret;
		count++;
		p = p->next;
	}
	return (count > 0) ? suma / count : 0;
}

void stergeMasinaDupaID(Lista* list, int id) {
	Nod* p = list->prim;
	while (p) {
		if (p->info.id == id) {
			if (p->prev) {
				p->prev->next = p->next;
			}
			else {
				list->prim = p->next;
			}
			if (p->next) {
				p->next->prev = p->prev;
			}
			else {
				list->ultim = p->prev;
			}
			free(p->info.model);
			free(p->info.numeSofer);
			free(p);
			return;
		}
		p = p->next;
	}
	printf("Masina cu ID %d nu a fost gasita.\n", id);
}

char* getNumeSoferMasinaScumpa(Lista list) {
	if (!list.prim) return NULL;
	Nod* p = list.prim;
	Nod* maxMasina = p;
	while (p) {
		if (p->info.pret > maxMasina->info.pret) {
			maxMasina = p;
		}
		p = p->next;
	}
	char* numeSofer = (char*)malloc(strlen(maxMasina->info.numeSofer) + 1);
	strcpy(numeSofer, maxMasina->info.numeSofer);
	return numeSofer;
}

int main() {
	Lista lista = citireLDMasiniDinFisier("masini.txt");

	printf("Lista initiala de masini:\n");
	afisareListaMasini(lista);

	printf("Pretul mediu al masinilor: %.2f\n", calculeazaPretMediu(lista));

	char* sofer = getNumeSoferMasinaScumpa(lista);
	if (sofer) {
		printf("Numele soferului celei mai scumpe masini: %s\n", sofer);
		free(sofer);
	}

	int idDeSters = 2;
	stergeMasinaDupaID(&lista, idDeSters);
	printf("\nLista dupa stergerea masinii cu ID %d:\n", idDeSters);
	afisareListaMasini(lista);

	dezalocareLDMasini(&lista);
	return 0;
}