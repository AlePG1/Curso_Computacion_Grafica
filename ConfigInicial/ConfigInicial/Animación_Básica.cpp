/*
* García González Alejandro       422066992
* 09 de abril del 2025            Practica 10. Animación Básica
*/

// Inclusión de bibliotecas estándar
#include <iostream>  // Para entrada/salida estándar
#include <cmath>     // Para funciones matemáticas

// Bibliotecas para gráficos 3D
#include <GL/glew.h>      // Manejo de extensiones OpenGL
#include <GLFW/glfw3.h>   // Biblioteca para creación de ventanas y manejo de inputs
#include "stb_image.h"    // Para carga de texturas

// Biblioteca matemática para gráficos
#include <glm/glm.hpp>              // Core de GLM
#include <glm/gtc/matrix_transform.hpp> // Transformaciones matemáticas
#include <glm/gtc/type_ptr.hpp>     // Conversión de tipos

// Para carga de modelos
#include "SOIL2/SOIL2.h"

// Archivos personalizados
#include "Shader.h"   // Clase para manejo de shaders
#include "Camera.h"   // Clase para manejo de cámara
#include "Model.h"    // Clase para manejo de modelos 3D

// Prototipos de funciones
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);  // Callback para teclado
void MouseCallback(GLFWwindow* window, double xPos, double yPos);  // Callback para mouse
void DoMovement();  // Función para manejar movimiento
void Animation();   // Función para manejar animaciones

// Dimensiones de la ventana
const GLuint WIDTH = 800, HEIGHT = 600;  // Ancho y alto iniciales
int SCREEN_WIDTH, SCREEN_HEIGHT;         // Para almacenar dimensiones reales

// Configuración de cámara
Camera  camera(glm::vec3(0.0f, 0.0f, 3.0f));  // Cámara en posición (0,0,3)
GLfloat lastX = WIDTH / 2.0;  // Última posición X del mouse
GLfloat lastY = HEIGHT / 2.0; // Última posición Y del mouse
bool keys[1024];              // Array para estado de teclas
bool firstMouse = true;       // Flag para primer movimiento del mouse

// Configuración de luces
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);  // Posición de luz principal
bool active;                           // Estado de luz activa/inactiva

// Posiciones de luces puntuales
glm::vec3 pointLightPositions[] = {
    glm::vec3(0.0f,2.0f, 0.0f),  // Luz 1
    glm::vec3(0.0f,0.0f, 0.0f),  // Luz 2
    glm::vec3(0.0f,0.0f,  0.0f), // Luz 3
    glm::vec3(0.0f,0.0f, 0.0f)   // Luz 4
};

