#include "SppGrasp.h"
#include "Memory.h"
#include "physics.h"

#include <cstdlib>
#include <conio.h>
#include <iostream>
#include <cmath>
#include <math.h>
#include <stdlib.h>


#include <time.h>

using namespace std;

#define max(a,b) (a>b ? a : b)
#define min(a,b) (a<b ? a : b)
#define key(a) (a==true ? false:true)
#define Cmin(a,b) (a<b ? true : false)
#define getRandom(min,max) (rand() % (int)(max-min)+min)
//#define getDobRandom(min,max) ((double)(rand() % (int)((max*100)-(min*100))+(min*100))/100)
#define getDobRandom(min,max) ((double)rand() /RAND_MAX)
#define MaxInt 0x7FFFFFFF


//int SppGrasp::MaxBins;
//int *SppGrasp::assign,*SppGrasp::capacity,*SppGrasp::bins,**SppGrasp::lc,**SppGrasp::flgs;
int *SppGrasp::assign,**SppGrasp::lc,**SppGrasp::flgs;
int SppGrasp::xs,SppGrasp::ys;

//Reacomodo de los objetos para estar a lo ancho
int **SppGrasp::RObjsWPos(int nRow,int nColum,int **arrBins){
	int i,aux;
	int **AuxAr;
	AuxAr=arrBins;

	for(i=0;i<nRow;i++){
		aux=AuxAr[i][0];
		AuxAr[i][0]=AuxAr[i][1];
		AuxAr[i][1]=aux;
	}

	return AuxAr;
}

//Ordenamiento de los objetos por anchos
int **SppGrasp::RObjsOrder(int nRow,int nColum,int **arrBins){
	int i,j;
	int **AuxAr,*axArr;
	AuxAr=arrBins;

	for(i=0;i<nRow;i++){
		for(j=i+1;j<nRow;j++){
			if(AuxAr[i][ob::ar]<AuxAr[j][ob::ar]){
				axArr=AuxAr[i];
				AuxAr[i]=AuxAr[j];
				AuxAr[j]=axArr;
			}
		}
	}
	return AuxAr;
}

//Busqueda del objeto con la ultima altura
int SppGrasp::FindLastH(int nRow, int **rObj){
	int i,j,index,max,res;
	max=0x80000000;

	for(i=0;i<nRow;i++){
		res=rObj[i][ob::py]+rObj[i][ob::mh];
		max=max(res,max);
	}
	return max;
}

//Reinicia la informacion del arreglo de los objetos
int **SppGrasp::ResetrObj(int nRow, int **rObj){
	int **AuxAr,i;
	AuxAr=rObj;
	for(i=0;i<nRow;i++){
		AuxAr[i][ob::s]=0;
		/*AuxAr[i][ob::px]=-1;
		AuxAr[i][ob::py]=-1;*/
	}

	return AuxAr;
}

//Se obtiene el arreglo ganador
int SppGrasp::WinnerResul(int nRow, double **ObjInf){
	int i,aux,min=0x7FFFFFFF;
	for(i=0;i<nRow;i++){
		if(ObjInf[i][SppGrasp::obIn::lstH]<min){
			min=ObjInf[i][SppGrasp::obIn::lstH];
			aux=i;
		}
	}
	return aux;
}

//Obtiene el incremento secuencial o aleatorio del valor entero
int SppGrasp::GetnextInt(int value,bool random){
	int val;
	if(random){
		val=getRandom(1,4);
	}else{
		if(value==3){
			value=0;
		}
		val=value+1;
	}
	return val;
}

//Obtiene el incremento secuencial o aleatorio del valor double
double SppGrasp::GetnextDoub(double value,bool random){
	double val;

	if(random){
		val=getDobRandom(0,1);
	}else{
		val=value+0.1;
		if(val>1 || val==1 || val>1.01){
			val=0;
		}
	}
	return val;
}

int **SppGrasp::CopiesArr(int nRow,int **rObj,int **rObjs){
	int **aux,i;
	aux=rObjs;

	for(i=0;i<nRow;i++){
		aux[i]=rObj[i];
	}

	return aux;
}

