#include "il/il.h"
#include "il/ilu.h"
#include "il/ilut.h"
#include <stdio.h>
#include <stdlib.h>
#include "glut.h"
#include <iostream>
#include <string>
#include <math.h>
#include "MarchingSquare.h"
#include "Controlador.h"
#include "Inpainting.h"
#include <sstream>
//#include "glui.h"
#include <algorithm>

using namespace std;
 
void exibeCB( void );
void tecladoCB( unsigned char tecla, int x, int y );
void redesenhaCB( int w, int h );
void inic( void );
void mouseCB( int botao, int estado, int x, int y );
void mouseMovCB( int x, int y);
void mouseMov( int x, int y);
bool carregarTexturas(char* imagem);
//void controlGUI( int control );
//void gerarGui();
void salvarImagem(string path);
void criarOBJ(char* imagem);

Controlador *controlador;
Inpainting inp;
int tamanhoTelaX = 800;
int tamanhoTelaY = 600;

//imagem
ILuint texid[2];
GLuint image[2];

//controle
bool movimentando = false;

int xMouse;
int yMouse;

int xSelect;
int ySelect;

//GLUI
int main_window;
int contImage = 0;
bool debug=true;
bool folderChoose;

bool inpaintOn=false;
float centroInpX=0;
float centroInpY=0;
float mouseInpX=0;
float mouseInpY=0;
vector<Triangulo> inpTriangulos;

//GLUI_EditText   *txtFolder;
//GLUI_EditText   *txtAtualImage;
//GLUI_EditText   *txtPorcPontos;

int main( int argc, char *argv[] ){
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
	inic();
	//gerarGui();
	glutMainLoop();
	return 1;
}

void inic( void ){

	glutInitWindowSize(tamanhoTelaX,tamanhoTelaY);
	glutInitWindowPosition( 0, 0 );
	main_window = glutCreateWindow( "Protótipo" );
	glutDisplayFunc( exibeCB );
	glutKeyboardFunc( tecladoCB );
	glutReshapeFunc( redesenhaCB );
	glutMouseFunc( mouseCB );
	glutMotionFunc( mouseMovCB );
	glutPassiveMotionFunc( mouseMov );
	
	glClearColor( 0, 0, 0, 0 );
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glClear( GL_COLOR_BUFFER_BIT );
	
	inp = Inpainting();

	string nomeImagem = string("");
	cout << "Digite o nome da imagem:";
	cin >> nomeImagem;

	while (!carregarTexturas((char*)nomeImagem.c_str()) ){
		cout << "Digite o nome da imagem:";
		cin >> nomeImagem;
	}

	double porcentagem = 0.0;
	cout << "Digite a porcentagem de pontos (ex 0.7):";
	cin >> porcentagem;
	porcentagem = porcentagem / 1000;

	int lerObj = 0;
	cout << "Deseja ler arquivo OBJ? (1/0):";
	cin >> lerObj;

	size_t pos = nomeImagem.find(".");
	string nomeObj = nomeImagem.substr(0,pos);
	
	unsigned char *imagePixels;
	imagePixels = new unsigned char[ilGetInteger(IL_IMAGE_WIDTH)*ilGetInteger(IL_IMAGE_HEIGHT)*4];
	ilCopyPixels(0,0,0,ilGetInteger(IL_IMAGE_WIDTH),ilGetInteger(IL_IMAGE_HEIGHT),1,IL_RGBA,IL_UNSIGNED_BYTE,imagePixels);
	controlador = new Controlador(ilGetInteger(IL_IMAGE_WIDTH),ilGetInteger(IL_IMAGE_HEIGHT),imagePixels,porcentagem,lerObj, (char*)nomeObj.c_str());
	
	for(int i = 0; i < (int)controlador->pontos->pontos.size();i++){
		controlador->pontos->pontos[i]->calcularTextura(ilGetInteger(IL_IMAGE_WIDTH),ilGetInteger(IL_IMAGE_HEIGHT));
	}
	if(!lerObj)criarOBJ((char*)nomeObj.c_str());
}
 
