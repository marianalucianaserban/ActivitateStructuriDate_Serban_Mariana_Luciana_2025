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
	Masina info;
	Nod* st;
	Nod* dr;
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

int calculeazaInaltimeArbore(Nod* radacina) {
	//calculeaza inaltimea arborelui care este data de 
	//lungimea maxima de la radacina pana la cel mai indepartat nod frunza
	if (radacina != NULL)
	{
		int inaltimeSt = calculeazaInaltimeArbore(radacina->st);
		int inaltimeDr = calculeazaInaltimeArbore(radacina->dr);
		return 1 + max(inaltimeDr, inaltimeSt); // return 1+(inaltimeSt> inaltimeDr ? inaltimeSt : inaltimeDr);
	}
	else
		return 0;
}

//ALTE FUNCTII NECESARE:
// - aici veti adauga noile functii de care aveti nevoie.
void rotireStanga(Nod** radacina)
{
	Nod* nod;
	nod = (*radacina)->dr;
	(*radacina)->dr = nod->st;
	nod->st = *radacina;
	*radacina = nod;

}

void rotireDreapta(Nod** radacina)
{
	Nod* nod;
	nod = (*radacina)->st;
	(*radacina)->st = nod->dr;
	nod->dr = *radacina;
	*radacina = nod;
}

int calculeazaGradEchilibru(Nod* radacina)
{
	if (radacina != NULL)
		return (calculeazaInaltimeArbore(radacina->st) - calculeazaInaltimeArbore(radacina->dr));
	else
		return 0;
}

void adaugaMasinaInArboreEchilibrat(Nod** radacina, Masina masinaNoua) {
	//adauga o noua masina pe care o primim ca parametru in arbore,
	//astfel incat sa respecte principiile de arbore binar de cautare ECHILIBRAT
	//dupa o anumita cheie pe care o decideti - poate fi ID
	if ((*radacina) != NULL)
	{
		if ((*radacina)->info.id > masinaNoua.id)
		{
			adaugaMasinaInArboreEchilibrat(&((*radacina)->st), masinaNoua);
		}
		else {
			adaugaMasinaInArboreEchilibrat(&((*radacina)->dr), masinaNoua);
		}
		int grad = calculeazaGradEchilibru((*radacina));
		if (grad == 2)//dezechilibru stanga
		{
			if (calculeazaGradEchilibru((*radacina)->st) == 1)
				rotireDreapta(radacina);
			else {
				rotireStanga(&((*radacina)->st));
				rotireDreapta(radacina);
			}

		}
		else if (grad == -2)
		{
			if (calculeazaGradEchilibru((*radacina)->dr) == 1)
			{
				rotireDreapta(&((*radacina)->dr));
			}
			rotireStanga(radacina);
		}

	}
	else
	{
		Nod* nod = (Nod*)malloc(sizeof(Nod));
		nod->info = masinaNoua;
		nod->dr = NULL;
		nod->st = NULL;
		*radacina = nod;
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
		adaugaMasinaInArboreEchilibrat(&radacina, m);
	}
	fclose(file);
	return radacina;
}

void afisareMasiniDinArborePreordine(Nod* radacina) {
	//afiseaza toate elemente de tip masina din arborele creat
	//prin apelarea functiei afisareMasina()
	//parcurgerea arborelui poate fi realizata in TREI moduri
	//folositi toate cele TREI moduri de parcurgere
	if (radacina != NULL)
	{
		afisareMasina(radacina->info);
		afisareMasiniDinArborePreordine(radacina->st);
		afisareMasiniDinArborePreordine(radacina->dr);

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

//Preluati urmatoarele functii din laboratorul precedent.
//Acestea ar trebuie sa functioneze pe noul arbore echilibrat.

int determinaNumarNoduri(/*arborele de masini*/);

float calculeazaPretTotal(Nod* radacina)
{
	if (radacina != NULL)
	{
		return radacina->info.pret + calculeazaPretTotal(radacina->dr) + calculeazaPretTotal(radacina->st);
	}
	else
		return 0;
}

float calculeazaPretulMasinilorUnuiSofer(/*arbore de masini*/ const char* numeSofer);
Masina getMasinaByID(/*arborele de masini*/int id);

int main() {

	Nod* arbore = citireArboreDeMasiniDinFisier("masini_arbore.txt");
	afisareMasiniDinArborePreordine(arbore);
	printf("Pretul total: %.2f", calculeazaPretTotal(arbore));
	return 0;
}