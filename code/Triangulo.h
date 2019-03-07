#ifndef TRIANGULO_H
#define TRIANGULO_H

#include "Vetor.h"
#include "Matriz.h"

#define X12   projecoes[0]
#define Y12   projecoes[1]
#define X20   projecoes[2]
#define Y20   projecoes[3]
#define X01   projecoes[4]
#define Y01   projecoes[5]


class Triangulo{
public:  
    Ponto *p0;
	Ponto *p1;
	Ponto *p2;
	float *projecoes;
	Matriz *g0;
	Matriz *g1;
	Matriz *g2;
	Matriz *g;
	Matriz *F;
	Matriz *h;
	Matriz *f;
	float *vFitted;
	float *pontosIniciais;

	vector<Textura> texturaInp;

public:
	Triangulo (Ponto *p0, Ponto *p1, Ponto *p2) {
		this->p0 = p0;
		this->p1 = p1;
		this->p2 = p2;
		this->pontosIniciais = new float[6];
		this->pontosIniciais[0]=this->p0->pX;
		this->pontosIniciais[1]=this->p0->pY;
		this->pontosIniciais[2]=this->p1->pX;
		this->pontosIniciais[3]=this->p1->pY;
		this->pontosIniciais[4]=this->p2->pX;
		this->pontosIniciais[5]=this->p2->pY;
		this->vFitted = new float[6];
		this->calcularProjecoes();
		this->calcularMatrizG0();
		this->calcularMatrizG1();
		this->calcularMatrizG2();
		this->calcularMatrizG();
		this->calcularMatrizF();
		this->calcularMatrizH();
    }

 public: 
	 void calcularProjecoes(){
		this->projecoes = new float[6];
		Vetor *v = new Vetor(this->p1,this->p2);
		Vetor *u = new Vetor(this->p1,this->p0);
		this->projecoes[0] = u->escalar(v)/ v->escalar(v);
		v->rotacao90();
		this->projecoes[1] = u->escalar(v)/ v->escalar(v);
		delete v;
		delete u;
		v = new Vetor(this->p2,this->p0);
		u = new Vetor(this->p2,this->p1);
		this->projecoes[2] = u->escalar(v)/ v->escalar(v);
		v->rotacao90();
		this->projecoes[3] = u->escalar(v)/ v->escalar(v);
		delete v;
		delete u;
		v = new Vetor(this->p0,this->p1);
		u = new Vetor(this->p0,this->p2);
		this->projecoes[4] = u->escalar(v)/v->escalar(v);
		v->rotacao90();
		this->projecoes[5] = u->escalar(v)/v->escalar(v);
		delete v;
		delete u;
	 }

	public: 
		void calcularMatrizG0(){
			this->g0 = new Matriz(6,6);
				
			float a11 = 1;
			float a12 = 0;
			float a13 = X12-1;
			float a14 = -1*Y12;
			float a15 = -1*X12;
			float a16 = Y12;

			float a21 = 0;
			float a22 = 1;
			float a23 = Y12;
			float a24 = X12 - 1;
			float a25 = -1*Y12;
			float a26 = -1*X12;

			float a31 = X12-1;
			float a32 = Y12;
			float a33 = -2*X12 + Y12*Y12 + X12*X12 + 1;
			float a34 = 0;
			float a35 = -1*X12*X12 + X12 - Y12*Y12;
			float a36 = -1*Y12;

			float a41 = -Y12;
			float a42 = X12-1;
			float a43 = 0;
			float a44 = Y12*Y12-2*X12+X12*X12+1;
			float a45 = Y12;
			float a46 = -1*Y12*Y12-1*X12*X12+X12;

			float a51 = -1*X12;
			float a52 = -1*Y12;
			float a53 = -1*X12*X12+X12-Y12*Y12;
			float a54 = Y12;
			float a55 = X12*X12+Y12*Y12;
			float a56 = 0;
			
			float a61 = Y12;
			float a62 = -1*X12;
			float a63 = -1*Y12;
			float a64 = -1*Y12*Y12-X12*X12+X12;
			float a65 = 0;
			float a66 = Y12*Y12+X12*X12;
			
			this->g0->valores[0][0] = a11;
			this->g0->valores[0][1] = a12;
			this->g0->valores[0][2] = a13;
			this->g0->valores[0][3] = a14;
			this->g0->valores[0][4] = a15;
			this->g0->valores[0][5] = a16;
			this->g0->valores[1][0] = a21;
			this->g0->valores[1][1] = a22;
			this->g0->valores[1][2] = a23;
			this->g0->valores[1][3] = a24;
			this->g0->valores[1][4] = a25;
			this->g0->valores[1][5] = a26;

			this->g0->valores[2][0] = a31;
			this->g0->valores[2][1] = a32;
			this->g0->valores[2][2] = a33;
			this->g0->valores[2][3] = a34;
			this->g0->valores[2][4] = a35;
			this->g0->valores[2][5] = a36;
			this->g0->valores[3][0] = a41;
			this->g0->valores[3][1] = a42;
			this->g0->valores[3][2] = a43;
			this->g0->valores[3][3] = a44;
			this->g0->valores[3][4] = a45;
			this->g0->valores[3][5] = a46;

			this->g0->valores[4][0] = a51;
			this->g0->valores[4][1] = a52;
			this->g0->valores[4][2] = a53;
			this->g0->valores[4][3] = a54;
			this->g0->valores[4][4] = a55;
			this->g0->valores[4][5] = a56;
			this->g0->valores[5][0] = a61;
			this->g0->valores[5][1] = a62;
			this->g0->valores[5][2] = a63;
			this->g0->valores[5][3] = a64;
			this->g0->valores[5][4] = a65;
			this->g0->valores[5][5] = a66;
		 }

