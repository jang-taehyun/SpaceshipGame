#pragma once

namespace Scene { class SceneManagerClass; }
namespace Graphic
{
	class D3DClass;
	class LightClass;

	namespace Model { class ModelManagerClass; }
	namespace Shader { class ShaderManagerClass; }
	namespace Texture
	{
		class UITextureManagerClass;
		class UIRenderClass;
	}
}

#ifdef DEBUG
namespace Graphic { class IMGUIClass; }
#endif // DEBUG

namespace Graphic
{
	class GraphicsClass
	{
	public:
		GraphicsClass(HWND hwnd, int ScreenWidth, int ScreenHeight);
		virtual ~GraphicsClass();

		void Frame(Scene::SceneManagerClass* SceneManager, bool IsLoad);

	private:
		void Initialize(HWND hwnd, int ScreenWidth, int ScreenHeight);
		void Load(Scene::SceneManagerClass* SceneManager);
		void Render(Scene::SceneManagerClass* SceneManager);

	private:
		static bool IsInitialize;

		std::unique_ptr<D3DClass> m_D3D = nullptr;
		std::unique_ptr<LightClass> m_Light = nullptr;
		std::unique_ptr<Model::ModelManagerClass> m_ModelManager = nullptr;
		std::unique_ptr<Shader::ShaderManagerClass> m_ShaderManager = nullptr;
		std::unique_ptr<Texture::UITextureManagerClass> m_UITextureManager = nullptr;
		std::unique_ptr<Texture::UIRenderClass> m_UIRender = nullptr;
		
	public:
		GraphicsClass(const GraphicsClass& other) = delete;
		GraphicsClass(GraphicsClass&& other) = delete;
		GraphicsClass& operator=(const GraphicsClass& other) = delete;
		GraphicsClass& operator=(GraphicsClass&& other) = delete;

#ifdef DEBUG
	public:
		void ImGuiRender(Scene::SceneManagerClass* SceneManager);
	private:
		IMGUIClass* m_IMGUI = nullptr;
#endif // DEBUG

	};
}