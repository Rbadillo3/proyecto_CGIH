/*
UNIVERSIDAD NACIONAL AUTÓNOMA DE MÉXICO
FACULTAD DE INGENIERIA
Proyecto Final: Tablero de Pinball
Grupo de Teoría: 04
Integrantes:
-Alvarez Badillo Rodrigo
-Arriaga Vitela Carlos Eduardo
-Rivas Arteaga Enrique Alan
*/


//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

//Para implementación de audio

#include <irrKlang.h>
using namespace irrklang;

// =========				Variables de control para animación				===============

//========== Dia y noche =====
float contadorDiaNoche = 0.0f;
bool dia = true;
bool noche = false;
bool skyboxDia = true;
bool skyboxNoche = false;

//====	Moneda
float movMoneda;
float movMonedaOffset;
float rotMonedaOffset;
float rotMoneda;
bool avanzaMoneda;

//====	Canica
float movCanicaY;
float movCanicaZ;
float movCanicaX;
float movPalanca;
float movPalancaOffset;
float movCanicaOffset;
float rotCanica;
float rotCanicaOffset;

bool avanzaCanicaX;
bool avanzaCanicaY;
bool avanzaCanicaZ;
bool palancaLista;
bool lanzamientoCanica;

//Animacion Canica 1
bool CanicaAnim;
int Canica1Edo;
float CanX;
float velCanX;
float CanZ;
float velCanZ;

bool Canica1Anim;

float movOffset;
bool avanza;


//Animacion Canica2
bool CanicaAnim2;
int Canica2Edo;
float Can2X;
float Can2Z;
int rebote;

bool Canica2Anim;

float newVel;

//Variables de control Animación Canica3 (Keyframes)
bool CanicaAnim3;
bool Canica3Anim;

//Variables de control de fin de animación
bool finAnim1;
bool finAnim2;
bool finAnim3;

//Variables enfocadas en la animación de Textura DE PUNTAJE
float decU, decV;
float numU, numV;
int num, num2;

//Para las decenas
float valorCoordUDecenas;
float valorCoordVDecenas;
float coordU;
float coordV;

//Variables para animación de Objeto Jerárquico
bool chopper1;
float giroChopper;
float saltoChopper1;
float saltoChopper;
bool chopper2;
float giroChopper2;
float velGiroChopper;
float saltoOffset;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;
Texture white;
Texture NumerosTexture;
Texture estrellaTexture;
Texture rocosaTexture;

// =========				Variables de MODELOS				===============


//====	Star Wars
Model CañoneraLAAT;
Model ComandanteFordo;
Model FighterTank;
Model Chopper;
Model Cuerpo;
Model Rueda;
Model Cabeza;
Model Brazo_Izq;
Model Brazo_Der;


void CreateStar() {
	unsigned int indicesEstrella[] = {
	0, 1, 2,
	3, 4, 5,
	5, 6, 7,
	7, 8, 9,
	9, 10, 11,
	2, 1, 9,
	1, 5, 9,
	5, 7, 9
	};

	GLfloat verticesEstrella[] = {
		//		x      		y 		     z				u	  v			nx	  ny    nz
			0.054193f, -0.55023f, 0.83147f,	         0.0f, 0.0f,		1.0f, 0.0f, 0.0f,
			0.15008f,  -0.12317f, 0.98079f,		     0.5f, 0.0f,		0.0f, 1.0f, 0.0f,
			-0.13795f,  -0.13795f, 0.98079f,		 1.0f, 0.0f,		0.0f, 0.0f, 1.0f,

			0.15008f,  -0.12317f, 0.98079f,		     0.5f, 0.0f,		0.0f, 0.0f, 1.0f,
			0.55023f,  -0.054193f, 0.83147f,		 0.5f, 0.0f,		0.0f, 1.0f, 0.0f,
			0.25788f,  0.12742f, 0.95233f,		     0.5f, 0.0f,		1.0f, 0.0f, 0.0f,

			0.30866f,  0.46194f, 0.83147f,		     0.5f, 0.0f,		1.0f, 0.0f, 0.0f,
			0.037329f,  0.28521f, 0.95233f,		     0.5f, 0.0f,		0.0f, 1.0f, 0.0f,

			-0.34285f,  0.39285f, 0.83147f,		     0.5f, 0.0f,		0.0f, 1.0f, 0.0f,
			-0.27245f,  0.092253f, 0.95233f,		 0.5f, 0.0f,		1.0f, 0.0f, 0.0f,

			-0.53089f,  -0.11439f, 0.83147f,		 0.5f, 0.0f,		1.0f, 0.0f, 0.0f,
			-0.13795f,  -0.13795f, 0.98079f,		 1.0f, 0.0f,		0.0f, 1.0f, 0.0f,

	};

	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(verticesEstrella, indicesEstrella, 96, 24);
	meshList.push_back(obj1);
}

void CreatePiramide() {

	unsigned int indices_piramide_triangular[] = {
			0,1,2,
			1,3,2,
			3,0,2,
			1,0,3

	};
	GLfloat vertices_piramide_triangular[] = {

		//		x      		y 		     z				u	  v			nx	  ny    nz
			 -0.5f,       -0.5f,       0.0f,			0.5f, 0.5f,		0.0f, 0.0f, 0.0f,
			  0.5f,		  -0.5f,	   0.0f,			0.5f, 0.5f,		0.0f, 0.0f, 0.0f,
			  0.0f,		   0.5f,	  -0.25f,			0.5f, 0.5f,		0.0f, 0.0f, 0.0f,
			  0.0f,       -0.5f,      -0.5f,			0.5f, 0.5f,		0.0f, 0.0f, 0.0f,

	};
	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices_piramide_triangular, indices_piramide_triangular, 12, 12);
	meshList.push_back(obj1);
}

void CreateScore() {
	unsigned int scoreIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat scoreVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};
	Mesh* obj6 = new Mesh();
	obj6->CreateMesh(scoreVertices, scoreIndices, 32, 6);
	meshList.push_back(obj6);
}



//====	Kirby
Model Kirby;
Model Apple;
Model Whispy;

//====	Billy y Mandy
Model Billy;
Model Grim;
Model Mandy;
Model Irwing;
Model Prisionero;
Model Ojo;
Model Mano;
Model Roca;
Model Lava;

//====	Elementos del PINBALL
Model Pinball;
Model Coin;
Model Canica;
Model Tapa;
Model FlipperI;
Model FlipperD;
Model Base_Palanca;
Model Palanca;
Model Resorte;
Model FlipperSable;
Model Pantalla;




Skybox skybox;

//materiales
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";


//función de calculo de normales por promedio de vértices 
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}

void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	unsigned int vegetacionIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	   4,5,6,
	   4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,


	};

	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh* obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);

}

// Pirámide triangular regular
void CrearPiramideTriangular()
{
	unsigned int indices_piramide_triangular[] = {
			0,1,2,
			1,3,2,
			3,0,2,
			1,0,3

	};
	GLfloat vertices_piramide_triangular[] = {
		-0.5f, -0.5f,0.0f,	//0
		0.5f,-0.5f,0.0f,	//1
		0.0f,0.5f, -0.25f,	//2
		0.0f,-0.5f,-0.5f,	//3

	};
	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices_piramide_triangular, indices_piramide_triangular, 12, 12);
	meshList.push_back(obj1);

}

