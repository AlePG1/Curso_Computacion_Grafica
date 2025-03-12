/*
* Previo 7. Texturizado                       Garc�a Gonz�lez Alejandro
* Fecha de entrega: 12 de marzo de 2025       422066992
*/
#include <iostream> // Incluye la librer�a est�ndar para operaciones de entrada/salida en C++
#include <cmath> // Incluye la librer�a de funciones matem�ticas

// GLEW
#include <GL/glew.h> // Librer�a para manejar extensiones de OpenGL

// GLFW
#include <GLFW/glfw3.h> // Librer�a para crear ventanas y manejar eventos del sistema

// Other Libs
#include "stb_image.h" // Librer�a para cargar im�genes en diferentes formatos

// GLM Mathematics
#include <glm/glm.hpp> // Librer�a base de GLM para operaciones matem�ticas con gr�ficos
#include <glm/gtc/matrix_transform.hpp> // Extensiones de GLM para transformaciones de matrices
#include <glm/gtc/type_ptr.hpp> // Extensiones de GLM para convertir matrices a punteros

// Other includes
#include "Shader.h" // Clase para manejar Shaders
#include "Camera.h" // Clase para manejar la C�mara


// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode); // Prototipo de funci�n para manejar eventos de teclado
void MouseCallback(GLFWwindow* window, double xPos, double yPos); // Prototipo de funci�n para manejar eventos de mouse
void DoMovement(); // Prototipo de funci�n para manejar el movimiento de la c�mara

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600; // Dimensiones iniciales de la ventana
int SCREEN_WIDTH, SCREEN_HEIGHT; // Variables para almacenar la resoluci�n real del viewport

// Camera
Camera  camera(glm::vec3(0.0f, 0.0f, 3.0f)); // Objeto de c�mara, posici�n inicial en (0,0,3)
GLfloat lastX = WIDTH / 2.0; // �ltima posici�n X conocida del mouse
GLfloat lastY = HEIGHT / 2.0; // �ltima posici�n Y conocida del mouse
bool keys[1024]; // Arreglo para rastrear el estado de teclas
bool firstMouse = true; // Bandera para indicar si es la primera interacci�n del mouse

// Light attributes
glm::vec3 lightPos(1.2f, 1.0f, 2.0f); // Posici�n de la luz en el mundo

// Deltatime
GLfloat deltaTime = 0.0f;	// Tiempo entre el frame actual y el �ltimo frame
GLfloat lastFrame = 0.0f;  	// Tiempo del �ltimo frame