bool carregarTexturas(char* imagem){
	string textura = string("imagens/")+string(imagem);
	ilInit();
	ilGenImages(2, texid);
	ilBindImage(texid[0]);
	if (!ilLoadImage((char*)textura.c_str()) ){
		cout << "Imagem inexistente." << endl;
		return false;
	}
	ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
	glGenTextures(2, image);

	glBindTexture(GL_TEXTURE_2D, image[0]); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP), ilGetInteger(IL_IMAGE_WIDTH),ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE,ilGetData());

	/*ilBindImage(texid[1]);
	ilLoadImage("triangulo.gif");
	ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
	glBindTexture(GL_TEXTURE_2D, image[1]); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP), tamanhoTelaX,tamanhoTelaY, 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE,ilGetData());*/
	
	return true;
}

/*void gerarGui(){
	GLUI *glui = GLUI_Master.create_glui( "TG", 0, 811, 0 );

	glui->add_statictext( ".....Versao Beta....." );
	glui->add_separator();

	glui->add_statictext( "Select Mode:" );
	glui->add_checkbox("Debug",NULL,1,controlGUI);
	glui->add_checkbox("ScaleAdjust",NULL,2,controlGUI);
	glui->add_separator();	  

	txtFolder = glui->add_edittext( "Folder:", GLUI_EDITTEXT_TEXT, "projeto",0, controlGUI );
	glui->add_checkbox("Select",NULL,3,controlGUI);
	glui->add_separator();	 
	
	glui->add_button( "Save",4,controlGUI);
	glui->add_separator();

	txtPorcPontos = glui->add_edittext( "Porcentagem Pontos:", GLUI_EDITTEXT_TEXT, "0.07",0, controlGUI );
	glui->add_button( "OK",5,controlGUI);
	glui->add_separator();
	
	glui->add_statictext( "Select image to Load:" );
	txtAtualImage = glui->add_edittext( "Image:", GLUI_EDITTEXT_TEXT, "",0, controlGUI );
	glui->add_button( "Load",6,controlGUI);
	glui->add_separator();	

	glui->add_button( "Quit", 0,(GLUI_Update_CB)exit );
	glui->set_main_gfx_window( main_window );
}

void controlGUI( int control ){
	if(control == 1){
		debug = !debug;
	}
	else if(control == 2){
		controlador->withScaleAdjust = !controlador->withScaleAdjust;
	}
	else if(control == 3){// escolher folder
		if (!folderChoose){
			string pasta  = string("..\\imagens\\Projetos\\") + txtFolder->text;
			CreateDirectory((char*)pasta.c_str(),NULL);
			txtFolder->disable();
		}
		folderChoose = !folderChoose;
	}
	else  if(control == 4){// escolher nome da imagem
			if(folderChoose){
				string path = string("..\\imagens\\Projetos\\")+ txtFolder->text + string("\\");
				salvarImagem(path);
			}else{
				cout << "Selecione o folder primeiro!" << endl;
			}
	}
	else  if(control == 5){ //numero de pontos
		string temp = txtPorcPontos->text;
		double porcentagem = atof ( (char*)temp.c_str() );
		porcentagem = porcentagem/100;
		delete controlador;
		carregarTexturas("teddy.gif");
		unsigned char *imagePixels;
		imagePixels = new unsigned char[ilGetInteger(IL_IMAGE_WIDTH)*ilGetInteger(IL_IMAGE_HEIGHT)*4];
		ilCopyPixels(0,0,0,ilGetInteger(IL_IMAGE_WIDTH),ilGetInteger(IL_IMAGE_HEIGHT),1,IL_RGBA,IL_UNSIGNED_BYTE,imagePixels);
		controlador = new Controlador(ilGetInteger(IL_IMAGE_WIDTH),ilGetInteger(IL_IMAGE_HEIGHT),imagePixels,porcentagem,false, NULL);

	}
	glutPostRedisplay();
}*/