//Busca el indice de la bandera con la Y menor
//Ver la posibilidad de poder filtrar por la menor X al mismo tiempo
int SppGrasp::flIndex(int nRow,int **Flg){
	int fIndex,miny, minx,i;
	miny=0x7FFFFFFF;
	minx=0x7FFFFFFF;
	for(i=0;i<nRow;i++){
		if(Flg[i][v::e]==1){
			if(Flg[i][v::y]<miny){
				miny=Flg[i][v::y];
				minx=Flg[i][v::x];
				fIndex=i;
			}else if(Flg[i][v::y]==miny){
				if(Flg[i][v::x]<minx){
					miny=Flg[i][v::y];
					minx=Flg[i][v::x];
					fIndex=i;
				}
			}
		}
	}

	return fIndex;
}

//Funcion ruleta para indicar el objeto ganador
int SppGrasp::Roulette(int nRow,int nColum, int **Lc){
	int i,min,max,val,rdm,vmin;
	//Maximos y minimos
	vmin=1;
	min=0x7FFFFFFF;
	max=0x80000000;
	for(i=0;i<nRow;i++){
		if(Lc[i][lcs::en]==1){
			//Encontrando el valor minimo
			min=min(Lc[i][lcs::ad],min);
			//Encontrando el valor maximo
			max=max(Lc[i][lcs::ad],max);
		}
	}
	
	//Conversion de valores menores a mayores y viceversa
	//Sumatoria
	val=0;
	for(i=0;i<nRow;i++){
		if(Lc[i][lcs::en]==1){
			//En dado caso de que el objeto sea negativo se convierte en positivo multiplicando por -1
			//Caso contrario se continua con la misma formula
			if(Lc[i][lcs::ad]<0){
				Lc[i][lcs::c]=Lc[i][lcs::ad]*-1;
			}else{
				Lc[i][lcs::c]=(max+min)-Lc[i][lcs::ad];
			}
			val=val+Lc[i][lcs::c];
		}
	}
	
	//�y si da 0's?
	if(val==1){
		vmin=0;
	}
	 
	//Generacion de numeros aleatorios
	rdm=getRandom(vmin,val);
	//rdm=getRandom(vmin,abs(val));
	
	/*if(val<0){
		rdm=rdm*-1;
	}*/

	//Resta de valores nuevos con el aleatorio
	min=0x7FFFFFFF;
	for(i=0;i<nRow;i++){
		if(Lc[i][lcs::en]==1){
			rdm=rdm-Lc[i][lcs::c];

			if(rdm<=0) {
				return i;
			}
		}
	}
}

//Obtencion de porcentajes para asignar en los arreglos
int SppGrasp::GetPercentage(int nRow,double *cps,int increase){
	int res=0;

	if (increase==1){
		*cps+=0.1;
	}

	res=std::floor(nRow*(*cps));

	if(res<5){
		return res=5;
	}
	return res;
}

//Conteo de Activos de la Lc
int SppGrasp::CountEnabled(int nRow, int **Lc){
	int i,c=0;

	for(i=0;i<nRow;i++){
		if(Lc[i][lcs::en]==1){
			c++;
		}
	}
	return c;
}

//Reduccion de las opciones de la Lc
int **SppGrasp::ReduceLc(int nRow,int *c, int **Lc){
	int i;
	int **AuxAr;
	AuxAr=Lc;

	for(i=0;i<nRow;i++){
		if(AuxAr[i][lcs::en]==1 && *c>0){
			*c-=1;
		}else{
			AuxAr[i][lcs::en]=0;
		}
	}
	return AuxAr;
}

//Seleccion de maximo y minimos de la lc
int SppGrasp::FindMaxMinLc(int nRow,int *min ,int **Lc){
	int maxi,i;
	*min=0x7FFFFFFF;
	maxi=0x80000000;
	
	for(i=0;i<nRow;i++){
		if(Lc[i][lcs::en]==1){
			//Encuentra el min para el limite
			*min=min(Lc[i][lcs::ad],*min);
			//Encuentra el max para el limite
			maxi=max(Lc[i][lcs::ad],maxi);
		}
	}

	return maxi;
}

//Inicializacion de parametros
void SppGrasp::Initialize(int nRow, int nColum){
	//MaxBins=0;
	flgs=Memory::ReseveBiVectorInt(nRow*2,nColum+5);
	assign=Memory::ReserveVectorInt(nRow);
	//bins=Memory::ReserveVectorInt(1);
	lc=Memory::ReseveBiVectorInt(nRow,nColum+1);
	//capacity=Memory::ReserveVectorInt(nRow*2);
}

