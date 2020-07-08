//============================================================================
// Name        : Dibujo de Triangulo por puntos ingresados en Codigo
// Professor   : Herminio Paucar
// Version     :
// Description : Utilizamos los Vertex y Fragment Shaders
//============================================================================
// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
// Include GLEW
#define GLEW_STATIC
#include<glm/glm.hpp>
#include <GL/glew.h>
#include <glm/vec3.hpp>
// Include GLFW
#include <GLFW/glfw3.h>
#include <string>
#include <fstream>
GLuint renderingProgram;
GLfloat* m_Vertices;
GLuint m_VBO;
GLuint m_VAO;

using namespace std;

// displays the contents of OpenGL's log when GLSL compilation failed
void printShaderLog(GLuint shader) {
    int len = 0;
    int chWrittn = 0;
    char* log;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
    if (len > 0) {
        log = (char*)malloc(len);
        glGetShaderInfoLog(shader, len, &chWrittn, log);
        cout << "Shader Info Log: " << log << endl;
        free(log);
    }
}

// displays the contents of OpenGL's log when GLSL linking failed
void printProgramLog(int prog) {
    int len = 0;
    int chWrittn = 0;
    char* log;
    glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &len);
    if (len > 0) {
        log = (char*)malloc(len);
        glGetProgramInfoLog(prog, len, &chWrittn, log);
        cout << "Program Info Log: " << log << endl;
        free(log);
    }
}

// checks the OpenGL error flag for the occurrrence of an OpenGL error
// detects both GLSL compilation errors and OpenGL runtime errors
bool checkOpenGLError() {
    bool foundError = false;
    int glErr = glGetError();
    while (glErr != GL_NO_ERROR) {
        cout << "glError: " << glErr << endl;
        foundError = true;
        glErr = glGetError();
    }
    return foundError;
}

string readShaderSource(const char *filePath) {
    string content = "";
    ifstream fileStream(filePath, ios::in);
//    cerr << "Error: " << strerror(errno) << endl;  // No such file or directory
//    cout << fileStream.is_open() << endl;  // 0
    string line = "";
    while (!fileStream.eof()) {
        getline(fileStream, line);
        content.append(line + "\n");
    }
    fileStream.close();
    return content;
}

GLuint createShaderProgram() {
    GLint vertCompiled;
    GLint fragCompiled;
    GLint linked;

    string vertShaderStr = readShaderSource("src/vertShader.glsl");
    string fragShaderStr = readShaderSource("src/fragShader.glsl");

    const char* vertShaderSrc = vertShaderStr.c_str();
    const char* fragShaderSrc = fragShaderStr.c_str();

    GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vShader, 1, &vertShaderSrc, NULL);
    glShaderSource(fShader, 1, &fragShaderSrc, NULL);

    glCompileShader(vShader);
    checkOpenGLError();
    glGetShaderiv(vShader, GL_COMPILE_STATUS, &vertCompiled);
    if (vertCompiled != 1) {
        cout << "vertex compilation failed" << endl;
        printShaderLog(vShader);
    }

    glCompileShader(fShader);
    checkOpenGLError();
    glGetShaderiv(fShader, GL_COMPILE_STATUS, &fragCompiled);
    if (fragCompiled != 1) {
        cout << "fragment compilation failed" << endl;
        printShaderLog(fShader);
    }

    GLuint vfProgram = glCreateProgram();
    glAttachShader(vfProgram, vShader);
    glAttachShader(vfProgram, fShader);

    glLinkProgram(vfProgram);
    checkOpenGLError();
    glGetProgramiv(vfProgram, GL_LINK_STATUS, &linked);
    if (linked != 1) {
        cout << "linking failed" << endl;
        printProgramLog(vfProgram);
    }

    return vfProgram;
}