// The MAIN function, from here we start the application and run the game loop
int main() // Funci�n principal
{
	// Init GLFW
	glfwInit(); // Inicializa la librer�a GLFW
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // Define la versi�n mayor de OpenGL como 3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // Define la versi�n menor de OpenGL como 3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Indica que se usar� el perfil principal de OpenGL
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Habilita compatibilidad hacia adelante (necesario para macOS)
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); // Indica que la ventana no ser� redimensionable

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Previo 7 Garcia Gonzalez Alejandro", nullptr, nullptr); // Crea la ventana principal con GLFW

	if (nullptr == window) // Comprueba si la ventana se cre� con �xito
	{
		std::cout << "Failed to create GLFW window" << std::endl; // Imprime error en caso de fallo
		glfwTerminate(); // Finaliza GLFW

		return EXIT_FAILURE; // Retorna fallo
	}

	glfwMakeContextCurrent(window); // Hace que el contexto de OpenGL actual sea el de esta ventana

	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT); // Obtiene el tama�o real del framebuffer

	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback); // Establece la funci�n de callback para eventos de teclado
	glfwSetCursorPosCallback(window, MouseCallback); // Establece la funci�n de callback para eventos de mouse

	// GLFW Options
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // Desactiva el cursor para capturar el movimiento del mouse

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE; // Habilita caracter�sticas modernas de GLEW
	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit()) // Inicializa GLEW y comprueba si hubo error
	{
		std::cout << "Failed to initialize GLEW" << std::endl; // Imprime error en caso de fallo
		return EXIT_FAILURE; // Retorna fallo
	}

	// Define the viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT); // Define las dimensiones en las que se va a dibujar

	// OpenGL options
	glEnable(GL_DEPTH_TEST); // Habilita la prueba de profundidad (z-buffer)


	// Build and compile our shader program
	Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag"); // Crea un objeto Shader con los archivos de v�rtices y fragmentos

	// Set up vertex data (and buffer(s)) and attribute pointers
	GLfloat vertices[] =
	{
		// Positions            // Colors              // Texture Coords
		-0.5f, -0.5f, 0.0f,    1.0f, 1.0f,1.0f,		0.0f,0.0f,
		0.5f, -0.5f, 0.0f,	   1.0f, 1.0f,1.0f,		1.0f,0.0f,
		0.5f,  0.5f, 0.0f,     1.0f, 1.0f,1.0f,	    1.0f,1.0f,
		-0.5f,  0.5f, 0.0f,    1.0f, 1.0f,1.0f,		0.0f,1.0f,


	}; // Arreglo de v�rtices: posici�n, color y coordenadas de textura

	GLuint indices[] =
	{  // Note that we start from 0!
		0,1,3,
		1,2,3

	}; // �ndices para dibujar los v�rtices en orden (dos tri�ngulos para un cuadrado)

	// First, set the container's VAO (and VBO)
	GLuint VBO, VAO, EBO; // Declaraci�n de identificadores para buffers y array de v�rtices
	glGenVertexArrays(1, &VAO); // Genera un VAO
	glGenBuffers(1, &VBO); // Genera un VBO
	glGenBuffers(1, &EBO); // Genera un EBO

	glBindVertexArray(VAO); // Enlaza el VAO
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // Enlaza el VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // Copia los datos de v�rtices a la memoria del VBO

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // Enlaza el EBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); // Copia los �ndices al EBO

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0); // Configura la forma de leer los datos de posici�n
	glEnableVertexAttribArray(0); // Activa el atributo de posici�n en la ubicaci�n 0
	// Color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat))); // Configura la forma de leer los datos de color
	glEnableVertexAttribArray(1); // Activa el atributo de color en la ubicaci�n 1
	// Texture Coordinate attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat))); // Configura la forma de leer los datos de coordenadas de textura
	glEnableVertexAttribArray(2); // Activa el atributo de coordenadas de textura en la ubicaci�n 2
	glBindVertexArray(0); // Desenlaza el VAO

	// Load textures
	GLuint texture1; // Identificador para la textura
	glGenTextures(1, &texture1); // Genera una textura
	glBindTexture(GL_TEXTURE_2D, texture1); // Enlaza la textura como 2D
	int textureWidth, textureHeight, nrChannels; // Variables para dimensiones y canales de la imagen
	stbi_set_flip_vertically_on_load(true); // Indica a stb_image que voltee verticalmente la imagen al cargar
	unsigned char* image; // Puntero para los datos de la imagen
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Configura el modo de repetici�n en S (X)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Configura el modo de repetici�n en T (Y)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // Filtro de minimizaci�n con mipmap lineal
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST); // Filtro de magnificaci�n con mipmap m�s cercano
	// Diffuse map
	image = stbi_load("images/dado.png", &textureWidth, &textureHeight, &nrChannels, 0); // Carga la imagen desde archivo
	glBindTexture(GL_TEXTURE_2D, texture1); // Reenlaza la textura
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image); // Crea la textura en memoria
	glGenerateMipmap(GL_TEXTURE_2D); // Genera los mipmaps
	if (image) // Verifica si la imagen se carg� correctamente
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image); // Asigna la imagen a la textura
		glGenerateMipmap(GL_TEXTURE_2D); // Genera mipmaps
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl; // Mensaje de error si la carga fall�
	}
	stbi_image_free(image); // Libera la memoria de la imagen



	// Game loop
	while (!glfwWindowShouldClose(window)) // Bucle principal de la aplicaci�n, se ejecuta mientras la ventana no deba cerrarse
	{
		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime(); // Obtiene el tiempo actual
		deltaTime = currentFrame - lastFrame; // Calcula el tiempo transcurrido entre frames
		lastFrame = currentFrame; // Actualiza el �ltimo frame al actual

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents(); // Procesa los eventos de la ventana (teclado, mouse, etc.)
		DoMovement(); // Llama a la funci�n que maneja el movimiento de la c�mara

		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f); // Define el color de borrado del buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Limpia el buffer de color y el de profundidad

		lampShader.Use(); // Usa el shader para renderizar
		//// Create camera transformations
		glm::mat4 view; // Crea una matriz view
		view = camera.GetViewMatrix(); // Obtiene la matriz de vista de la c�mara
		glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f); // Crea la matriz de proyecci�n en perspectiva
		glm::mat4 model(1); // Crea la matriz modelo y la inicializa en identidad
		// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(lampShader.Program, "model"); // Obtiene la ubicaci�n uniform de la matriz modelo en el shader
		GLint viewLoc = glGetUniformLocation(lampShader.Program, "view"); // Obtiene la ubicaci�n uniform de la matriz view en el shader
		GLint projLoc = glGetUniformLocation(lampShader.Program, "projection"); // Obtiene la ubicaci�n uniform de la matriz projection en el shader

		// Bind diffuse map
		glActiveTexture(GL_TEXTURE0); // Activa la textura en la unidad de textura 0
		glBindTexture(GL_TEXTURE_2D, texture1); // Enlaza la textura para su uso

		// Set matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view)); // Env�a la matriz de vista al shader
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection)); // Env�a la matriz de proyecci�n al shader
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); // Env�a la matriz modelo al shader
		// Draw the light object (using light's vertex attributes)
		glBindVertexArray(VAO); // Enlaza el VAO para dibujar
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // Dibuja los elementos (tri�ngulos) usando el EBO
		glBindVertexArray(0); // Desenlaza el VAO

		// Swap the screen buffers
		glfwSwapBuffers(window); // Intercambia los buffers para mostrar en pantalla
	}

	glDeleteVertexArrays(1, &VAO); // Elimina el VAO de la memoria
	glDeleteBuffers(1, &VBO); // Elimina el VBO de la memoria
	glDeleteBuffers(1, &EBO); // Elimina el EBO de la memoria
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate(); // Finaliza GLFW y libera recursos

	return 0; // Retorna 0 para indicar �xito
}

