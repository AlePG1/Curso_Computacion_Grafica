/*
Previo 11                                   García González Alejandro
Fecha de entrega: 20 de abril del 2025      422066992
*/
#include <iostream>
#include <cmath>

// Bibliotecas para OpenGL
#include <GL/glew.h>    // GLEW para cargar extensiones de OpenGL
#include <GLFW/glfw3.h> // GLFW para manejo de ventanas y entrada

// Otras bibliotecas
#include "stb_image.h"  // Para carga de imágenes

// Matemáticas para gráficos
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Para carga de modelos
#include "SOIL2/SOIL2.h"

// Archivos personalizados
#include "Shader.h"   // Clase para manejo de shaders
#include "Camera.h"   // Clase para la cámara
#include "Model.h"    // Clase para manejo de modelos 3D

// Prototipos de funciones
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();
void Animation();

// Dimensiones de la ventana
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT; // Variables para almacenar dimensiones reales

// Constantes para la animación del perro
const float FLOOR_END_Z = 2.5f;    // Posición Z donde termina el piso
const float DOG_MARGIN = 0.2f;     // Margen para que el perro se detenga antes del borde

// Configuración de cámara
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f)); // Posición inicial de la cámara
GLfloat lastX = WIDTH / 2.0;       // Última posición X del mouse
GLfloat lastY = HEIGHT / 2.0;      // Última posición Y del mouse
bool keys[1024];                   // Array para estado de teclas
bool firstMouse = true;            // Flag para primer movimiento del mouse

// Configuración de luces
glm::vec3 lightPos(0.0f, 0.0f, 0.0f); // Posición de la luz
bool active;                        // Estado de la luz

// Posiciones de luces puntuales
glm::vec3 pointLightPositions[] = {
    glm::vec3(0.0f,2.0f, 0.0f),
    glm::vec3(0.0f,0.0f, 0.0f),
    glm::vec3(0.0f,0.0f,  0.0f),
    glm::vec3(0.0f,0.0f, 0.0f)
};

