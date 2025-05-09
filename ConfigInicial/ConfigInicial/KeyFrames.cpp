/*  García González Alejandro          422066992       
/*                                                 
/*   Práctica opcional: SkyBox          Fecha de entrega: 09/05/2025   
*/ 
#include <iostream>        // Librería estándar para entrada/salida
#include <cmath>           // Funciones matemáticas (sin, cos, etc.)
#include <fstream>         // Lectura/escritura de archivos
#include <vector>          // Contenedor dinámico vector
#include <string>          // Clase string para cadenas de texto

// GLEW
#include <GL/glew.h>       // Gestión de extensiones de OpenGL

// GLFW
#include <GLFW/glfw3.h>    // Creación de ventanas y manejo de eventos

// Other Libs
#include "stb_image.h"     // Carga de imágenes para texturas

// GLM Mathematics
#include <glm/glm.hpp>                              // Tipos básicos de GLM (vec3, mat4, etc.)
#include <glm/gtc/matrix_transform.hpp>             // Funciones de transformación (translate, rotate, scale)
#include <glm/gtc/type_ptr.hpp>                     // Conversión de matrices/vector a puntero

// Load Models
#include "SOIL2/SOIL2.h"    // Carga de texturas con SOIL2

// Other includes
#include "Shader.h"        // Clase para compilar y usar shaders
#include "Camera.h"        // Clase cámara para controlar vista
#include "Model.h"         // Clase para cargar y dibujar modelos 3D
#include "Texture.h"       // Clase auxiliar para texturas

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode); // Callback de teclado
void MouseCallback(GLFWwindow* window, double xPos, double yPos);                   // Callback de ratón
void DoMovement();           // Procesa movimiento continuo (teclas pulsadas)
void Animation();            // Lógica de interpolación y reproducción de keyframes

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600; // Dimensiones iniciales de la ventana
int SCREEN_WIDTH, SCREEN_HEIGHT;        // Dimensiones reales del framebuffer

// Camera
Camera  camera(glm::vec3(0.0f, 0.0f, 3.0f)); // Posición inicial de la cámara
GLfloat lastX = WIDTH / 2.0;                 // Posición X previa del ratón
GLfloat lastY = HEIGHT / 2.0;                // Posición Y previa del ratón
bool keys[1024];                             // Array de estados de teclas
bool firstMouse = true;                      // Indicador de primer movimiento del ratón

// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f); // Posición de la luz direccional
bool active;                         // Estado de activación de la luz animada

// Positions of the point lights
glm::vec3 pointLightPositions[] = {
    glm::vec3(0.0f, 2.0f, 0.0f),   // Primera luz puntual
    glm::vec3(0.0f, 0.0f, 0.0f),   // Segunda luz puntual
    glm::vec3(0.0f, 0.0f, 0.0f),   // Tercera luz puntual
    glm::vec3(0.0f, 0.0f, 0.0f)    // Cuarta luz puntual
};

// Vértices de un cubo (posición + normal)
float vertices[] = {
     -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, // Cara trasera, vértice 1
      0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, // Cara trasera, vértice 2
      0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, // Cara trasera, vértice 3
      0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, // Cara trasera, vértice 4
     -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, // Cara trasera, vértice 5
     -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, // Cara trasera, vértice 6

     -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, // Cara frontal, vértice 1
      0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, // Cara frontal, vértice 2
      0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, // Cara frontal, vértice 3
      0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, // Cara frontal, vértice 4
     -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, // Cara frontal, vértice 5
     -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, // Cara frontal, vértice 6

     -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, // Cara izquierda, vértice 1
     -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, // Cara izquierda, vértice 2
     -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, // Cara izquierda, vértice 3
     -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, // Cara izquierda, vértice 4
     -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, // Cara izquierda, vértice 5
     -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, // Cara izquierda, vértice 6

      0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, // Cara derecha, vértice 1
      0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, // Cara derecha, vértice 2
      0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, // Cara derecha, vértice 3
      0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, // Cara derecha, vértice 4
      0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, // Cara derecha, vértice 5
      0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, // Cara derecha, vértice 6

     -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, // Cara inferior, vértice 1
      0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, // Cara inferior, vértice 2
      0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, // Cara inferior, vértice 3
      0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, // Cara inferior, vértice 4
     -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, // Cara inferior, vértice 5
     -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, // Cara inferior, vértice 6

     -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, // Cara superior, vértice 1
      0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, // Cara superior, vértice 2
      0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, // Cara superior, vértice 3
      0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, // Cara superior, vértice 4
     -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, // Cara superior, vértice 5
     -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f  // Cara superior, vértice 6
};

