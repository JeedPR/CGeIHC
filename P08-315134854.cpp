/*
Práctica 8: Iluminación y Jerarquías
*/
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
#include "Model.h"
#include "Skybox.h"

#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"

const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<MeshModel*> meshListModel;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;
Texture holocronTexture;
Texture dadoTexture;

Model Blackhawk_M;
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
Model Avion_M;
// Modelo de la nave espacial
Model NaveEspacial_M;

Skybox skybox;

Material Material_brillante;
Material Material_opaco;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;


// Variables de movimiento de la nave 
GLfloat naveMovY = 0.0f;

DirectionalLight mainLight;
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

static const char* vShader = "shaders/shader_light.vert";
static const char* fShader = "shaders/shader_light.frag";

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
            -1.0f, -1.0f, -0.6f,    0.0f, 0.0f,        0.0f, 0.0f, 0.0f,
            0.0f, -1.0f, 1.0f,        0.5f, 0.0f,        0.0f, 0.0f, 0.0f,
            1.0f, -1.0f, -0.6f,        1.0f, 0.0f,        0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f,        0.5f, 1.0f,        0.0f, 0.0f, 0.0f
    };

    unsigned int floorIndices[] = {
        0, 2, 1,
        1, 2, 3
    };

    GLfloat floorVertices[] = {
        -10.0f, 0.0f, -10.0f,    0.0f, 0.0f,        0.0f, -1.0f, 0.0f,
        10.0f, 0.0f, -10.0f,    10.0f, 0.0f,    0.0f, -1.0f, 0.0f,
        -10.0f, 0.0f, 10.0f,    0.0f, 10.0f,    0.0f, -1.0f, 0.0f,
        10.0f, 0.0f, 10.0f,        10.0f, 10.0f,    0.0f, -1.0f, 0.0f
    };

    unsigned int vegetacionIndices[] = {
       0, 1, 2,
       0, 2, 3,
       4, 5, 6,
       4, 6, 7
    };

    GLfloat vegetacionVertices[] = {
        -0.5f, -0.5f, 0.0f,        0.0f, 0.0f,        0.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.0f,        1.0f, 0.0f,        0.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.0f,        1.0f, 1.0f,        0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.0f,        0.0f, 1.0f,        0.0f, 0.0f, 0.0f,

        0.0f, -0.5f, -0.5f,        0.0f, 0.0f,        0.0f, 0.0f, 0.0f,
        0.0f, -0.5f, 0.5f,        1.0f, 0.0f,        0.0f, 0.0f, 0.0f,
        0.0f, 0.5f, 0.5f,        1.0f, 1.0f,        0.0f, 0.0f, 0.0f,
        0.0f, 0.5f, -0.5f,        0.0f, 1.0f,        0.0f, 0.0f, 0.0f,
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

    MeshModel* obj4 = new MeshModel();
    obj4->CreateMeshModel(vegetacionVertices, vegetacionIndices, 64, 12);
    meshListModel.push_back(obj4);

    calcAverageNormals(indices, 12, vertices, 32, 8, 5);
    calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);



}

void CreateShaders()
{
    Shader* shader1 = new Shader();
    shader1->CreateFromFiles(vShader, fShader);
    shaderList.push_back(*shader1);
}

