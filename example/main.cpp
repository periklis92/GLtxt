#include <iostream>
#include <cassert>
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <gltxt/gltxt.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

int main(int, char**) {
    int res = glfwInit();
    assert(res);
    GLFWwindow* win = glfwCreateWindow(1000, 1000, "GLTXT", NULL, NULL);
    assert(win);
    glfwMakeContextCurrent(win);
    res = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    assert(res);
    gltxt::Init();
    gltxt::Font font = gltxt::Font::LoadFontFromFile("./resources/CasatCapMed_PERSONAL_USE.ttf", 32);
    gltxt::Label label = gltxt::NewLabelFromFont("My name isg Peri", font);
    glViewport(0, 0, 1000, 1000);
    // glm::mat4 proj = glm::perspective(glm::radians(45.), 1., 0.03, 1000.);
    glm::mat4 proj = glm::ortho(-2., 2., 2., -2.);
    // glm::mat4 view = glm::lookAt(glm::vec3(0., 0., -5.), glm::vec3(0.), glm::vec3(0., -1., 0.));
    glm::mat4 view = glm::mat4(1.f);
    glm::mat4 model = glm::mat4(1.f);
    glm::mat4 model2 = glm::mat4(1.f);
    model = glm::scale(model, glm::vec3(.007, .007, .007));
    model = glm::translate(model, glm::vec3(0, 0, 0));
    model2 = glm::scale(model2, glm::vec3(.007, .007, .007));
    model2 = glm::translate(model2, glm::vec3(120., 0., 50));
    model2 = glm::rotate(model2, glm::radians(90.f), glm::vec3(0.f, 1.f, 0.f));
    while(!glfwWindowShouldClose(win))
    {
        // glEnable(GL_DEPTH);
        // glDepthFunc(GL_LESS); 
        glClearColor(.22f, .35f, 1.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glBindTexture(GL_TEXTURE_2D, font.GetTexHandle());
        gltxt::PrepareRender(&(proj * view)[0][0]);
        // view = glm::rotate(view, 0.01f, glm::vec3(0.f, 1.f, 0.f));
        label.SetModel(&model[0][0]);
        label.Render();
        model2 = glm::translate(model2, glm::vec3(0, 0, -0.01));
        label.SetModel(&model2[0][0]);
        label.Render();
        // gltxt::RenderMesh(mesh);
        glfwSwapBuffers(win);
        glfwPollEvents();
    }
    gltxt::Shutdown();
    glfwTerminate();
    return 0;
}
