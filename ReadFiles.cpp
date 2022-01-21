#include "ReadFiles.h"
#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <time.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include "SppGrasp.h"

using namespace std;

#define Gmax(a,b) (a>b ? a : b)

//Cuenta los archivos
int ReadFiles::CountLines(const char* TxtName) {
	int count = 0;
	string line;
	ifstream inFile;
	inFile.open(TxtName);

	if (!inFile) {
		cout << "No se ha podido leer el archivo";
		exit(1); // terminate with error
	}

	while (getline(inFile, line)) {
		count++;
	}

	inFile.close();
	//cout << "Sum = " << count << endl; 
	return count;
}

//Lectura de archivos
void ReadFiles::ReadF(const char* TxtName, char*** ListF, int n) {
	int i,j=0;

	//FILE* f = fopen(TxtName, "r");
	FILE* f;
	errno_t err;
	
	err = fopen_s(&f,TxtName, "r");

	if (err == 0) {
		for (i = 0; i < n; i++) {
			fscanf_s(f, "%s", (*ListF)[i],100);
		}


		if (f) {
			err= fclose(f);

			if (err!=0) {
				exit(6);
			}

		}
	}else {
		exit(6);
	}
	
}

//Obtiene los parametros mayores de los archivos (Proyecto SPP)
int ReadFiles::GetMaxObjects(char** ListF, int n) {
	int i, MaxObjs,v;
	int Objs;
	FILE* f;
	MaxObjs = 0;


	for (i = 0; i < n; i++) {
		fopen_s(&f,ListF[i], "r"); //Es un dato de tipo apuntador
		fscanf_s(f, "%*[^\n]\n"); //Salto de linea
		//fscanf_s(f, "%i", &v, 1); //Numero de objetos
		fscanf_s(f, "%i", &Objs,1); //Numero de objetos
		MaxObjs = Gmax(MaxObjs, Objs); //Validacion del objeto mas grande
		fclose(f);
	}
	return MaxObjs;
}

//Lectura de la instancia
void ReadFiles::ReadI(const char* TxtName, int *** ObjWH, int * binSize, int * numElements, float * AddAr, int subNumElements) {
	int i, j, Add;
	FILE* f;
	Add = 0;
	fopen_s(&f, TxtName, "r");
	
	fscanf_s(f, "%i", &(*binSize),1);
	fscanf_s(f, "%i", &(*numElements),1);
	fscanf_s(f, "%*[^\n]\n");
	for (i = 0; i < *numElements; i++) {
		for (j = 0; j < subNumElements; j++) {
			fscanf_s(f, "%i,", &(*ObjWH)[i][j],1);
		}
		(*ObjWH)[i][SppGrasp::ob::s] = 0;
		(*ObjWH)[i][SppGrasp::ob::px] = -1;
		(*ObjWH)[i][SppGrasp::ob::py] = -1;
		(*ObjWH)[i][SppGrasp::ob::ar] = ((*ObjWH)[i][SppGrasp::ob::mw]) * ((*ObjWH)[i][SppGrasp::ob::mh]);
		Add += (*ObjWH)[i][SppGrasp::ob::ar];
	}
	*AddAr = Add;
	fclose(f);
}

//Archivo de Resultados
void ReadFiles::ResultRecords(char* Name, int a, int TimesR, int numElements, int binSize, float StrArea, int waste, double** TestSolutions){
	int i;
	FILE* f;

	fopen_s(&f, Name, "a+");
	fprintf_s(f, "Dataset B%i", a);
	fprintf_s(f, "\nN: %i", numElements);
	fprintf_s(f, "\nWidth: %i", binSize);
	fprintf_s(f, "\nStripArea: %f", (float)StrArea);
	fprintf_s(f, "\nWasteFunction: %i", waste);
	fprintf_s(f, "\nLb,Ub,L,B,T\n");

	for (i = 0; i < TimesR; i++) {
		fprintf(f, "%i,", (int)TestSolutions[i][SppGrasp::TestSol::Lb]);
		fprintf(f, "%i,", (int)TestSolutions[i][SppGrasp::TestSol::Ub]);
		fprintf(f, "%i,", (int)TestSolutions[i][SppGrasp::TestSol::L]);
		fprintf(f, "%f,", (float)TestSolutions[i][SppGrasp::TestSol::Bt]);
		fprintf(f, "%f\n", (float)TestSolutions[i][SppGrasp::TestSol::T]);
	}
	fclose(f);


}

