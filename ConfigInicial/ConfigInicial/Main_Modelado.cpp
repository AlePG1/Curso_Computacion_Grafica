//García González Alejandro
//422066992
//Práctica 4
//26 de febrero del 2025


#include<iostream>

//#define GLEW_STATIC

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



// Shaders
#include "Shader.h"

void Inputs(GLFWwindow *window);


const GLint WIDTH = 800, HEIGHT = 600;
float movX=0.0f;
float movY=0.0f;
float movZ=-9.0f;
float rot = 0.0f;
int main() {
	glfwInit();
	//Verificación de compatibilidad 
	// Set all the required options for GLFW
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);*/

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Practica 4 Garcia Gonzalez Alejandro", nullptr, nullptr);

	int screenWidth, screenHeight;

	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

	//Verificación de errores de creacion  ventana
	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;

	//Verificación de errores de inicialización de glew

	if (GLEW_OK != glewInit()) {
		std::cout << "Failed to initialise GLEW" << std::endl;
		return EXIT_FAILURE;
	}


	// Define las dimensiones del viewport
	glViewport(0, 0, screenWidth, screenHeight);


	// Setup OpenGL options
	glEnable(GL_DEPTH_TEST);

	// enable alpha support
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	// Build and compile our shader program
	Shader ourShader("Shader/core.vs", "Shader/core.frag");


	// Set up vertex data (and buffer(s)) and attribute pointers

	

	// use with Perspective Projection
	float vertices[] = {
		// ----------------------------------------------------------------
		// Cara frontal (color zorro)
		-0.5f, -0.5f,  0.5f,   0.89f, 0.40f, 0.13f,
		 0.5f, -0.5f,  0.5f,   0.89f, 0.40f, 0.13f,
		 0.5f,  0.5f,  0.5f,   0.89f, 0.40f, 0.13f,
		 0.5f,  0.5f,  0.5f,   0.89f, 0.40f, 0.13f,
		-0.5f,  0.5f,  0.5f,   0.89f, 0.40f, 0.13f,
		-0.5f, -0.5f,  0.5f,   0.89f, 0.40f, 0.13f,

		// ----------------------------------------------------------------
		// Cara trasera (color negro)
		-0.5f, -0.5f, -0.5f,   0.89f, 0.40f, 0.13f,
		 0.5f, -0.5f, -0.5f,   0.89f, 0.40f, 0.13f,
		 0.5f,  0.5f, -0.5f,   0.89f, 0.40f, 0.13f,
		 0.5f,  0.5f, -0.5f,   0.89f, 0.40f, 0.13f,
		-0.5f,  0.5f, -0.5f,   0.89f, 0.40f, 0.13f,
		-0.5f, -0.5f, -0.5f,   0.89f, 0.40f, 0.13f,

		// ----------------------------------------------------------------
		// Cara derecha (color zorro)
		 0.5f, -0.5f,  0.5f,   0.89f, 0.40f, 0.13f,
		 0.5f, -0.5f, -0.5f,   0.89f, 0.40f, 0.13f,
		 0.5f,  0.5f, -0.5f,   0.89f, 0.40f, 0.13f,
		 0.5f,  0.5f, -0.5f,   0.89f, 0.40f, 0.13f,
		 0.5f,  0.5f,  0.5f,   0.89f, 0.40f, 0.13f,
		 0.5f, -0.5f,  0.5f,   0.89f, 0.40f, 0.13f,

		 // ----------------------------------------------------------------
		 // Cara izquierda (color zorro)
		 -0.5f,  0.5f,  0.5f,   0.89f, 0.40f, 0.13f,
		 -0.5f,  0.5f, -0.5f,   0.89f, 0.40f, 0.13f,
		 -0.5f, -0.5f, -0.5f,   0.89f, 0.40f, 0.13f,
		 -0.5f, -0.5f, -0.5f,   0.89f, 0.40f, 0.13f,
		 -0.5f, -0.5f,  0.5f,   0.89f, 0.40f, 0.13f,
		 -0.5f,  0.5f,  0.5f,   0.89f, 0.40f, 0.13f,

		 // ----------------------------------------------------------------
		 // Cara inferior (color zorro)
		 -0.5f, -0.5f, -0.5f,   0.89f, 0.40f, 0.13f,
		  0.5f, -0.5f, -0.5f,   0.89f, 0.40f, 0.13f,
		  0.5f, -0.5f,  0.5f,   0.89f, 0.40f, 0.13f,
		  0.5f, -0.5f,  0.5f,   0.89f, 0.40f, 0.13f,
		 -0.5f, -0.5f,  0.5f,   0.89f, 0.40f, 0.13f,
		 -0.5f, -0.5f, -0.5f,   0.89f, 0.40f, 0.13f,

		 // ----------------------------------------------------------------
		 // Cara superior (color zorro)
		 -0.5f,  0.5f, -0.5f,   0.89f, 0.40f, 0.13f,
		  0.5f,  0.5f, -0.5f,   0.89f, 0.40f, 0.13f,
		  0.5f,  0.5f,  0.5f,   0.89f, 0.40f, 0.13f,
		  0.5f,  0.5f,  0.5f,   0.89f, 0.40f, 0.13f,
		 -0.5f,  0.5f,  0.5f,   0.89f, 0.40f, 0.13f,
		 -0.5f,  0.5f, -0.5f,   0.89f, 0.40f, 0.13f
	};

	// ----------------------------------------------
    // Arreglo de vértices para un cubo completamente NEGRO
	float verticesBlack[] = {
		// Cara frontal
		-0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 0.0f,

		// Cara trasera
		-0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,   0.0f, 0.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,   0.0f, 0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,   0.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 0.0f,

		// Cara derecha
		 0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,   0.0f, 0.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,   0.0f, 0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 0.0f,

		 // Cara izquierda
		 -0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 0.0f,
		 -0.5f,  0.5f, -0.5f,   0.0f, 0.0f, 0.0f,
		 -0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 0.0f,
		 -0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 0.0f,
		 -0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 0.0f,
		 -0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 0.0f,

		 // Cara inferior
		 -0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 0.0f,
		  0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 0.0f,
		  0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 0.0f,
		  0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 0.0f,
		 -0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 0.0f,
		 -0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 0.0f,

		 // Cara superior
		 -0.5f,  0.5f, -0.5f,   0.0f, 0.0f, 0.0f,
		  0.5f,  0.5f, -0.5f,   0.0f, 0.0f, 0.0f,
		  0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 0.0f,
		  0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 0.0f,
		 -0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 0.0f,
		 -0.5f,  0.5f, -0.5f,   0.0f, 0.0f, 0.0f
	};

	// ----------------------------------------------
