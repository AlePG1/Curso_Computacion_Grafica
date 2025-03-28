/*Práctica 8
* García González Alejandro    422066992
* 27 de Marzo 2024
*/

// Inclusiones estándar
#include <string>  // Librería estándar para el manejo de cadenas

// GLEW
#include <GL/glew.h>  // Librería para manejar las extensiones de OpenGL

// GLFW
#include <GLFW/glfw3.h>  // Librería para la creación de ventanas y manejo de eventos de OpenGL

// Librerías para OpenGL
#include "Shader.h"  // Header para la clase Shader
#include "Camera.h"  // Header para la clase Camera
#include "Model.h"   // Header para la clase Model

// Matematáticas GLM
#include <glm/glm.hpp>  // Librería principal de GLM para matemáticas
#include <glm/gtc/matrix_transform.hpp>  // Transformaciones de matrices (traslación, rotación, escalado)
#include <glm/gtc/type_ptr.hpp>  // Convierte matrices a punteros

// Otras librerías
#include "SOIL2/SOIL2.h"  // Librería para cargar imágenes (SOIL2)
#include "stb_image.h"  // Librería para cargar imágenes (STB)

// Propiedades de la ventana
const GLuint WIDTH = 1280, HEIGHT = 800;  // Ancho y alto de la ventana
int SCREEN_WIDTH, SCREEN_HEIGHT;  // Variables para el tamaño actual de la ventana

// Prototipos de funciones
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);  // Callback para el teclado
void MouseCallback(GLFWwindow* window, double xPos, double yPos);  // Callback para el mouse
void DoMovement();  // Función para manejar los movimientos de la cámara

// Cámara
Camera camera(glm::vec3(0.0f, 30.0f, 5.0f));  // Crea la cámara en la posición especificada
bool keys[1024];  // Array para manejar las teclas presionadas
GLfloat lastX = 400, lastY = 300;  // Última posición del mouse
bool firstMouse = true;  // Indica si es el primer movimiento del mouse

// Atributos de la luz
glm::vec3 lightPos(100.5f, 0.5f, 0.0f);  // Posición de la luz principal (Sol)
glm::vec3 secondLightPos(100.8f, 0.8f, 0.0f);  // Posición de la segunda luz (Luna)
float movelightPos = 0.0f;  // Movimiento de la luz principal
float movelightPos2 = 0.0f;  // Movimiento de la segunda luz
float reloj = 0.0f;  // Tiempo del ciclo día/noche
float activador = 0.0f;  // Activador de la fase de día/noche
GLfloat deltaTime = 0.0f;  // Tiempo entre fotogramas
GLfloat lastFrame = 0.0f;  // Último fotograma para calcular deltaTime
float rot = 0.0f;  // Ángulo de rotación de la cámara
bool activanim = false;  // Activación de la animación

