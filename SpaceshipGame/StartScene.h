#pragma once

#include "BaseSceneClass.h"

class InputClass;

class StartScene : public BaseSceneClass
{
public:
	StartScene() = default;
	virtual ~StartScene() = default;

	virtual inline std::unique_ptr<ISceneClass> Clone() const override { return std::make_unique<StartScene>(*this); }

	virtual void Frame(const InputClass* input);
};