// Definición de vértices para un cubo (posición x,y,z y normal x,y,z)
float vertices[] = {
    // Cara frontal
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

    // Cara trasera
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

    // Cara izquierda
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

    // Cara derecha
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

     // Cara inferior
     -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
      0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
      0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
      0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

     // Cara superior
     -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
      0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
      0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
      0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

// Variable para control de luz
glm::vec3 Light1 = glm::vec3(0);  // Color de luz inicial (apagada)

// Variables para animación del perro
float  posIniD = 0.0f;       // Posición inicial del perro
float posIniB = 3.5f;         // Posición inicial de la pelota
float posXD = -1.0f;          // Posición X actual del perro
float posyD = 0.0f;           // Posición Y actual del perro
float poszD = -1.0f;          // Posición Z actual del perro
float rotDog = posIniD;       // Rotación del perro
bool AnimDog = false;         // Flag para animación del perro activa/inactiva
float angulo = 0.0f;          // Ángulo de rotación del perro

// Variables para animación de la pelota
float posXB = 1.0f;           // Posición X actual de la pelota
float posYB = 0.0f;           // Posición Y actual de la pelota
float posZB = 1.0f;           // Posición Z actual de la pelota
float rotBall = posIniB;      // Rotación de la pelota
bool AnimBall = false;        // Flag para animación de pelota activa/inactiva

// Variables para el salto
bool jump = false;            // Control del salto (activo/inactivo)
bool controlUp = true;        // Control de dirección del salto (subida/bajada)
float velocidadS = 1.0f;      // Velocidad del salto

// Variables para control de tiempo entre frames
GLfloat deltaTime = 0.0f;     // Tiempo entre frame actual y anterior
GLfloat lastFrame = 0.0f;     // Tiempo del último frame

// Función principal
int main()
{
    // Inicialización de GLFW
    glfwInit();

    // Creación de ventana
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Animacion basica Garcia Gonzalez Alejandro", nullptr, nullptr);
    if (nullptr == window) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    // Configuración de contexto actual
    glfwMakeContextCurrent(window);

    // Obtención de dimensiones reales del framebuffer
    glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

    // Configuración de callbacks
    glfwSetKeyCallback(window, KeyCallback);        // Para teclado
    glfwSetCursorPosCallback(window, MouseCallback); // Para mouse

    // Inicialización de GLEW
    glewExperimental = GL_TRUE;
    if (GLEW_OK != glewInit()) {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }

    // Configuración del viewport
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    // Carga de shaders
    Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag"); // Shader de iluminación
    Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");             // Shader de lámpara

    // Carga de modelos 3D
    Model Dog((char*)"Models/RedDog.obj");  // Modelo del perro
    Model Piso((char*)"Models/piso.obj");   // Modelo del piso
    Model Ball((char*)"Models/ball.obj");   // Modelo de la pelota

    // Configuración de VAO y VBO para el cubo de luz
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);  // Generar VAO
    glGenBuffers(1, &VBO);       // Generar VBO

    // Configuración del VAO
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Atributo de posición (location = 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // Atributo de normal (location = 1)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Configuración de unidades de textura
    lightingShader.Use();
    glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.difuse"), 0);
    glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.specular"), 1);

    // Configuración de matriz de proyección
    glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);

    // Loop principal de renderizado
    while (!glfwWindowShouldClose(window)) {
        // Cálculo del deltaTime
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Manejo de eventos
        glfwPollEvents();  // Procesar eventos pendientes
        DoMovement();      // Manejar movimiento de cámara
        Animation();       // Actualizar animaciones

        // Limpieza del buffer de color y profundidad
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Habilitar prueba de profundidad
        glEnable(GL_DEPTH_TEST);

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

        // Configuración de luz puntual 1 (con efecto de parpadeo)
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

        // Propiedades del material
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 5.0f);

        // Transformaciones de cámara
        glm::mat4 view = camera.GetViewMatrix();
        GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
        GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
        GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

        // Pasar matrices al shader
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        // Dibujar piso
        glm::mat4 model(1);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        Piso.Draw(lightingShader);

        // Dibujar perro
        model = glm::mat4(1);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
        model = glm::translate(model, glm::vec3(posXD, posyD, poszD));
        model = glm::rotate(model, angulo, glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        Dog.Draw(lightingShader);

        // Dibujar pelota (con transparencia)
        model = glm::mat4(1);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 1);
        model = glm::translate(model, glm::vec3(posXB, posYB, posZB));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        Ball.Draw(lightingShader);
        glDisable(GL_BLEND);

        // Dibujar fuente de luz
        lampShader.Use();
        modelLoc = glGetUniformLocation(lampShader.Program, "model");
        viewLoc = glGetUniformLocation(lampShader.Program, "view");
        projLoc = glGetUniformLocation(lampShader.Program, "projection");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
        model = glm::mat4(1);
        model = glm::translate(model, pointLightPositions[0]);
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        // Intercambiar buffers
        glfwSwapBuffers(window);
    }

    // Limpieza final
    glfwTerminate();
    return 0;
}