void CrearDado()
{
    unsigned int cubo_indices[] = {
        0, 1, 2,  2, 3, 0,
        8, 9, 10, 10, 11, 8,
        12, 13, 14, 14, 15, 12,
        16, 17, 18, 18, 19, 16,
        20, 21, 22, 22, 23, 20,
        4, 5, 6,  6, 7, 4,
    };
    GLfloat cubo_vertices[] = {
        -0.5f, -0.5f,  0.5f,    0.25f,  0.333f,        0.0f,    0.0f,    -1.0f,
        0.5f,  -0.5f,  0.5f,    0.50f,  0.333f,        0.0f,    0.0f,    -1.0f,
        0.5f,   0.5f,  0.5f,    0.50f,  0.666f,        0.0f,    0.0f,    -1.0f,
        -0.5f,  0.5f,  0.5f,    0.25f,  0.666f,        0.0f,    0.0f,    -1.0f,
        0.5f, -0.5f,  0.5f,        0.5f,   0.333f,        -1.0f,    0.0f,    0.0f,
        0.5f, -0.5f,  -0.5f,    0.75f,    0.333f,        -1.0f,    0.0f,    0.0f,
        0.5f,  0.5f,  -0.5f,    0.75f,    0.666f,        -1.0f,    0.0f,    0.0f,
        0.5f,  0.5f,  0.5f,        0.5f,    0.666f,        -1.0f,    0.0f,    0.0f,
        -0.5f, -0.5f, -0.5f,    1.0f,  0.333f,        0.0f,    0.0f,    1.0f,
        0.5f, -0.5f, -0.5f,        0.75f,    0.333f,        0.0f,    0.0f,    1.0f,
        0.5f,  0.5f, -0.5f,        0.75f,    0.666f,        0.0f,    0.0f,    1.0f,
        -0.5f,  0.5f, -0.5f,    1.0f,    0.666f,        0.0f,    0.0f,    1.0f,
        -0.5f, -0.5f,  -0.5f,    0.0f,   0.333f,        1.0f,    0.0f,    0.0f,
        -0.5f, -0.5f,  0.5f,    0.25f,    0.333f,        1.0f,    0.0f,    0.0f,
        -0.5f,  0.5f,  0.5f,    0.25f,    0.666f,        1.0f,    0.0f,    0.0f,
        -0.5f,  0.5f,  -0.5f,    0.0f,    0.666f,        1.0f,    0.0f,    0.0f,
        -0.5f, -0.5f,  0.5f,    0.5f,   0.0f,        0.0f,    1.0f,    0.0f,
        0.5f,  -0.5f,  0.5f,    0.5f,    0.333f,        0.0f,    1.0f,    0.0f,
        0.5f,  -0.5f,  -0.5f,    0.75f,    0.333f,        0.0f,    1.0f,    0.0f,
        -0.5f, -0.5f,  -0.5f,    0.75f,    0.0f,        0.0f,    1.0f,    0.0f,
        -0.5f, 0.5f,  0.5f,    0.5f,   1.0f,        0.0f,    -1.0f,    0.0f,
        0.5f,  0.5f,  0.5f,    0.5f,    0.666f,        0.0f,    -1.0f,    0.0f,
        0.5f,  0.5f,  -0.5f,    0.75f,    0.666f,        0.0f,    -1.0f,    0.0f,
        -0.5f, 0.5f,  -0.5f,    0.75f,    1.0f,        0.0f,    -1.0f,    0.0f,
    };

    MeshModel* dado = new MeshModel();
    dado->CreateMeshModel(cubo_vertices, cubo_indices, 192, 36);
    meshListModel.push_back(dado);
}

