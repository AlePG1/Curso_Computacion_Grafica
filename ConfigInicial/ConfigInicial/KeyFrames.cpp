/*
*  Previo 12. Keyframes                      García González Alejandro
*  Fecha de entrega 27 de abril del 2025     422066992
*/
#include <iostream>             // Para std::cout, std::endl
#include <cmath>                // Para funciones matemáticas (sin, cos, etc.)

// GLEW: gestión de extensiones OpenGL
#include <GL/glew.h>

// GLFW: ventana, contexto y eventos
#include <GLFW/glfw3.h>

// stb_image: carga de imágenes para texturas
#include "stb_image.h"

// GLM: matemáticas (vectores y matrices)
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // translate, rotate, scale, perspective
#include <glm/gtc/type_ptr.hpp>         // glm::value_ptr

// SOIL2: otra librería de carga de texturas (no usada directamente aquí)
#include "SOIL2/SOIL2.h"

// Clases auxiliares para shaders, cámara y modelos
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// Prototipos de funciones de callbacks y utilidades
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode); // teclado
void MouseCallback(GLFWwindow* window, double xPos, double yPos);                 // ratón
void DoMovement();        // procesa movimiento según teclas
void Animation();         // actualiza animación de keyframes
void createSitAnimation();// genera keyframes para animación de sentarse

// Dimensiones iniciales de la ventana
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT; // dimensiones reales del framebuffer

// Cámara: posición inicial y control de ratón
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0;    // última posición X del cursor
GLfloat lastY = HEIGHT / 2.0;   // última posición Y del cursor
bool firstMouse = true;         // indicador para inicializar el ratón correctamente

// Estado de teclas
bool keys[1024]; // pulsado/no pulsado para cada código de tecla

// Atributos de luz
glm::vec3 lightPos(0.0f, 0.0f, 0.0f); // posición de la luz direccional
bool active;                          // estado de la luz puntual activa/apagada

// Posiciones de hasta 4 luces puntuales
glm::vec3 pointLightPositions[] = {
    glm::vec3(0.0f, 2.0f,  0.0f),
    glm::vec3(0.0f, 0.0f,  0.0f),
    glm::vec3(0.0f, 0.0f,  0.0f),
    glm::vec3(0.0f, 0.0f,  0.0f)
};