// Función para manejar movimiento con teclado
void DoMovement()
{
    // Movimiento hacia adelante
    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP]) {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    // Movimiento hacia atrás
    if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN]) {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    // Movimiento hacia izquierda
    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT]) {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }
    // Movimiento hacia derecha
    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT]) {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }

    // Controles para mover la luz puntual
    if (keys[GLFW_KEY_T]) pointLightPositions[0].x += 0.01f;
    if (keys[GLFW_KEY_G]) pointLightPositions[0].x -= 0.01f;
    if (keys[GLFW_KEY_Y]) pointLightPositions[0].y += 0.01f;
    if (keys[GLFW_KEY_H]) pointLightPositions[0].y -= 0.01f;
    if (keys[GLFW_KEY_U]) pointLightPositions[0].z -= 0.1f;
    if (keys[GLFW_KEY_J]) pointLightPositions[0].z += 0.01f;
}

// Callback para teclado
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    // Cerrar ventana con ESC
    if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    // Registrar estado de teclas
    if (key >= 0 && key < 1024) {
        if (action == GLFW_PRESS) {
            keys[key] = true;
        }
        else if (action == GLFW_RELEASE) {
            keys[key] = false;
        }
    }

    // Alternar luz con espacio
    if (keys[GLFW_KEY_SPACE]) {
        active = !active;
        Light1 = active ? glm::vec3(1.0f, 1.0f, 0.0f) : glm::vec3(0);
    }

    // Alternar animaciones con P
    if (keys[GLFW_KEY_P]) {
        AnimBall = !AnimBall;
        AnimDog = !AnimDog;
        jump = false;
    }
}

// Función de animación
void Animation() {
    const float baseSpeed = 1.5f;        // Velocidad base de animación
    const float jumpHeight = 0.5f;       // Altura máxima del salto
    const float orbitRadius = 2.0f;      // Radio de la órbita circular
    const float twoPi = 2.0f * glm::pi<float>();  // 2π para cálculos

    // Animación de la pelota
    if (AnimBall) {
        rotBall -= baseSpeed * deltaTime;  // Actualizar ángulo
        rotBall = fmod(rotBall, twoPi);    // Mantener dentro de 0-2π
        if (rotBall < 0) rotBall += twoPi; // Asegurar positivo

        // Calcular nueva posición (movimiento circular)
        posXB = orbitRadius * cos(rotBall);
        posZB = orbitRadius * sin(rotBall);
    }

    // Animación del perro
    if (AnimDog) {
        rotDog += baseSpeed * deltaTime;  // Actualizar ángulo
        rotDog = fmod(rotDog, twoPi);     // Mantener dentro de 0-2π
        if (rotDog < 0) rotDog += twoPi;  // Asegurar positivo

        // Calcular nueva posición (movimiento circular)
        posXD = orbitRadius * cos(rotDog);
        poszD = orbitRadius * sin(rotDog);

        // Calcular ángulo de rotación para que mire en dirección de movimiento
        angulo = atan2(-sin(rotDog), cos(rotDog));
    }

    // Detección de colisión para el salto
    if (AnimDog && !jump) {
        float distance = glm::length(glm::vec2(posXD - posXB, poszD - posZB));
        if (distance < 0.3f * orbitRadius) {  // Si están cerca
            jump = true;
            controlUp = true;
        }
    }

    // Sistema de salto
    if (jump) {
        static float jumpProgress = 0.0f;
        jumpProgress += baseSpeed * 3.0f * deltaTime;  // Avanzar progreso del salto

        // Movimiento parabólico suave usando función seno
        posyD = jumpHeight * sin(glm::clamp(jumpProgress, 0.0f, 1.0f) * glm::pi<float>());
        posYB = posyD;  // Pelota sigue al perro

        // Finalizar salto
        if (jumpProgress >= 1.0f) {
            jump = false;
            jumpProgress = 0.0f;
            posyD = 0.0f;
            posYB = 0.0f;
        }
    }
}

// Callback para movimiento del mouse
void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
    // Para primer movimiento, solo actualizar última posición
    if (firstMouse) {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    // Calcular desplazamiento desde última posición
    GLfloat xOffset = xPos - lastX;
    GLfloat yOffset = lastY - yPos;  // Invertido porque las coordenadas Y van de abajo hacia arriba

    // Actualizar última posición
    lastX = xPos;
    lastY = yPos;

    // Procesar movimiento de cámara
    camera.ProcessMouseMovement(xOffset, yOffset);
}