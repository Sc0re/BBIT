#ifndef INPAINTING_H
#define INPAINTING_H

#include <vector>
#include <iostream>
#include <fstream>
#include "Vetor.h"

/*
    NOTE: Inpainting is not pertinent to this project. It is left here because the project will not build without it.
	TODO: Strip all inpainting related code from this project.
*/

using namespace std;

class Inpainting{
public:
	vector<Ponto*> poligono;
	vector<Triangulo*> triangulos;

	
	Inpainting (){
	
	}

	void imprimir(){
		for(int i = 0; i < (int)this->poligono.size() ;i++){
			cout << "ponto: x:" << this->poligono[i]->pX << " y:" << this->poligono[i]->pY << endl;
		}
	}

	void triangularizar(){
		
		this->escreverPontos();
		this->pegarTriangulos();
		//this->pintar();
	}

	void escreverPontos(){
		ofstream out;
		out.open("pontos.txt");
		out << "2\n";
		out << this->poligono.size();
		out << "\n";
		for(int i = 0; i < (int)this->poligono.size();i++){
			out << poligono[i]->pX;
			out << " ";
			out << poligono[i]->pY;
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
			this->triangulos.push_back(new Triangulo(this->poligono[ponto1],this->poligono[ponto2],this->poligono[ponto3])); 
		}
	}

	void pintar(){
		
	}


};
#endif