	 public: 
		void calcularMatrizG1(){
			this->g1 = new Matriz(6,6);
			
			float a11 = X20*X20+Y20*Y20;
			float a12 = 0;
			float a13 = -1*X20;
			float a14 = -1*Y20;
			float a15 = X20-X20*X20-Y20*Y20;
			float a16 = Y20;

			float a21 = 0;
			float a22 = Y20*Y20+X20*X20;
			float a23 = Y20;
			float a24 = -1*X20;
			float a25 = -1*Y20;
			float a26 = X20-Y20*Y20-X20*X20;

			float a31 = -1*X20;
			float a32 = Y20;
			float a33 = 1;
			float a34 = 0;
			float a35 = X20-1;
			float a36 = -1*Y20;

			float a41 = -1*Y20;
			float a42 = -1*X20;
			float a43 = 0;
			float a44 = 1;
			float a45 = Y20;
			float a46 = X20-1;

			float a51 = X20-X20*X20-Y20*Y20;
			float a52 = -1*Y20;
			float a53 = X20-1;
			float a54 = Y20;
			float a55 = -2*X20+1+X20*X20+Y20*Y20;
			float a56 = 0;
			
			float a61 = Y20;
			float a62 = X20-Y20*Y20-X20*X20;
			float a63 = -1*Y20;
			float a64 = X20 - 1;
			float a65 = 0;
			float a66 = -2*X20+ 1 +X20*X20+Y20*Y20;
			
			this->g1->valores[0][0] = a11;
			this->g1->valores[0][1] = a12;
			this->g1->valores[0][2] = a13;
			this->g1->valores[0][3] = a14;
			this->g1->valores[0][4] = a15;
			this->g1->valores[0][5] = a16;
			this->g1->valores[1][0] = a21;
			this->g1->valores[1][1] = a22;
			this->g1->valores[1][2] = a23;
			this->g1->valores[1][3] = a24;
			this->g1->valores[1][4] = a25;
			this->g1->valores[1][5] = a26;

			this->g1->valores[2][0] = a31;
			this->g1->valores[2][1] = a32;
			this->g1->valores[2][2] = a33;
			this->g1->valores[2][3] = a34;
			this->g1->valores[2][4] = a35;
			this->g1->valores[2][5] = a36;
			this->g1->valores[3][0] = a41;
			this->g1->valores[3][1] = a42;
			this->g1->valores[3][2] = a43;
			this->g1->valores[3][3] = a44;
			this->g1->valores[3][4] = a45;
			this->g1->valores[3][5] = a46;

			this->g1->valores[4][0] = a51;
			this->g1->valores[4][1] = a52;
			this->g1->valores[4][2] = a53;
			this->g1->valores[4][3] = a54;
			this->g1->valores[4][4] = a55;
			this->g1->valores[4][5] = a56;
			this->g1->valores[5][0] = a61;
			this->g1->valores[5][1] = a62;
			this->g1->valores[5][2] = a63;
			this->g1->valores[5][3] = a64;
			this->g1->valores[5][4] = a65;
			this->g1->valores[5][5] = a66;
		}

