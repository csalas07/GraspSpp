#include "physics.h"

#include <cmath>

#include "SppGrasp.h"

namespace Physics
{
    bool BallBall(float _bx1, float _by1, float _br1, float _bx2, float _by2, float _br2)
	{
		// Encontramos distancia entre los 2
		float xDist = _bx1 - _bx2;   //Distancia C
		float yDist = _by1 - _by2;   // Distancia Y
		float distance = sqrt((xDist*xDist) + (yDist*yDist));  //distancia total

		// pobamos colision
		if (_br1 + _br2 > distance)
			return true;   //COlision
		else
			return false; //No lo fie
	}

	bool BallLine(float _bx, float _by, int _br, float _lx1, float _ly1, float _lx2, float _ly2)
    {
        // Largo de la linea
        float distX = _lx1 - _lx2;
        float distY = _ly1 - _ly2;
        float lineLength = sqrt( distX*distX + distY*distY );

        // Resolvemos R
        float r = ( ( (_bx-_lx1)*(_lx2-_lx1) ) + ( (_by-_ly1)*(_ly2-_ly1) )) / pow(lineLength, 2);

        // Obtenemos el X e Y mas cercano de la linea a la esfera
        float closestX = _lx1 + r * (_lx2-_lx1);
        float closestY = _ly1 + r * (_ly2-_ly1);

        // Obtenemos distancia de este punto a la esfera
        float distToPointX = closestX - _bx;
        float distToPointY = closestY - _by;
        float distToPoint = sqrt( distToPointX*distToPointX + distToPointY*distToPointY );

        // Si la distancia es menor o igual al radio de la esfera, es colision
        if (distToPoint <= _br)
            return true;
        else
            return false;
    }

	bool RectBall(float _rx, float _ry, float _rw, float _rh, float _bx, float _by, float _br)
	{
		//Primero probamos las orillas, usamos teorema de pitagoras

		// si la esfera esta completamente adentro en ancho de la caja
		if (_bx + _br  >= _rx - _rw / 2 && _bx - _br <= _rx + _rw / 2 && std::abs(_ry - _by) <= _br)
		{
			return true;
		}
		//Si no, lo verificamos en  altura de la caja
		else if (_by + _br >= _ry - _rh / 2 && _by - _br <= _ry + _rh / 2 && std::abs(_rx - _bx) <= _br)
		{
			return true;
		}

		// Buscamos esquina mas cercana
		// Usamos teoria de punto y esfera
		// arriba-izquierdo
		float xDist = (_rx - _rw / 2) - _bx;  // igual que BallBall, definimos punto no centro
		float yDist = (_ry - _rh / 2) - _by;
		float shortestDist = sqrt((xDist*xDist) + (yDist * yDist));

		// ariba-derecho
		xDist = (_rx + _rw / 2) - _bx;
		yDist = (_ry - _rh / 2) - _by;
		float distanceUR = sqrt((xDist*xDist) + (yDist * yDist));
		if (distanceUR < shortestDist) //comprobamos cual es el mas cercano
		{
			shortestDist = distanceUR;      //Actualizamos
		}

		// abajo-derecho
		xDist = (_rx + _rw / 2) - _bx;
		yDist = (_ry + _rh / 2) - _by;
		float distanceLR = sqrt((xDist*xDist) + (yDist * yDist));
		if (distanceLR < shortestDist)//comprobamos cual es el mas cercano
		{
			shortestDist = distanceLR; //actualizamos
		}

		// abajo-izquierdo
		xDist = (_rx - _rw / 2) - _bx;
		yDist = (_ry + _rh / 2) - _by;
		float distanceLL = sqrt((xDist*xDist) + (yDist * yDist));
		if (distanceLL < shortestDist)//comprobamos cual es el mas cercano
		{
			shortestDist = distanceLL; //actualizamos
		}

		//probamos colision si es menor al radio
		if (shortestDist < _br)
			return true;  //colision
		else
			return false;
	}


