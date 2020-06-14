#include "MiniginPCH.h"
#include "Renderer.h"
#include <SDL.h>
#include "SceneManager.h"
#include "..\BubbleBobble\GameInfo.h"
#include "Texture2D.h"

void dae::Renderer::Init(SDL_Window * window, unsigned int width, unsigned int height)
{
	m_Renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (m_Renderer == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
	}
	m_WindowWidth = width;
	m_WindowHeight = height;
}

void dae::Renderer::Render() const
{
	SDL_RenderClear(m_Renderer);

	SceneManager::GetInstance().Render();
	GameInfo::GetInstance().Render();

	SDL_RenderPresent(m_Renderer);
}

void dae::Renderer::Destroy()
{
	if (m_Renderer != nullptr)
	{
		SDL_DestroyRenderer(m_Renderer);
		m_Renderer = nullptr;
	}
}

void dae::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y) const
{
	SDL_Rect dst;
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	SDL_QueryTexture(texture.GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

void dae::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y, const float width, const float height) const
{
	SDL_Rect dst;
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	dst.w = static_cast<int>(width);
	dst.h = static_cast<int>(height);

	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

void dae::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y, const float width, const float height, float angle, bool flipped) const
{
	SDL_Rect dst;
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	dst.w = static_cast<int>(width);
	dst.h = static_cast<int>(height);
	
	if(flipped)
		SDL_RenderCopyEx(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst, (double)angle, nullptr, SDL_FLIP_HORIZONTAL);
	else
		SDL_RenderCopyEx(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst, (double)angle, nullptr, SDL_FLIP_NONE);
}

void dae::Renderer::RenderTexture(SDL_Texture* texture, float x, float y, float width, float height, float angle, bool flipped) const
{
	SDL_Rect dst;
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	dst.w = static_cast<int>(width);
	dst.h = static_cast<int>(height);

	if (flipped)
		SDL_RenderCopyEx(GetSDLRenderer(), texture, nullptr, &dst, (double)angle, nullptr, SDL_FLIP_HORIZONTAL);
	else
		SDL_RenderCopyEx(GetSDLRenderer(), texture, nullptr, &dst, (double)angle, nullptr, SDL_FLIP_NONE);
}

void dae::Renderer::RenderTexture(const Texture2D& texture, const SDL_Rect& srcRect, const float x, const float y, const float width, const float height, float angle, bool flipped) const
{
	SDL_Rect dst;
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	dst.w = static_cast<int>(width);
	dst.h = static_cast<int>(height);

	if(flipped)
		SDL_RenderCopyEx(GetSDLRenderer(), texture.GetSDLTexture(), &srcRect, &dst, (double)angle, nullptr, SDL_FLIP_HORIZONTAL);
	else
		SDL_RenderCopyEx(GetSDLRenderer(), texture.GetSDLTexture(), &srcRect, &dst, (double)angle, nullptr, SDL_FLIP_NONE);
}

void dae::Renderer::RenderTexture(SDL_Texture* texture, const SDL_Rect& srcRect, float x, float y, float width, float height, float angle, bool flipped) const
{
	SDL_Rect dst;
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	dst.w = static_cast<int>(width);
	dst.h = static_cast<int>(height);

	if(flipped)
		SDL_RenderCopyEx(GetSDLRenderer(), texture, &srcRect, &dst, (double)angle, nullptr, SDL_FLIP_HORIZONTAL);
	else
		SDL_RenderCopyEx(GetSDLRenderer(), texture, &srcRect, &dst, (double)angle, nullptr, SDL_FLIP_NONE);
}