	public: 
		void calcularMatrizG2(){
			
			this->g2 = new Matriz(6,6);
			
			float a11 = 1-2*X01+X01*X01+Y01*Y01;
			float a12 = 0;
			float a13 = -1*X01*X01+X01-Y01*Y01;
			float a14 = -1*Y01;
			float a15 = X01-1;
			float a16 = Y01;

			float a21 = 0;
			float a22 = Y01*Y01-2*X01+X01*X01+1;
			float a23 = Y01;
			float a24 = -1*Y01*Y01-X01*X01+X01;
			float a25 = -1*Y01;
			float a26 = X01-1;

			float a31 = -1*X01*X01+X01-Y01*Y01;
			float a32 = Y01;
			float a33 = X01*X01+Y01*Y01;
			float a34 = 0;
			float a35 = -1*X01;
			float a36 = -1*Y01;

			float a41 = -1*Y01;
			float a42 = -1*Y01*Y01-X01*X01+X01;
			float a43 = 0;
			float a44 = Y01*Y01+X01*X01;
			float a45 = Y01;
			float a46 = -1*X01;

			float a51 = X01-1;
			float a52 = -1*Y01;
			float a53 = -1*X01;
			float a54 = Y01;
			float a55 = 1;
			float a56 = 0;
			
			float a61 = Y01;
			float a62 = X01-1;
			float a63 = -1*Y01;
			float a64 = -1*X01;
			float a65 = 0;
			float a66 = 1;
			
			this->g2->valores[0][0] = a11;
			this->g2->valores[0][1] = a12;
			this->g2->valores[0][2] = a13;
			this->g2->valores[0][3] = a14;
			this->g2->valores[0][4] = a15;
			this->g2->valores[0][5] = a16;
			this->g2->valores[1][0] = a21;
			this->g2->valores[1][1] = a22;
			this->g2->valores[1][2] = a23;
			this->g2->valores[1][3] = a24;
			this->g2->valores[1][4] = a25;
			this->g2->valores[1][5] = a26;

			this->g2->valores[2][0] = a31;
			this->g2->valores[2][1] = a32;
			this->g2->valores[2][2] = a33;
			this->g2->valores[2][3] = a34;
			this->g2->valores[2][4] = a35;
			this->g2->valores[2][5] = a36;
			this->g2->valores[3][0] = a41;
			this->g2->valores[3][1] = a42;
			this->g2->valores[3][2] = a43;
			this->g2->valores[3][3] = a44;
			this->g2->valores[3][4] = a45;
			this->g2->valores[3][5] = a46;

			this->g2->valores[4][0] = a51;
			this->g2->valores[4][1] = a52;
			this->g2->valores[4][2] = a53;
			this->g2->valores[4][3] = a54;
			this->g2->valores[4][4] = a55;
			this->g2->valores[4][5] = a56;
			this->g2->valores[5][0] = a61;
			this->g2->valores[5][1] = a62;
			this->g2->valores[5][2] = a63;
			this->g2->valores[5][3] = a64;
			this->g2->valores[5][4] = a65;
			this->g2->valores[5][5] = a66;
		}

	public:
		void calcularMatrizG(){
			this->g = this->g0->copia();
			this->g->soma(this->g1);
			this->g->soma(this->g2);
			this->g->identificadores.push_back(p0->identificador);
			this->g->identificadores.push_back(p1->identificador);
			this->g->identificadores.push_back(p2->identificador);
		}
	public:
		void calcularMatrizH(){
			this->h =  new Matriz(6,6);
			h->valores[0][0] = 2;
			h->valores[0][1] = 0;
			h->valores[0][2] = -1;
			h->valores[0][3] = 0;
			h->valores[0][4] = -1;
			h->valores[0][5] = 0;

			h->valores[1][0] = 0;
			h->valores[1][1] = 2;
			h->valores[1][2] = 0;
			h->valores[1][3] = -1;
			h->valores[1][4] = 0;
			h->valores[1][5] = -1;

			h->valores[2][0] = -1;
			h->valores[2][1] = 0;
			h->valores[2][2] = 2;
			h->valores[2][3] = 0;
			h->valores[2][4] = -1;
			h->valores[2][5] = 0;

			h->valores[3][0] = 0;
			h->valores[3][1] = -1;
			h->valores[3][2] = 0;
			h->valores[3][3] = 2;
			h->valores[3][4] = 0;
			h->valores[3][5] = -1;

			h->valores[4][0] = -1;
			h->valores[4][1] = 0;
			h->valores[4][2] = -1;
			h->valores[4][3] = 0;
			h->valores[4][4] = 2;
			h->valores[4][5] = 0;

			h->valores[5][0] = 0;
			h->valores[5][1] = -1;
			h->valores[5][2] = 0;
			h->valores[5][3] = -1;
			h->valores[5][4] = 0;
			h->valores[5][5] = 2;
		}

