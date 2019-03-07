#ifndef CONTROLADOR_H
#define CONTROLADOR_H

#include "Triangulo.h"
#include "Pontos.h"
#include "InicializadorTriangulos.h"

/*
		NOTE: The naming scheme for the functions in this header file are based off of Igarashi's paper.
		      Use that for reference when attempting to understand the matrix functions.
*/

class Controlador{
public:
	Pontos *pontos;
	vector<Triangulo*> triangulos;
	int tamanhoTelaX;
	int tamanhoTelaY;
	Matriz *g;
	Matriz *gTemp;
	Matriz *GLB;

	Matriz *h;
	Matriz *hTemp;
	Matriz *Hinv;
	Matriz *D;

	InicializadorTriangulos *inicializador;

	bool withScaleAdjust;
	
//               screen width     screen height                                 percentage of points
Controlador (int tamanhoTelaX, int tamanhoTelaY,unsigned char *imagePixels, double porcentagemPontos, bool loadObj, char* nomeObj) {
	this->withScaleAdjust = false;
	this->tamanhoTelaX = tamanhoTelaX;
	this->tamanhoTelaY = tamanhoTelaY;
	this->inicializador = new InicializadorTriangulos(imagePixels,tamanhoTelaX,tamanhoTelaY, porcentagemPontos, loadObj, nomeObj);
	this->inicializar();

}

void inicializar(){
	Pontos *pontosTemp = new Pontos();
	
	this->inicializarPontosETriangulos(pontosTemp);
	this->criarMatrizG();

	pontos = new Pontos();
	for(int i = 0; i < (int)g->identificadores.size();i++){
		for(int j = 0; j < (int)pontosTemp->pontos.size();j++){
			if(pontosTemp->pontos[j]->identificador == g->identificadores[i]){
				pontos->incluirPonto(pontosTemp->pontos[j]);
			}
		}
	}
	this->criarMatrizH();

	this->GLB = new Matriz(0,0);
	this->gTemp = new Matriz(0,0);
	this->Hinv = new Matriz(0,0);
	this->D = new Matriz(0,0);
	this->hTemp = new Matriz(0,0);
}

// Change screen size
void mudarTamanhoTela(int w,int h){
	tamanhoTelaX = w;
	tamanhoTelaY = h;
}

// Restart Controller
void reiniciarControlador(){
	vector<Ponto*> pontosAntigos;
	for(int i = 0; i < (int)this->pontos->pontos.size();i++){
		pontosAntigos.push_back(this->pontos->pontos[i]);
	}
	this->pontos->pontos.clear();

	vector<Triangulo*> triangulosAntigos;
	for(int i = 0; i < (int)this->triangulos.size();i++){
		triangulosAntigos.push_back(this->triangulos[i]);
	}
	this->triangulos.clear();

	delete g;
	delete gTemp;
	delete GLB;
	delete h;
	delete hTemp;
	delete Hinv;
	delete D;


	Pontos *pontosTemp = new Pontos();
	
	for(int i = 0; i < (int)pontosAntigos.size();i++){
		pontosTemp->incluirPonto(pontosAntigos[i]);
	}
	this->triangulos = triangulosAntigos;

	this->criarMatrizG();

	pontos = new Pontos();
	for(int i = 0; i < (int)g->identificadores.size();i++){
		for(int j = 0; j < (int)pontosTemp->pontos.size();j++){
			if(pontosTemp->pontos[j]->identificador == g->identificadores[i]){
				pontos->incluirPonto(pontosTemp->pontos[j]);
			}
		}
	}

	this->criarMatrizH();

	this->GLB = new Matriz(0,0);
	this->gTemp = new Matriz(0,0);
	this->Hinv = new Matriz(0,0);
	this->D = new Matriz(0,0);
	this->hTemp = new Matriz(0,0);
}

void inicializarPontosETriangulos(Pontos *pontosTemp){
	/*Ponto *p0 = new Ponto(100,100,0);
	Ponto *p1 = new Ponto(200,100,1); 
	Ponto *p2 = new Ponto(150,150,2);
	Ponto *p3 = new Ponto(350,180,3);
	
	pontosTemp->incluirPonto(p0);
	pontosTemp->incluirPonto(p1);
	pontosTemp->incluirPonto(p2);
	pontosTemp->incluirPonto(p3);

	Triangulo *t1 = new Triangulo(p0,p1,p2);
	Triangulo *t2 = new Triangulo(p1,p2,p3);
	this->triangulos.push_back(t1);
	this->triangulos.push_back(t2);*/

	for(int i = 0; i < (int)this->inicializador->pontos.size();i++){
		pontosTemp->incluirPonto(this->inicializador->pontos[i]);
	}
	this->triangulos = this->inicializador->triangulos;
}

void criarMatrizG(){
	Matriz gTemp2 = Matriz(0,0);
	for(int i = 0; i < (int)triangulos.size();i++){
		gTemp2.concatenarIdentificadores(triangulos[i]->g->identificadores);
	}
	g = new Matriz((int)gTemp2.identificadores.size()*2,(int)gTemp2.identificadores.size()*2);
	g->identificadores = gTemp2.identificadores;
	
	for(int i = 0; i < (int)triangulos.size();i++){
		triangulos[i]->concatenar(g,NULL,0);
	}
}

void organizarG(){
	delete gTemp;
	this->gTemp = new Matriz(g->linhas,g->colunas);

	for(int i = 0; i < gTemp->linhas;i++){
		for(int j = 0; j < gTemp->colunas;j++){
			gTemp->valores[i][j] = -1;
		}
	}
	
	int contadorI = 0;
	for(int i = 0; i < gTemp->linhas;i++){
		if(!pontos->pontos[i/2]->controle){
			int contadorJ = 0;
			for(int j = 0; j < gTemp->colunas;j++){
				if(!pontos->pontos[j/2]->controle){
					gTemp->valores[contadorI][contadorJ] = g->valores[i][j];
					contadorJ++;
				}
			}
			contadorI++;
		}
	}

	for(int i = 0; i < gTemp->linhas;i++){
		if(pontos->pontos[i/2]->controle){
			int contadorJ = 0;
			for(int j = 0; j < gTemp->colunas;j++){
				if(!pontos->pontos[j/2]->controle){
					gTemp->valores[contadorI][contadorJ] = g->valores[i][j];
					contadorJ++;
				}
			}
			contadorI++;
		}
	}

	contadorI = 0;
	for(int i = 0; i < gTemp->linhas;i++){
		if(!pontos->pontos[i/2]->controle){
			int contadorJ = ((int)pontos->pontos.size() - pontos->qntPontosControle)*2;
			for(int j = 0; j < gTemp->colunas;j++){
				if(pontos->pontos[j/2]->controle){
					gTemp->valores[contadorI][contadorJ] = g->valores[i][j];
					contadorJ++;
				}
			}
			contadorI++;
		}
	}

	for(int i = 0; i < gTemp->linhas;i++){
		if(pontos->pontos[i/2]->controle){
			int contadorJ = ((int)pontos->pontos.size() - pontos->qntPontosControle)*2;
			for(int j = 0; j < gTemp->colunas;j++){
				if(pontos->pontos[j/2]->controle){
					gTemp->valores[contadorI][contadorJ] = g->valores[i][j];
					contadorJ++;
				}
			}
			contadorI++;
		}
	}
}

void calcularGLeB(){
	//G' é nxn, de B é
	//nxm, e de q é mx2.
	//u = -inversa(G') x B x q
	if(pontos->qntPontosControle > 0){
		int n = pontos->qntPontosControle; //número de vértices de controle
		int m = pontos->qntPontosLivres(); //número de vértices free
		
		delete GLB;
		GLB = gTemp->submatriz(0,0,(m*2)-1,(m*2)-1);
		Matriz *G00T = GLB->transposta();
		GLB->soma(G00T);
		delete G00T;
		

		Matriz *B = gTemp->submatriz(0,(m*2),(m*2)-1,gTemp->colunas-1);
		Matriz *G10= gTemp->submatriz((m*2),0,gTemp->linhas-1,(m*2)-1);
		Matriz *G10T = G10->transposta();
		delete G10;
		B->soma(G10T);
		delete G10T;

		GLB->matrizInversa();
		GLB->multiplicar(B);
		GLB->multiplicar(-1);
		delete B;
	}
}

Matriz* criarMatrizq(){
	Matriz *q = new Matriz(this->pontos->qntPontosControle*2,1);

	float *temp = pontos->coordenadasPontosControle();
	for(int i = 0; i < q->linhas;i++){
		q->valores[i][0] = temp[i];
	}
	delete [] temp;
	return q;
}

void criarMatrizH(){
	this->h = new Matriz((int)g->identificadores.size()*2,(int)g->identificadores.size()*2);
	
	this->h->identificadores = this->g->identificadores;
	
	Matriz *coeficientesH = new Matriz(this->h->linhas,this->h->colunas);

	for(int i = 0; i < (int)triangulos.size();i++){
		triangulos[i]->concatenar(h,coeficientesH,1);
	}

	delete coeficientesH;
	
}
void organizarH(){
	delete hTemp;
	this->hTemp = new Matriz(h->linhas,h->colunas);

	for(int i = 0; i < hTemp->linhas;i++){
		for(int j = 0; j < hTemp->colunas;j++){
			hTemp->valores[i][j] = -1;
		}
	}
	
	int contadorI = 0;
	for(int i = 0; i < hTemp->linhas;i++){
		if(!pontos->pontos[i/2]->controle){
			int contadorJ = 0;
			for(int j = 0; j < hTemp->colunas;j++){
				if(!pontos->pontos[j/2]->controle){
					hTemp->valores[contadorI][contadorJ] = h->valores[i][j];
					contadorJ++;
				}
			}
			contadorI++;
		}
	}

	for(int i = 0; i < hTemp->linhas;i++){
		if(pontos->pontos[i/2]->controle){
			int contadorJ = 0;
			for(int j = 0; j < hTemp->colunas;j++){
				if(!pontos->pontos[j/2]->controle){
					hTemp->valores[contadorI][contadorJ] = h->valores[i][j];
					contadorJ++;
				}
			}
			contadorI++;
		}
	}

	contadorI = 0;
	for(int i = 0; i < hTemp->linhas;i++){
		if(!pontos->pontos[i/2]->controle){
			int contadorJ = ((int)pontos->pontos.size() - pontos->qntPontosControle)*2;
			for(int j = 0; j < hTemp->colunas;j++){
				if(pontos->pontos[j/2]->controle){
					hTemp->valores[contadorI][contadorJ] = h->valores[i][j];
					contadorJ++;
				}
			}
			contadorI++;
		}
	}

	for(int i = 0; i < hTemp->linhas;i++){
		if(pontos->pontos[i/2]->controle){
			int contadorJ = ((int)pontos->pontos.size() - pontos->qntPontosControle)*2;
			for(int j = 0; j < hTemp->colunas;j++){
				if(pontos->pontos[j/2]->controle){
					hTemp->valores[contadorI][contadorJ] = h->valores[i][j];
					contadorJ++;
				}
			}
			contadorI++;
		}
	}
}
void calcularHinvD(){
	if(pontos->qntPontosControle > 0){
		int n = pontos->qntPontosControle; //número de vértices de controle
		int m = pontos->qntPontosLivres(); //número de vértices free
		delete Hinv;
		this->Hinv = hTemp->submatriz(0,0,(m*2)-1,(m*2)-1);
		Matriz *HLT = Hinv->transposta();
		this->Hinv->soma(HLT);
		delete HLT;
		
		delete D;
		this->D = hTemp->submatriz(0,(m*2),(m*2)-1,hTemp->colunas-1);
		Matriz *H10= hTemp->submatriz((m*2),0,hTemp->linhas-1,(m*2)-1);
		Matriz *H10T = H10->transposta();
		
		this->D->soma(H10T);
		delete H10;
		delete H10T;

		this->Hinv->matrizInversa();
	}
}

Matriz* criarMatrizf0(){
	Matriz *f0 = new Matriz(this->pontos->qntPontosLivres()*2,1);
	
	//criando os identificadores
	for(int i = 0;i < (int)this->h->identificadores.size();i++){
		for(int j = 0; j < (int)this->pontos->pontos.size();j++){
			if(this->h->identificadores[i] == this->pontos->pontos[j]->identificador){
				if(!this->pontos->pontos[j]->controle){
					f0->identificadores.push_back(this->h->identificadores[i]);
				}
			}
		}
	}
	
	Matriz *coeficientesf0 = new Matriz(f0->linhas,f0->colunas);

	for(int i = 0; i < (int)triangulos.size();i++){
		triangulos[i]->concatenarf0(f0,coeficientesf0);
	}

	delete coeficientesf0;

	return f0;
}

void calcularMovimento(){
	this->calcularMovimentoEtapa1();
	//if (this->withScaleAdjust){
		this->calcularMovimentoEtapa2();
		this->calcularMovimentoEtapa3();
	//}
}

void calcularMovimentoEtapa1(){
	//calcula os vertices linha
	Matriz *q = criarMatrizq();
	Matriz *glbCopia = this->GLB->copia();
	glbCopia->multiplicar(q);
	pontos->atualizarPontosLivres(glbCopia->valores);
	delete glbCopia;
	delete q;
}

void calcularMovimentoEtapa2(){
	//achar os fitteds
	for(int i = 0; i < (int)this->triangulos.size();i++){
		this->triangulos[i]->calcularVFitteds();
	}
}

void calcularMovimentoEtapa3(){
	//calcula os vertices duas linhas
	Matriz *q = this->criarMatrizq();
	Matriz *f0 = this->criarMatrizf0();
	Matriz *DCopia = this->D->copia();
	Matriz *HinvCopia = this->Hinv->copia();
	DCopia->multiplicar(q);
	DCopia->soma(f0);
	delete q;
	delete f0;
	HinvCopia->multiplicar(DCopia);
	HinvCopia->multiplicar(-1);
	pontos->atualizarPontosLivres(HinvCopia->valores);
	delete DCopia;
	delete HinvCopia;
}

Pontos getPoints()
{
	return *pontos;
}

};
#endif