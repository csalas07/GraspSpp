#pragma once
class SppGrasp
{
	//static int MaxBins;
	//static int *assign,*capacity,*bins,**lc,**flgs,xs,ys;
	static int *assign,**lc,**flgs,xs,ys;
private:
	//Validacion para la funcion de colision
	static bool ObjetoColisiona(int **flg,int **rObj,int nRow,int fi,int io);
	//Validacion de choque de entre objeto y bandera
	static bool Obj_Flag_Colisiona(int *flg,int *rObj,int **flgs,int cf,int nRow,int binsize);
	//Busca el indice de la bandera con la Y menor
	static int flIndex(int nRow,int **Flg);
	//Ruleta de valores
	static int Roulette(int nRow,int nColum,int **Lcs);
	//Encuentra banderas libres
	static int GetFreeFlagIndex(int nr,int **flags);
	//Encuentra el valor de la altura de la bandera
	static int GetH(int Px,int Py,int **rObj,int nRow);
	//Encuentra banderas libres por la derecha
	static int FindFlagR(int xs,int ys,int **flg,int nRow);
	//Encuentra banderas libres por la izquierda
	static int FindFlagL(int xs,int ys,int **flg,int nRow);
	//Funcion para indicar el ancho de la  bandera
	static int GetW(int fx,int fy,int **rObj,int nRow,int binSize);
	//Obtencion de porcentajes
	static int GetPercentage(int nRow,double *cps,int increase);
	//Contador de activos en el LC
	static int CountEnabled(int nRow, int **Lc);
	//Reduccion de opciones de la LC
	static int **ReduceLc(int nRow,int *c, int **Lc);
	//Busqueda de maximos y minimos de la LC
	static int FindMaxMinLc(int nRow,int *min ,int **Lc);
	//Sube la bandera para aumentar anchuras
	//static int RiseFlag(int *flag,int nRow,int **rObj);
	static int RiseFlag(int *flag,int nRow,int **rObj,int **flg,int nR,int binSize);
	//Baja la bandera
	static int DownFlag(int *flag,int nRow,int **rObj);
public:
	//Reacomodo de los objetos para estar a lo ancho
	static int **RObjsWPos(int nRow,int nColum,int **arrBins);
	//Ordenamiento de los objetos por anchos
	static int **RObjsOrder(int nRow,int nColum,int **arrBins);
	//Busqueda del objeto con la ultima altura
	static int FindLastH(int nRow, int **rObj);
	//Reinicia la informacion del arreglo de los objetos
	static int **ResetrObj(int nRow, int **rObj);
	//Obtiene el resultado ganador
	static int WinnerResul(int nRow, double **ObjInf);
	//Obtiene el incremento secuencial o aleatorio del valor entero
	static int GetnextInt(int value, bool random);
	//Obtiene el incremento secuencial o aleatorio del valor double
	static double GetnextDoub(double value, bool random);

	static int **CopiesArr(int nRow,int **rObj,int **rObjs);
	//Inicializacion de parametros
	static void Initialize(int nRow,int nColum);
	static void CleanData(int nRow);
	//Ordenamiento para strip packing
	static int *SppOrder(int binSize,int waste,double beta,int nRow,int nCol,int **rObj);
	
	enum v{
		e,
		w,
		hl,
		hr,
		wd,
		x,
		y
	};

	enum ob{
		mh,
		mw,
		s,
		px,
		py,
		ar
	};

	enum lcs{
		en,
		ad,
		c,
	};

	enum obIn{
		wst,
		b,
		lstH,
		Tg,
	};

	enum TestSol{
		Lb,
		Ub,
		L,
		Bt,
		T,
		SAr,
		Tn,
		Tw
	};
};

