#pragma once
#include <string>
#include <memory>

struct SDL_Texture;

namespace dae
{
	class Texture2D
	{
	public:

		// Rule of 5
		Texture2D(SDL_Texture* texture);
		Texture2D(const Texture2D &) = delete;
		Texture2D(Texture2D &&) = delete;
		Texture2D & operator= (const Texture2D &) = delete;
		Texture2D & operator= (const Texture2D &&) = delete;
		~Texture2D();

		// Getters and Setters
		SDL_Texture* GetSDLTexture() const;
		int GetTextureWidth() const { return m_TextureWidth; }
		int GetTextureHeight() const { return m_TextureHeight; }

	private:

		// Variables
		SDL_Texture* m_Texture;
		int m_TextureWidth;
		int m_TextureHeight;

	};
}