void SppGrasp::CleanData(int nRow){
	int i, nR;
	nR=nRow*2;
	//Clean Flag
	for(i=0;i<nR;i++){
		flgs[i][v::e]=0;
		flgs[i][v::w]=0;
		flgs[i][v::hl]=MaxInt;
		flgs[i][v::hr]=MaxInt;
		flgs[i][v::wd]=0;
		flgs[i][v::x]=0;
		flgs[i][v::y]=0;
	}
	//Clean assign,lc
	for(i=0;i<nRow;i++){
		assign[i]=MaxInt;
		lc[i][lcs::en]=MaxInt;
		lc[i][lcs::ad]=MaxInt;
		lc[i][lcs::c]=MaxInt;
	}
}

//Ordenamiento de objetos
int * SppGrasp::SppOrder(int binSize,int waste,double beta,int nRow,int nCol,int **rObj){
	int i,j,k,d,r,aux,icr,v,nR,DsPts,WCal,freeFlag,delFlag;//,waste;
	int min,max,desp,lim,fit,c,HipH,nFit,miss,recivingFlag;
	double cp;

	//Inicializaci�n antes del proceso
	nR=nRow*2;
	k=0;
	//times=0;
	miss=nRow;
	fit=0;
	//waste=3;
	//auxiliar
	aux=0;
	//constant percentage
	cp=0.1;
	//division
	d=0;
	//incremento
	icr=0;
	//Apertura de la primer bandera
	flgs[0][v::e]=1;
	flgs[0][v::w]=binSize;
	flgs[0][v::hl]=MaxInt;
	flgs[0][v::hr]=MaxInt;
	flgs[0][v::wd]=binSize;
	flgs[0][v::x]=0;
	flgs[0][v::y]=0;

	//Recorrido de las banderas
	do{
		do{
			//Condicion para aumentar el numero de valores de la lista
			//Obtencion del porcentaje para la lista previa de candidatos
			d=GetPercentage(nRow,&cp,icr);
			aux+=d;
			//Si el contador es mas grande que total del data set, este se ajustara al num. total del dataset
			if(aux>nRow){
				r=aux-nRow;
				aux=aux-r;
			}
			i=flIndex(nR,flgs);
			min=0x7FFFFFFF;
			icr=0;
			max=0x80000000;
			nFit=0;
			//Recorrido de los objetos
			for(j=0;j<nRow;j++){
				desp=0;
				if(rObj[j][ob::s]==0){
					//Condicion para meterlo a la LC
					if(rObj[j][ob::mw]<=flgs[i][v::w]){
						lc[j][lcs::en]=1;
						//funcion de desperdicio
						if(flgs[i][v::hl]==MaxInt && flgs[i][v::hr]==MaxInt){
							desp=rObj[j][ob::mw]*rObj[j][ob::mh] * -1;
						} else {
							switch(waste){
								case 1:
									desp=min((abs(flgs[i][v::wd]-rObj[j][ob::mw])),(abs(flgs[i][v::w]-rObj[j][ob::mw])))+
										min((abs(flgs[i][v::hl]-rObj[j][ob::mh])),(abs(flgs[i][v::hr]-rObj[j][ob::mh])));
									break;
								case 2:
									desp=min((abs(flgs[i][v::hl]-rObj[j][ob::mh])),(abs(flgs[i][v::hr]-rObj[j][ob::mh])));
									break;
								case 3: 
									desp=min(abs(flgs[i][v::wd]-rObj[j][ob::mw]),abs(flgs[i][v::w]-rObj[j][ob::mw]))*
										max(rObj[j][ob::mh],min(flgs[i][v::hl],flgs[i][v::hr])) +
										min(abs(flgs[i][v::hl]-rObj[j][ob::mh]),abs(flgs[i][v::hr]-rObj[j][ob::mh]))*
										rObj[j][ob::mw];
									break;
							}
						}

						if(desp==0){
							desp=desp+1;
						}

						lc[j][lcs::ad]=desp;

						//Encuentra el min para el limite
						min=min(desp,min);
						//Encuentra el max para el limite
						max=max(desp,max);
					} else{
						lc[j][lcs::en]=0;
						lc[j][lcs::ad]=0;
						nFit+=1;
					}
				}else{
					lc[j][lcs::en]=0;
					lc[j][lcs::ad]=0;
				}
			}
			if(nFit==miss){
				//Si el dataset llego al 100% entonces se cancela la bandera.
				//En caso contrario solo se liberara un 10% m�s del dataset
				if(aux==nRow){
					//antes de ser eliminada la bandera, se utilizara la funcion riseflag
					//esta funcion que permite alzar a la bandera en busqueda de ampliar su ancho
					v=0;
					DsPts=0;
					WCal=0;
					HipH=0;
					v=RiseFlag(flgs[i],nRow,rObj,flgs,nR,binSize);
					if(v!=MaxInt){
						//Asignacion de Altura dependiendo su posicion en X y su Hr
						if(flgs[i][v::x]==0 && flgs[i][v::hl]==MaxInt){
							HipH=v;
						}else{
							HipH=flgs[i][v::y]+flgs[i][v::hr];
						}
						//Buscar banderas a la derecha con la altura hipotetica designada
						delFlag=FindFlagR(flgs[i][v::x],HipH,flgs,nR);
						if(delFlag==-1){
							flgs[i][v::e]=0;
							continue;
						}
						//Calcular la distancia entre puntos
						DsPts=Physics::Distance(flgs[i][v::x],HipH,flgs[delFlag][v::x],flgs[delFlag][v::y]);
						//Calucular el W para compararse
						WCal=GetW(flgs[i][v::x],HipH,rObj,nRow,binSize);
						//Condicion para saber si la distancia entre puntos es menor que la del calculo del ancho
						if(DsPts<=WCal){
							//Asignacion del alturas
							flgs[i][v::y]=HipH;
							flgs[i][v::w]=WCal;
							flgs[i][v::wd]=Physics::Distance(flgs[i][v::x],flgs[i][v::y],flgs[delFlag][v::x]+flgs[delFlag][v::wd],flgs[delFlag][v::y]);
							flgs[delFlag][v::e]=0;
						}else{
							flgs[i][v::w]=GetW(flgs[i][v::x],flgs[i][v::y],rObj,nRow,binSize);
							flgs[i][v::wd]=flgs[i][v::w];
						}
						flgs[i][v::hr]=GetH(flgs[i][v::x]+flgs[i][v::w],flgs[i][v::y],rObj,nRow);
						flgs[i][v::hl]=GetH(flgs[i][v::x]-1,flgs[i][v::y],rObj,nRow);
					}else{
						flgs[i][v::e]=0;
					}
					cp=0;
					aux=0;
				}
				icr=1;
			}
		}while(nFit == miss);

		//Conteo de objetos de la lc enabled
		r=CountEnabled(nRow,lc);
		//Objetos contemplados para reducir el lc
		c=aux;
		//Si lo contemplado de los porcentajes de la variable "c" son mayores a lo que se tiene en lc,
		//entonces la variable "r" entra en sustitucion del contenido en la variable "c"
		if(c>=r){
			c=r;
		}

		//Reduccion del lc
		lc=ReduceLc(nRow,&c,lc);

		//Actualizar el Limite
		max=FindMaxMinLc(nRow,&min,lc);

		//Recorrer el LC para colocarlo en el LCR
		//cambiar el aplha 0.1
		lim=min+beta*(max-(min));
		for(j=0;j<nRow;j++){
			if(lc[j][lcs::en]==1){
				if(lc[j][lcs::ad]>lim){
					lc[j][lcs::en]=0;
				} 
			}
		}

		//Seleccion por ruleta
		j=Roulette(nRow,nCol,lc);

		//funcion para determinar si los objetos colisionan
		if(ObjetoColisiona(flgs,rObj,nRow,i,j)){
			//actualizar las banderas en caso de colision
			flgs[i][v::w] = GetW(flgs[i][v::x],flgs[i][v::y],rObj,nRow,binSize);
			if(flgs[i][v::wd]>flgs[i][v::w]){
				flgs[i][v::wd] = flgs[i][v::w];
			}
			//piensale un poco mas para hr y hl
			flgs[i][v::hr] = GetH(flgs[i][v::x]+flgs[i][v::w],flgs[i][v::y],rObj,nRow);
			flgs[i][v::hl] = GetH(flgs[i][v::x]-1,flgs[i][v::y],rObj,nRow);
			//continua proporcionando el porcentaje actual
			continue;
		}else if(Obj_Flag_Colisiona(flgs[i],rObj[j],flgs,i,nR,binSize)){
			delFlag=FindFlagR(flgs[i][v::x],flgs[i][v::y],flgs,nR);
			flgs[i][v::w]=GetW(flgs[i][v::x],flgs[i][v::y],rObj,nRow,binSize);
			flgs[i][v::wd]=Physics::Distance(flgs[i][v::x],flgs[i][v::y],flgs[delFlag][v::x]+flgs[delFlag][v::wd],flgs[delFlag][v::y]);
			flgs[i][v::hr]=GetH(flgs[i][v::x]+flgs[i][v::w],flgs[i][v::y],rObj,nRow);
			flgs[delFlag][v::e]=0;
			continue;
		}

		assign[k]=j;
		fit+=1;
		//valor miss se decrementa 1
		miss--;
		//Asignacion de puntos X,Y del objeto
		rObj[j][ob::px]=flgs[i][v::x];
		rObj[j][ob::py]=flgs[i][v::y];
		freeFlag = GetFreeFlagIndex(nR,flgs);
		//Genarar bandera Arriba
		flgs[freeFlag][v::e] = 1;
		flgs[freeFlag][v::x] = flgs[i][v::x];
		flgs[freeFlag][v::y] = flgs[i][v::y]+rObj[j][ob::mh];
		flgs[freeFlag][v::w] = GetW(flgs[freeFlag][v::x],flgs[freeFlag][v::y],rObj,nRow,binSize);
		flgs[freeFlag][v::wd] = rObj[j][ob::mw];
		//flgs[freeFlag][v::hr] = GetH(flgs[freeFlag][v::x]+rObj[j][ob::mw],flgs[freeFlag][v::y],rObj,nRow);
		flgs[freeFlag][v::hr] = GetH(flgs[freeFlag][v::x]+flgs[freeFlag][v::w],flgs[freeFlag][v::y],rObj,nRow);
		
		//Condicion para juntar banderas
		//PONER BREAK POINT, SI NO ENTRA AQUI, REMOVER LA CONDICION
		if (flgs[freeFlag][v::hr] == MaxInt){
			//Verificacion de la bandera para hr
			if (GetH(flgs[freeFlag][v::x]+rObj[j][ob::mw],flgs[freeFlag][v::y]-1,rObj,nRow) != MaxInt){
				//Encontr� colisi�n y debo de buscar la bandera BR, tomar sus datos y desaparecerla.
				delFlag=FindFlagR(flgs[freeFlag][v::x],flgs[freeFlag][v::y],flgs,nR);
				//en lo que se piensa la condicion para encontrar la bandera a borrar
				flgs[freeFlag][v::hr]=flgs[delFlag][v::hr];
				flgs[freeFlag][v::wd]+=flgs[delFlag][v::wd];
				flgs[delFlag][v::e]=0;
			}
		}
		if (flgs[freeFlag][v::x] == 0){
			flgs[freeFlag][v::hl] = MaxInt;
		}else{
			flgs[freeFlag][v::hl] = GetH(flgs[freeFlag][v::x]-1,flgs[freeFlag][v::y],rObj,nRow);
			//Verificacion de la bandera para hl
			if (flgs[freeFlag][v::hl] == MaxInt)
			{
				if (GetH(flgs[freeFlag][v::x]-1,flgs[freeFlag][v::y]-1,rObj,nRow) != MaxInt)
				{
					//Encontr� colisi�n y debo de buscar la bandera BL, pasar los datos de la bandera actual y desaparecer la bandera actual.
					recivingFlag=FindFlagL(flgs[freeFlag][v::x],flgs[freeFlag][v::y],flgs,nR);
					//en lo que se piensa la condicion para encontrar la bandera a recibir
					flgs[recivingFlag][v::hr]=flgs[freeFlag][v::hr];
					flgs[recivingFlag][v::wd]+=flgs[freeFlag][v::wd];
					flgs[freeFlag][v::e]=0;
				}
			}
		}

		freeFlag = GetFreeFlagIndex(nR,flgs);
		//Genarar bandera Abajo
		flgs[freeFlag][v::e] = 1;
		flgs[freeFlag][v::x] = flgs[i][v::x]+rObj[j][ob::mw];
		flgs[freeFlag][v::y] = flgs[i][v::y];

		/*if(DownFlag(flgs[freeFlag],nRow,rObj)!=MaxInt){

		}*/

		flgs[freeFlag][v::w] = GetW(flgs[freeFlag][v::x],flgs[freeFlag][v::y],rObj,nRow,binSize);
		//ELIMINAR LA BANDERA SI EL W=0
		if(flgs[freeFlag][v::w]==0){
			flgs[freeFlag][v::e] = 0;
			//se indica set al objeto
			rObj[j][ob::s]=1;
			//se desactiva la bandera actual
			flgs[i][v::e]=0;
			//Aumento de 1 unidad al valor k
			k++;
			//se reinicia el porcentaje
			icr=1;
			cp=0;
			continue;
		}
		flgs[freeFlag][v::wd] = flgs[freeFlag][v::w]; //HAY POSIBILIDAD DE MEJORA CREANDO ALGO ADHOC PARA VERIFICAR EL TAMA�O DEL PISO DE LA BANDERA
		flgs[freeFlag][v::hl] = rObj[j][ob::mh];

		//Condicion para juntar banderas
		flgs[freeFlag][v::hr] = GetH(flgs[freeFlag][v::x] + flgs[freeFlag][v::w],flgs[freeFlag][v::y],rObj,nRow);
		if (flgs[freeFlag][v::hr] == MaxInt)
		{
			if (GetH(flgs[freeFlag][v::x]+flgs[freeFlag][v::w],flgs[freeFlag][v::y]-1,rObj,nRow) != MaxInt)
			{
				//Encontr� colisi�n y debo de buscar la bandera BR y tomar sus datos y desaparecerla.
				delFlag=FindFlagR(flgs[freeFlag][v::x],flgs[freeFlag][v::y],flgs,nR);
				flgs[freeFlag][v::hr]=flgs[delFlag][v::hr];
				flgs[delFlag][v::e]=0;
			}
		}
		//se indica set al objeto
		rObj[j][ob::s]=1;
		//se desactiva la bandera actual
		flgs[i][v::e]=0;
		//valor k para el arreglo assign se incrementa
		k++;
		//se reinicia el porcentaje
		icr=1;
		cp=0;
	}while(miss > 0);
	return assign;
}