	bool RectRect(int *flag,int *nObj,int *otherObj){
		float x1,y1,xw1,yh1,x2,y2,xw2,yh2;
		x1=flag[SppGrasp::v::x];
		y1=flag[SppGrasp::v::y];
		xw1=nObj[SppGrasp::ob::mw]+x1;
		yh1=nObj[SppGrasp::ob::mh]+y1;
		x2=otherObj[SppGrasp::ob::px];
		y2=otherObj[SppGrasp::ob::py];
		xw2=otherObj[SppGrasp::ob::mw]+x2;
		yh2=otherObj[SppGrasp::ob::mh]+y2;

		if(x1<xw2 && xw1>x2 && y1<yh2 && yh1>y2){
			return true;
		}

        return false;
    }

	//Teoria barycentric
	bool PointTriangle(float _tx1, float _ty1, float _tx2, float _ty2, float _tx3, float _ty3, float _px, float _py)
	{
		float a0 = std::abs((_tx2 - _tx1)*(_ty3 - _ty1) - (_tx3 - _tx1)*(_ty2 - _ty1));
		float a1 = std::abs((_tx1 - _px)*(_ty2 - _py) - (_tx2 - _px)*(_ty1 - _py));
		float a2 = std::abs((_tx2 - _px)*(_ty3 - _py) - (_tx3 - _px)*(_ty2 - _py));
		float a3 = std::abs((_tx3 - _px)*(_ty1 - _py) - (_tx1 - _px)*(_ty3 - _py));

		//Evaluamos
		return (std::abs(a1 + a2 + a3 - a0) <= 1 / 256);
	}

	bool PointBall(float _px, float _py, float _bx, float _by, float _br)
	{
		//Distancia entre los dos objectos
		float xDist = _px - _bx; //Distancia en X
		float yDist = _py - _by; //Distancia en Y
		float distance = sqrt((xDist*xDist) + (yDist*yDist));  // Distancai total

		//Evaluamos
		if (_br > distance)
			return true;    //COlision
		else
			return false;
	}

	bool PointRect(int _px, int _py, int _rx, int _ry, int _rx2, int _ry2)
	{
	    if(_px >= _rx && _px <= _rx2)
            if(_py >= _ry && _py <= _ry2)
                return true;

        return false;
	}

	bool PointPoint(float _x1, float _y1, float _x2, float _y2)
	{
        if (_x1 == _x2 && _y1 == _y2)
          return true;

        return false;
    }

    bool PointLine(float _px, float _py, float _lx1, float _ly1, float _lx2, float _ly2)
    {
        //Obtenemos pendiente de la linea
        float lineSlope = (_ly2 - _ly1)/(_lx2 - _lx1);

        //Obtenemos pendiente entre punto y la linea
        float pointSlope = (_ly2 - _py)/(_lx2 - _px);

        //Si las pendientes son iguales, el punto esta en la linea
        if (lineSlope == pointSlope)
            return true;

        return false;
    }

    bool LineLine(float _x1, float _y1, float _x2, float _y2, float _x3, float _y3, float _x4, float _y4)
    {
        // Encontrar uA y uB
        float uA = ( (_x4-_x3)*(_y1-_y3) - (_y4-_y3)*(_x1-_x3)) / ((_y4-_y3)*(_x2-_x1) - (_x4-_x3)*(_y2-_y1) );
        float uB = ( (_x2-_x1)*(_y1-_y3) - (_y2-_y1)*(_x1-_x3)) / ((_y4-_y3)*(_x2-_x1) - (_x4-_x3)*(_y2-_y1) );

        if (uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1)
            return true;

        return false;
    }

	int Distance(float _x1, float _y1, float _x2, float _y2){
		int result;
		result=sqrt(pow(_x2-_x1,2)+pow(_y2-_y1,2));
		return result;
	}

}
