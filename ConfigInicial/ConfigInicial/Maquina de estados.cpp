/*
*   Fecha de entrega: 25 de abril del 2025                422066992
*	Práctica 11: Animación por máquina de estados         García González Alejandro
*/                                                  
#include <iostream>                                 // Biblioteca estándar de entrada/salida
#include <cmath>                                    // Biblioteca de funciones matemáticas
#include <fstream>                                  // Biblioteca de manejo de archivos
#include <vector>                                   // Biblioteca de contenedores dinámicos
#include <string>                                   // Biblioteca de cadenas de texto

// GLEW
#include <GL/glew.h>                                // Encabezados de GLEW para extensiones OpenGL

// GLFW
#include <GLFW/glfw3.h>                             // Encabezados de GLFW para creación de ventanas y contexto

// Other Libs
#include "stb_image.h"                              // Biblioteca para carga de imágenes (texturas)

// GLM Mathematics
#include <glm/glm.hpp>                              // Biblioteca GLM para vectores y matrices
#include <glm/gtc/matrix_transform.hpp>             // Funciones de transformación de GLM
#include <glm/gtc/type_ptr.hpp>                     // Permite obtener punteros a datos GLM

//Load Models
#include "SOIL2/SOIL2.h"                            // Biblioteca SOIL2 para texturas

// Other includes
#include "Shader.h"                                 // Clase Shader para compilar y usar shaders
#include "Camera.h"                                 // Clase Camera para manejar la cámara
#include "Model.h"                                  // Clase Model para cargar y dibujar modelos 3D

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);   // Prot. función manejo de teclado
void MouseCallback(GLFWwindow* window, double xPos, double yPos);                    // Prot. función manejo de ratón
void DoMovement();                                  // Prot. función para mover la cámara
void Animation();                                   // Prot. función para animar la escena

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;             // Tamaño fijo de la ventana
int SCREEN_WIDTH, SCREEN_HEIGHT;                    // Variables para tamaño real de framebuffer

// Camera
Camera  camera(glm::vec3(0.0f, 0.0f, 3.0f));         // Cámara inicial posicionada en (0,0,3)
GLfloat lastX = WIDTH / 2.0;                        // Última posición X del ratón (inicial en centro)
GLfloat lastY = HEIGHT / 2.0;                       // Última posición Y del ratón (inicial en centro)
bool keys[1024];                                     // Array para estado de teclas (1024 posibles)
bool firstMouse = true;                              // Flag para captura inicial del ratón

// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);                // Posición de la luz direccional
bool active;                                        // Flag para activar/desactivar efecto de luz

// Positions of the point lights
glm::vec3 pointLightPositions[] = {                  // Posiciones de luces de tipo punto
	glm::vec3(0.0f,2.0f, 0.0f),
	glm::vec3(0.0f,0.0f, 0.0f),
	glm::vec3(0.0f,0.0f,  0.0f),
	glm::vec3(0.0f,0.0f, 0.0f)
};

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



// Anim
glm::vec3 Light1 = glm::vec3(0);                     // Vector de parámetros de animación de luz
float rotBall = 0.0f;                                // Ángulo de rotación de la pelota
bool AnimBall = false;                               // Flag para animar la pelota
bool AnimDog = false;                                // Flag para animar el perro
float rotDog = 0.0f;                                 // Ángulo de rotación del perro
int dogAnim = 0;                                     // Estado de la máquina del perro (0 = parado, 1 = camina)
float FLegs = 0.0f;                                  // Ángulo de las patas delanteras
float RLegs = 0.0f;                                  // Ángulo de las patas traseras
float head = 0.0f;                                   // Ángulo de la cabeza
float tail = 0.0f;                                   // Ángulo de la cola
glm::vec3 dogPos(0.0f, 0.0f, 0.0f);                  // Posición del perro
float dogRot = 0.0f;                                 // Rotación (yaw) del perro
bool step = false;                                   // Flag para controlar el paso (adelante/atrás)
bool dirz = false;                                   // Flag de dirección Z alcanzada
bool dirx = false;                                   // Flag de dirección X alcanzada
bool diag = false;                                   // Flag de dirección diagonal alcanzada