// Arreglo de vértices para un cubo completamente AMARILLO
	float verticesYellow[] = {
		// Cara frontal
		-0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 0.0f,

		// Cara trasera
		-0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 0.0f,

		// Cara derecha
		 0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 0.0f,

		 // Cara izquierda
		 -0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 0.0f,
		 -0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 0.0f,
		 -0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 0.0f,
		 -0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 0.0f,
		 -0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 0.0f,
		 -0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 0.0f,

		 // Cara inferior
		 -0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 0.0f,
		  0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 0.0f,
		  0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 0.0f,
		  0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 0.0f,
		 -0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 0.0f,
		 -0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 0.0f,

		 // Cara superior
		 -0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 0.0f,
		  0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 0.0f,
		  0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 0.0f,
		  0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 0.0f,
		 -0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 0.0f,
		 -0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 0.0f
	};

	// --------------------------------------------------
    // VAO/VBO para el cubo AMARILLO
	GLuint VAO3, VBO3;
	glGenVertexArrays(1, &VAO3);
	glGenBuffers(1, &VBO3);

	glBindVertexArray(VAO3);

	glBindBuffer(GL_ARRAY_BUFFER, VBO3);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesYellow), verticesYellow, GL_STATIC_DRAW);

	// Atributo de posición (igual que antes)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Atributo de color (todo amarillo)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	//Fin AMARILLO


	// --------------------------------------------------
    // VAO/VBO para el cubo NEGRO
	GLuint VAO2, VBO2;
	glGenVertexArrays(1, &VAO2);
	glGenBuffers(1, &VBO2);

	glBindVertexArray(VAO2);

	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesBlack), verticesBlack, GL_STATIC_DRAW);

	// Atributo de posición (igual que antes)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Atributo de color (todas las caras en negro)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
    // Fin NEGRO



	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//glGenBuffers(1, &EBO);

	// Enlazar  Vertex Array Object
	glBindVertexArray(VAO);

	//2.- Copiamos nuestros arreglo de vertices en un buffer de vertices para que OpenGL lo use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 3.Copiamos nuestro arreglo de indices en  un elemento del buffer para que OpenGL lo use
	/*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);*/

	// 4. Despues colocamos las caracteristicas de los vertices

	//Posicion
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	//Color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)

	
	glm::mat4 projection=glm::mat4(1);

	projection = glm::perspective(glm::radians(45.0f), (GLfloat)screenWidth / (GLfloat)screenHeight, 0.1f, 100.0f);//FOV, Radio de aspecto,znear,zfar
	//projection = glm::ortho(0.0f, (GLfloat)screenWidth, 0.0f, (GLfloat)screenHeight, 0.1f, 1000.0f);//Izq,Der,Fondo,Alto,Cercania,Lejania
	while (!glfwWindowShouldClose(window))
	{
		
		Inputs(window);
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Render
		// Clear the colorbuffer
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);


		// Draw our first triangle
		ourShader.Use();
		glm::mat4 model=glm::mat4(1);
		glm::mat4 view=glm::mat4(1);
	

		view = glm::translate(view, glm::vec3(movX,movY, movZ));
		view = glm::rotate(view, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));

		GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
		GLint projecLoc = glGetUniformLocation(ourShader.Program, "projection");


		glUniformMatrix4fv(projecLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	

		glBindVertexArray(VAO);
	

		//Pata 1
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f)); // Tamaño pata mesa
		model = glm::translate(model, glm::vec3(-1.2f, -0.2f, 2.0f));//posición pata
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		////Pata 2
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f)); // Tamaño pata mesa
		model = glm::translate(model, glm::vec3(-1.2f, -0.2f, -2.0f));//posición pata
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		////Pata 3
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f)); // Tamaño pata mesa
		model = glm::translate(model, glm::vec3(1.2f, -0.2f, -2.0f));//posición pata
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		////Pata 4
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f)); // Tamaño pata mesa
		model = glm::translate(model, glm::vec3(1.2f, -0.2f, 2.0f));//posición pata
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Torso
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(2.0f, 0.7f, 4.0f)); // Tamaño caja
		model = glm::translate(model, glm::vec3(0.0f, 0.7f, 0.0f));//posición pata
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//CaraP1
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(2.0f, 1.0f, 1.0f)); // Tamaño caja
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, 1.9f));//posición pata
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//CaraP2
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(1.0f, 0.5f, 0.2f)); // Tamaño caja
		model = glm::translate(model, glm::vec3(0.0f, 1.5f, 12.7f));//posición pata
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//CaraP4
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.5f, 0.3f, 0.2f)); // Tamaño caja
		model = glm::translate(model, glm::vec3(0.0f, 2.0f, 13.5f));//posición pata
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Oreja izquierda1
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f)); // Tamaño caja
		model = glm::translate(model, glm::vec3(-2.8f, 5.1f, 5.0f));//posición oreja
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Oreja derecha1
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f)); // Tamaño caja
		model = glm::translate(model, glm::vec3(2.8f, 5.1f, 5.0f));//posición oreja
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);


        // Ahora dibujamos el cubo NEGRO
		glBindVertexArray(VAO2);  // Activamos el VAO del cubo negro

		//Cola
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(1.0f, 0.2f, 2.2f)); // Tamaño caja
		model = glm::translate(model, glm::vec3(0.0f, 3.0f, -1.0f));//posición pata
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Boca
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.9f, 0.2f, 0.7f)); // Tamaño caja
		model = glm::translate(model, glm::vec3(0.0f, 3.4f, 3.0f));//posición pata
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Ojos izquierdo
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.3f, 0.2f, 0.7f)); // Tamaño caja
		model = glm::translate(model, glm::vec3(1.6f, 5.3f, 3.0f));//posición pata
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Ojos derecho
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.3f, 0.2f, 0.7f)); // Tamaño caja
		model = glm::translate(model, glm::vec3(-1.6f, 5.3f, 3.0f));//posición pata
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Oreja izquierda2
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f)); // Tamaño caja
		model = glm::translate(model, glm::vec3(-2.0f, 5.1f, 5.0f));//posición oreja
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Oreja derecha2
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f)); // Tamaño caja
		model = glm::translate(model, glm::vec3(2.0f, 5.1f, 5.0f));//posición oreja
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Oreja izquierda3
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f)); // Tamaño caja
		model = glm::translate(model, glm::vec3(-2.8f, 5.8f, 5.0f));//posición oreja
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Oreja derecha3
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f)); // Tamaño caja
		model = glm::translate(model, glm::vec3(2.8f, 5.8f, 5.0f));//posición oreja
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//CaraP3
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.5f, 0.3f, 0.2f)); // Tamaño caja
		model = glm::translate(model, glm::vec3(0.0f, 2.9f, 13.5f));//posición pata
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Pata 1P
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.6f, 0.2f, 0.6f)); // Tamaño pata mesa
		model = glm::translate(model, glm::vec3(-1.2f, -2.5f, 2.0f));//posición pata
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		////Pata 2P
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.6f, 0.2f, 0.6f)); // Tamaño pata mesa
		model = glm::translate(model, glm::vec3(-1.2f, -2.5f, -2.0f));//posición pata
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		////Pata 3P
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.6f, 0.2f, 0.6f)); // Tamaño pata mesa
		model = glm::translate(model, glm::vec3(1.2f, -2.5f, -2.0f));//posición pata
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		////Pata 4P
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.6f, 0.2f, 0.6f)); // Tamaño pata mesa
		model = glm::translate(model, glm::vec3(1.2f, -2.5f, 2.0f));//posición pata
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);



		// Dibujas primero tu cubo original con VAO (zorro)
		// Dibujas luego el cubo negro con VAO2
		// Ahora dibujas el cubo amarillo con VAO3

		glBindVertexArray(VAO3); // Activamos el VAO del cubo amarillo

		//Torso
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(2.0f, 0.5f, 4.0f)); // Tamaño caja
		model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f));//posición pata
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);



		glBindVertexArray(0);
		// Swap the screen buffers
		glfwSwapBuffers(window);
	
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);


	glfwTerminate();
	return EXIT_SUCCESS;
 }

 void Inputs(GLFWwindow *window) {
	 if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)  //GLFW_RELEASE
		 glfwSetWindowShouldClose(window, true);
	 if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		 movX += 0.08f;
	 if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		 movX -= 0.08f;
	 if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
		 movY += 0.08f;
	 if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
		 movY -= 0.08f;
	 if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		 movZ -= 0.08f;
	 if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		 movZ += 0.08f;
	 if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		 rot += 0.4f;
	 if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		 rot -= 0.4f;
 }