// Vertex data for a cube (posición + normales)
float vertices[] = {
     -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  // Cara trasera
      0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
      0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
      0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

     -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  // Cara frontal
      0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
      0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
      0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
     -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
     -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

     -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  // Cara izquierda
     -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
     -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
     -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
     -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
     -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

      0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  // Cara derecha
      0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
      0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
      0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
      0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
      0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

     -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  // Cara inferior
      0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
      0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
      0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

     -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  // Cara superior
      0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
      0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
      0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

// Color temporal para la luz puntual (se pulsa entre vec3(0) y vec3(r,g,b))
glm::vec3 Light1 = glm::vec3(0);

// Variables de animación globales
float rotBall = 0.0f;               // ángulo de rotación de la pelota
float rotDog = 0.0f;               // rotación horizontal del perro
float dogPosX = 0.0f, dogPosY = 0.0f, dogPosZ = 0.0f; // posición del perro

// Ángulos para extremidades y partes móviles del perro
float FLegLeft = 0.0f;  // pata delantera izquierda
float FLegRight = 0.0f;  // pata delantera derecha
float BLegLeft = 0.0f;  // pata trasera izquierda
float BLegRight = 0.0f;  // pata trasera derecha
float head = 0.0f;  // giro de la cabeza
float tail = 0.0f;  // giro de la cola
float torsoHeight = 0.0f;  // altura vertical del torso
float torsoIncline = 0.0f;  // inclinación (rotación en X) del torso

// Parámetros de keyframe
#define MAX_FRAMES 20   // máximo número de keyframes
int i_max_steps = 60;  // número de pasos entre keyframes (interpolación)
int i_curr_steps = 0;  // paso actual dentro de la interpolación

// Estructura para un keyframe completo
typedef struct _frame {
    // estado del perro
    float rotDog, rotDogInc; // rotación y su incremento por paso
    float dogPosX, dogPosY, dogPosZ; // posición y luego incrementos
    float incX, incY, incZ;
    // ángulos de extremidades, cabeza, cola y torso
    float FLegLeft, FLegRight, BLegLeft, BLegRight, head, tail, torsoHeight;
    // incrementos de esos ángulos
    float FLegLeftInc, FLegRightInc, BLegLeftInc, BLegRightInc;
    float headInc, tailInc, torsoHeightInc;
} FRAME;

FRAME KeyFrame[MAX_FRAMES]; // array de keyframes
int FrameIndex = 0;         // siguiente índice libre para guardar
bool play = false;          // bandera de reproducción activa
int playIndex = 0;          // índice de keyframe actual en reproducción

// Guarda el estado actual en KeyFrame[FrameIndex]
void saveFrame(void) {
    printf("Guardando frame %d\n", FrameIndex);
    KeyFrame[FrameIndex].dogPosX = dogPosX;
    KeyFrame[FrameIndex].dogPosY = dogPosY;
    KeyFrame[FrameIndex].dogPosZ = dogPosZ;
    KeyFrame[FrameIndex].rotDog = rotDog;
    KeyFrame[FrameIndex].FLegLeft = FLegLeft;
    KeyFrame[FrameIndex].FLegRight = FLegRight;
    KeyFrame[FrameIndex].BLegLeft = BLegLeft;
    KeyFrame[FrameIndex].BLegRight = BLegRight;
    KeyFrame[FrameIndex].head = head;
    KeyFrame[FrameIndex].tail = tail;
    KeyFrame[FrameIndex].torsoHeight = torsoHeight;
    FrameIndex++; // avanza al siguiente slot
}

// Restaura parámetros desde el primer keyframe (inicial)
void resetElements(void) {
    dogPosX = KeyFrame[0].dogPosX;
    dogPosY = KeyFrame[0].dogPosY;
    dogPosZ = KeyFrame[0].dogPosZ;
    rotDog = KeyFrame[0].rotDog;
    FLegLeft = KeyFrame[0].FLegLeft;
    FLegRight = KeyFrame[0].FLegRight;
    BLegLeft = KeyFrame[0].BLegLeft;
    BLegRight = KeyFrame[0].BLegRight;
    head = KeyFrame[0].head;
    tail = KeyFrame[0].tail;
    torsoHeight = KeyFrame[0].torsoHeight;
}

// Calcula incrementos entre el keyframe actual y el siguiente
void interpolation(void) {
    // posición e incremento de posición
    KeyFrame[playIndex].incX = (KeyFrame[playIndex + 1].dogPosX - KeyFrame[playIndex].dogPosX) / i_max_steps;
    KeyFrame[playIndex].incY = (KeyFrame[playIndex + 1].dogPosY - KeyFrame[playIndex].dogPosY) / i_max_steps;
    KeyFrame[playIndex].incZ = (KeyFrame[playIndex + 1].dogPosZ - KeyFrame[playIndex].dogPosZ) / i_max_steps;
    // rotación e incremento de rotación
    KeyFrame[playIndex].rotDogInc = (KeyFrame[playIndex + 1].rotDog - KeyFrame[playIndex].rotDog) / i_max_steps;
    // extremidades y su incremento
    KeyFrame[playIndex].FLegLeftInc = (KeyFrame[playIndex + 1].FLegLeft - KeyFrame[playIndex].FLegLeft) / i_max_steps;
    KeyFrame[playIndex].FLegRightInc = (KeyFrame[playIndex + 1].FLegRight - KeyFrame[playIndex].FLegRight) / i_max_steps;
    KeyFrame[playIndex].BLegLeftInc = (KeyFrame[playIndex + 1].BLegLeft - KeyFrame[playIndex].BLegLeft) / i_max_steps;
    KeyFrame[playIndex].BLegRightInc = (KeyFrame[playIndex + 1].BLegRight - KeyFrame[playIndex].BLegRight) / i_max_steps;
    // cabeza, cola y torso
    KeyFrame[playIndex].headInc = (KeyFrame[playIndex + 1].head - KeyFrame[playIndex].head) / i_max_steps;
    KeyFrame[playIndex].tailInc = (KeyFrame[playIndex + 1].tail - KeyFrame[playIndex].tail) / i_max_steps;
    KeyFrame[playIndex].torsoHeightInc = (KeyFrame[playIndex + 1].torsoHeight - KeyFrame[playIndex].torsoHeight) / i_max_steps;
}

// Genera automáticamente 15 keyframes para la animación de "sentarse"
void createSitAnimation() {
    FrameIndex = 0;     // reset índice
    i_max_steps = 9000; // muchos pasos para suavidad

    const float standingHeight = 0.0f;
    const float sittingHeight = -0.12f;

    // Frame 0%: posición de pie
    FLegLeft = FLegRight = BLegLeft = BLegRight = 0.0f;
    head = 0.0f; tail = 15.0f; torsoHeight = standingHeight;
    rotDog = 0.0f; torsoIncline = 0.0f;
    saveFrame();

    // Frame 8%
    BLegLeft = BLegRight = 5.0f; FLegLeft = 1.25f; FLegRight = -7.5f;
    torsoHeight = standingHeight + sittingHeight * 0.08f;
    rotDog = 2.5f; torsoIncline = -3.75f; head = 2.0f; tail = 17.5f;
    saveFrame();

    // Frame 15%
    BLegLeft = BLegRight = 10.0f; FLegLeft = 2.5f; FLegRight = -15.0f;
    torsoHeight = standingHeight + sittingHeight * 0.15f;
    rotDog = 5.0f; torsoIncline = -7.5f; head = 4.0f; tail = 20.0f;
    saveFrame();

    // Frame 23%
    BLegLeft = BLegRight = 15.0f; FLegLeft = 3.75f; FLegRight = -22.5f;
    torsoHeight = standingHeight + sittingHeight * 0.23f;
    rotDog = 7.5f; torsoIncline = -11.25f; head = 6.0f; tail = 22.5f;
    saveFrame();

    // Frame 30%
    BLegLeft = BLegRight = 20.0f; FLegLeft = 5.0f; FLegRight = -30.0f;
    torsoHeight = standingHeight + sittingHeight * 0.30f;
    rotDog = 10.0f; torsoIncline = -15.0f; head = 8.0f; tail = 25.0f;
    saveFrame();

    // Frame 38%
    BLegLeft = BLegRight = 22.5f; FLegLeft = 6.25f; FLegRight = -33.75f;
    torsoHeight = standingHeight + sittingHeight * 0.38f;
    rotDog = 13.75f; torsoIncline = -18.75f; head = 9.75f; tail = 27.5f;
    saveFrame();

    // Frame 45%
    BLegLeft = BLegRight = 25.0f; FLegLeft = 7.5f; FLegRight = -37.5f;
    torsoHeight = standingHeight + sittingHeight * 0.45f;
    rotDog = 17.5f; torsoIncline = -22.5f; head = 11.5f; tail = 30.0f;
    saveFrame();

    // Frame 53%
    BLegLeft = BLegRight = 27.5f; FLegLeft = 8.75f; FLegRight = -41.25f;
    torsoHeight = standingHeight + sittingHeight * 0.53f;
    rotDog = 21.25f; torsoIncline = -26.25f; head = 13.25f; tail = 32.5f;
    saveFrame();

    // Frame 60%
    BLegLeft = BLegRight = 30.0f; FLegLeft = 10.0f; FLegRight = -45.0f;
    torsoHeight = standingHeight + sittingHeight * 0.60f;
    rotDog = 25.0f; torsoIncline = -30.0f; head = 15.0f; tail = 35.0f;
    saveFrame();

    // Frame 68%
    BLegLeft = BLegRight = 32.5f; FLegLeft = 11.25f; FLegRight = -48.75f;
    torsoHeight = standingHeight + sittingHeight * 0.68f;
    rotDog = 27.5f; torsoIncline = -31.25f; head = 16.25f; tail = 37.5f;
    saveFrame();

    // Frame 75%
    BLegLeft = BLegRight = 35.0f; FLegLeft = 12.5f; FLegRight = -52.5f;
    torsoHeight = standingHeight + sittingHeight * 0.75f;
    rotDog = 30.0f; torsoIncline = -32.5f; head = 17.5f; tail = 40.0f;
    saveFrame();

    // Frame 83%
    BLegLeft = BLegRight = 37.5f; FLegLeft = 13.75f; FLegRight = -56.25f;
    torsoHeight = standingHeight + sittingHeight * 0.83f;
    rotDog = 33.75f; torsoIncline = -33.75f; head = 18.75f; tail = 42.5f;
    saveFrame();

    // Frame 90%
    BLegLeft = BLegRight = 40.0f; FLegLeft = 15.0f; FLegRight = -60.0f;
    torsoHeight = sittingHeight; rotDog = 35.0f; torsoIncline = -35.0f;
    head = 20.0f; tail = 45.0f;
    saveFrame();

    // Frame 97%
    BLegLeft = BLegRight = 40.0f; FLegLeft = 15.0f; FLegRight = -57.5f;
    torsoHeight = sittingHeight + 0.001f; rotDog = 33.5f; torsoIncline = -33.5f;
    head = 19.0f; tail = 45.0f;
    saveFrame();

    // Frame 100%
    BLegLeft = BLegRight = 40.0f; FLegLeft = 15.0f; FLegRight = -55.0f;
    torsoHeight = sittingHeight + 0.001f; rotDog = 32.0f; torsoIncline = -32.0f;
    head = 18.0f; tail = 45.0f;
    saveFrame();
}

// Para cálculo de deltaTime entre frames
GLfloat deltaTime = 0.0f; // tiempo transcurrido desde último frame
GLfloat lastFrame = 0.0f; // instante del último frame

int main() {
    // Inicializa GLFW
    glfwInit();

    // Crea ventana y contexto OpenGL
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Previo 12. keyframes - Garcia Gonzalez Alejandro", nullptr, nullptr);
    if (!window) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate(); // limpia recursos de GLFW
        return EXIT_FAILURE;
    }
    // Vincula contexto y obtiene tamaño real del framebuffer
    glfwMakeContextCurrent(window);
    glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

    // Registra callbacks de entrada
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, MouseCallback);

    // Inicializa GLEW (requiere un contexto activo)
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }

    // Define viewport a todo el framebuffer
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    // Compila y enlaza shaders
    Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
    Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");

    // Carga modelos 3D del perro, pelota y suelo
    Model DogBody((char*)"Models/DogBody.obj");
    Model HeadDog((char*)"Models/HeadDog.obj");
    Model DogTail((char*)"Models/TailDog.obj");
    Model F_RightLeg((char*)"Models/F_RightLegDog.obj");
    Model F_LeftLeg((char*)"Models/F_LeftLegDog.obj");
    Model B_RightLeg((char*)"Models/B_RightLegDog.obj");
    Model B_LeftLeg((char*)"Models/B_LeftLegDog.obj");
    Model Piso((char*)"Models/piso.obj");
    Model Ball((char*)"Models/ball.obj");

    // Inicializa todos los keyframes a cero
    for (int i = 0; i < MAX_FRAMES; ++i) {
        KeyFrame[i] = {}; // todos los valores a 0
    }

    // Genera VAO y VBO para el cubo (lámpara)
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Atributo posición: location=0 en el shader
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // Atributo normal: location=1 en el shader
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),
        (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    // Indica al shader dónde encontrar las texturas (unidad 0 y 1)
    lightingShader.Use();
    glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.difuse"), 0);
    glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.specular"), 1);

    // Matriz de proyección perspectiva
    glm::mat4 projection = glm::perspective(
        camera.GetZoom(),
        (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT,
        0.1f, 100.0f
    );

    // Bucle principal de renderizado
    while (!glfwWindowShouldClose(window)) {
        // Calcula deltaTime
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Procesa eventos de entrada
        glfwPollEvents();
        DoMovement();  // actualiza parámetros según teclas
        Animation();   // interpola y aplica animación si play==true

        // Limpia buffers de color y profundidad
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        // Usa shader de iluminación
        lightingShader.Use();
        // Pasa posición de cámara al shader
        GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
        glUniform3f(viewPosLoc,
            camera.GetPosition().x,
            camera.GetPosition().y,
            camera.GetPosition().z);

        // Configura luz direccional (sol)
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"),
            -0.2f, -1.0f, -0.3f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"),
            0.6f, 0.6f, 0.6f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"),
            0.6f, 0.6f, 0.6f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"),
            0.3f, 0.3f, 0.3f);

        // Luz puntual pulsante (color cambia con el tiempo)
        glm::vec3 lightColor;
        lightColor.x = fabs(sin(glfwGetTime() * Light1.x));
        lightColor.y = fabs(sin(glfwGetTime() * Light1.y));
        lightColor.z = sin(glfwGetTime() * Light1.z);
        // Pasa propiedades de la luz puntual 0
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"),
            pointLightPositions[0].x,
            pointLightPositions[0].y,
            pointLightPositions[0].z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"),
            lightColor.x, lightColor.y, lightColor.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"),
            lightColor.x, lightColor.y, lightColor.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"),
            1.0f, 0.2f, 0.2f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.045f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.075f);

        // SpotLight: linterna acoplada a la cámara
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"),
            camera.GetPosition().x,
            camera.GetPosition().y,
            camera.GetPosition().z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"),
            camera.GetFront().x,
            camera.GetFront().y,
            camera.GetFront().z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"),
            0.2f, 0.2f, 0.8f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"),
            0.2f, 0.2f, 0.8f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"),
            0.0f, 0.0f, 0.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.3f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.7f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"),
            glm::cos(glm::radians(12.0f)));
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"),
            glm::cos(glm::radians(18.0f)));

        // Propiedad de material: brillo (shininess)
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 5.0f);

        // Matrices de vista y proyección
        glm::mat4 view = camera.GetViewMatrix();
        GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
        GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
        GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        // --- Dibujo de escena ---

        // 1) Suelo
        glm::mat4 model = glm::mat4(1.0f); // matriz identidad
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        Piso.Draw(lightingShader);

        // 2) Perro: cuerpo, cabeza, cola, patas
        model = glm::translate(glm::mat4(1.0f),
            glm::vec3(dogPosX, dogPosY + torsoHeight, dogPosZ));
        model = glm::rotate(model, glm::radians(rotDog), glm::vec3(0, 1, 0));      // rotación horizontal
        model = glm::rotate(model, glm::radians(torsoIncline), glm::vec3(1, 0, 0));// inclinación torso
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        DogBody.Draw(lightingShader); // cuerpo

        // Guardamos la transform aplicada al cuerpo para usarla en extremidades
        glm::mat4 modelBase = model;

        // Cabeza
        model = glm::translate(modelBase, glm::vec3(0.0f, 0.093f, 0.208f));
        model = glm::rotate(model, glm::radians(head), glm::vec3(1, 0, 0));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        HeadDog.Draw(lightingShader);

        // Cola
        model = glm::translate(modelBase, glm::vec3(0.0f, 0.026f, -0.288f));
        model = glm::rotate(model, glm::radians(tail), glm::vec3(0, 0, -1));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        DogTail.Draw(lightingShader);

        // Pata delantera izquierda
        model = glm::translate(modelBase, glm::vec3(0.112f, -0.044f, 0.074f));
        model = glm::rotate(model, glm::radians(FLegLeft), glm::vec3(-1, 0, 0));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        F_LeftLeg.Draw(lightingShader);

        // Pata delantera derecha
        model = glm::translate(modelBase, glm::vec3(-0.111f, -0.055f, 0.074f));
        model = glm::rotate(model, glm::radians(FLegRight), glm::vec3(1, 0, 0));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        F_RightLeg.Draw(lightingShader);

        // Pata trasera izquierda
        model = glm::translate(modelBase, glm::vec3(0.082f, -0.046f, -0.218f));
        model = glm::rotate(model, glm::radians(BLegLeft), glm::vec3(-1, 0, 0));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        B_LeftLeg.Draw(lightingShader);

        // Pata trasera derecha
        model = glm::translate(modelBase, glm::vec3(-0.083f, -0.057f, -0.231f));
        model = glm::rotate(model, glm::radians(BLegRight), glm::vec3(-1, 0, 0));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        B_RightLeg.Draw(lightingShader);

        // 3) Pelota con transparencia
        model = glm::mat4(1.0f);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 1);
        model = glm::rotate(model, glm::radians(rotBall), glm::vec3(0, 1, 0));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        Ball.Draw(lightingShader);
        glDisable(GL_BLEND);

        // 4) Lámpara representando la luz puntual
        lampShader.Use();
        modelLoc = glGetUniformLocation(lampShader.Program, "model");
        viewLoc = glGetUniformLocation(lampShader.Program, "view");
        projLoc = glGetUniformLocation(lampShader.Program, "projection");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
        model = glm::translate(glm::mat4(1.0f), pointLightPositions[0]);
        model = glm::scale(model, glm::vec3(0.2f)); // escala pequeña
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        // Intercambia buffers de dibujo (doble buffering)
        glfwSwapBuffers(window);
    }

    // Libera recursos de GLFW y sale
    glfwTerminate();
    return 0;
}

