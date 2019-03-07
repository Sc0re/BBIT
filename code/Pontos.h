#ifndef PONTOS_H
#define PONTOS_H

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

	void incluirPonto(int x, int y){
		this->pontos.push_back(new Ponto(x,y,(int)this->pontos.size()));
	}

	void incluirPonto(Ponto *p){
		this->pontos.push_back(p);
	}

	bool isPontoControle(int indice){
		return this->pontos[indice]->controle;
	}

	void adicionarPontoControle(int x, int y){
		int indicePonto = this->buscarPontoMaisProximo(x,y);
		if (!this->pontos[indicePonto]->controle){
			this->pontos[indicePonto]->controle = true;
			this->qntPontosControle++;
			this->pontoSelecionado = indicePonto;
		}
	}

	void removerPontoControle(int x, int y){
		int indicePonto = this->buscarPontoMaisProximo(x,y);
		if (this->pontos[indicePonto]->controle){
			this->pontos[indicePonto]->controle = false;
			this->qntPontosControle--;
		}
	}

	void moverPonto(int x, int y){
		this->pontos[this->pontoSelecionado]->pX = x;
		this->pontos[this->pontoSelecionado]->pY = y;
	}

	int qntPontosLivres(){
		return ((int)this->pontos.size() - this->qntPontosControle);
	}

	int buscarPontoMaisProximo(int x, int y){
		int retorno = 0;
		double diTemp = 100000;
		for(int i = 0; i < (int)this->pontos.size();i++){
			double dxi = x - this->pontos[i]->pX;
			double dyi = y - this->pontos[i]->pY;
			double di = sqrt(((double) (dxi*dxi) + (dyi*dyi)) );
			if(diTemp > di){
				retorno = i;
				diTemp = di;
			}
		}
		return retorno;
	}


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

	void atualizarPontosLivres(float **pontos){
		int contador = 0;
		for(int i = 0; i < (int)this->pontos.size();i++){
			if(!this->isPontoControle(i)){
				this->pontos[i]->pX = pontos[contador++][0];
				this->pontos[i]->pY = pontos[contador++][0];
			}
		}
	}

	void imprimir(){
		for(int i = 0; i < (int)this->pontos.size();i++){
			cout << "x:" << this->pontos[i]->pX << " y:" << this->pontos[i]->pY << " id:" << this->pontos[i]->identificador << endl;
		}
	}

	int pegarPosicaoPonto(int ident){
		int retorno = 0;
		for(int i = 0; i < (int)this->pontos.size();i++){
			if (this->pontos[i]->identificador == ident)retorno=i;
		}
		return retorno;
	}
};
#endif