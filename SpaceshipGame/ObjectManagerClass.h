#pragma once

namespace System { class InputClass; }
namespace Text { class TextManagerClass; }
namespace Sound { class SoundManagerClass; }

namespace Object
{ 
	class IObjectClass;
	class ObjectFactoryClass;
}

namespace Object
{
	class ObjectManagerClass
	{
	public:
		ObjectManagerClass();
		virtual ~ObjectManagerClass();

		IObjectClass* GetCamera() const { return (m_ObjectList.size() ? m_ObjectList[m_CameraIdx].get() : nullptr); }

		UINT GetCameraIdx() const { return m_CameraIdx; }
		void SetCameraIdx(UINT idx) { m_CameraIdx = idx; }
		UINT GetObjectCount() const { return static_cast<UINT>(m_ObjectList.size()); }

		IObjectClass* GetGameObject(UINT idx) const;
		UINT GetModelMask() const { return m_ModelMask; }

		void Frame(const System::InputClass* input, Text::TextManagerClass* texts, Sound::SoundManagerClass* sounds, float frame_time, bool IsESCPopupWindowActivated);
		UINT Load(ID ObjectID, Graphic::Model::ID ModelID);
		void Release();

	private:
		static bool IsInitialize;

		std::vector<std::unique_ptr<IObjectClass>> m_ObjectList;
		std::unique_ptr<ObjectFactoryClass> m_Loader = nullptr;

		UINT m_CameraIdx = 0;
		UINT m_ModelMask = 0;

	public:
		ObjectManagerClass(const ObjectManagerClass& other) = delete;
		ObjectManagerClass(ObjectManagerClass&& other) = delete;
		ObjectManagerClass& operator=(const ObjectManagerClass& other) = delete;
		ObjectManagerClass& operator=(ObjectManagerClass&& other) = delete;
	};
}