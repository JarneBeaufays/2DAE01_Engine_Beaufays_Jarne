#pragma once
namespace dae
{
	class SceneObject
	{
	public:

		virtual void Delete() { m_Delete = true; }
		virtual void Update() = 0;
		virtual void Render() const = 0;
		virtual bool GetDelete() const { return m_Delete; }

		SceneObject() = default;
		virtual ~SceneObject() = default;
		SceneObject(const SceneObject& other) = delete;
		SceneObject(SceneObject&& other) = delete;
		SceneObject& operator=(const SceneObject& other) = delete;
		SceneObject& operator=(SceneObject&& other) = delete;

	private:

		bool m_Delete{ false };

	};
}