// Moves/alters the camera positions based on user input
void DoMovement() // Funci�n para manejar el movimiento de la c�mara
{
	// Camera controls
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP]) // Si se presiona W o la flecha arriba
	{
		camera.ProcessKeyboard(FORWARD, deltaTime); // Mueve la c�mara hacia adelante
	}

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN]) // Si se presiona S o la flecha abajo
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime); // Mueve la c�mara hacia atr�s
	}

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT]) // Si se presiona A o la flecha izquierda
	{
		camera.ProcessKeyboard(LEFT, deltaTime); // Mueve la c�mara a la izquierda
	}

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT]) // Si se presiona D o la flecha derecha
	{
		camera.ProcessKeyboard(RIGHT, deltaTime); // Mueve la c�mara a la derecha
	}
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) // Funci�n callback para eventos de teclado
{
	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action) // Si se presiona la tecla ESC
	{
		glfwSetWindowShouldClose(window, GL_TRUE); // Indica que la ventana debe cerrarse
	}

	if (key >= 0 && key < 1024) // Validaci�n del rango de la tecla
	{
		if (action == GLFW_PRESS) // Si se presiona la tecla
		{
			keys[key] = true; // Marca la tecla como presionada
		}
		else if (action == GLFW_RELEASE) // Si se suelta la tecla
		{
			keys[key] = false; // Marca la tecla como no presionada
		}
	}
}

void MouseCallback(GLFWwindow* window, double xPos, double yPos) // Funci�n callback para eventos de movimiento del mouse
{
	if (firstMouse) // Si es la primera vez que se detecta el mouse
	{
		lastX = xPos; // Inicializa la �ltima posici�n X
		lastY = yPos; // Inicializa la �ltima posici�n Y
		firstMouse = false; // Cambia la bandera
	}

	GLfloat xOffset = xPos - lastX; // Diferencia horizontal del mouse
	GLfloat yOffset = lastY - yPos;  // Diferencia vertical del mouse (invertida para corresponder al movimiento real)

	lastX = xPos; // Actualiza la �ltima posici�n X
	lastY = yPos; // Actualiza la �ltima posici�n Y

	camera.ProcessMouseMovement(xOffset, yOffset); // Ajusta la orientaci�n de la c�mara seg�n el movimiento del mouse
}