// Variables de animación
glm::vec3 Light1 = glm::vec3(0); // Factores de animación de color
float rotBall = 0.0f;            // Rotación de la pelota
float rotDog = 0.0f;             // Rotación global del perro
int dogAnim = 0;                 // Estado de la animación del perro (no usado)
float FLegs = 0.0f;              // Ángulo patas delanteras
float RLegs = 0.0f;              // Ángulo patas traseras
float head = 0.0f;               // Ángulo cabeza
float tail = 0.0f;               // Ángulo cola
float dogBody = 0.0f;            // Ángulo cuerpo
float Rleg = 0.0f;               // Ángulo segundo segmento pata delantera derecha
float BLegs = 0.0f;              // Ángulo segundo segmento pata trasera

// KeyFrames
float dogPosX, dogPosY, dogPosZ; // Posición del perro

#define MAX_FRAMES 9             // Número máximo de keyframes
int i_max_steps = 190;           // Pasos de interpolación entre keyframes
int i_curr_steps = 0;            // Paso actual de interpolación

// Estructura para un keyframe
typedef struct _frame {
    float rotDog;    // Rotación perro
    float rotDogInc; // Incremento rotación
    float dogPosX;   // Posición X
    float dogPosY;   // Posición Y
    float dogPosZ;   // Posición Z
    float incX;      // Incremento posición X
    float incY;      // Incremento posición Y
    float incZ;      // Incremento posición Z
    float head;      // Ángulo cabeza
    float headInc;   // Incremento cabeza
    float RLegs;     // Ángulo patas traseras
    float RLegsInc;  // Incremento patas traseras
    float FLegs;     // Ángulo patas delanteras
    float FLegsInc;  // Incremento patas delanteras
    float tail;      // Ángulo cola
    float tailInc;   // Incremento cola
    float dogBody;   // Ángulo cuerpo
    float dogBodyInc;// Incremento cuerpo
    float RLeg;      // Ángulo segundo segmento pata delantera derecha
    float RLegInc;   // Incremento segundo segmento
    float BLegs;     // Ángulo segundo segmento pata trasera
    float BLegsInc;  // Incremento segundo segmento
} FRAME;

FRAME KeyFrame[MAX_FRAMES]; // Array de keyframes
int FrameIndex = 0;        // Índice actual de keyframe
bool play = false;         // Reproduciendo animación
int playIndex = 0;         // Índice de keyframe en reproducción

// Guarda un keyframe con los valores actuales
void saveFrame(void)
{
    printf("frameindex %d\n", FrameIndex);         // Imprime índice

    KeyFrame[FrameIndex].dogPosX = dogPosX;         // Guarda posición X
    KeyFrame[FrameIndex].dogPosY = dogPosY;         // Guarda posición Y
    KeyFrame[FrameIndex].dogPosZ = dogPosZ;         // Guarda posición Z
    KeyFrame[FrameIndex].head = head;            // Guarda ángulo cabeza
    KeyFrame[FrameIndex].tail = tail;            // Guarda ángulo cola
    KeyFrame[FrameIndex].RLegs = RLegs;           // Guarda ángulo patas traseras
    KeyFrame[FrameIndex].FLegs = FLegs;           // Guarda ángulo patas delanteras
    KeyFrame[FrameIndex].RLeg = Rleg;            // Guarda ángulo segundo segmento
    KeyFrame[FrameIndex].BLegs = BLegs;           // Guarda ángulo segundo segmento pata trasera
    KeyFrame[FrameIndex].dogBody = dogBody;         // Guarda ángulo cuerpo
    KeyFrame[FrameIndex].rotDog = rotDog;          // Guarda rotación global
    FrameIndex++;                                   // Incrementa índice
}

// Guarda los keyframes en un CSV
void guardarEnCSV(const std::string& ruta, FRAME* frames, const std::string& nombreArchivo) {
    std::ofstream archivo(ruta + "/" + nombreArchivo); // Abre archivo

    if (!archivo.is_open()) {                          // Si falla apertura
        std::cerr << "No se pudo abrir el archivo para escritura." << std::endl;
        return;                                        // Sale de la función
    }

    for (int i = 0; i < FrameIndex; i++) {             // Recorre keyframes guardados
        archivo << frames[i].dogPosX << "," << frames[i].dogPosY << "," << frames[i].dogPosZ << ","
            << frames[i].head << "," << frames[i].tail << ","
            << frames[i].RLegs << "," << frames[i].FLegs << ","
            << frames[i].RLeg << "," << frames[i].dogBody << ","
            << frames[i].BLegs << "," << frames[i].dogBody << "," // Nota: dogBody repetido
            << frames[i].rotDog << "\n";             // Escribe línea CSV
    }

    archivo.close();                                   // Cierra archivo
}

