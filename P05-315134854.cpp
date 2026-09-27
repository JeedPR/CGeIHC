/*
Práctica 5: Optimización y Carga de Modelos
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
#include "Mesh_tn.h"
#include "Shader_m.h"
#include "Camera.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

const float toRadians = 3.14159265f / 180.0f;
//float angulocola = 0.0f;
Window mainWindow;
std::vector<Mesh*> meshList; //solo recibe xyz
std::vector<MeshColor*> meshListColor; // recibe xyz rgb
std::vector<MeshModel*> meshListModel; // recibe xyz uv nx ny nz
std::vector<Shader> shaderList;

Camera camera;
//Lista de Modelos a importar
Model Rover_M;
Model BaseRover_M;
Model BaseBrazo_M;
Model Brazo_M;
Model LlantaDelIzq_M;
Model LlantaDelDer_M;
Model LlantaMedIzq_M;
Model LlantaMedDer_M;
Model LlantaTraIzq_M;
Model LlantaTraDer_M;
Model HolocronCentro_M;
Model Esquina1_M;
Model Esquina2_M;
Model Esquina3_M;
Model Esquina4_M;
Model Esquina5_M;
Model Esquina6_M;
Model Esquina7_M;
Model Esquina8_M;

//Modelos del Satélite
Model BaseSatelite_M;
Model PanelDerecho_M;
Model PanelIzquierdo_M;
Model BaseAntena_M;
Model Antena_M;

//Lista de Skybox a crear
Skybox skybox;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// Vertex Shader
static const char* vShader = "shaders/shader_m.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_m.frag";


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


	MeshModel* obj1 = new MeshModel();
	obj1->CreateMeshModel(vertices, indices, 32, 12);
	meshListModel.push_back(obj1);

	MeshModel* obj2 = new MeshModel();
	obj2->CreateMeshModel(vertices, indices, 32, 12);
	meshListModel.push_back(obj2);

	MeshModel* obj3 = new MeshModel();
	obj3->CreateMeshModel(floorVertices, floorIndices, 32, 6);
	meshListModel.push_back(obj3);


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

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.5f, 7.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.3f);
	//Cargar modelos
	Rover_M = Model();
	Rover_M.LoadModel("Models/RoverModificado.obj");

	BaseRover_M = Model();
	BaseRover_M.LoadModel("Models/baseRover.obj");

	BaseBrazo_M = Model();
	BaseBrazo_M.LoadModel("Models/BaseBrazo.obj");

	Brazo_M = Model();
	Brazo_M.LoadModel("Models/Brazo.obj");

	LlantaDelIzq_M = Model();
	LlantaDelIzq_M.LoadModel("Models/LlantaDelIzq.obj");

	LlantaDelDer_M = Model();
	LlantaDelDer_M.LoadModel("Models/LlantaDelDer.obj");

	LlantaMedIzq_M = Model();
	LlantaMedIzq_M.LoadModel("Models/LlantaMedIzq.obj");

	LlantaMedDer_M = Model();
	LlantaMedDer_M.LoadModel("Models/LlantaMedDer.obj");

	LlantaTraIzq_M = Model();
	LlantaTraIzq_M.LoadModel("Models/LlantaTraIzq.obj");

	LlantaTraDer_M = Model();
	LlantaTraDer_M.LoadModel("Models/LlantaTraDer.obj");

	HolocronCentro_M = Model();
	HolocronCentro_M.LoadModel("Models/holocronBase.obj");

	Esquina1_M = Model();
	Esquina1_M.LoadModel("Models/holocron1.obj");

	Esquina2_M = Model();
	Esquina2_M.LoadModel("Models/holocron2.obj");

	Esquina3_M = Model();
	Esquina3_M.LoadModel("Models/holocron3.obj");

	Esquina4_M = Model();
	Esquina4_M.LoadModel("Models/holocron4.obj");

	Esquina5_M = Model();
	Esquina5_M.LoadModel("Models/holocron5.obj");

	Esquina6_M = Model();
	Esquina6_M.LoadModel("Models/holocron6.obj");

	Esquina7_M = Model();
	Esquina7_M.LoadModel("Models/holocron7.obj");

	Esquina8_M = Model();
	Esquina8_M.LoadModel("Models/holocron8.obj");

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


	//Crear Skybox con sus 6 texturas
	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);


	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0, uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);

	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//Se dibuja el Skybox
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);

		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformColor = shaderList[0].getColorLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		// INICIA DIBUJO DEL PISO
		color = glm::vec3(0.5f, 0.5f, 0.5f); //piso de color gris
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshListModel[2]->RenderMeshModel();

		//------------*INICIA DIBUJO DE NUESTROS DEMÁS OBJETOS-------------------*

//Modelo Inicial (PADRE DE TODOS)
		color = glm::vec3(0.0f, 0.0f, 1.0f); //modelo de color azul

		glm::mat4 modelBase = glm::mat4(1.0);
		modelBase = glm::translate(modelBase, glm::vec3(0.0f, -2.0f, -1.5f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelBase));
		BaseRover_M.RenderModel();//modificar por el modelo de solo cuerpo del Rover, para que se pueda separar el brazo y las llantas
		color = glm::vec3(0.0f, 0.0f, 1.0f);
		//En sesión se separara una parte del modelo y se unirá por jeraquía al cuerpo

		//Brazo BASE (HIJO DE LA BASE)
		glm::mat4 modelBaseBrazo = modelBase;
		modelBaseBrazo = glm::translate(modelBaseBrazo, glm::vec3(0.0f, 0.0f, 0.0f));
		color = glm::vec3(0.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelBaseBrazo));
		BaseBrazo_M.RenderModel();

		// BRAZO (HIJO DE LA BASE DEL BRAZO) — ROTA con articulacion1
		glm::mat4 modelBrazo = modelBaseBrazo;
		modelBrazo = glm::translate(modelBrazo, glm::vec3(-2.2f, 6.0f, 1.0f));
		modelBrazo = glm::rotate(modelBrazo, glm::radians(mainWindow.getarticulacion1()), glm::vec3(0.0f, 0.0f, 1.0f));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelBrazo));
		Brazo_M.RenderModel();

		//Siguientes modelos
		/* Ejercicio:
		1.- Separar las llantas
		2.- Hacer que al presionar una tecla cada pata de rueda pueda rotar un máximo de 45° "hacia adelante y hacia atrás"
		*/

		// Llanta delantera derecha (HIJO DE LA BASE) — ROTA con llanta1
		glm::mat4 modelLlantaDelDer = modelBase;
		modelLlantaDelDer = glm::translate(modelLlantaDelDer, glm::vec3(-2.2f, 4.2f, -2.75f));
		modelLlantaDelDer = glm::rotate(modelLlantaDelDer, glm::radians(mainWindow.getllanta1()), glm::vec3(0.0f, 0.0f, 1.0f));
		color = glm::vec3(0.5f, 5.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelLlantaDelDer));
		LlantaDelDer_M.RenderModel();

		// Llanta delantera izquierda (HIJO DE LA BASE) — ROTA con llanta2
		glm::mat4 modelLlantaDelIzq = modelBase;
		modelLlantaDelIzq = glm::translate(modelLlantaDelIzq, glm::vec3(-2.2f, 4.5f, 3.05f));
		modelLlantaDelIzq = glm::rotate(modelLlantaDelIzq, glm::radians(mainWindow.getllanta2()), glm::vec3(0.0f, 0.0f, 1.0f));
		color = glm::vec3(0.5f, 0.5f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelLlantaDelIzq));
		LlantaDelIzq_M.RenderModel();

		//Llanta media derecha (HIJO DE LA BASE) — ROTA con llanta3
		glm::mat4 modelLlantaMedDer = modelBase;
		modelLlantaMedDer = glm::translate(modelLlantaMedDer, glm::vec3(2.8f, 3.5f, -3.0f));
		modelLlantaMedDer = glm::rotate(modelLlantaMedDer, glm::radians(mainWindow.getllanta3()), glm::vec3(0.0f, 0.0f, 1.0f));
		color = glm::vec3(1.0f, 0.5f, 0.5f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelLlantaMedDer));
		LlantaMedDer_M.RenderModel();

		// Llanta media izquierda (HIJO DE LA BASE) — ROTA con llanta4
		glm::mat4 modelLlantaMedIzq = modelBase;
		modelLlantaMedIzq = glm::translate(modelLlantaMedIzq, glm::vec3(2.8f, 3.5f, 3.0f));
		modelLlantaMedIzq = glm::rotate(modelLlantaMedIzq, glm::radians(mainWindow.getllanta4()), glm::vec3(0.0f, 0.0f, 1.0f));
		color = glm::vec3(1.0f, 1.0f, 0.5f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelLlantaMedIzq));
		LlantaMedIzq_M.RenderModel();

		//Llanta trasera derecha (HIJO DE LA BASE) — ROTA con llanta5
		glm::mat4 modelLlantaTraDer = modelBase;
		modelLlantaTraDer = glm::translate(modelLlantaTraDer, glm::vec3(2.8f, 3.5f, -3.0f));
		modelLlantaTraDer = glm::rotate(modelLlantaTraDer, glm::radians(mainWindow.getllanta5()), glm::vec3(0.0f, 0.0f, 1.0f));
		color = glm::vec3(0.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelLlantaTraDer));
		LlantaTraDer_M.RenderModel();

		// Llanta trasera izquierda (HIJO DE LA BASE) — ROTA con llanta6
		glm::mat4 modelLlantaTraIzq = modelBase;
		modelLlantaTraIzq = glm::translate(modelLlantaTraIzq, glm::vec3(2.8f, 3.5f, 3.0f));
		modelLlantaTraIzq = glm::rotate(modelLlantaTraIzq, glm::radians(mainWindow.getllanta6()), glm::vec3(0.0f, 0.0f, 1.0f));
		color = glm::vec3(0.3f, 0.3f, 0.3f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelLlantaTraIzq));
		LlantaTraIzq_M.RenderModel();

		//====================== HOLOCRON ======================

		// Centro del Holocron (PADRE DE TODAS LAS ESQUINAS)
		glm::mat4 modelHolocronCentro = glm::mat4(1.0);
		modelHolocronCentro = glm::translate(modelHolocronCentro, glm::vec3(5.0f, 1.0f, -3.0f));
		color = glm::vec3(0.8f, 0.7f, 0.2f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelHolocronCentro));
		HolocronCentro_M.RenderModel();

		// Esquina 1 (HIJA) — gira sobre sí misma
		glm::mat4 modelEsquina1 = modelHolocronCentro;
		modelEsquina1 = glm::translate(modelEsquina1, glm::vec3(52.0f, 4.5f, 9.0f));
		modelEsquina1 = glm::rotate(modelEsquina1, glm::radians(mainWindow.getesquina1()), glm::vec3(0.0f, 1.0f, 0.0f)); //AJUSTA EJE
		color = glm::vec3(0.6f, 0.6f, 0.9f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelEsquina1));
		Esquina1_M.RenderModel();

		// Esquina 2 (HIJA)
		glm::mat4 modelEsquina2 = modelHolocronCentro;
		modelEsquina2 = glm::translate(modelEsquina2, glm::vec3(44.0f, 4.5f, 9.0f));
		modelEsquina2 = glm::rotate(modelEsquina2, glm::radians(mainWindow.getesquina2()), glm::vec3(0.0f, 1.0f, 0.0f));
		color = glm::vec3(0.6f, 0.6f, 0.9f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelEsquina2));
		Esquina2_M.RenderModel();

		// Esquina 3 (HIJA)
		glm::mat4 modelEsquina3 = modelHolocronCentro;
		modelEsquina3 = glm::translate(modelEsquina3, glm::vec3(44.0f, 4.5f, 1.5f));
		modelEsquina3 = glm::rotate(modelEsquina3, glm::radians(mainWindow.getesquina3()), glm::vec3(0.0f, 1.0f, 0.0f));
		color = glm::vec3(0.6f, 0.6f, 0.9f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelEsquina3));
		Esquina3_M.RenderModel();

		// Esquina 4 (HIJA)
		glm::mat4 modelEsquina4 = modelHolocronCentro;
		modelEsquina4 = glm::translate(modelEsquina4, glm::vec3(52.0f, 4.5f, 1.5f));
		modelEsquina4 = glm::rotate(modelEsquina4, glm::radians(mainWindow.getesquina4()), glm::vec3(0.0f, 1.0f, 0.0f));
		color = glm::vec3(0.6f, 0.6f, 0.9f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelEsquina4));
		Esquina4_M.RenderModel();

		// Esquina 5 (HIJA)
		glm::mat4 modelEsquina5 = modelHolocronCentro;
		modelEsquina5 = glm::translate(modelEsquina5, glm::vec3(44.0f, -3.0f, 1.5f));
		modelEsquina5 = glm::rotate(modelEsquina5, glm::radians(mainWindow.getesquina5()), glm::vec3(0.0f, 1.0f, 0.0f));
		color = glm::vec3(0.6f, 0.6f, 0.9f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelEsquina5));
		Esquina5_M.RenderModel();

		// Esquina 6 (HIJA)
		glm::mat4 modelEsquina6 = modelHolocronCentro;
		modelEsquina6 = glm::translate(modelEsquina6, glm::vec3(44.0f, -3.0f, 9.2f));
		modelEsquina6 = glm::rotate(modelEsquina6, glm::radians(mainWindow.getesquina6()), glm::vec3(0.0f, 1.0f, 0.0f));
		color = glm::vec3(0.6f, 0.6f, 0.9f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelEsquina6));
		Esquina6_M.RenderModel();

		// Esquina 7 (HIJA)
		glm::mat4 modelEsquina7 = modelHolocronCentro;
		modelEsquina7 = glm::translate(modelEsquina7, glm::vec3(52.0f, -3.0f, 9.2f));
		modelEsquina7 = glm::rotate(modelEsquina7, glm::radians(mainWindow.getesquina7()), glm::vec3(0.0f, 1.0f, 0.0f));
		color = glm::vec3(0.6f, 0.6f, 0.9f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelEsquina7));
		Esquina7_M.RenderModel();

		// Esquina 8 (HIJA)
		glm::mat4 modelEsquina8 = modelHolocronCentro;
		modelEsquina8 = glm::translate(modelEsquina8, glm::vec3(52.0f, -3.0f, 1.5f));
		modelEsquina8 = glm::rotate(modelEsquina8, glm::radians(mainWindow.getesquina8()), glm::vec3(0.0f, 1.0f, 0.0f));
		color = glm::vec3(0.6f, 0.6f, 0.9f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelEsquina8));
		Esquina8_M.RenderModel();

		//====================== SATÉLITE ======================

