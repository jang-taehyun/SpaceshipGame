#pragma once

#include "ISceneClass.h"

namespace Scene
{
	class SceneClass : public ISceneClass
	{
	public:
		SceneClass(ID current, ID next);
		virtual ~SceneClass();

		virtual ID GetCurrentSceneState() const override { return m_CurrentSceneState; }
		virtual ID GetNextSceneState() const override { return m_NextSceneState; }
		virtual Graphic::Terrain::TerrainID GetTerrainID() const override { return m_TerrainID; }
		virtual Graphic::Terrain::SkyDomeID GetSkyDomeID() const override { return m_SkyDomeID; }
		virtual bool IsSceneEnded() const override { return m_IsSceneEnded; }

		virtual HWND GetSceneWindowHandle() const override { return m_hSceneWindow; }
		virtual LRESULT CALLBACK MessageHandler(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam) override;

		virtual void Frame(const System::InputClass* input, Object::ObjectManagerClass* objects, Text::TextManagerClass* texts, UI::UIManagerClass* UIs, Sound::SoundManagerClass* sounds, float frame_time) override = 0;
		virtual Object::IObjectClass* GetActiveCamera() override { return nullptr; }
		virtual std::unique_ptr<ISceneClass> Clone() const override = 0;

	protected:
		void SetSceneEnded() { m_IsSceneEnded = true; }
		void SetNextScene(ID next) { m_NextSceneState = next; }
		void SetTerrainID(Graphic::Terrain::TerrainID id) { m_TerrainID = id; }
		void SetSkyDomeID(Graphic::Terrain::SkyDomeID id) { m_SkyDomeID = id; }

	private:
		ID m_CurrentSceneState = ID::NONE;
		ID m_NextSceneState = ID::NONE;
		HWND m_hSceneWindow = 0;

		Graphic::Terrain::TerrainID m_TerrainID = Graphic::Terrain::TerrainID::NONE;
		Graphic::Terrain::SkyDomeID m_SkyDomeID = Graphic::Terrain::SkyDomeID::NONE;

		bool m_IsSceneEnded = false;
		bool m_IsShowingCursor = true;
		bool m_IsECSKeyPressed = false;
	};

	// static ISceneClass* SceneHandler = nullptr;
	static LRESULT CALLBACK SceneWndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);
}