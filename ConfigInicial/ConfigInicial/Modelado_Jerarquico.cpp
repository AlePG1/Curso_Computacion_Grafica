/*
*Practica 5                          	        García González Alejandro
*Fecha de entrega : 06 de marzo del 2025    	422066992
*/
#include<iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


// Shaders
#include "Shader.h"

void Inputs(GLFWwindow* window);


const GLint WIDTH = 1200, HEIGHT = 800;

//For Keyboard´para editar la vista
float	movX = -5.0f,
movY = 0.0f,
movZ = -15.0f,
rot = 0.0f;

//For model esta es para manipular el modelo,esto permite generar la articulzacion
float	hombro = 0.0f,
codo = 0.0f,
muneca = 0.0f,
Dedo1_A = 0.0f,
Dedo1_B = 0.0f,
Dedo1_C = 0.0f,

Dedo2_A = 0.0f,
Dedo2_B = 0.0f,
Dedo2_C = 0.0f,

Dedo3_A = 0.0f,
Dedo3_B = 0.0f,
Dedo3_C = 0.0f,

Dedo4_A = 0.0f,
Dedo4_B = 0.0f,
Dedo4_C = 0.0f,

Dedo5_A = 0.0f,
Dedo5_B = 0.0f;


int main() {
	glfwInit();
	//Verificación de compatibilidad 
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Practica 5 Garcia Gonzalez Alejandro", nullptr, nullptr);

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
	// use with Orthographic Projection




	// use with Perspective Projection.Componentes en : X Y Z
	float vertices[] = {
		-0.5f, -0.5f, 0.5f,
		0.5f, -0.5f, 0.5f,
		0.5f,  0.5f, 0.5f,
		0.5f,  0.5f, 0.5f,
		-0.5f,  0.5f, 0.5f,
		-0.5f, -0.5f, 0.5f,

		-0.5f, -0.5f,-0.5f,
		 0.5f, -0.5f,-0.5f,
		 0.5f,  0.5f,-0.5f,
		 0.5f,  0.5f,-0.5f,
		-0.5f,  0.5f,-0.5f,
		-0.5f, -0.5f,-0.5f,

		 0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  -0.5f, 0.5f,

		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,

		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f,  0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		0.5f,  0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
	};




	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//glGenBuffers(1, &EBO);

	// Enlazar  Vertex Array Object
	glBindVertexArray(VAO);

	//2.- Copiamos nuestros arreglo de vertices en un buffer de vertices para que OpenGL lo use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	//Posicion
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);



	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)


	glm::mat4 projection = glm::mat4(1);

	//Esto genera la matriz de proyeccion en perspectiva
	projection = glm::perspective(glm::radians(45.0f), (GLfloat)screenWidth / (GLfloat)screenHeight, 0.1f, 100.0f);//FOV, Radio de aspecto,znear,zfar
	glm::vec3 color = glm::vec3(0.0f, 0.0f, 1.0f);
	while (!glfwWindowShouldClose(window))
	{
		//Funciones para el control de teclado

		Inputs(window);
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Render
		// Clear the colorbuffer
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		//Aqui se comienza a dibujar 
		ourShader.Use();
		glm::mat4 model = glm::mat4(1);//Funciones para manipular el modelo
		glm::mat4 view = glm::mat4(1);//Funciones para manipular la vista
		//Elementos nuevos para ayudar a controlar la informacion de la posicion de cada uno de los elementos
		glm::mat4 modelTemp = glm::mat4(1.0f); //Temp
		glm::mat4 modelTemp2 = glm::mat4(1.0f); //Temp
		glm::mat4 modelTemp3 = glm::mat4(1.0f); //Temp
		glm::mat4 modelTemp4 = glm::mat4(1.0f); //Temp
		glm::mat4 modelTemp5 = glm::mat4(1.0f); //Temp

		//View set up 
		view = glm::translate(view, glm::vec3(movX, movY, movZ));//Se incializan los valores  para la vista
		view = glm::rotate(view, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));

		GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
		GLint projecLoc = glGetUniformLocation(ourShader.Program, "projection");
		GLint uniformColor = ourShader.uniformColor;
		//SAE MANDAN A LA MATRIZ
		glUniformMatrix4fv(projecLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));


		glBindVertexArray(VAO);

		// Modelado del Biceps
		model = glm::rotate(model, glm::radians(hombro), glm::vec3(0.0f, 0.0, 1.0f)); // Se aplica la rotación para el movimiento del hombro en el eje Z
		modelTemp = model = glm::translate(model, glm::vec3(1.5f, 0.0f, 0.0f)); // Se almacena la transformación en una matriz temporal para conservar el pivote
		// Este punto de pivote se ubica a 1.5 unidades en X, permitiendo la articulación del hombro
		model = glm::scale(model, glm::vec3(3.0f, 1.0f, 1.75f)); // Se establecen las dimensiones del rectángulo en X, Y y Z
		color = glm::vec3(1.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36); // Se envían los datos a los shaders

		// Modelado del Antebrazo
		model = glm::translate(modelTemp, glm::vec3(1.5f, 0.0f, 0.0f)); // Se establece el punto de rotación en la articulación del codo
		model = glm::rotate(model, glm::radians(codo), glm::vec3(0.0f, 1.0f, 0.0f)); // Se rota el antebrazo en el eje Y
		modelTemp = model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f)); // Se almacena la transformación previa para mantener la referencia del pivote
		model = glm::scale(model, glm::vec3(2.0f, 1.0f, 1.75f)); // Se aplican las dimensiones al modelo
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36); // Se envían los datos a los shaders

		// Modelado de la Palma
		model = glm::translate(modelTemp, glm::vec3(1.0f, 0.0f, 0.0f)); // Se establece el punto de rotación de la muñeca
		model = glm::rotate(model, glm::radians(muneca), glm::vec3(1.0f, 0.0f, 0.0f)); // Se rota la muñeca en el eje X
		modelTemp2 = modelTemp = model = glm::translate(model, glm::vec3(0.25f, 0.0f, 0.0f)); // Se guarda la referencia de transformación
		model = glm::scale(model, glm::vec3(0.5f, 1.0f, 1.75f)); // Se aplican las dimensiones del modelo
		color = glm::vec3(1.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36); // Se envían los datos a los shaders

		// Modelado del Dedo 1A
		model = glm::translate(modelTemp, glm::vec3(0.25f, 0.35f, 0.75f)); // Se establece la posición base del dedo en la palma
		model = glm::rotate(model, glm::radians(Dedo1_A), glm::vec3(0.0f, 0.0f, 1.0f)); // Se aplica rotación en el eje Z
		modelTemp = model = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f)); // Se define la referencia para la siguiente falange
		model = glm::scale(model, glm::vec3(1.0f, 0.3f, 0.25f)); // Se ajusta la escala del modelo
		color = glm::vec3(45.0f / 255.0f, 126.0f / 255.0f, 174.0f / 255.0f); // Color del segmento
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36); // Se envían los datos a los shaders

		// Modelado del Dedo 1B
		model = glm::translate(modelTemp, glm::vec3(0.5f, 0.0f, 0.0f)); // Se posiciona la articulación intermedia
		model = glm::rotate(model, glm::radians(Dedo1_B), glm::vec3(0.0f, 0.0f, 1.0f)); // Se rota el segmento en el eje Z
		modelTemp = model = glm::translate(model, glm::vec3(0.375f, 0.0f, 0.0f)); // Se establece el pivote para la siguiente falange
		model = glm::scale(model, glm::vec3(0.75f, 0.3f, 0.25f)); // Se ajusta el tamaño del modelo
		color = glm::vec3(255.0f / 255.0f, 0.0f / 255.0f, 64.0f / 255.0f); // Se define el color del segmento
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36); // Se envían los datos a los shaders

		// Modelado del Dedo 1C
		model = glm::translate(modelTemp, glm::vec3(0.375f, 0.0f, 0.0f)); // Se posiciona la última articulación del dedo
		model = glm::rotate(model, glm::radians(Dedo1_C), glm::vec3(0.0f, 0.0f, 1.0f)); // Se rota la falange distal
		modelTemp = model = glm::translate(model, glm::vec3(0.25f, 0.0f, 0.0f)); // Se define el punto de referencia
		model = glm::scale(model, glm::vec3(0.5f, 0.3f, 0.25f)); // Se ajusta el tamaño del modelo
		color = glm::vec3(0.0f / 255.0f, 255.0f / 255.0f, 191.0f / 255.0f); // Color asignado al segmento
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36); // Se envían los datos a los shaders




		// Modelo del Dedo 2A (Base del dedo)
		model = glm::translate(modelTemp2, glm::vec3(0.25f, 0.35f, 0.25f)); // Posiciona la base del dedo en la palma
		model = glm::rotate(model, glm::radians(Dedo2_A), glm::vec3(0.0f, 0.0f, 1.0f)); // Aplica rotación en el eje Z
		modelTemp = model = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f)); // Guarda la referencia para la siguiente falange
		model = glm::scale(model, glm::vec3(1.0f, 0.3f, 0.25f)); // Ajusta el tamaño del segmento
		color = glm::vec3(45.0f / 255.0f, 126.0f / 255.0f, 174.0f / 255.0f); // Color del segmento
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36); // Envía datos a los shaders

		// Modelo del Dedo 2B (Falange intermedia)
		model = glm::translate(modelTemp, glm::vec3(0.5f, 0.0f, 0.0f)); // Ajusta la posición para la articulación intermedia
		model = glm::rotate(model, glm::radians(Dedo2_B), glm::vec3(0.0f, 0.0f, 1.0f)); // Aplica rotación en el eje Z
		modelTemp = model = glm::translate(model, glm::vec3(0.375f, 0.0f, 0.0f)); // Establece el pivote de rotación
		model = glm::scale(model, glm::vec3(0.75f, 0.3f, 0.25f)); // Ajusta la escala del segmento
		color = glm::vec3(255.0f / 255.0f, 0.0f / 255.0f, 64.0f / 255.0f); // Color del segmento
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36); // Envía datos a los shaders

		// Modelo del Dedo 2C (Punta del dedo)
		model = glm::translate(modelTemp, glm::vec3(0.375f, 0.0f, 0.0f)); // Posiciona la última falange
		model = glm::rotate(model, glm::radians(Dedo2_C), glm::vec3(0.0f, 0.0f, 1.0f)); // Aplica rotación en el eje Z
		modelTemp = model = glm::translate(model, glm::vec3(0.25f, 0.0f, 0.0f)); // Ajusta la posición final
		model = glm::scale(model, glm::vec3(0.5f, 0.3f, 0.25f)); // Escala el segmento
		color = glm::vec3(0.0f / 255.0f, 255.0f / 255.0f, 191.0f / 255.0f); // Color del segmento
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36); // Envía datos a los shaders




		// Modelo del Dedo 3A (Base del dedo)
		model = glm::translate(modelTemp2, glm::vec3(0.25f, 0.35f, -0.25f)); // Posiciona la base del dedo en la palma
		model = glm::rotate(model, glm::radians(Dedo3_A), glm::vec3(0.0f, 0.0f, 1.0f)); // Rotación en el eje Z
		modelTemp = model = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 0.3f, 0.25f));
		color = glm::vec3(45.0f / 255.0f, 126.0f / 255.0f, 174.0f / 255.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Modelo del Dedo 3B (Falange intermedia)
		model = glm::translate(modelTemp, glm::vec3(0.5f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(Dedo3_B), glm::vec3(0.0f, 0.0f, 1.0f));
		modelTemp = model = glm::translate(model, glm::vec3(0.375f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.75f, 0.3f, 0.25f));
		color = glm::vec3(255.0f / 255.0f, 0.0f / 255.0f, 64.0f / 255.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Modelo del Dedo 3C (Punta del dedo)
		model = glm::translate(modelTemp, glm::vec3(0.375f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(Dedo3_C), glm::vec3(0.0f, 0.0f, 1.0f));
		modelTemp = model = glm::translate(model, glm::vec3(0.25f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.3f, 0.25f));
		color = glm::vec3(0.0f / 255.0f, 255.0f / 255.0f, 191.0f / 255.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);



		// Modelo del Dedo 4A (Base del dedo)
		model = glm::translate(modelTemp2, glm::vec3(0.25f, 0.35f, -0.75f)); // Posiciona la base del dedo en la palma
		model = glm::rotate(model, glm::radians(Dedo4_A), glm::vec3(0.0f, 0.0f, 1.0f)); // Aplica rotación en el eje Z
		modelTemp = model = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f)); // Establece la referencia para la siguiente falange
		model = glm::scale(model, glm::vec3(1.0f, 0.3f, 0.25f)); // Ajusta el tamaño del segmento
		color = glm::vec3(45.0f / 255.0f, 126.0f / 255.0f, 174.0f / 255.0f); // Color del segmento
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36); // Envía los datos a los shaders

		// Modelo del Dedo 4B (Falange intermedia)
		model = glm::translate(modelTemp, glm::vec3(0.5f, 0.0f, 0.0f)); // Ajusta la posición para la articulación intermedia
		model = glm::rotate(model, glm::radians(Dedo4_B), glm::vec3(0.0f, 0.0f, 1.0f)); // Aplica rotación en el eje Z
		modelTemp = model = glm::translate(model, glm::vec3(0.375f, 0.0f, 0.0f)); // Define el pivote para la siguiente falange
		model = glm::scale(model, glm::vec3(0.75f, 0.3f, 0.25f)); // Ajusta el tamaño del segmento
		color = glm::vec3(255.0f / 255.0f, 0.0f / 255.0f, 64.0f / 255.0f); // Color del segmento
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36); // Envía los datos a los shaders

		// Modelo del Dedo 4C (Punta del dedo)
		model = glm::translate(modelTemp, glm::vec3(0.375f, 0.0f, 0.0f)); // Posiciona la última falange del dedo
		model = glm::rotate(model, glm::radians(Dedo4_C), glm::vec3(0.0f, 0.0f, 1.0f)); // Aplica rotación en el eje Z
		modelTemp = model = glm::translate(model, glm::vec3(0.25f, 0.0f, 0.0f)); // Ajusta la posición final
		model = glm::scale(model, glm::vec3(0.5f, 0.3f, 0.25f)); // Escala el segmento
		color = glm::vec3(0.0f / 255.0f, 255.0f / 255.0f, 191.0f / 255.0f); // Color del segmento
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36); // Envía los datos a los shaders




		// Modelo del Dedo 5A (Base del dedo)
		model = glm::translate(modelTemp2, glm::vec3(0.25f, -0.05f, -1.0f)); // Posiciona la base del dedo en la palma
		model = glm::rotate(model, glm::radians(Dedo5_A), glm::vec3(0.0f, 0.0f, 1.0f)); // Aplica rotación en el eje Z
		modelTemp = model = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 0.3f, 0.25f));
		color = glm::vec3(0.0f / 255.0f, 255.0f / 255.0f, 51.0f / 255.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Modelo del Dedo 5B (Falange intermedia)
		model = glm::translate(modelTemp, glm::vec3(0.5f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(Dedo5_B), glm::vec3(0.0f, 1.0f, 0.0f));
		modelTemp = model = glm::translate(model, glm::vec3(0.375f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.75f, 0.3f, 0.25f));
		color = glm::vec3(231.0f / 255.0f, 25.0f / 255.0f, 255.0f / 255.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
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

void Inputs(GLFWwindow* window) {
	//Cerrar la ventana con ESC
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	//Movimiento de la cámara
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) movX += 0.002f;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) movX -= 0.002f;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) movY += 0.002f;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) movY -= 0.002f;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) movZ -= 0.002f;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) movZ += 0.002f;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) rot += 0.02f;
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) rot -= 0.02f;

	//Movimiento del Brazo
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS && hombro < 90.0f) hombro += 0.07f;
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS && hombro > -90.0f) hombro -= 0.07f;

	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS && codo < 90.0f) codo += 0.07f;
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS && codo > 0.0f) codo -= 0.07f;

	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS && muneca < 20.0f) muneca += 0.07f;
	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS && muneca > -20.0f) muneca -= 0.07f;

	//Movimiento de los dedos (Base, Intermedio, Punta)
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS && Dedo1_A < 0.0f)
		Dedo1_A = Dedo2_A = Dedo3_A = Dedo4_A = Dedo5_A += 0.07f;
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS && Dedo1_A > -30.0f)
		Dedo1_A = Dedo2_A = Dedo3_A = Dedo4_A = Dedo5_A -= 0.07f;

	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS && Dedo1_B < 0.0f)
		Dedo1_B = Dedo2_B = Dedo3_B = Dedo4_B = Dedo5_B += 0.07f;
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS && Dedo1_B > -90.0f)
		Dedo1_B = Dedo2_B = Dedo3_B = Dedo4_B = Dedo5_B -= 0.07f;

	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS && Dedo1_C < 0.0f)
		Dedo1_C = Dedo2_C = Dedo3_C = Dedo4_C += 0.07f;
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS && Dedo1_C > -115.0f)
		Dedo1_C = Dedo2_C = Dedo3_C = Dedo4_C -= 0.07f;
}