int SppGrasp::GetFreeFlagIndex(int nr,int **flags){
	int i;
	for(i=0;i<nr;i++){
		if(flags[i][v::e]==0){
			return i;
		}
	}
}

//Funcion para validar colisiones entre objetos
bool SppGrasp::ObjetoColisiona(int **flg,int **rObj,int nRow,int fi,int io){
	int i;
	for(i=0;i<nRow;i++){
		if(rObj[i][2]==1){
			if (Physics::RectRect(flg[fi],rObj[io],rObj[i])){
				return true;
			}
		}
	}
	return false;
}

bool SppGrasp::Obj_Flag_Colisiona(int *flg,int *rObj,int **flgs,int cf,int nRow,int binsize){
	int x1,y1,i;
	x1=flg[v::x];
	y1=flg[v::y];

	for(i=0;i<nRow;i++){
		if(flgs[i][v::e]==1 && i!=cf){
			//if(Physics::LineLine(x1,y1,binsize,y1,flgs[i][v::x],flgs[i][v::y],flgs[i][v::x],flgs[i][v::y]+rObj[ob::mh])){
			if(Physics::LineLine(x1,y1,x1+rObj[ob::mw],y1,flgs[i][v::x],flgs[i][v::y],flgs[i][v::x],flgs[i][v::y]+rObj[ob::mh])){
				return true;
			}
		}
	}

	return false;
}

