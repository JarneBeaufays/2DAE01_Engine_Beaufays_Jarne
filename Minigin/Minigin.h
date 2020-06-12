#pragma once
struct SDL_Window;
namespace dae
{
	class Minigin
	{
	public:

		// Functions
		SDL_Window* GetWindow() const { return m_Window; }
		void Initialize(unsigned int width, unsigned int height);
		void Cleanup();
		void Run();

	private:

		// Variables
		static const int MsPerFrame = 16; //16 for 60 fps, 33 for 30 fps
		SDL_Window* m_Window{};
	
	};
}