void mouseCB( int botao, int estado, int x, int y ){
	if(!inpaintOn){

		xMouse = x;
		yMouse = y;
		y = controlador->tamanhoTelaY -1 -y;
		static bool isSelecionado = false;
		if (estado == 0){
			isSelecionado = controlador->pontos->isPontoControle(controlador->pontos->buscarPontoMaisProximo(x,y));
			if(!isSelecionado){
				controlador->pontos->adicionarPontoControle(x,y);
				controlador->organizarG();
				controlador->calcularGLeB();
				controlador->organizarH();
				controlador->calcularHinvD();
			}
		}
		if (estado == 1){
			if(movimentando){
				movimentando = false;
			}else{
				if(isSelecionado){
					controlador->pontos->removerPontoControle(x,y);
					controlador->organizarG();
					controlador->calcularGLeB();
					controlador->organizarH();
					controlador->calcularHinvD();
				}
			}
		}
	}else{
		inpaintOn=false;
	}
	glutPostRedisplay();
}
void mouseMovCB( int x, int y){
	double dxi = x - xMouse;
	double dyi = y - yMouse;
	double di = sqrt(((double) (dxi*dxi) + (dyi*dyi)) );
	if(di > 25){
		y = controlador->tamanhoTelaY -1 -y;
		//movimentos de translação
		float movX = x - controlador->pontos->pontos[controlador->pontos->pontoSelecionado]->pX;
		float movY = y - controlador->pontos->pontos[controlador->pontos->pontoSelecionado]->pY;
		controlador->pontos->moverPonto(x,y);
		movimentando = true;
		//trantando a translação
		if(controlador->pontos->qntPontosControle == 1){
			for(int i = 0;i < (int)controlador->pontos->pontos.size();i++){
				if(i != controlador->pontos->pontoSelecionado){
					controlador->pontos->pontos[i]->pX += movX;
					controlador->pontos->pontos[i]->pY += movY;
				}
			}
		}else controlador->calcularMovimento();
		glutPostRedisplay();
	}
}

void exibeCB( void ){

	glClearColor( 0, 0, 0, 0 );
	glClear( GL_COLOR_BUFFER_BIT );

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, image[0]); 
	
	for(int i = 0; i < (int)controlador->triangulos.size();i++){
		glBegin(GL_TRIANGLES);
			glTexCoord2f(controlador->triangulos[i]->p0->textura->tX,controlador->triangulos[i]->p0->textura->tY);glVertex2f(controlador->triangulos[i]->p0->pX,controlador->triangulos[i]->p0->pY);
			glTexCoord2f(controlador->triangulos[i]->p1->textura->tX,controlador->triangulos[i]->p1->textura->tY);glVertex2f(controlador->triangulos[i]->p1->pX,controlador->triangulos[i]->p1->pY);
			glTexCoord2f(controlador->triangulos[i]->p2->textura->tX,controlador->triangulos[i]->p2->textura->tY);glVertex2f(controlador->triangulos[i]->p2->pX,controlador->triangulos[i]->p2->pY);
		glEnd();
	}
	
	for(int i = 0; i < (int)inp.triangulos.size();i++){
		glBegin(GL_TRIANGLES);
			glTexCoord2f(inp.triangulos[i]->texturaInp[0].tX,inp.triangulos[i]->texturaInp[0].tY);glVertex2i(inp.triangulos[i]->p0->pX,inp.triangulos[i]->p0->pY);
			glTexCoord2f(inp.triangulos[i]->texturaInp[1].tX,inp.triangulos[i]->texturaInp[1].tY);glVertex2i(inp.triangulos[i]->p1->pX,inp.triangulos[i]->p1->pY);
			glTexCoord2f(inp.triangulos[i]->texturaInp[2].tX,inp.triangulos[i]->texturaInp[2].tY);glVertex2i(inp.triangulos[i]->p2->pX,inp.triangulos[i]->p2->pY);
		glEnd();
	}

	glDisable(GL_TEXTURE_2D);

	if (debug){
		for(int i = 0; i < (int)controlador->triangulos.size();i++){
				glBegin(GL_LINE_LOOP);
					glVertex2i(controlador->triangulos[i]->p0->pX,controlador->triangulos[i]->p0->pY);
					glVertex2i(controlador->triangulos[i]->p1->pX,controlador->triangulos[i]->p1->pY);
					glVertex2i(controlador->triangulos[i]->p2->pX,controlador->triangulos[i]->p2->pY);
				glEnd();
		}
	

		for(int i = 0; i < (int)inp.triangulos.size();i++){
				glBegin(GL_LINE_LOOP);
					glVertex2i(inp.triangulos[i]->p0->pX,inp.triangulos[i]->p0->pY);
					glVertex2i(inp.triangulos[i]->p1->pX,inp.triangulos[i]->p1->pY);
					glVertex2i(inp.triangulos[i]->p2->pX,inp.triangulos[i]->p2->pY);
				glEnd();
		}
	}
	glPointSize(8);
	for(int i = 0; i < (int)controlador->pontos->pontos.size();i++){
		if(controlador->pontos->isPontoControle(i)){
			glBegin(GL_POINTS);
				glVertex2i(controlador->pontos->pontos[i]->pX,controlador->pontos->pontos[i]->pY);
			glEnd();
		}
	}
	glPointSize(4);
	for(int i = 0; i < (int)inp.poligono.size();i++){
		glBegin(GL_POINTS);
			glVertex2i(inp.poligono[i]->pX,inp.poligono[i]->pY);
		glEnd();
	}

	if(inpaintOn){
			glBegin(GL_LINE_LOOP);
			for(int i = 0; i < (int)inp.poligono.size();i++){
					glVertex2i(inp.poligono[i]->pX+(mouseInpX-centroInpX),inp.poligono[i]->pY+(mouseInpY-centroInpY));
				}
			glEnd();
	}

    glutSwapBuffers();
	glFlush();
}