int SppGrasp::GetH(int Px,int Py,int **rObj,int nRow){

	int i;

	//verificar con la colision con physics
	//si existe coliosion retornar el true

	for(i=0;i<nRow;i++){
		if(rObj[i][ob::s]==1){
			if(Physics::PointRect(Px,Py,rObj[i][ob::px],rObj[i][ob::py],rObj[i][ob::px]+rObj[i][ob::mw]-1,rObj[i][ob::py]+rObj[i][ob::mh]-1)){
				return rObj[i][ob::mh]+rObj[i][ob::py]-(Py);
			}
		}
	}
	return MaxInt;
}

int SppGrasp::FindFlagR(int	xs,int ys,int **flg,int nRow){
	int i, minx=MaxInt, indMinx=-1;

	for(i=0;i<nRow;i++){
		if(flg[i][v::y]==ys && flg[i][v::e]==1){
			if(flg[i][v::x]>xs){
				if(flg[i][v::x]<minx){
					minx=flg[i][v::x];
					indMinx=i;
				}
			}
		}
	}
	return indMinx;
}

int SppGrasp::FindFlagL(int	xs,int ys,int **flg,int nRow){
	int i, maxX=-1, indMaxX=-1;

	for(i=0;i<nRow;i++){
		if(flg[i][v::y]==ys && flgs[i][v::e]==1){
			if(flg[i][v::x]<xs){
				if(flg[i][v::x]>maxX){
					maxX=flg[i][v::x];
					indMaxX=i;
				}
			}
		}
	}
	return indMaxX;
}

