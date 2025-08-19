#pragma once

#include "ISceneClass.h"

namespace Scene
{
	class SceneClass : public ISceneClass
	{
	public:
		SceneClass(SceneID current, SceneID next);
		virtual ~SceneClass();

		virtual SceneID GetCurrentSceneState() const override { return m_CurrentSceneState; }
		virtual SceneID GetNextSceneState() const override { return m_NextSceneState; }
		virtual Graphic::Terrain::TerrainID GetTerrainID() const override { return m_TerrainID; }
		virtual Graphic::Terrain::SkyDomeID GetSkyDomeID() const override { return m_SkyDomeID; }
		virtual bool IsSceneEnded() const override { return m_IsSceneEnded; }

		virtual void Frame(const System::InputClass* input, Object::ObjectManagerClass* objects, Text::TextManagerClass* texts, UI::UIManagerClass* UIs, Sound::SoundManagerClass* sounds, float frame_time) override = 0;
		virtual Object::IObjectClass* GetActiveCamera() override = 0;
		virtual std::unique_ptr<ISceneClass> Clone() const override = 0;

	protected:
		virtual void SetSceneEnded() override { m_IsSceneEnded = true; }
		virtual void SetNextScene(SceneID next) override { m_NextSceneState = next; }
		virtual void SetTerrainID(Graphic::Terrain::TerrainID id) override { m_TerrainID = id; }
		virtual void SetSkyDomeID(Graphic::Terrain::SkyDomeID id) override { m_SkyDomeID = id; }

	private:
		SceneID m_CurrentSceneState = SceneID::NONE;
		SceneID m_NextSceneState = SceneID::NONE;

		Graphic::Terrain::TerrainID m_TerrainID = Graphic::Terrain::TerrainID::NONE;
		Graphic::Terrain::SkyDomeID m_SkyDomeID = Graphic::Terrain::SkyDomeID::NONE;

		bool m_IsSceneEnded = false;
		bool m_IsShowingCursor = true;
	};
}