//Práctica 2: índices, mesh, proyecciones, transformaciones geométricas
#include <stdio.h>
#include <string.h>
#include<cmath>
#include<vector>
#include <glew.h>
#include <glfw3.h>
//glm
#include<glm.hpp>
#include<gtc\matrix_transform.hpp>
#include<gtc\type_ptr.hpp>
//clases para dar orden y limpieza al código
#include"Mesh.h"
#include"Shader.h"
#include"Window.h"
//Dimensiones de la ventana
const float toRadians = 3.14159265f/180.0; //grados a radianes
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<MeshColor*> meshColorList;
std::vector<Shader>shaderList;
//Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
static const char* vShaderColor = "shaders/shadercolor.vert";
static const char* fShaderColor = "shaders/shadercolor.frag";

//shaders nuevos se crearían acá
static const char* vShaderrojo = "shaders/shaderrojo.vert";
static const char* vShaderverde = "shaders/shaderverde.vert";
static const char* vShaderazul = "shaders/shaderazul.vert";
static const char* vShadercafe = "shaders/shadercafe.vert";
static const char* vShadermagenta = "shaders/shadermagenta.vert";
static const char* vShaderamarillo = "shaders/shaderamarillo.vert";
static const char* vShadernegro = "shaders/shadernegro.vert";

float angulo = 0.0f;

//color café en RGB : 0.478, 0.255, 0.067

//Pirámide triangular regular
void CreaPiramide()
{
	unsigned int indices[] = { 
		0,1,2,
		1,3,2,
		3,0,2,
		1,0,3
		
	};
	GLfloat vertices[] = {
		-0.5f, -0.5f,0.0f,	//0
		0.5f,-0.5f,0.0f,	//1
		0.0f,0.5f, -0.25f,	//2
		0.0f,-0.5f,-0.5f,	//3

	};
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(obj1);
}

//Vértices de un cubo
void CrearCubo()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
	};

GLfloat cubo_vertices[] = {
	// front
	-0.5f, -0.5f,  0.5f,
	0.5f, -0.5f,  0.5f,
	0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	// back
	-0.5f, -0.5f, -0.5f,
	0.5f, -0.5f, -0.5f,
	0.5f,  0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f
};
Mesh *cubo = new Mesh();
cubo->CreateMesh(cubo_vertices, cubo_indices,24, 36);
meshList.push_back(cubo);
}