// Carga los keyframes desde un CSV
void cargarDesdeCSV(const std::string& ruta, const std::string& nombreArchivo) {
    std::ifstream archivo(ruta + "/" + nombreArchivo); // Abre archivo

    if (!archivo.is_open()) {                          // Si falla apertura
        std::cerr << "No se pudo abrir el archivo para lectura." << std::endl;
        return;                                        // Sale de la función
    }

    std::string linea;
    FrameIndex = 0;                                    // Reinicia índice

    while (std::getline(archivo, linea) && FrameIndex < MAX_FRAMES) { // Lee líneas
        std::stringstream ss(linea);                   // Stream de la línea
        std::string valor;
        FRAME frame;

        std::getline(ss, valor, ','); frame.dogPosX = std::stof(valor);  // Lee dogPosX
        std::getline(ss, valor, ','); frame.dogPosY = std::stof(valor);  // Lee dogPosY
        std::getline(ss, valor, ','); frame.dogPosZ = std::stof(valor);  // Lee dogPosZ
        std::getline(ss, valor, ','); frame.head = std::stof(valor);  // Lee head
        std::getline(ss, valor, ','); frame.tail = std::stof(valor);  // Lee tail
        std::getline(ss, valor, ','); frame.RLegs = std::stof(valor);  // Lee RLegs
        std::getline(ss, valor, ','); frame.FLegs = std::stof(valor);  // Lee FLegs
        std::getline(ss, valor, ','); frame.RLeg = std::stof(valor);  // Lee RLeg
        std::getline(ss, valor, ','); frame.dogBody = std::stof(valor);  // Lee dogBody
        std::getline(ss, valor, ','); frame.BLegs = std::stof(valor);  // Lee BLegs
        std::getline(ss, valor, ','); frame.dogBody = std::stof(valor);  // Lee dogBody repetido
        std::getline(ss, valor, ','); frame.rotDog = std::stof(valor);  // Lee rotDog

        KeyFrame[FrameIndex++] = frame;                // Almacena el frame
    }

    archivo.close();                                   // Cierra archivo
}

// Restaura elementos al primer keyframe
void resetElements(void) {
    dogPosX = KeyFrame[0].dogPosX;     // Restaura posición X
    dogPosY = KeyFrame[0].dogPosY;     // Restaura posición Y
    dogPosZ = KeyFrame[0].dogPosZ;     // Restaura posición Z
    head = KeyFrame[0].head;        // Restaura ángulo cabeza
    tail = KeyFrame[0].tail;        // Restaura ángulo cola
    RLegs = KeyFrame[0].RLegs;       // Restaura patas traseras
    FLegs = KeyFrame[0].FLegs;       // Restaura patas delanteras
    Rleg = KeyFrame[0].RLeg;        // Restaura segundo segmento delantera
    dogBody = KeyFrame[0].dogBody;     // Restaura cuerpo
    BLegs = KeyFrame[0].BLegs;       // Restaura segundo segmento trasera
    rotDog = KeyFrame[0].rotDog;      // Restaura rotación
}

// Calcula los incrementos de interpolación para el keyframe actual
void interpolation(void) {
    KeyFrame[playIndex].incX = (KeyFrame[playIndex + 1].dogPosX - KeyFrame[playIndex].dogPosX) / i_max_steps;    // ΔX
    KeyFrame[playIndex].incY = (KeyFrame[playIndex + 1].dogPosY - KeyFrame[playIndex].dogPosY) / i_max_steps;    // ΔY
    KeyFrame[playIndex].incZ = (KeyFrame[playIndex + 1].dogPosZ - KeyFrame[playIndex].dogPosZ) / i_max_steps;    // ΔZ
    KeyFrame[playIndex].headInc = (KeyFrame[playIndex + 1].head - KeyFrame[playIndex].head) / i_max_steps;    // Δcabeza
    KeyFrame[playIndex].tailInc = (KeyFrame[playIndex + 1].tail - KeyFrame[playIndex].tail) / i_max_steps;    // Δcola
    KeyFrame[playIndex].RLegsInc = (KeyFrame[playIndex + 1].RLegs - KeyFrame[playIndex].RLegs) / i_max_steps;    // Δp traseras
    KeyFrame[playIndex].FLegsInc = (KeyFrame[playIndex + 1].FLegs - KeyFrame[playIndex].FLegs) / i_max_steps;    // Δp delanteras
    KeyFrame[playIndex].rotDogInc = (KeyFrame[playIndex + 1].rotDog - KeyFrame[playIndex].rotDog) / i_max_steps;    // Δrotación
    KeyFrame[playIndex].RLegInc = (KeyFrame[playIndex + 1].RLeg - KeyFrame[playIndex].RLeg) / i_max_steps;    // Δsegm delantera
    KeyFrame[playIndex].dogBodyInc = (KeyFrame[playIndex + 1].dogBody - KeyFrame[playIndex].dogBody) / i_max_steps;    // Δcuerpo
    KeyFrame[playIndex].BLegsInc = (KeyFrame[playIndex + 1].BLegs - KeyFrame[playIndex].BLegs) / i_max_steps;    // Δsegm trasera
}