int SppGrasp::GetW(int fx,int fy,int **rObj,int nRow,int binSize){

	int i,minx=binSize;

	for(i=0;i<nRow;i++){
		if(rObj[i][ob::s]==1){
			if(Physics::LineLine(fx,fy,binSize,fy,rObj[i][ob::px],rObj[i][ob::py],rObj[i][ob::px],rObj[i][ob::py]+rObj[i][ob::mh]-1)){
				if(rObj[i][ob::py]<=fy && rObj[i][ob::py]+rObj[i][ob::mh]>=fy){
					//colisiona con la linea horizonral en Y+1
					if(rObj[i][ob::px]>=fx){
						//es de los de la derecha
						if(minx>rObj[i][ob::px]){
							//obtenemos el de menor posici�n en x
							minx=rObj[i][ob::px];
						}
					}
				}
			}
		}
	}
	return minx-fx;
}

int SppGrasp::RiseFlag(int *flag,int nRow,int **rObj,int **flg,int nR,int binSize){
	int Y,X,V,W,i;
	int xs,ys,hl,hr;

	xs=flag[v::x];
	ys=flag[v::y];
	hl=flag[v::hl];
	hr=flag[v::hr];

	//if(hr!=MaxInt){
	if(hr!=MaxInt && xs>0){

		if(xs>0 && hl==MaxInt){
			Y=ys+1;
			//Buscamos banderas a la izquierda subiendo progresivamente la altura del punto
			for(i=0;i<hr;i++){
				V=FindFlagL(xs,Y,flg,nR);
				//Encontro una bandera a la izquierda, no se puede alzar la bandera
				if(V!=-1){
					return MaxInt;
				}
				Y++;
			}
			//No encontro bandera a la izquierda, se pudo alzar
			if(V==-1){
				V=1;
				return V;
			}
		}


		//x=0, y=78
		/*if(xs==0 && hl==MaxInt){
			Y=ys;
			do{
				X=GetW(xs,Y,rObj,nRow,binSize);
				V=GetH(xs+X,Y,rObj,nRow);
				Y++;
			}while(V==MaxInt);
			return Y;
		}*/

		if(hr<hl){
			Y=ys+hr-1;
			X=xs+flag[v::w];
			V=GetH(X,Y,rObj,nRow);
			if(V!=MaxInt){
				//Su ancho se pudo expandir
				return V;
			}
		}
	}

	//x=0, y=78
	if(xs==0 && hl==MaxInt){
		V=1;
		W=GetW(xs,ys,rObj,nRow,binSize);
		X=xs+W;
		Y=ys;
		do{
			//Busco si existe espacio disponible a la derecha
			if(GetH(X,Y,rObj,nRow)==MaxInt){
				//Verifico la existencia de colision
				if(GetH(X,Y-1,rObj,nRow)!=MaxInt){
					//Encontro colision, hay que buscar una bandera a la derecha del punto
					if(FindFlagR(xs,Y,flg,nR)!=-1){
						V=MaxInt;
					}
				}
			}else{
				Y++;
			}
		}while(V!=MaxInt);
		return Y;
	}

	/*if(xs==0 && hr==MaxInt){
		Y=ys;
		do{
			
			V=GetW(xs,Y,rObj,nRow,binSize);
			//V=GetH(X,Y,rObj,nRow);
			if(V!=binSize){
				Y++;
			}
		}while(V!=binSize);
		return Y;
	}*/

	return MaxInt;
}

int SppGrasp::DownFlag(int *flag,int nRow,int **rObj){
	int Y,X,V,i;
	int xs,ys;
	bool coll;

	xs=flag[v::x];
	ys=flag[v::y];
	coll=false;
	Y=ys;
	do{
		Y--;
		for(i=0;i<nRow;i++){
			if(rObj[i][ob::s]==1){
				if(GetH(xs,Y,rObj,nRow)!=MaxInt){
					coll=true;
				}
			}
		}

		if(Y==-1 || Y==ys){
			Y=MaxInt;
			break;
		}else{
			Y++;
		}
	}while(coll==false);

	return Y;
}