void CrearHolocron()
{
    unsigned int holocron_indices[] = {
        7, 4, 5,  7, 6, 4,
        0, 1, 2,  0, 3, 1,
        48, 49, 51, 48, 50, 51,
        14, 12, 13,  14, 15, 12,
        10, 11, 8,  10, 9, 11,
        18, 17, 19,  18, 16, 17,
        20, 21, 22,
        31, 30, 29,
        32, 33, 35,  32, 34, 33,
        45, 46, 47,  45, 44, 46,
        25, 24, 23,
        26, 27, 28,
        36, 37, 38,  36, 39, 37,
        43, 41, 40,  43, 42, 41,
    };

    GLfloat holocron_vertices[] = {
         0.0f,    2.0f,  -2.0f,     0.0f,        0.5f,   0.0f,    -1.0f,     0.0f,
         0.0f,    2.0f,   2.0f,     0.25f,        0.5f,   0.0f,    -1.0f,     0.0f,
         2.0f,    2.0f,   0.0f,        0.1f,   0.66f,   0.0f,    -1.0f,     0.0f,
        -2.0f,    2.0f,   0.0f,        0.125f,   0.33f,   0.0f,    -1.0f,     0.0f,
         0.0f,  2.0f,  2.0f,         0.25f,        0.5f,   0.0f,     0.0f,    -1.0f,
        -2.0f,  0.0f,  2.0f,        0.375f,   0.33f,   0.0f,     0.0f,    -1.0f,
         2.0f,  0.0f,  2.0f,        0.375f,   0.66f,   0.0f,     0.0f,    -1.0f,
         0.0f, -2.0f,  2.0f,         0.5f,   0.5f,   0.0f,     0.0f,    -1.0f,
          2.0f,  0.0f,  2.0f,  0.5f,   0.825f,  -1.0f,     0.0f,     0.0f,
          2.0f,  0.0f, -2.0f,  0.75f,   0.825f,  -1.0f,     0.0f,     0.0f,
          2.0f, -2.0f,  0.0f,  0.625f,   0.66f,  -1.0f,     0.0f,     0.0f,
          2.0f,  2.0f,  0.0f,  0.625f,   1.0f,  -1.0f,     0.0f,     0.0f,
           0.0f,  2.0f, -2.0f,  0.75f,   0.495f,   0.0f,     0.0f,     1.0f,
           2.0f,  0.0f, -2.0f,  0.875f,   0.66f,   0.0f,     0.0f,     1.0f,
           0.0f, -2.0f, -2.0f,  1.0f,   0.495f,   0.0f,     0.0f,     1.0f,
          -2.0f,  0.0f, -2.0f,  0.875f,   0.33f,   0.0f,     0.0f,     1.0f,
          -2.0f, -2.0f,  0.0f,  0.75f,   0.165f,   1.0f,     0.0f,     0.0f,
          -2.0f,  0.0f,  2.0f,  0.625f,   0.33f,   1.0f,     0.0f,     0.0f,
          -2.0f,  0.0f, -2.0f,  0.625f,   0.0f,   1.0f,     0.0f,     0.0f,
          -2.0f,  2.0f,  0.0f,  0.5f,   0.165f,   1.0f,     0.0f,     0.0f,
           0.0f,  2.0f,  2.0f,  0.375f,   0.66f, -0.57735f, -0.57735f, -0.57735f,
           2.0f,  0.0f,  2.0f,  0.125f,   0.66f, -0.57735f, -0.57735f, -0.57735f,
           2.0f,  2.0f,  0.0f,  0.25f,   0.5f, -0.57735f, -0.57735f, -0.57735f,
            0.0f,  2.0f, -2.0f,  0.725f,   0.7116f, -0.57735f, -0.57735f,  0.57735f,
            2.0f,  0.0f, -2.0f,  0.625f,   0.5384f, -0.57735f, -0.57735f,  0.57735f,
            2.0f,  2.0f,  0.0f,  0.525f,   0.7116f, -0.57735f, -0.57735f,  0.57735f,
             0.0f,  2.0f, -2.0f,  0.775f,   0.7116f,  0.57735f, -0.57735f,  0.57735f,
            -2.0f,  0.0f, -2.0f,  0.875f,   0.5384f,  0.57735f, -0.57735f,  0.57735f,
            -2.0f,  2.0f,  0.0f,  0.975f,   0.7116f,  0.57735f, -0.57735f,  0.57735f,
             0.0f,  2.0f,  2.0f,  0.375f,   0.33f,  0.57735f, -0.57735f, -0.57735f,
            -2.0f,  0.0f,  2.0f,  0.125f,   0.33f,  0.57735f, -0.57735f, -0.57735f,
            -2.0f,  2.0f,  0.0f,  0.25f,   0.5f,  0.57735f, -0.57735f, -0.57735f,
             2.0f,  0.0f,  2.0f,  0.375f,   0.4616f, -0.57735f,  0.57735f, -0.57735f,
             1.0f, -2.0f,  1.0f,  0.375f,   0.2884f, -0.57735f,  0.57735f, -0.57735f,
             0.0f, -2.0f,  2.0f,  0.275f,   0.2884f, -0.57735f,  0.57735f, -0.57735f,
             2.0f, -2.0f,  0.0f,  0.475f,   0.2884f, -0.57735f,  0.57735f, -0.57735f,
              2.0f,  0.0f, -2.0f,  0.625f,   0.4616f, -0.57735f,  0.57735f,  0.57735f,
              1.0f, -2.0f, -1.0f,  0.625f,   0.2884f, -0.57735f,  0.57735f,  0.57735f,
              0.0f, -2.0f, -2.0f,  0.725f,   0.2884f, -0.57735f,  0.57735f,  0.57735f,
              2.0f, -2.0f,  0.0f,  0.525f,   0.2884f, -0.57735f,  0.57735f,  0.57735f,
              -2.0f, -2.0f,  0.0f,  0.975f,   0.2884f,  0.57735f,  0.57735f,  0.57735f,
              -1.0f, -2.0f, -1.0f,  0.875f,   0.2884f,  0.57735f,  0.57735f,  0.57735f,
               0.0f, -2.0f, -2.0f,  0.775f,   0.2884f,  0.57735f,  0.57735f,  0.57735f,
              -2.0f,  0.0f, -2.0f,  0.875f,   0.4616f,  0.57735f,  0.57735f,  0.57735f,
              -2.0f, -2.0f,  0.0f,  0.025f,   0.0384f,  0.57735f,  0.57735f, -0.57735f,
              -2.0f,  0.0f,  2.0f,  0.125f,   0.2116f,  0.57735f,  0.57735f, -0.57735f,
              -1.0f, -2.0f,  1.0f,  0.125f,   0.0384f,  0.57735f,  0.57735f, -0.57735f,
               0.0f, -2.0f,  2.0f,  0.225f,   0.0384f,  0.57735f,  0.57735f, -0.57735f,
                 -2.0f, -2.0f,  0.0f,  0.625f,   0.66f,   0.0f,       1.0f,       0.0f,
                    0.0f, -2.0f,  2.0f,  0.5f,   0.5f,   0.0f,       1.0f,       0.0f,
                    0.0f, -2.0f, -2.0f,  0.75f,   0.495f,   0.0f,       1.0f,       0.0f,
                    2.0f, -2.0f,  0.0f,  0.625f,   0.33f,   0.0f,       1.0f,       0.0f
    };

    MeshModel* holocron = new MeshModel();
    holocron->CreateMeshModel(
        holocron_vertices,
        holocron_indices,
        sizeof(holocron_vertices) / sizeof(GLfloat),
        sizeof(holocron_indices) / sizeof(unsigned int)
    );
    meshListModel.push_back(holocron);
}