void CrearLetrasyFiguras()
{
	/*GLfloat vertices_letras[] = {
	
	//J
			-0.9f, 0.5f, 0.0f, 0.47f, 1.0f, 0.9f,
			-0.9f, 0.35f, 0.0f, 0.47f, 1.0f, 0.9f,
			-0.4f, 0.35f, 0.0f, 0.47f, 1.0f, 0.9f,

			-0.9f, 0.5f, 0.0f, 0.47f, 1.0f, 0.9f,
			-0.4f, 0.5f, 0.0f, 0.47f, 1.0f, 0.9f,
			-0.4f, 0.35f, 0.0f, 0.47f, 1.0f, 0.9f,

			-0.65f, 0.35f, 0.0f, 0.47f, 1.0f, 0.9f,
			-0.5f, 0.35f, 0.0f, 0.47f, 1.0f, 0.9f,
			-0.5f, -0.35f, 0.0f, 0.47f, 1.0f, 0.9f,

			-0.65f, 0.35f, 0.0f, 0.47f, 1.0f, 0.9f,
			-0.65f, -0.35f, 0.0f, 0.47f, 1.0f, 0.9f,
			-0.5f, -0.35f, 0.0f, 0.47f, 1.0f, 0.9f,

			-0.5f, -0.35f, 0.0f, 0.47f, 1.0f, 0.9f,
			-0.6f, -0.5f, 0.0f, 0.47f, 1.0f, 0.9f,
			-0.6f, -0.35f, 0.0f, 0.47f, 1.0f, 0.9f,

			-0.8f, -0.35f, 0.0f, 0.47f, 1.0f, 0.9f,
			-0.6f, -0.5f, 0.0f, 0.47f, 1.0f, 0.9f,
			-0.6f, -0.35f, 0.0f, 0.47f, 1.0f, 0.9f,

			-0.8f, -0.35f, 0.0f, 0.47f, 1.0f, 0.9f,
			-0.6f, -0.5f, 0.0f, 0.47f, 1.0f, 0.9f,
			-0.8f, -0.5f, 0.0f, 0.47f, 1.0f, 0.9f,

			-0.8f, -0.35f, 0.0f, 0.47f, 1.0f, 0.9f,
			-0.9f, -0.35f, 0.0f, 0.47f, 1.0f, 0.9f,
			-0.8f, -0.5f, 0.0f, 0.47f, 1.0f, 0.9f,

			-0.75f, -0.35f, 0.0f, 0.47f, 1.0f, 0.9f,
			-0.9f, -0.35f, 0.0f, 0.47f, 1.0f, 0.9f,
			-0.75f, -0.2f, 0.0f, 0.47f, 1.0f, 0.9f,

			-0.9f, -0.2f, 0.0f, 0.47f, 1.0f, 0.9f,
			-0.9f, -0.35f, 0.0f, 0.47f, 1.0f, 0.9f,
			-0.75f, -0.2f, 0.0f, 0.47f, 1.0f, 0.9f,


			//P
			-0.3f, 0.5f, 0.0f, 0.569f, 0.047f, 0.047f,
			-0.3f, -0.5f, 0.0f, 0.569f, 0.047f, 0.047f,
			-0.15f, -0.5f, 0.0f, 0.569f, 0.047f, 0.047f,

			-0.15f, 0.5f, 0.0f, 0.569f, 0.047f, 0.047f,
			-0.3f, 0.5f, 0.0f, 0.569f, 0.047f, 0.047f,
			-0.15f, -0.5f, 0.0f, 0.569f, 0.047f, 0.047f,

			-0.15f, 0.35f, 0.0f, 0.569f, 0.047f, 0.047f,
			-0.05f, 0.35f, 0.0f, 0.569f, 0.047f, 0.047f,
			-0.05f, 0.45f, 0.0f, 0.569f, 0.047f, 0.047f,

			0.15f, 0.45f, 0.0f, 0.569f, 0.047f, 0.047f,
			-0.05f, 0.35f, 0.0f, 0.569f, 0.047f, 0.047f,
			-0.05f, 0.45f, 0.0f, 0.569f, 0.047f, 0.047f,

			0.15f, 0.45f, 0.0f, 0.569f, 0.047f, 0.047f,
			-0.05f, 0.35f, 0.0f, 0.569f, 0.047f, 0.047f,
			0.15f, 0.35f, 0.0f, 0.569f, 0.047f, 0.047f,

			0.15f, 0.45f, 0.0f, 0.569f, 0.047f, 0.047f,
			0.2f, 0.35f, 0.0f, 0.569f, 0.047f, 0.047f,
			0.15f, 0.35f, 0.0f, 0.569f, 0.047f, 0.047f,

			0.1f, 0.35f, 0.0f, 0.569f, 0.047f, 0.047f,
			0.1f, 0.0f, 0.0f, 0.569f, 0.047f, 0.047f,
			0.2f, 0.35f, 0.0f, 0.569f, 0.047f, 0.047f,

			0.2f, 0.0f, 0.0f, 0.569f, 0.047f, 0.047f,
			0.1f, 0.0f, 0.0f, 0.569f, 0.047f, 0.047f,
			0.2f, 0.35f, 0.0f, 0.569f, 0.047f, 0.047f,

			0.2f, 0.0f, 0.0f, 0.569f, 0.047f, 0.047f,
			0.1f, 0.0f, 0.0f, 0.569f, 0.047f, 0.047f,
			0.15f, -0.1f, 0.0f, 0.569f, 0.047f, 0.047f,

			0.1f, -0.1f, 0.0f, 0.569f, 0.047f, 0.047f,
			0.1f, 0.0f, 0.0f, 0.569f, 0.047f, 0.047f,
			0.15f, -0.1f, 0.0f, 0.569f, 0.047f, 0.047f,

			0.1f, -0.1f, 0.0f, 0.569f, 0.047f, 0.047f,
			0.1f, 0.0f, 0.0f, 0.569f, 0.047f, 0.047f,
			-0.1f, 0.0f, 0.0f, 0.569f, 0.047f, 0.047f,

			-0.1f, -0.1f, 0.0f, 0.569f, 0.047f, 0.047f,
			-0.1f, 0.0f, 0.0f, 0.569f, 0.047f, 0.047f,
			0.1f, -0.1f, 0.0f, 0.569f, 0.047f, 0.047f,

			-0.15f, 0.0f, 0.0f, 0.569f, 0.047f, 0.047f,
			-0.1f, 0.0f, 0.0f, 0.569f, 0.047f, 0.047f,
			-0.1f, -0.1f, 0.0f, 0.569f, 0.047f, 0.047f,

			-0.05f, 0.0f, 0.0f, 0.569f, 0.047f, 0.047f,
			-0.15f, 0.0f, 0.0f, 0.569f, 0.047f, 0.047f,
			-0.15f, 0.35f, 0.0f, 0.569f, 0.047f, 0.047f,

			-0.05f, 0.0f, 0.0f, 0.569f, 0.047f, 0.047f,
			-0.05f, 0.35f, 0.0f, 0.569f, 0.047f, 0.047f,
			-0.15f, 0.35f, 0.0f, 0.569f, 0.047f, 0.047f,


			//R
			0.3f, 0.5f, 0.0f, 0.588f, 0.071f, 0.588f,
			0.3f, -0.5f, 0.0f, 0.588f, 0.071f, 0.588f,
			0.45f, -0.5f, 0.0f, 0.588f, 0.071f, 0.588f,

			0.45f, 0.5f, 0.0f, 0.588f, 0.071f, 0.588f,
			0.3f, 0.5f, 0.0f, 0.588f, 0.071f, 0.588f,
			0.45f, -0.5f, 0.0f, 0.588f, 0.071f, 0.588f,

			0.45f, 0.35f, 0.0f, 0.588f, 0.071f, 0.588f,
			0.55f, 0.35f, 0.0f, 0.588f, 0.071f, 0.588f,
			0.55f, 0.45f, 0.0f, 0.588f, 0.071f, 0.588f,

			0.75f, 0.45f, 0.0f, 0.588f, 0.071f, 0.588f,
			0.55f, 0.35f, 0.0f, 0.588f, 0.071f, 0.588f,
			0.55f, 0.45f, 0.0f, 0.588f, 0.071f, 0.588f,

			0.75f, 0.45f, 0.0f, 0.588f, 0.071f, 0.588f,
			0.55f, 0.35f, 0.0f, 0.588f, 0.071f, 0.588f,
			0.75f, 0.35f, 0.0f, 0.588f, 0.071f, 0.588f,

			0.75f, 0.45f, 0.0f, 0.588f, 0.071f, 0.588f,
			0.8f, 0.35f, 0.0f, 0.588f, 0.071f, 0.588f,
			0.75f, 0.35f, 0.0f, 0.588f, 0.071f, 0.588f,

			0.7f, 0.35f, 0.0f, 0.588f, 0.071f, 0.588f,
			0.7f, 0.0f, 0.0f, 0.588f, 0.071f, 0.588f,
			0.8f, 0.35f, 0.0f, 0.588f, 0.071f, 0.588f,

			0.8f, 0.0f, 0.0f, 0.588f, 0.071f, 0.588f,
			0.7f, 0.0f, 0.0f, 0.588f, 0.071f, 0.588f,
			0.8f, 0.35f, 0.0f, 0.588f, 0.071f, 0.588f,

			0.8f, 0.0f, 0.0f, 0.588f, 0.071f, 0.588f,
			0.7f, 0.0f, 0.0f, 0.588f, 0.071f, 0.588f,
			0.75f, -0.1f, 0.0f, 0.588f, 0.071f, 0.588f,

			0.7f, -0.1f, 0.0f, 0.588f, 0.071f, 0.588f,
			0.7f, 0.0f, 0.0f, 0.588f, 0.071f, 0.588f,
			0.75f, -0.1f, 0.0f, 0.588f, 0.071f, 0.588f,

			0.7f, -0.1f, 0.0f, 0.588f, 0.071f, 0.588f,
			0.7f, 0.0f, 0.0f, 0.588f, 0.071f, 0.588f,
			0.5f, 0.0f, 0.0f, 0.588f, 0.071f, 0.588f,

			0.5f, -0.1f, 0.0f, 0.588f, 0.071f, 0.588f,
			0.5f, 0.0f, 0.0f, 0.588f, 0.071f, 0.588f,
			0.7f, -0.1f, 0.0f, 0.588f, 0.071f, 0.588f,

			0.45f, 0.0f, 0.0f, 0.588f, 0.071f, 0.588f,
			0.5f, 0.0f, 0.0f, 0.588f, 0.071f, 0.588f,
			0.5f, -0.1f, 0.0f, 0.588f, 0.071f, 0.588f,

			0.55f, 0.0f, 0.0f, 0.588f, 0.071f, 0.588f,
			0.45f, 0.0f, 0.0f, 0.588f, 0.071f, 0.588f,
			0.45f, 0.35f, 0.0f, 0.588f, 0.071f, 0.588f,

			0.55f, 0.0f, 0.0f, 0.588f, 0.071f, 0.588f,
			0.55f, 0.35f, 0.0f, 0.588f, 0.071f, 0.588f,
			0.45f, 0.35f, 0.0f, 0.588f, 0.071f, 0.588f,

			0.5f, -0.1f, 0.0f, 0.588f, 0.071f, 0.588f,
			0.65f, -0.5f, 0.0f, 0.588f, 0.071f, 0.588f,
			0.8f, -0.5f, 0.0f, 0.588f, 0.071f, 0.588f,

			0.65f, -0.1f, 0.0f, 0.588f, 0.071f, 0.588f,
			0.5f, -0.1f, 0.0f, 0.588f, 0.071f, 0.588f,
			0.8f, -0.5f, 0.0f, 0.588f, 0.071f, 0.588f,

			0.45f, -0.1f, 0.0f, 0.588f, 0.071f, 0.588f,
			0.45f, 0.0f, 0.0f, 0.588f, 0.071f, 0.588f,
			0.5f, -0.1f, 0.0f, 0.588f, 0.071f, 0.588f,
	};
	MeshColor *letras = new MeshColor();
	letras->CreateMeshColor(vertices_letras,800);
	meshColorList.push_back(letras);
	*/

		GLfloat vertices_triangulorojo[] = {
			// X       Y       Z       R       G       B
			-1.0f,   -1.0f,   0.5f,   1.0f,   0.0f,   0.0f,
			 1.0f,   -1.0f,   0.5f,   1.0f,   0.0f,   0.0f,
			 0.0f,    1.0f,   0.5f,   1.0f,   0.0f,   0.0f
		};

		MeshColor* triangulorojo = new MeshColor();
		triangulorojo->CreateMeshColor(vertices_triangulorojo, 18);
		meshColorList.push_back(triangulorojo);
		


		GLfloat vertices_cuadradoverde[] = {
			// X       Y       Z       R       G       B
			-0.5f,   -0.5f,   0.5f,   0.0f,   1.0f,   0.0f,
			 0.5f,   -0.5f,   0.5f,   0.0f,   1.0f,   0.0f,
			 0.5f,    0.5f,   0.5f,   0.0f,   1.0f,   0.0f,

			-0.5f,   -0.5f,   0.5f,   0.0f,   1.0f,   0.0f,
			 0.5f,    0.5f,   0.5f,   0.0f,   1.0f,   0.0f,
			-0.5f,    0.5f,   0.5f,   0.0f,   1.0f,   0.0f
		};

		MeshColor* cuadradoverde = new MeshColor();
		cuadradoverde->CreateMeshColor(vertices_cuadradoverde, 36);
		meshColorList.push_back(cuadradoverde);
		

//tamarillo
		GLfloat vertices_trianguloamarillo[] = {
			// X       Y       Z       R       G       B
			-1.0f,   -1.0f,   0.5f,   1.0f,   1.0f,   0.0f,
			 1.0f,   -1.0f,   0.5f,   1.0f,   1.0f,   0.0f,
			 0.0f,    1.0f,   0.5f,   1.0f,   1.0f,   0.0f
		};

		MeshColor* trianguloamarillo = new MeshColor();
		trianguloamarillo->CreateMeshColor(vertices_trianguloamarillo, 18);
		meshColorList.push_back(trianguloamarillo);

//tverde
		GLfloat vertices_trianguloverde[] = {
			// X       Y       Z       R       G       B
			-1.0f,   -1.0f,   0.5f,   0.0f,   1.0f,   0.0f,
			 1.0f,   -1.0f,   0.5f,   0.0f,   1.0f,   0.0f,
			 0.0f,    1.0f,   0.5f,   0.0f,   1.0f,   0.0f
		};

		MeshColor* trianguloverde = new MeshColor();
		trianguloverde->CreateMeshColor(vertices_trianguloverde, 18);
		meshColorList.push_back(trianguloverde);


//tmorado
		GLfloat vertices_triangulomorado[] = {
			// X       Y       Z       R       G       B
			-1.0f,   -1.0f,   0.5f,   1.0f,   0.0f,   1.0f,
			 1.0f,   -1.0f,   0.5f,   1.0f,   0.0f,   1.0f,
			 0.0f,    1.0f,   0.5f,   1.0f,   0.0f,   1.0f
		};

		MeshColor* triangulomorado = new MeshColor();
		triangulomorado->CreateMeshColor(vertices_triangulomorado, 18);
		meshColorList.push_back(triangulomorado);

//ccafe
		GLfloat vertices_cuadradocafe[] = {
			// X       Y       Z       R          G          B
			-0.5f,   -0.5f,   0.5f,   0.478f,    0.255f,    0.067f,
			 0.5f,   -0.5f,   0.5f,   0.478f,    0.255f,    0.067f,
			 0.5f,    0.5f,   0.5f,   0.478f,    0.255f,    0.067f,

			-0.5f,   -0.5f,   0.5f,   0.478f,    0.255f,    0.067f,
			 0.5f,    0.5f,   0.5f,   0.478f,    0.255f,    0.067f,
			-0.5f,    0.5f,   0.5f,   0.478f,    0.255f,    0.067f
		};

		MeshColor* cuadradocafe = new MeshColor();
		cuadradocafe->CreateMeshColor(vertices_cuadradocafe, 36);
		meshColorList.push_back(cuadradocafe);


//cazul
		GLfloat vertices_cuadradoazul[] = {
			// X       Y       Z       R       G       B
			-0.5f,   -0.5f,   0.5f,   0.0f,   0.0f,   1.0f,
			 0.5f,   -0.5f,   0.5f,   0.0f,   0.0f,   1.0f,
			 0.5f,    0.5f,   0.5f,   0.0f,   0.0f,   1.0f,

			-0.5f,   -0.5f,   0.5f,   0.0f,   0.0f,   1.0f,
			 0.5f,    0.5f,   0.5f,   0.0f,   0.0f,   1.0f,
			-0.5f,    0.5f,   0.5f,   0.0f,   0.0f,   1.0f
		};

		MeshColor* cuadradoazul = new MeshColor();
		cuadradoazul->CreateMeshColor(vertices_cuadradoazul, 36);
		meshColorList.push_back(cuadradoazul);

//cnegro
		GLfloat vertices_cuadradonegro[] = {
			// X       Y       Z       R       G       B
			-0.5f,   -0.5f,   0.5f,   0.0f,   0.0f,   0.0f,
			 0.5f,   -0.5f,   0.5f,   0.0f,   0.0f,   0.0f,
			 0.5f,    0.5f,   0.5f,   0.0f,   0.0f,   0.0f,

			-0.5f,   -0.5f,   0.5f,   0.0f,   0.0f,   0.0f,
			 0.5f,    0.5f,   0.5f,   0.0f,   0.0f,   0.0f,
			-0.5f,    0.5f,   0.5f,   0.0f,   0.0f,   0.0f
		};

		MeshColor* cuadradonegro = new MeshColor();
		cuadradonegro->CreateMeshColor(vertices_cuadradonegro, 36);
		meshColorList.push_back(cuadradonegro);

	}