//Archivo de Acomodos
void ReadFiles::AccommodationRecords(char* Name, int a, int numElements, int MaxSubElements, int binSize,int** ObjWHWin, int* assignWin){
	int i;
	FILE* f;

	fopen_s(&f, Name, "a+");
	fprintf_s(f, "%i\n", binSize);
	fprintf_s(f, "%i,", numElements);
	fprintf_s(f, "%i\n", MaxSubElements);

	for (i = 0; i < numElements; i++) {
		fprintf_s(f, "%i,", ObjWHWin[assignWin[i]][SppGrasp::ob::mh]);
		fprintf_s(f, "%i,", ObjWHWin[assignWin[i]][SppGrasp::ob::mw]);
		fprintf_s(f, "%i,", ObjWHWin[assignWin[i]][SppGrasp::ob::px]);
		fprintf_s(f, "%i\n", ObjWHWin[assignWin[i]][SppGrasp::ob::py]);
	}
	fclose(f);
}

void ReadFiles::ResultRecordTwo(char* Name, char** ListF, int NumFiles, double** TestSolutions){
	int i;
	FILE* f;

	fopen_s(&f, Name, "a+");
	fprintf_s(f, "Dataset %s", Name);
	fprintf_s(f, "\nName,N,W,StrA,Best,Beta,Mean\n");
	for (i = 0; i < NumFiles; i++){
		fprintf(f, "%s,", ListF[i]);
		fprintf(f, "%i,", (int)TestSolutions[i][SppGrasp::TestSol::Tn]);
		fprintf(f, "%i,", (int)TestSolutions[i][SppGrasp::TestSol::Tw]);
		fprintf(f, "%f,", (float)TestSolutions[i][SppGrasp::TestSol::SAr]);
		fprintf(f, "%i,", (int)TestSolutions[i][SppGrasp::TestSol::L]);
		fprintf(f, "%f,", (float)TestSolutions[i][SppGrasp::TestSol::Bt]);
		fprintf(f, "%f\n", (float)TestSolutions[i][SppGrasp::TestSol::T]);
	}
	fclose(f);
}

void ReadFiles::ResultRecordThree(char* Name, char** ListF, int NumFiles, double** TestSolutions){
	int i;
	FILE* f;

	fopen_s(&f, Name, "a+");
	fprintf_s(f, "Dataset %s", Name);
	fprintf_s(f, "\nName,N,W,StrA,Lb,Ub,Best,Beta,Time\n");
	for (i = 0; i < NumFiles; i++) {
		fprintf(f, "%s,", ListF[i]);
		fprintf(f, "%i,", (int)TestSolutions[i][SppGrasp::TestSol::Tn]);
		fprintf(f, "%i,", (int)TestSolutions[i][SppGrasp::TestSol::Tw]);
		fprintf(f, "%f,", (float)TestSolutions[i][SppGrasp::TestSol::SAr]);
		fprintf(f, "%i,", (int)TestSolutions[i][SppGrasp::TestSol::Lb]);
		fprintf(f, "%i,", (int)TestSolutions[i][SppGrasp::TestSol::Ub]);
		fprintf(f, "%i,", (int)TestSolutions[i][SppGrasp::TestSol::L]);
		fprintf(f, "%f,", (float)TestSolutions[i][SppGrasp::TestSol::Bt]);
		fprintf(f, "%f\n", (float)TestSolutions[i][SppGrasp::TestSol::T]);
	}
	fclose(f);
}


void ReadFiles::ResultRecordFour(char* Name, char** ListF, int NumFiles, double** TestSolutions) {
	int i;
	FILE* f;

	fopen_s(&f, Name, "a+");
	fprintf_s(f, "Dataset %s", Name);
	fprintf_s(f, "\nName,N,Waste,StrA,Lb,Ub,Best,Beta,Time\n");
	for (i = 0; i < NumFiles; i++) {
		fprintf(f, "%s,", ListF[i]);
		fprintf(f, "%i,", (int)TestSolutions[i][SppGrasp::TestSol::Tn]);
		fprintf(f, "%i,", (int)TestSolutions[i][SppGrasp::TestSol::Tw]);
		fprintf(f, "%f,", (float)TestSolutions[i][SppGrasp::TestSol::SAr]);
		fprintf(f, "%i,", (int)TestSolutions[i][SppGrasp::TestSol::Lb]);
		fprintf(f, "%i,", (int)TestSolutions[i][SppGrasp::TestSol::Ub]);
		fprintf(f, "%i,", (int)TestSolutions[i][SppGrasp::TestSol::L]);
		fprintf(f, "%f,", (float)TestSolutions[i][SppGrasp::TestSol::Bt]);
		fprintf(f, "%f\n", (float)TestSolutions[i][SppGrasp::TestSol::T]);
	}
	fclose(f);
}