void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}


////////////////////// FUNCIONES Y VALORES INICIALES DE KEYFRAMES //////////////////////

//variables para keyframes
float reproduciranimacion, habilitaranimacion, reinicioFrame, ciclo, ciclo2, contador = 0.0f;

//función para teclado de keyframes 
void inputKeyframes(bool* keys);

bool animacion = false;



//NEW// Keyframes
float posXavion = 2.0, posYavion = 5.0, posZavion = -3.0;
float	movAvion_x = 0.0f, movAvion_y = 0.0f;

#define MAX_FRAMES 100
int i_max_steps = 90;
int i_curr_steps = 18;		//Cantidad de frames
typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float movAvion_x;		//Variable para PosicionX
	float movAvion_y;		//Variable para PosicionY
	float movAvion_xInc;		//Variable para IncrementoX
	float movAvion_yInc;		//Variable para IncrementoY
}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 18;			//introducir datos
bool play = false;
int playIndex = 0;


void resetElements(void) //Tecla 9
{

	movAvion_x = KeyFrame[0].movAvion_x;
	movAvion_y = KeyFrame[0].movAvion_y;
}

void interpolation(void)
{
	KeyFrame[playIndex].movAvion_xInc = (KeyFrame[playIndex + 1].movAvion_x - KeyFrame[playIndex].movAvion_x) / i_max_steps;
	KeyFrame[playIndex].movAvion_yInc = (KeyFrame[playIndex + 1].movAvion_y - KeyFrame[playIndex].movAvion_y) / i_max_steps;

}


void animate(void)
{
	//Movimiento del objeto con barra espaciadora
	if (play)
	{
		if (i_curr_steps >= i_max_steps) //fin de animación entre frames?
		{
			playIndex++;
			//printf("playindex : %d\n", playIndex);
			if (playIndex > FrameIndex - 2)	//Fin de toda la animación con último frame?
			{
				//printf("Frame index= %d\n", FrameIndex);
				//printf("termino la animacion\n");
				playIndex = 0;
				play = false;
			}
			else //Interpolación del próximo cuadro
			{

				i_curr_steps = 0; //Resetea contador
				//Interpolar
				interpolation();
			}
		}
		else
		{
			//Dibujar Animación
			movAvion_x += KeyFrame[playIndex].movAvion_xInc;
			movAvion_y += KeyFrame[playIndex].movAvion_yInc;
			i_curr_steps++;
		}

	}
}

////////////////////// *** FIN KEYFRAMES *** //////////////////////



