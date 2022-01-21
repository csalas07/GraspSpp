#ifndef PHYSYCS_H
#define PHYSYCS_H

//Esta libreria basica, solo detecta si hay o no una colision entre 2 figuras
//Regla general:
//Pelota o esferas = Ball
//Cajas, cuadros o rectangulos = Rect
//Puntos = Point
// Todas las variables es la inicial del tipo por ejemplo de Ball _b seguido del dato
//                     x = posicion en X
//                     y = posicon en Y
//                     w = Ancho o Width
//                     h = Alto o Height
//                     r = radio
//Desarrollado por ARKMs con fines educativos, pero puede ser utilizado en cualquier proyecto (gratutio o paga)

namespace Physics
{
    //Esferea con esfera
	bool BallBall(float _bx1, float _by1, float _br1, float _bx2, float _by2, float _br2);

	//Linea y Esfera
	bool BallLine(float _bx, float _by, int _br, float _lx1, float _ly1, float _lx2, float _ly2);

	//Rectangulo y esfera
	bool RectBall(float _rx, float _ry, float _rw, float _rh, float _bx, float _by, float _br);

	//Rectangulo y rectangulo
	bool RectRect(int *flag,int *nObj,int *otherObj);
	//bool RectRect(float _rx1, float _ry1, float _rw1, float _rh1, float _rx2, float _ry2, float _rw2, float _rh2);

	//Punto y triangulo Teoria barycentric
	bool PointTriangle(float _tx1, float _ty1, float _tx2, float _ty2, float _tx3, float _ty3, float _px, float _py);

	//Punto y esfera
	bool PointBall(float _px, float _py, float _bx, float _by, float _br);

	//Punto y rectangulo
	bool PointRect(int _px, int _py, int _rx, int _ry, int _rx2, int _ry2);

	//Punto y punto
	bool PointPoint(float _x1, float _y1, float _x2, float _y2);

	//Punto y Linea
	bool PointLine(float _px, float _py, float _lx1, float _ly1, float _lx2, float _ly2);

	//Linea y Linea
	bool LineLine(float _x1, float _y1, float _x2, float _y2, float _x3, float _y3, float _x4, float _y4);

	//Distancia
	int Distance(float _x1, float _y1, float _x2, float _y2);
}

#endif
