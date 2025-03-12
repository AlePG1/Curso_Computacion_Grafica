/*
* Garc�a Gonz�lez Alejandro        422066992
* Pr�ctica 6.                      Fecha de entrega: 11 de marzo del 2024
* */
//Std. Includes
#include <string> // Incluye la librer�a est�ndar para el uso de strings

// GLEW
#include <GL/glew.h> // Incluye GLEW, que gestiona extensiones de OpenGL

// GLFW
#include <GLFW/glfw3.h> // Incluye GLFW, usado para crear ventanas y manejar eventos

// GL includes
#include "Shader.h" // Incluye la clase Shader para manejo de shaders
#include "Camera.h" // Incluye la clase Camera para la c�mara en la escena
#include "Model.h"  // Incluye la clase Model para la carga y dibujo de modelos 3D

// GLM Mathemtics
#include <glm/glm.hpp>               // Incluye tipos y funciones b�sicas de GLM
#include <glm/gtc/matrix_transform.hpp> // Incluye transformaciones como translate, rotate, scale
#include <glm/gtc/type_ptr.hpp>         // Incluye utilidades para convertir matrices a punteros

// Other Libs
#include "SOIL2/SOIL2.h" // Librer�a para el manejo de im�genes (texturas)
#include "stb_image.h"    // Librer�a para cargar im�genes

// Properties
const GLuint WIDTH = 800, HEIGHT = 600; // Ancho y alto de la ventana en p�xeles
int SCREEN_WIDTH, SCREEN_HEIGHT;        // Variables para almacenar el tama�o real del framebuffer

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode); // Prototipo de funci�n para manejo de teclado
void MouseCallback(GLFWwindow* window, double xPos, double yPos);                  // Prototipo de funci�n para manejo de mouse
void DoMovement();                                                                 // Prototipo de funci�n para manejar el movimiento

// Camera
Camera camera(glm::vec3(0.0f, 10.0f, 3.0f)); // Se crea la c�mara con posici�n inicial
bool keys[1024];                            // Arreglo para almacenar el estado de las teclas
GLfloat lastX = 400, lastY = 300;           // Coordenadas iniciales del mouse
bool firstMouse = true;                     // Bandera para saber si es la primera lectura del mouse

GLfloat deltaTime = 0.0f;   // Delta time entre frames
GLfloat lastFrame = 0.0f;   // Tiempo del frame anterior