// Deltatime
GLfloat deltaTime = 0.0f;	                          // Tiempo entre frame actual y anterior
GLfloat lastFrame = 0.0f;  	                      // Tiempo del frame anterior

int main()
{
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);*/

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(          // Crea la ventana OpenGL de tamaño WIDTH×HEIGHT
		WIDTH, HEIGHT,
		"Animacion maquina de estados - Garcia Gonzalez Alejandro",
		nullptr, nullptr);

	if (nullptr == window)                         // Comprueba si la ventana se creó correctamente
	{
		std::cout << "Failed to create GLFW window" << std::endl;  // Imprime error
		glfwTerminate();                           // Finaliza GLFW
		return EXIT_FAILURE;                       // Salida con error
	}

	glfwMakeContextCurrent(window);                 // Hace actual el contexto de la ventana

	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT); // Obtiene tamaño real del framebuffer

	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);        // Registra función de teclado
	glfwSetCursorPosCallback(window, MouseCallback);// Registra función de ratón

	// Initialize GLEW to setup the OpenGL Function pointers
	glewExperimental = GL_TRUE;                     // Permite usar punteros modernos
	if (GLEW_OK != glewInit())                     // Inicializa GLEW y comprueba errores
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Define the viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT); // Ajusta el viewport al tamaño de la ventana

	// Compila y configura shaders
	Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag"); // Shader para iluminación de materiales
	Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");            // Shader para dibujar lámparas

	// Carga modelos 3D
	Model DogBody((char*)"Models/DogBody.obj");       // Cuerpo del perro
	Model HeadDog((char*)"Models/HeadDog.obj");       // Cabeza del perro
	Model DogTail((char*)"Models/TailDog.obj");       // Cola del perro
	Model F_RightLeg((char*)"Models/F_RightLegDog.obj"); // Pata delantera derecha
	Model F_LeftLeg((char*)"Models/F_LeftLegDog.obj");   // Pata delantera izquierda
	Model B_RightLeg((char*)"Models/B_RightLegDog.obj"); // Pata trasera derecha
	Model B_LeftLeg((char*)"Models/B_LeftLegDog.obj");   // Pata trasera izquierda
	Model Piso((char*)"Models/piso.obj");             // Suelo
	Model Ball((char*)"Models/ball.obj");             // Pelota


	// Configuración de VAO y VBO para el cubo de luz
	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);                       // Genera un VAO
	glGenBuffers(1, &VBO);                            // Genera un VBO
	glBindVertexArray(VAO);                           // Vincula el VAO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);               // Vincula el VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // Sube vértices

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0); // Layout posición
	glEnableVertexAttribArray(0);                      // Habilita atributo 0

	// Normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))); // Layout normales
	glEnableVertexAttribArray(1);                      // Habilita atributo 1

	// Set texture units
	lightingShader.Use();                              // Usa shader de iluminación
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.difuse"), 0);  // Asigna diffuse a unidad 0
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.specular"), 1);// Asigna specular a unidad 1

	// Proyección en perspectiva
	glm::mat4 projection = glm::perspective(
		camera.GetZoom(),                              // Field of view
		(GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT,// Aspect ratio
		0.1f, 100.0f);                                 // Near y far planes

	// Game loop
	while (!glfwWindowShouldClose(window))             // Bucle principal hasta cerrar ventana
	{

		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();           // Tiempo actual
		deltaTime = currentFrame - lastFrame;           // Diferencia respecto al frame anterior
		lastFrame = currentFrame;                       // Actualiza tiempo del último frame

		// Procesa eventos y actualizaciones
		glfwPollEvents();                               // Captura eventos (teclado/ratón)
		DoMovement();                                   // Actualiza cámara según teclas
		Animation();                                    // Ejecuta animación (pelota/perro)

		// Limpia buffers de color y profundidad
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);           // Color de fondo
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Limpia pantalla

		// Habilita prueba de profundidad
		glEnable(GL_DEPTH_TEST);

		// Guarda modelo temporal
		glm::mat4 modelTemp = glm::mat4(1.0f);          // Matriz identidad

		// Usa shader de iluminación para dibujar escena
		lightingShader.Use();

		// Vincula la unidad de texturas 0 al sampler "diffuse" del shader
		glUniform1i(glGetUniformLocation(lightingShader.Program, "diffuse"), 0);
		//glUniform1i(glGetUniformLocation(lightingShader.Program, "specular"),1); // (opcional) unidad 1 para specular, comentada

		// Obtiene la ubicación del uniform "viewPos" en el shader
		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		// Sube la posición de la cámara al shader para cálculos de iluminación especular
		glUniform3f(viewPosLoc,
			camera.GetPosition().x,
			camera.GetPosition().y,
			camera.GetPosition().z);

		// --- Configuración de la luz direccional ---
		// Dirección desde la que incide la luz
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"),
			-0.2f, -1.0f, -0.3f);
		// Componente ambiental de la luz direccional
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"),
			0.6f, 0.6f, 0.6f);
		// Componente difusa de la luz direccional
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"),
			0.6f, 0.6f, 0.6f);
		// Componente especular de la luz direccional
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"),
			0.3f, 0.3f, 0.3f);

		// --- Configuración de la luz puntual 0 ---
		// Calcula color pulsante basándose en Light1 y el tiempo
		glm::vec3 lightColor;
		lightColor.x = abs(sin(glfwGetTime() * Light1.x));
		lightColor.y = abs(sin(glfwGetTime() * Light1.y));
		lightColor.z = sin(glfwGetTime() * Light1.z);

		// Posición de la luz puntual 0
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"),
			pointLightPositions[0].x,
			pointLightPositions[0].y,
			pointLightPositions[0].z);
		// Componente ambiental de la luz puntual 0
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"),
			lightColor.x, lightColor.y, lightColor.z);
		// Componente difusa de la luz puntual 0
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"),
			lightColor.x, lightColor.y, lightColor.z);
		// Componente especular de la luz puntual 0
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"),
			1.0f, 0.2f, 0.2f);
		// Parámetros de atenuación: constante, lineal y cuadrática
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.045f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.075f);

		// --- Configuración del spot light (foco) ---
		// Posición del foco (misma que la cámara)
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"),
			camera.GetPosition().x,
			camera.GetPosition().y,
			camera.GetPosition().z);
		// Dirección del foco (hacia donde mira la cámara)
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"),
			camera.GetFront().x,
			camera.GetFront().y,
			camera.GetFront().z);
		// Componente ambiental del foco
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"),
			0.2f, 0.2f, 0.8f);
		// Componente difusa del foco
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"),
			0.2f, 0.2f, 0.8f);
		// Componente especular del foco
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"),
			0.0f, 0.0f, 0.0f);
		// Atenuación del foco
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.3f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.7f);
		// Ángulo interior (cutOff) del cono del foco
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"),
			glm::cos(glm::radians(12.0f)));
		// Ángulo exterior (outerCutOff) del cono del foco
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"),
			glm::cos(glm::radians(18.0f)));

		// Establece el brillo especular del material
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"),
			5.0f);

		// Prepara la matriz de vista desde la cámara
		glm::mat4 view = camera.GetViewMatrix();

		// Obtiene las ubicaciones de los uniforms de matrices en el shader
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		// Sube la matriz de vista al shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		// Sube la matriz de proyección al shader (declarada antes como 'projection')
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		// Inicializa la matriz modelo a identidad
		glm::mat4 model(1);

		// --- Dibujo del piso ---
		// Actualiza uniform 'model' con la matriz identidad
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Piso.Draw(lightingShader);  // Dibuja el suelo

		// --- Dibujo del perro ---
		// Sin transparencia
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);

		// Body: traslada y rota según dogPos y dogRot
		model = glm::translate(glm::mat4(1), dogPos);
		model = glm::rotate(model, glm::radians(dogRot), glm::vec3(0, 1, 0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		DogBody.Draw(lightingShader);

		// Head: parte de la matriz del cuerpo y rota la cabeza
		glm::mat4 headMat = model;
		headMat = glm::translate(headMat, glm::vec3(0.0f, 0.093f, 0.208f));
		headMat = glm::rotate(headMat, glm::radians(head), glm::vec3(0, 0, 1));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(headMat));
		HeadDog.Draw(lightingShader);

		// Tail: parte de la matriz del cuerpo y rota la cola
		glm::mat4 tailMat = model;
		tailMat = glm::translate(tailMat, glm::vec3(0.0f, 0.026f, -0.288f));
		tailMat = glm::rotate(tailMat, glm::radians(tail), glm::vec3(0, 0, -1));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(tailMat));
		DogTail.Draw(lightingShader);

		// Patas: se repite patrón para cada pata, ajustando translate y rotate
		// Front Left Leg
		glm::mat4 fllMat = model;
		fllMat = glm::translate(fllMat, glm::vec3(0.112f, -0.044f, 0.074f));
		fllMat = glm::rotate(fllMat, glm::radians(FLegs), glm::vec3(-1, 0, 0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(fllMat));
		F_LeftLeg.Draw(lightingShader);
		// Front Right Leg
		glm::mat4 frlMat = model;
		frlMat = glm::translate(frlMat, glm::vec3(-0.111f, -0.055f, 0.074f));
		frlMat = glm::rotate(frlMat, glm::radians(FLegs), glm::vec3(1, 0, 0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(frlMat));
		F_RightLeg.Draw(lightingShader);
		// Back Left Leg
		glm::mat4 bllMat = model;
		bllMat = glm::translate(bllMat, glm::vec3(0.082f, -0.046f, -0.218f));
		bllMat = glm::rotate(bllMat, glm::radians(RLegs), glm::vec3(1, 0, 0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(bllMat));
		B_LeftLeg.Draw(lightingShader);
		// Back Right Leg
		glm::mat4 brlMat = model;
		brlMat = glm::translate(brlMat, glm::vec3(-0.083f, -0.057f, -0.231f));
		brlMat = glm::rotate(brlMat, glm::radians(RLegs), glm::vec3(-1, 0, 0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(brlMat));
		B_RightLeg.Draw(lightingShader);

		// --- Dibujo de la pelota con transparencia ---
		// Activa mezcla alfa
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 1);
		// Rota la pelota según rotBall
		model = glm::rotate(glm::mat4(1), glm::radians(rotBall), glm::vec3(0, 1, 0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Ball.Draw(lightingShader);
		// Desactiva mezcla alfa
		glDisable(GL_BLEND);
		glBindVertexArray(0);

		// --- Dibujo de la lámpara como cubo emissivo ---
		// Cambia al shader de lámpara
		lampShader.Use();
		// Obtiene locations de matrices en el shader de lámpara
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");
		// Sube matrices de vista y proyección
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		// Modelo de lámpara: posicionado en lightPos y escalado pequeño
		model = glm::translate(glm::mat4(1), lightPos);
		model = glm::scale(model, glm::vec3(0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		// Dibuja cubo de luz
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		// Intercambia buffers para mostrar el frame renderizado
		glfwSwapBuffers(window);

	}


	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();



	return 0;
}

// Mueve/modifica la posición de la cámara según la entrada del usuario
void DoMovement()
{
	// Controles de cámara: avanzar
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
	{
		// Procesa movimiento hacia delante usando deltaTime
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}

	// Controles de cámara: retroceder
	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{
		// Procesa movimiento hacia atrás usando deltaTime
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}

	// Controles de cámara: desplazarse a la izquierda
	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{
		// Procesa movimiento lateral izquierdo usando deltaTime
		camera.ProcessKeyboard(LEFT, deltaTime);
	}

	// Controles de cámara: desplazarse a la derecha
	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{
		// Procesa movimiento lateral derecho usando deltaTime
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}

	// Ajuste de la posición de la luz puntual en X positiva
	if (keys[GLFW_KEY_T])
	{
		pointLightPositions[0].x += 0.01f;
	}
	// Ajuste de la posición de la luz puntual en X negativa
	if (keys[GLFW_KEY_G])
	{
		pointLightPositions[0].x -= 0.01f;
	}

	// Ajuste de la posición de la luz puntual en Y positiva
	if (keys[GLFW_KEY_Y])
	{
		pointLightPositions[0].y += 0.01f;
	}
	// Ajuste de la posición de la luz puntual en Y negativa
	if (keys[GLFW_KEY_H])
	{
		pointLightPositions[0].y -= 0.01f;
	}

	// Ajuste de la posición de la luz puntual en Z negativa
	if (keys[GLFW_KEY_U])
	{
		pointLightPositions[0].z -= 0.1f;
	}
	// Ajuste de la posición de la luz puntual en Z positiva
	if (keys[GLFW_KEY_J])
	{
		pointLightPositions[0].z += 0.01f;
	}
}


// Se llama cada vez que se presiona o libera una tecla en la ventana GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// Si se presiona ESC, marcar la ventana para cerrar
	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	// Asegurarse de que la tecla esté en el rango válido del array 'keys'
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			// Marcar tecla como presionada
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			// Marcar tecla como liberada
			keys[key] = false;
		}
	}

	// Toggle del efecto de luz pulsando SPACE
	if (keys[GLFW_KEY_SPACE])
	{
		active = !active;  // Alternar estado de 'active'
		if (active)
		{
			// Si se activa, configurar parámetro Light1 para animación de luz
			Light1 = glm::vec3(0.2f, 0.8f, 1.0f);
		}
		else
		{
			// Si se desactiva, poner Light1 a cero (sin componente de animación)
			Light1 = glm::vec3(0);
		}
	}

	// Toggle de animación de la pelota pulsando L
	if (keys[GLFW_KEY_L])
	{
		AnimBall = !AnimBall;  // Alternar estado de animación de la pelota
	}

	// Toggle de animación del perro pulsando K
	if (keys[GLFW_KEY_K])
	{
		dogAnim = !dogAnim;    // Alternar estado de la máquina de estados del perro
	}
}

// Función que controla la animación de la pelota y del perro
void Animation() {
	// Si está activada la animación de la pelota, incrementar su rotación
	if (AnimBall) {
		rotBall += 0.4f;              // Aumenta el ángulo de la bola
	}

	// Si está activada la animación del perro, incrementar su rotación
	if (AnimDog) {
		rotDog -= 0.6f;               // Disminuye el ángulo de rotación del perro
	}

	// Verificar si el perro ha vuelto al centro (posición y rotación casi cero)
	// y ya ha recorrido todas las direcciones (dirz, dirx y diag)
	if (dogPos.x <= 0.001f && dogPos.x >= -0.001f &&
		dogPos.z <= 0.001f && dogPos.z >= -0.001f &&
		dogRot <= 0.001f && dogRot >= -0.001f &&
		dirz && dirx && diag)
	{
		// Reiniciar flags de dirección
		dirz = false;
		dirx = false;
		diag = false;
		dogAnim = 0;                 // Detener la animación del perro
		return;                      // Salir para no seguir moviendo
	}

	// Si dogAnim == 1, ejecutar la animación de “caminar”
	if (dogAnim == 1) { // Walk animation
		// Primer tramo: avanzar en el eje Z positivo hasta z = 2.2
		if (dogPos.z <= 2.2f && dirz == false && diag == false) {
			// Alternar movimiento de patas, cabeza y cola para simular paso
			if (!step) {
				RLegs += 0.03f; FLegs += 0.03f;
				head += 0.03f; tail += 0.03f;
				if (RLegs > 15.0f) step = true;  // Cambiar de fase de paso
			}
			else {
				RLegs -= 0.03f; FLegs -= 0.03f;
				head -= 0.03f; tail -= 0.03f;
				if (RLegs < -15.0f) step = false; // Volver a fase inicial
			}
			dogPos.z += 0.0001f;                // Mover posición en Z
		}
		else {
			// Giro de 90 grados cuando alcanza el límite Z
			if (dogRot <= 90.0f) { // gira
				if (!step) {
					RLegs += 0.03f; FLegs += 0.03f;
					head += 0.03f; tail += 0.03f;
					if (RLegs > 15.0f) step = true;
				}
				else {
					RLegs -= 0.03f; FLegs -= 0.03f;
					head -= 0.03f; tail -= 0.03f;
					if (RLegs < -15.0f) step = false;
				}
				dogRot += 0.01f;                  // Incrementar rotación hasta 90°
			}
			else {
				dirz = true;                      // Marca que completó primer tramo
				// Segundo tramo: avanzar en el eje X positivo hasta x = 2.0
				if (dogPos.x <= 2.0f && dirx == false && diag == false) {
					if (!step) {
						RLegs += 0.03f; FLegs += 0.03f;
						head += 0.03f; tail += 0.03f;
						if (RLegs > 15.0f) step = true;
					}
					else {
						RLegs -= 0.03f; FLegs -= 0.03f;
						head -= 0.03f; tail -= 0.03f;
						if (RLegs < -15.0f) step = false;
					}
					dogPos.x += 0.0001f;            // Mover posición en X
				}
				else {
					// Giro hacia atrás hasta 180 grados
					if (dogRot <= 180.0f) { // gira hacia atrás
						if (!step) {
							RLegs += 0.03f; FLegs += 0.03f;
							head += 0.03f; tail += 0.03f;
							if (RLegs > 15.0f) step = true;
						}
						else {
							RLegs -= 0.03f; FLegs -= 0.03f;
							head -= 0.03f; tail -= 0.03f;
							if (RLegs < -15.0f) step = false;
						}
						dogRot += 0.01f;              // Incrementar rotación hasta 180°
					}
					else { // Tercer tramo: retroceder en Z negativo hasta z = -2.1
						if (dogPos.z >= -2.1f && dirz == true && diag == false) {
							if (!step) {
								RLegs += 0.03f; FLegs += 0.03f;
								head += 0.03f; tail += 0.03f;
								if (RLegs > 15.0f) step = true;
							}
							else {
								RLegs -= 0.03f; FLegs -= 0.03f;
								head -= 0.03f; tail -= 0.03f;
								if (RLegs < -15.0f) step = false;
							}
							dogPos.z -= 0.0001f;        // Mover posición en Z negativo
						}
						else {
							// Giro a 320 grados para dirigirse al tramo diagonal
							if (dogRot <= 320.0f) { // gira hacia la diagonal
								if (!step) {
									RLegs += 0.03f; FLegs += 0.03f;
									head += 0.03f; tail += 0.03f;
									if (RLegs > 15.0f) step = true;
								}
								else {
									RLegs -= 0.03f; FLegs -= 0.03f;
									head -= 0.03f; tail -= 0.03f;
									if (RLegs < -15.0f) step = false;
								}
								dogRot += 0.01f;          // Incrementar rotación hasta 320°
							}
							else {
								// Cuarto tramo: marcar diagonal y mover en Z+ y X−
								dirz = false;
								dirx = true;
								diag = true;
								if (dogPos.z <= 0.0f && dogPos.x >= 0.0f) {
									if (!step) {
										RLegs += 0.03f; FLegs += 0.03f;
										head += 0.03f; tail += 0.03f;
										if (RLegs > 15.0f) step = true;
									}
									else {
										RLegs -= 0.03f; FLegs -= 0.03f;
										head -= 0.03f; tail -= 0.03f;
										if (RLegs < -15.0f) step = false;
									}
									dogPos.z += 0.0001f;    // Mover Z hacia positivo
									dogPos.x -= 0.0001f;    // Mover X hacia negativo
								}
							}
						}
					}
				}
			}
		}
	}
}


void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}