// Manejo de teclas: movimiento del perro, cámara y luz
void DoMovement() {
    // Rotación del perro
    if (keys[GLFW_KEY_2]) rotDog += 1.0f;
    if (keys[GLFW_KEY_3]) rotDog -= 1.0f;

    // Desplazamiento del perro en Z
    if (keys[GLFW_KEY_H]) dogPosZ += 0.01f;
    if (keys[GLFW_KEY_Y]) dogPosZ -= 0.01f;
    // Desplazamiento en X
    if (keys[GLFW_KEY_G]) dogPosX -= 0.01f;
    if (keys[GLFW_KEY_J]) dogPosX += 0.01f;

    // Control de extremidades delanteras (Z/X para izquierda, C/V para derecha)
    if (keys[GLFW_KEY_Z]) FLegLeft = std::min(FLegLeft + 1.0f, 45.0f);
    if (keys[GLFW_KEY_X]) FLegLeft = std::max(FLegLeft - 1.0f, -45.0f);
    if (keys[GLFW_KEY_C]) FLegRight = std::min(FLegRight + 1.0f, 45.0f);
    if (keys[GLFW_KEY_V]) FLegRight = std::max(FLegRight - 1.0f, -45.0f);

    // Extremidades traseras (E/R para izquierda, T/Y para derecha)
    if (keys[GLFW_KEY_E]) BLegLeft = std::min(BLegLeft + 1.0f, 45.0f);
    if (keys[GLFW_KEY_R]) BLegLeft = std::max(BLegLeft - 1.0f, -45.0f);
    if (keys[GLFW_KEY_T]) BLegRight = std::min(BLegRight + 1.0f, 45.0f);
    if (keys[GLFW_KEY_Y]) BLegRight = std::max(BLegRight - 1.0f, -45.0f);

    // Cabeza (U/I) y cola (O/P)
    if (keys[GLFW_KEY_U]) head = std::min(head + 1.0f, 30.0f);
    if (keys[GLFW_KEY_I]) head = std::max(head - 1.0f, -30.0f);
    if (keys[GLFW_KEY_O]) tail = std::min(tail + 1.0f, 45.0f);
    if (keys[GLFW_KEY_P]) tail = std::max(tail - 1.0f, -45.0f);

    // Altura del torso (K/L)
    if (keys[GLFW_KEY_K]) torsoHeight = std::min(torsoHeight + 0.01f, 0.5f);
    if (keys[GLFW_KEY_L]) torsoHeight = std::max(torsoHeight - 0.01f, -0.2f);

    // Controles de la cámara (W/A/S/D y flechas)
    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])    camera.ProcessKeyboard(FORWARD, deltaTime);
    if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])  camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])  camera.ProcessKeyboard(LEFT, deltaTime);
    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT]) camera.ProcessKeyboard(RIGHT, deltaTime);

    // Ajuste posición de la luz puntual (I/K => X, O/L => Y, ;/' => Z)
    if (keys[GLFW_KEY_I]) pointLightPositions[0].x += 0.01f;
    if (keys[GLFW_KEY_K]) pointLightPositions[0].x -= 0.01f;
    if (keys[GLFW_KEY_O]) pointLightPositions[0].y += 0.01f;
    if (keys[GLFW_KEY_L]) pointLightPositions[0].y -= 0.01f;
    if (keys[GLFW_KEY_P]) pointLightPositions[0].z -= 0.10f;
    if (keys[GLFW_KEY_SEMICOLON]) pointLightPositions[0].z += 0.01f;
}

