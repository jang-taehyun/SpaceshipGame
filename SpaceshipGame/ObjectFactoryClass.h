#pragma once

#include <functional>

namespace Object { class IObjectClass; }

namespace Object
{
	std::unique_ptr<IObjectClass> CreateActor(Graphic::Model::ID ModelID);
	std::unique_ptr<IObjectClass> CreateCollision(Graphic::Model::ID ModelID);
	std::unique_ptr<IObjectClass> CreateCamera(Graphic::Model::ID ModelID);

	class ObjectFactoryClass
	{
	public:
		ObjectFactoryClass();
		~ObjectFactoryClass();

		std::unique_ptr<IObjectClass> Load(ID ObjectID, Graphic::Model::ID ModelID) const;

	private:
		static bool IsInitialize;

		std::map<ID, std::function<std::unique_ptr<IObjectClass>(Graphic::Model::ID)>> m_Creator;
	};
}