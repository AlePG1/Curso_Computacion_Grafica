#include<iostream>
// Alejandro García González
// 422066992
// Práctica 3
// Fecha de entrega: 20 de febrero del 2025
//#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Shaders
#include "Shader.h"

const GLint WIDTH = 800, HEIGHT = 600;

int main() {
    glfwInit();
    //Verificación de compatibilidad 
    // Set all the required options for GLFW
    /*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);*/

    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Práctica 3 García González Alejadro", nullptr, nullptr);

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

    //GLfloat vertices[] = {
    //      -0.5f*500, -0.5f, 0.5f, 1.0f, 0.0f,0.0f,//Front
    //      0.5f * 500, -0.5f * 500, 0.5f * 500,  1.0f, 0.0f,0.0f,
    //      0.5f * 500,  0.5f * 500, 0.5f * 500,  1.0f, 0.0f,0.0f,
    //      0.5f * 500,  0.5f * 500, 0.5f * 500,  1.0f, 0.0f,0.0f,
    //      -0.5f * 500,  0.5f * 500, 0.5f * 500, 1.0f, 0.0f,0.0f,
    //      -0.5f * 500, -0.5f * 500, 0.5f * 500, 1.0f, 0.0f,0.0f,
    //      
    //    -0.5f * 500, -0.5f * 500,-0.5f * 500, 0.0f, 1.0f,0.0f,//Back
    //      0.5f * 500, -0.5f * 500,-0.5f * 500, 0.0f, 1.0f,0.0f,
    //      0.5f * 500,  0.5f * 500,-0.5f * 500, 0.0f, 1.0f,0.0f,
    //      0.5f * 500,  0.5f * 500,-0.5f * 500, 0.0f, 1.0f,0.0f,
    //      -0.5f * 500,  0.5f * 500,-0.5f * 500, 0.0f, 1.0f,0.0f,
    //      -0.5f * 500, -0.5f * 500,-0.5f * 500, 0.0f, 1.0f,0.0f,
    //};

     //use with Perspective Projection
    float vertices[] = {
        -0.5f, -0.5f, 0.5f, 1.0f, 0.0f,0.0f,//Front
        0.5f, -0.5f, 0.5f,  1.0f, 0.0f,0.0f,
        0.5f,  0.5f, 0.5f,  1.0f, 0.0f,0.0f,
        0.5f,  0.5f, 0.5f,  1.0f, 0.0f,0.0f,
        -0.5f,  0.5f, 0.5f, 1.0f, 0.0f,0.0f,
        -0.5f, -0.5f, 0.5f, 1.0f, 0.0f,0.0f,

        -0.5f, -0.5f,-0.5f, 0.0f, 1.0f,0.0f,//Back
         0.5f, -0.5f,-0.5f, 0.0f, 1.0f,0.0f,
         0.5f,  0.5f,-0.5f, 0.0f, 1.0f,0.0f,
         0.5f,  0.5f,-0.5f, 0.0f, 1.0f,0.0f,
        -0.5f,  0.5f,-0.5f, 0.0f, 1.0f,0.0f,
        -0.5f, -0.5f,-0.5f, 0.0f, 1.0f,0.0f,

         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 0.0f,1.0f,
         0.5f,  0.5f, -0.5f,  0.0f, 0.0f,1.0f,
         0.5f,  0.5f, -0.5f,  0.0f, 0.0f,1.0f,
         0.5f,  0.5f,  0.5f,  0.0f, 0.0f,1.0f,
         0.5f,  -0.5f, 0.5f, 0.0f, 0.0f,1.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 1.0f,0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,0.0f,
        -0.5f, -0.5f, -0.5f,  1.0f, 1.0f,0.0f,
        -0.5f, -0.5f, -0.5f,  1.0f, 1.0f,0.0f,
        -0.5f, -0.5f,  0.5f,  1.0f, 1.0f,0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 1.0f,0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 1.0f,1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 1.0f,1.0f,
        -0.5f, -0.5f,  0.5f, 0.0f, 1.0f,1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,1.0f,

        -0.5f,  0.5f, -0.5f, 1.0f, 0.2f,0.5f,
        0.5f,  0.5f, -0.5f,  1.0f, 0.2f,0.5f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.2f,0.5f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.2f,0.5f,
        -0.5f,  0.5f,  0.5f, 1.0f, 0.2f,0.5f,
        -0.5f,  0.5f, -0.5f, 1.0f, 0.2f,0.5f,
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
    // 3.Copiamos nuestro arreglo de indices en  un elemento del buffer para que OpenGL lo use
    /*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);*/

    // 4. Despues colocamos las caracteristicas de los vertices

    //Posicion
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    //Color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);


    glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)


    glm::mat4 projection = glm::mat4(1);

    projection = glm::perspective(45.0f, (GLfloat)screenWidth / (GLfloat)screenHeight, 0.1f, 100.0f);//FOV, Radio de aspecto,znear,zfar
    //projection = glm::ortho(0.0f, (GLfloat)screenWidth, 0.0f, (GLfloat)screenHeight, 0.1f, 1000.0f);//Izq,Der,Fondo,Alto,Cercania,Lejania
    while (!glfwWindowShouldClose(window))
    {
        // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();

        // Render
        // Clear the colorbuffer
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        // Draw our first triangle
        ourShader.Use();
        glm::mat4 model = glm::mat4(1);
        glm::mat4 view = glm::mat4(1);

        // Se establece la matriz view común para ambos cubos:

        view = glm::translate(view, glm::vec3(-2.0f, -2.0f, -12.0f));
        // Primer cubo: se rota y escala, sin traslación adicional en model.
        model = glm::rotate(model, 5.5f, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));

        // view = glm::translate(view, glm::vec3(screenWidth / 2, screenHeight / 2, -700.0f));

        GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
        GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
        GLint projecLoc = glGetUniformLocation(ourShader.Program, "projection");

        glUniformMatrix4fv(projecLoc, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-2.5f, 0.0f, 0.0f));
        model = glm::rotate(model, 2.5f, glm::vec3(-2.0f, 0.0f, 0.2f));
        model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);


        glm::mat4 model3 = glm::mat4(1);
        model3 = glm::translate(model3, glm::vec3(2.5f, 0.0f, 0.0f));
        model3 = glm::rotate(model3, 2.5f, glm::vec3(-1.0f, 0.0f, 0.1f));
        model3 = glm::scale(model3, glm::vec3(2.0f, 2.0f, 2.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model3));
        glDrawArrays(GL_TRIANGLES, 0, 36);


        glm::mat4 model4 = glm::mat4(1);
        model4 = glm::translate(model4, glm::vec3(5.0f, 0.0f, 0.0f));
        model4 = glm::rotate(model4, 5.5f, glm::vec3(-0.9f, 0.0f, -0.1f));
        model4 = glm::scale(model4, glm::vec3(2.0f, 2.0f, 2.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model4));
        glDrawArrays(GL_TRIANGLES, 0, 36);


        // Segundo nivel
        glm::mat4 model5 = glm::mat4(1);
        model5 = glm::translate(model5, glm::vec3(3.7f, 2.0f, 1.0f));
        model5 = glm::rotate(model5, 7.5f, glm::vec3(-2.5f, -2.0f, -3.5f));
        model5 = glm::scale(model5, glm::vec3(2.0f, 2.0f, 2.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model5));
        glDrawArrays(GL_TRIANGLES, 0, 36);


        glm::mat4 model6 = glm::mat4(1);
        model6 = glm::translate(model6, glm::vec3(1.2f, 2.0f, 1.0f));
        model6 = glm::rotate(model6, 2.5f, glm::vec3(1.0f, 0.0f, 0.0f));
        model6 = glm::scale(model6, glm::vec3(2.0f, 2.0f, 2.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model6));
        glDrawArrays(GL_TRIANGLES, 0, 36);


        glm::mat4 model7 = glm::mat4(1);
        model7 = glm::translate(model7, glm::vec3(-1.4f, 2.0f, 1.0f));
        model7 = glm::rotate(model7, -5.5f, glm::vec3(1.0f, -0.5f, -0.3f));
        model7 = glm::scale(model7, glm::vec3(2.0f, 2.0f, 2.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model7));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Tercer nivel
        glm::mat4 model8 = glm::mat4(1);
        model8 = glm::translate(model8, glm::vec3(-0.1f, 4.0f, 2.0f));
        model8 = glm::rotate(model8, 9.5f, glm::vec3(-1.6f, -9.9f, 6.5f));
        model8 = glm::scale(model8, glm::vec3(2.0f, 2.0f, 2.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model8));
        glDrawArrays(GL_TRIANGLES, 0, 36);


        glm::mat4 model9 = glm::mat4(1);
        model9 = glm::translate(model9, glm::vec3(2.5f, 4.0f, 2.0f));
        model9 = glm::rotate(model9, 2.5f, glm::vec3(1.0f, 0.0f, 0.0f));
        model9 = glm::scale(model9, glm::vec3(2.0f, 2.0f, 2.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model9));
        glDrawArrays(GL_TRIANGLES, 0, 36);


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
