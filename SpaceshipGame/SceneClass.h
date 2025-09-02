#pragma once

#include "ISceneClass.h"

namespace Scene
{
	class SceneClass : public ISceneClass
	{
	public:
		SceneClass(ID current, ID next);
		virtual ~SceneClass();

		virtual ID GetCurrentSceneID() const override { return m_CurrentSceneID; }
		virtual ID GetNextSceneID() const override { return m_NextSceneID; }
		virtual Graphic::Terrain::TerrainID GetTerrainID() const override { return m_TerrainID; }
		virtual Graphic::Terrain::SkyDomeID GetSkyDomeID() const override { return m_SkyDomeID; }
		virtual bool IsSceneEnded() const override { return m_IsSceneEnded; }

		virtual void Frame(const System::InputClass* input, Object::ObjectManagerClass* objects, Text::TextManagerClass* texts, UI::UIManagerClass* UIs, Sound::SoundManagerClass* sounds, float frame_time) override = 0;
		virtual Object::IObjectClass* GetActiveCamera() override { return nullptr; }
		virtual std::unique_ptr<ISceneClass> Clone() const override = 0;

	protected:
		void SetSceneEnded() { m_IsSceneEnded = true; }
		void SetShowMouseCursor(bool IsShow);
		void SetNextScene(ID next) { m_NextSceneID = next; }
		void SetTerrainID(Graphic::Terrain::TerrainID id) { m_TerrainID = id; }
		void SetSkyDomeID(Graphic::Terrain::SkyDomeID id) { m_SkyDomeID = id; }

	private:
		ID m_CurrentSceneID = ID::NONE;
		ID m_NextSceneID = ID::NONE;

		Graphic::Terrain::TerrainID m_TerrainID = Graphic::Terrain::TerrainID::NONE;
		Graphic::Terrain::SkyDomeID m_SkyDomeID = Graphic::Terrain::SkyDomeID::NONE;

		bool m_IsSceneEnded = false;
		bool m_IsECSPopupAppear = false;
	};
}