int main()
{
    // Init GLFW
    glfwInit(); // Inicializa GLFW
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // Versi�n mayor de OpenGL: 3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // Versi�n menor de OpenGL: 3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Perfil de OpenGL: Core
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);           // Compatibilidad hacia delante
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);                      // Desactiva la opci�n de redimensionar la ventana

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Practica 6 Garcia Gonzalez Alejandro", nullptr, nullptr); // Crea la ventana

    if (nullptr == window) // Verifica si la ventana se cre� correctamente
    {
        std::cout << "Failed to create GLFW window" << std::endl; // Mensaje de error en caso de fallo
        glfwTerminate(); // Termina la ejecuci�n de GLFW

        return EXIT_FAILURE; // Retorna fallo
    }

    glfwMakeContextCurrent(window); // Hace de esta ventana el contexto actual de OpenGL

    glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT); // Obtiene el tama�o real del framebuffer y lo almacena

    // Set the required callback functions
    glfwSetKeyCallback(window, KeyCallback);           // Asigna la funci�n de callback para teclado
    glfwSetCursorPosCallback(window, MouseCallback);   // Asigna la funci�n de callback para mouse

    // GLFW Options
    //glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_DISABLED ); // Opci�n para ocultar el cursor y capturarlo

    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE; // Habilita GLEW experimental para asegurar compatibilidad
    // Initialize GLEW to setup the OpenGL Function pointers
    if (GLEW_OK != glewInit())  // Inicializa GLEW y verifica si fue exitoso
    {
        std::cout << "Failed to initialize GLEW" << std::endl; // Mensaje de error si falla
        return EXIT_FAILURE; // Retorna fallo
    }

    // Define the viewport dimensions
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT); // Define la parte de la ventana a usar para dibujar

    // OpenGL options
    glEnable(GL_DEPTH_TEST); // Habilita la prueba de profundidad

    // Setup and compile our shaders
    Shader shader("Shader/modelLoading.vs", "Shader/modelLoading.frag"); // Carga y compila los shaders

    // Load models
    Model desierto((char*)"Models/en_el_desierto_de_yemen.obj");                     // Carga el modelo del desierto
    Model dog((char*)"Models/RedDog.obj");                                           // Carga el modelo de perro
    Model car((char*)"Models/mercedes-benz_sl_65_amg_black_series_r230.obj");        // Carga el modelo de coche
    Model estatua((char*)"Models/Estatua/estatua__statue.obj");                      // Carga el modelo de estatua
    Model cactus((char*)"Models/Cactus/flowering_cactus.obj");                       // Carga el modelo de cactus
    Model rango((char*)"Models/Rango/modern_luxury_wedding_arch_house_building_design.obj"); // Carga el modelo de la casa
    Model fig1((char*)"Models/Fig1/light_tactical_vehicle_aquus_areg.obj");          // Carga el modelo de veh�culo
    glm::mat4 projection = glm::perspective(camera.GetZoom(), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
    // Crea la matriz de proyecci�n con la relaci�n de aspecto y el rango de visi�n

    // Game loop
    while (!glfwWindowShouldClose(window)) // Mientras la ventana no deba cerrar
    {
        // Set frame time
        GLfloat currentFrame = glfwGetTime();   // Tiempo actual
        deltaTime = currentFrame - lastFrame;   // Delta time entre frames
        lastFrame = currentFrame;               // Actualiza el tiempo del �ltimo frame

        // Check and call events
        glfwPollEvents(); // Procesa eventos (teclado, mouse, etc.)
        DoMovement();     // Maneja el movimiento de la c�mara

        // Clear the colorbuffer
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f); // Asigna color de limpieza (gris)
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Limpia el buffer de color y el de profundidad

        shader.Use(); // Usa el shader previamente compilado

        glm::mat4 view = camera.GetViewMatrix(); // Obtiene la matriz de vista desde la c�mara
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        // Env�a la matriz de proyecci�n al shader
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        // Env�a la matriz de vista al shader

        // Draw the loaded model
        glm::mat4 model(1); // Crea la matriz de modelo como identidad
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        // Env�a la matriz de modelo (identidad) al shader

        // Matriz para el desierto: lo colocamos en el suelo (Y = 0)
        glm::mat4 modelDesierto = glm::mat4(1.0f);                                                   // Inicia matriz en identidad
        modelDesierto = glm::translate(modelDesierto, glm::vec3(0.5f, 0.0f, 0.5f));                   // Traslada el desierto
        modelDesierto = glm::scale(modelDesierto, glm::vec3(0.7f, 0.7f, 0.7f));                       // Escala el desierto
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelDesierto));
        // Env�a la matriz de modelo al shader
        desierto.Draw(shader); // Dibuja el modelo del desierto

        // Matriz para el perro: lo trasladamos para que quede arriba del desierto (Y > 0)
        glm::mat4 modelPerro = glm::mat4(1.0f);                                                      // Inicia matriz en identidad
        modelPerro = glm::translate(modelPerro, glm::vec3(-3.0f, 7.5f, 0.0f));                        // Traslada el perro
        modelPerro = glm::scale(modelPerro, glm::vec3(2.0f, 3.0f, 2.0f));                             // Escala el perro
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelPerro));
        // Env�a la matriz al shader
        dog.Draw(shader); // Dibuja el modelo del perro

        glm::mat4 modelCar = glm::mat4(1.0f);                                                         // Inicia matriz en identidad
        modelCar = glm::translate(modelCar, glm::vec3(-3.0f, 6.5f, 0.0f));                            // Traslada el coche
        modelCar = glm::scale(modelCar, glm::vec3(0.3f, 0.3f, 0.3f));                                 // Escala el coche
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelCar));
        // Env�a la matriz al shader
        car.Draw(shader); // Dibuja el modelo del coche

        glm::mat4 modelEstatua = glm::mat4(1.0f);                                                     // Inicia matriz en identidad
        modelEstatua = glm::translate(modelEstatua, glm::vec3(-10.0f, 6.5f, 0.0f));                   // Traslada la estatua
        modelEstatua = glm::scale(modelEstatua, glm::vec3(0.9f, 0.9f, 0.9f));                         // Escala la estatua
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelEstatua));
        // Env�a la matriz al shader
        estatua.Draw(shader); // Dibuja el modelo de la estatua

        glm::mat4 modelCactus = glm::mat4(1.0f);                                                      // Inicia matriz en identidad
        modelCactus = glm::translate(modelCactus, glm::vec3(-5.0f, 6.5f, 0.0f));                      // Traslada el cactus
        modelCactus = glm::scale(modelCactus, glm::vec3(2.9f, 2.9f, 2.9f));                           // Escala el cactus
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelCactus));
        // Env�a la matriz al shader
        cactus.Draw(shader); // Dibuja el modelo del cactus

        glm::mat4 modelRango = glm::mat4(1.0f);                                                       // Inicia matriz en identidad
        modelRango = glm::translate(modelRango, glm::vec3(40.0f, 7.0f, -20.0f));                      // Traslada la construcci�n
        modelRango = glm::scale(modelRango, glm::vec3(2.9f, 2.9f, 2.9f));                             // Escala la construcci�n
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelRango));
        // Env�a la matriz al shader
        rango.Draw(shader); // Dibuja el modelo de la construcci�n

        glm::mat4 modelFig1 = glm::mat4(1.0f);                                                        // Inicia matriz en identidad
        modelFig1 = glm::translate(modelFig1, glm::vec3(-15.0f, 7.5f, 0.0f));                         // Traslada el veh�culo
        modelFig1 = glm::scale(modelFig1, glm::vec3(1.0f, 1.0f, 1.0f));                               // Escala el veh�culo
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelFig1));
        // Env�a la matriz al shader
        fig1.Draw(shader); // Dibuja el modelo del veh�culo

        // Swap the buffers
        glfwSwapBuffers(window); // Intercambia los buffers de dibujo
    }

    glfwTerminate(); // Cierra GLFW y limpia los recursos
    return 0;        // Retorna �xito
}

