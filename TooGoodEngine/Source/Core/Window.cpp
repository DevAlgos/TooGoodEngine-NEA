#include "Window.h"

#include "Application.h"


namespace TooGoodEngine {

	Window::Window(uint32_t width, uint32_t height, const std::string& title, EventDispatcher<Application>& dispatcher)
		: m_Width(width), m_Height(height), m_Title(title), m_Window(nullptr), m_Dispatcher(dispatcher)
	{
		//if glfw fails to init this is a critical error so we halt.
		if (!glfwInit()) 
			TGE_HALT();

		//set some hints to glfw as to which opengl version we want to use.
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

		m_Window = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), nullptr, nullptr);

		glfwSetWindowUserPointer(m_Window, this);

		//window resize callback
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) 
			{
				Window* myWindow = (Window*)glfwGetWindowUserPointer(window);

				myWindow->SetWidth((uint32_t)width);
				myWindow->SetHeight((uint32_t)height);

				WindowResizeEvent event((uint32_t)width, (uint32_t)height);
				myWindow->GetDispatcher().Dispatch(&event);
			});
		
		//window close callback
		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) 
			{
				Window* myWindow = (Window*)glfwGetWindowUserPointer(window);
				
				ApplicationCloseEvent closeEvent;
				myWindow->GetDispatcher().Dispatch(&closeEvent);
			});

		glfwMakeContextCurrent(m_Window);
		
		//if glad fails to init this is a critical error so we halt.

		if (!gladLoadGL()) 
			TGE_HALT();

		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(Window::OpenGLDebugCallback, nullptr);
	}

	Window::~Window()
	{
		if (m_Window)
		{
			glfwDestroyWindow(m_Window);
			glfwTerminate();
		}
	}

	void Window::Update()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_Window);
	}

	void Window::OpenGLDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
	{
		switch (severity)
		{
			case GL_DEBUG_SEVERITY_LOW:	   TGE_LOG_INFO(message);	   break;
			case GL_DEBUG_SEVERITY_MEDIUM: TGE_LOG_WARNING(message);   break;
			case GL_DEBUG_SEVERITY_HIGH:   TGE_LOG_ERROR(message);     break;
			default:  break;
		}
	}
}