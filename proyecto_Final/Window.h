#pragma once
#include<stdio.h>
#include<glew.h>
#include<glfw3.h>

class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);
	int Initialise();
	GLfloat getBufferWidth() { return bufferWidth; }
	GLfloat getBufferHeight() { return bufferHeight; }
	GLfloat getXChange();
	GLfloat getYChange();


	//-- VARIABLES QUE SE UTILIZAN EN EL CÓDIGO ---- //
	GLfloat getFlipperI() { return FlipperI; }
	GLfloat getFlipperISW() { return FlipperISW; }
	GLfloat getFlipperD() { return FlipperD; }
	GLfloat getReset() { return Reset; }
	GLfloat getPalanca() { return Palanca; }
	GLfloat getMonedaPinball() { return MonedaPinball; }
	GLfloat getActivaPalanca() { return ActivaPalanca; }
	GLfloat getActivaLanzamiento() { return  ActivaLanzamiento; }
	GLboolean getapagaLuz() { return apagaluz; }	//Metodo para apagar la luz de PINBALL 1
	int getCamaraVis() { return CamaraVis; }
	GLboolean getapagaLuzFoco() { return apagaluzFoco; }	//Metodo para apagar la luz superior de foco
	GLboolean getapagaLuzLinternaRoja() { return apagaluzLinternaRoja; }	//Metodo para apagar la luz
	GLboolean getapagaLuzLinternaAzul() { return apagaluzLinternaAzul; }	//Metodo para apagar la luz
	GLfloat getCanica1() { return Canica1; }
	GLfloat getCanica2() { return Canica2; }

	GLfloat getPieDKirby() { return PieDKirby; }
	GLfloat getPieIKirby() { return PieIKirby; }
	GLfloat getBraDKirby() { return BraDKirby; }
	GLfloat getBraIKirby() { return BraIKirby; }

	bool getShouldClose() {
		return  glfwWindowShouldClose(mainWindow);
	}
	bool* getsKeys() { return keys; }
	void swapBuffers() { return glfwSwapBuffers(mainWindow); }

	~Window();
private:
	GLFWwindow* mainWindow;
	GLint width, height;
	bool keys[1024];
	GLint bufferWidth, bufferHeight;
	void createCallbacks();

	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;

	//-- VARIABLES QUE SE UTILIZAN EN EL CÓDIGO ------ //
	GLfloat Reset;
	GLfloat FlipperD;
	GLfloat FlipperI;
	GLfloat FlipperISW;
	GLfloat Palanca;
	GLfloat MonedaPinball;
	GLfloat ActivaPalanca;
	GLfloat ActivaLanzamiento;
	GLboolean apagaluzFoco;				//Variable de control de apagado de la luz Verde
	GLboolean apagaluzLinternaRoja;				//Variable de control de apagado de la luz Roja
	GLboolean apagaluzLinternaAzul;				//Variable de control de apagado de la luz Azul
	int CamaraVis;
	bool Canica1;
	bool Canica2;

	GLfloat PieDKirby;
	GLfloat PieIKirby;
	GLfloat BraDKirby;
	GLfloat BraIKirby;

	// ---------------------------------------------- //

	GLboolean apagaluz;						//Variable de control de apagado de la lampara
	GLboolean apagaluz2;					//Variable de control de apagado de la lampara 2
	GLboolean apagaluzControl;					//Variable de control de apagado de la lampara control
	GLboolean apagaluzHelicoptero;			//Variable de control de apagado de Helicoptero
	
	GLboolean apagaluzCocheD;				//Variable de control de apagado de luz coche
	GLboolean apagaluzCocheT;				//Variable de control de apagado de luz coche
	GLfloat muevexCarro;
	GLfloat muevexCarroAdelante;
	GLfloat muevexCarroAtras;
	GLboolean CarroAdelante;
	GLboolean CarroAtras;
	GLfloat articulacion1;
	GLfloat angulocola = 0.0f;
	GLfloat anguloCofre = 0.0f;
	GLfloat angulopata1 = 0.0f;
	GLfloat angulopata2 = 0.0f;
	GLfloat angulopata3 = 0.0f;
	GLfloat angulopata4 = 0.0f;
	bool mouseFirstMoved;
	static void ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode);
	static void ManejaMouse(GLFWwindow* window, double xPos, double yPos);
	static void ManejaClicMouse(GLFWwindow* window, int button, int action, int mods);


};