int main()
{
    mainWindow = Window(1366, 768);
    mainWindow.Initialise();
    CreateObjects();
    CrearDado();
    CrearHolocron();
    CreateShaders();

    camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);

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
    holocronTexture = Texture("Textures/holocrontextura.png");
    holocronTexture.LoadTextureA();
    dadoTexture = Texture("Textures/dadostarwars.png");
    dadoTexture.LoadTextureA();

    Blackhawk_M = Model();
    Blackhawk_M.LoadModel("Models/uh60.obj");

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

    Avion_M = Model();
    Avion_M.LoadModel("Models/roger.obj");

   
    NaveEspacial_M = Model();
    NaveEspacial_M.LoadModel("Models/NaveEspacial.obj");

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

    // 0. Luz general de la nave 
    pointLights[0] = PointLight(1.0f, 1.0f, 1.0f,
        0.1f, 1.0f,
        0.0f, 5.0f, 0.0f,
        1.0f, 0.35f, 0.44f);   // antes: 1.0, 0.05, 0.01
    pointLightCount++;

    // 1. Luz azul Motor 1 
    pointLights[1] = PointLight(0.0f, 0.5f, 1.0f,
        0.2f, 2.0f,
        0.0f, 0.0f, 0.0f,
        1.0f, 0.07f, 0.015f);     // antes: 1.0, 0.07, 0.017
    pointLightCount++;

    // 2. Luz azul Motor 2 
    pointLights[2] = PointLight(0.0f, 0.5f, 1.0f,
        0.2f, 2.0f,
        0.0f, 0.0f, 0.0f,
        1.0f, 0.07f, 0.015f);     // antes: 1.0, 0.07, 0.017
    pointLightCount++;

    unsigned int spotLightCount = 0;

    // SpotLight 0: Cámara (Linterna)
    spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
        0.0f, 2.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        5.0f);
    spotLightCount++;

    // SpotLight 1: Faro frontal para la nave espacial
    spotLights[1] = SpotLight(1.0f, 1.0f, 1.0f,
        0.1f, 2.0f,
        0.0f, 5.0f, 0.0f,
        0.0f, 0.0f, -1.0f,
        1.0f, 0.0f, 0.0f,
        25.0f);
    spotLightCount++;

    GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
        uniformSpecularIntensity = 0, uniformShininess = 0;
    GLuint uniformColor = 0;
    glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);

    while (!mainWindow.getShouldClose())
    {
        GLfloat now = glfwGetTime();
        deltaTime = now - lastTime;
        deltaTime += (now - lastTime) / limitFPS;
        lastTime = now;

        // --- ACTUALIZACIÓN DE ESTADOS DE LUCES Y MOTORES
        GLfloat naveIntensidad = mainWindow.getNaveLucesOn() ? 1.0f : 0.0f;
        GLfloat motor1Intensity = mainWindow.getMotor1On() ? 2.0f : 0.0f;
        GLfloat motor2Intensity = mainWindow.getMotor2On() ? 2.0f : 0.0f;

        pointLights[0].SetDiffuseIntensity(naveIntensidad); // Apaga/prende luz general de la nave con tecla L
        pointLights[1].SetDiffuseIntensity(motor1Intensity); // Apaga/prende Motor 1 con tecla 1
        pointLights[2].SetDiffuseIntensity(motor2Intensity); // Apaga/prende Motor 2 con tecla 2

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

        uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
        uniformShininess = shaderList[0].GetShininessLocation();

        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
        glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

        glm::vec3 lowerLight = camera.getCameraPosition();
        lowerLight.y -= 0.3f;
        spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

        // Enviar luces al shader
        shaderList[0].SetDirectionalLight(&mainLight);
        shaderList[0].SetPointLights(pointLights, pointLightCount);
        shaderList[0].SetSpotLights(spotLights, spotLightCount);

        glm::mat4 model(1.0);
        glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

        // Piso
        model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
        model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));

        pisoTexture.UseTexture();
        Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
        meshListModel[2]->RenderMeshModel();

        // --- RENDERIZAR NAVE ESPACIAL  ---
        glm::mat4 modelNave = glm::mat4(1.0f);
        GLfloat naveMovY = sin(glfwGetTime() * 2.0f) * 0.2f; // Efecto de flotación suave
        modelNave = glm::translate(modelNave, glm::vec3(0.0f, 6.0f + naveMovY, -10.0f));
        modelNave = glm::scale(modelNave, glm::vec3(1.0f, 1.0f, 1.0f));

        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelNave));
        NaveEspacial_M.RenderModel(); // Dibuja la nave con sus texturas de Blender

        // Actualizar SpotLight frontal de la nave y su luz general de posición
        glm::vec4 naveLightPos = modelNave * glm::vec4(0.0f, 0.0f, -2.0f, 1.0f);
        spotLights[1].SetFlash(glm::vec3(naveLightPos), glm::vec3(0.0f, 0.0f, -1.0f));
        pointLights[0].SetPosition(glm::vec3(modelNave * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)));

        // --- POSICIONES DE LAS LUCES DE LOS MOTORES ---
        glm::vec4 motor1Pos = modelNave * glm::vec4(-0.7f, 0.25f, 3.8f, 1.0f); // Reactor Izquierdo
        
        glm::vec4 motor2Pos = modelNave * glm::vec4(0.7f, 0.25f, 3.8f, 1.0f); //reactor derecho

   

        pointLights[1].SetPosition(glm::vec3(motor1Pos));
        pointLights[2].SetPosition(glm::vec3(motor2Pos));
