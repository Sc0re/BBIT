#ifndef PONTO_H
#define PONTO_H

#include "Textura.h"

class Ponto{
public:  
    float pX;
	float pY;
	bool controle;
	int identificador;
	Textura *textura;

	Ponto (float x, float y, int identificador ) {
		this->pX = x;
		this->pY = y;
		this->controle = false;
		this->identificador = identificador;
    }
	Ponto (float x, float y) {
		this->pX = x;
		this->pY = y;
		this->identificador = -1;
	}

	void calcularTextura(float width, float height){
		this->textura = new Textura( (this->pX)/(width-1) , (height-1-this->pY)/(height-1) );
	}

	Textura pegarTextura(float width, float height, float fatorX, float fatorY){
		return Textura( (this->pX+fatorX)/(width-1) , (height-1-this->pY-fatorY)/(height-1) );
	}

};
#endif