#pragma once

namespace System { class InputClass; }
namespace Sound { class SoundClass; }
namespace Object
{
	class CameraClass;
	class ActorManagerClass;
}
namespace Scene { class ISceneClass; }

namespace Scene
{
	class SceneManagerClass
	{
	public:
		SceneManagerClass();
		virtual ~SceneManagerClass();

	public:
		HRESULT Frame(const System::InputClass* input, float frame_time);

	private:
		HRESULT ChangeScene();

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
		std::unique_ptr<Scene::ISceneClass> m_Scene = nullptr;

	public:
		SceneManagerClass(const SceneManagerClass& other) = delete;
		SceneManagerClass(SceneManagerClass&& other) = delete;
		SceneManagerClass& operator=(const SceneManagerClass& other) = delete;
		SceneManagerClass& operator=(SceneManagerClass&& other) = delete;
	};

}