#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "core/Application.h"
#include "core/Scene.h"
#include "core/InputListener.h"
#include "audio/AudioMaster.h"
#include "renderer/Renderer.h"
#include "renderer/Camera.h"
#include "renderer/rData.h"
#include "renderer/Font.h"
#include "renderer/Texture.h"
#include "library/StateMachine.h"
#include "renderer/TextRenderer.h"
#include "renderer/SpriteRenderer.h"
#include "ecs/Transform.h"

// TODO: there really should be a single struct `Application` that holds a lot of the static stuff in this engine.
namespace Pontilus
{
    Scene defaultScene = 
    {
        []()
        {
            static Renderer::IconMap defaultMap = Renderer::IconMap("../assets/textures/ghostSwole.png", 675, 570, 0);
            static Renderer::Font jetBrainsMono = Renderer::Font("../assets/fonts/JetBrainsMono-Medium.ttf", 26);
            ECS::GameObject &defaultMessage = defaultScene.spawn();
            defaultMessage.addComponent(new ECS::Transform{{0.0, 0.0, 0.0}, {40.0, 50.0, 1.0}, {0.0, 0.0, 0.0}});
            ECS::GameObject &defaultLogo = defaultScene.spawn();
            defaultLogo.addComponent(new ECS::Transform{{0.0, -20.0, 0.0}, {20.0, 16.0, 1.0}, {0.0, 0.0, 0.0}});

            Renderer::Texture t = defaultMap.get(0);

            defaultMessage.addComponent(new Renderer::TextRenderer(
                "Whoops! The Scene hasn't been specified yet. Make sure to call pontilus.set_scene(s) before pontilus.loop().",
                jetBrainsMono, {1.0f, 1.0f, 1.0f, 1.0f}));
            defaultLogo.addComponent(new Renderer::SpriteRenderer(t, {1.0f, 1.0f, 1.0f, 1.0f}));
        },
        [](double dt)
        {

        },
        []()
        {

        }
    };

    GLuint glProgramID;
    
    static void printError(int error, const char *description)
    {
        fputs(description, stderr);
        fputs("\n", stderr);
    }

    size_t Window::_id = 0;
    
    Window::Window() {
        this->id = _id++;
        this->width = 800; // default width
        this->height = 600; // default height
        this->title = std::string("Unnamed Window");

        this->ptr = glfwCreateWindow(this->width, this->height, this->title.c_str(), NULL, NULL);
        __pAssert(this->ptr, "Could not initialize GLFW window.");
        
        glfwMakeContextCurrent(this->ptr);

        // v-sync
        glfwSwapInterval(1);

        // setup callbacks
        glfwSetWindowSizeCallback(this->ptr, 
            [](GLFWwindow *r, int newWidth, int newHeight)
            {
                Window &w = Application::get()->getWindowByGLFW(r);
                w.setWidth(newWidth);
                w.setHeight(newHeight);
                glViewport(0, 0, w.getWidth(), w.getHeight());
            });
        
        glfwSetCursorPosCallback(this->ptr, IO::mousePosCallback);
        glfwSetScrollCallback(this->ptr, IO::mouseScrollCallback);
        glfwSetMouseButtonCallback(this->ptr, IO::mouseButtonCallback);

        glfwSetKeyCallback(this->ptr, IO::keyPressedCallback);

        // make the window visible
        glfwShowWindow(this->ptr);

        this->scene = nullptr;
        this->isClosing = false;

        this->renderTargetIDs.push_back(Renderer::RendererController::QUAD_TARGET);
        this->renderTargetIDs.push_back(Renderer::RendererController::MESH_TARGET);
        this->renderTargetIDs.push_back(Renderer::RendererController::LINE_TARGET);
        this->renderTargetIDs.push_back(Renderer::RendererController::FULL_WINDOW_TARGET);
    }

    void Window::setTitle(std::string &t) {
        this->title = std::string(t);

        glfwSetWindowTitle(this->ptr, this->title.c_str());
    }