// Deltatime
GLfloat deltaTime = 0.0f;  // Tiempo entre frames
GLfloat lastFrame = 0.0f;  // Marca del último frame

int main()
{
    // Init GLFW
    glfwInit(); // Inicializa GLFW

    // Create window
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Animacion keyframes - Garcia Gonzalez Alejandro", nullptr, nullptr); // Crea ventana

    if (nullptr == window) { // Comprueba creación
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();     // Termina GLFW
        return EXIT_FAILURE; // Sale con error
    }

    glfwMakeContextCurrent(window);                   // Hace contexto actual
    glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT); // Obtiene tamaño real

    // Callbacks
    glfwSetKeyCallback(window, KeyCallback);         // Teclado
    glfwSetCursorPosCallback(window, MouseCallback); // Ratón

    // GLEW
    glewExperimental = GL_TRUE;                       // Modo experimental
    if (GLEW_OK != glewInit()) {                     // Inicializa GLEW
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;                         // Sale con error
    }

    // Viewport
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);   // Define viewport

    // Carga shaders
    Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag"); // Shader iluminación
    Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");           // Shader lámpara
    Shader skyboxshader("Shader/SkyBox.vs", "Shader/SkyBox.frag");     // Shader skybox

    // Carga modelos
    Model DogBody((char*)"Models/DogBody.obj");               // Cuerpo perro
    Model HeadDog((char*)"Models/HeadDog.obj");               // Cabeza perro
    Model DogTail((char*)"Models/TailDog.obj");               // Cola perro
    Model F_RightLeg((char*)"Models/F_RightLegDog.obj");      // Pata delantera derecha
    Model F_LeftLeg((char*)"Models/F_LeftLegDog.obj");        // Pata delantera izquierda
    Model B_RightLeg((char*)"Models/B_RightLegDog.obj");      // Pata trasera derecha
    Model B_LeftLeg((char*)"Models/B_LeftLegDog.obj");        // Pata trasera izquierda
    Model Piso((char*)"Models/piso.obj");                     // Suelo
    Model Ball((char*)"Models/ball.obj");                     // Pelota

    // Inicializa keyframes a cero
    for (int i = 0; i < MAX_FRAMES; i++) {
        KeyFrame[i].dogPosX = 0; KeyFrame[i].dogPosY = 0; KeyFrame[i].dogPosZ = 0;
        KeyFrame[i].incX = 0;   KeyFrame[i].incY = 0;   KeyFrame[i].incZ = 0;
        KeyFrame[i].rotDog = 0; KeyFrame[i].rotDogInc = 0;
        KeyFrame[i].head = 0;   KeyFrame[i].headInc = 0;
        KeyFrame[i].tail = 0;   KeyFrame[i].tailInc = 0;
        KeyFrame[i].FLegs = 0;  KeyFrame[i].FLegsInc = 0;
        KeyFrame[i].RLegs = 0;  KeyFrame[i].RLegsInc = 0;
        KeyFrame[i].RLeg = 0;   KeyFrame[i].RLegInc = 0;
        KeyFrame[i].dogBody = 0; KeyFrame[i].dogBodyInc = 0;
        KeyFrame[i].BLegs = 0;  KeyFrame[i].BLegsInc = 0;
    }

    // Skybox: vértices de cubo unitario
    GLfloat skyboxVertices[] = {
        -1.0f,  1.0f, -1.0f,  // Cara trasera superior izquierda
        -1.0f, -1.0f, -1.0f,  // Cara trasera inferior izquierda
         1.0f, -1.0f, -1.0f,  // Cara trasera inferior derecha
         1.0f, -1.0f, -1.0f,  // Repite triángulo
         1.0f,  1.0f, -1.0f,  
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    // Índices para dibujar skybox con un EBO (no usado explícitamente)
    GLuint indices[] = {
        0,1,2,3,
        4,5,6,7,
        8,9,10,11,
        12,13,14,15,
        16,17,18,19,
        20,21,22,23,
        24,25,26,27,
        28,29,30,31,
        32,33,34,35
    };

    // --- Configuración VAO/VBO/EBO para el cubo principal ---
    GLuint VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);          // Genera VAO
    glGenBuffers(1, &VBO);               // Genera VBO
    glGenBuffers(1, &EBO);               // Genera EBO

    glBindVertexArray(VAO);              // Enlaza VAO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);  // Enlaza VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // Subida de vértices

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // Enlaza EBO
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); // Subida índices

    // Atributos de vértice
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0); // Posición
    glEnableVertexAttribArray(0);  // Habilita posición
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))); // Normal
    glEnableVertexAttribArray(1);  // Habilita normal

    // Textura difusa y especular en shaders
    lightingShader.Use(); // Activa shader
    glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.difuse"), 0);   // Unidad 0
    glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.specular"), 1);// Unidad 1

    // --- Configuración Skybox VAO/VBO ---
    GLuint skyboxVBO, skyboxVAO;
    glGenVertexArrays(1, &skyboxVAO);       // Genera VAO skybox
    glGenBuffers(1, &skyboxVBO);            // Genera VBO skybox
    glBindVertexArray(skyboxVAO);           // Enlaza VAO skybox
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);// Enlaza VBO skybox
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);// Subida de vértices
    glEnableVertexAttribArray(0);           // Habilita posición
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);// Atributo posición

    // Carga caras del cubemap
    std::vector<const GLchar*> faces;
    faces.push_back("SkyBox/right.jpg");    // Cara derecha
    faces.push_back("SkyBox/left.jpg");     // Cara izquierda
    faces.push_back("SkyBox/top.jpg");      // Cara superior
    faces.push_back("SkyBox/bottom.jpg");   // Cara inferior
    faces.push_back("SkyBox/back.jpg");     // Cara trasera
    faces.push_back("SkyBox/front.jpg");    // Cara frontal

    GLuint cubemapTexture = TextureLoading::LoadCubemap(faces); // Carga cubemap

    // Matriz de proyección
    glm::mat4 projection = glm::perspective(camera.GetZoom(),
        (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT,
        0.1f, 100.0f); // FOV, aspecto, near, far

    // Game loop
    while (!glfwWindowShouldClose(window)) { // Mientras no se cierre la ventana
        // Calcula deltaTime
        GLfloat currentFrame = glfwGetTime();        // Tiempo actual
        deltaTime = currentFrame - lastFrame;        // Delta
        lastFrame = currentFrame;                    // Actualiza última marca

        // Eventos y actualización
        glfwPollEvents();   // Procesa eventos
        DoMovement();       // Movimiento continuo
        Animation();        // Interpolación/animación

        // Limpia buffers
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f); // Color de fondo
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Limpia color y profundidad

        glEnable(GL_DEPTH_TEST); // Habilita prueba de profundidad

        glm::mat4 modelTemp = glm::mat4(1.0f); // Matriz temporal identidad

        // Usa shader de iluminación
        lightingShader.Use();
        glUniform1i(glGetUniformLocation(lightingShader.Program, "diffuse"), 0); // Unidad difusa
        GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos"); // Localización viewPos
        glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z); // Envía posición cámara

        // Configura luz direccional
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f); // Dirección
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.6f, 0.6f, 0.6f); // Ambien te
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.6f, 0.6f, 0.6f); // Difusa
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.3f, 0.3f, 0.3f); // Especular

        // Calcula color animado de luz puntual 1
        glm::vec3 lightColor;
        lightColor.x = abs(sin(glfwGetTime() * Light1.x)); // Canal R animado
        lightColor.y = abs(sin(glfwGetTime() * Light1.y)); // Canal G animado
        lightColor.z = sin(glfwGetTime() * Light1.z); // Canal B animado

        // Configura luz puntual 0
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"),
            pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z); // Posición
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"),
            lightColor.x, lightColor.y, lightColor.z); // Componente ambiente
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"),
            lightColor.x, lightColor.y, lightColor.z); // Componente difusa
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"),
            1.0f, 0.2f, 0.2f); // Componente especular
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);  // Atenuación constante
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.045f);// Atenuación lineal
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.075f);// Atenuación cuadrática

        // Configura spotlight (foco) desde cámara
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"),
            camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);// Posición foco
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"),
            camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);        // Dirección foco
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"),
            0.2f, 0.2f, 0.8f); // Ambiente foco
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"),
            0.2f, 0.2f, 0.8f); // Difusa foco
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"),
            0.0f, 0.0f, 0.0f); // Especular foco
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);    // Atenuación
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.3f);    // Atenuación
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.7f);    // Atenuación
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"),
            glm::cos(glm::radians(12.0f))); // Ángulo interno
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"),
            glm::cos(glm::radians(18.0f))); // Ángulo externo

        // Propiedad de material
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 5.0f); // Brillo

        // Matriz de vista
        glm::mat4 view = camera.GetViewMatrix(); // Calcula vista

        // Obtiene localizaciones de uniforms
        GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");      // Modelo
        GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");       // Vista
        GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");// Proyección

        // Envía matrices a shader
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));        // Vista
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));  // Proyección

        // --- Dibujar escena ---

        // Piso
        glm::mat4 model = glm::mat4(1.0f);   // Identidad
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); // Envía modelo
        Piso.Draw(lightingShader);          // Dibuja suelo

        // Cuerpo del perro
        model = glm::mat4(1.0f);             // Reinicia modelo
        modelTemp = glm::translate(model, glm::vec3(dogPosX, dogPosY, dogPosZ)); // Traslada
        model = glm::rotate(modelTemp, glm::radians(rotDog), glm::vec3(1.0f, 0.0f, 0.0f)); // Rota eje X
        model = glm::rotate(model, glm::radians(dogBody), glm::vec3(0.0f, 1.0f, 0.0f));    // Rota eje Y
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); // Envía modelo
        DogBody.Draw(lightingShader);        // Dibuja cuerpo

        // Cabeza
        model = modelTemp;                   // Vuelve a posición base
        model = glm::translate(model, glm::vec3(0.0f, 0.093f, 0.208f)); // Traslada a unión cabeza
        model = glm::rotate(model, glm::radians(head), glm::vec3(0.0f, 0.0f, 1.0f)); // Rota cabeza
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); // Envía modelo
        HeadDog.Draw(lightingShader);        // Dibuja cabeza

        // Cola
        model = modelTemp;                   // Base
        model = glm::translate(model, glm::vec3(0.0f, 0.026f, -0.288f)); // Traslada unión cola
        model = glm::rotate(model, glm::radians(tail), glm::vec3(0.0f, 0.0f, -1.0f)); // Rota cola
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); // Envía modelo
        DogTail.Draw(lightingShader);        // Dibuja cola

        // Pata delantera izquierda
        model = modelTemp;                   // Base
        model = glm::translate(model, glm::vec3(0.112f, -0.044f, 0.074f)); // Traslada unión pata
        model = glm::rotate(model, glm::radians(FLegs), glm::vec3(-1.0f, 0.0f, 0.0f)); // Rota pata
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); // Envía modelo
        F_LeftLeg.Draw(lightingShader);      // Dibuja pata

        // Pata delantera derecha
        model = modelTemp;                   // Base
        model = glm::translate(model, glm::vec3(-0.111f, -0.055f, 0.074f)); // Traslada unión pata
        model = glm::rotate(model, glm::radians(FLegs), glm::vec3(1.0f, 0.0f, 0.0f)); // Primer segmento
        model = glm::rotate(model, glm::radians(Rleg), glm::vec3(1.0f, 0.0f, 0.0f));  // Segundo segmento
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); // Envía modelo
        F_RightLeg.Draw(lightingShader);     // Dibuja pata

        // Pata trasera izquierda
        model = modelTemp;                   // Base
        model = glm::translate(model, glm::vec3(0.082f, -0.046f, -0.218f)); // Traslada unión pata
        model = glm::rotate(model, glm::radians(RLegs), glm::vec3(1.0f, 0.0f, 0.0f)); // Primer segmento
        model = glm::rotate(model, glm::radians(BLegs), glm::vec3(1.0f, 0.0f, 0.0f)); // Segundo segmento
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); // Envía modelo
        B_LeftLeg.Draw(lightingShader);      // Dibuja pata

        // Pata trasera derecha
        model = modelTemp;                   // Base
        model = glm::translate(model, glm::vec3(-0.083f, -0.057f, -0.231f)); // Traslada unión pata
        model = glm::rotate(model, glm::radians(RLegs), glm::vec3(1.0f, 0.0f, 0.0f)); // Primer segmento
        model = glm::rotate(model, glm::radians(BLegs), glm::vec3(-1.0f, 0.0f, 0.0f));// Segundo segmento invertido
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); // Envía modelo
        B_RightLeg.Draw(lightingShader);     // Dibuja pata

        // Pelota con transparencia
        model = glm::mat4(1.0f);             // Identidad
        glEnable(GL_BLEND);                  // Activa blending
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Función de mezcla
        glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 1); // Habilita transparencia en shader
        model = glm::rotate(model, glm::radians(rotBall), glm::vec3(0.0f, 1.0f, 0.0f)); // Rota pelota
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); // Envía modelo
        Ball.Draw(lightingShader);           // Dibuja pelota
        glDisable(GL_BLEND);                 // Desactiva blending

        glBindVertexArray(0);                // Desenlaza VAO

        // --- Dibuja lámpara de luz puntual ---
        lampShader.Use();                    // Activa shader lámpara
        modelLoc = glGetUniformLocation(lampShader.Program, "model");       // Localización modelo
        viewLoc = glGetUniformLocation(lampShader.Program, "view");        // Localización vista
        projLoc = glGetUniformLocation(lampShader.Program, "projection");  // Localización proyección

        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));      // Envía vista
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));// Envía proyección

        // Lámpara en posición direccional
        model = glm::mat4(1.0f);             // Identidad
        model = glm::translate(model, lightPos); // Traslada a lightPos
        model = glm::scale(model, glm::vec3(0.2f)); // Escala pequeña
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); // Envía modelo

        // Lámpara de la luz puntual 0
        model = glm::mat4(1.0f);             // Identidad
        model = glm::translate(model, pointLightPositions[0]); // Traslada a luz puntual
        model = glm::scale(model, glm::vec3(0.2f)); // Escala pequeña
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); // Envía modelo
        glBindVertexArray(VAO);              // Enlaza VAO cubo
        glDrawArrays(GL_TRIANGLES, 0, 36);   // Dibuja cubo lámpara

        glBindVertexArray(0);                // Desenlaza VAO

        // --- Dibuja skybox ---
        glDepthFunc(GL_LEQUAL);              // Permite dibujar skybox detrás
        skyboxshader.Use();                  // Activa shader skybox
        view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // Vista sin traslación
        glUniformMatrix4fv(glGetUniformLocation(skyboxshader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view)); // Envía vista
        glUniformMatrix4fv(glGetUniformLocation(skyboxshader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection)); // Envía proyección
        glBindVertexArray(skyboxVAO);        // Enlaza VAO skybox
        glActiveTexture(GL_TEXTURE1);        // Activa unidad 1
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture); // Enlaza cubemap
        glDrawArrays(GL_TRIANGLES, 0, 36);   // Dibuja skybox
        glBindVertexArray(0);                // Desenlaza VAO
        glDepthFunc(GL_LESS);                // Restaura depth función

        // Swap buffers
        glfwSwapBuffers(window);             // Intercambia buffers
    }

    // Limpieza de recursos
    glDeleteVertexArrays(1, &VAO);          // Elimina VAO
    glDeleteBuffers(1, &VBO);               // Elimina VBO
    glDeleteBuffers(1, &EBO);               // Elimina EBO
    glDeleteVertexArrays(1, &skyboxVAO);    // Elimina VAO skybox
    glDeleteBuffers(1, &skyboxVBO);         // Elimina VBO skybox

    glfwTerminate();                        // Termina GLFW
    return 0;                               // Fin programa
}

