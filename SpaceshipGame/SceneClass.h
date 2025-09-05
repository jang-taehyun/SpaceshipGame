#pragma once

#include "ISceneClass.h"

namespace Scene
{
	class SceneClass : public ISceneClass
	{
	private:
		enum class ObjectID
		{
			POPUP_BACKGROUND,
			POPUP_UI,
			POPUP_UI_TEXT,
			POPUP_BUTTON_UI_OK,
			POPUP_BUTTON_UI_CANCEL,
			POPUP_BUTTON_TEXT_OK,
			POPUP_BUTTON_TEXT_CANCEL,
		};

	public:
		SceneClass(ID current, ID next, Text::TextManagerClass* texts, UI::UIManagerClass* UIs);
		SceneClass(const SceneClass& other);
		SceneClass(SceneClass&& other) noexcept;
		virtual ~SceneClass();

		SceneClass& operator=(const SceneClass& other);
		SceneClass& operator=(SceneClass&& other) noexcept;

		virtual ID GetCurrentSceneID() const override { return m_CurrentSceneID; }
		virtual ID GetNextSceneID() const override { return m_NextSceneID; }
		virtual Graphic::Terrain::TerrainID GetTerrainID() const override { return m_TerrainID; }
		virtual Graphic::Terrain::SkyDomeID GetSkyDomeID() const override { return m_SkyDomeID; }
		virtual bool IsSceneEnded() const override { return m_IsSceneEnded; }

		virtual void Frame(const System::InputClass* input, Object::ObjectManagerClass* objects, Text::TextManagerClass* texts, UI::UIManagerClass* UIs, Sound::SoundManagerClass* sounds, float frame_time) override;

		virtual std::unique_ptr<ISceneClass> Clone() const override = 0;

	protected:
		bool GetIsShowMouseCursor() const;
		bool GetIsESCPopupActive() const { return m_ESCPopupActive; }

		void SetSceneEnded() { m_IsSceneEnded = true; }
		void SetShowMouseCursor(bool IsShow);
		void SetNextScene(ID next) { m_NextSceneID = next; }
		void SetTerrainID(Graphic::Terrain::TerrainID id) { m_TerrainID = id; }
		void SetSkyDomeID(Graphic::Terrain::SkyDomeID id) { m_SkyDomeID = id; }

		virtual void LoadESCPopupWindow(Text::TextManagerClass* texts, UI::UIManagerClass* UIs);
		virtual bool ProcessESCPopUp(const System::InputClass* input, Text::TextManagerClass* texts, UI::UIManagerClass* UIs);
		virtual void ActiveESCPopup(Text::TextManagerClass* texts, UI::UIManagerClass* UIs);
		virtual void DeactiveESCPopup(Text::TextManagerClass* texts, UI::UIManagerClass* UIs);

		virtual void ActiveChildSceneUI(Text::TextManagerClass* texts, UI::UIManagerClass* UIs) = 0;
		virtual void DeactiveChildSceneUI(Text::TextManagerClass* texts, UI::UIManagerClass* UIs) = 0;
		virtual void ProcessChildScene(const System::InputClass* input, Object::ObjectManagerClass* objects, Text::TextManagerClass* texts, UI::UIManagerClass* UIs, Sound::SoundManagerClass* sounds, float frame_time) = 0;

	private:
		ID m_CurrentSceneID = ID::NONE;
		ID m_NextSceneID = ID::NONE;

		Graphic::Terrain::TerrainID m_TerrainID = Graphic::Terrain::TerrainID::NONE;
		Graphic::Terrain::SkyDomeID m_SkyDomeID = Graphic::Terrain::SkyDomeID::NONE;

		bool m_IsSceneEnded = false;
		bool m_ESCPopupActive = false;
		std::map<ObjectID, UINT> m_UIIdxList;
	};
}