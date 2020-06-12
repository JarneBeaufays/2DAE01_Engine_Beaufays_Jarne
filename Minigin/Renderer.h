#pragma once
#include "Singleton.h"
#include "SDL.h"

struct SDL_Window;
struct SDL_Renderer;

namespace dae
{
	class Texture2D;
	/**
	 * Simple RAII wrapper for the SDL renderer
	 */
	class Renderer final : public Singleton<Renderer>
	{
	public:

		void Init(SDL_Window* window, unsigned int width, unsigned int height);
		void Render() const;
		void Destroy();

		void RenderTexture(const Texture2D& texture, float x, float y) const;
		void RenderTexture(const Texture2D& texture, float x, float y, float width, float height) const;
		void RenderTexture(const Texture2D& texture, float x, float y, float width, float height, float angle, bool flipped) const;
		void RenderTexture(SDL_Texture* texture, float x, float y, float width, float height, float angle, bool flipped) const;
		void RenderTexture(const Texture2D& texture, const SDL_Rect& srcRect, float x, float y, float width, float height, float angle, bool flipped) const;
		void RenderTexture(SDL_Texture* texture, const SDL_Rect& srcRect, float x, float y, float width, float height, float angle, bool flipped) const;

		unsigned int GetWindowHeight() const { return m_WindowHeight; }
		unsigned int GetWindowWidth() const { return m_WindowWidth; }
		SDL_Renderer* GetSDLRenderer() const { return m_Renderer; }

	private:

		SDL_Renderer* m_Renderer{};
		unsigned int  m_WindowHeight;
		unsigned int  m_WindowWidth;

	};
}

