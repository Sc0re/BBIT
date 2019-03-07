#ifndef VETOR_H
#define VETOR_H

#include "Ponto.h"
#include <math.h>

class Vetor{
public:  
    float vX;
	float vY;

public:
	Vetor (Ponto *p1, Ponto *p2) {
		this->vX = p2->pX-p1->pX;
		this->vY = p2->pY-p1->pY;
    }

 public: 
	 float modulo(){
		return sqrt((double)(this->vX * this->vX) + (this->vY * this->vY));
	 }

public: 
	float escalar(Vetor *v){
		return ( (this->vX*v->vX)+(this->vY*v->vY) );
	}

public: 
	float cos(Vetor *v){
		return ( (this->escalar(v))/(this->modulo()*v->modulo()) );
	}

public: 
	float sen(Vetor *v){
		return sqrt(1-(this->cos(v)*this->cos(v)));
	}

public:
	void rotacao90(){
		int temp = this->vX;
		this->vX = -1 * this->vY;
		this->vY = temp;
	}

public:
	float angulo(){
		Vetor *vx = new Vetor(new Ponto(0,0),new Ponto(1,0));
		return acos(this->cos(vx));
	}

};
#endif