int main()
{
    // Inicializa GLFW
    glfwInit();

    // Configura las opciones requeridas para GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);  // Versión principal de OpenGL (3.x)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);  // Versión menor de OpenGL (3.x)
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // Perfil de OpenGL: core
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);  // Compatibilidad hacia adelante
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);  // La ventana no será redimensionable

    // Crea una ventana con las características especificadas
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Practica 8 Garcia Gonzalez Alejandro", nullptr, nullptr);

    // Verifica si la ventana fue creada correctamente
    if (nullptr == window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;  // Mensaje de error
        glfwTerminate();  // Termina GLFW
        return EXIT_FAILURE;  // Termina el programa con error
    }

    glfwMakeContextCurrent(window);  // Hace que el contexto de OpenGL sea el actual

    glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);  // Obtiene el tamaño de la ventana

    // Configura las funciones de callback para teclado y mouse
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, MouseCallback);

    // Inicializa GLEW para cargar las extensiones de OpenGL
    glewExperimental = GL_TRUE;
    if (GLEW_OK != glewInit())
    {
        std::cout << "Failed to initialize GLEW" << std::endl;  // Error al inicializar GLEW
        return EXIT_FAILURE;  // Termina el programa con error
    }

    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);  // Define las dimensiones del viewport

    // Habilita el test de profundidad para el renderizado 3D
    glEnable(GL_DEPTH_TEST);

    // Compila los shaders
    Shader shader("Shader/modelLoading.vs", "Shader/modelLoading.frag");  // Shader para cargar modelos
    Shader lampshader("Shader/lamp.vs", "Shader/lamp.frag");  // Shader para la luz
    Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");  // Shader para la iluminación

    // Carga los modelos
    Model desierto((char*)"Models/en_el_desierto_de_yemen.obj");  // Carga el modelo del desierto
    Model dog((char*)"Models/RedDog.obj");  // Carga el modelo del perro
    Model car((char*)"Models/mercedes-benz_sl_65_amg_black_series_r230.obj");  // Carga el modelo del coche
    Model estatua((char*)"Models/Estatua/estatua__statue.obj");  // Carga el modelo de la estatua
    Model cactus((char*)"Models/Cactus/flowering_cactus.obj");  // Carga el modelo del cactus
    Model rango((char*)"Models/Rango/modern_luxury_wedding_arch_house_building_design.obj");  // Carga el modelo de la casa
    Model fig1((char*)"Models/Fig1/light_tactical_vehicle_aquus_areg.obj");  // Carga el modelo del vehículo
    //Model luna((char*)"Models/Luna/la_luna__viaje_virtual_guiado__3d___vr.obj");  // Carga el modelo de la luna
    Model luna((char*)"Models/Lunaa/tripo_convert_36d7d968-399d-46df-97a0-976f8c3bb536.obj");  // Carga el modelo de la luna
    Model sol((char*)"Models/Sol/sol.obj");  // Carga el modelo del sol
    glm::mat4 projection = glm::perspective(camera.GetZoom(), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);  // Proyección perspectiva

    // Definición de los vértices de un cubo
    float vertices[] = {
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

    // Configuración del VAO y VBO para el cubo
    GLuint VBO, VAO;  // Variables para el VAO y VBO
    glGenVertexArrays(1, &VAO);  // Genera el VAO
    glGenBuffers(1, &VBO);  // Genera el VBO
    glBindVertexArray(VAO);  // Activa el VAO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);  // Activa el VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);  // Carga los vértices en el VBO
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);  // Atributo de posición
    glEnableVertexAttribArray(0);  // Habilita el atributo de posición
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));  // Atributo de normales
    glEnableVertexAttribArray(1);  // Habilita el atributo de normales

    // Carga las texturas
    GLuint texture;  // Variable para la textura
    glGenTextures(1, &texture);  // Genera la textura
    glBindTexture(GL_TEXTURE_2D, texture);  // Activa la textura
    int textureWidth, textureHeight, nrChannels;  // Variables para el tamaño y los canales de la textura
    stbi_set_flip_vertically_on_load(true);  // Configura para voltear la imagen verticalmente
    unsigned char* image;  // Puntero a los datos de la imagen

    // Configura las propiedades de la textura
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);  // Ajuste para el eje X
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);  // Ajuste para el eje Y
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);  // Filtro para la textura cuando se ve a distancia
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);  // Filtro para la textura cuando se ve de cerca

    // Carga la imagen
    image = stbi_load("Models/Texture_albedo.jpg", &textureWidth, &textureHeight, &nrChannels, 0);  // Carga la imagen
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);  // Crea la textura
    glGenerateMipmap(GL_TEXTURE_2D);  // Genera los mipmaps de la textura
    if (image)  // Si la imagen se carga correctamente
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);  // Crea la textura
        glGenerateMipmap(GL_TEXTURE_2D);  // Genera mipmaps
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;  // Error al cargar la textura
    }
    stbi_image_free(image);  // Libera la memoria de la imagen

    // Bucle principal del juego
    while (!glfwWindowShouldClose(window))
    {
        // Calcula el tiempo entre fotogramas
        GLfloat currentFrame = glfwGetTime();  // Obtiene el tiempo actual
        deltaTime = currentFrame - lastFrame;  // Calcula el tiempo transcurrido
        lastFrame = currentFrame;  // Actualiza el último fotograma

        // Procesa los eventos de la ventana
        glfwPollEvents();
        DoMovement();  // Llama a la función para mover la cámara

        // Limpia el buffer de color y profundidad
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);  // Establece el color de fondo
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // Limpia los buffers de color y profundidad

        lightingShader.Use();  // Usa el shader de iluminación

        // Movimiento del sol y la luna
        lightPos.x = cos(reloj) * 50.0f;  // Movimiento del sol
        lightPos.y = sin(reloj) * 50.0f;  // Movimiento del sol

        secondLightPos.x = -cos(reloj) * 50.0f;  // Movimiento de la luna
        secondLightPos.y = -sin(reloj) * 50.0f;  // Movimiento de la luna

        // Envia la posición de la luz y la cámara al shader
        GLint lightPosLoc = glGetUniformLocation(lightingShader.Program, "light.position");
        GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
        glUniform3f(lightPosLoc, lightPos.x + movelightPos, lightPos.y + movelightPos, lightPos.z + movelightPos);
        glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

        // Envia la posición de la segunda luz (luna) al shader
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.position"), secondLightPos.x, secondLightPos.y, secondLightPos.z);

        // Configura las propiedades de las luces (día/noche)
        if (reloj < 3.1 && reloj > 0 && activador == 1) {
            // Fase del día (luz del sol)
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light.ambient"), 10.0f, 5.0f, 2.0f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light.diffuse"), 0.2f, 0.7f, 0.4f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light.specular"), 0.3f, 0.6f, 0.4f);

            // Configuración de la luz secundaria (luna) al mínimo
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.ambient"), 0.1f, 0.1f, 0.1f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.diffuse"), 0.1f, 0.1f, 0.1f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.specular"), 0.1f, 0.1f, 0.1f);
        }
        else {
            // Fase de la noche (luz de la luna)
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light.ambient"), 0.1f, 0.1f, 0.1f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light.diffuse"), 0.1f, 0.1f, 0.1f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light.specular"), 0.1f, 0.1f, 0.1f);

            // Mejora de la luz secundaria (lunar)
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.ambient"), 1.5f, 1.5f, 2.0f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.diffuse"), 0.1f, 1.05f, 1.1f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.specular"), 0.4f, 0.9f, 1.0f);
        }

        // Envia las matrices de proyección y vista
        glm::mat4 view = camera.GetViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

        // Configura las propiedades materiales
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.ambient"), 0.7f, 0.7f, 0.7f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0.7f, 0.7f, 0.7f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.6f, 0.6f, 0.6f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 0.6f);

        // Draw the loaded model
        glm::mat4 model(1); // Crea la matriz de modelo como identidad
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        // Envía la matriz de modelo (identidad) al shader

        // Matriz para el desierto: lo colocamos en el suelo (Y = 0)
        glm::mat4 modelDesierto = glm::mat4(1.0f);                                                   // Inicia matriz en identidad
        modelDesierto = glm::translate(modelDesierto, glm::vec3(0.5f, 0.0f, 0.5f));                   // Traslada el desierto
        modelDesierto = glm::scale(modelDesierto, glm::vec3(0.7f, 0.7f, 0.7f));                       // Escala el desierto
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelDesierto));
        // Envía la matriz de modelo al shader
        desierto.Draw(lightingShader); // Dibuja el modelo del desierto

        // Matriz para el perro: lo trasladamos para que quede arriba del desierto (Y > 0)
        glm::mat4 modelPerro = glm::mat4(1.0f);                                                      // Inicia matriz en identidad
        modelPerro = glm::translate(modelPerro, glm::vec3(-3.0f, 7.5f, 0.0f));                        // Traslada el perro
        modelPerro = glm::scale(modelPerro, glm::vec3(2.0f, 3.0f, 2.0f));                             // Escala el perro
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelPerro));
        // Envía la matriz al shader
        dog.Draw(lightingShader); // Dibuja el modelo del perro

        glm::mat4 modelCar = glm::mat4(1.0f);                                                         // Inicia matriz en identidad
        modelCar = glm::translate(modelCar, glm::vec3(-3.0f, 6.5f, 0.0f));                            // Traslada el coche
        modelCar = glm::scale(modelCar, glm::vec3(0.3f, 0.3f, 0.3f));                                 // Escala el coche
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelCar));
        // Envía la matriz al shader
        car.Draw(lightingShader); // Dibuja el modelo del coche

        glm::mat4 modelEstatua = glm::mat4(1.0f);                                                     // Inicia matriz en identidad
        modelEstatua = glm::translate(modelEstatua, glm::vec3(-10.0f, 6.5f, 0.0f));                   // Traslada la estatua
        modelEstatua = glm::scale(modelEstatua, glm::vec3(0.9f, 0.9f, 0.9f));                         // Escala la estatua
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelEstatua));
        // Envía la matriz al shader
        estatua.Draw(lightingShader); // Dibuja el modelo de la estatua

        glm::mat4 modelCactus = glm::mat4(1.0f);                                                      // Inicia matriz en identidad
        modelCactus = glm::translate(modelCactus, glm::vec3(-5.0f, 6.5f, 0.0f));                      // Traslada el cactus
        modelCactus = glm::scale(modelCactus, glm::vec3(2.9f, 2.9f, 2.9f));                           // Escala el cactus
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelCactus));
        // Envía la matriz al shader
        cactus.Draw(lightingShader); // Dibuja el modelo del cactus

        glm::mat4 modelRango = glm::mat4(1.0f);                                                       // Inicia matriz en identidad
        modelRango = glm::translate(modelRango, glm::vec3(0.0f, 8.0f, -20.0f));                      // Traslada la construcción
        modelRango = glm::scale(modelRango, glm::vec3(2.9f, 2.9f, 2.9f));                             // Escala la construcción
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelRango));
        // Envía la matriz al shader
        rango.Draw(lightingShader); // Dibuja el modelo de la construcción

        glm::mat4 modelFig1 = glm::mat4(1.0f);                                                        // Inicia matriz en identidad
        modelFig1 = glm::translate(modelFig1, glm::vec3(-15.0f, 7.5f, 0.0f));                         // Traslada el vehículo
        modelFig1 = glm::scale(modelFig1, glm::vec3(1.0f, 1.0f, 1.0f));                               // Escala el vehículo
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelFig1));
        // Envía la matriz al shader
        fig1.Draw(lightingShader); // Dibuja el modelo del vehículo

        //glDrawArrays(GL_TRIANGLES, 0, 36);


        glBindVertexArray(0);


        lampshader.Use();
        glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

        if (activador == 1) {
            // Dibujar el sol (Lampara 1)
            model = glm::mat4(1.0f);
            model = glm::translate(model, lightPos + movelightPos);
            model = glm::scale(model, glm::vec3(1.1f));
            glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
            sol.Draw(lampshader);
        }
        else {
            // Dibujar el luna (Lampara 1)
            model = glm::mat4(1.0f);
            model = glm::translate(model, lightPos + movelightPos);
            model = glm::scale(model, glm::vec3(3.1f));
            glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
            luna.Draw(lampshader);
        }
        glBindVertexArray(0);

        // Swap the buffers
        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);  // Elimina el VAO
    glDeleteBuffers(1, &VBO);  // Elimina el VBO

    glfwTerminate();  // Termina GLFW
    return 0;  // Retorna 0 para indicar éxito
}