// Procesa movimiento continuo basado en teclas
void DoMovement()
{
    // Controles de animación manual del perro
    if (keys[GLFW_KEY_V]) { BLegs += 0.0007f; }  // Sienta patas traseras
    if (keys[GLFW_KEY_P]) { dogBody -= 0.007f; }  // Baja cuerpo
    if (keys[GLFW_KEY_O]) { Rleg -= 0.007f; }  // Eleva segundo segmento delantera
    if (keys[GLFW_KEY_I]) { Rleg += 0.007f; }  // Baja segundo segmento delantera
    if (keys[GLFW_KEY_1]) { FLegs += 0.007f; }  // Eleva patas delanteras
    if (keys[GLFW_KEY_0]) { FLegs -= 0.007f; }  // Baja patas delanteras
    if (keys[GLFW_KEY_8]) { RLegs += 0.007f; }  // Eleva patas traseras
    if (keys[GLFW_KEY_9]) { RLegs -= 0.007f; }  // Baja patas traseras
    if (keys[GLFW_KEY_6]) { tail += 0.7f; }  // Eleva cola
    if (keys[GLFW_KEY_7]) { tail -= 0.7f; }  // Baja cola
    if (keys[GLFW_KEY_4]) { head += 0.7f; }  // Gira cabeza
    if (keys[GLFW_KEY_5]) { head -= 0.7f; }  // Gira cabeza opuesto
    if (keys[GLFW_KEY_2]) { rotDog += 0.007f; }  // Rota perro adelante
    if (keys[GLFW_KEY_3]) { rotDog -= 0.007f; }  // Rota perro atrás
    if (keys[GLFW_KEY_H]) { dogPosZ += 0.001f; }  // Mueve perro adelante
    if (keys[GLFW_KEY_Y]) { dogPosZ -= 0.001f; }  // Mueve perro atrás
    if (keys[GLFW_KEY_G]) { dogPosX -= 0.001f; }  // Mueve perro izquierda
    if (keys[GLFW_KEY_J]) { dogPosX += 0.001f; }  // Mueve perro derecha

    // Controles de cámara
    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])    camera.ProcessKeyboard(FORWARD, deltaTime); // Adelante
    if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])  camera.ProcessKeyboard(BACKWARD, deltaTime); // Atrás
    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])  camera.ProcessKeyboard(LEFT, deltaTime); // Izquierda
    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT]) camera.ProcessKeyboard(RIGHT, deltaTime); // Derecha

    // Mueve luz puntual 0
    if (keys[GLFW_KEY_T]) pointLightPositions[0].x += 0.01f; // Derecha
    if (keys[GLFW_KEY_G]) pointLightPositions[0].x -= 0.01f; // Izquierda
    if (keys[GLFW_KEY_Y]) pointLightPositions[0].y += 0.01f; // Arriba
    if (keys[GLFW_KEY_H]) pointLightPositions[0].y -= 0.01f; // Abajo
    if (keys[GLFW_KEY_U]) pointLightPositions[0].z -= 0.01f; // Dentro
    if (keys[GLFW_KEY_J]) pointLightPositions[0].z += 0.01f; // Fuera
}

