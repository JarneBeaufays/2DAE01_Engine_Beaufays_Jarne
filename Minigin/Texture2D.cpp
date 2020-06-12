#include "MiniginPCH.h"
#include "Texture2D.h"
#include <SDL.h>

dae::Texture2D::Texture2D(SDL_Texture* texture)
{
	// Setting our texture
	m_Texture = texture;

	// Getting some usefull information regarding our texture
	SDL_QueryTexture(texture, nullptr, nullptr, &m_TextureWidth, &m_TextureHeight);
}

dae::Texture2D::~Texture2D()
{
	SDL_DestroyTexture(m_Texture);
}

SDL_Texture* dae::Texture2D::GetSDLTexture() const
{
	return m_Texture;
}