/*
        // --- MARCADORES VISIBLES DE LAS LUCES ---
        glUniform3f(uniformColor, 0.0f, 0.8f, 1.0f);   // cian brillante
        plainTexture.UseTexture();
        Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);

        // Marcador del motor 1
        glm::mat4 marcaLuz1 = glm::mat4(1.0f);
        marcaLuz1 = glm::translate(marcaLuz1, glm::vec3(motor1Pos));
        marcaLuz1 = glm::scale(marcaLuz1, glm::vec3(0.15f, 0.15f, 0.15f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(marcaLuz1));
        meshListModel[4]->RenderMeshModel();   // el dado, reducido

        // Marcador del motor 2
        glm::mat4 marcaLuz2 = glm::mat4(1.0f);
        marcaLuz2 = glm::translate(marcaLuz2, glm::vec3(motor2Pos));
        marcaLuz2 = glm::scale(marcaLuz2, glm::vec3(0.15f, 0.15f, 0.15f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(marcaLuz2));
        meshListModel[4]->RenderMeshModel();


        // Blending (Agave)
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        AgaveTexture.UseTexture();
        Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
        meshListModel[3]->RenderMeshModel();
        glDisable(GL_BLEND);
        */
        glUseProgram(0);

        mainWindow.swapBuffers();
    }

    return 0;
}