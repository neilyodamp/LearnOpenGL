#include <iostream>
#include <ostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)
	{
		// set window should close.
		glfwSetWindowShouldClose(window, true);
	}
}

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";


const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"uniform vec4 ourColor;\n"
"void main()\n"
"{\n"
"   FragColor = ourColor;\n"
"}\0";

int main()
{
	// glfw init.
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// create GLFW window.
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);

	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window " << std::endl;
	}

	glfwMakeContextCurrent(window);

	// set glfwGetProcAddress function is load function.
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	// 
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);



	const unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
	glCompileShader(vertexShader);

	// Log vertex shader sources compiler info.
	{
		int successs;
		char infoLog[512];

		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &successs);

		if (!successs)
		{
			glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
	}

	const unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
	glCompileShader(fragmentShader);

	// Log fragment shader sources compiler info.
	{
		int successs;
		char infoLog[512];

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &successs);

		if (!successs)
		{
			glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
	}

	const unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// Log  shader program link info.
	{
		int successs;
		char infoLog[512];

		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &successs);

		if (!successs)
		{
			glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
		}
	}

	// set use program.
	glUseProgram(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	constexpr float vertices[] = {
		-0.5f,	-0.5f,	0.0f,
		0.5f,	-0.5f,	0.0f,
		0.0f,	0.5f,	0.0f,
	};

	// copy buffer to  gpu memory.
	unsigned int VBO;
	glGenBuffers(1, &VBO);

	// binds a `buffer object` to the specified `buffer binding point`.
	// If no `buffer object` with name buffer exists, one is created with that name.
	// when a `buffer object` is bound to a target, the previous binding for that target is automatically broken.
	// The state of a `buffer object` immediately after it is first bound is an `unmapped zero-sized memory buffer` with GL_READ_WRITE access and GL_STATIC_DRAW usage.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// will create new buffer data, pre data will delete
	// STATIC , STREAM , DYNAMIC.
	// DRAW , READ , COPY.
	// In its initial state, the new data store is not mapped, it has a NULL mapped pointer, and its mapped access is GL_READ_WRITE.
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// VAO bind must before enable vertex attribute array . 
	// OpenGL core must use VAO
	//
	unsigned int VAO;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast<void*>(0));
	glEnableVertexAttribArray(0);

	// un bind buffer.
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// window should close
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		//--- write render code.


		// set color to clear color buffer.
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// draw buffer is not can use.
		// glDrawBuffer(VBO);

		//glBindVertexArray(VAO);

		float timeValue = glfwGetTime();
		float greenValue = sin(timeValue) / 2.0f + 0.5f;
		int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
		glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		//--- end
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glBindVertexArray(0);

	// delete it.
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

	glfwTerminate();
	return 0;
}