// Moves/alters the camera positions based on user input
void DoMovement()
{
    // Camera controls
    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP]) // Si se presiona W o flecha arriba
    {
        camera.ProcessKeyboard(FORWARD, deltaTime); // Mueve la c�mara hacia adelante
    }

    if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN]) // Si se presiona S o flecha abajo
    {
        camera.ProcessKeyboard(BACKWARD, deltaTime); // Mueve la c�mara hacia atr�s
    }

    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT]) // Si se presiona A o flecha izquierda
    {
        camera.ProcessKeyboard(LEFT, deltaTime); // Mueve la c�mara a la izquierda
    }

    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT]) // Si se presiona D o flecha derecha
    {
        camera.ProcessKeyboard(RIGHT, deltaTime); // Mueve la c�mara a la derecha
    }
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action) // Si se presiona la tecla ESC
    {
        glfwSetWindowShouldClose(window, GL_TRUE); // Indica que la ventana debe cerrarse
    }

    if (key >= 0 && key < 1024) // Si la tecla est� dentro del rango v�lido
    {
        if (action == GLFW_PRESS) // Si se presiona la tecla
        {
            keys[key] = true;    // Se marca como verdadera en el arreglo keys
        }
        else if (action == GLFW_RELEASE) // Si se suelta la tecla
        {
            keys[key] = false;   // Se marca como falsa en el arreglo keys
        }
    }
}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
    if (firstMouse) // Verifica si es la primera lectura de mouse
    {
        lastX = xPos;  // Asigna la posici�n actual del mouse
        lastY = yPos;  // Asigna la posici�n actual del mouse
        firstMouse = false; // Marca que ya no es la primera lectura
    }

    GLfloat xOffset = xPos - lastX; // Calcula la diferencia en X
    GLfloat yOffset = lastY - yPos; // Calcula la diferencia en Y (invertido porque el eje Y va de abajo a arriba)

    lastX = xPos; // Actualiza la posici�n anterior del mouse en X
    lastY = yPos; // Actualiza la posici�n anterior del mouse en Y

    camera.ProcessMouseMovement(xOffset, yOffset); // Procesa el movimiento del mouse en la c�mara
}
