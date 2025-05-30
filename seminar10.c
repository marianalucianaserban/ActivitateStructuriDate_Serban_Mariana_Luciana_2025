#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
} Masina;

typedef struct Nod {
	Masina info;
	struct Nod* stanga;
	struct Nod* dreapta;
	int gradacinaEchilibru;
} Nod;


Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	if (!fgets(buffer, 100, file)) return (Masina) { 0 };

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
	printf("Id: %d\nNr. usi: %d\nPret: %.2f\nModel: %s\nNume sofer: %s\nSerie: %c\n\n",
		masina.id, masina.nrUsi, masina.pret, masina.model, masina.numeSofer, masina.serie);
}


int inaltime(Nod* radacina) {
	if (!radacina) return 0;
	int hSt = inaltime(radacina->stanga);
	int hDr = inaltime(radacina->dreapta);
	return 1 + (hSt > hDr ? hSt : hDr);
}

void rotireStanga(Nod** radacinaacina) {
	Nod* aux = (*radacinaacina)->dreapta;
	(*radacinaacina)->dreapta = aux->stanga;
	aux->stanga = (*radacinaacina);
	(*radacinaacina) = aux;
}

void rotireDreapta(Nod** radacinaacina) {
	Nod* aux = (*radacinaacina)->stanga;
	(*radacinaacina)->stanga = aux->dreapta;
	aux->dreapta = (*radacinaacina);
	(*radacinaacina) = aux;
}


void adaugaMasinaInArboreEchilibrat(Nod** radacinaacina, Masina m) {
	if (*radacinaacina == NULL) {
		Nod* nou = (Nod*)malloc(sizeof(Nod));
		nou->info = m;
		nou->stanga = nou->dreapta = NULL;
		nou->gradacinaEchilibru = 0;
		*radacinaacina = nou;
	}
	else if (m.id < (*radacinaacina)->info.id) {
		adaugaMasinaInArboreEchilibrat(&(*radacinaacina)->stanga, m);
	}
	else {
		adaugaMasinaInArboreEchilibrat(&(*radacinaacina)->dreapta, m);
	}

	int st = inaltime((*radacinaacina)->stanga);
	int dr = inaltime((*radacinaacina)->dreapta);
	(*radacinaacina)->gradacinaEchilibru = st - dr;


	if ((*radacinaacina)->gradacinaEchilibru > 1) {
		if (m.id < (*radacinaacina)->stanga->info.id)
			rotireDreapta(radacinaacina);
		else {
			rotireStanga(&(*radacinaacina)->stanga);
			rotireDreapta(radacinaacina);
		}
	}
	else if ((*radacinaacina)->gradacinaEchilibru < -1) {
		if (m.id > (*radacinaacina)->dreapta->info.id)
			rotireStanga(radacinaacina);
		else {
			rotireDreapta(&(*radacinaacina)->dreapta);
			rotireStanga(radacinaacina);
		}
	}
}

Nod* citireArboreDeMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	if (!f) return NULL;
	Nod* radacina = NULL;
	while (!feof(f)) {
		Masina m = citireMasinaDinFisier(f);
		if (m.model) adaugaMasinaInArboreEchilibrat(&radacina, m);
	}
	fclose(f);
	return radacina;
}

void afisareMasiniDinArborePreOrdine(Nod* radacina) {
	if (radacina) {
		afisareMasina(radacina->info);
		afisareMasiniDinArborePreOrdine(radacina->stanga);
		afisareMasiniDinArborePreOrdine(radacina->dreapta);
	}
}

void dezalocareArboreDeMasini(Nod* radacina) {
	if (radacina) {
		dezalocareArboreDeMasini(radacina->stanga);
		dezalocareArboreDeMasini(radacina->dreapta);
		free(radacina->info.model);
		free(radacina->info.numeSofer);
		free(radacina);
	}
}

Masina getMasinaByID(Nod* radacina, int id) {
	if (!radacina) 
		return (Masina) { 
		.model = NULL 
	};

	if (radacina->info.id == id) 
		return radacina->info;

	if (id < radacina->info.id) 
		return getMasinaByID(radacina->stanga, id);

	else return getMasinaByID(radacina->dreapta, id);
}

int determinaNumarNoduri(Nod* radacina) {
	if (!radacina) 
		return 0;

	return 1 + determinaNumarNoduri(radacina->stanga) + determinaNumarNoduri(radacina->dreapta);
}

float calculeazaPretTotal(Nod* radacina) {
	if (!radacina) return 0;
	return radacina->info.pret + calculeazaPretTotal(radacina->stanga) + calculeazaPretTotal(radacina->dreapta);
}

float calculeazaPretulMasinilorUnuiSofer(Nod* radacina, const char* sofer) {
	if (!radacina) return 0;
	float s = strcmp(radacina->info.numeSofer, sofer) == 0 ? radacina->info.pret : 0;

	return s + calculeazaPretulMasinilorUnuiSofer(radacina->stanga, sofer) + calculeazaPretulMasinilorUnuiSofer(radacina->dreapta, sofer);
}

int main() {
	Nod* arbore = citireArboreDeMasiniDinFisier("masini.txt");

	printf("Afisare preordine:\n");
	afisareMasiniDinArborePreOrdine(arbore);

	printf("Numar total noduri: %d\n", determinaNumarNoduri(arbore));
	printf("Pret total masini: %.2f\n", calculeazaPretTotal(arbore));
	printf("\nInaltime arbore: %d\n", inaltime(arbore));


	int idCautat = 2;

	Masina m = getMasinaByID(arbore, idCautat);
	if (m.model) {
		printf("\nMasina cu ID %d:\n", idCautat);
		afisareMasina(m);
	}
	else {
		printf("\nMasina cu ID %d nu a fost gasita\n", idCautat);
	}

	printf("Pretul total al masinilor soferului Ionescu %.2f\n", calculeazaPretulMasinilorUnuiSofer(arbore, "Ionescu"));

	dezalocareArboreDeMasini(arbore);
	return 0;

}