#ifndef INICIALIZADORTRIANGULOS_H
#define INICIALIZADORTRIANGULOS_H

#include <vector>
#include <time.h>
#include <math.h>
#include <iostream>
#include <fstream>

class InicializadorTriangulos{
public:  
	unsigned char *imagePixels;
	int imagemWidth;
	int imagemHeight;
	vector<Ponto*> pontos;
	vector<Triangulo*> triangulos;
	int areaImagem;
	int numPontosBorda;
	int numPontos;
	double porcentagemPontos;

	InicializadorTriangulos (unsigned char *imagem, int imagemWidth, int imagemHeight, double porcentagemPontos, bool loadObj, char* nomeObj) {
		if (loadObj){
			this->LerArquivoOBJ(nomeObj);
		}else{
			this->porcentagemPontos = porcentagemPontos;
			this->imagePixels = imagem;
			this->imagemWidth = imagemWidth;
			this->imagemHeight = imagemHeight;
			this->pegarAreaImagem();
			this->inicializarBorda();
			this->inicializarTodosPontos();
			this->pegarTriangulos();
		}
    }

	void pegarAreaImagem(){
		this->areaImagem = 0;
		for(int i = 0; i < imagemWidth;i++){
			for(int j = 0; j < imagemHeight;j++){
				if(pontoContido(i,j))this->areaImagem++;
			}
		}
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

	void inicializarBorda(){
		MarchingSquare ms = MarchingSquare(this->imagePixels,this->imagemWidth,this->imagemHeight);
		vector<Ponto*> borda = ms.pegarPontosBorda();
		for(int i = 0; i < (int)borda.size();i++){
			this->pontos.push_back(borda[i]);
		}
		this->numPontosBorda = (int)borda.size();
	}

	void inicializarTodosPontos(){
		this->pegarPontos();
		for(int i = 0; i < 10;i++){
			distribuirPontos();
		}
		this->escreverPontos();
	}

	void pegarPontos(){
		int identificador = this->numPontosBorda;
		this->numPontos = (this->numPontosBorda)+(this->areaImagem*this->porcentagemPontos);
		srand (time(NULL));
		
		while(identificador < numPontos){
			float posicaoX = ((float)rand()/RAND_MAX) * (imagemWidth - 1);
			float posicaoY = ((float)rand()/RAND_MAX) * (imagemHeight - 1);

			if(pontoContido(posicaoX,posicaoY) ==  true){
				pontos.push_back(new Ponto(posicaoX,posicaoY,identificador));
				identificador++;
			}
		}
	}

	void distribuirPontos(){

		for(int i = this->numPontosBorda; i < this->numPontos;i++){
			Ponto p = pegarPosicao(pontos[i]->pX, pontos[i]->pY, i);
			double fator = 1;

			if(!pontoContido(p.pX,p.pY)){
				float dxi = p.pX - pontos[i]->pX;
				float dyi = p.pY - pontos[i]->pY;

				while(fator >= 0 && !pontoContido(p.pX,p.pY)){
					p.pX = pontos[i]->pX + (dxi*fator);
					p.pY = pontos[i]->pY + (dyi*fator);
					fator = fator - 0.5;
				}
			}

			if(pontoContido(p.pX,p.pY)){
				pontos[i]->pX = p.pX;
				pontos[i]->pY = p.pY;
			}
		}
	}

	Ponto pegarPosicao(float x, float y, int indice){
		int i = 0;
		float somaX = 0;
		float somaY = 0;
		float wd = 0.07;
		float r = 2 * sqrt (((float) this->areaImagem/this->numPontos));
		while(i < numPontos){
			if(i != indice){
				float dxi = x - this->pontos[i]->pX;
				float dyi = y - this->pontos[i]->pY;
				float di = sqrt(((double) (dxi*dxi) + (dyi*dyi)) );
				if(di < r){
					float fi = 1 - (di/r);
					float dxi2 = (dxi/di) * fi * r;
					float dyi2 = (dyi/di) * fi * r;
					somaX += dxi2;
					somaY += dyi2;
				}
			}
			i++;
		}
		float x2 = x + (wd * somaX);
		float y2 = y + (wd * somaY);

		Ponto p = Ponto(x2,y2,this->pontos[indice]->identificador);
		return p;
	}

	void escreverPontos(){
		ofstream out;
		out.open("pontos.txt");
		out << "2\n";
		out << numPontos;
		out << "\n";
		for(int i = 0; i < (int)this->pontos.size();i++){
			out << pontos[i]->pX;
			out << " ";
			out << pontos[i]->pY;
			out << "\n";
		}
		out.close();
		system("qdelaunay < pontos.txt Qt i > triangulos.txt"); // pega os triangulos
	}

	void pegarTriangulos(){
		ifstream in;
		in.open("triangulos.txt");
		int numTriangulos;
		int ponto1;
		int ponto2;
		int ponto3;
		in >> numTriangulos;
		for(int i = 0; i < numTriangulos;i++){
			in >> ponto1;
			in >> ponto2;
			in >> ponto3;
			Triangulo temp = Triangulo(this->pontos[ponto1],this->pontos[ponto2],this->pontos[ponto3]);
			if(this->isToPrint(temp)){
				this->triangulos.push_back(new Triangulo(this->pontos[ponto1],this->pontos[ponto2],this->pontos[ponto3])); 
			}
		}
	}
	
	bool isToPrint(Triangulo t){
		bool pintarTriangulo = true;
		int contador = 0;

		//distancia entre pontos 0 e 1
		int dxi = t.p0->pX - t.p1->pX;
		int dyi = t.p0->pY - t.p1->pY;

		if(!pontoContido(t.p1->pX + (dxi*0.5),t.p1->pY + (dyi*0.5))){
			contador++;
		}

		//distancia entre pontos 0 e 2
		dxi = t.p0->pX - t.p2->pX;
		dyi = t.p0->pY - t.p2->pY;

		if(!pontoContido(t.p2->pX + (dxi*0.5),t.p2->pY + (dyi*0.5))){
			contador++;
		}

		//distancia entre pontos 1 e 2
		dxi = t.p1->pX - t.p2->pX;
		dyi = t.p1->pY - t.p2->pY;

		if(!pontoContido(t.p2->pX + (dxi*0.5),t.p2->pY + (dyi*0.5))){
			contador++;
		}
		
		if(contador >= 2){
			pintarTriangulo = false;
		}

		return pintarTriangulo;
	}

	void LerArquivoOBJ(char *nome){
		ifstream in;
		string local = string("..//Obj//")+string(nome)+string(".obj");
		in.open((char*)local.c_str());
		char c;
		float p1;
		float p2;
		float p3;
		int ident;
		while (in >> c){
			if (c == 'v'){
				in >> p1;
				in >> p2;
				in >> ident;
				this->pontos.push_back(new Ponto(p1,p2,ident));
			}else if (c == 'f'){
				in >> p1;
				in >> p2;
				in >> p3;
				this->triangulos.push_back(new Triangulo(this->pontos[p1-1],this->pontos[p2-1],this->pontos[p3-1]));
			}
		
		}
	}
};
#endif