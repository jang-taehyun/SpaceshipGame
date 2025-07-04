#pragma once

namespace System { class InputClass; }
namespace Text { class TextManagerClass; }
namespace UI { class UIManagerClass; }
namespace Sound { class SoundManagerClass; }
namespace Object
{
	class ObjectManagerClass;
	class IObjectClass;
}

namespace Scene
{
	class ISceneClass
	{
	public:
		virtual ~ISceneClass() = default;

		virtual inline SceneState GetCurrentSceneState() const = 0;
		virtual inline SceneState GetNextSceneState() const = 0;
		virtual inline bool IsSceneEnded() const = 0;

		virtual inline Object::IObjectClass* GetCamera() const = 0;

		virtual void Frame(const System::InputClass* input, Object::ObjectManagerClass* objects, Text::TextManagerClass* texts, UI::UIManagerClass* UIs, Sound::SoundManagerClass* sounds, float frame_time) = 0;
		virtual inline std::unique_ptr<ISceneClass> Clone() const = 0;

	protected:
		virtual inline void SetSceneEnded() = 0;

		virtual inline void SetNextScene(SceneState next) = 0;
		virtual inline void SetModelIDs(UINT IDs) = 0;
		virtual inline void SetUITextureIDs(UINT IDs) = 0;
	};
}