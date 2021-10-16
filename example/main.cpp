#include <iostream>
#include <cassert>
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <gltxt/gltxt.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

int main(int, char**) {
    int res = glfwInit();    

    GLFWwindow* win = glfwCreateWindow(1000, 1000, "GLTXT", NULL, NULL);    

    glfwMakeContextCurrent(win);

    res = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glViewport(0, 0, 1000, 1000);
    
    gltxt::Init();
    gltxt::LoadFontFromFile("default", "./resources/CasatCapMed_PERSONAL_USE.ttf", 32);

    gltxt::Label label = gltxt::CreateLabelFromFont("Hello from GLTXT", "default");  
    float fontX = 0, fontY = 0;
    label.GetSizePixels(&fontX, &fontY);  

    glm::mat4 proj = glm::perspective(glm::radians(45.), 1., 0.03, 1000.);
    glm::mat4 view = glm::lookAt(glm::vec3(0., 0., -5.), glm::vec3(0.), glm::vec3(0., -1., 0.));

    glm::mat4 model = glm::mat4(1.f);
    model = glm::scale(model, glm::vec3(.007f, .007f, .007f));
    model = glm::translate(model, glm::vec3(fontX / -2.f, fontY / -2.f, 0)); // Move to the center of the screen (0, 0)

    while(!glfwWindowShouldClose(win))
    {
        
        glClearColor(.22f, .35f, 1.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        gltxt::PrepareRender(&(proj * view)[0][0]);
        
        label.SetModel(&model[0][0]);
        label.Render();
        
        glfwSwapBuffers(win);
        glfwPollEvents();
    }
    gltxt::Shutdown();
    glfwTerminate();
    return 0;
}
