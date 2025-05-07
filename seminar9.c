#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//trebuie sa folositi fisierul masini.txt
//sau va creati un alt fisier cu alte date

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

//creare structura pentru un nod dintr-un arbore binar de cautare

typedef struct Nod Nod;
struct Nod {
	Nod* st;
	Nod* dr;
	Masina info;
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


void adaugaMasinaInArbore(Nod** radacina, Masina masinaNoua) {
	//adauga o noua masina pe care o primim ca parametru in arbore,
	//astfel incat sa respecte princiippile de arbore binar de cautare
	//dupa o anumita cheie pe care o decideti - poate fi ID

	if ((*radacina) != NULL)
	{
		if ((*radacina)->info.id > masinaNoua.id)
			adaugaMasinaInArbore(&((*radacina)->st), masinaNoua);
		else if ((*radacina)->info.id < masinaNoua.id)
			adaugaMasinaInArbore(&((*radacina)->dr), masinaNoua);
		else
		{
			(*radacina)->info.nrUsi = masinaNoua.nrUsi;
			(*radacina)->info.pret = masinaNoua.pret;
			(*radacina)->info.serie = masinaNoua.serie;
			if ((*radacina)->info.model != NULL)
				free((*radacina)->info.model);
			if ((*radacina)->info.numeSofer != NULL)
				free((*radacina)->info.numeSofer);
			(*radacina)->info.model = (char*)malloc(strlen(masinaNoua.model) + 1);
			strcpy_s((*radacina)->info.model, strlen(masinaNoua.model) + 1, masinaNoua.model);
			(*radacina)->info.numeSofer = (char*)malloc(strlen(masinaNoua.numeSofer) + 1);
			strcpy_s((*radacina)->info.numeSofer, strlen(masinaNoua.numeSofer) + 1, masinaNoua.numeSofer);


		}
	}
	else
	{
		(*radacina) = malloc(sizeof(Nod));
		(*radacina)->info = masinaNoua;
		(*radacina)->info.model = (char*)malloc(strlen(masinaNoua.model) + 1);
		strcpy_s((*radacina)->info.model, strlen(masinaNoua.model) + 1, masinaNoua.model);
		(*radacina)->info.numeSofer = (char*)malloc(strlen(masinaNoua.numeSofer) + 1);
		strcpy_s((*radacina)->info.numeSofer, strlen(masinaNoua.numeSofer) + 1, masinaNoua.numeSofer);
		(*radacina)->st = NULL;
		(*radacina)->dr = NULL;
	}

}

Nod* citireArboreDeMasiniDinFisier(const char* numeFisier) {
	//functia primeste numele fisierului, il deschide si citeste toate masinile din fisier
	//prin apelul repetat al functiei citireMasinaDinFisier()
	//ATENTIE - la final inchidem fisierul/stream-ul
	FILE* file = fopen(numeFisier, "r");
	Nod* radacina = NULL;
	while (!feof(file))
	{
		//facem ca mai jos si nu direct in functie,  adaugaMasinaInArbore(&radacina, citireMasinaDinDisier(file))
		//deoarece daca fac ca mai sus o sa aloc de 2 ori spatiu, o sa aloc spatiu o data in citiremasina si inca
		// o data spatiu in adaugaMasinaInArbore pentru ca am facut deepcopy

		Masina m = citireMasinaDinFisier(file);
		adaugaMasinaInArbore(&radacina, m);
		free(m.model);
		free(m.numeSofer);
	}
	fclose(file);
	return radacina;
}

void afisareArborePreOrdine(Nod* radacina) {
	//afiseaza toate elemente de tip masina din arborele creat
	//prin apelarea functiei afisareMasina()
	//parcurgerea arborelui poate fi realizata in TREI moduri
	//folositi toate cele TREI moduri de parcurgere

	//inordine -> SRD
	//postordine -> SDR
	//preordine -> RSD
	//in/post/pre ne zice pozitia lu R, mereu punem SD ca suntem europeni

	//o sa folosim preordine (RSD)
	if (radacina != NULL)
	{
		afisareMasina(radacina->info);
		afisareArborePreOrdine(radacina->st);
		afisareArborePreOrdine(radacina->dr);

	}
}

void afisareArboreInOrdine(Nod* radacina) {

	if (radacina != NULL) {
		afisareArboreInOrdine(radacina->st);
		afisareMasina(radacina->info);
		afisareArboreInOrdine(radacina->dr);

	}
}

void dezalocareArboreDeMasini(Nod** radacina) {
	//sunt dezalocate toate masinile si arborele de elemente
	if ((*radacina) != NULL)
	{
		dezalocareArboreDeMasini(&(*radacina)->st);
		dezalocareArboreDeMasini(&(*radacina)->dr);
		free((*radacina)->info.model);
		free((*radacina)->info.numeSofer);
		free(*radacina);
		*radacina = NULL;
	}
}

Masina getMasinaByID(Nod* radacina, int id) {
	Masina m;
	m.id = -1;
	if (id > radacina->info.id)
	{
		return getMasinaByID(radacina->dr, id);
	}
	else if (id < radacina->info.id)
	{
		return getMasinaByID(radacina->st, id);
	}
	else
	{
		m = radacina->info;
		m.numeSofer = (char*)malloc(strlen(radacina->info.numeSofer) + 1);
		strcpy_s(m.numeSofer, strlen(radacina->info.numeSofer) + 1, radacina->info.numeSofer);
		m.model = (char*)malloc(strlen(radacina->info.model) + 1);
		strcpy_s(m.model, strlen(radacina->info.model) + 1, radacina->info.model);

		return m;


	}
}

int determinaNumarNoduri(Nod* radacina) {
	//calculeaza numarul total de noduri din arborele binar de cautare
	if (radacina != NULL)
		return 1 + determinaNumarNoduri(radacina->st) + determinaNumarNoduri(radacina->dr);
	else return 0;
}

int calculeazaInaltimeArbore(Nod* radacina) {
	//calculeaza inaltimea arborelui care este data de 
	//lungimea maxima de la radacina pana la cel mai indepartat nod frunza
	Nod* x = radacina;
	if (radacina != NULL)
	{
		if (x->dr != NULL && x->st != NULL)
		{

			return 1 + max(calculeazaInaltimeArbore(x->dr), calculeazaInaltimeArbore(x->st));

		}
		else
			if (x->dr != NULL)
				return 1 + calculeazaInaltimeArbore(x->dr);
			else
				if (x->st != NULL)
					return 1 + calculeazaInaltimeArbore(x->st);
				else
					return 1;
	}
	else
		return 0;
}

int calculeazaInaltimeArbore2(Nod* radacina) {

	if (radacina == NULL)
		return 0;

	return 1 + max(calculeazaInaltimeArbore2(radacina->dr), calculeazaInaltimeArbore2(radacina->st));
}



float calculeazaPretTotal(Nod* radacina) {
	//calculeaza pretul tuturor masinilor din arbore.
	if (radacina != NULL)
	{
		return radacina->info.pret + calculeazaPretTotal(radacina->dr) + calculeazaPretTotal(radacina->st);
	}
	else
		return 0;
}

float calculeazaPretulMasinilorUnuiSofer(Nod* radacina, const char* numeSofer) {
	//calculeaza pretul tuturor masinilor unui sofer.
	if (radacina != NULL)
	{
		if (strcmp(radacina->info.numeSofer, numeSofer) == 0)
			return radacina->info.pret + calculeazaPretulMasinilorUnuiSofer(radacina->dr, numeSofer) + calculeazaPretulMasinilorUnuiSofer(radacina->st, numeSofer);
		else
			return calculeazaPretulMasinilorUnuiSofer(radacina->dr, numeSofer) + calculeazaPretulMasinilorUnuiSofer(radacina->st, numeSofer);
	}
	else
		return 0;
}

int main() {

	Nod* arbore = citireArboreDeMasiniDinFisier("masini_arbore.txt");
	afisareArboreInOrdine(arbore);
	Masina m = getMasinaByID(arbore, 2);
	printf("\n\n-----------------------\n");
	afisareMasina(m);

	int i = determinaNumarNoduri(arbore);
	printf("\n\nNumar noduri: %d", i);

	printf("\n\nInaltime arbore 1: %d", calculeazaInaltimeArbore(arbore));
	printf("\n\n\Inaltime arbore 2: %d", calculeazaInaltimeArbore2(arbore));
	printf("\n\n\Pret total: %.2f", calculeazaPretTotal(arbore));
	printf("\n\n\Pret total per sofer: %.2f", calculeazaPretulMasinilorUnuiSofer(arbore, "Ionescu"));

	dezalocareArboreDeMasini(&arbore);

	return 0;
}