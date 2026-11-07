/*
Animación:
Simple o básica: Por banderas y condicionales (más de 1 transformación geométrica se ve modificada)
Compleja: Por medio de funciones y algoritmos.
Textura Animada
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

#include "Window.h"
#include "Mesh_tn.h"
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

#include "PlanetaryRaceTrack.h"




FRAME KeyFrame[MAX_FRAMES];
int maxFrames = 0;
bool playKeyframes = false;
float g_i_curr_steps = 0;
float g_i_max_steps = 70;
int g_frame_play_index = 0;
int keyframeLoopStart = 0;    // <-- NUEVO

// Cápsula
float cohetePosX = 10.5f, cohetePosY = 18.0f, cohetePosZ = -31.0f;
float coheteRotX = 0.0f, coheteRotY = 0.0f, coheteRotZ = 0.0f;

// PiezaSuelta1
float pieza1PosX = 10.5f, pieza1PosY = 18.0f, pieza1PosZ = -31.0f;
float pieza1RotX = 0.0f, pieza1RotY = 0.0f, pieza1RotZ = 0.0f;

// PiezaSuelta2
float pieza2PosX = 10.5f, pieza2PosY = 18.0f, pieza2PosZ = -31.0f;
float pieza2RotX = 0.0f, pieza2RotY = 0.0f, pieza2RotZ = 0.0f;

// PiezaSuelta3
float pieza3PosX = 10.5f, pieza3PosY = 18.0f, pieza3PosZ = -31.0f;
float pieza3RotX = 0.0f, pieza3RotY = 0.0f, pieza3RotZ = 0.0f;







Shader trackShader;          // Shader independiente para la pista
PlanetaryRaceTrack raceTrack; // Instancia de la pista

const float toRadians = 3.14159265f / 180.0f;

//variables para animación
bool avanza;

float toffsetflechau = 0.0f;
float toffsetflechav = 0.0f;
float toffsetnumerou = 0.0f;
float toffsetnumerov = 0.0f;
float toffsetnumerocambiau = 0.0;
float angulovaria = 0.0f;
float movCoche = 0.0f;
float movOffset = 0.01f;


float trackProgress = 0.0f;
float shipSpeed = 0.6f; // Velocidad con la que la nave recorre el circuito

// Variables para la animación del satélite
float sateliteMovX = 0.0f;
bool avanzaSatelite = true;
float anguloPaneles = 0.0f;
float anguloAntena = 0.0f;


// Partículas de humo
struct ParticulaHumo
{
	glm::vec3 posicion;
	double nacimiento;
};

std::vector<ParticulaHumo> particulasHumo;

double ultimoHumo = 0.0;


// Variables para el Holocron
float holocronRotY = 0.0f;


// Variables para la animación y trayectoria del Cohete
float tiempoCohete = 0.0f;
float coheteMovY = 0.0f;


Window mainWindow;
std::vector<MeshModel*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;
Texture FlechaTexture;
Texture NumerosTexture;
Texture Numero1Texture;
Texture Numero2Texture;


Model NaveEspacial_M;

Texture HumoTexture;	

//Modelos del Satélite
Model BaseSatelite_M;
Model PanelDerecho_M;
Model PanelIzquierdo_M;
Model BaseAntena_M;
Model Antena_M;

Model Avion_M;

Model Holocron_M;

Model Plataforma_M;
Model Cohete_M;          // cápsula
Model PiezaSuelta1_M;
Model PiezaSuelta2_M;
Model PiezaSuelta3_M;

Skybox skybox;


//materiales
Material Material_brillante;
Material Material_opaco;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";



void saveKeyFrames() {
	FILE* fKeyFrames = nullptr;
	errno_t err = fopen_s(&fKeyFrames, "keyframescohete.txt", "w");
	if (err != 0 || fKeyFrames == NULL) { printf("Error al abrir para escritura\n"); return; }
	fprintf(fKeyFrames, "%d\n", maxFrames);
	fprintf(fKeyFrames, "%d\n", keyframeLoopStart);
	for (int i = 0; i < maxFrames; i++) {
		fprintf(fKeyFrames,
			"%f %f %f %f %f %f "
			"%f %f %f %f %f %f "
			"%f %f %f %f %f %f "
			"%f %f %f %f %f %f\n",
			KeyFrame[i].posX, KeyFrame[i].posY, KeyFrame[i].posZ,
			KeyFrame[i].rotX, KeyFrame[i].rotY, KeyFrame[i].rotZ,
			KeyFrame[i].p1_posX, KeyFrame[i].p1_posY, KeyFrame[i].p1_posZ,
			KeyFrame[i].p1_rotX, KeyFrame[i].p1_rotY, KeyFrame[i].p1_rotZ,
			KeyFrame[i].p2_posX, KeyFrame[i].p2_posY, KeyFrame[i].p2_posZ,
			KeyFrame[i].p2_rotX, KeyFrame[i].p2_rotY, KeyFrame[i].p2_rotZ,
			KeyFrame[i].p3_posX, KeyFrame[i].p3_posY, KeyFrame[i].p3_posZ,
			KeyFrame[i].p3_rotX, KeyFrame[i].p3_rotY, KeyFrame[i].p3_rotZ);
	}
	fclose(fKeyFrames);
	printf("Keyframes guardados. Total=%d, loopStart=%d\n", maxFrames, keyframeLoopStart);
}

void loadKeyFrames() {
	FILE* fKeyFrames = nullptr;
	errno_t err = fopen_s(&fKeyFrames, "keyframescohete.txt", "r");
	if (err != 0 || fKeyFrames == NULL) { printf("Error al abrir para lectura\n"); return; }
	fscanf_s(fKeyFrames, "%d", &maxFrames);
	fscanf_s(fKeyFrames, "%d", &keyframeLoopStart);
	for (int i = 0; i < maxFrames; i++) {
		fscanf_s(fKeyFrames,
			"%f %f %f %f %f %f "
			"%f %f %f %f %f %f "
			"%f %f %f %f %f %f "
			"%f %f %f %f %f %f",
			&KeyFrame[i].posX, &KeyFrame[i].posY, &KeyFrame[i].posZ,
			&KeyFrame[i].rotX, &KeyFrame[i].rotY, &KeyFrame[i].rotZ,
			&KeyFrame[i].p1_posX, &KeyFrame[i].p1_posY, &KeyFrame[i].p1_posZ,
			&KeyFrame[i].p1_rotX, &KeyFrame[i].p1_rotY, &KeyFrame[i].p1_rotZ,
			&KeyFrame[i].p2_posX, &KeyFrame[i].p2_posY, &KeyFrame[i].p2_posZ,
			&KeyFrame[i].p2_rotX, &KeyFrame[i].p2_rotY, &KeyFrame[i].p2_rotZ,
			&KeyFrame[i].p3_posX, &KeyFrame[i].p3_posY, &KeyFrame[i].p3_posZ,
			&KeyFrame[i].p3_rotX, &KeyFrame[i].p3_rotY, &KeyFrame[i].p3_rotZ);
	}
	fclose(fKeyFrames);
	printf("Keyframes cargados. Total=%d, loopStart=%d\n", maxFrames, keyframeLoopStart);
}




//cálculo del promedio de las normales para sombreado de Phong
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
	
	unsigned int flechaIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat flechaVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,
	};

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

	unsigned int numeroIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat numeroVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.67f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		0.25f, 0.67f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		0.25f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,
	};

	MeshModel *obj1 = new MeshModel();
	obj1->CreateMeshModel(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	MeshModel *obj2 = new MeshModel();
	obj2->CreateMeshModel(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	MeshModel *obj3 = new MeshModel();
	obj3->CreateMeshModel(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	MeshModel* obj4 = new MeshModel();
	obj4->CreateMeshModel(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	MeshModel* obj5 = new MeshModel();
	obj5->CreateMeshModel(flechaVertices, flechaIndices, 32, 6);
	meshList.push_back(obj5);

	MeshModel* obj6 = new MeshModel();
	obj6->CreateMeshModel(scoreVertices, scoreIndices, 32, 6);
	meshList.push_back(obj6);

	MeshModel* obj7 = new MeshModel();
	obj7->CreateMeshModel(numeroVertices, numeroIndices, 32, 6);
	meshList.push_back(obj7);
}

void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}
void CreateShaderPista()
{

	Shader* trackShader = new Shader();
	trackShader->CreateFromFiles("shaders/shader_track.vert", "shaders/shader_track.frag");
	shaderList.push_back(*trackShader);
	
}
int main()
{
	mainWindow = Window(1366, 768);
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();
	trackShader.CreateFromFiles("shaders/shader_track.vert", "shaders/shader_track.frag");
	raceTrack.buildMesh();

	

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.5f, 0.5f);

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
	FlechaTexture = Texture("Textures/flechas.tga");
	FlechaTexture.LoadTextureA();
	NumerosTexture = Texture("Textures/numerosbase.tga");
	NumerosTexture.LoadTextureA();
	Numero1Texture = Texture("Textures/numero1.tga");
	Numero1Texture.LoadTextureA();
	Numero2Texture = Texture("Textures/numero2.tga");
	Numero2Texture.LoadTextureA();


	HumoTexture = Texture("Textures/humo.png");
	HumoTexture.LoadTextureA();


	NaveEspacial_M = Model();
	NaveEspacial_M.LoadModel("Models/navespacial.obj");

	//Modelos del Satélite
	BaseSatelite_M = Model();
	BaseSatelite_M.LoadModel("Models/basesatelite.obj");

	PanelDerecho_M = Model();
	PanelDerecho_M.LoadModel("Models/panelder.obj");

	PanelIzquierdo_M = Model();
	PanelIzquierdo_M.LoadModel("Models/panelizq.obj");

	BaseAntena_M = Model();
	BaseAntena_M.LoadModel("Models/baseantena.obj");

	Antena_M = Model();
	Antena_M.LoadModel("Models/antena.obj");

	Avion_M = Model();
	Avion_M.LoadModel("Models/roger.obj");


	Holocron_M = Model();
	Holocron_M.LoadModel("Models/holocronSWtexturizado.obj");

	Cohete_M = Model();
	Cohete_M.LoadModel("Models/Cohete.obj");


	Plataforma_M = Model();
	Plataforma_M.LoadModel("Models/Plataforma.obj");


	PiezaSuelta1_M = Model(); 
	PiezaSuelta1_M.LoadModel("Models/PiezaSuelta1.obj");
	PiezaSuelta2_M = Model();  
	PiezaSuelta2_M.LoadModel("Models/PiezaSuelta2.obj");
	PiezaSuelta3_M = Model();  
	PiezaSuelta3_M.LoadModel("Models/PiezaSuelta3.obj");


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

	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);

	unsigned int pointLightCount = 0;
	pointLights[0] = PointLight(1.0f, 0.0f, 0.0f,
		0.0f, 1.0f,
		0.0f, 2.5f, 1.5f,
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


	// 3. Luz de faro hacia abajo para el Avión
	spotLights[1] = SpotLight(0.0f, 1.0f, 1.0f,
		0.1f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		30.0f);
	spotLightCount++;

	// 4. Luz de faro frontal para el Avión 
	spotLights[2] = SpotLight(0.0f, 1.0f, 1.0f,
		0.1f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, -1.0f,
		1.0f, 0.0f, 0.0f,
		30.0f);
	spotLightCount++;

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0, uniformTextureOffset=0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	
	movCoche = 0.0f;
	movOffset = 0.01f;

	glm::vec3 lowerLight(0.0f,0.0f,0.0f);

	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec2 toffset = glm::vec2(0.0f, 0.0f);

	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		angulovaria += 0.5f * deltaTime;




		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());


		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();
		uniformTextureOffset = shaderList[0].getOffsetLocation();

		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);

		model = glm::mat4(1.0);
		modelaux = glm::mat4(1.0);
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		toffset = glm::vec2(0.0f, 0.0f);

		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

		meshList[2]->RenderMeshModel();

		/*
		//Agave
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, -4.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		AgaveTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMeshModel();
		*/




		//Holocron

		// 1. Actualizar ángulos de rotación (gira sobre Y y un poco sobre X)
		holocronRotY += 2.0f * deltaTime; // 45 grados por segundo

		// 2. Flotación usando una función senoidal
		float holocronFlote = sin(glfwGetTime() * 1.0f) * 4.0f;

		glm::mat4 modelHolocron = glm::mat4(1.0);
		modelHolocron = glm::translate(modelHolocron, glm::vec3(-0.5f, 3.5f + holocronFlote, -35.0f));
		// Aplicar rotaciones
		modelHolocron = glm::rotate(modelHolocron, glm::radians(holocronRotY), glm::vec3(0.0f, 1.0f, 0.0f));
		modelHolocron = glm::scale(modelHolocron, glm::vec3(0.5f, 0.5f, 0.5f));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelHolocron));
		Holocron_M.RenderModel();

		//plataforma

		glm::mat4 modelPlataforma = glm::mat4(1.0);
		modelPlataforma = glm::translate(modelPlataforma, glm::vec3(10.5f, 0.0f, -30.0f));
		modelPlataforma = glm::rotate(modelPlataforma, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelPlataforma = glm::scale(modelPlataforma, glm::vec3(5.0f, 5.0f, 5.0f));
		
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelPlataforma));
		Plataforma_M.RenderModel();



		//	Cohete