// Función para mover la cámara según las teclas presionadas
void DoMovement()
{
    // Control de la cámara por teclado
    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
    {
        camera.ProcessKeyboard(FORWARD, deltaTime);  // Movimiento hacia adelante
    }

    if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
    {
        camera.ProcessKeyboard(BACKWARD, deltaTime);  // Movimiento hacia atrás
    }

    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
    {
        camera.ProcessKeyboard(LEFT, deltaTime);  // Movimiento hacia la izquierda
    }

    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
    {
        camera.ProcessKeyboard(RIGHT, deltaTime);  // Movimiento hacia la derecha
    }

    if (activanim)  // Si la animación está activada
    {
        if (rot > -90.0f)  // Rota hasta -90 grados
            rot -= 0.1f;
    }
}

// Función de callback para teclas presionadas o soltadas
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);  // Cierra la ventana si se presiona ESC
    }

    if (key >= 0 && key < 1024)  // Si la tecla está dentro del rango válido
    {
        if (action == GLFW_PRESS)
        {
            keys[key] = true;  // Marca la tecla como presionada
        }
        else if (action == GLFW_RELEASE)
        {
            keys[key] = false;  // Marca la tecla como soltada
        }
    }

    if (reloj < 3 && (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS))  // Si se presiona 'O'
    {
        activador = 1.0f;  // Activa la fase de día
        reloj += 0.1f;  // Incrementa el reloj
    }

    if (reloj > 0 && (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS))  // Si se presiona 'L'
    {
        activador = 0.0f;  // Activa la fase de noche
        reloj -= 0.1f;  // Decrementa el reloj
    }
}

// Función de callback para el movimiento del mouse
void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
    if (firstMouse)  // Si es el primer movimiento del mouse
    {
        lastX = xPos;  // Guarda la posición inicial del mouse
        lastY = yPos;  // Guarda la posición inicial del mouse
        firstMouse = false;  // Marca que no es el primer movimiento
    }

    GLfloat xOffset = xPos - lastX;  // Calcula el desplazamiento en el eje X
    GLfloat yOffset = lastY - yPos;  // Calcula el desplazamiento en el eje Y (invertido)

    lastX = xPos;  // Actualiza la posición del mouse en X
    lastY = yPos;  // Actualiza la posición del mouse en Y

    camera.ProcessMouseMovement(xOffset, yOffset);  // Procesa el movimiento del mouse
}
