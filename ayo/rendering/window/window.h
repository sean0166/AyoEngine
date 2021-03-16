#pragma once
#include <string>

struct GLFWwindow;

namespace Ayo 
{
	class Window
	{
	public:
		Window(int height, int width, const std::string& title);
		~Window();

		void update() const;
		void clearColorBuffer() const; 
		void processInput() const;
		bool isCloseRequested() const;

		/*TODO: Temp, move this out to render loop*/
		double getTime() const;

	private:
		GLFWwindow* m_pWindow;
		int m_height;
		int m_width;

		std::string m_title;

	};

}