// ============================================================
// ANIMACIÓN POR KEYFRAMES (cápsula + 3 piezas)
// ============================================================
		if (playKeyframes && maxFrames > 1) {
			if (g_i_curr_steps >= g_i_max_steps) {
				g_i_curr_steps = 0;
				g_frame_play_index++;
				if (g_frame_play_index >= maxFrames) {
					g_frame_play_index = keyframeLoopStart;   // <-- LOOP desde el frame marcado
				}
			}
			// siguiente frame con wrap para loop suave
			int nextIdx = g_frame_play_index + 1;
			if (nextIdx >= maxFrames) nextIdx = keyframeLoopStart;

			float t = g_i_curr_steps / g_i_max_steps;
			FRAME cur = KeyFrame[g_frame_play_index];
			FRAME nxt = KeyFrame[nextIdx];

			auto L = [](float a, float b, float t) { return a + (b - a) * t; };

			// Cápsula
			cohetePosX = L(cur.posX, nxt.posX, t);
			cohetePosY = L(cur.posY, nxt.posY, t);
			cohetePosZ = L(cur.posZ, nxt.posZ, t);
			coheteRotX = L(cur.rotX, nxt.rotX, t);
			coheteRotY = L(cur.rotY, nxt.rotY, t);
			coheteRotZ = L(cur.rotZ, nxt.rotZ, t);

			// Pieza 1
			pieza1PosX = L(cur.p1_posX, nxt.p1_posX, t);
			pieza1PosY = L(cur.p1_posY, nxt.p1_posY, t);
			pieza1PosZ = L(cur.p1_posZ, nxt.p1_posZ, t);
			pieza1RotX = L(cur.p1_rotX, nxt.p1_rotX, t);
			pieza1RotY = L(cur.p1_rotY, nxt.p1_rotY, t);
			pieza1RotZ = L(cur.p1_rotZ, nxt.p1_rotZ, t);

			// Pieza 2
			pieza2PosX = L(cur.p2_posX, nxt.p2_posX, t);
			pieza2PosY = L(cur.p2_posY, nxt.p2_posY, t);
			pieza2PosZ = L(cur.p2_posZ, nxt.p2_posZ, t);
			pieza2RotX = L(cur.p2_rotX, nxt.p2_rotX, t);
			pieza2RotY = L(cur.p2_rotY, nxt.p2_rotY, t);
			pieza2RotZ = L(cur.p2_rotZ, nxt.p2_rotZ, t);

			// Pieza 3
			pieza3PosX = L(cur.p3_posX, nxt.p3_posX, t);
			pieza3PosY = L(cur.p3_posY, nxt.p3_posY, t);
			pieza3PosZ = L(cur.p3_posZ, nxt.p3_posZ, t);
			pieza3RotX = L(cur.p3_rotX, nxt.p3_rotX, t);
			pieza3RotY = L(cur.p3_rotY, nxt.p3_rotY, t);
			pieza3RotZ = L(cur.p3_rotZ, nxt.p3_rotZ, t);

			g_i_curr_steps += 1.0f;
		}

		// ---- Función auxiliar para construir la matriz de un cuerpo ----
		auto buildMatrix = [](float px, float py, float pz,
			float rx, float ry, float rz) {
				glm::mat4 m = glm::mat4(1.0f);
				m = glm::translate(m, glm::vec3(px, py, pz));
				m = glm::rotate(m, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // base fija
				m = glm::rotate(m, glm::radians(rx), glm::vec3(1.0f, 0.0f, 0.0f));
				m = glm::rotate(m, glm::radians(ry), glm::vec3(0.0f, 1.0f, 0.0f));
				m = glm::rotate(m, glm::radians(rz), glm::vec3(0.0f, 0.0f, 1.0f));
				m = glm::scale(m, glm::vec3(3.0f, 3.0f, 3.0f));
				return m;
			};

		// ---- Render cápsula ----
		glm::mat4 modelCohete = buildMatrix(cohetePosX, cohetePosY, cohetePosZ,
			coheteRotX, coheteRotY, coheteRotZ);
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelCohete));
		Cohete_M.RenderModel();

		// ---- Render PiezaSuelta1 ----
		glm::mat4 modelP1 = buildMatrix(pieza1PosX, pieza1PosY, pieza1PosZ,
			pieza1RotX, pieza1RotY, pieza1RotZ);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelP1));
		PiezaSuelta1_M.RenderModel();

		// ---- Render PiezaSuelta2 ----
		glm::mat4 modelP2 = buildMatrix(pieza2PosX, pieza2PosY, pieza2PosZ,
			pieza2RotX, pieza2RotY, pieza2RotZ);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelP2));
		PiezaSuelta2_M.RenderModel();

		// ---- Render PiezaSuelta3 ----
		glm::mat4 modelP3 = buildMatrix(pieza3PosX, pieza3PosY, pieza3PosZ,
			pieza3RotX, pieza3RotY, pieza3RotZ);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelP3));
		PiezaSuelta3_M.RenderModel();






		// ANIMACIÓN DE LA NAVE ESPACIAL SOBRE EL CIRCUITO DE LA PISTA

			trackProgress += shipSpeed * deltaTime;
			if (trackProgress >= raceTrack.pathData.size()) {
				trackProgress = 0.0f; 
			}

			int index = static_cast<int>(trackProgress) % raceTrack.pathData.size();
			const auto& sample = raceTrack.pathData[index];

			// ELEVACIÓN SOBRE LA PISTA
			float alturaFlotacion = 1.5f;
			glm::vec3 posicionConAltura = sample.pos + (sample.up * alturaFlotacion);

			// Construir la matriz de transformación con la nueva posición elevada
			glm::mat4 modelNave = glm::mat4(1.0f);
			modelNave = glm::translate(modelNave, posicionConAltura);
			glm::mat4 rotMat(1.0f);
			rotMat[0] = glm::vec4(sample.right, 0.0f);
			rotMat[1] = glm::vec4(sample.up, 0.0f);
			rotMat[2] = glm::vec4(-sample.fwd, 0.0f); 
			modelNave = modelNave * rotMat;
			modelNave = glm::scale(modelNave, glm::vec3(1.0f, 1.0f, 1.0f));

			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelNave));
			NaveEspacial_M.RenderModel();


