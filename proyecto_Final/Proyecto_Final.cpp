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

//#include <irrKlang.h>
//using namespace irrklang;

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
Texture estrellaTexture;
Texture rocosaTexture;

// =========				Variables de MODELOS				===============


//====	Star Wars
Model CañoneraLAAT;
Model ComandanteFordo;
Model FighterTank;

//====	Kirby


//====	Billy y Mandy
Model Billy;
Model Grim;
Model Mandy;
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

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh* obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	Mesh* obj5 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj5);

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);

}


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

void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}


int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateStar();
	CreatePiramide();
	CreateObjects();
	CreateShaders();

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

	Camera avatarCamera(
		glm::vec3(0.0f, 0.0f, -80.0f),
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


	//	Se cargan todos los objetos 

	// ----------- Elementos Pinball -------------
	Pinball = Model();
	Pinball.LoadModel("Models/PINBALL_V4.obj");

	Coin = Model();
	Coin.LoadModel("Models/Coin.obj");

	Canica = Model();
	Canica.LoadModel("Models/canica.obj");

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


	// ----------- Elementos Billy y Mandy -------------
	Billy = Model();
	Billy.LoadModel("Models/Billy.obj");

	Grim = Model();
	Grim.LoadModel("Models/Grim.obj");

	Mandy = Model();
	Mandy.LoadModel("Models/Mandy.obj");

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

	/*ISoundEngine* soundEngine = createIrrKlangDevice();

	if (!soundEngine) {
		// Manejo de error si no se pudo crear el motor de sonido
		return 1;
	}

	// Ruta al archivo MP3 que deseas reproducir
	const char* mp3FilePath = "Models/Kirby_s-Pinball-Land-Title-Theme.wav";

	// Reproduce el archivo WAV
	///*

	ISound* sound = soundEngine->play2D(mp3FilePath, true, false, true);

	if (!sound) {
		// Manejo de error si no se pudo reproducir el archivo
		return 1;
	}*/

	// Espera




	std::vector<std::string> skyboxFaces;
	/*skyboxFaces.push_back("Textures/Skybox/starWarsNight_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/starWarsNight_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/starWarsNight_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/starWarsNight_up.tga");
	skyboxFaces.push_back("Textures/Skybox/starWarsNight_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/starWarsNight_ft.tga");*/

	skyboxFaces.push_back("Textures/Skybox/starWars_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/starWars_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/starWars_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/starWars_up.tga");
	skyboxFaces.push_back("Textures/Skybox/starWars_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/starWars_ft.tga");

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
			camera = &avatarCamera; // Cambiar a la cámara estática

		}

		glm::mat4 viewMatrix = camera->calculateViewMatrix();



		//posicion de la camara Billy
		glm::vec3 poscam = avatarCamera.getCameraPosition();
		glm::vec3 dircam = avatarCamera.getCameraDirection();

		float angulo_cam = atan(dircam.z / dircam.x);
		angulo_cam += (90 * toRadians);
		if (dircam.x > 0) {
			angulo_cam += (180 * toRadians);
		}
		glm::vec3 posAvatar = glm::vec3(0, 0, 0);


		posAvatar.x = poscam.x + (angulo_cam);
		posAvatar.z = poscam.z + (angulo_cam);
		posAvatar.y = poscam.y - 20;



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

	/*	if (contadorDiaNoche <= 1.0f and dia) {
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

		printf("\nContador = %f", contadorDiaNoche);
		mainLight.ChangeDiffuseAmbient(contadorDiaNoche, 0.3);
		*/

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
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();

		// ---------OBJETOS DE BILLY-----------
		//Billy
		model = glm::mat4(1.0);
		model = glm::translate(model,posAvatar);
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		model = glm::rotate(model, -angulo_cam , glm::vec3(0.0f, 1.0f, 0.0f));
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

		//Prisionero
		model =modelauxCuerpoPinball;
		model = glm::translate(model, glm::vec3(60.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
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

		//Lava
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(120.0f, 20.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Lava.RenderModel();

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

		model = modelauxCuerpoPinball;
		model = glm::translate(model, glm::vec3(0.0f,300.0f,0.0f));
		model = glm::scale(model, glm::vec3(40.0f, 40.0f, 40.0f));
		model = glm::rotate(model, 90* toRadians,glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		estrellaTexture.UseTexture();
		meshList[0]->RenderMesh();

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
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
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
		rocosaTexture.UseTexture();
		meshList[2]->RenderMesh();

		//Piramide Flipper

		model = modelauxCuerpoPinball;
		model = glm::translate(model, glm::vec3(-125.0f, 44.9f, 450.0f));
		modelauxFlipper = model;
		model = glm::scale(model, glm::vec3(30.0f, 30.0f, 30.0f));
		model = glm::rotate(model, 15 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera->calculateViewMatrix()));
		meshList[2]->RenderMesh();

		//Extremo izq

		model = modelauxCuerpoPinball;
		model = glm::translate(model, glm::vec3(-183.0f, 62.0f, 375.0f));
		model = glm::scale(model, glm::vec3(25.0f, 25.0f, 25.0f));
		model = glm::rotate(model, 15 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 40 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera->calculateViewMatrix()));
		meshList[2]->RenderMesh();

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
		meshList[2]->RenderMesh();

		//Piramide Flipper
		modelaux = model;
		model = modelauxCuerpoPinball;
		model = glm::translate(model, glm::vec3(65.0f, 44.9f, 450.0f));
		model = glm::scale(model, glm::vec3(30.0f, 30.0f, 30.0f));
		model = glm::rotate(model, 15 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera->calculateViewMatrix()));
		meshList[2]->RenderMesh();

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
		meshList[2]->RenderMesh();



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
