#include "Window.h"

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
	Canica1 = false;
	CamaraVis = 1;
	// ---------------------------------------------- //
	muevexCarro = 0.01f;
	muevexCarroAdelante = 0.01f;
	muevexCarroAtras = 0.01f;
	CarroAdelante = false;
	CarroAtras = false;
	apagaluz = true;
	apagaluzLinternaVerde = false;
	apagaluzLinternaRoja = false;
	apagaluzLinternaAzul = false;
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
	mainWindow = glfwCreateWindow(width, height, "Proyecto Final CGEIHC VERSION 5: CAMARA FUNCIONAL Y NUEVA TEXTURA", NULL, NULL);

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

	//Para articulación de la rueda (Movimiento)
	if (key == GLFW_KEY_Y)		//Adelante
	{
		theWindow->articulacion1 += 10.0;
	}
	if (key == GLFW_KEY_U)		//Atras
	{
		theWindow->articulacion1 -= 10.0;
	}

	if (key == GLFW_KEY_V)
	{
		if (theWindow->anguloCofre > 45.0)
		{
		}
		else
		{
			theWindow->anguloCofre += 10.0;
		}
	}

	if (key == GLFW_KEY_B)
	{
		if (theWindow->anguloCofre < 1.0)
		{
		}
		else
		{
			theWindow->anguloCofre -= 10.0;
		}
	}

	if (key == GLFW_KEY_X && action == GLFW_PRESS)
	{
		theWindow->apagaluz = !(theWindow->apagaluz);
	}

	if (key == GLFW_KEY_L && action == GLFW_PRESS)
	{
		theWindow->apagaluz2 = !(theWindow->apagaluz2);
	}

	//	//Para animación de Canica1
	if (key == GLFW_KEY_O)
	{
		theWindow->Canica1 = true;
	}
	if (key == GLFW_KEY_P)
	{
		theWindow->Canica1 = false;
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






	if (key == GLFW_KEY_M && action == GLFW_PRESS)
	{
		theWindow->apagaluzLinternaVerde = !(theWindow->apagaluzLinternaVerde);
	}

	if (key == GLFW_KEY_N && action == GLFW_PRESS)
	{
		theWindow->apagaluzLinternaRoja = !(theWindow->apagaluzLinternaRoja);
	}

	if (key == GLFW_KEY_B && action == GLFW_PRESS)
	{
		theWindow->apagaluzLinternaAzul = !(theWindow->apagaluzLinternaAzul);
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

	// ---------------------------------------------- //

	if (key == GLFW_KEY_Y)
	{
		theWindow->CamaraVis = 1;


	}
	if (key == GLFW_KEY_U)
	{
		theWindow->CamaraVis = 2;
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