// LÓGICA DE ANIMACIÓN DEL SATÉLITE


// 1. Movimiento de ida y vuelta en X
		if (avanzaSatelite)
		{
			sateliteMovX += 0.1f * deltaTime;
			if (sateliteMovX > 35.0f) {
				avanzaSatelite = false;
			}
		}
		else
		{
			sateliteMovX -= 0.2f * deltaTime;
			if (sateliteMovX < -35.0f) {
				avanzaSatelite = true;
			}
		}

		// 2. Flotación vertical suave usando una función senoidal
		float sateliteMovY = sin(glfwGetTime() * 2.0f) * 1.5f;

		// 3. Ángulos oscilantes automáticos (-45° a 45°) para paneles y antena
		anguloPaneles = sin(glfwGetTime() * 1.0f) * 30.0f;
		anguloAntena = cos(glfwGetTime() * 2.0f) * 45.0f;




		//SATÉLITE 


		// Base del satélite (PADRE DE TODO) 
		glm::mat4 modelSatelite = glm::mat4(1.0f);
		modelSatelite = glm::translate(modelSatelite, glm::vec3(-15.0f + sateliteMovX, -2.0f + sateliteMovY, -3.0f));
		color = glm::vec3(0.7f, 0.7f, 0.7f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelSatelite));
		BaseSatelite_M.RenderModel();

		// Panel derecho (HIJO de la base) 
		glm::mat4 modelPanelDer = modelSatelite;
		modelPanelDer = glm::translate(modelPanelDer, glm::vec3(-5.5f, 15.7f, 13.0f));
		modelPanelDer = glm::rotate(modelPanelDer, glm::radians(anguloPaneles), glm::vec3(0.0f, 0.0f, 1.0f)); 
		color = glm::vec3(0.2f, 0.3f, 0.8f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelPanelDer));
		PanelDerecho_M.RenderModel();

		// Panel izquierdo (HIJO de la base)
		glm::mat4 modelPanelIzq = modelSatelite;
		modelPanelIzq = glm::translate(modelPanelIzq, glm::vec3(-5.9f, 15.9f, 12.7f));
		modelPanelIzq = glm::rotate(modelPanelIzq, glm::radians(-anguloPaneles), glm::vec3(0.0f, 0.0f, 1.0f));
		color = glm::vec3(0.2f, 0.3f, 0.8f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelPanelIzq));
		PanelIzquierdo_M.RenderModel();

		// Base de la antena (HIJO de la base) 
		glm::mat4 modelBaseAntena = modelSatelite;
		modelBaseAntena = glm::translate(modelBaseAntena, glm::vec3(0.0f, 0.0f, 0.0f));
		color = glm::vec3(0.4f, 0.4f, 0.4f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelBaseAntena));
		BaseAntena_M.RenderModel();

		// Antena (HIJA de la base de la antena) 
		glm::mat4 modelAntena = modelBaseAntena;
		modelAntena = glm::translate(modelAntena, glm::vec3(-5.5f, 16.3f, 12.5f));
		modelAntena = glm::rotate(modelAntena, glm::radians(anguloAntena), glm::vec3(1.0f, 0.0f, 0.0f));
		color = glm::vec3(0.6f, 0.6f, 0.6f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelAntena)); 
		Antena_M.RenderModel();





		// ANIMACIÓN DEL AVIÓN MEDIANTE FUNCIONES TRIGONOMÉTRICAS

		// Tiempo y velocidad del vuelo
		float tiempoAvion = (float)glfwGetTime();
		float velocidadAvion = 0.7f;
		float anguloAvion = tiempoAvion * velocidadAvion;

		// Movimiento circular con ascensos y descensos
		float avionX = 15.5f - 12.0f * sin(anguloAvion);
		float avionY = 10.5f + 3.0f * sin(2.0f * anguloAvion);
		float avionZ = 20.0f + 12.0f * (1.0f - cos(anguloAvion));

		// Dirección del vuelo mediante la derivada
		glm::vec3 direccionAvion(
			-12.0f * velocidadAvion * cos(anguloAvion),
			6.0f * velocidadAvion * cos(2.0f * anguloAvion),
			12.0f * velocidadAvion * sin(anguloAvion)
		);

		direccionAvion = glm::normalize(direccionAvion);

		// Calcular la orientación del avión
		glm::vec3 arribaMundo(0.0f, 1.0f, 0.0f);
		glm::vec3 derechaAvion = glm::normalize(glm::cross(direccionAvion, arribaMundo));
		glm::vec3 arribaAvion = glm::cross(derechaAvion, direccionAvion);

		glm::mat4 rotacionAvion(1.0f);
		rotacionAvion[0] = glm::vec4(derechaAvion, 0.0f);
		rotacionAvion[1] = glm::vec4(arribaAvion, 0.0f);
		rotacionAvion[2] = glm::vec4(-direccionAvion, 0.0f);

		// Inclinación lateral durante el vuelo
		float inclinacionAvion = 30.0f * sin(anguloAvion);

		// Matriz de transformación del avión
		glm::mat4 modelAvion = glm::mat4(1.0f);
		modelAvion = glm::translate(modelAvion, glm::vec3(avionX, avionY, avionZ));
		modelAvion = modelAvion * rotacionAvion;
		modelAvion = glm::rotate(modelAvion, glm::radians(inclinacionAvion), glm::vec3(0.0f, 0.0f, 1.0f));
		modelAvion = glm::scale(modelAvion, glm::vec3(0.5f, 0.5f, 0.5f));


		// Dibujar el avión
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelAvion));
		Avion_M.RenderModel();


		glm::vec4 avionPosAbajo = modelAvion * glm::vec4(0.0f, -0.5f, 0.0f, 1.0f);
		glm::vec3 avionDirAbajo = glm::vec3(modelAvion * glm::vec4(0.0f, -1.0f, 0.0f, 0.0f));
		avionDirAbajo = glm::normalize(avionDirAbajo); 
		spotLights[1].SetFlash(glm::vec3(avionPosAbajo), avionDirAbajo);


		glm::vec4 avionPosFrente = modelAvion * glm::vec4(0.0f, 0.0f, -2.0f, 1.0f);
		glm::vec3 avionDirFrente = glm::vec3(modelAvion * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f));
		avionDirFrente = glm::normalize(avionDirFrente);
		spotLights[2].SetFlash(glm::vec3(avionPosFrente), avionDirFrente);


		// GENERACIÓN DE PARTÍCULAS DE HUMO

		double tiempoActual = glfwGetTime();

		// Generar humo cada 0.05 segundos
		if (tiempoActual - ultimoHumo >= 0.05)
		{
			// Posiciones locales de los motores
			glm::vec4 motorIzquierdo(-2.0f, -0.2f, 1.5f, 1.0f);
			glm::vec4 motorDerecho(2.0f, -0.2f, 1.5f, 1.0f);

			// Transformar las posiciones según el movimiento del avión
			glm::vec3 posicionIzquierda =
				glm::vec3(modelAvion * motorIzquierdo);

			glm::vec3 posicionDerecha =
				glm::vec3(modelAvion * motorDerecho);

			// Crear las partículas
			ParticulaHumo humoIzquierdo;
			humoIzquierdo.posicion = posicionIzquierda;
			humoIzquierdo.nacimiento = tiempoActual;

			ParticulaHumo humoDerecho;
			humoDerecho.posicion = posicionDerecha;
			humoDerecho.nacimiento = tiempoActual;

			// Guardar las partículas
			particulasHumo.push_back(humoIzquierdo);
			particulasHumo.push_back(humoDerecho);

			ultimoHumo = tiempoActual;
		}

		for (int i = 0; i < (int)particulasHumo.size();)
		{
			double edad = tiempoActual - particulasHumo[i].nacimiento;

			if (edad >= 2.0)
			{
				particulasHumo.erase(particulasHumo.begin() + i);
			}
			else
			{
				i++;
			}
		}

		// DIBUJAR EL HUMO UTILIZANDO UNA TEXTURA

		// Obtener las variables del shader
		GLint programaActual = 0;
		glGetIntegerv(GL_CURRENT_PROGRAM, &programaActual);

		GLint uniformEsHumo =
			glGetUniformLocation(programaActual, "esHumo");

		GLint uniformOpacidadHumo =
			glGetUniformLocation(programaActual, "opacidadHumo");

		// Activar el modo de humo
		glUniform1i(uniformEsHumo, 1);


		// Restaurar matrices de cámara y proyección
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		// Reiniciar desplazamiento de textura
		glm::vec2 offsetHumo(0.0f, 0.0f);
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(offsetHumo));

		// Activar transparencia
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Evitar que el humo escriba en el buffer de profundidad
		glDepthMask(GL_FALSE);

		// Dibujar las dos caras de los planos
		GLboolean cullingAnterior = glIsEnabled(GL_CULL_FACE);
		glDisable(GL_CULL_FACE);

		// Aplicar textura y material
		HumoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

		// Dibujar cada partícula
		for (int i = 0; i < (int)particulasHumo.size(); i++)
		{

			// Calcular la edad de la partícula
			float edad = (float)(tiempoActual - particulasHumo[i].nacimiento);

			// Reducir la opacidad conforme pasa el tiempo
			float opacidad = 0.7f * pow(1.0f - edad / 2.0f, 5.0f);

			// Enviar la opacidad al shader
			glUniform1f(uniformOpacidadHumo, opacidad);

			// El humo aumenta su tamaño con el tiempo
			float tamanoHumo = 0.5f + edad * 0.4f;

			// Posición de la partícula
			glm::vec3 posicionHumo = particulasHumo[i].posicion;

			// El humo asciende ligeramente
			posicionHumo.y += edad * 0.3f;

			// Transformaciones
			glm::mat4 modelHumo(1.0f);

			modelHumo = glm::translate(modelHumo, posicionHumo);
			modelHumo = glm::scale(modelHumo, glm::vec3(tamanoHumo));

			glUniformMatrix4fv(
				uniformModel, 1, GL_FALSE, glm::value_ptr(modelHumo)
			);
			// Desactivar el modo de humo
			glUniform1i(uniformEsHumo, 0);

			// Color blanco
			color = glm::vec3(1.0f, 1.0f, 1.0f);
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));

			// Reutilizar los planos de vegetación
			meshList[3]->RenderMeshModel();
		}

		// Restaurar estados
		glDepthMask(GL_TRUE);

		if (cullingAnterior)
			glEnable(GL_CULL_FACE);


		//textura con movimiento
		toffsetflechau += 0.001;
		toffsetflechav = 0.000;
		if (toffsetflechau > 1.0)
			toffsetflechau = 0.0;
		toffset = glm::vec2(toffsetflechau, toffsetflechav);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-2.0f, 1.0f, -6.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		FlechaTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[4]->RenderMeshModel();

		//plano con todos los números
		toffsetnumerou = 0.0;
		toffsetnumerov = 0.0;
		toffset = glm::vec2(toffsetnumerou, toffsetnumerov);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-6.0f, 2.0f, -6.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		NumerosTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMeshModel();

		//número 1
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.0f, 2.0f, -6.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		NumerosTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[6]->RenderMeshModel();

		for (int i = 1; i<4; i++)
		{
			toffsetnumerou += 0.25;
			toffsetnumerov = 0.0;
			toffset = glm::vec2(toffsetnumerou, toffsetnumerov);
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-10.0f - (i * 3.0), 2.0f, -6.0f));
			model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
			glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			color = glm::vec3(1.0f, 1.0f, 1.0f);
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			NumerosTexture.UseTexture();
			Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
			meshList[6]->RenderMeshModel();
		}

		for (int j = 1; j < 5; j++)
		{
			toffsetnumerou += 0.25;
			toffsetnumerov = -0.33;
			toffset = glm::vec2(toffsetnumerou, toffsetnumerov);
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-7.0f - (j * 3.0), 5.0f, -6.0f));
			model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
			glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			color = glm::vec3(1.0f, 1.0f, 1.0f);
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			NumerosTexture.UseTexture();
			Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
			meshList[6]->RenderMeshModel();
		}
 
		//número cambiante 
		toffsetnumerocambiau += 0.25; 
		if (toffsetnumerocambiau > 1.0)
			toffsetnumerocambiau = 0.0;
		toffsetnumerov = 0.0;
		toffset = glm::vec2(toffsetnumerocambiau, toffsetnumerov);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.0f, 10.0f, -6.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		NumerosTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[6]->RenderMeshModel();

		//cambiar automáticamente entre textura número 1 y número 2
		toffsetnumerou = 0.0;
		toffsetnumerov = 0.0;
		toffset = glm::vec2(toffsetnumerou, toffsetnumerov);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-13.0f, 10.0f, -6.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Numero1Texture.UseTexture();
		
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMeshModel();



		trackShader.UseShader();

		glm::mat4 modelTrack = glm::mat4(1.0f);
		glUniformMatrix4fv(trackShader.GetProjectionLocation(), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(trackShader.GetViewLocation(), 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniformMatrix4fv(trackShader.GetModelLocation(), 1, GL_FALSE, glm::value_ptr(modelTrack));


		// Dibujar la pista
		raceTrack.draw();

		

		glDisable(GL_BLEND);

		glUseProgram(0);

		mainWindow.swapBuffers();

	}

	return 0;
}