void init (GLFWwindow* window) {
    renderingProgram = createShaderProgram();
    // The first 3 points are to Vertex position of Triangle
    // The other 3 points are to Vertex color
    glm::vec3 h0(0.4,0.8,0.5);
    glm::vec3 h1 = h0+glm::vec3(1.0*-0.3,1.0*-0.6,1.0*-0.0);//(1,2,5)
    glm::vec3 h2 = h0+glm::vec3(1.0*+0.3,1.0*-0.4,1.0*-0.0);//(7,4,5)

    glm::vec3 c0 (1.0,1.0,0.0);
    glm::vec3 c01 (1.0,1.0,0.0);
    glm::vec3 c02 (1.0,1.0,0.0);

    glm::vec3 a0(-0.7,0.8,0.5);
    glm::vec3 a1 = a0+glm::vec3(1.0*-0.0,1.0*-0.8,1.0*-0.0);//(-7,0,5)
    glm::vec3 a2 = a0+glm::vec3(1.0*+0.6,1.0*-0.8,1.0*-0.0);//(-1,0,5)

    glm::vec3 c1 (0.0,1.0,0.0);
    glm::vec3 c11 (0.0,1.0,0.0);
    glm::vec3 c12 (0.0,1.0,0.0);

    glm::vec3 b0(-0.5,-0.2,0.5);
    glm::vec3 b1 = b0+glm::vec3(1.0*-0.3,1.0*-0.5,1.0*-0.0);//(-8,-7,5)
    glm::vec3 b2 = b0+glm::vec3(1.0*+0.3,1.0*-0.5,1.0*-0.0);//(-2,-7,5)

    glm::vec3 c2 (1.0,0.0,0.0);
    glm::vec3 c21 (1.0,0.0,0.0);
    glm::vec3 c22 (1.0,0.0,0.0);

    glm::vec3 d0(0.3,-0.2,0.5);
    glm::vec3 d1 = d0+glm::vec3(1.0*-0.0,1.0*-0.4,1.0*-0.0);//(3,-6,5)
    glm::vec3 d2 = d0+glm::vec3(1.0*+0.6,1.0*-0.4,1.0*-0.0);//(9,-6,5)
    glm::vec3 d3 = d0+glm::vec3(1.0*+0.6,1.0*-0.0,1.0*-0.0);//(9,-2,5)

    glm::vec3 m3 (0.0,0.0,1.0);
    glm::vec3 m31 (0.0,0.0,1.0);
    glm::vec3 m32 (0.0,0.0,1.0);
    glm::vec3 m33 (0.0,0.0,1.0);

    glm::vec3 m_Vertices[36]={
    		h0,c0,	h1,c01,	h2,c02,
			a0,c1,	a1,c11,	a2,c12,
			b0,c2,	b1,c21,	b2,c22,
			d0,m3,d1,m31,d2,m32,d3,m33,
    };

	// Cria um ID na GPU para nosso buffer
	glGenBuffers(1, &m_VBO);

	// Cria um ID na GPU para um array de  buffers
	glGenVertexArrays(1, &m_VAO);

	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

	// Reserva memoria na GPU para um TARGET receber dados
	// Copia esses dados pra essa área de memoria
	glBufferData(
			GL_ARRAY_BUFFER,	// TARGET associado ao nosso buffer
			78 * sizeof(GLfloat),	// tamanho do buffer
			m_Vertices,			// Dados a serem copiados pra GPU
			GL_STATIC_DRAW		// Política de acesso aos dados, para otimização
		);

	// Nesse ponto, ja copiamos nossos dados pra GPU.
	// Mas, OpenGL nao faz ideia do que eles representam
	// Sao 6 vértices ? 3 vérices e 3 cores ?
	// Os vertices vem antes ou depois das cores ?
	// Ou estão intercalados vértices depois cores, depois vertices, depois cores novamente....
	// Precisamos dizer com nossos dados precisam ser interpretados:
	glVertexAttribPointer(
			0,			// Lembra do (layout = 0 ) no vertex shader ? Esse valor indica qual atributo estamos indicando
			3,			// cada vertice é composto de 3 valores
			GL_FLOAT,	// cada valor do vértice é do tipo GLfloat
			GL_FALSE,	// Quer normalizar os dados e converter tudo pra NDC ? ( no nosso caso, já esta tudo correto, então deixamos como FALSE)
			6 * sizeof(GLfloat),// De quantos em quantos bytes, este atributo é encontrado no buffer ? No nosso caso 3 floats pros vertices + 3 floats pra cor = 6 floats
			(GLvoid*) 0	// Onde está o primeiro valor deste atributo no buffer. Nesse caso, está no início do buffer
		);

	glEnableVertexAttribArray(0);	// Habilita este atributo

	// Faremos a mesma coisa pra cor de cada vértice
	glVertexAttribPointer(
			1,			// Lembra do (layout = 1 ) no vertex shader ? Esse valor indica qual atributo estamos indicando
			3,			// cada vertice é composto de 3 valores
			GL_FLOAT,	// cada valor do vértice é do tipo GLfloat
			GL_FALSE,	// Quer normalizar os dados e converter tudo pra NDC ? ( no nosso caso, já esta tudo correto, então deixamos como FALSE)
			6 * sizeof(GLfloat),// De quantos em quantos bytes, este atributo é encontrado no buffer ? No nosso caso 3 floats pros vertices + 3 floats pra cor = 6 floats
			(GLvoid*) (3 * sizeof(GLfloat))	// Onde está o primeiro valor deste atributo no buffer. Nesse caso, 3 floats após o início do buffer
		);

	glEnableVertexAttribArray(1);		// Habilita este atributo


	glBindVertexArray(0);
}

void display(GLFWwindow* window, double currentTime) {
    glUseProgram(renderingProgram);

	// Use este VAO e suas configurações
	glBindVertexArray(m_VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDrawArrays(GL_TRIANGLES, 3, 3);
    glDrawArrays(GL_TRIANGLES, 6, 3);
    glDrawArrays(GL_TRIANGLE_FAN, 9, 4);
	glBindVertexArray(0);

}

int main(void) {
    if (!glfwInit()) {
    	exit(EXIT_FAILURE);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  //
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            //
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); 	// Resizable option.

    GLFWwindow* window = glfwCreateWindow(800, 800, "Tarea Triangulos by HAROLD GIUSSEPPI ALBERTO RAMOS", NULL, NULL);
    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK) {
    	exit(EXIT_FAILURE);
    }
    glfwSwapInterval(1);

    init(window);

    while (!glfwWindowShouldClose(window)) {
        display(window, glfwGetTime());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