// Callback de teclas: guarda keyframes, inicia/parapusa animación, toggles de luz
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    // Si M se pulsa, inicia o pausa animación guardada
    if (keys[GLFW_KEY_M]) {
        if (!play && FrameIndex > 1) {
            resetElements();   // vuelve a primer frame
            interpolation();   // calcula incrementos
            play = true;       // activa reproducción
            playIndex = 0;
            i_curr_steps = 0;
        }
        else {
            play = false;      // pausa reproducción
        }
    }
    // Si N se pulsa, guarda manualmente un keyframe
    if (keys[GLFW_KEY_N] && FrameIndex < MAX_FRAMES) {
        saveFrame();
    }
    // Si B se pulsa, genera animación de sentarse automáticamente
    if (keys[GLFW_KEY_B]) {
        createSitAnimation();
    }
    // ESC cierra la ventana
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    // Actualiza estado de array de teclas
    if (key >= 0 && key < 1024) {
        if (action == GLFW_PRESS)      keys[key] = true;
        else if (action == GLFW_RELEASE) keys[key] = false;
    }
    // SPACE toggles luz puntual pulsante
    if (keys[GLFW_KEY_SPACE]) {
        active = !active;
        Light1 = active ? glm::vec3(0.2f, 0.8f, 1.0f) : glm::vec3(0.0f);
    }
}