void mouseMov( int x, int y){
	if(inpaintOn){
		y = controlador->tamanhoTelaY -1 -y;
		mouseInpX=x;
		mouseInpY=y;
		for(int i = 0;i < (int)inp.triangulos.size();i++){
			inp.triangulos[i]->texturaInp[0]=inp.triangulos[i]->p0->pegarTextura(ilGetInteger(IL_IMAGE_WIDTH),ilGetInteger(IL_IMAGE_HEIGHT),mouseInpX-centroInpX,mouseInpY-centroInpY);
			inp.triangulos[i]->texturaInp[1]=inp.triangulos[i]->p1->pegarTextura(ilGetInteger(IL_IMAGE_WIDTH),ilGetInteger(IL_IMAGE_HEIGHT),mouseInpX-centroInpX,mouseInpY-centroInpY);
			inp.triangulos[i]->texturaInp[2]=inp.triangulos[i]->p2->pegarTextura(ilGetInteger(IL_IMAGE_WIDTH),ilGetInteger(IL_IMAGE_HEIGHT),mouseInpX-centroInpX,mouseInpY-centroInpY);
		}
	}
	glutPostRedisplay();
}

void tecladoCB( unsigned char tecla, int x, int y ){
	y = controlador->tamanhoTelaY -1 -y;
	if(tecla=='d'){
		xSelect = x;
		ySelect = y;
	}
	if(tecla=='f'){
		int xSelect2 = x;
		int ySelect2 = y;

		if (xSelect2 < xSelect){
			int temp = xSelect2;
			xSelect2 = xSelect;
			xSelect = temp;
		}

		if (ySelect2 < ySelect){
			int temp = ySelect2;
			ySelect2 = ySelect;
			ySelect = temp;
		}

		for(int i = 0; i < (int)controlador->pontos->pontos.size();i++){
			if(controlador->pontos->pontos[i]->pX <= xSelect2  &&  controlador->pontos->pontos[i]->pY <= ySelect2){
				if(controlador->pontos->pontos[i]->pX >= xSelect  &&  controlador->pontos->pontos[i]->pY >= ySelect){
					inp.poligono.push_back(controlador->pontos->pontos[i]);
				}
			}
		}
	}
	if(tecla=='i'){
		inp.imprimir();
	}
	if(tecla=='c'){
		//controlador->pontos->incluirPonto(x,y);
		inp.poligono.push_back(controlador->pontos->pontos[controlador->pontos->buscarPontoMaisProximo(x,y)]);
	}
	if(tecla=='v'){
		//controlador->pontos->incluirPonto(x,y);
		inp.poligono.push_back(controlador->pontos->pontos[controlador->pontos->buscarPontoMaisProximo(x,y)]);
		
		Ponto *p0 = inp.poligono[inp.poligono.size()-2];
		Ponto *p1 = inp.poligono[inp.poligono.size()-1];
		
		float xT = (p0->pX)+(0.25*p1->pX)-(0.25*p0->pX);
		float yT = (p0->pY)+(0.25*p1->pY)-(0.25*p0->pY);

		Ponto *p = new Ponto(xT,yT,(int)controlador->pontos->pontos.size());
		//controlador->pontos->incluirPonto( xT, yT);
		inp.poligono.push_back(p);
		

		xT = (p0->pX)+(0.5*p1->pX)-(0.5*p0->pX);
		yT = (p0->pY)+(0.5*p1->pY)-(0.5*p0->pY);

		p = new Ponto(xT,yT,(int)controlador->pontos->pontos.size());
		//controlador->pontos->incluirPonto( xT, yT);
		inp.poligono.push_back(p);

		xT = (p0->pX)+(0.75*p1->pX)-(0.75*p0->pX);
		yT = (p0->pY)+(0.75*p1->pY)-(0.75*p0->pY);
		p = new Ponto(xT,yT,(int)controlador->pontos->pontos.size());
		//controlador->pontos->incluirPonto( xT, yT);
		inp.poligono.push_back(p);
	}
	if(tecla=='s'){
		inp.poligono.push_back(controlador->pontos->pontos[controlador->pontos->buscarPontoMaisProximo(x,y)]);
	}
	if(tecla=='p'){
		inpaintOn=true;
		inp.triangularizar();
		for(int i = 0;  i < (int)inp.poligono.size();i++){
			centroInpX += inp.poligono[i]->pX;
			centroInpY += inp.poligono[i]->pY;
		}
		centroInpX = centroInpX/inp.poligono.size();
		centroInpY = centroInpY/inp.poligono.size();

		int indiceTri = -1;

		for(int i = 0; i < (int)controlador->triangulos.size() && indiceTri == -1;i++){
			if(controlador->triangulos[i]->pontoContido(inp.poligono[0]))indiceTri=i;
		}

		for(int i = 0;i < (int)inp.triangulos.size();i++){
			inp.triangulos[i]->texturaInp.push_back(Textura(controlador->triangulos[indiceTri]->p0->textura->tX,controlador->triangulos[indiceTri]->p0->textura->tY));
			inp.triangulos[i]->texturaInp.push_back(Textura(controlador->triangulos[indiceTri]->p1->textura->tX,controlador->triangulos[indiceTri]->p1->textura->tY));
			inp.triangulos[i]->texturaInp.push_back(Textura(controlador->triangulos[indiceTri]->p2->textura->tX,controlador->triangulos[indiceTri]->p2->textura->tY));
		}
		//inp.poligono.clear();
	}

	glutPostRedisplay();
}
void redesenhaCB( int w, int h ){
	glViewport( 0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	controlador->mudarTamanhoTela(w,h);
	gluOrtho2D( 0, controlador->tamanhoTelaX, 0, controlador->tamanhoTelaY);
}

void salvarImagem(string path){
	//lendo os pixels da tela atraves do opengl
	unsigned char *temp = new unsigned char[tamanhoTelaX * tamanhoTelaY * 4];
	glReadPixels(0, 0, tamanhoTelaX, tamanhoTelaY, GL_RGBA, GL_UNSIGNED_BYTE, temp);
	
	unsigned char *pixels = new unsigned char[tamanhoTelaX * tamanhoTelaY * 4];
	
	int indice = 0;
	for(int y = 0; y < tamanhoTelaY;y++){
		for(int x = 0; x < tamanhoTelaX;x++){
			int indiceR = (tamanhoTelaX*(tamanhoTelaY-y-1) + x)*4;
			pixels[indice++] = temp[indiceR++];
			pixels[indice++] = temp[indiceR++];
			pixels[indice++] = temp[indiceR++];
			pixels[indice++] = temp[indiceR++];
		}
	}
	
	ilSetPixels(0,0,0,tamanhoTelaX,tamanhoTelaY,1,IL_RGBA,IL_UNSIGNED_BYTE,pixels);
	
	//contador da imagem
	
	//nome da imagem
	stringstream contador;
	contador << ++contImage;
	
	//nome da imagem
	string nome = path + contador.str() + string(".jpg");
	ilSaveImage((char*)nome.c_str());

	delete temp;
	delete pixels;
	glutPostRedisplay();
}

void criarOBJ(char* imagem){
	string dirArquivo = string("..//Obj//")+string(imagem)+string(".obj");
	ofstream out((char*)dirArquivo.c_str());
	for(int i = 0;i < (int)controlador->pontos->pontos.size();i++){
		int pos = controlador->pontos->pegarPosicaoPonto(i);
		out << "v ";
		out << controlador->pontos->pontos[pos]->pX << " ";
		out << controlador->pontos->pontos[pos]->pY << " ";
		out << controlador->pontos->pontos[pos]->identificador << endl;
	}

	for(int i = 0;i < (int)controlador->triangulos.size();i++){
		out << "f ";
		out << controlador->triangulos[i]->g->identificadores[0]+1 << " ";
		out << controlador->triangulos[i]->g->identificadores[1]+1 << " ";
		out << controlador->triangulos[i]->g->identificadores[2]+1 << " " << endl;;
	}
	out.close();
}