// Base del satélite (PADRE DE TODO)
		glm::mat4 modelSatelite = glm::mat4(1.0);
		modelSatelite = glm::translate(modelSatelite, glm::vec3(-15.0f, -2.0f, -3.0f)); //AJUSTA POSICIÓN EN ESCENA
		modelSatelite = glm::rotate(modelSatelite, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		modelSatelite = glm::rotate(modelSatelite, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
		modelSatelite = glm::rotate(modelSatelite, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
		color = glm::vec3(0.7f, 0.7f, 0.7f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelSatelite));
		BaseSatelite_M.RenderModel();

		// Panel derecho (HIJO de la base)

		glm::mat4 modelPanelDer = modelSatelite;
		modelPanelDer = glm::translate(modelPanelDer, glm::vec3(-5.5f, 15.7f, 13.0f)); //AJUSTA OFFSET AL PUNTO DE ANCLAJE
		modelPanelDer = glm::rotate(modelPanelDer, glm::radians(mainWindow.getpanelDerecho()), glm::vec3(0.0f, 1.0f, 0.0f)); //AJUSTA EJE DE GIRO
		color = glm::vec3(0.2f, 0.3f, 0.8f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelPanelDer));
		PanelDerecho_M.RenderModel();

		// Panel izquierdo (HIJO de la base)
		glm::mat4 modelPanelIzq = modelSatelite;
		modelPanelIzq = glm::translate(modelPanelIzq, glm::vec3(-5.9f, 15.9f, 12.7f)); //AJUSTA OFFSET AL PUNTO DE ANCLAJE
		modelPanelIzq = glm::rotate(modelPanelIzq, glm::radians(mainWindow.getpanelIzquierdo()), glm::vec3(0.0f, 1.0f, 0.0f)); //AJUSTA EJE DE GIRO
		color = glm::vec3(0.2f, 0.3f, 0.8f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelPanelIzq));
		PanelIzquierdo_M.RenderModel();

		// Base de la antena (HIJO de la base) — sube/baja
		glm::mat4 modelBaseAntena = modelSatelite;
		modelBaseAntena = glm::translate(modelBaseAntena, glm::vec3(0.0f, 0.0f, 0.0)); //AJUSTA OFFSET AL PUNTO DE ANCLAJE
		modelBaseAntena = glm::rotate(modelBaseAntena, glm::radians(mainWindow.getbaseAntena()), glm::vec3(1.0f, 0.0f, 0.0f)); //AJUSTA EJE DE GIRO
		color = glm::vec3(0.4f, 0.4f, 0.4f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelBaseAntena));
		BaseAntena_M.RenderModel();

		// Antena (HIJA de la base de la antena) — hereda su rotación, NO tiene rotación propia
		glm::mat4 modelAntena = modelBaseAntena;
		modelAntena = glm::translate(modelAntena, glm::vec3(-5.5f, 16.3f, 12.5f));
		color = glm::vec3(0.6f, 0.6f, 0.6f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelAntena));
		Antena_M.RenderModel();
		
		
		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}