void CreateShaders()
{

	Shader* shader1 = new Shader(); //shader para usar índices: objetos: cubo y  pirámide
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
	
	Shader *shader2 = new Shader();//shader para usar color como parte del VAO: letras 
	shader2->CreateFromFiles(vShaderColor, fShaderColor);
	shaderList.push_back(*shader2);

	Shader* shaderrojo = new Shader();
	shaderrojo->CreateFromFiles(vShaderrojo, fShaderColor);
	shaderList.push_back(*shaderrojo);

	Shader* shaderverde = new Shader();
	shaderverde->CreateFromFiles(vShaderverde, fShaderColor);
	shaderList.push_back(*shaderverde);

	Shader* shaderazul = new Shader();
	shaderazul->CreateFromFiles(vShaderazul, fShaderColor);
	shaderList.push_back(*shaderazul);

	Shader* shadercafe = new Shader();
	shadercafe->CreateFromFiles(vShadercafe, fShaderColor);
	shaderList.push_back(*shadercafe);

	Shader* shadermagenta = new Shader();
	shadermagenta->CreateFromFiles(vShadermagenta, fShaderColor);
	shaderList.push_back(*shadermagenta);

	Shader* shaderamarillo = new Shader();
	shaderamarillo->CreateFromFiles(vShaderamarillo, fShaderColor);
	shaderList.push_back(*shaderamarillo);

	Shader* shadernegro = new Shader();
	shadernegro->CreateFromFiles(vShadernegro, fShaderColor);
	shaderList.push_back(*shadernegro);
}


