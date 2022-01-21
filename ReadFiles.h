#pragma once
class ReadFiles
{
public:
	//Cuenta el numero de archivos
	static int CountLines(const char* TxtName); 
	//Almacena la lista de archivos
	static void ReadF(const char* TxtName, char*** ListF, int n);
	//Obtiene los valores maximos
	static int GetMaxObjects(char** ListF, int n);
	//Lectura de la instancia
	static void ReadI(const char* TxtName, int*** ObjWH, int* binSize, int* numElements, float* AddAr, int subNumElements);
	//Archivo de Resultados
	static void ResultRecords(char* Name, int a, int TimesR, int numElements, int binSize, float StrArea, int waste, double** TestSolutions);
	//Archivos de Acomodos
	static void	AccommodationRecords(char* Name, int a, int numElements, int MaxSubElements, int binSize, int** ObjWHWin, int* assignWin);
	//Resultados de Alvarez Dos
	static void ResultRecordTwo(char* Name, char** ListF, int NumFiles, double** TestSolutions);
	//Resultados de Alvarez Uno y Martello
	static void ResultRecordThree(char* Name, char** ListF, int NumFiles, double** TestSolutions);
	//
	static void ResultRecordFour(char* Name, char** ListF, int NumFiles, double** TestSolutions);

};