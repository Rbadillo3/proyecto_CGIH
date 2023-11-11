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
bool dia = false;

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

//Animacion Canica1
bool CanicaAnim;
int Canica1Edo;
float CanX;
float velCanX;
float CanZ;
float velCanZ;

//Animacion Canica2
bool CanicaAnim2;
int Canica2Edo;
float Can2X;
float Can2Z;
int rebote;

float newVel;


float movOffset;
bool avanza;


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

// =========				Variables de MODELOS				===============


//====	Star Wars
Model CañoneraLAAT;
Model ComandanteFordo;
Model FighterTank;

//====	Kirby
Model Kirby;//Línea añadida
Model Apple;
Model Whispy;

//====	Billy y Mandy
Model Billy;
Model Grim;
Model Mandy;

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
float reproduciranimacion, habilitaranimacion,  reinicioFrame, ciclo, ciclo2, contador = 0.0f;

//función para teclado de keyframes 
void inputKeyframes(bool* keys);

bool animacion = false;



//NEW// Keyframes
float posXavion = 2.0, posYavion = 5.0, posZavion = -3.0;
float	movAvion_x = 0.0f, movAvion_y = 0.0f;
float giroAvion = 0;

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
			printf("playindex : %d\n", playIndex);
			if (playIndex > FrameIndex - 2)	//Fin de toda la animación con último frame?
			{
				printf("Frame index= %d\n", FrameIndex);
				printf("termino la animacion\n");
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

	CreateObjects();
	CreateShaders();
	CrearPiramideTriangular();//índice 1 en MeshList

	
	// ================ CÁMARAS ================================
	Camera dynamicCamera(
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		-60.0f,
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


	//	Se cargan todos los objetos 

	// ----------- Elementos Pinball -------------
	Pinball = Model();
	Pinball.LoadModel("Models/PINBALL_V5.obj");

	Coin = Model();
	Coin.LoadModel("Models/Coin.obj");

	Canica = Model();
	Canica.LoadModel("Models/CanicaD.obj");

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


	// ----------- Elementos Star Wars -------------
	CañoneraLAAT = Model();
	CañoneraLAAT.LoadModel("Models/rep_la_at_gunship.obj");

	FighterTank = Model();
	FighterTank.LoadModel("Models/rep_hover_fightertank.obj");

	ComandanteFordo = Model();
	ComandanteFordo.LoadModel("Models/rep_inf_arctrooper.obj");

	// ----------- Elementos Kirby -------------
	//Líneas añadidas
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

	ISoundEngine* soundEngine = createIrrKlangDevice();

	if (!soundEngine) {
		// Manejo de error si no se pudo crear el motor de sonido
		return 1;
	}

	// Ruta al archivo MP3 que deseas reproducir
	const char* mp3FilePath = "Models/Kirby_s-Pinball-Land-Title-Theme.wav";

	// Reproduce el archivo WAV
	///*

	//ISound* sound = soundEngine->play2D(mp3FilePath, true, false, true);

	//if (!sound) {
	//	// Manejo de error si no se pudo reproducir el archivo
	//	return 1;
	//}
	//*/

	// Espera




	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);


	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);
	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	//Declaración de primer luz puntual
	pointLights[0] = PointLight(1.0f, 0.0f, 0.0f,
		0.0f, 1.0f,
		-6.0f, 1.5f, 1.5f,
		0.3f, 0.2f, 0.1f);
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

	//luz fija
	spotLights[1] = SpotLight(0.0f, 1.0f, 0.0f,
		1.0f, 2.0f,
		5.0f, 10.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;

	//se crean mas luces puntuales y spotlight 

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 2500.0f); //Campo de Visión



	movMoneda = 0.0f;
	movMonedaOffset = 0.55f;
	rotMoneda = 0.0f;
	rotMonedaOffset = 5.0f;
	avanzaMoneda = true;

	//Valores canica
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
	palancaLista = false;

	//Líneas añadidas
	//Valores Animación placeholder
	CanicaAnim = true;
	Canica1Edo = 0; //La canica empieza en el estado 0
	CanX = 0.0f;
	velCanX = 3.0f;
	CanZ = 0.0f;
	velCanZ = 3.0f;
	//Valores Canica 2
	CanicaAnim2 = true;
	Canica2Edo = 0;
	Can2X = 0.0f;
	Can2Z = 0.0f;
	rebote = 5;
	//Velocidad de animacion simple
	newVel = 1.0f;

	//Valores Keyframes
	glm::vec3 posblackhawk = glm::vec3(2.0f, 0.0f, 0.0f);

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


		glm::mat4 viewMatrix = camera->calculateViewMatrix();
		// Usar viewMatrix para tus transformaciones y enviarla a los shaders



		if (mainWindow.getMonedaPinball()) {
			if (avanzaMoneda)
			{
				if (movMoneda < 105.0f)
				{
					movMoneda += movMonedaOffset * deltaTime;
					printf("avanza%f \n ", movMoneda);
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
				printf("avanza%f \n ", movMoneda);
				rotCanica += rotCanicaOffset * deltaTime;
			}
			else
			{
				avanzaCanicaZ = false;
				avanzaCanicaX = true;
			}
		}

		if (avanzaCanicaX == true) {
			printf("avanza la canica PA LA DERECHA%f \n ", movMoneda);
			if (movCanicaX < 205.0f) {
				movCanicaX += movCanicaOffset * deltaTime;
				printf("avanza%f \n ", movMoneda);
				rotCanica += rotCanicaOffset * deltaTime;
			}
			else
			{
				avanzaCanicaX = false;
				palancaLista = true;
				//CanicaAnim = true; //Línea añadida
			}
		}

		if (palancaLista == true) {
			if (mainWindow.getActivaPalanca() == true) {
				if (movPalanca < 12.0f) {
					movPalanca += movPalancaOffset * deltaTime;
					printf("avanza la palanca con un valor de : %f \n ", movPalanca);
				}
				else {
					printf("La palanca alcanzo su valor máximo de estiramiento\n");
					palancaLista = false;
				}
			}
			/*
			else if(mainWindow.getActivaPalanca() == false)
			{
				movPalanca = 0.0f;
			}
			*/

		}

		//Líneas añadidas
		//Aniumación Normal Canica
		if (CanicaAnim) { //Si la canica está en posición
			if (mainWindow.getCanica1() == true) { //Activación de Animación1
				//Estado 0 de Canica
				if (Canica1Edo == 0) {
					if (CanZ < 1010.f) {
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
						CanX -= velCanX * deltaTime *newVel;
					}
					else {
						Canica1Edo = 2;
					}
				}
				//Estado 2 Canica
				if (Canica1Edo == 2) { //Choca con nave
					if (CanZ > 900.0f) {
						CanZ -= velCanZ * deltaTime *newVel;
						CanX -= 1.0f * deltaTime *newVel;
					}
					else {
						Canica1Edo = 3;
					}
				}
				//Estado 3 Canica
				if (Canica1Edo == 3) {
					if (CanX > -420.0f) {
						CanX -= velCanX * deltaTime*newVel;
						CanZ -= 0.8f * deltaTime*newVel;
					}
					else {
						Canica1Edo = 4;
					}
				}
				//Estado 4 Canica
				if (Canica1Edo == 4) {
					if (CanZ > 545.0f) {
						CanZ -= velCanZ * deltaTime*newVel;
						CanX += 2.0f * deltaTime*newVel;
					}
					else {
						Canica1Edo = 5;
						//printf("%f\n", CanX);
					}
				}
				//Estado 5 Canica
				if (Canica1Edo == 5) {
					if (CanX < -58.0f) {
						CanX += velCanX * deltaTime*newVel;
						CanZ += 1.3f * deltaTime*newVel;
					}
					else {
						Canica1Edo = 6;
					}
				}
				//Estado 6 canica
				if (Canica1Edo == 6) {
					if (CanZ > 240.0f) {
						CanZ -= velCanZ * deltaTime*newVel;
						CanX -= 1.0f * deltaTime*newVel;
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
					if (CanZ > -30.0f) {
						CanZ -= 3.0 * deltaTime * newVel;
					}
				}
			}
		}

		//Animación Normal Canica 2
		if (CanicaAnim2) {
			if (mainWindow.getCanica2() == true) {
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
						printf("%f", Can2Z);
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
					}
				}
				//can2 edo20
				if (Canica2Edo == 20) {
					if (Can2Z > -35.0f) {
						Can2Z -= 3.0f * deltaTime * newVel;
					}
				}
			}
		}



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

		if (contadorDiaNoche <= 1.0f and dia) {
			contadorDiaNoche += deltaTime * 0.0002f;
		}
		else if (contadorDiaNoche >= 1.0f and dia) {
			dia = false;
		}
		else if (contadorDiaNoche >= 0.0f and !dia) {
			contadorDiaNoche -= deltaTime * 0.0002f;
		}
		else if (contadorDiaNoche <= 0.0f and !dia) {
			dia = true;
		}

		//printf("\nContador = %f", contadorDiaNoche);
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
		inputKeyframes(mainWindow.getsKeys());
		animate(); 

		// -----------= Definición de Matrices y Matrices de Apoyo =-------------
		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::mat4 modelauxCuerpoPinball(1.0);
		glm::mat4 modelauxFlipper(1.0);


		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();

		// ---------OBJETOS DE BILLY-----------
				//Billy
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -80.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Billy.RenderModel();

		//Grim (Puro Hueso pa los compas)
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(20.0f, 0.0f, -80.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Grim.RenderModel();

		//Mandy
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(40.0f, 0.0f, -80.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Mandy.RenderModel();

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


		// --------- OBJETOS DE KIRBY -----------
		// Líneas añadidas
		//Kirby
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(60.0f, 0.0f, -80.0f));
		model = glm::scale(model, glm::vec3(50.0f, 50.0f, 50.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Kirby.RenderModel();

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

		///*
		//Tablero de Pinball
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 25.0f, 0.0f));
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -movMoneda));
		modelaux = model;
		modelauxCuerpoPinball = model;
		model = glm::scale(model, glm::vec3(100.0f, 100.0f, 100.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
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
		Palanca.RenderModel();

		//Resorte
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 10.35f, -20.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.0f, 5.2f - (movPalanca / 3), 4.0f));
		//model = glm::scale(model, glm::vec3(1.0f, movPalanca, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Resorte.RenderModel();




		//Monedita
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-107.0f, 10.0f, 700.0f));
		model = glm::translate(model, glm::vec3(0.0f, 14.3f, -movMoneda));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, -rotMoneda * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(8.5f, 8.5f, 8.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Coin.RenderModel();

		//Canica del Pinball
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 42.5f, 545.5f));
		model = glm::translate(model, glm::vec3(movCanicaX, 0.0f, -movCanicaZ));
		model = glm::rotate(model, -rotCanica * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, -rotCanica * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.5f, 3.5f, 3.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Canica.RenderModel();

		//Líneas añadidas
		//Canica placeholder para animación
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(205.0f, 42.5f, 510.5f));
		model = glm::rotate(model, 15 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f)); //Rotación del modelo para match plano
		model = glm::translate(model, glm::vec3(CanX, 0.0f, -CanZ));
		model = glm::scale(model, glm::vec3(3.5f, 3.5f, 3.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Canica.RenderModel();

		//Canica placeholder para animación 2
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(205.0f, 42.5f, 510.5f));
		model = glm::rotate(model, 15 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f)); //Rotación del modelo para match plano
		model = glm::translate(model, glm::vec3(Can2X, 0.0f, -Can2Z));
		model = glm::scale(model, glm::vec3(3.5f, 3.5f, 3.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Canica.RenderModel();

		//Canica placeholder para keyframes
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(205.0f, 42.5f, 510.5f));
		model = glm::rotate(model, 15 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f)); //Rotación del modelo para match plano
		posblackhawk = glm::vec3(posXavion + movAvion_x, posYavion, posZavion - movAvion_y);
		model = glm::translate(model, posblackhawk);
		model = glm::scale(model, glm::vec3(3.5f, 3.5f, 3.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Canica.RenderModel();

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
		color = glm::vec3(1.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();

		//Piramide Flipper
		
		model = modelauxCuerpoPinball;
		model = glm::translate(model, glm::vec3(-125.0f, 44.9f, 450.0f));
		modelauxFlipper = model;
		model = glm::scale(model, glm::vec3(30.0f, 30.0f, 30.0f));
		model = glm::rotate(model, 15 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera->calculateViewMatrix()));
		color = glm::vec3(1.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();



		//Extremo izq
		
		model = modelauxCuerpoPinball;
		model = glm::translate(model, glm::vec3(-183.0f, 62.0f, 375.0f));
		model = glm::scale(model, glm::vec3(25.0f, 25.0f, 25.0f));
		model = glm::rotate(model, 15 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 40 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera->calculateViewMatrix()));
		color = glm::vec3(1.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();

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
		color = glm::vec3(1.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();

		//Piramide Flipper
		modelaux = model;
		model = modelauxCuerpoPinball;
		model = glm::translate(model, glm::vec3(65.0f, 44.9f, 450.0f));
		model = glm::scale(model, glm::vec3(30.0f, 30.0f, 30.0f));
		model = glm::rotate(model, 15 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera->calculateViewMatrix()));
		color = glm::vec3(1.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();

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
		color = glm::vec3(1.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();



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
	if (keys[GLFW_KEY_0]) 
	{
		printf("Cero\n");
		if (reproduciranimacion < 1)
		{
			printf("dentro de reproducción\n");
			if (play == false && (FrameIndex > 1))
			{
				printf("dentro de play\n");
				resetElements();
				//First Interpolation				
				interpolation();
				play = true;
				playIndex = 0;
				i_curr_steps = 0;
				reproduciranimacion++;
				printf("\n presiona 0 para habilitar reproducir de nuevo la animación'\n");
				habilitaranimacion = 0;

			}
			else
			{
				play = false;

			}
		}
	}
	if (keys[GLFW_KEY_9])
	{
		if (habilitaranimacion < 1 && reproduciranimacion>0)
		{
			printf("Ya puedes reproducir de nuevo la animación con la tecla de barra espaciadora'\n");
			reproduciranimacion = 0;

		}
	}
}