int main()
{
	mainWindow = Window(800, 600);
	mainWindow.Initialise();
	CreaPiramide(); //índice 0 en MeshList
	CrearCubo();//índice 1 en MeshList
	CrearLetrasyFiguras(); //usa MeshColor, índices en MeshColorList
	CreateShaders();
	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	//Projection: Matriz de Dimensión 4x4 para indicar si vemos en 2D( orthogonal) o en 3D) perspectiva
	glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
	//glm::mat4 projection = glm::perspective(glm::radians(60.0f)	,mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);
	
	//Model: Matriz de Dimensión 4x4 en la cual se almacena la multiplicación de las transformaciones geométricas.
	glm::mat4 model(1.0); //fuera del while se usa para inicializar la matriz con una identidad
	
	//Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		//Recibir eventos del usuario
		glfwPollEvents();
		//Limpiar la ventana
		glClearColor(0.8f,0.8f,0.8f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad
		
													
		//Para las letras hay que usar el segundo set de shaders con índice 1 en ShaderList 
		shaderList[1].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();
		//Para las figuras usamos el segundo set de shaders
		shaderList[1].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();

		glUniformMatrix4fv(
			uniformProjection,
			1,
			GL_FALSE,
			glm::value_ptr(projection)
		);
/*
//base negra
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -0.26f, -3.0f));
		model = glm::scale(model, glm::vec3(1.95f, 0.08f, 1.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		meshColorList[7]->RenderMeshColor();


//ESTRUCTURA IZQUIERDA
		//t amarillo
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.65f, 0.55f, -3.0f));
		model = glm::rotate(model, 180 * toRadians,
			glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.15f, 0.15f, 1.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		meshColorList[2]->RenderMeshColor();


		//t rojo
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.65f, 0.25f, -3.0f));
		model = glm::rotate(model, 180 * toRadians,
			glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.15f, 0.15f, 1.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		meshColorList[0]->RenderMeshColor();


		//t verde
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.65f, -0.05f, -3.0f));
		model = glm::rotate(model, 180 * toRadians,
			glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.15f, 0.15f, 1.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		meshColorList[3]->RenderMeshColor();

		//barras cafes

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.83f, 0.3f, -3.5f));
		model = glm::scale(model, glm::vec3(0.05f, 1.1f, 1.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshColorList[5]->RenderMeshColor();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.47f, 0.3f, -3.5f));
		model = glm::scale(model, glm::vec3(0.05f, 1.1f, 1.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshColorList[5]->RenderMeshColor();


//ESTRUCTURA CENTRAL

		//romboazul
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.1f, -15.0f));
		model = glm::rotate(model, 45 * toRadians,
			glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.45f, 0.45f, 1.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		meshColorList[6]->RenderMeshColor();


		//rombo cafe
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.1f, -3.0f));
		model = glm::rotate(model, 45 * toRadians,
			glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.20f, 0.20f, 1.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		meshColorList[5]->RenderMeshColor();

		//t amarillo
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.24f, 0.34f, -3.2f));
		model = glm::rotate(model, 45 * toRadians,
			glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.22f, 0.12f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		meshColorList[2]->RenderMeshColor();


		//t rojo
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.24f, 0.34f, -3.2f));
		model = glm::rotate(model, 315 * toRadians,
			glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.22f, 0.12f, 1.0f));
		

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		meshColorList[0]->RenderMeshColor();


		//t morado
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.24f, -0.14f, -3.2f));
		model = glm::rotate(model, 135 * toRadians,
			glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.22f, 0.12f, 1.0f));


		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		meshColorList[4]->RenderMeshColor();


		//t verde
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.24f, -0.14f, -3.2f));
		model = glm::rotate(model, 225 * toRadians,
			glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.22f, 0.12f, 1.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		meshColorList[3]->RenderMeshColor();

//ESTRUCTURA DE LA DERECHA

		//t morado
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.65f, 0.23f, -3.0f));
		model = glm::scale(model, glm::vec3(0.15f, 0.15f, 1.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		meshColorList[4]->RenderMeshColor();


		//t verde
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.50f, -0.07f, -3.0f));
		model = glm::scale(model, glm::vec3(0.15f, 0.15f, 1.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		meshColorList[3]->RenderMeshColor();


		//t rojo
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.80f, -0.07f, -3.0f));
		model = glm::scale(model, glm::vec3(0.15f, 0.15f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshColorList[0]->RenderMeshColor();


		//t amarillo
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.65f, -0.07f, -3.1f));
		model = glm::rotate(model, 180 * toRadians,
			glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.15f, 0.15f, 1.0f));
		glUniformMatrix4fv(uniformModel,1,GL_FALSE,glm::value_ptr(model));
		meshColorList[2]->RenderMeshColor();
		

		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
		
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[0]->RenderMeshColor();
	
		//Para el cubo y la pirámide se usa el primer set de shaders con índice 0 en ShaderList
		shaderList[0].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		angulo += 0.01;
		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();
		
		*/	
		//base negra - CUBO

		shaderList[8].useShader();

		uniformModel = shaderList[8].getModelLocation();
		uniformProjection = shaderList[8].getProjectLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE,
			glm::value_ptr(projection));


		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -0.26f, -3.0f));
		model = glm::scale(model, glm::vec3(1.95f, 0.08f, 1.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE,
			glm::value_ptr(model));

		meshList[1]->RenderMesh();



		// ESTRUCTURA IZQUIERDA


		//t amarillo - PIRAMIDE
		shaderList[7].useShader();

		uniformModel = shaderList[6].getModelLocation();
		uniformProjection = shaderList[6].getProjectLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE,
			glm::value_ptr(projection));

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.65f, 0.55f, -3.0f));
		model = glm::rotate(model, 180 * toRadians,
			glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 1.15f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE,
			glm::value_ptr(model));

		meshList[0]->RenderMesh();


		//t rojo - PIRAMIDE
		shaderList[2].useShader();

		uniformModel = shaderList[2].getModelLocation();
		uniformProjection = shaderList[2].getProjectLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE,
			glm::value_ptr(projection));

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.65f, 0.25f, -3.0f));
		model = glm::rotate(model, 180 * toRadians,
			glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 1.15f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE,
			glm::value_ptr(model));

		meshList[0]->RenderMesh();


		//t verde - PIRAMIDE
		shaderList[3].useShader();

		uniformModel = shaderList[3].getModelLocation();
		uniformProjection = shaderList[3].getProjectLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE,
			glm::value_ptr(projection));

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.65f, -0.05f, -3.0f));
		model = glm::rotate(model, 180 * toRadians,
			glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 1.15f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE,
			glm::value_ptr(model));

		meshList[0]->RenderMesh();


		//barras cafes - CUBOS

		//barra izquierda
		shaderList[5].useShader();

		uniformModel = shaderList[5].getModelLocation();
		uniformProjection = shaderList[5].getProjectLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE,
			glm::value_ptr(projection));

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.83f, 0.3f, -3.5f));
		model = glm::scale(model, glm::vec3(0.05f, 1.1f, 1.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE,
			glm::value_ptr(model));

		meshList[1]->RenderMesh();


		//barra derecha
		shaderList[5].useShader();

		uniformModel = shaderList[5].getModelLocation();
		uniformProjection = shaderList[5].getProjectLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE,
			glm::value_ptr(projection));

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.47f, 0.3f, -3.5f));
		model = glm::scale(model, glm::vec3(0.05f, 1.1f, 1.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE,
			glm::value_ptr(model));

		meshList[1]->RenderMesh();



		// ESTRUCTURA CENTRAL


		//rombo azul - CUBO
		shaderList[4].useShader();

		uniformModel = shaderList[4].getModelLocation();
		uniformProjection = shaderList[4].getProjectLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE,
			glm::value_ptr(projection));

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.1f, -15.0f));
		model = glm::rotate(model, 45 * toRadians,
			glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.45f, 0.45f, 1.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE,
			glm::value_ptr(model));

		meshList[1]->RenderMesh();


		//rombo cafe - CUBO
		shaderList[5].useShader();

		uniformModel = shaderList[5].getModelLocation();
		uniformProjection = shaderList[5].getProjectLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE,
			glm::value_ptr(projection));

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.1f, -3.0f));
		model = glm::rotate(model, 45 * toRadians,
			glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.20f, 0.20f, 1.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE,
			glm::value_ptr(model));

		meshList[1]->RenderMesh();


		//t amarillo - PIRAMIDE
		shaderList[7].useShader();

		uniformModel = shaderList[6].getModelLocation();
		uniformProjection = shaderList[6].getProjectLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE,
			glm::value_ptr(projection));

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.24f, 0.34f, -3.2f));
		model = glm::rotate(model, 45 * toRadians,
			glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.45f, 0.23f, 1.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE,
			glm::value_ptr(model));

		meshList[0]->RenderMesh();


		//t rojo - PIRAMIDE
		shaderList[2].useShader();

		uniformModel = shaderList[2].getModelLocation();
		uniformProjection = shaderList[2].getProjectLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE,
			glm::value_ptr(projection));

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.24f, 0.34f, -3.2f));
		model = glm::rotate(model, 315 * toRadians,
			glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.46f, 0.23f, 1.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE,
			glm::value_ptr(model));

		meshList[0]->RenderMesh();


		//t morado - PIRAMIDE
		shaderList[6].useShader();

		uniformModel = shaderList[7].getModelLocation();
		uniformProjection = shaderList[7].getProjectLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE,
			glm::value_ptr(projection));

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.24f, -0.14f, -3.2f));
		model = glm::rotate(model, 135 * toRadians,
			glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.44f, 0.23f, 1.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE,
			glm::value_ptr(model));

		meshList[0]->RenderMesh();


		//t verde - PIRAMIDE
		shaderList[3].useShader();

		uniformModel = shaderList[3].getModelLocation();
		uniformProjection = shaderList[3].getProjectLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE,
			glm::value_ptr(projection));

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.24f, -0.14f, -3.2f));
		model = glm::rotate(model, 225 * toRadians,
			glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.44f, 0.22f, 1.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE,
			glm::value_ptr(model));

		meshList[0]->RenderMesh();



		// ESTRUCTURA DE LA DERECHA


		//t morado - PIRAMIDE
		shaderList[6].useShader();

		uniformModel = shaderList[7].getModelLocation();
		uniformProjection = shaderList[7].getProjectLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE,
			glm::value_ptr(projection));

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.65f, 0.23f, -3.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 1.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE,
			glm::value_ptr(model));

		meshList[0]->RenderMesh();


		//t verde - PIRAMIDE
		shaderList[3].useShader();

		uniformModel = shaderList[3].getModelLocation();
		uniformProjection = shaderList[3].getProjectLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE,
			glm::value_ptr(projection));

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.50f, -0.07f, -3.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 1.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE,
			glm::value_ptr(model));

		meshList[0]->RenderMesh();


		//t rojo - PIRAMIDE
		shaderList[2].useShader();

		uniformModel = shaderList[2].getModelLocation();
		uniformProjection = shaderList[2].getProjectLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE,
			glm::value_ptr(projection));

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.80f, -0.07f, -3.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 1.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE,
			glm::value_ptr(model));

		meshList[0]->RenderMesh();


		//t amarillo - PIRAMIDE
		shaderList[7].useShader();

		uniformModel = shaderList[6].getModelLocation();
		uniformProjection = shaderList[6].getProjectLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE,
			glm::value_ptr(projection));

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.65f, -0.07f, -3.1f));
		model = glm::rotate(model, 180 * toRadians,
			glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 1.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE,
			glm::value_ptr(model));

		meshList[0]->RenderMesh();

		glUseProgram(0);
		mainWindow.swapBuffers();

	}
	return 0;
}


// inicializar matriz: glm::mat4 model(1.0);
// reestablecer matriz: model = glm::mat4(1.0);
//Traslación
//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
//////////////// ROTACIÓN //////////////////
//model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
////////////////  ESCALA ////////////////
//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
///////////////////// T+R////////////////
/*model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
*/
/////////////R+T//////////
/*model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
*/