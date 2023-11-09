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
	GLfloat getBufferWidth() { return bufferWidth; }#include "Window.h"

Window::Window()
{
	width = 800;
	height = 600;
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;

	//-- VARIABLES QUE SE UTILIZAN EN EL CÓDIGO ---- //

	FlipperD = 0.0f;
	FlipperI = 0.0f;
	Palanca = 0.0f;
	MonedaPinball = false;	
	ActivaPalanca = false;
	CamaraVis = 1;
	// ---------------------------------------------- //
	muevexCarro = 0.01f;
	muevexCarroAdelante = 0.01f;
	muevexCarroAtras = 0.01f;
	CarroAdelante = false;
	CarroAtras = false;
	apagaluz = true;
	apagaluz2 = true;
	apagaluzControl = true;
	apagaluzHelicoptero = false;//Definimos un estado inicial de nuestra variable
	apagaluzCocheD = false;		//Definimos un estado inicial de nuestra variable
	apagaluzCocheT = false;		//Definimos un estado inicial de nuestra variable

	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}


int Window::Initialise()
{
	//Inicialización de GLFW
	if (!glfwInit())
	{
		printf("Falló inicializar GLFW");
		glfwTerminate();
		return 1;
	}
	//Asignando variables de GLFW y propiedades de ventana
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//para solo usar el core profile de OpenGL y no tener retrocompatibilidad
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//CREAR VENTANA
	mainWindow = glfwCreateWindow(width, height, "Proyecto Final CGEIHC", NULL, NULL);

	if (!mainWindow)
	{
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;
	}
	//Obtener tamaño de Buffer
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	//asignar el contexto
	glfwMakeContextCurrent(mainWindow);

	//MANEJAR TECLADO y MOUSE
	createCallbacks();


	//permitir nuevas extensiones
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Falló inicialización de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST); //HABILITAR BUFFER DE PROFUNDIDAD
	// Asignar valores de la ventana y coordenadas

	//Asignar Viewport
	glViewport(0, 0, bufferWidth, bufferHeight);
	//Callback para detectar que se está usando la ventana
	glfwSetWindowUserPointer(mainWindow, this);
}

void Window::createCallbacks()
{
	glfwSetKeyCallback(mainWindow, ManejaTeclado);
	glfwSetCursorPosCallback(mainWindow, ManejaMouse);
}
GLfloat Window::getXChange()
{
	GLfloat theChange = xChange;
	xChange = 0.0f;
	return theChange;
}

GLfloat Window::getYChange()
{
	GLfloat theChange = yChange;
	yChange = 0.0f;
	return theChange;
}




void Window::ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}


	if (key == GLFW_KEY_1 && action == GLFW_PRESS)
	{
		theWindow->MonedaPinball = !(theWindow->MonedaPinball);
	}


	if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS) {
		// Ejecutar la acción mientras se mantiene presionada la tecla "6"
		theWindow->ActivaPalanca = true;
	}
	else 
	{
		// Realizar cualquier otra acción cuando la tecla se suelta
		theWindow->ActivaPalanca = false;
	}



	// ---- MOVIMIENTO DE LOS FLIPPERS ------- //

	if (key == GLFW_KEY_3)
	{
		if (theWindow->FlipperI >= 55.0) {

			theWindow->FlipperI = 0.0f;
		}
		else {
			theWindow->FlipperI += 55.0;
		}
	}

	if (key == GLFW_KEY_4)
	{
		if (theWindow->FlipperD >= 55.0) {

			theWindow->FlipperD = 0.0f;
		}
		else {
			theWindow->FlipperD += 55.0;
		}
	}

	if (key == GLFW_KEY_3)
	{
		if (theWindow->Palanca >= 55.0) {

			theWindow->Palanca = 0.0f;
		}
		else {
			theWindow->Palanca += 5.0;
		}
	}

	// ---------------------- CAMARA --------------------- //

	if (key == GLFW_KEY_Y)
	{
		theWindow->CamaraVis = 1;
	}
	if (key == GLFW_KEY_U)
	{
		theWindow->CamaraVis = 2;
	}
	if (key == GLFW_KEY_I)
	{
		theWindow->CamaraVis = 3;
	}





	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			theWindow->keys[key] = true;
			//printf("se presiono la tecla %d'\n", key);
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->keys[key] = false;
			//printf("se solto la tecla %d'\n", key);
		}
	}
}

void Window::ManejaMouse(GLFWwindow* window, double xPos, double yPos)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (theWindow->mouseFirstMoved)
	{
		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
		theWindow->mouseFirstMoved = false;
	}

	theWindow->xChange = xPos - theWindow->lastX;
	theWindow->yChange = theWindow->lastY - yPos;

	theWindow->lastX = xPos;
	theWindow->lastY = yPos;
}


Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();

}

	GLfloat getBufferHeight() { return bufferHeight; }
	GLfloat getXChange();
	GLfloat getYChange();


	//-- VARIABLES QUE SE UTILIZAN EN EL CÓDIGO ---- //
	GLfloat getFlipperI() { return FlipperI; }
	GLfloat getFlipperD() { return FlipperD; }
	GLfloat getPalanca() { return Palanca; }
	GLfloat getMonedaPinball() { return MonedaPinball; }
	GLfloat getActivaPalanca() { return ActivaPalanca; }

	int getCamaraVis() { return CamaraVis; }


	// ---------------------------------------------- //

	GLboolean getapagaLuz() { return apagaluz; }	//Metodo para apagar la luz de Lampara
	GLboolean getapagaLuz2() { return apagaluz2; }	//Metodo para apagar la luz de Lampara 2
	GLboolean getapagaLuzControl() { return apagaluzControl; }	//Metodo para apagar la luz de Lamparas
	GLboolean getapagaLuzHelicoptero() { return apagaluzHelicoptero; }	//Metodo para apagar la luz
	GLboolean getapagaLuzLinterna() { return apagaluzLinterna; }	//Metodo para apagar la luz
	GLboolean getapagaLuzCocheT() { return apagaluzCocheT; }	//Metodo para apagar la luz Trasera
	GLboolean getapagaLuzCocheD() { return apagaluzCocheD; }	//Metodo para apagar la luz Delantera 
	GLfloat getCarroAdelante() { return CarroAdelante; }
	GLfloat getCarroAtras() { return CarroAtras; }
	GLfloat getmuevexCarro() { return muevexCarro; }
	GLfloat getarticulacion1() { return articulacion1; }
	GLfloat getangulocola() { return angulocola; }
	GLfloat getanguloCofre() { return anguloCofre; }
	GLfloat getangulopata1() { return angulopata1; }
	GLfloat getangulopata2() { return angulopata2; }
	GLfloat getangulopata3() { return angulopata3; }
	GLfloat getangulopata4() { return angulopata4; }
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
	GLfloat FlipperD;
	GLfloat FlipperI;
	GLfloat Palanca;
	GLfloat MonedaPinball;
	GLfloat ActivaPalanca;
	int CamaraVis;

	// ---------------------------------------------- //

	GLboolean apagaluz;						//Variable de control de apagado de la lampara
	GLboolean apagaluz2;					//Variable de control de apagado de la lampara 2
	GLboolean apagaluzControl;					//Variable de control de apagado de la lampara control
	GLboolean apagaluzHelicoptero;			//Variable de control de apagado de Helicoptero
	GLboolean apagaluzLinterna;				//Variable de control de apagado de la Linterna
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

};