int main()
{
	mainWindow = Window(1920, 1080); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateStar();
	CreatePiramide();
	CreateScore();
	CreateObjects();
	CreateShaders();


	// ================ CÁMARAS ================================
	Camera dynamicCamera(
		glm::vec3(205.0f, 42.5f, 510.5f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		-90.0f,
		0.0f,
		0.3f,
		0.5f);

	Camera staticCamera(
		glm::vec3(0.0f, 1600.0f, 1125.0f), // Posición de la cámara en el eje Y sobre el origen
		glm::vec3(0.0f, 1.0f, 0.0f),  // Mirando hacia abajo en el eje Y
		-90.0f,                               // Yaw de 45 grados para rotar la cámara
		-50.0f,                          // Pitch a -90 grados para mirar hacia abajo
		0.0f,                           // La velocidad de movimiento no es relevante para la cámara estática
		0.0f                            // La velocidad de giro tampoco es relevante para la cámara estática
	);

	Camera avatarCamera(
		glm::vec3(0.0f, 100.0f, 590.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		90.0f,
		15.0f,
		0.3f,
		0.5f                           // La velocidad de giro tampoco es relevante para la cámara estática
	);

	Camera* camera = &dynamicCamera;

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	AgaveTexture = Texture("Textures/Agave.tga");
	AgaveTexture.LoadTextureA();

	estrellaTexture = Texture("Textures/estrella.tga");
	estrellaTexture.LoadTextureA();
	rocosaTexture = Texture("Textures/rocosa.tga");
	rocosaTexture.LoadTextureA();

	NumerosTexture = Texture("Textures/numerosbase.tga");
	NumerosTexture.LoadTextureA();

	//	Se cargan todos los objetos 

	// ----------- Elementos Pinball -------------
	Pinball = Model();
	Pinball.LoadModel("Models/PINBALL_V6.obj");

	Coin = Model();
	Coin.LoadModel("Models/Coin.obj");

	Canica = Model();
	Canica.LoadModel("Models/CanicaGold.obj");

	Tapa = Model();
	Tapa.LoadModel("Models/Tapa.obj");

	FlipperI = Model();
	FlipperI.LoadModel("Models/hueso_FlipperIzquierda.obj");

	FlipperD = Model();
	FlipperD.LoadModel("Models/hueso_FlipperDerecha.obj");

	Base_Palanca = Model();
	Base_Palanca.LoadModel("Models/Base_Palanca.obj");

	Palanca = Model();
	Palanca.LoadModel("Models/palanca.obj");

	Resorte = Model();
	Resorte.LoadModel("Models/spring.obj");

	//Pantalla = Model();
	//Pantalla.LoadModel("Models/pantalla.obj");


	// ----------- Elementos Star Wars -------------
	CañoneraLAAT = Model();
	CañoneraLAAT.LoadModel("Models/rep_la_at_gunship.obj");
	FighterTank = Model();
	FighterTank.LoadModel("Models/rep_hover_fightertank.obj");
	ComandanteFordo = Model();
	ComandanteFordo.LoadModel("Models/Fordo.obj");
	FlipperSable = Model();
	FlipperSable.LoadModel("Models/lightsaber.obj");

	//Chopper Objeto Jerárquico Animado Instanciado Varias Veces (OJAIVV)
	Chopper = Model();
	Chopper.LoadModel("Models/SWRChopper.obj");
	Cuerpo = Model();
	Cuerpo.LoadModel("Models/Cuerpo.obj");
	Rueda = Model();
	Rueda.LoadModel("Models/Rueda.obj");
	Cabeza = Model();
	Cabeza.LoadModel("Models/Cabeza.obj");
	Brazo_Izq = Model();
	Brazo_Izq.LoadModel("Models/BrazoIzquierdo.obj");
	Brazo_Der = Model();
	Brazo_Der.LoadModel("Models/BrazoDerecho.obj");

	// ----------- Elementos Kirby -------------

	Kirby = Model();
	Kirby.LoadModel("Models/kirby.obj");
	Apple = Model();
	Apple.LoadModel("Models/Apple.obj");
	Whispy = Model();
	Whispy.LoadModel("Models/Arbol.obj");

	// ----------- Elementos Billy y Mandy -------------
	Billy = Model();
	Billy.LoadModel("Models/Billy.obj");

	Grim = Model();
	Grim.LoadModel("Models/Grim.obj");

	Mandy = Model();
	Mandy.LoadModel("Models/Mandy.obj");

	Irwing = Model();
	Irwing.LoadModel("Models/IrwinBanana.obj");

	Prisionero = Model();
	Prisionero.LoadModel("Models/Prisoner_Corpse_Damage_0.obj");

	Ojo = Model();
	Ojo.LoadModel("Models/Hell eye.obj");

	Mano = Model();
	Mano.LoadModel("Models/JNlefthnd.obj");

	Roca = Model();
	Roca.LoadModel("Models/rtm_rock_space.obj");

	Lava = Model();
	Lava.LoadModel("Models/lava.obj");


	ISoundEngine* soundEngine = nullptr;

	if (avanzaMoneda = true) {
		//printf("Sonido");
		// Crear el motor de sonido
		soundEngine = createIrrKlangDevice();

		if (!soundEngine) {
			// Manejo de error si no se pudo crear el motor de sonido
			return 1;
		}
	}
	// Ruta al archivo MP3 que deseas reproducir
	const char* mp3FilePath = "Models/Bowser-Peaches-_Official-Music-Video_-_-The-Super-Mario-Bros.-Movie.wav";

	// Reproduce el archivo WAV
	///*

	if (avanzaMoneda = true) {
		// Cargar un sonido
		ISound* sound = soundEngine->play2D(mp3FilePath, true, false, true);
		//ISound* sound3dimens = soundEngine->play3D("Models/The Clone Wars_ ARC Trooper Theme _ EPIC VERSION.wav", vec3df(0, 0, 0), true, false, true);
		if (sound) {
			sound->setVolume(0.4f);
		}

		//if (sound3dimens) {
		//	sound3dimens->setVolume(1.0f);
		//	sound->setIsPaused(false);
		//	
		//	sound3dimens->setPosition(vec3df(0.0f, 0.0f, 0.0f)); // establecer la posición en el espacio 3D
		//	sound->setMaxDistance(230.0f);
		//	float ListenerX = 0.0f;  // Coordenada X del oyente
		//	float ListenerY = 0.0f;  // Coordenada Y del oyente
		//	float ListenerZ = 0.0f;  // Coordenada Z del oyente
		//}



	}


	//*/

	// Espera




	std::vector<std::string> skyboxFacesDia;
	skyboxFacesDia.push_back("Textures/Skybox/starWars_rt.tga"); //Right
	skyboxFacesDia.push_back("Textures/Skybox/starWars_lf.tga"); //Left
	skyboxFacesDia.push_back("Textures/Skybox/starWars_dn.tga"); //Down
	skyboxFacesDia.push_back("Textures/Skybox/starWars_up.tga"); //Up
	skyboxFacesDia.push_back("Textures/Skybox/starWars_bk.tga"); //Back
	skyboxFacesDia.push_back("Textures/Skybox/starWars_ft.tga"); //Front

	std::vector<std::string> skyboxFacesNoche;
	skyboxFacesNoche.push_back("Textures/Skybox/starWarsNight_rt.tga");
	skyboxFacesNoche.push_back("Textures/Skybox/starWarsNight_lf.tga");
	skyboxFacesNoche.push_back("Textures/Skybox/starWarsNight_dn.tga");
	skyboxFacesNoche.push_back("Textures/Skybox/starWarsNight_up.tga");
	skyboxFacesNoche.push_back("Textures/Skybox/starWarsNight_bk.tga");
	skyboxFacesNoche.push_back("Textures/Skybox/starWarsNight_ft.tga");


	skybox = Skybox(skyboxFacesDia);


	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);



	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);
	//contador de luces puntuales
	unsigned int pointLightCount = 0;


	// =============	Declaración de la primera luz puntual	=======================


	pointLights[0] = PointLight(
		1.0f, 1.0f, 1.0f,   // RGB
		40.0f, 80.0f,       // aIntensity - dIntensity
		0.0f, 60.0f, 0.0f,   // La posición central del tablero
		1.0f / 100.0f, 1.0f / 200.0f, 1.0f / 550.0f // Coeficientes de atenuación (Con, Lineal y Exponencial)
	);
	pointLightCount++;

	pointLights[1] = PointLight(
		1.0f, 0.0f, 0.0f,   // RGB
		20.0f, 50.0f,       // aIntensity - dIntensity
		0.0f, 55.0f, 420.0f,   // La posición central del tablero
		1.0f / 100.0f, 1.0f / 100.0f, 1.0f / 100.0f // Coeficientes de atenuación (Con, Lineal y Exponencial)
	);
	pointLightCount++;

	pointLights[2] = PointLight(
		0.0f, 0.0f, 1.0f,   // RGB
		20.0f, 50.0f,       // aIntensity - dIntensity
		0.0f, 220.0f, -335.0f,   // La posición central del tablero
		1.0f / 100.0f, 1.0f / 100.0f, 1.0f / 50.0f // Coeficientes de atenuación (Con, Lineal y Exponencial)
	);
	pointLightCount++;



	unsigned int spotLightCount = 0;
	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;



	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0, uniformTextureOffset = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 2500.0f); //Campo de Visión



	movMoneda = 0.0f;
	movMonedaOffset = 0.55f;
	rotMoneda = 0.0f;
	rotMonedaOffset = 5.0f;
	avanzaMoneda = true;

	movCanicaX = 0.0f;
	movCanicaY = 0.0f;
	movCanicaZ = 0.0f;
	movPalanca = 0.0f;
	movCanicaOffset = 3.75f;
	rotCanica = 0.0f;
	rotCanicaOffset = 5.0f;
	avanzaCanicaX = false;
	avanzaCanicaY = false;
	avanzaCanicaZ = false;
	movPalancaOffset = 0.15f;
	lanzamientoCanica = false;



	//Valores Animación placeholder
	CanicaAnim = false;
	Canica1Edo = 0; //La canica empieza en el estado 0
	CanX = 0.0f;
	velCanX = 3.0f;
	CanZ = 0.0f;
	velCanZ = 3.0f;

	Canica1Anim = false;

	//Variables de PUNTUACION DEL PINBALL QUE SE VEN EN EL OBJETO
	decU, decV, numU, numV = 0.0f;
	num = 0;
	num2 = 0;


	//Valores Canica 2
	CanicaAnim2 = false;
	Canica2Edo = 0;
	Can2X = 0.0f;
	Can2Z = 0.0f;
	rebote = 5;

	Canica2Anim = false;

	//Velocidad de animacion simple
	newVel = 2.0f;

	//Valores control fin animación canica
	finAnim1 = false;
	finAnim2 = false;
	finAnim3 = false;

	//Valores animación objeto jerárquico
	giroChopper = 0.0f;
	saltoChopper = 0.0f;
	giroChopper2 = 0.0f;
	velGiroChopper = 25.0f;
	saltoOffset = 0.7f;
	chopper1 = false;
	chopper2 = false;

	//Valores Keyframes
	glm::vec3 posblackhawk = glm::vec3(2.0f, 0.0f, 0.0f);
	CanicaAnim3 = false;
	Canica3Anim = false;

	//---------PARA TENER KEYFRAMES GUARDADOS NO VOLATILES QUE SIEMPRE SE UTILIZARAN SE DECLARAN AQUÍ
	KeyFrame[0].movAvion_x = 0.0f;
	KeyFrame[0].movAvion_y = 0.0f;


	KeyFrame[1].movAvion_x = 0.0f;
	KeyFrame[1].movAvion_y = 505.0f;


	KeyFrame[2].movAvion_x = 0.0f;
	KeyFrame[2].movAvion_y = 1010.0f;

	KeyFrame[3].movAvion_x = -100.0f;
	KeyFrame[3].movAvion_y = 1100.0f;

	KeyFrame[4].movAvion_x = -180.0f;
	KeyFrame[4].movAvion_y = 900.0f;

	KeyFrame[5].movAvion_x = -220.0f;
	KeyFrame[5].movAvion_y = 780.0f;

	KeyFrame[6].movAvion_x = -315.0f;
	KeyFrame[6].movAvion_y = 538.0f;

	KeyFrame[7].movAvion_x = -200.0f;
	KeyFrame[7].movAvion_y = 540.0f;

	KeyFrame[8].movAvion_x = -60.0f;
	KeyFrame[8].movAvion_y = 600.0f;

	KeyFrame[9].movAvion_x = -100.0f;
	KeyFrame[9].movAvion_y = 540.0f;

	KeyFrame[10].movAvion_x = -420.0f;
	KeyFrame[10].movAvion_y = 610.0f;

	KeyFrame[11].movAvion_x = -200.0f;
	KeyFrame[11].movAvion_y = 790.0f;

	KeyFrame[12].movAvion_x = -200.0f;
	KeyFrame[12].movAvion_y = 670.0f;

	KeyFrame[13].movAvion_x = -120.0f;
	KeyFrame[13].movAvion_y = 530.0f;

	KeyFrame[14].movAvion_x = -420.0f;
	KeyFrame[14].movAvion_y = 180.0f;

	KeyFrame[15].movAvion_x = -200.0f;
	KeyFrame[15].movAvion_y = 100.0f;

	KeyFrame[16].movAvion_x = -60.0f;
	KeyFrame[16].movAvion_y = 180.0f;

	KeyFrame[17].movAvion_x = -60.0f;
	KeyFrame[17].movAvion_y = -30.0f;
	//---------FIN DE KEYFRAMES GUARDADOS NO VOLÁTILES



	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		// =========== SELECCIÓN DE CÁMARA ============
		if (mainWindow.getCamaraVis() == 1) {

			camera = &dynamicCamera;
			camera->keyControl(mainWindow.getsKeys(), deltaTime);
			camera->mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
		}
		else if (mainWindow.getCamaraVis() == 2) {
			camera = &staticCamera; // Cambiar a la cámara estática
		}
		else if (mainWindow.getCamaraVis() == 3) {
			camera = &avatarCamera; // Cambiar a la cámara del avatar
		}


		glm::mat4 viewMatrix = camera->calculateViewMatrix();
		// Usar viewMatrix para tus transformaciones y enviarla a los shaders

		//	Posición de la camara del Avatar
		glm::vec3 poscam = avatarCamera.getCameraPosition();
		glm::vec3 dircam = avatarCamera.getCameraDirection();
		float angulo_cam = atan(dircam.z / dircam.x);
		angulo_cam += (90 * toRadians);
		if (dircam.x > 0) {
			angulo_cam += (180 * toRadians);
		}
		glm::vec3 posAvatar = glm::vec3(-80.0f, 75.0f, 300.0f);


		posAvatar.x = poscam.x + 1;
		posAvatar.z = poscam.z;
		posAvatar.y = poscam.y - 20;



		if (mainWindow.getMonedaPinball()) {
			if (avanzaMoneda)
			{
				if (movMoneda < 160.0f)
				{
					movMoneda += movMonedaOffset * deltaTime;
					//printf("avanza%f \n ", movMoneda);
					rotMoneda += rotMonedaOffset * deltaTime;
				}
				else
				{
					avanzaMoneda = false;
					avanzaCanicaZ = true;

				}
			}
		}

		if (avanzaCanicaZ == true) {
			//printf("avanza la canica PA ARRIBA%f \n ", movMoneda);
			if (movCanicaZ < 35.0f) {
				movCanicaZ += movCanicaOffset * deltaTime;
				//printf("avanza%f \n ", movMoneda);
				rotCanica += rotCanicaOffset * deltaTime;
			}
			else
			{
				avanzaCanicaZ = false;
				avanzaCanicaX = true;
			}
		}

		if (avanzaCanicaX == true) {
			//printf("avanza la canica PA LA DERECHA%f \n ", movMoneda);
			if (movCanicaX < 205.0f) {
				movCanicaX += movCanicaOffset * deltaTime;
				//printf("avanza%f \n ", movMoneda);
				rotCanica += rotCanicaOffset * deltaTime;
			}
			else
			{
				avanzaCanicaX = false;
				palancaLista = true;

				//Activamos Canica 1
				if (!finAnim1)
					CanicaAnim = true;
				else if (!finAnim2 && finAnim1) //Activamos Canica 2
					CanicaAnim2 = true;
				else if (!finAnim3 && finAnim2)
					CanicaAnim3 = true;

				//Se reinician los valores de la canica 0
				movCanicaX = 0.0f;
				movCanicaZ = 0.0f;
			}
		}

		if (palancaLista == true) {
			if (mainWindow.getActivaPalanca() == true) {
				if (movPalanca < 12.0f) {
					movPalanca += movPalancaOffset * deltaTime;
					//printf("avanza la palanca con un valor de : %f \n ", movPalanca);
				}

				else {
					//printf("La palanca alcanzo su valor máximo de estiramiento\n");
					palancaLista = false;
					lanzamientoCanica = true;
				}
			}
		}

		//En caso de que el usuario quiera no llegar al limite de la palanca
		if (palancaLista == true) {
			if (mainWindow.getActivaLanzamiento() == true) {
				movPalanca = 0.0f;
				palancaLista = false;
			}
		}


		if (lanzamientoCanica == true) {
			movPalanca = 0.0f;
			//printf("entraste a lanzamiento : %f \n ", movPalanca);
			//Lanzamos Canica 1
			if (!finAnim1) {
				Canica1Anim = true;
				lanzamientoCanica = false;
			}
			else if (!finAnim2 && finAnim1) {
				Canica2Anim = true;
				lanzamientoCanica = false;
			}
			else if (!finAnim3 && finAnim2) {
				Canica3Anim = true;
			}
		}



		//Aniumación Normal Canica
		if (CanicaAnim) { //Si la canica está en posición
			if (Canica1Anim) { //Activación de 

				//Estado 0 de Canica
				if (Canica1Edo == 0) {
					if (CanZ < 1010.0f) {
						CanZ += 18.0f * deltaTime;
					}
					else {
						Canica1Edo = 1;
					}
				}
				//Estado 1 de Canica
				if (Canica1Edo == 1) {
					if (CanX > -205.0f) {
						CanZ += 2.5f * deltaTime * newVel;
						CanX -= velCanX * deltaTime * newVel;
					}
					else {
						Canica1Edo = 2;
					}
				}
				//Estado 2 Canica
				if (Canica1Edo == 2) { //Choca con nave
					if (CanZ > 900.0f) {
						CanZ -= velCanZ * deltaTime * newVel;
						CanX -= 1.0f * deltaTime * newVel;
					}
					else {
						Canica1Edo = 3;
					}
				}
				//Estado 3 Canica
				if (Canica1Edo == 3) {
					if (CanX > -420.0f) {
						CanX -= velCanX * deltaTime * newVel;
						CanZ -= 0.8f * deltaTime * newVel;
					}
					else {
						Canica1Edo = 4;
					}
				}
				//Estado 4 Canica
				if (Canica1Edo == 4) {
					if (CanZ > 545.0f) {
						CanZ -= velCanZ * deltaTime * newVel;
						CanX += 2.0f * deltaTime * newVel;
					}
					else {
						Canica1Edo = 5;
					}
				}
				//Estado 5 Canica
				if (Canica1Edo == 5) {
					if (CanX < -58.0f) {
						CanX += velCanX * deltaTime * newVel;
						CanZ += 1.3f * deltaTime * newVel;
					}
					else {
						Canica1Edo = 6;
					}
				}
				//Estado 6 canica
				if (Canica1Edo == 6) {
					if (CanZ > 240.0f) {
						CanZ -= velCanZ * deltaTime * newVel;
						CanX -= 1.0f * deltaTime * newVel;
					}
					else {
						Canica1Edo = 7;
					}
				}
				//Estado 7 canica
				if (Canica1Edo == 7) {
					if (CanX > -420.0f) {
						CanZ += 0.9 * deltaTime * newVel;
						CanX -= velCanX * deltaTime * newVel;
					}
					else {
						Canica1Edo = 8;
					}
				}
				//Estado 8 canica
				if (Canica1Edo == 8) {
					if (CanZ > 150.0f) {
						CanX += 0.1f * deltaTime * newVel;
						CanZ -= 3.0 * deltaTime * newVel;
					}
					else {
						Canica1Edo = 9;
					}
				}
				//Estado canica 9
				if (Canica1Edo == 9) {
					if (CanZ > -40.0f) {
						CanZ -= 3.0 * deltaTime * newVel;
					}
					else {
						CanicaAnim = false;
						finAnim1 = true;
						avanzaCanicaZ = true;
					}
				}
			}
		}


		//Animación Normal Canica 2
		if (CanicaAnim2) {
			if (Canica2Anim) {
				//Canica 2 edo 0
				if (Canica2Edo == 0) {
					if (Can2Z < 1000.0f) {
						Can2Z += 16.0f * deltaTime;
					}
					else {
						Canica2Edo = 1;
					}
				}
				//canica2 edo1
				if (Canica2Edo == 1) {
					if (Can2Z < 1100.0f) {
						Can2X -= 2.0 * deltaTime * newVel;
						Can2Z += 3.0 * deltaTime * newVel;
					}
					else {
						Canica2Edo = 2;
					}
				}
				//canica2 edo2
				if (Canica2Edo == 2) {
					if (Can2Z < 1160.0f) {
						Can2X -= 3.0f * deltaTime * newVel;
						Can2Z += 1.0f * deltaTime * newVel;
					}
					else {
						Canica2Edo = 3;
					}
				}
				//canica2 edo3
				if (Canica2Edo == 3) {
					if (Can2X > -420.0f) {
						Can2X -= 3.0f * deltaTime * newVel;
						Can2Z -= 3.0f * deltaTime * newVel;
					}
					else {
						Canica2Edo = 4;
					}
				}
				//canica 2 edo 4
				if (Canica2Edo == 4) {
					if (Can2Z > 830.0f) {
						Can2Z -= 3.0f * deltaTime * newVel;
						Can2X += 0.4f * deltaTime * newVel;
					}
					else {
						Canica2Edo = 5;
					}
				}
				//canica2 edo5
				if (Canica2Edo == 5) {
					if (Can2Z < 900.0f) {
						Can2Z += 3.0f * deltaTime * newVel;
						Can2X += 2.0f * deltaTime * newVel;
					}
					else {
						Canica2Edo = 6;
					}
				}
				//canica2 edo6
				if (Canica2Edo == 6) {
					if (Can2Z < 980.0f) {
						Can2Z += 2.0f * deltaTime * newVel;
						Can2X += 3.0f * deltaTime * newVel;
					}
					else {
						Canica2Edo = 7;
						chopper2 = true;
					}
				}
				//canica2 edo7
				if (Canica2Edo == 7) {
					if (Can2Z > 900.0f) {
						Can2Z -= 3.0f * deltaTime * newVel;
						Can2X += 2.0f * deltaTime * newVel;
					}
					else {
						Canica2Edo = 8;
						chopper2 = false;
					}
				}
				//canica2 edo8
				if (Canica2Edo == 8) {
					if (Can2Z > 515.0f) {
						Can2Z -= 3.0f * deltaTime * newVel;
						Can2X += 0.38 * deltaTime * newVel;
					}
					else {
						Canica2Edo = 9;
					}
				}
				//canica2 edo9
				if (Canica2Edo == 9) {
					if (Can2Z < 620.0f) {
						Can2Z += 3.0f * deltaTime * newVel;
						Can2X -= 2.0f * deltaTime * newVel;
					}
					else {
						Canica2Edo = 10;
					}
				}
				//canica2 edo10
				if (Canica2Edo == 10) {
					if (Can2Z > 509.0f) {
						Can2Z -= 3.0f * deltaTime * newVel;
						Can2X -= 2.3f * deltaTime * newVel;
					}
					else {
						Canica2Edo = 11;
					}
				}
				//canica2 edo11
				if (Canica2Edo == 11) {
					if (Can2Z > 110.0f) {
						Can2Z -= 3.0f * deltaTime * newVel;
						Can2X += 0.25f * deltaTime * newVel;
					}
					else {
						Canica2Edo = 12;
					}
				}
				//canica2 edo12
				if (Canica2Edo == 12) {
					if (Can2X > -420.0f) {
						Can2X -= 4.0f * deltaTime * newVel;
						Can2Z += 2.6f * deltaTime * newVel;
					}
					else {
						Canica2Edo = 13;
					}
				}
				//canica2 edo13
				if (Canica2Edo == 13) {
					if (Can2Z < 350.0f) {
						Can2X += 3.8f * deltaTime * newVel;
						Can2Z += 2.8f * deltaTime * newVel;
					}
					else {
						Canica2Edo = 14;
					}
				}
				//can2 edo14
				if (Canica2Edo == 14) {
					if (Can2X > -420.0f) {
						Can2Z += 3.0f * deltaTime * newVel;
						Can2X -= 2.5 * deltaTime * newVel;
					}
					else {
						Canica2Edo = 15;
					}
				}
				//can2 edo15
				if (Canica2Edo == 15) {
					if (Can2X < -100.0f) {
						Can2X += 2.5 * deltaTime * newVel;
						Can2Z += 3.0f * deltaTime * newVel;
					}
					else {
						Canica2Edo = 16;
					}
				}
				//can2 edo16
				if (Canica2Edo == 16) {
					if (Can2Z > 880.0f) {
						Can2Z -= 3.0f * deltaTime * newVel;
					}
					else {
						Canica2Edo = 17;
					}
				}
				//canica2 edo17
				if (Canica2Edo == 17) {
					if (Can2Z < 940.0f) {
						Can2Z += 3.0f * deltaTime * newVel;
					}
					else {
						if (rebote == 0) {
							Canica2Edo = 18;
						}
						else {
							Canica2Edo = 16;
							rebote--;
						}
					}
				}
				//canica2 edo18
				if (Canica2Edo == 18) {
					if (Can2Z > 890.0f) {
						Can2Z -= 0.8 * deltaTime * newVel;
						Can2X -= 3.0f * deltaTime * newVel;
					}
					else {
						Canica2Edo = 19;
						chopper1 = true;
					}
				}
				//can2 edo19
				if (Canica2Edo == 19) {
					if (Can2X < -60.0f) {
						Can2X += 3.0 * deltaTime * newVel;
						Can2Z -= 3.0 * deltaTime * newVel;
					}
					else {
						Canica2Edo = 20;
						chopper1 = false;
					}
				}
				//can2 edo20
				if (Canica2Edo == 20) {
					if (Can2Z > -35.0f) {
						Can2Z -= 3.0f * deltaTime * newVel;
					}
					else {
						CanicaAnim2 = false;
						finAnim2 = true;
						avanzaCanicaZ = true;
					}
				}
			}
		}

		//Animaciones OJAIVV
		if (chopper1) {
			giroChopper += velGiroChopper * deltaTime;
			saltoChopper1 += saltoOffset * deltaTime;
		}

		if (!chopper1 && saltoChopper1 > 0.0f) {
			giroChopper += velGiroChopper * deltaTime;
			saltoChopper1 -= saltoOffset * deltaTime;
		}

		if (chopper2) {
			giroChopper2 += velGiroChopper * deltaTime;
			saltoChopper += saltoOffset * deltaTime;
		}

		if (!chopper2 && saltoChopper > 0.0f) {
			giroChopper2 += velGiroChopper * deltaTime;
			saltoChopper -= saltoOffset * deltaTime;
		}
		if (playIndex == 5) {
			chopper1 = true;
		}
		if (playIndex == 6) {
			chopper1 = false;
		}
		if (playIndex == 11) {
			chopper1 = true;
		}
		if (playIndex == 12) {
			chopper1 = false;
		}

		glm::vec3 cameraPosition = glm::vec3(205.0f + CanX, 500.5f, 515.5f - CanZ); // Ajusta según sea necesario

		// Calcula el nuevo yaw y pitch si es necesario 
		GLfloat cameraYaw = -90.0f; // Por ejemplo, mirando directamente hacia el eje -Z
		GLfloat cameraPitch = 0.0f; // Mantener el horizonte nivelado



		if (mainWindow.getapagaLuzCocheT()) {
			//RESET
			movMoneda = 0.0f;
			movMonedaOffset = 0.35f;
			rotMoneda = 0.0f;
			rotMonedaOffset = 5.0f;

			movCanicaX = 0.0f;
			movCanicaOffset = 2.75f;
			rotCanica = 0.0f;
			rotCanicaOffset = 5.0f;
		}

		// ---------MANEJO DE ILUMINACIÓN PARA CICLO DE DÍA Y DE NOCHE-----------

		if (dia) {
			if (contadorDiaNoche <= 1.0f) {
				contadorDiaNoche += deltaTime * 0.002f;
				skybox = Skybox(skyboxFacesDia);
				skyboxDia = true;
			}
			else {
				dia = false;
				noche = true;
				skyboxDia = false;
				skybox = Skybox(skyboxFacesNoche);
			}
		}
		if (noche) {
			if (contadorDiaNoche >= 0.0f) {
				contadorDiaNoche -= deltaTime * 0.002f;
				skybox = Skybox(skyboxFacesNoche);
				skyboxNoche = true;
			}
			else {
				dia = true;
				noche = false;
				skyboxNoche = false;
				skybox = Skybox(skyboxFacesDia);
			}
		}
		mainLight.ChangeDiffuseAmbient(contadorDiaNoche, 0.3);


		//Recibir eventos del usuario
		glfwPollEvents();
		camera->keyControl(mainWindow.getsKeys(), deltaTime);
		camera->mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera->calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();
		uniformTextureOffset = shaderList[0].getOffsetLocation();

		//Offset para textura score
		uniformTextureOffset = shaderList[0].getOffsetLocation();


		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera->calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera->getCameraPosition().x, camera->getCameraPosition().y, camera->getCameraPosition().z);

		// luz ligada a la cámara de tipo flash
		//sirve para que en tiempo de ejecución (dentro del while) se cambien propiedades de la luz
		glm::vec3 lowerLight = camera->getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera->getCameraDirection());

		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);


		//-------Para Keyframes
		if (CanicaAnim3)
			inputKeyframes(mainWindow.getsKeys());
		animate();


		// -----------= Definición de Matrices y Matrices de Apoyo =-------------
		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::mat4 modelauxCuerpoPinball(1.0);
		glm::mat4 modelauxFlipper(1.0);
		glm::mat4 modelauxKirbyC(1.0);
		glm::mat4 modelauxCuerpoChopper(1.0);

		glm::vec2 toffset = glm::vec2(0.0f, 0.0f);


		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);



		// ---------OBJETOS DE BILLY-----------
				//Billy
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-70.0f, 30.0f, 500.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		model = glm::rotate(model, 15 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Billy.RenderModel();

		//Grim (Puro Hueso pa los compas)
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(60.0f, 64.0f, 360.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		model = glm::rotate(model, 15 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Grim.RenderModel();

		//Mandy
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-60.0f, 38.0f, 470.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		model = glm::rotate(model, 15 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Mandy.RenderModel();

		//Irwing
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-45.0f, 30.0f, 500.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		model = glm::rotate(model, 15 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Irwing.RenderModel();

		//Prisionero 1
		model = modelauxCuerpoPinball;
		model = glm::translate(model, glm::vec3(-80.0f, 64.0f, 360.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.15f, 0.2f));
		model = glm::rotate(model, 15 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Prisionero.RenderModel();

		//Prisionero 2
		model = modelauxCuerpoPinball;
		model = glm::translate(model, glm::vec3(30.0f, 64.0f, 360.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.15f, 0.2f));
		model = glm::rotate(model, 15 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Prisionero.RenderModel();

		//Ojo1
		model = modelauxCuerpoPinball;
		model = glm::translate(model, glm::vec3(-80.0f, 75.0f, 300.0f));
		model = glm::scale(model, glm::vec3(90.0f, 90.0f, 90.0f));
		model = glm::rotate(model, 15 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Ojo.RenderModel();

		//Ojo2
		model = modelauxCuerpoPinball;
		model = glm::translate(model, glm::vec3(30.0f, 75.0f, 300.0f));
		model = glm::scale(model, glm::vec3(90.0f, 90.0f, 90.0f));
		model = glm::rotate(model, 15 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Ojo.RenderModel();

		//Mano
		model = modelauxCuerpoPinball;
		model = glm::translate(model, glm::vec3(-90.0f, 50.0f, 450.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		model = glm::rotate(model, 100 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Mano.RenderModel();

		//Roca
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(100.0f, 5.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Roca.RenderModel();


		// ---------OBJETOS DE STAR WARS-----------
		//Cañonera 1
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(124.0f, 274.8f, -420.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, 15 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.85f, 0.85f, 0.85f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		CañoneraLAAT.RenderModel();

		//Cañonera 2
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(124.0f, 252.5f, -335.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, 15 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.85f, 0.85f, 0.85f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		CañoneraLAAT.RenderModel();

		//Tanque
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-100.0f, 255.8f, -350.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, -15 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		FighterTank.RenderModel();


		//Comandante FORDO
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(124.0f, 249.2f, -335.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, 15 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(4.2f, 4.2f, 4.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ComandanteFordo.RenderModel();

		//Chopper 1
		//Cuerpo
		model = modelauxCuerpoPinball;
		model = glm::translate(model, glm::vec3(0.0f, 230.0f + saltoChopper1, -265.0f));
		model = glm::scale(model, glm::vec3(12.5f, 12.5f, 12.5f));
		model = glm::rotate(model, 15 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		modelauxCuerpoChopper = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Cuerpo.RenderModel();
		//Cabeza
		model = modelauxCuerpoChopper;
		model = glm::translate(model, glm::vec3(0.0f, 1.3f, 0.0f));
		model = glm::rotate(model, giroChopper * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Cabeza.RenderModel();
		//Brazo Izquierdo
		model = modelauxCuerpoChopper;
		model = glm::translate(model, glm::vec3(0.45f, 1.5f, 0.0f));
		//model = glm::rotate(model, -15 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Brazo_Izq.RenderModel();
		//Brazo Derecho
		model = modelauxCuerpoChopper;
		model = glm::translate(model, glm::vec3(-0.45f, 1.5f, 0.0f));
		//model = glm::rotate(model, -15 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Brazo_Der.RenderModel();

		//Chopper 2
		//Cuerpo
		model = modelauxCuerpoPinball;
		model = glm::translate(model, glm::vec3(-35.0f, 274.0f + saltoChopper, -438.0f));
		model = glm::scale(model, glm::vec3(12.5f, 12.5f, 12.5f));
		model = glm::rotate(model, 15 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		modelauxCuerpoChopper = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Cuerpo.RenderModel();
		//Cabeza
		model = modelauxCuerpoChopper;
		model = glm::translate(model, glm::vec3(0.0f, 1.3f, 0.0f));
		model = glm::rotate(model, giroChopper2 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Cabeza.RenderModel();
		//Brazo Izquierdo
		model = modelauxCuerpoChopper;
		model = glm::translate(model, glm::vec3(0.45f, 1.5f, 0.0f));
		//model = glm::rotate(model, -15 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Brazo_Izq.RenderModel();
		//Brazo Derecho
		model = modelauxCuerpoChopper;
		model = glm::translate(model, glm::vec3(-0.45f, 1.5f, 0.0f));
		//model = glm::rotate(model, -15 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Brazo_Der.RenderModel();

		// --------- OBJETOS DE KIRBY -----------

		//Kirby
		model = glm::mat4(1.0);
		model = glm::translate(model, posAvatar);
		modelauxKirbyC = model;
		model = glm::scale(model, glm::vec3(65.0f, 65.0f, 65.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Kirby.RenderModel();

		//Estrella
		model = modelauxKirbyC;
		model = glm::translate(model, glm::vec3(0.0f, 33.25f, 0.0f));
		model = glm::scale(model, glm::vec3(35.0f, 35.0f, 35.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		estrellaTexture.UseTexture();
		meshList[0]->RenderMesh();

		//Whispy Woods
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 154.0f, 5.0f));
		model = glm::rotate(model, 0 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, 15 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.6f, 2.6f, 2.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Whispy.RenderModel();

		//Manzana 1
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 185.0f, -120.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, -15 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.6f, 2.6f, 2.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Apple.RenderModel();

		//Manzana 2
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.0f, 115.0f, 170.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, -15 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Apple.RenderModel();

		//Manzana 3
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(100.0f, 155.0f, 0.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, -15 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Apple.RenderModel();

		//Manzana 4
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-100.0f, 155.0f, -10.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, -15 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Apple.RenderModel();


		// --------- OBJETOS DE PINBALL -----------

		//Para apagar la primera PointLight [Verde]
		if (mainWindow.getapagaLuzLinternaVerde())
		{
			shaderList[0].SetPointLights(&pointLights[0], 1);
		}

		//Para apagar la segunda PointLight [Roja]
		if (mainWindow.getapagaLuzLinternaRoja())
		{
			shaderList[0].SetPointLights(&pointLights[1], 1);
		}

		//Para apagar la segunda PointLight [Azul]
		if (mainWindow.getapagaLuzLinternaAzul())
		{
			shaderList[0].SetPointLights(&pointLights[2], 1);
		}

		// Para apagar la linterna 
		if (mainWindow.getapagaLuz())
		{
			shaderList[0].SetSpotLights(&spotLights[1], 1);
		}


		///*
		//Tablero de Pinball
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 25.0f, 0.0f));
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -movMoneda));
		modelaux = model;
		modelauxCuerpoPinball = model;
		model = glm::scale(model, glm::vec3(100.0f, 100.0f, 100.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pinball.RenderModel();
		//*/

		//Base de Palanca de Pinball
		model = modelaux;
		model = glm::translate(model, glm::vec3(204.0f, 8.0f, 609.5f));
		modelaux = model;
		model = glm::rotate(model, -80 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, -10 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Base_Palanca.RenderModel();

		//Palanca de Pinball
		model = modelaux;
		model = glm::translate(model, glm::vec3(-1.5f, 1.35f, -13.5f));
		modelaux = model;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, movPalanca));
		model = glm::rotate(model, -80 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, -10 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Palanca.RenderModel();

		//Pantalla de SCORE
		//model = modelaux;
		//model = glm::translate(model, glm::vec3(0.0f, 470.0f, -700.0f));
		//model = glm::scale(model, glm::vec3(10.6f, 10.0f, 10.0f));
		//model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		//glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera->calculateViewMatrix()));
		//Pantalla.RenderModel();


		//Resorte
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 9.2f, -25.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.5f, 5.2f - (movPalanca / 3), 3.5f));
		//model = glm::scale(model, glm::vec3(1.0f, movPalanca, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Resorte.RenderModel();




		//Monedita
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-107.0f, 10.0f, 700.0f));
		model = glm::translate(model, glm::vec3(0.0f, 14.3f, -movMoneda));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, -rotMoneda * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(8.5f, 8.5f, 8.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Coin.RenderModel();


		//Canica del Pinball
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 42.5f, 545.5f));
		model = glm::translate(model, glm::vec3(movCanicaX, 0.0f, -movCanicaZ));
		model = glm::rotate(model, -rotCanica * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, -rotCanica * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.5f, 3.5f, 3.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		if (!CanicaAnim)
			Canica.RenderModel();

		//Canica placeholder para animación
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(205.0f, 42.5f, 510.5));
		model = glm::rotate(model, 15 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f)); //Rotación del modelo para match plano
		model = glm::translate(model, glm::vec3(CanX, 0.0f, -CanZ));
		//model = glm::rotate(model, -rotCanica * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::rotate(model, -rotCanica * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.5f, 3.5f, 3.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		if (CanicaAnim)
			Canica.RenderModel();


		//Canica placeholder para animación 2
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(205.0f, 42.5f, 510.5f));
		model = glm::rotate(model, 15 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f)); //Rotación del modelo para match plano
		model = glm::translate(model, glm::vec3(Can2X, 0.0f, -Can2Z));
		model = glm::scale(model, glm::vec3(3.5f, 3.5f, 3.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		if (CanicaAnim2)
			Canica.RenderModel();

		//Canica placeholder para keyframes
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(205.0f, 42.5, 510.5f));
		model = glm::rotate(model, 15 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f)); //Rotación del modelo para match plano
		posblackhawk = glm::vec3(posXavion + movAvion_x, posYavion, posZavion - movAvion_y);
		model = glm::translate(model, posblackhawk);
		model = glm::scale(model, glm::vec3(3.5f, 3.5f, 3.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		if (CanicaAnim3)
			Canica.RenderModel();


		//------------ FLIPPER IZQUIERDO STAR WARS ----------------
		model = modelauxCuerpoPinball;
		model = glm::translate(model, glm::vec3(-230.0f, 215.0f, -280.0f));
		model = glm::rotate(model, 15 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(8.5f, 8.5f, 8.5f));
		model = glm::rotate(model, glm::radians(mainWindow.getFlipperISW()), glm::vec3(-1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		FlipperSable.RenderModel();

		//------------ FLIPPER IZQUIERDO ----------------
		model = modelauxCuerpoPinball;
		model = glm::translate(model, glm::vec3(-90.0f, 19.0f, 427.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		model = glm::rotate(model, 15 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getFlipperI()), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		FlipperI.RenderModel();

		//------------ FLIPPER DERECHO ----------------
		model = modelauxCuerpoPinball;
		model = glm::translate(model, glm::vec3(30.0f, 20.0f, 423.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		model = glm::rotate(model, 15 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getFlipperD()), glm::vec3(0.0f, -1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		FlipperD.RenderModel();

		//PIRÁMIDES  TEMARICAS DEL INFIERNO

		// --Sección Izquierda de Pirámides
		//Pirámide Central

		model = modelauxCuerpoPinball;
		model = glm::translate(model, glm::vec3(-170.0f, 62.8f, 427.0f));
		model = glm::scale(model, glm::vec3(40.0f, 40.0f, 40.0f));
		model = glm::rotate(model, 15 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 60 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera->calculateViewMatrix()));
		rocosaTexture.UseTexture();
		meshList[4]->RenderMesh();

		//Piramide Flipper

		model = modelauxCuerpoPinball;
		model = glm::translate(model, glm::vec3(-125.0f, 44.9f, 450.0f));
		modelauxFlipper = model;
		model = glm::scale(model, glm::vec3(30.0f, 30.0f, 30.0f));
		model = glm::rotate(model, 15 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera->calculateViewMatrix()));
		rocosaTexture.UseTexture();
		meshList[4]->RenderMesh();



		//Extremo izq

		model = modelauxCuerpoPinball;
		model = glm::translate(model, glm::vec3(-183.0f, 62.0f, 375.0f));
		model = glm::scale(model, glm::vec3(25.0f, 25.0f, 25.0f));
		model = glm::rotate(model, 15 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 40 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera->calculateViewMatrix()));
		rocosaTexture.UseTexture();
		meshList[4]->RenderMesh();

		// --Sección Derecha de Pirámides
		//Pirámide Central

		model = modelauxCuerpoPinball;
		model = glm::translate(model, glm::vec3(122.0f, 62.8f, 427.0f));
		model = glm::scale(model, glm::vec3(40.0f, 40.0f, 40.0f));
		model = glm::rotate(model, 15 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 60 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera->calculateViewMatrix()));
		rocosaTexture.UseTexture();
		meshList[4]->RenderMesh();

		//Piramide Flipper
		modelaux = model;
		model = modelauxCuerpoPinball;
		model = glm::translate(model, glm::vec3(65.0f, 44.9f, 450.0f));
		model = glm::scale(model, glm::vec3(30.0f, 30.0f, 30.0f));
		model = glm::rotate(model, 15 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera->calculateViewMatrix()));
		rocosaTexture.UseTexture();
		meshList[4]->RenderMesh();

		//Extremo der
		modelaux = model;
		model = modelauxCuerpoPinball;
		model = glm::translate(model, glm::vec3(130.0f, 62.0f, 375.0f));
		model = glm::scale(model, glm::vec3(25.0f, 25.0f, 25.0f));
		model = glm::rotate(model, 15 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 80 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera->calculateViewMatrix()));
		rocosaTexture.UseTexture();
		meshList[4]->RenderMesh();

		//Aminación unidad
		if (num == 1) {
			numU = 0.0f;
			numV = 0.0f;
		}
		if (num == 2) {
			numU = 0.25f;
			numV = 0.0f;
		}
		if (num == 3) {
			numU = 0.5f;
			numV = 0.0f;
		}
		if (num == 4) {
			numU = 0.75f;
			numV = 0.0f;
		}
		if (num == 5) {
			numU = 0.0f;
			numV = -0.33f;
		}
		if (num == 6) {
			numU = 0.25f;
			numV = -0.33f;
		}
		if (num == 7) {
			numU = 0.5f;
			numV = -0.33f;
		}
		if (num == 8) {
			numU = 0.75f;
			numV = -0.33f;
		}
		if (num == 9) {
			numU = 0.0f;
			numV = -0.67f;
		}
		if (num == 0) {
			numU = 0.25f;
			decV = -0.67f;
		}

		//Aminación decena
		if (num2 == 1) {
			decU = 0.0f;
			decV = 0.0f;
		}
		if (num2 == 2) {
			decU = 0.25f;
			decV = 0.0f;
		}
		if (num2 == 3) {
			decU = 0.5f;
			decV = 0.0f;
		}
		if (num2 == 4) {
			decU = 0.75f;
			decV = 0.0f;
		}
		if (num2 == 5) {
			decU = 0.0f;
			decV = -0.33f;
		}
		if (num2 == 6) {
			decU = 0.25f;
			decV = -0.33f;
		}
		if (num2 == 7) {
			decU = 0.5f;
			decV = -0.33f;
		}
		if (num2 == 8) {
			decU = 0.75f;
			decV = -0.33f;
		}
		if (num2 == 9) {
			decU = 0.0f;
			decV = -0.67f;
		}
		if (num2 == 0) {
			decU = 0.25f;
			decV = -0.67f;
		}




		//Score
		//Unidades
		//toffset = glm::vec2(numU, numV);
		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(120.0f, 545.0f, -669.0f));
		//model = glm::scale(model, glm::vec3(99.0f, 99.0f, 99.0f));
		//model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		//glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//NumerosTexture.UseTexture();
		//Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		//meshList[2]->RenderMesh();

		////Decenas
		//toffset = glm::vec2(decU, decV);
		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(0.0f, 545.0f, -669.0f));
		//model = glm::scale(model, glm::vec3(99.0f, 99.0f, 99.0f));
		//model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		//glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//NumerosTexture.UseTexture();
		//Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		//meshList[2]->RenderMesh();



		//------------------- CRISTAL TRANSPARENTE -------------------
		modelaux = model;
		model = modelauxCuerpoPinball;
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, -2.0f));
		model = glm::scale(model, glm::vec3(100.0f, 100.0f, 100.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera->calculateViewMatrix()));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		Tapa.RenderModel();
		glDisable(GL_BLEND);


		shaderList[0].SetDirectionalLight(&mainLight);
		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}


void inputKeyframes(bool* keys)
{
	if (Canica3Anim)
	{
		//printf("Cero\n");
		if (reproduciranimacion < 1)
		{
			//printf("dentro de reproducción\n");
			if (play == false && (FrameIndex > 1))
			{
				//printf("dentro de play\n");
				resetElements();
				//First Interpolation				
				interpolation();
				play = true;
				playIndex = 0;
				i_curr_steps = 0;
				reproduciranimacion++;
				//printf("\n presiona 0 para habilitar reproducir de nuevo la animación'\n");
				habilitaranimacion = 0;

			}
			else
			{
				play = false;
				CanicaAnim3 = false;
				finAnim3 = true;
			}
		}
		else {

		}
	}
}
