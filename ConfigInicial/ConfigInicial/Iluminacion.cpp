/*
* García González Alejandro   422066992
* Previo 8 Iluminación        Fecha de entrega: 23/marzo/2024
*/

// Std. Includes
#include <string> // Librería para manipulación de cadenas de texto

// GLEW
#include <GL/glew.h> // Librería para la gestión de extensiones y funciones de OpenGL

// GLFW
#include <GLFW/glfw3.h> // Librería para crear ventanas y gestionar eventos

// GL includes
#include "Shader.h" // Librería para manejar shaders
#include "Camera.h" // Librería para manejar la cámara
#include "Model.h"  // Librería para cargar y renderizar modelos 3D

// GLM Mathemtics
#include <glm/glm.hpp> // Librería para operaciones matemáticas (vectores, matrices)
#include <glm/gtc/matrix_transform.hpp> // Funciones para transformaciones de matrices (traslación, rotación, etc.)
#include <glm/gtc/type_ptr.hpp> // Funciones para convertir matrices a punteros

// Other Libs
#include "SOIL2/SOIL2.h" // Librería para carga de texturas
#include "stb_image.h" // Librería para carga de imágenes (dependencia para SOIL2)

// Properties
const GLuint WIDTH = 800, HEIGHT = 600; // Definir las dimensiones de la ventana
int SCREEN_WIDTH, SCREEN_HEIGHT; // Dimensiones de la ventana

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode); // Prototipo para la función de control de teclas
void MouseCallback(GLFWwindow* window, double xPos, double yPos); // Prototipo para la función de control del ratón
void DoMovement(); // Prototipo para la función que maneja el movimiento de la cámara

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 0.0f)); // Instancia de la cámara en la posición (0, 0, 0)
bool keys[1024]; // Arreglo para registrar las teclas presionadas
GLfloat lastX = 400, lastY = 300; // Posición inicial del cursor en la ventana
bool firstMouse = true; // Variable para detectar el primer movimiento del ratón

// Light attributes
glm::vec3 lightPos(0.5f, 0.5f, 2.5f); // Posición de la primera fuente de luz
glm::vec3 lightPos2(-0.5f, -0.5f, -2.5f); // Posición de la segunda fuente de luz
//glm::vec3 lightPos3(0.0f, 0.0f, 0.0f); // Posición de la tercera fuente de luz (comentada)

float movelightPos = 0.0f;      // Variable para mover la luz
GLfloat deltaTime = 0.0f;       // Tiempo entre fotogramas
GLfloat lastFrame = 0.0f;       // Último fotograma para el cálculo del tiempo
float rot = 0.0f;               // Ángulo de rotación
bool activanim = false;         // Flag para activar la animación