	public:
		void calcularMatrizf(){
			this->f = new Matriz(1,6);
			f->valores[0][0] = (2*this->vFitted[2])-(4*this->vFitted[0])+(2*this->vFitted[4]);
			f->valores[0][1] = (2*this->vFitted[3])-(4*this->vFitted[1])+(2*this->vFitted[5]);
			f->valores[0][2] = (2*this->vFitted[0])-(4*this->vFitted[2])+(2*this->vFitted[4]);
			f->valores[0][3] = (2*this->vFitted[1])-(4*this->vFitted[3])+(2*this->vFitted[5]);
			f->valores[0][4] = (2*this->vFitted[2])-(4*this->vFitted[4])+(2*this->vFitted[0]);
			f->valores[0][5] = (2*this->vFitted[3])-(4*this->vFitted[5])+(2*this->vFitted[1]);

		}
	public:
		void concatenar(Matriz *gGlobal, Matriz *hCoef, int ident){
			Matriz *temp;
			if(ident == 0) temp = g->copia();
			if(ident == 1) temp = h->copia(); 
			Ponto *pLinha;
			Ponto *pColuna;
			for(int i = 0; i < 3;i++){
				for(int j = 0; j < 3;j++){
					if(i==0)pLinha = this->p0;
					if(i==1)pLinha = this->p1;
					if(i==2)pLinha = this->p2;
					if(j==0)pColuna = this->p0;
					if(j==1)pColuna = this->p1;
					if(j==2)pColuna = this->p2;
					int indiceLinha = this->pegarIndicePontoMatriz(pLinha->identificador,gGlobal->identificadores);
					int indiceColuna = this->pegarIndicePontoMatriz(pColuna->identificador,gGlobal->identificadores);
					gGlobal->valores[(indiceLinha*2)][(indiceColuna*2)] += temp->valores[(i*2)][(j*2)];
					gGlobal->valores[(indiceLinha*2)][(indiceColuna*2)+1] += temp->valores[(i*2)][(j*2)+1];
					gGlobal->valores[(indiceLinha*2)+1][(indiceColuna*2)] += temp->valores[(i*2)+1][(j*2)];
					gGlobal->valores[(indiceLinha*2)+1][(indiceColuna*2)+1] += temp->valores[(i*2)+1][(j*2)+1];
					
					if(ident == 1){
						hCoef->valores[(indiceLinha*2)][(indiceColuna*2)] += 1;					
						hCoef->valores[(indiceLinha*2)][(indiceColuna*2)+1] += 1;
						hCoef->valores[(indiceLinha*2)+1][(indiceColuna*2)] += 1;
						hCoef->valores[(indiceLinha*2)+1][(indiceColuna*2)+1] += 1;
					}
				}
			}
			delete temp;
		}
	public:
		void concatenarf0(Matriz *f0Global, Matriz *fCoef){
			Ponto *p;
			for(int i = 0; i < 3;i++){
				if(i==0)p = this->p0;
				if(i==1)p = this->p1;
				if(i==2)p = this->p2;
				int indiceLinha = this->pegarIndicePontoMatriz(p->identificador,f0Global->identificadores);
				if(indiceLinha != -1){
					f0Global->valores[(indiceLinha*2)][0] += this->f->valores[0][(i*2)];
					f0Global->valores[(indiceLinha*2)+1][0] += this->f->valores[0][(i*2)+1];

					fCoef->valores[(indiceLinha*2)][0] += 1;
					fCoef->valores[(indiceLinha*2)+1][0] += 1;
				}
			}
		}

