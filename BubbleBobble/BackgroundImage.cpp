#include "BackgroundImage.h"
#include "ImageComponent.h"
#include "Renderer.h"
#include <string>

BackgroundImage::BackgroundImage(std::string filePath)
	: GameObject{ }
{
	// Getting our component
	ImageComponent* imageComponent = new ImageComponent{ this, filePath, false };
	b2Vec2 window{ (float)dae::Renderer::GetInstance().GetWindowWidth(), (float)dae::Renderer::GetInstance().GetWindowHeight() };
	imageComponent->Resize(window);
	imageComponent->SetOffset(0.0f, -1.f * dae::Renderer::GetInstance().GetWindowHeight());

	// Adding our component to our Game Object
	AddComponent(imageComponent);
}