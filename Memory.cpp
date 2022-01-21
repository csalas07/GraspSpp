#include "Memory.h"
#include <stdlib.h>
#include <tchar.h>
#include <time.h>
#include <iostream>

void Memory::abortar(int numErr, char msg[]) {
	char date[10];
	char time[10];
	_strdate_s(date);
	_strdate_s(time);

	FILE* log;
	fopen_s(&log,"error.log", "a");

	switch (numErr) {
	case 1:
		fprintf_s(log, "%s\t%s\tError al abrir el archivo \"%s\"\n", date, time, msg);
		printf("\n%s\t%s\t%s\n", date, time, msg);
		break;
	case 2:
		fprintf_s(log, "%s\t%s\t%s\n", date, time, msg);
		printf("\n%s\t%s\t%s\n", date, time, msg);
		break;
	}

	fclose(log);
	exit(6);
}

int* Memory::ReserveVectorInt(int size) {
	int i;
	int* aux;

	aux = (int*)calloc(size, sizeof(int));

	if (!aux)
		i = 0;
	//abortar(2,(char*)"Error al reservar memoria a un vector int");


	return aux;
}

int** Memory::ReseveBiVectorInt(int row, int columns) {
	int** aux;
	int i;

	aux = (int**)calloc(row, sizeof(int*));

	for (i = 0; i < row; i++) {
		aux[i] = (int*)calloc(columns, sizeof(int));
	}

	if (!aux)
		abortar(2, (char*)"Error al reservar memoria a un Bivector int");

	return aux;
}

double* Memory::ReserveVectorDlb(int size) {

	double* aux;

	aux = (double*)calloc(size, sizeof(double));

	if (!aux)
		abortar(2, (char*)"Error al reservar memoria a un vector double");

	return aux;
}

double** Memory::ReseveBiVectorDlb(int row, int columns) {
	double** aux;
	int i;

	aux = (double**)calloc(row, sizeof(double*));

	for (i = 0; i < row; i++) {
		aux[i] = (double*)calloc(columns, sizeof(double));
	}

	if (!aux)
		abortar(2, (char*)"Error al reservar memoria a un Bivector double");

	return aux;
}

char* Memory::ReserveVectorCharInt(int size) {
	char* aux;

	aux = (char*)calloc(size, sizeof(char));

	if (!aux)
		abortar(2, (char*)"Error al reservar memoria a un vector Char");

	return aux;
}

char** Memory::ReserveBiVectorChar(int row, int columns) {
	char** aux;
	int i;

	aux = (char**)calloc(row, sizeof(char*));

	for (i = 0; i < row; i++) {
		aux[i] = (char*)calloc(columns, sizeof(char));
	}

	if (!aux)
		abortar(2, (char*)"Error al reservar memoria a un Bivector char");

	return aux;
}

float** Memory::ReseveBiVectorflt(int row, int columns) {
	float** aux;
	int i;

	aux = (float**)calloc(row, sizeof(float*));

	for (i = 0; i < row; i++) {
		aux[i] = (float*)calloc(columns, sizeof(float));
	}

	if (!aux)
		abortar(2, (char*)"Error al reservar memoria a un Bivector double");

	return aux;
}

int*** Memory::ReseveTriVectorInt(int row, int columns, int tdim) {
	int*** aux;
	int i, j;

	aux = (int***)calloc(row, sizeof(int**));

	for (i = 0; i < row; i++) {
		aux[i] = (int**)calloc(columns, sizeof(int*));
		for (j = 0; j < columns; j++) {
			aux[i][j] = (int*)calloc(tdim, sizeof(int));
		}
	}

	if (!aux)
		abortar(2, (char*)"Error al reservar memoria a un Trivector int");

	return aux;
}

double*** Memory::ReseveTriVectorDlb(int row, int columns, int tdim) {
	double*** aux;
	int i, j;

	aux = (double***)calloc(row, sizeof(double**));

	for (i = 0; i < row; i++) {
		aux[i] = (double**)calloc(columns, sizeof(double*));
		for (j = 0; j < columns; j++) {
			aux[i][j] = (double*)calloc(tdim, sizeof(double));
		}
	}

	if (!aux)
		abortar(2, (char*)"Error al reservar memoria a un Trivector double");

	return aux;
}

float*** Memory::ReseveTriVectorflt(int row, int columns, int tdim) {
	float*** aux;
	int i, j;

	aux = (float***)calloc(row, sizeof(float**));

	for (i = 0; i < row; i++) {
		aux[i] = (float**)calloc(columns, sizeof(float*));
		for (j = 0; j < columns; j++) {
			aux[i][j] = (float*)calloc(tdim, sizeof(float));
		}
	}

	if (!aux)
		abortar(2, (char*)"Error al reservar memoria a un Trivector double");

	return aux;
}

void Memory::DestroyMemoryInt(int** Arr){
	delete[](*Arr);
}

void Memory::DestroyBiMemoryInt(int*** Biarr, int Col){
	//Free each sub-array
	int i;
	for (i = 0; i < Col; i++) {
		delete[] (*Biarr)[i];
	}
	//Free the array of pointers
	delete[] (*Biarr);
}


