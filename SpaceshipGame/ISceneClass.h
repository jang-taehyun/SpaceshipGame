#pragma once

namespace System { class InputClass; }
namespace Text { class TextManagerClass; }
namespace UI { class UIManagerClass; }
namespace Sound { class SoundManagerClass; }
namespace Object
{
	class ActorManagerClass;
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

		virtual void Frame(const System::InputClass* input, float frame_time) = 0;
		virtual inline std::unique_ptr<ISceneClass> Clone() const = 0;

		virtual inline Text::TextManagerClass* GetTextManager() const = 0;
		virtual inline UI::UIManagerClass* GetUIManager() const = 0;
		virtual inline Object::ActorManagerClass* GetActorManager() const = 0;

		virtual inline Object::IObjectClass* GetCamera() const = 0;

	protected:
		virtual inline Sound::SoundManagerClass* GetSoundManager() const = 0;
		virtual inline void SetNextScene(SceneState next) = 0;
		virtual inline void SetSceneEnded() = 0;
	};
}