// Reproducción de la animación guardada, con control de velocidad
void Animation() {
    if (!play) return; // si no está en play, no hacer nada

    const float speedFactor = 0.5f; // ajustar velocidad global
    // si llegamos al final de los pasos entre keyframes
    if (i_curr_steps >= i_max_steps) {
        playIndex++;
        if (playIndex > FrameIndex - 2) {
            printf("Animation finished\n");
            play = false;    // fin de la animación
            playIndex = 0;
        }
        else {
            i_curr_steps = 0;
            interpolation(); // recalcula incrementos para siguiente tramo
        }
    }
    else {
        static float accumulatedDelta = 0.0f;
        accumulatedDelta += deltaTime * speedFactor;
        // actualiza 60 veces por segundo aproximado
        if (accumulatedDelta >= 1.0f / 60.0f) {
            // aplica incrementos guardados
            dogPosX += KeyFrame[playIndex].incX;
            dogPosY += KeyFrame[playIndex].incY;
            dogPosZ += KeyFrame[playIndex].incZ;
            rotDog += KeyFrame[playIndex].rotDogInc;
            FLegLeft += KeyFrame[playIndex].FLegLeftInc;
            FLegRight += KeyFrame[playIndex].FLegRightInc;
            BLegLeft += KeyFrame[playIndex].BLegLeftInc;
            BLegRight += KeyFrame[playIndex].BLegRightInc;
            head += KeyFrame[playIndex].headInc;
            tail += KeyFrame[playIndex].tailInc;
            torsoHeight += KeyFrame[playIndex].torsoHeightInc;

            i_curr_steps++;
            accumulatedDelta -= 1.0f / 60.0f;
        }
    }
}

// Callback de movimiento del ratón: rota la cámara
void MouseCallback(GLFWwindow* window, double xPos, double yPos) {
    if (firstMouse) {
        // primera lectura, solo inicializa lastX/Y
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }
    // calcula offset desde la última posición
    GLfloat xOffset = xPos - lastX;
    GLfloat yOffset = lastY - yPos;
    lastX = xPos;
    lastY = yPos;

    // pasa el offset a la cámara para actualizar su orientación
    camera.ProcessMouseMovement(xOffset, yOffset);
}