	public:
		int pegarIndicePontoMatriz(int identificadorPonto,vector<int> identificadores){
			int indice = -1;
			for(int i = 0; i < (int)identificadores.size();i++){
				if(identificadores[i] == identificadorPonto) indice = i;
			}
			return indice;
		}

	public:
		void calcularMatrizF(){
			this->F = new Matriz(4,4);
			this->F->valores[0][0] = (4)-(4*X01)+(2*X01*X01)+(2*Y01*Y01);
			this->F->valores[0][1] = 0;
			this->F->valores[0][2] = (-2*X01*X01)+(2*X01)-(2*Y01*Y01);
			this->F->valores[0][3] = -2*Y01;

			this->F->valores[1][0] = 0;
			this->F->valores[1][1] = (4)-(4*X01)+(2*X01*X01)+(2*Y01*Y01);
			this->F->valores[1][2] = 2*Y01;
			this->F->valores[1][3] = (-2*X01*X01)+(2*X01)-(2*Y01*Y01);
			
			this->F->valores[2][0] = (-2*X01*X01)+(2*X01)-(2*Y01*Y01);
			this->F->valores[2][1] = 2*Y01;
			this->F->valores[2][2] = 2+(2*X01*X01)+(2*Y01*Y01);
			this->F->valores[2][3] = 0;

			this->F->valores[3][0] = -2*Y01;
			this->F->valores[3][1] = (-2*X01*X01)+(2*X01)-(2*Y01*Y01);
			this->F->valores[3][2] = 0;
			this->F->valores[3][3] = 2+(2*X01*X01)+(2*Y01*Y01);

			this->F->matrizInversa();
			this->F->multiplicar(-1);
		}

	public:
		void calcularVFitteds(){

			Matriz *c = new Matriz(4,1);
			c->valores[0][0] = (-2*this->p0->pX)-(2*this->p2->pX)+(2*this->p2->pX*X01)+(2*this->p2->pY*Y01);
			c->valores[1][0] = (-2*this->p0->pY)-(2*this->p2->pY)-(2*this->p2->pX*Y01)+(2*this->p2->pY*X01);
			c->valores[2][0] = (-2*this->p1->pX)-(2*this->p2->pX*X01)-(2*this->p2->pY*Y01);
			c->valores[3][0] = (-2*this->p1->pY)+(2*this->p2->pX*Y01)-(2*this->p2->pY*X01);

			Matriz *w = this->F->copia();
			w->multiplicar(c);

			float temp = sqrt( pow(w->valores[0][0]-w->valores[2][0],2)+pow(w->valores[1][0]-w->valores[3][0],2) );
			float temp2 = sqrt( pow((this->pontosIniciais[0]-this->pontosIniciais[2]),2) + pow((this->pontosIniciais[1]-this->pontosIniciais[3]),2) );
			float factor = (temp/temp2);
			//cout << factor << endl;
			delete c;
			
			double v2x = (w->valores[0][0]) + (X01*(w->valores[2][0]-w->valores[0][0])) + (Y01*(w->valores[1][0]-w->valores[3][0]));
			double v2y = (w->valores[1][0]) + (X01*(w->valores[3][0]-w->valores[1][0])) + (Y01*(w->valores[2][0]-w->valores[0][0]));
			
			float bariX = (w->valores[0][0]+w->valores[2][0]+v2x)/3;
			float bariY = (w->valores[1][0]+w->valores[3][0]+v2y)/3;

			this->vFitted[0] = (w->valores[0][0]-bariX)/factor;
			this->vFitted[1] = (w->valores[1][0]-bariY)/factor;
			this->vFitted[2] = (w->valores[2][0]-bariX)/factor;
			this->vFitted[3] = (w->valores[3][0]-bariY)/factor;
			this->vFitted[4] = (v2x-bariX)/factor;
			this->vFitted[5] = (v2y-bariY)/factor;
			
			this->calcularMatrizf();
		}

	public:
		bool pontoContido(Ponto *p){
			if(p->pX==this->p0->pX && p->pY==this->p0->pY)return true;
			else if(p->pX==this->p1->pX && p->pY==this->p1->pY)return true;
			else if(p->pX==this->p2->pX && p->pY==this->p2->pY)return true;
			else return false;
		}

};
#endif