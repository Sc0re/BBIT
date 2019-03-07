#ifndef MATRIZ_H
#define MATRIZ_H

#pragma warning (disable: 4244)

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <vector>
#include "AuxMatrizMethods.h" 

using namespace math;

typedef matrix<double> Matrix;

#define SWAP(a,b) {float temp=(a);(a)=(b);(b)=temp;}

using namespace std;

class Matriz{
public:  
    int linhas;
	int colunas;
	float** valores;
	vector<int> identificadores;

	Matriz (int linhas, int colunas ) {
		this->linhas = linhas;
		this->colunas = colunas;
		this->valores = this->criarValores(linhas,colunas);
    }

	~Matriz(){
		this->deletarValores(this->linhas);
	}

	Matrix gerarMatrix(){
		Matrix temp = Matrix(this->linhas,this->colunas);
		for(int i = 0; i < this->linhas;i++){
			for(int j = 0; j < this->colunas;j++){
				temp(i,j) = this->valores[i][j];
			}
		}
		return temp;
	}

	void atualizarValores(Matrix m){
		for(int i = 0; i < this->linhas;i++){
			for(int j = 0; j < this->colunas;j++){
				this->valores[i][j] = m(i,j);
			}
		}
	}

	Matriz* copia(){
		Matriz *retorno = new Matriz(this->linhas,this->colunas);
		for(int i = 0;i < this->linhas;i++){
			for(int j = 0; j < this->colunas;j++){
				retorno->valores[i][j] = this->valores[i][j];
			}
		}
		return retorno;
	}

	void soma(Matriz *m2){
		for(int i = 0;i < this->linhas;i++){
			for(int j = 0;j < this->colunas;j++){
				this->valores[i][j] = this->valores[i][j] + m2->valores[i][j];
			}
		}
	}

	void subtrai(Matriz *m2){
		for(int i = 0;i < this->linhas;i++){
			for(int j = 0;j < this->colunas;j++){
				this->valores[i][j] = this->valores[i][j] - m2->valores[i][j];
			}
		}
	}

	void multiplicar(double valor){
		for(int i = 0;i < this->linhas;i++){
			for(int j = 0;j < this->colunas;j++){
				this->valores[i][j] = this->valores[i][j] * valor;
			}
		}
	}

	void multiplicar(Matriz *matriz){
		float** temp = this->criarValores(this->linhas,matriz->colunas);
		for(int i = 0;i < this->linhas;i++){
			for(int coluna = 0;coluna < matriz->colunas;coluna++){
				float soma = 0;
				for(int j = 0;j < this->colunas;j++){
					soma = soma + (this->valores[i][j] * matriz->valores[j][coluna]);
				}
				temp[i][coluna] = soma;
			}
		}
		this->deletarValores(this->linhas);
		this->valores = temp;
		this->colunas =  matriz->colunas;
	}

	float** criarValores(int linhas, int colunas){
		float **retorno = new float*[linhas];
		for(int i = 0;i < linhas;i++){
			retorno[i] = new float[colunas];
		}
		
		for(int i = 0;i < linhas;i++){
			for(int j = 0;j < colunas;j++){
				retorno[i][j]=0;
			}
		}
		return retorno;
	}

	void deletarValores(int linhas){
		for(int i = 0;i < linhas;i++){
			delete [] this->valores[i];
		}
		delete [] this->valores;
	}
	
	Matriz* submatriz(int iIni,int jIni,int iFim,int jFim){
		Matriz *matriz = new Matriz(iFim-iIni+1,jFim-jIni+1);
		int l = 0;
		for(int i = iIni;i <= iFim;i++){
			int c = 0;
			for(int j = jIni; j <= jFim;j++){
				matriz->valores[l][c] = this->valores[i][j];
				c++;
			}
			l++;
		}
		return matriz;
	}
	
	Matriz* transposta(){
		Matriz *matriz = new Matriz(this->colunas,this->linhas);
		for(int i = 0; i < this->linhas;i++){
			for(int j = 0; j < this->colunas;j++){
				matriz->valores[j][i] = this->valores[i][j];
			}
		}
		return matriz;
	}

	void imprimir(){
		cout << "linhas:" << this->linhas << " colunas:" << this->colunas << endl;
		cout << setiosflags( ios::fixed | ios::right ) <<setprecision ( 3 );
		for(int i = 0;i < this->linhas;i++){
			for(int j = 0;j < this->colunas;j++){
				cout << this->valores[i][j] << " ";
			}
			cout << "" << endl;
		}
		cout << "" << endl;
	}
	
	void matrizInversa(){
		Matrix m = gerarMatrix();
		m = !m; // Profiling the CPU during runtime using high triangle count meshes shows this as the bottleneck (over 99% of computation time). Needs replacement.
		this->atualizarValores(m);
		
		/*int n = this->linhas;
		int *indxc,*indxr,*ipiv;
		int i,icol,irow,j,k,l,ll;
		float big,dum,pivinv;

		indxc=new int[n];
		indxr=new int[n];
		ipiv=new int[n];
		for (j=0;j<n;j++) ipiv[j]=0;
		for (i=0;i<n;i++) {
			big=0.0;
			for (j=0;j<n;j++)
				if (ipiv[j] != 1)
					for (k=0;k<n;k++) {
						if (ipiv[k] == 0) {
							if (fabs(this->valores[j][k]) >= big) {
								big=fabs(this->valores[j][k]);
								irow=j;
								icol=k;
							}
						} else if (ipiv[k] > 1) cout << "GAUSSJ: Singular Matrix-1" << endl;
					}
			++(ipiv[icol]);
			if (irow != icol) {
				for (l=0;l<n;l++) SWAP(this->valores[irow][l],this->valores[icol][l])
			}
			indxr[i]=irow;
			indxc[i]=icol;
			if (this->valores[icol][icol] == 0.0) cout << "GAUSSJ: Singular Matrix-2" << endl;
			pivinv=1.0/this->valores[icol][icol];
			this->valores[icol][icol]=1.0;
			for (l=0;l<n;l++) this->valores[icol][l] *= pivinv;
			for (ll=0;ll<n;ll++)
				if (ll != icol) {
					dum=this->valores[ll][icol];
					this->valores[ll][icol]=0.0;
					for (l=0;l<n;l++) this->valores[ll][l] -= this->valores[icol][l]*dum;
				}
		}
		for (l=n-1;l>=0;l--) {
			if (indxr[l] != indxc[l])
				for (k=0;k<n;k++)
					SWAP(this->valores[k][indxr[l]],this->valores[k][indxc[l]]);
		}*/
	}

	void concatenarIdentificadores(vector<int> ident){
		for(int j = 0;j < (int)ident.size();j++){
			bool igual = false;
			for(int i = 0; i < (int)this->identificadores.size();i++){
				if(this->identificadores[i] == ident[j])igual = true;
			}
			if(!igual)this->identificadores.push_back(ident[j]);
		}
	}

};
#endif