#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

static unsigned int CompileShader( unsigned int type , const std::string& source)
{
     unsigned int id = glCreateShader(type);
     const char* scr = source.c_str();
     glShaderSource(id,1,&scr,nullptr);
     glCompileShader(id);

     int result;

     glGetShaderiv(id, GL_COMPILE_STATUS, &result);
     if (result == GL_FALSE)
     {
         int length;
         glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
         char* message = (char*)alloca(length * sizeof(char));
         glGetShaderInfoLog(id, length, &length, message);
         std::cout << "faild to compile "<<(type == GL_VERTEX_SHADER ? "vertex": "fragment") << std::endl;
         std::cout << message << std::endl;
         glDeleteShader(id);
         return 0;
     }

     return id;
}
static int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
     unsigned int program = glCreateProgram();
     unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
     unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

     glAttachShader(program, vs);
     glAttachShader(program, fs);
     glLinkProgram(program);
     glValidateProgram(program);
     
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

    float positions[6] = {
       -0.5f,-0.5f,
        0.0f, 0.5f,
        0.5f,-0.5f
    };
    //Initialize buffer.  
    unsigned int buffer;
    glGenBuffers(1, &buffer);
    //select buffer
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    //set buffer data
    glBufferData(GL_ARRAY_BUFFER,6 *sizeof(float),positions, GL_STREAM_DRAW);

    //glEnableVertexArrayAttrib(0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

    //write Simple shader 

    //vertex shader function
    std::string vertexShader =
        "#version 330 core\n"
        "\n"
        "layout(location = 0) in vec4 position;\n"
        "\n"
        "void main()\n"
        "{\n"
        "gl_position = position;\n"
        "}\n";
    //fragment shader function
    std::string fragmentShader =
        "#version 330 core\n"
        "\n"
        "layout(location = 0) out vec4 color;\n"
        "\n"
        "void main()\n"
        "{\n"
        "color = vec4(1.0,0.0,0.0,1.0);\n"
        "}\n";

    unsigned int shader = CreateShader(vertexShader,fragmentShader);

    glUseProgram(shader);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

     
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(Win))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0,3); 
         

       /* glBegin(GL_TRIANGLES);
        glVertex2f(-0.5f,-0.5f);
        glVertex2f(0.0f, 0.5f);
        glVertex2f(0.5f,-0.5f);
        glEnd();*/

        /* Swap front and back buffers */
        glfwSwapBuffers(Win);

        /* Poll for and process events */
        glfwPollEvents();
    }
    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}