// Vértices para un cubo (usado para representar las luces)
float vertices[] = {
    // Posiciones          // Normales
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

// Variables para animación
glm::vec3 Light1 = glm::vec3(0); // Color de la luz 1

// Variables de animación
float rotBall = 0.0f;    // Rotación de la pelota
bool AnimBall = false;   // Flag para animar pelota
bool AnimDog = false;    // Flag para animar perro
float rotDog = 0.0f;     // Rotación del perro
int dogAnim = 0;         // Estado de animación del perro
bool step = false;       // Flag para paso de animación
float FLegs = 0.0f;     // Ángulo de patas delanteras
float RLegs = 0.0f;      // Ángulo de patas traseras
float head = 0.0f;       // Ángulo de cabeza
float tail = 0.0f;       // Ángulo de cola
glm::vec3 dogPos(0.0f, 0.0f, 0.0f); // Posición del perro
float dogRot = 0.0f;     // Rotación del perro

// Variables para control de tiempo
GLfloat deltaTime = 0.0f;    // Tiempo entre frames
GLfloat lastFrame = 0.0f;    // Tiempo del último frame

int main()
{
    // Inicialización de GLFW
    glfwInit();

    // Creación de la ventana GLFW
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Animacion maquina de estados - Garcia Gonzalez Alejandro", nullptr, nullptr);

    // Verificación de creación de ventana
    if (nullptr == window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    // Establecer la ventana como contexto actual
    glfwMakeContextCurrent(window);

    // Obtener dimensiones reales del framebuffer
    glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

    // Configuración de callbacks
    glfwSetKeyCallback(window, KeyCallback);        // Callback para teclado
    glfwSetCursorPosCallback(window, MouseCallback); // Callback para mouse

    // Inicialización de GLEW
    glewExperimental = GL_TRUE;
    if (GLEW_OK != glewInit())
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }

    // Configuración del viewport
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    // Carga de shaders
    Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag"); // Shader para iluminación
    Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");             // Shader para lámparas

    // Carga de modelos 3D
    Model DogBody((char*)"Models/DogBody.obj");
    Model HeadDog((char*)"Models/HeadDog.obj");
    Model DogTail((char*)"Models/TailDog.obj");
    Model F_RightLeg((char*)"Models/F_RightLegDog.obj");
    Model F_LeftLeg((char*)"Models/F_LeftLegDog.obj");
    Model B_RightLeg((char*)"Models/B_RightLegDog.obj");
    Model B_LeftLeg((char*)"Models/B_LeftLegDog.obj");
    Model Piso((char*)"Models/piso.obj");
    Model Ball((char*)"Models/ball.obj");

    // Configuración de VAO y VBO para el cubo de luz
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Enlazar VAO y VBO
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Atributos de posición
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // Atributos de normales
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Configuración de unidades de textura
    lightingShader.Use();
    glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.difuse"), 0);
    glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.specular"), 1);

    // Configuración de proyección perspectiva
    glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);

    // Bucle principal del juego
    while (!glfwWindowShouldClose(window))
    {
        // Cálculo del tiempo entre frames
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Procesamiento de eventos
        glfwPollEvents();
        DoMovement();   // Movimiento de cámara
        Animation();    // Animaciones

        // Limpieza del buffer
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Habilitar profundidad
        glEnable(GL_DEPTH_TEST);

        // Matriz temporal para transformaciones
        glm::mat4 modelTemp = glm::mat4(1.0f);

        // Usar shader de iluminación
        lightingShader.Use();

        // Configuración de texturas
        glUniform1i(glGetUniformLocation(lightingShader.Program, "diffuse"), 0);

        // Configuración de posición de vista
        GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
        glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

        // Configuración de luz direccional
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.6f, 0.6f, 0.6f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.6f, 0.6f, 0.6f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.3f, 0.3f, 0.3f);

        // Configuración de luz puntual 1 (con animación de color)
        glm::vec3 lightColor;
        lightColor.x = abs(sin(glfwGetTime() * Light1.x));
        lightColor.y = abs(sin(glfwGetTime() * Light1.y));
        lightColor.z = sin(glfwGetTime() * Light1.z);

        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), lightColor.x, lightColor.y, lightColor.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), lightColor.x, lightColor.y, lightColor.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 1.0f, 0.2f, 0.2f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.045f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.075f);

        // Configuración de spotlight (foco)
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.2f, 0.2f, 0.8f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.2f, 0.2f, 0.8f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 0.0f, 0.0f, 0.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.3f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.7f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.0f)));
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(18.0f)));

        // Configuración de material
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 5.0f);

        // Obtener matriz de vista
        glm::mat4 view = camera.GetViewMatrix();

        // Obtener ubicaciones de uniformes
        GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
        GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
        GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

        // Pasar matrices al shader
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        // Dibujar modelos
        glm::mat4 model(1);

        // Dibujar piso
        view = camera.GetViewMatrix();
        model = glm::mat4(1);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        Piso.Draw(lightingShader);

        // Dibujar perro
        model = glm::mat4(1);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);

        // Cuerpo del perro
        modelTemp = model = glm::translate(model, dogPos);
        modelTemp = model = glm::rotate(model, glm::radians(dogRot), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        DogBody.Draw(lightingShader);

        // Cabeza del perro
        model = modelTemp;
        model = glm::translate(model, glm::vec3(0.0f, 0.093f, 0.208f));
        model = glm::rotate(model, glm::radians(head), glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        HeadDog.Draw(lightingShader);

        // Cola del perro
        model = modelTemp;
        model = glm::translate(model, glm::vec3(0.0f, 0.026f, -0.288f));
        model = glm::rotate(model, glm::radians(tail), glm::vec3(0.0f, 0.0f, -1.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        DogTail.Draw(lightingShader);

        // Pata delantera izquierda
        model = modelTemp;
        model = glm::translate(model, glm::vec3(0.112f, -0.044f, 0.074f));
        model = glm::rotate(model, glm::radians(FLegs), glm::vec3(-1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        F_LeftLeg.Draw(lightingShader);

        // Pata delantera derecha
        model = modelTemp;
        model = glm::translate(model, glm::vec3(-0.111f, -0.055f, 0.074f));
        model = glm::rotate(model, glm::radians(FLegs), glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        F_RightLeg.Draw(lightingShader);

        // Pata trasera izquierda
        model = modelTemp;
        model = glm::translate(model, glm::vec3(0.082f, -0.046, -0.218));
        model = glm::rotate(model, glm::radians(RLegs), glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        B_LeftLeg.Draw(lightingShader);

        // Pata trasera derecha
        model = modelTemp;
        model = glm::translate(model, glm::vec3(-0.083f, -0.057f, -0.231f));
        model = glm::rotate(model, glm::radians(RLegs), glm::vec3(-1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        B_RightLeg.Draw(lightingShader);

        // Dibujar pelota (con transparencia)
        model = glm::mat4(1);
        glEnable(GL_BLEND); // Activar mezcla de colores para transparencia
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 1);
        model = glm::rotate(model, glm::radians(rotBall), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        Ball.Draw(lightingShader);
        glDisable(GL_BLEND); // Desactivar transparencia

        // Dibujar fuente de luz (usando shader simple)
        lampShader.Use();
        modelLoc = glGetUniformLocation(lampShader.Program, "model");
        viewLoc = glGetUniformLocation(lampShader.Program, "view");
        projLoc = glGetUniformLocation(lampShader.Program, "projection");

        // Pasar matrices al shader de luz
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        // Dibujar cubo representando la luz
        model = glm::mat4(1);
        model = glm::translate(model, pointLightPositions[0]);
        model = glm::scale(model, glm::vec3(0.2f)); // Escalar el cubo
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Intercambiar buffers
        glfwSwapBuffers(window);
    }

    // Limpieza final
    glfwTerminate();
    return 0;
}

// Función para manejar movimiento de cámara
void DoMovement()
{
    // Movimiento hacia adelante
    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
    {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }

    // Movimiento hacia atrás
    if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
    {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }

    // Movimiento a la izquierda
    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
    {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }

    // Movimiento a la derecha
    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
    {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }

    // Controles para mover la luz en X
    if (keys[GLFW_KEY_T]) pointLightPositions[0].x += 0.01f;
    if (keys[GLFW_KEY_G]) pointLightPositions[0].x -= 0.01f;

    // Controles para mover la luz en Y
    if (keys[GLFW_KEY_Y]) pointLightPositions[0].y += 0.01f;
    if (keys[GLFW_KEY_H]) pointLightPositions[0].y -= 0.01f;

    // Controles para mover la luz en Z
    if (keys[GLFW_KEY_U]) pointLightPositions[0].z -= 0.1f;
    if (keys[GLFW_KEY_J]) pointLightPositions[0].z += 0.01f;
}

// Callback para teclado
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    // Salir con ESC
    if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    // Registrar estado de teclas
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
        {
            keys[key] = true;
        }
        else if (action == GLFW_RELEASE)
        {
            keys[key] = false;
        }
    }

    // Activar/desactivar luz con espacio
    if (keys[GLFW_KEY_SPACE])
    {
        active = !active;
        if (active)
        {
            Light1 = glm::vec3(0.2f, 0.8f, 1.0f); // Luz azul-verde
        }
        else
        {
            Light1 = glm::vec3(0); // Apagar luz
        }
    }

    // Activar/desactivar animación de pelota con N
    if (keys[GLFW_KEY_N])
    {
        AnimBall = !AnimBall;
    }

    // Activar animación de perro con B
    if (keys[GLFW_KEY_B])
    {
        dogAnim = 1;
    }
}

// Función para manejar animaciones
void Animation() {
    // Animación de pelota (rotación)
    if (AnimBall)
    {
        rotBall += 0.4f;
    }

    // Animación de perro (rotación)
    if (AnimDog)
    {
        rotDog -= 0.6f;
    }

    // Animación de caminata del perro
    if (dogAnim == 1) {  // Si la animación del perro está activada (dogAnim es 1)
        // Movimiento de patas y cabeza en dos fases (ida y vuelta)
        if (!step) {  // Si la variable step es false, significa que el perro está en la fase de mover las patas hacia adelante
            RLegs += 10.0f * deltaTime;  // Aumenta el ángulo de la pata trasera derecha para simular el movimiento
            FLegs += 10.0f * deltaTime;  // Aumenta el ángulo de la pata delantera derecha
            head += 10.0f * deltaTime;   // Aumenta el ángulo de la cabeza para que se mueva hacia adelante
            tail += 10.0f * deltaTime;   // Aumenta el ángulo de la cola para que se mueva con el cuerpo
            if (RLegs > 15.0f) {  // Si el ángulo de la pata trasera derecha supera 15 grados, cambiar la fase de movimiento
                step = true;  // Cambiar a la fase de mover las patas hacia atrás
            }
        }
        else {  // Si la variable step es true, significa que el perro está en la fase de mover las patas hacia atrás
            RLegs -= 10.0f * deltaTime;  // Disminuye el ángulo de la pata trasera derecha para simular el movimiento hacia atrás
            FLegs -= 10.0f * deltaTime;  // Disminuye el ángulo de la pata delantera derecha
            head -= 10.0f * deltaTime;   // Disminuye el ángulo de la cabeza para que se mueva hacia atrás
            tail -= 10.0f * deltaTime;   // Disminuye el ángulo de la cola para que se mueva con el cuerpo
            if (RLegs < -15.0f) {  // Si el ángulo de la pata trasera derecha es menor a -15 grados, cambiar la fase de movimiento
                step = false;  // Cambiar a la fase de mover las patas hacia adelante
            }
        }

        // Mover perro hacia adelante
        dogPos.z += 0.1f * deltaTime;  // Avanza la posición del perro en el eje Z (hacia adelante) dependiendo del tiempo transcurrido entre frames

        // Verificar límite de movimiento
        float stopZ = FLOOR_END_Z - DOG_MARGIN;  // Calcula la posición límite en Z, con un margen de seguridad
        if (dogPos.z < stopZ) {  // Si la posición Z del perro es menor que el límite de stopZ
            dogPos.z += 0.1f * deltaTime;  // Sigue avanzando el perro hacia adelante
        }
        else {  // Si el perro ha llegado al límite
            dogPos.z = stopZ;  // Coloca al perro exactamente en el límite
            dogAnim = 0;  // Detiene la animación del perro
        }
    }

}

// Callback para movimiento del mouse
void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
    // Manejar primer movimiento
    if (firstMouse)
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    // Calcular desplazamiento
    GLfloat xOffset = xPos - lastX;
    GLfloat yOffset = lastY - yPos; // Invertido porque las coordenadas Y van de abajo hacia arriba

    // Actualizar última posición
    lastX = xPos;
    lastY = yPos;

    // Mover cámara según movimiento del mouse
    camera.ProcessMouseMovement(xOffset, yOffset);
}