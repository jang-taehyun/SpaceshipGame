#pragma once

namespace System { class InputClass; }
namespace Text { class TextManagerClass; }
namespace Sound { class SoundManagerClass; }
namespace UI
{
	class IUIClass;
	class UIManagerClass;
}
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

		virtual ID GetCurrentSceneID() const = 0;
		virtual ID GetNextSceneID() const = 0;
		virtual Graphic::Terrain::TerrainID GetTerrainID() const = 0;
		virtual Graphic::Terrain::SkyDomeID GetSkyDomeID() const = 0;
		virtual bool IsSceneEnded() const = 0;
		
		virtual void Frame(const System::InputClass* input, Object::ObjectManagerClass* objects, Text::TextManagerClass* texts, UI::UIManagerClass* UIs, Sound::SoundManagerClass* sounds, float frame_time) = 0;
		virtual Object::IObjectClass* GetActiveCamera() = 0;
		virtual std::unique_ptr<ISceneClass> Clone() const = 0;
	};
}