int main()
{
    // Init GLFW
    glfwInit(); // Inicializa la librería GLFW
    // Establece opciones necesarias para GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // Define la versión principal de OpenGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // Define la versión menor de OpenGL
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Define el perfil de OpenGL
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Habilita compatibilidad hacia adelante
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); // Desactiva la redimensión de la ventana

    // Crear la ventana GLFW
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Previo 8. Garcia Gonzalez Alejandro", nullptr, nullptr);

    // Verifica si la ventana fue creada correctamente
    if (nullptr == window)
    {
        std::cout << "Failed to create GLFW window" << std::endl; // Muestra un error si falla
        glfwTerminate(); // Termina GLFW
        return EXIT_FAILURE; // Termina el programa
    }

    glfwMakeContextCurrent(window); // Establece el contexto de la ventana

    glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT); // Obtiene el tamaño del framebuffer

    // Establece las funciones de callback para teclas y ratón
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, MouseCallback);

    // GLFW Options
    //glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_DISABLED ); // Desactiva el cursor (comentado)

    // Inicia GLEW para cargar funciones de OpenGL
    glewExperimental = GL_TRUE; // Habilita el uso de un enfoque moderno para obtener las funciones de OpenGL
    if (GLEW_OK != glewInit()) // Verifica que GLEW se haya inicializado correctamente
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE; // Termina el programa si GLEW falla
    }

    // Define las dimensiones del viewport
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    // Activa la prueba de profundidad (para renderizar correctamente objetos 3D)
    glEnable(GL_DEPTH_TEST);

    // Carga y compila los shaders
    Shader shader("Shader/modelLoading.vs", "Shader/modelLoading.frag");
    Shader lampshader("Shader/lamp.vs", "Shader/lamp.frag");
    Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");

    // Carga los modelos 3D
    Model red_dog((char*)"Models/RedDog.obj");
    Model sol((char*)"Models/Estatua/estatua__statue.obj");
    glm::mat4 projection = glm::perspective(camera.GetZoom(), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

    // Define los vértices de un cubo
    float vertices[] = {
        // x     y      z      Vector Normal 
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

    // First, set the container's VAO (and VBO)
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO); // Genera un Vertex Array Object
    glGenBuffers(1, &VBO); // Genera un Vertex Buffer Object
    glBindVertexArray(VAO); // Víncula el VAO actual
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // Víncula el VBO actual
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // Copia los vértices en el buffer
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0); // Define el atributo de posición
    glEnableVertexAttribArray(0); // Habilita el atributo de posición
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))); // Define el atributo de normales
    glEnableVertexAttribArray(1); // Habilita el atributo de normales

    // Load textures
    GLuint texture;
    glGenTextures(1, &texture); // Genera una textura
    glBindTexture(GL_TEXTURE_2D, texture); // Víncula la textura a GL_TEXTURE_2D
    int textureWidth, textureHeight, nrChannels;
    stbi_set_flip_vertically_on_load(true); // Establece que la textura debe ser cargada con el eje Y invertido
    unsigned char* image;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Establece la repetición de la textura en el eje X
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Establece la repetición de la textura en el eje Y
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // Establece el filtro de minimización
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST); // Establece el filtro de maximización

    image = stbi_load("Models/Texture_albedo.jpg", &textureWidth, &textureHeight, &nrChannels, 0); // Carga la textura
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image); // Aplica la textura
    glGenerateMipmap(GL_TEXTURE_2D); // Genera los mipmaps de la textura
    if (image) // Si la imagen se carga correctamente
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D); // Genera los mipmaps de la textura
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl; // Si falla, muestra un mensaje de error
    }
    stbi_image_free(image); // Libera la memoria de la imagen cargada

    // Game loop
    while (!glfwWindowShouldClose(window)) // Mientras la ventana no se cierre
    {
        // Set frame time
        GLfloat currentFrame = glfwGetTime(); // Obtiene el tiempo actual
        deltaTime = currentFrame - lastFrame; // Calcula el tiempo entre fotogramas
        lastFrame = currentFrame; // Actualiza el tiempo del último fotograma

        // Check and call events
        glfwPollEvents(); // Verifica eventos de la ventana (teclado, ratón, etc.)
        DoMovement(); // Realiza el movimiento de la cámara

        // Clear the colorbuffer
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Establece el color de limpieza (fondo)
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Limpia el color y el buffer de profundidad

        float movelightPos1 = 0.0f; // Movimiento de la primera luz
        float movelightPos2 = 0.0f; // Movimiento de la segunda luz

        //------------------------------ Luz 1 
        lightingShader.Use(); // Usa el shader de iluminación
        GLint lightPosLoc = glGetUniformLocation(lightingShader.Program, "light.position"); // Obtiene la ubicación de la posición de la luz
        GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");           // Obtiene la ubicación de la posición de la cámara
        glUniform3f(lightPosLoc, lightPos.x + movelightPos, lightPos.y + movelightPos, lightPos.z + movelightPos); // Define la posición de la luz
        glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z); // Define la posición de la cámara

        // Set lights properties
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.ambient"), 0.3f, 0.3f, 0.3f); // Define la propiedad de luz ambiental
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.diffuse"), 0.8f, 0.7f, 0.8f); // Define la propiedad de luz difusa
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.specular"), 0.0f, 0.0f, 0.0f); // Define la propiedad de luz especular


        //------------------------------------ Luz 2 
        GLint lightPosLoc2 = glGetUniformLocation(lightingShader.Program, "light2.position"); // Obtiene la ubicación de la segunda luz
        GLint viewPosLoc2 = glGetUniformLocation(lightingShader.Program, "viewPos");           // Obtiene la ubicación de la cámara
        glUniform3f(lightPosLoc2, lightPos2.x + movelightPos2, lightPos2.y + movelightPos2, lightPos2.z + movelightPos2); // Define la posición de la luz 2
        glUniform3f(viewPosLoc2, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z); // Define la posición de la cámara

        // Set lights properties
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.ambient"), 0.3f, 0.3f, 0.3f); // Define la luz ambiental para la luz 2
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.diffuse"), 9.8f, 9.7f, 9.8f); // Define la luz difusa para la luz 2
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.specular"), 0.0f, 0.0f, 0.0f); // Define la luz especular para la luz 2


        glm::mat4 view = camera.GetViewMatrix(); // Obtiene la matriz de vista de la cámara
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection)); // Define la matriz de proyección
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view)); // Define la matriz de vista


        // ------------------------ Configuración de materiales se queda general 
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.ambient"), 0.5f, 0.5f, 0.5f); // Establece las propiedades de material
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0.8f, 0.2f, 0.1f); // Establece las propiedades de material
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 1.0f, 0.0f, 0.0f); // Establece las propiedades de material
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 0.8f); // Establece el brillo del material



        // Draw the loaded model
        glm::mat4 model(1); // Matriz de modelo
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f)); // Escala del modelo
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model)); // Aplica la matriz de modelo
        glBindVertexArray(VAO); // Víncula el VAO
        red_dog.Draw(lightingShader); // Dibuja el modelo (perro rojo)
        //mercedez.Draw(lightingShader); // Dibuja otro modelo (comentado)
        sol.Draw(lightingShader); // Dibuja el modelo de la estatua
        //luna.Draw(lightingShader); // Dibuja otro modelo (comentado)


        glDrawArrays(GL_TRIANGLES, 0, 36); // Dibuja el cubo
        glBindVertexArray(0); // Desvincula el VAO



        // ------------------ Representación de la primera fuente de luz (Luz 1) -------------------
        lampshader.Use(); // Usa el shader para la luz
        glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection)); // Aplica la matriz de proyección
        glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view)); // Aplica la matriz de vista

        // Primer modelo para la primera fuente de luz
        model = glm::mat4(1.0f); // Inicializa la matriz de modelo
        model = glm::translate(model, lightPos + movelightPos); // Traslada el modelo a la posición de la luz
        model = glm::scale(model, glm::vec3(0.3f));  // Tamaño de la luz (ajusta según sea necesario)
        glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model)); // Aplica la matriz de modelo
        glBindVertexArray(VAO); // Víncula el VAO
        glDrawArrays(GL_TRIANGLES, 0, 36);  // Dibuja la luz
        glBindVertexArray(0); // Desvincula el VAO

        // ------------------ Representación de la segunda fuente de luz (Luz 2) -------------------
        lampshader.Use(); // Usa el shader para la luz
        glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection)); // Aplica la matriz de proyección
        glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view)); // Aplica la matriz de vista

        // Segundo modelo para la segunda fuente de luz
        model = glm::mat4(1.0f); // Inicializa la matriz de modelo
        model = glm::translate(model, lightPos2 + movelightPos2);  // Traslada el modelo a la posición de la luz 2
        model = glm::scale(model, glm::vec3(0.3f));  // Tamaño de la luz (ajusta según sea necesario)
        glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model)); // Aplica la matriz de modelo
        glBindVertexArray(VAO); // Víncula el VAO
        glDrawArrays(GL_TRIANGLES, 0, 36);  // Dibuja la segunda luz
        glBindVertexArray(0); // Desvincula el VAO


        // Swap the buffers
        glfwSwapBuffers(window); // Intercambia los buffers (para renderizado en la ventana)
    }

    glDeleteVertexArrays(1, &VAO); // Elimina el VAO
    glDeleteBuffers(1, &VBO); // Elimina el VBO

    glfwTerminate(); // Finaliza GLFW
    return 0; // Termina el programa
}

