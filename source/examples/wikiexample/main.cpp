
#include <glm/glm.hpp>

#include <glbinding/gl/gl.h>
#include <glbinding/ContextInfo.h>
#include <glbinding/Version.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <globjects/globjects.h>
#include <globjects/logging.h>

#include <globjects/Buffer.h>
#include <globjects/Program.h>
#include <globjects/Shader.h>
#include <globjects/VertexArray.h>
#include <globjects/VertexAttributeBinding.h>
#include <globjects/base/StaticStringSource.h>


using namespace gl;
using namespace glm;
using namespace globjects;

namespace 
{
    const char * vertexShaderCode = R"(
#version 140
#extension GL_ARB_explicit_attrib_location : require

layout (location = 0) in vec2 corner;

out vec4 color;

void main()
{
    gl_Position = vec4(corner * 2.0 - 1.0, 0.0, 1.0);
    color = vec4(corner, 0.0, 1.0);
}

)";
    const char * fragmentShaderCode = R"(
#version 140
#extension GL_ARB_explicit_attrib_location : require

layout (location = 0) out vec4 fragColor;

in vec4 color;

void main()
{
    fragColor = color;
}

)";
}


void key_callback(GLFWwindow * window, int key, int /*scancode*/, int action, int /*modes*/)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE)
        glfwSetWindowShouldClose(window, true);
}

void draw(Program * program, VertexArray * vao)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    program->use();
    vao->drawArrays(GL_TRIANGLE_STRIP, 0, 4);
}


int main(int /*argc*/, char * /*argv*/[])
{
    // Initialize GLFW with error callback and needed OpenGL version window hint
    glfwInit();
    glfwSetErrorCallback( [] (int /*error*/, const char * description) { puts(description); } );
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    // Create a context and, if valid, make it current
    GLFWwindow * window = glfwCreateWindow(1024, 768, "", NULL, NULL);
    if (window == nullptr)
    {
        critical() << "Context creation failed. Terminate execution.";

        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);

    // Create callback that when user presses ESC, the context should be destroyed and window closed
    glfwSetKeyCallback(window, key_callback);

    // Initialize globjects (internally initializes glbinding, and registers the current context)
    globjects::init();

    // Dump information about context and graphics card
    info() << std::endl
        << "OpenGL Version:  " << glbinding::ContextInfo::version() << std::endl
        << "OpenGL Vendor:   " << glbinding::ContextInfo::vendor() << std::endl
        << "OpenGL Renderer: " << glbinding::ContextInfo::renderer() << std::endl;
    

    glClearColor(0.2f, 0.3f, 0.4f, 1.f);

    {
        // Initialize
        ref_ptr<Buffer> cornerBuffer = new Buffer();
        ref_ptr<Program> program = new Program();
        ref_ptr<VertexArray> vao = new VertexArray();

        program->attach(
            Shader::fromString(GL_VERTEX_SHADER,  vertexShaderCode),
            Shader::fromString(GL_FRAGMENT_SHADER, fragmentShaderCode));

        cornerBuffer->setData(std::array<vec2, 4>{ {
            vec2(0, 0), vec2(1, 0), vec2(0, 1), vec2(1, 1) } }, GL_STATIC_DRAW);

        vao->binding(0)->setAttribute(0);
        vao->binding(0)->setBuffer(cornerBuffer, 0, sizeof(vec2));
        vao->binding(0)->setFormat(2, GL_FLOAT);
        vao->enable(0);


        // Main loop
        while (!glfwWindowShouldClose(window))
        {
            glfwPollEvents();
            draw(program, vao);
            glfwSwapBuffers(window);
        }

    }

    // Properly shutdown GLFW
    glfwTerminate();

    return 0;
}
