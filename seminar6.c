#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// chaining, liniar probing, quadratic probing
// double hashing, rehashing
// rehashing - cand avem toate pozitiile ocupate si mai avem de adaugat, facem o noua dimensiune si rehashuim
//mecanisme de evitare a coliziunilor
//linear probing = probam liniar fiecare pozitie, sa vedem daca este ocupata
//quadratic probing = risipim elementele si le dau sa fie mai indepartate
//double hashing
//chaining - in fiecare element al vectorului e un pointer. Cand e coliziune mai adaug un nod

typedef struct StructuraMasina Masina;
struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};

typedef struct Nod Nod;
struct Nod {
	Masina info;
	Nod* next;
};

typedef struct HashTable HashTable;
struct HashTable {
	int dim;
	Nod** vector; // vector de pointeri
};

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
	m1.model = (char*)malloc(strlen(aux) + 1);
	strcpy_s(m1.model, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	m1.numeSofer = (char*)malloc(strlen(aux) + 1);
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
		cap = cap->next;
	}
}

void adaugaMasinaInLista(Nod* cap, Masina masinaNoua) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = masinaNoua;
	nou->next = NULL;
	while (cap->next) {
		cap = cap->next;
	}
	cap->next = nou;
}


HashTable initializareHashTable(int dimensiune) {
	HashTable ht;
	ht.dim = dimensiune;
	ht.vector = (Nod*)malloc(sizeof(Nod) * dimensiune);
	for (int i = 0; i < dimensiune; i++) {
		ht.vector[i] = NULL;
	}
	return ht;
}

int calculeazaHash(const char* model, int dimensiune) {
	int suma = 0;
	for (int i = 0; i < strlen(model); i++) {
		suma += model[i];
	}
	return suma % dimensiune;
}

void inserareMasinaInTabela(HashTable hash, Masina masina) {
	int pozitie = calculeazaHash(masina.model, hash.dim);
	if (hash.vector[pozitie] != NULL) { // ? coliziune
		adaugaMasinaInLista(hash.vector[pozitie], masina);
	}
	else {
		hash.vector[pozitie] = (Nod*)malloc(sizeof(Nod));
		hash.vector[pozitie]->info = masina;
		hash.vector[pozitie]->next = NULL;
	}
}

HashTable citireMasiniDinFisier(const char* numeFisier, int dimensiune) {
	FILE* file = fopen(numeFisier, "r");
	HashTable hash = initializareHashTable(dimensiune);
	while (!feof(file)) {
		Masina masina = citireMasinaDinFisier(file);
		inserareMasinaInTabela(hash, masina);
	}
	fclose(file);
	return hash;
}

void afisareTabelaDeMasini(HashTable ht) {
	for (int i = 0; i < ht.dim; i++) {
		printf("\nClusterul %d:\n", i);
		afisareListaMasini(ht.vector[i]);
	}
}

void dezalocareTabelaDeMasini(HashTable* ht) {
	if (ht == NULL || ht->vector == NULL)
		return;

	for (int i = 0; i < ht->dim; i++) {
		Nod* p = ht->vector[i];
		while (p) {
			Nod* temp = p;
			p = p->next;
			free(temp->info.model);
			free(temp->info.numeSofer);
			free(temp);
		}
	}
	free(ht->vector);
	ht->vector = NULL;
	ht->dim = 0;
}


float* calculeazaPreturiMediiPerClustere(HashTable ht, int* nrClustere) {
	float* preturiMedii = (float*)malloc(sizeof(float) * ht.dim);
	*nrClustere = 0;

	for (int i = 0; i < ht.dim; i++) {
		Nod* p = ht.vector[i];
		int count = 0;
		float suma = 0;

		while (p) {
			suma += p->info.pret;
			count++;
			p = p->next;
		}

		if (count > 0) {
			preturiMedii[*nrClustere] = suma / count;
			(*nrClustere)++;
		}
	}

	preturiMedii = (float*)realloc(preturiMedii, sizeof(float) * (*nrClustere));
	return preturiMedii;
}


Masina getMasinaDupaCheie(HashTable ht, const char* model) {

	int poz = calculeazaHash(model, ht.dim);
	Masina masina;
	masina.id = -1; 
	if (ht.vector[poz] != NULL) {
		Nod* p = ht.vector[poz];
		while (p != NULL && strcmp(p->info.model, model) != 0) {
			p = p->next;
		}
		if (p != NULL) {
			return p->info;
		}
	}
	return masina;
}

int main() {
	HashTable hash = citireMasiniDinFisier("masini.txt", 10);
	afisareTabelaDeMasini(hash);
	printf("\n\nMasina de model Mokka: \n");
	Masina m = getMasinaDupaCheie(hash, "Mokka");
	afisareMasina(m);
	printf("\n");

	int nrClustere;
	float* medii = calculeazaPreturiMediiPerClustere(hash, &nrClustere);
	printf("\nPreturi medii per cluster:\n");
	for (int i = 0; i < nrClustere; i++) {
		printf("Cluster %d: %.2f\n", i, medii[i]);
	}
	free(medii);

	dezalocareTabelaDeMasini(&hash);
	return 0;
}