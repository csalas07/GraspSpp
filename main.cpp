#include <conio.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <string>
#include <time.h>
#include <ctime>
#include <math.h>
#include <cmath>
#include <chrono>
#include "Memory.h"
#include "ReadFiles.h"
#include "physics.h"
#include "SppGrasp.h"

using namespace std;
#define Vmin(a,b) (a<b ? true : false)
#define Valmin(a,b) (a<b ? a : b)
#define Vmax(a,b) (a>b ? a : b)
#define getRan(min,max) (rand() % (int)(max-min)+min)
#define MaxInt 0x7FFFFFFF

void ReadTest(const char* Fname, int binS, int numElm, int MaxSubElem, int** ArrO);

int main(){
	unsigned t0, t1;
	double timeres;
	int i,j,k,a,b,it,NFiles,MaxObj,MaxSubElements,TimesR,waste;
	int binSize, numElements, Comparetype;
	int *assignWin;
	int *assign;
	double *ObjInfo,*ObjInfoWin;
	double **TestSolutions;
	int **ObjWH,**ObjWHWin;
	char **FileList;
	char *Fname;
	double beta;
	int UpB=0,MaxH=0, Lb=MaxInt, MinLb=0;
	float StrArea=0,AddAr=0, Mean=0;
	FILE *f;

	//t0=clock();
	//srand(127);
	
	//srand(time(NULL));

	/*while ((std::chrono::duration_cast<std::chrono::seconds>(endT - Start).count() != 60)){
		cout << "Cuenta: " << UpB++ << endl;
		endT = std::chrono::system_clock::now();
		cout << "Tiempo" << std::chrono::duration_cast<std::chrono::seconds>(endT - Start).count() << " second" << endl;
	}*/


#pragma region Asignacion de memoria y datos
	Comparetype = 5;
	MaxSubElements = 2; //Dimensiones
	//TimesR = 20; //Veces que sera ejecutado
	TimesR = 50; //Veces que sera ejecutado
	it = 1000; //Iteraciones para obtener soluciones
	NFiles=ReadFiles::CountLines("ListF.txt");	//Conteo de archivos
	FileList=Memory::ReserveBiVectorChar(NFiles,100); //Memoria para la lista de archivos
	ReadFiles::ReadF("ListF.txt", &FileList, NFiles); //Lectura de la lista de archivos
	MaxObj=ReadFiles::GetMaxObjects(FileList,NFiles); //Tama�o maximo de elementos
	SppGrasp::Initialize(MaxObj,MaxSubElements); //Inicializan los valores de la Clase SppGrasp
	assignWin = Memory::ReserveVectorInt(MaxObj); //Memoria para las asignaciones de posicion de los objetos ganadoras
	assign = Memory::ReserveVectorInt(MaxObj); //Memoria para las asignaciones de posicion de los objetos
	ObjInfoWin = Memory::ReserveVectorDlb(4); //Memoria para almacenar las variables Waste,Beta y Last H ganadoras
	ObjInfo = Memory::ReserveVectorDlb(4); //Memoria para almacenar las variables Waste,Beta y Last H
	ObjWH = Memory::ReseveBiVectorInt(MaxObj, MaxSubElements + 4); //Memoria M x N para los objetos
	ObjWHWin = Memory::ReseveBiVectorInt(MaxObj, MaxSubElements + 4); //Memoria M x N para los objetos ganadores

	//TestSolutions = Memory::ReseveBiVectorDlb(TimesR, 5); //Memoria para las pruebas

	TestSolutions = Memory::ReseveBiVectorDlb(NFiles, 8); //Memoria para las pruebas

	cout << "Procesando Instancias\n" << endl;
#pragma endregion

#pragma region Ciclo para ejectar los archivos
	for(a=0;a<NFiles;a++){
		
		ReadFiles::ReadI(FileList[a], &ObjWH, &binSize, &numElements, &AddAr, MaxSubElements); //Lectura de la Instancia
		ObjWH=SppGrasp::RObjsOrder(numElements,MaxSubElements,ObjWH); //Acomodo de objetos a lo ancho/Ordenamiento por areas
		//ReadTest(FileList[a],binSize, numElements, MaxSubElements, ObjWH);
#pragma region Acomodo de piezas
		StrArea = AddAr / binSize; //Area del contenedor
		AddAr = 0;
		Mean = 0;
		auto Start = std::chrono::system_clock::now();
		auto endT = std::chrono::system_clock::now();
		waste=3; //Funcion de desperdicio a utilizar
		beta=SppGrasp::GetnextDoub(0,true); //Valor beta aleatorio
		//Reinicio de los datos
		ObjInfo[SppGrasp::obIn::wst]= MaxInt;
		ObjInfo[SppGrasp::obIn::b]= MaxInt;
		ObjInfo[SppGrasp::obIn::lstH]= MaxInt;
		ObjInfoWin[SppGrasp::obIn::lstH] = MaxInt;
		Lb = MaxInt;
		UpB = 0;
		switch (Comparetype){
			case 1: //Alvarez 1 y Wei
				//Ejecucion de las n veces que pidan los articulos
				for (b = 0; b < TimesR; b++) {
					t0 = clock(); //Tiempo de ejecucuion Inicio
					//Iteraciones para las soluciones
					for (i = 0; i < it; i++) {
						MaxH = 0;
						assign = SppGrasp::SppOrder(binSize, waste, beta, numElements, MaxSubElements, ObjWH); //Funcion SPP de la Clase SppGrasp
						ObjInfo[SppGrasp::obIn::wst] = waste; //Asignaci�n de la funci�n de desperdicio
						ObjInfo[SppGrasp::obIn::b] = beta; //Valor Beta aleatorio
						ObjInfo[SppGrasp::obIn::lstH] = SppGrasp::FindLastH(numElements, ObjWH); //Funcion para encontrar la ultima altura, recuerda que es Y+H+1
						MaxH = ObjInfo[SppGrasp::obIn::lstH]; //Uper bound del strip
						//Corregir para que se valide con MaxInt
						if (i == 0) {
							for (j = 0; j < numElements; j++) {
								for (k = 0; k < MaxSubElements + 4; k++) {
									ObjWHWin[j][k] = ObjWH[j][k];
								}
							}

							for (j = 0; j < numElements; j++) {
								assignWin[j] = assign[j];
							}

							for (j = 0; j < 3; j++) {
								ObjInfoWin[j] = ObjInfo[j];
							}
						}
						else if (Vmin(ObjInfo[SppGrasp::obIn::lstH], ObjInfoWin[SppGrasp::obIn::lstH])) {
							for (j = 0; j < numElements; j++) {
								for (k = 0; k < MaxSubElements + 4; k++) {
									ObjWHWin[j][k] = ObjWH[j][k];
								}
							}

							for (j = 0; j < numElements; j++) {
								assignWin[j] = assign[j];
							}

							for (j = 0; j < 3; j++) {
								ObjInfoWin[j] = ObjInfo[j];
							}
						}
						//Uper bound ganador
						UpB = Vmax(UpB, MaxH);

						//Reinicio
						for (j = 0; j < numElements; j++) {
							ObjWH[j][SppGrasp::ob::s] = 0;
							ObjWH[j][SppGrasp::ob::px] = -1;
							ObjWH[j][SppGrasp::ob::py] = -1;
						}
						SppGrasp::CleanData(numElements);
						//waste=SppGrasp::GetnextInt(waste,false); //Seleccion aleatoria o secuencial de la funcion de desperdicio
						beta = SppGrasp::GetnextDoub(beta, true);
					}
					t1 = clock();
					timeres = (double(t1 - t0) / CLOCKS_PER_SEC);
					//Llenado del arreglo TestSolutions
					TestSolutions[b][SppGrasp::TestSol::Lb] = ObjInfoWin[SppGrasp::obIn::lstH];
					TestSolutions[b][SppGrasp::TestSol::Ub] = UpB;
					TestSolutions[b][SppGrasp::TestSol::L] = ObjInfoWin[SppGrasp::obIn::lstH];
					TestSolutions[b][SppGrasp::TestSol::Bt] = beta;
					TestSolutions[b][SppGrasp::TestSol::T] = timeres;
				}
				Fname = Memory::ReserveVectorCharInt(200);
				//sprintf_s(Fname, 200, "%s_%i_W%i.txt", "TestWei", a + 1, waste); //Nombre del archivo de resultados
				sprintf_s(Fname, 200, "%s_%i_W%i.txt", "TestAlvarez", a + 1, waste); //Nombre del archivo de resultados
				ReadFiles::ResultRecords(Fname, a, TimesR, numElements, binSize, StrArea, waste, TestSolutions); //Salida del archivo
				Fname = Memory::ReserveVectorCharInt(200);
				//sprintf_s(Fname, 200, "%s_%i_W%i.txt", "AcomodoWei", a + 1, waste); //Nombre del archivo de acomodos
				sprintf_s(Fname, 200, "%s_%i_W%i.txt", "AcomodoAlvarez", a + 1, waste); //Nombre del archivo de acomodos
				ReadFiles::AccommodationRecords(Fname, a, numElements, MaxSubElements, binSize, ObjWHWin, assignWin); //Salida del archivo
				break;
			case 2: //Alvarez 2
				while ((std::chrono::duration_cast<std::chrono::seconds>(endT - Start).count() != 60)) {
					MaxH = 0;
					MinLb = 0;
					assign = SppGrasp::SppOrder(binSize, waste, beta, numElements, MaxSubElements, ObjWH); //Funcion SPP de la Clase SppGrasp
					//cout << "Cuenta: " << UpB++ << endl;
					endT = std::chrono::system_clock::now();

					ObjInfo[SppGrasp::obIn::wst] = waste; //Asignaci�n de la funci�n de desperdicio
					ObjInfo[SppGrasp::obIn::b] = beta; //Valor Beta aleatorio
					ObjInfo[SppGrasp::obIn::lstH] = SppGrasp::FindLastH(numElements, ObjWH); //Funcion para encontrar la ultima altura, recuerda que es Y+H+1

					AddAr += ObjInfo[SppGrasp::obIn::lstH];
					UpB++; //Contador solamente

					if (Vmin(ObjInfo[SppGrasp::obIn::lstH], ObjInfoWin[SppGrasp::obIn::lstH])){
						for (j = 0; j < 3; j++) {
							ObjInfoWin[j] = ObjInfo[j];
						}
					}

					//Reinicio
					for (j = 0; j < numElements; j++) {
						ObjWH[j][SppGrasp::ob::s] = 0;
						ObjWH[j][SppGrasp::ob::px] = -1;
						ObjWH[j][SppGrasp::ob::py] = -1;
					}
					SppGrasp::CleanData(numElements);
					//waste=SppGrasp::GetnextInt(waste,false); //Seleccion aleatoria o secuencial de la funcion de desperdicio
					beta = SppGrasp::GetnextDoub(beta, true);
					//cout << "Tiempo: " << std::chrono::duration_cast<std::chrono::seconds>(endT - Start).count() << " seg." << endl;
				}
				Mean = AddAr / UpB;//Promedio
				//Llenado del arreglo TestSolutions
				TestSolutions[a][SppGrasp::TestSol::Lb] = ObjInfoWin[SppGrasp::obIn::lstH];
				TestSolutions[a][SppGrasp::TestSol::Ub] = 0;
				TestSolutions[a][SppGrasp::TestSol::L] = ObjInfoWin[SppGrasp::obIn::lstH];
				TestSolutions[a][SppGrasp::TestSol::Bt] = ObjInfoWin[SppGrasp::obIn::b];
				TestSolutions[a][SppGrasp::TestSol::T] = Mean;
				TestSolutions[a][SppGrasp::TestSol::SAr] = StrArea;
				TestSolutions[a][SppGrasp::TestSol::Tn] = numElements;
				TestSolutions[a][SppGrasp::TestSol::Tw] = binSize;

				cout << "Ya termine " << FileList[a] << endl;
				break;
			case 3: //Martello
				while ((std::chrono::duration_cast<std::chrono::seconds>(endT - Start).count() != 300)) {
					MaxH = 0;
					MinLb = 0;
					t0 = clock(); //Tiempo de ejecucuion Inicio
					assign = SppGrasp::SppOrder(binSize, waste, beta, numElements, MaxSubElements, ObjWH); //Funcion SPP de la Clase SppGrasp
					//cout << "Cuenta: " << UpB++ << endl;
					endT = std::chrono::system_clock::now();

					ObjInfo[SppGrasp::obIn::wst] = waste; //Asignaci�n de la funci�n de desperdicio
					ObjInfo[SppGrasp::obIn::b] = beta; //Valor Beta aleatorio
					ObjInfo[SppGrasp::obIn::lstH] = SppGrasp::FindLastH(numElements, ObjWH); //Funcion para encontrar la ultima altura, recuerda que es Y+H+1
					t1 = clock();
					timeres = (double(t1 - t0) / CLOCKS_PER_SEC);
					ObjInfo[SppGrasp::obIn::Tg] = timeres;
					MaxH = ObjInfo[SppGrasp::obIn::lstH]; //Uper bound del strip
					UpB = Vmax(UpB, MaxH);
					//AddAr += ObjInfo[SppGrasp::obIn::lstH];
					//UpB++; //Contador solamente

					if (Vmin(ObjInfo[SppGrasp::obIn::lstH], ObjInfoWin[SppGrasp::obIn::lstH])) {
						for (j = 0; j < 4; j++) {
							ObjInfoWin[j] = ObjInfo[j];
						}
					}

					//Reinicio
					for (j = 0; j < numElements; j++) {
						ObjWH[j][SppGrasp::ob::s] = 0;
						ObjWH[j][SppGrasp::ob::px] = -1;
						ObjWH[j][SppGrasp::ob::py] = -1;
					}
					SppGrasp::CleanData(numElements);
					//waste=SppGrasp::GetnextInt(waste,false); //Seleccion aleatoria o secuencial de la funcion de desperdicio
					beta = SppGrasp::GetnextDoub(beta, true);
					//cout << "Tiempo: " << std::chrono::duration_cast<std::chrono::seconds>(endT - Start).count() << " seg." << endl;
				}
				//Mean = AddAr / UpB;//Promedio
				//Llenado del arreglo TestSolutions
				TestSolutions[a][SppGrasp::TestSol::Lb] = ObjInfoWin[SppGrasp::obIn::lstH];
				TestSolutions[a][SppGrasp::TestSol::Ub] = UpB;
				TestSolutions[a][SppGrasp::TestSol::L] = ObjInfoWin[SppGrasp::obIn::lstH];
				TestSolutions[a][SppGrasp::TestSol::Bt] = ObjInfoWin[SppGrasp::obIn::b];
				TestSolutions[a][SppGrasp::TestSol::T] = ObjInfoWin[SppGrasp::obIn::Tg];
				TestSolutions[a][SppGrasp::TestSol::SAr] = StrArea;
				TestSolutions[a][SppGrasp::TestSol::Tn] = numElements;
				TestSolutions[a][SppGrasp::TestSol::Tw] = binSize;

				cout << "Ya termine " << FileList[a] << endl;
				//Fname = Memory::ReserveVectorCharInt(200);
				//sprintf_s(Fname, 200, "%s_%i_W%i.txt", "AlvarezDos", a + 1, waste); //Nombre del archivo de resultados
				//sprintf_s(Fname, 200, "%s_%i_W%i.txt", "Martello", a + 1, waste); //Nombre del archivo de resultados
				//ReadFiles::ResultRecordTwo(Fname, FileList, NFiles, TestSolutions);
				break;
			case 4:
				//Ejecucion de las n veces que pidan los articulos
				for (b = 0; b < TimesR; b++) {
					//Iteraciones para las soluciones
					for (i = 0; i < it; i++) {
						MaxH = 0;
						MinLb = 0;
						t0 = clock(); //Tiempo de ejecucuion Inicio
						assign = SppGrasp::SppOrder(binSize, waste, beta, numElements, MaxSubElements, ObjWH); //Funcion SPP de la Clase SppGrasp
						t1 = clock();
						timeres = (double(t1 - t0) / CLOCKS_PER_SEC);
						ObjInfo[SppGrasp::obIn::wst] = waste; //Asignaci�n de la funci�n de desperdicio
						ObjInfo[SppGrasp::obIn::b] = beta; //Valor Beta aleatorio
						ObjInfo[SppGrasp::obIn::lstH] = SppGrasp::FindLastH(numElements, ObjWH); //Funcion para encontrar la ultima altura, recuerda que es Y+H+1
						ObjInfo[SppGrasp::obIn::Tg] = timeres;
						MinLb = ObjInfo[SppGrasp::obIn::lstH];
						MaxH = ObjInfo[SppGrasp::obIn::lstH]; //Uper bound del strip
						//Uper bound ganador
						UpB = Vmax(UpB, MaxH);
						Lb = Valmin(Lb, MinLb);
						if (Vmin(ObjInfo[SppGrasp::obIn::lstH], ObjInfoWin[SppGrasp::obIn::lstH])) {
							for (j = 0; j < 4; j++) {
								ObjInfoWin[j] = ObjInfo[j];
							}
						}

						//Reinicio
						for (j = 0; j < numElements; j++) {
							ObjWH[j][SppGrasp::ob::s] = 0;
							ObjWH[j][SppGrasp::ob::px] = -1;
							ObjWH[j][SppGrasp::ob::py] = -1;
						}
						SppGrasp::CleanData(numElements);
						//waste=SppGrasp::GetnextInt(waste,false); //Seleccion aleatoria o secuencial de la funcion de desperdicio
						beta = SppGrasp::GetnextDoub(beta, true);
					}
				}
				//Llenado del arreglo TestSolutions
				TestSolutions[a][SppGrasp::TestSol::Lb] = ObjInfoWin[SppGrasp::obIn::lstH];
				TestSolutions[a][SppGrasp::TestSol::Ub] = UpB;
				TestSolutions[a][SppGrasp::TestSol::L] = Lb;//ObjInfoWin[SppGrasp::obIn::lstH];
				TestSolutions[a][SppGrasp::TestSol::Bt] = ObjInfoWin[SppGrasp::obIn::b];
				TestSolutions[a][SppGrasp::TestSol::T] = ObjInfoWin[SppGrasp::obIn::Tg];
				TestSolutions[a][SppGrasp::TestSol::SAr] = StrArea;
				TestSolutions[a][SppGrasp::TestSol::Tn] = numElements;
				TestSolutions[a][SppGrasp::TestSol::Tw] = binSize;
				cout << "Ya termine " << FileList[a] << endl;
				break;
			case 5:
				//Iteraciones para las soluciones
				for (i = 0; i < it; i++) {
					MaxH = 0;
					MinLb = 0;
					t0 = clock(); //Tiempo de ejecucuion Inicio
					assign = SppGrasp::SppOrder(binSize, waste, beta, numElements, MaxSubElements, ObjWH); //Funcion SPP de la Clase SppGrasp
					t1 = clock();
					timeres = (double(t1 - t0) / CLOCKS_PER_SEC);
					ObjInfo[SppGrasp::obIn::wst] = waste; //Asignaci�n de la funci�n de desperdicio
					ObjInfo[SppGrasp::obIn::b] = beta; //Valor Beta aleatorio
					ObjInfo[SppGrasp::obIn::lstH] = SppGrasp::FindLastH(numElements, ObjWH); //Funcion para encontrar la ultima altura, recuerda que es Y+H+1
					ObjInfo[SppGrasp::obIn::Tg] = timeres;
					MinLb = ObjInfo[SppGrasp::obIn::lstH];
					MaxH = ObjInfo[SppGrasp::obIn::lstH]; //Uper bound del strip
					//Uper bound ganador
					UpB = Vmax(UpB, MaxH);
					Lb = Valmin(Lb, MinLb);
					if (Vmin(ObjInfo[SppGrasp::obIn::lstH], ObjInfoWin[SppGrasp::obIn::lstH])) {
						for (j = 0; j < 4; j++) {
							ObjInfoWin[j] = ObjInfo[j];
						}
					}

					//Reinicio
					for (j = 0; j < numElements; j++) {
						ObjWH[j][SppGrasp::ob::s] = 0;
						ObjWH[j][SppGrasp::ob::px] = -1;
						ObjWH[j][SppGrasp::ob::py] = -1;
					}
					SppGrasp::CleanData(numElements);
					//waste=SppGrasp::GetnextInt(waste,false); //Seleccion aleatoria o secuencial de la funcion de desperdicio
					beta = SppGrasp::GetnextDoub(beta, true);
				}
				//Llenado del arreglo TestSolutions
				TestSolutions[a][SppGrasp::TestSol::Lb] = ObjInfoWin[SppGrasp::obIn::lstH];
				TestSolutions[a][SppGrasp::TestSol::Ub] = UpB;
				TestSolutions[a][SppGrasp::TestSol::L] = ObjInfoWin[SppGrasp::obIn::lstH];
				TestSolutions[a][SppGrasp::TestSol::Bt] = ObjInfoWin[SppGrasp::obIn::b];
				TestSolutions[a][SppGrasp::TestSol::T] = ObjInfoWin[SppGrasp::obIn::Tg];
				TestSolutions[a][SppGrasp::TestSol::SAr] = StrArea;
				TestSolutions[a][SppGrasp::TestSol::Tn] = numElements;
				TestSolutions[a][SppGrasp::TestSol::Tw] = ObjInfoWin[SppGrasp::obIn::wst];
				cout << "Ya termine " << FileList[a] << endl;
				break;
		}
#pragma endregion
		binSize=0;
		StrArea=0;
		AddAr=0;
	}
#pragma endregion

#pragma region
	Fname = Memory::ReserveVectorCharInt(200);
	switch (Comparetype){
		case 2:
			sprintf_s(Fname, 200, "%s_%i_W%i.txt", "AlvarezDos", 1, 1); //Nombre del archivo de resultados
			ReadFiles::ResultRecordTwo(Fname, FileList, NFiles, TestSolutions);
			break;
		case 3:
			sprintf_s(Fname, 200, "%s_%i_W%i.txt", "Martello", 1, 1); //Nombre del archivo de resultados
			ReadFiles::ResultRecordThree(Fname, FileList, NFiles, TestSolutions);
			break;
		case 4:
			sprintf_s(Fname, 200, "%s_%i_W%i.txt", "AlvarezUno", 1, 1); //Nombre del archivo de resultados
			ReadFiles::ResultRecordThree(Fname, FileList, NFiles, TestSolutions);
			break;
		case 5:
			//sprintf_s(Fname, 200, "%s_%i_W%i.txt", "AlvarezUno", 1, 1); //Nombre del archivo de resultados
			sprintf_s(Fname, 200, "%s_%i_W%i.txt", "TestHAlvarezRG_NT", 1, waste); //Nombre del archivo de resultados
			ReadFiles::ResultRecordFour(Fname, FileList, NFiles, TestSolutions);
			break;
		default:
			cout << "Las instancias de Wei ya estan impresas por lo tanto...." << endl;
			break;
	}
#pragma endregion Resultados en TXT de los acomodos

#pragma region
	/*for (i = 0; i < numElements; i++){
		cout <<"Objeto "<< assignWin[i] <<" con datos de " << ObjWHWin[assignWin[i]][SppGrasp::ob::mh] <<", "<< 
			ObjWHWin[assignWin[i]][SppGrasp::ob::mw]<<" Coordenadas: " << 
			ObjWHWin[assignWin[i]][SppGrasp::ob::px] << ", " << ObjWHWin[assignWin[i]][SppGrasp::ob::py];
		printf("\n");
	}
	printf("\n");
	cout << "n: " << numElements << endl;
	cout << "w: " << binSize << endl;
	cout << "Waste: " << ObjInfoWin[SppGrasp::obIn::wst] << endl;
	cout << "Beta: " << ObjInfoWin[SppGrasp::obIn::b] << endl;
	cout << "Last H.: " << ObjInfoWin[SppGrasp::obIn::lstH] << endl;

	t1 = clock();
	time = (double(t1-t0)/CLOCKS_PER_SEC);
	printf("\n");
	cout << "Tiempo: " << time << endl;*/
	cout << "Ya termine" << endl;
#pragma endregion muestra de figuras
	_getch();
}

void ReadTest(const char* Fname, int binS, int numElm, int MaxSubElem, int ** ArrO) {
	int i, j;

	cout << "Archivo " << Fname << endl;
	cout << "Tamaño del contenedor: " << binS << endl;
	cout << "Numero de rows: " << numElm << endl;
	cout << "Numero de columns: " << MaxSubElem << endl;

	for (i = 0; i < numElm; i++) {
		//cout << "Iteracion " << i << ": ";
		for (j = 0; j < MaxSubElem; j++) {
			cout << ArrO[i][j] << ",";
		}
		printf("\n");
	}
	printf("\n");
}