    void Window::update(double dt) {
        if (!glfwWindowShouldClose(this->ptr)) {
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

            // set default background
            glClear(GL_COLOR_BUFFER_BIT);

            // poll events
            glfwPollEvents();

            // update engines
            //TODO: fixed update for updateAll() and getCurrentScene()->update()
            this->scene->updateObjects(dt);
            this->scene->update(dt);
            // Physics2D::fixedUpdate();

            // render
            for (size_t rID : this->renderTargetIDs)
                Renderer::RendererController::get().render(rID, this->camera);

            // swap buffers (makes things smoother)
            glfwSwapBuffers(this->ptr);
        } else {
            this->isClosing = true;
        }
    }

    Window::~Window() {
        if (this->scene) {
            this->scene->clean();
            this->scene->freeObjects();
            this->scene->setUsed(false);
        }

        glfwDestroyWindow(this->ptr);
    }

    void Window::setScene(Scene &s) {
        if (this->scene != nullptr)
        {
            this->scene->clean();
            this->scene->freeObjects();
            this->scene->setUsed(false);
        }

        if (!s.isUsed()) {
            this->scene = &s;
            __pAssert(this->scene->init, "New scene doesn't have init function set.");
            __pAssert(this->scene->update, "New scene doesn't have update function set.")
            __pAssert(this->scene->clean, "New scene doesn't have clean function set.")
            this->scene->init();
            this->scene->setUsed(true);
            return;
        }

        __pError("Cannot set scene to window of ID %lu; scene is already used!", this->id);
    }

    Application *Application::inst = nullptr;

    Application *Application::get() {
        if (!inst) {
            inst = new Application();
            inst->init();
        }

        return inst;
    }

    Application::Application() {
    }

    Window &Application::getWindow(size_t id) {
        for (auto window : this->windows) {
            if (window->getID() == id) {
                return *window;
            }
        }

        __pError("Cannot find window of id %lu in application.", id);
    }

    Window &Application::getWindowByGLFW(GLFWwindow *ptr) {
        for (auto window : this->windows) {
            if (ptr == window->getGLFW()) {
                return *window;
            }
        }

        __pError("Cannot get window of GLFW pointer %p; is there a dangling GLFW context??", ptr);
    }

    void Application::init()
    {
        glfwSetErrorCallback(printError);

        __pAssert(glfwInit(), "Could not initialize GLFW instance");

        // GLFW
        glfwDefaultWindowHints();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
        glfwWindowHint(GLFW_MAXIMIZED, GLFW_FALSE);
        
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
        
        // Initialize the window
        
        this->windows.push_back(new Window());

        Window *primary = this->windows[0];

        // initialize opengl backend
        if (!gladLoadGL((GLADloadfunc) glfwGetProcAddress))
        {
            printf("Failed to initialize OpenGL context\n");
            return;
        }

        glViewport(0, 0, primary->getWidth(), primary->getHeight());
        // transparency stuff
        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

        Renderer::RendererController::get().start();
        Audio::initAudio();

        primary->setScene(defaultScene);

        // say hi
        __pMessage("Hello: %s", glGetString(GL_VERSION));
    }

    void Application::run()
    {
        // might use these sometime
        double t1;
        double t2;
        double highestdt = 0.016f;
        double lowestdt = 0.016f;
        while (!this->windows.empty())
        {
            t1 = glfwGetTime();

            for (auto window : this->windows) {
                window->update(dt);
            }

            for (size_t i = 0; i < this->windows.size(); i++) {
                Window *w = this->windows[i];
                if (w->closing()) {
                    delete w;
                    this->windows.erase(this->windows.begin() + i);
                    i--;
                }
            }

            // framerate calculations
            t2 = glfwGetTime();
            this->dt = t2 - t1;
            highestdt = highestdt > dt ? highestdt : dt;
            lowestdt = lowestdt < dt ? lowestdt : dt;

            IO::endFrame();
        }
            
        Renderer::RendererController::get().close();
        Audio::closeAudio();

        glLinkProgram(0);
        glfwTerminate();
    }
}

