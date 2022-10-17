#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

static struct ShaderProgramSource ParseShader(const std::string& filepath)
{

	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::ifstream stream(filepath);
	
	std::string line;

	std::stringstream ss[2];

	ShaderType type = ShaderType::NONE;

	while (getline(stream,line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if(line.find("vertex") != std::string::npos)
			{
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos)
			{
				type = ShaderType::FRAGMENT;
			}
		}
		else
		{
			ss[(int)type] << line << '\n'; 
		}
	}
	return { ss[0].str(),ss[1].str() };
}

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
	unsigned int id = glCreateShader(type);
	const char* scr = source.c_str();
	glShaderSource(id, 1, &scr, nullptr);
	glCompileShader(id);

	//run the driver compiler Error 
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "faild to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;
	}

	return id;
}
//Creating Shader Function
static int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	//Initialize the program.  
	unsigned int program = glCreateProgram();
	//Initialize the vertex and fragment shader.  
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	//attach The fragment and vertex shader to the program.  
	glAttachShader(program, vs);
	glAttachShader(program, fs);

	glLinkProgram(program);
	glValidateProgram(program);
	//Deleting the vertex and the fragment.  
	//And it still passed to the program.  
	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}
int main(void)
{
	GLFWwindow* Win;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	Win = glfwCreateWindow(640, 480, "First App", NULL, NULL);
	if (!Win)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(Win);

	if (glewInit() != GLEW_OK)
		std::cout << "error" << std::endl;

	std::cout << glGetString(GL_VERSION) << std::endl;


	//Initialize the array
	float positions[] = {
	   -0.5f,-0.5f,//0
		0.5f,-0.5f,//1
		0.5f, 0.5f,//2
	   -0.5f, 0.5f,//3
	};
	unsigned int indices[6] = {
		0,1,2,
		2,3,0
	};
	 
	//Initialize and generate buffer.  
	unsigned int buffer;
	glGenBuffers(1, &buffer);
	//select buffer
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	//set buffer data
	glBufferData(GL_ARRAY_BUFFER, 2* 6 * sizeof(float), positions, GL_STREAM_DRAW);
	//Enable thread to the vertex on the GPU.  
	glEnableVertexAttribArray(0);
	//Tell the function that this location of the memory containing. Two by two floats.  From zero.  
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

	unsigned int ibo;
	glGenBuffers(1, &ibo);
	//select buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	//set buffer data
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 2 * 6 * sizeof(unsigned int), indices, GL_STREAM_DRAW);

	//write Simple shader 
	ShaderProgramSource source = ParseShader("Shaders/Basic.shader");
	//Output the shader file. On the console for debugging purposes. 
	std::cout << "Vertex" << std::endl;
	std::cout << source.VertexSource << std::endl;
	std::cout << "Fragment" << std::endl;
	std::cout << source.FragmentSource << std::endl;
	//Assign the vertex and the fragment.  The shader int.  
	unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);

	glUseProgram(shader);

	glBindBuffer(GL_ARRAY_BUFFER, 0);


	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(Win))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

		//glDrawArrays(GL_TRIANGLES, 0, 6);

		/* Swap front and back buffers */
		glfwSwapBuffers(Win);

		/* Poll for and process events */
		glfwPollEvents();
	}
	glDeleteProgram(shader);

	glfwTerminate();
	return 0;
}