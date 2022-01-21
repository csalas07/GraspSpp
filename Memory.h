#pragma once
class Memory
{
public :
	static void abortar(int numErr, char msg[]);
	static int* ReserveVectorInt(int size);
	static int** ReseveBiVectorInt(int row, int columns);
	static double* ReserveVectorDlb(int size);
	static double** ReseveBiVectorDlb(int row, int columns);
	static char* ReserveVectorCharInt(int size);
	static char** ReserveBiVectorChar(int row, int columns);
	static float** ReseveBiVectorflt(int row, int columns);
	static int*** ReseveTriVectorInt(int row, int columns, int tdim);
	static double*** ReseveTriVectorDlb(int row, int columns, int tdim);
	static float*** ReseveTriVectorflt(int row, int columns, int tdim);

	static void DestroyMemoryInt(int **Arr);
	static void DestroyBiMemoryInt(int ***Biarr,int Col);
};

