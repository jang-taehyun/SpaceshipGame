#pragma once

namespace System { class InputClass; }
namespace Sound { class SoundClass; }
namespace Object
{
	class CameraClass;
	class ActorManagerClass;
}
namespace Scene
{
	class ISceneClass;
	class SceneLoaderClass;
}

namespace Scene
{
	class SceneManagerClass
	{
	public:
		SceneManagerClass();
		~SceneManagerClass();

	public:
		void Frame(const System::InputClass* input, float frame_time);

	private:
		void ChangeScene();

	private:
		HRESULT ProcessCamera(const Object::CameraClass* camera, const System::InputClass* input, float frame_time);
		HRESULT ProcessActor(const Object::ActorManagerClass* actor_manager, const System::InputClass* input, float frame_time);
		void ProcessSceneInfo();

		void UpdateCameraFrameTime(const Object::CameraClass* camera, float frame_time);
		HRESULT UpdateCameraAffine(const Object::CameraClass* camera, const System::InputClass* input);
		void UpdateObjectFrameTime(Object::ActorManagerClass* const& actor_manager, const float& frame_time);
		HRESULT UpdateActorCollisionState(Object::ActorManagerClass* const& actor_manager, const System::InputClass* const& input);
		HRESULT UpdateActorAffine(Object::ActorManagerClass* const& actor_manager, const InputClass* const& input);

	private:
		bool m_IsShowingCursor = true;
		std::unique_ptr<Sound::SoundClass> m_Sound = nullptr;


	private:
		static bool IsInitialize;
		std::unique_ptr<ISceneClass> m_Scene = nullptr;
		std::unique_ptr<SceneLoaderClass> m_SceneLoader = nullptr;

	public:
		SceneManagerClass(const SceneManagerClass& other) = delete;
		SceneManagerClass(SceneManagerClass&& other) = delete;
		SceneManagerClass& operator=(const SceneManagerClass& other) = delete;
		SceneManagerClass& operator=(SceneManagerClass&& other) = delete;
	};

}