// Función que maneja el movimiento de la cámara según la entrada del usuario
void DoMovement()
{
    // Controles de la cámara
    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP]) // Movimiento hacia adelante
    {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }

    if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN]) // Movimiento hacia atrás
    {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }

    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT]) // Movimiento hacia la izquierda
    {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }

    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT]) // Movimiento hacia la derecha
    {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }

    if (activanim) // Si la animación está activa
    {
        if (rot > -90.0f) // Si la rotación no ha alcanzado el límite
            rot -= 0.1f; // Resta la rotación
    }

}

// Función de callback para las teclas
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action) // Si se presiona la tecla ESC
    {
        glfwSetWindowShouldClose(window, GL_TRUE); // Cierra la ventana
    }

    if (key >= 0 && key < 1024) // Si la tecla está en el rango válido
    {
        if (action == GLFW_PRESS) // Si se presiona la tecla
        {
            keys[key] = true; // Marca la tecla como presionada
        }
        else if (action == GLFW_RELEASE) // Si se suelta la tecla
        {
            keys[key] = false; // Marca la tecla como liberada
        }
    }

    // Mover la primera fuente de luz (luz 1)
    if (keys[GLFW_KEY_O]) // Si se presiona la tecla O
    {
        movelightPos += 0.1f; // Mueve la luz en la dirección positiva
    }

    if (keys[GLFW_KEY_L]) // Si se presiona la tecla L
    {
        movelightPos -= 0.1f; // Mueve la luz en la dirección negativa
    }

    // Mover la segunda fuente de luz (luz 2) con las teclas I y K
    if (keys[GLFW_KEY_I])  // Mover la luz 2 hacia arriba
    {
        lightPos2.y += 0.1f;
    }

    if (keys[GLFW_KEY_K])  // Mover la luz 2 hacia abajo
    {
        lightPos2.y -= 0.1f;
    }
}

// Función de callback para el movimiento del ratón
void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
    if (firstMouse) // Si es el primer movimiento del ratón
    {
        lastX = xPos; // Establece la posición X inicial
        lastY = yPos; // Establece la posición Y inicial
        firstMouse = false; // Marca que el ratón ha sido movido por primera vez
    }

    GLfloat xOffset = xPos - lastX; // Calcula el desplazamiento en X
    GLfloat yOffset = lastY - yPos;  // Calcula el desplazamiento en Y (invertido)

    lastX = xPos; // Actualiza la posición X
    lastY = yPos; // Actualiza la posición Y

    camera.ProcessMouseMovement(xOffset, yOffset); // Procesa el movimiento del ratón para cambiar la orientación de la cámara
}