// Callback de teclado: pulsaciones y releases
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    // Inicia/parar animación con L
    if (keys[GLFW_KEY_L]) {
        if (!play && FrameIndex > 1) {    // Si no está reproduciendo y hay >1 frame
            resetElements();              // Resetea
            interpolation();              // Calcula primer interpolación
            play = true;                  // Activa reproducción
            playIndex = 0;                // Primer keyframe
            i_curr_steps = 0;             // Reinicia contador
        }
        else {
            play = false;                 // Para reproducción
        }
    }

    // Guarda keyframe con K
    if (keys[GLFW_KEY_K]) {
        if (FrameIndex < MAX_FRAMES) saveFrame(); // Añade nuevo keyframe
    }

    // Esc cierra ventana
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    // Actualiza estado de teclas
    if (key >= 0 && key < 1024) {
        if (action == GLFW_PRESS)      keys[key] = true;
        else if (action == GLFW_RELEASE) keys[key] = false;
    }

    // Alterna luz animada con espacio
    if (keys[GLFW_KEY_SPACE]) {
        active = !active;              // Cambia estado
        if (active) Light1 = glm::vec3(0.2f, 0.8f, 1.0f); // Activa animación
        else        Light1 = glm::vec3(0);               // Desactiva animación
    }
}

// Reproduce animación interpolada
void Animation() {
    if (play) {
        if (i_curr_steps >= i_max_steps) { // Si terminó transición
            playIndex++;                  // Siguiente keyframe
            if (playIndex > FrameIndex - 2) { // Si terminó todos
                printf("termina anim\n");      // Mensaje
                playIndex = 0;           // Reinicia índice
                play = false;           // Detiene reproducción
            }
            else {                    // Sigue animando
                i_curr_steps = 0;       // Reinicia pasos
                interpolation();        // Recalcula incrementos
            }
        }
        else {
            // Aplica incrementos por paso
            dogPosX += KeyFrame[playIndex].incX;
            dogPosY += KeyFrame[playIndex].incY;
            dogPosZ += KeyFrame[playIndex].incZ;
            head += KeyFrame[playIndex].headInc;
            tail += KeyFrame[playIndex].tailInc;
            RLegs += KeyFrame[playIndex].RLegsInc;
            FLegs += KeyFrame[playIndex].FLegsInc;
            BLegs += KeyFrame[playIndex].BLegsInc;
            Rleg += KeyFrame[playIndex].RLegInc;
            dogBody += KeyFrame[playIndex].dogBodyInc;
            rotDog += KeyFrame[playIndex].rotDogInc;
            i_curr_steps++;              // Incrementa contador
        }
    }
}

// Callback de movimiento de ratón para rotar cámara
void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
    if (firstMouse) {               // Primer movimiento
        lastX = xPos;               // Inicializa lastX
        lastY = yPos;               // Inicializa lastY
        firstMouse = false;         // Desactiva flag
    }

    GLfloat xOffset = xPos - lastX; // ΔX del ratón
    GLfloat yOffset = lastY - yPos; // ΔY (invertido eje Y)

    lastX = xPos;                   // Actualiza lastX
    lastY = yPos;                   // Actualiza lastY

    camera.ProcessMouseMovement(xOffset, yOffset); // Mueve cámara
}
