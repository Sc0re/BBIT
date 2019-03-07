#ifndef PONTOS_H
#define PONTOS_H

#pragma warning (disable: 4244)

#include "Ponto.h"
#include <math.h>
#include <iostream>
#include <vector>

using namespace std;

class Pontos{
public:  
    vector<Ponto*> pontos;
	int pontoSelecionado;
	int qntPontosControle;

	Pontos () {
		this->pontoSelecionado = -1;
		this->qntPontosControle = 0;
    }

	~Pontos(){
		for(int i = 0;i < (int)this->pontos.size();i++){
			delete this->pontos[i];
		}
		pontos.clear();
	}

	// Include Point (based on coordinates)
	void incluirPonto(int x, int y){
		this->pontos.push_back(new Ponto(x,y,(int)this->pontos.size()));
	}

	// Include Point (based on class object)
	void incluirPonto(Ponto *p){
		this->pontos.push_back(p);
	}

	// Is Point Controlled
	bool isPontoControle(int indice){
		return this->pontos[indice]->controle;
	}

	/*
	=================
	Add Control Point
	=================
	*/
	void adicionarPontoControle(int x, int y){
		int indicePonto = this->buscarPontoMaisProximo(x,y);
		if (!this->pontos[indicePonto]->controle){
			this->pontos[indicePonto]->controle = true;
			this->qntPontosControle++;
			this->pontoSelecionado = indicePonto;
		}
	}

	/*
	====================
	Remove Control Point
	====================
	*/
	void removerPontoControle(int x, int y){
		int indicePonto = this->buscarPontoMaisProximo(x,y);
		if (this->pontos[indicePonto]->controle){
			this->pontos[indicePonto]->controle = false;
			this->qntPontosControle--;
		}
	}

	/*
	==========
	Move Point
	==========
	Moves the currently controlled point to 
	the parameter coordinates (x) and (y).
	*/
	void moverPonto(int x, int y){
		this->pontos[this->pontoSelecionado]->pX = x;
		this->pontos[this->pontoSelecionado]->pY = y;
	}

	/*
	====================
	Quantify Free Points
	====================
	Returns the difference between the total number of points and 
	the control points.
	*/
	int qntPontosLivres(){
		return ((int)this->pontos.size() - this->qntPontosControle);
	}

	/*
	========================
	Search for nearest point
	========================
	(int)   retorno: return value for the function
	(double) diTemp: distance threshold for point selection
	                 if a point is further than this, it defaults
					 to the origin point (index 0)
	*/
	int buscarPontoMaisProximo(int x, int y){
		int retorno = 0;
		double diTemp = 100;
		for(int i = 0; i < (int)this->pontos.size();i++){
			double dxi = x - this->pontos[i]->pX;
			double dyi = y - this->pontos[i]->pY;
			double di = sqrt(((double) (dxi*dxi) + (dyi*dyi)) );
			if(diTemp > di){
				retorno = i;
				diTemp = di;
			}
		}
		cout << retorno << endl;
		return retorno;
	}

	/*
	=========================
	Control Point Coordinates
	=========================
	(float) retorno: Return value for the function
	(int)  contador: "counter" - not sure yet
	*/
	float* coordenadasPontosControle(){
		float *retorno = new float[this->qntPontosControle*2];
		int contador = 0;
		for(int i = 0; i < (int)this->pontos.size();i++){
			if(this->pontos[i]->controle){
				retorno[contador++] = this->pontos[i]->pX;
				retorno[contador++] = this->pontos[i]->pY;
			}
		}
		return retorno;
	}

	/*
	==================
	Update Free Points
	==================
	*/
	void atualizarPontosLivres(float **pontos){
		int contador = 0;
		for(int i = 0; i < (int)this->pontos.size();i++){
			if(!this->isPontoControle(i)){
				this->pontos[i]->pX = pontos[contador++][0];
				this->pontos[i]->pY = pontos[contador++][0];
			}
		}
	}

	/*
	====================
	Print Out Point Info
    ====================
	*/
	void imprimir(){
		for(int i = 0; i < (int)this->pontos.size();i++){
			cout << "x:" << this->pontos[i]->pX << " y:" << this->pontos[i]->pY << " id:" << this->pontos[i]->identificador << endl;
		}
	}

	/*
	==================
	Get Point Position
    ==================
	*/
	int pegarPosicaoPonto(int ident){
		int retorno = 0;
		for(int i = 0; i < (int)this->pontos.size();i++){
			if (this->pontos[i]->identificador == ident)retorno=i;
		}
		return retorno;
	}
};
#endif