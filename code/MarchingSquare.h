#ifndef MARCHINGSQUARE_H
#define MARCHINGSQUARE_H

#include <string>
#include <vector>
#include "Ponto.h"

using namespace std;

class MarchingSquare{
public:  
	unsigned char *imagePixels;
	int imagemWidth;
	int imagemHeight;

	MarchingSquare (unsigned char *imagem, int imagemWidth, int imagemHeight) {
		this->imagePixels = imagem;
		this->imagemWidth = imagemWidth;
		this->imagemHeight = imagemHeight;
    }

	bool pontoContido(int x, int y){
		bool retorno = false;
		if(x >= 0 && y >= 0 && x < imagemWidth && y < imagemHeight){
			int indice = (imagemWidth*(imagemHeight-y-1) + x)*4 + 3;
			if(imagePixels[indice] == 255){
				retorno = true;
			}
		}
		return retorno;
	}


	string pegarEstado(int x, int y){
		string retorno =  string("");
		vector<Ponto> pontosVerificando;
		pontosVerificando.push_back(Ponto(x-1,y,-1));
		pontosVerificando.push_back(Ponto(x,y,-1));
		pontosVerificando.push_back(Ponto(x-1,y-1,-1));
		pontosVerificando.push_back(Ponto(x,y-1,-1));
		if(pontoContido(pontosVerificando[0].pX,pontosVerificando[0].pY)){
			retorno += string("1");
		}
		if(pontoContido(pontosVerificando[1].pX,pontosVerificando[1].pY)){
			retorno += string("2");
		}
		if(pontoContido(pontosVerificando[2].pX,pontosVerificando[2].pY)){
			retorno += string("3");
		}
		if(pontoContido(pontosVerificando[3].pX,pontosVerificando[3].pY)){
			retorno += string("4");
		}
		return retorno;
	}

	vector<Ponto*> pegarPontosBorda(){
		Ponto pontoTemp = Ponto(0,0);
		int x;
		int y;
		bool achou = false;
		vector<Ponto> borda;
		for(int i = 1;i < imagemWidth && !achou;i++){
			for(int j = 1;j < imagemHeight && !achou;j++){
				if(pontoContido(i,j)){
					achou = true;
					pontoTemp =  Ponto(i,j,-1);
					borda.push_back(pontoTemp);
					x = i;
					y = j;
				}
			}
		}

		int gamb=0;
		do{
			string estado = pegarEstado(x,y);
			gamb++;
			//direita
			if(estado == string("") ||  estado == string("2") || estado == string("12") || estado == string("123")){
				pontoTemp = Ponto(x,y);
				x = x + 1;
			}		
			//baixo
			if(estado == string("24") ||  estado == string("4") || estado == string("124") ){
				pontoTemp = Ponto(x-1,y-1);
				y = y - 1;
			}
			//cima
			if(estado == string("1") ||  estado == string("134") || estado == string("14") || estado == string("13")){
				pontoTemp = Ponto(x-1,y);
				y = y + 1;
			}
			//esquerda
			if(estado == string("34") ||  estado == string("234") || estado == string("23") || estado == string("3")){
				pontoTemp = Ponto(x-1,y-1);
				x = x - 1;
			}

			borda.push_back(pontoTemp);
		}while( (x != borda[0].pX  || y != borda[0].pY) && gamb < 30000);
		
		std::vector<Ponto*> retorno;
		int indice = 0;
		int fator = (int)borda.size()*0.015;//Configura aqui o intervalo de pontos a serem pegos da borda.
		for(int i = 0; i < (int)borda.size();i = i+fator){
			retorno.push_back(new Ponto(borda[i].pX,borda[i].pY,indice));
			indice++;
		}
		return retorno;
	}
};
#endif