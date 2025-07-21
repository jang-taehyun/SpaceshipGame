#pragma once

#include "resource.h"

/*
* TODO: terrain 공부해서 추가
*/

/**
* CODE 규칙
* 
* 1. 메모리 규칙
* 1) new, delete 사용 금지
* 2) 소유권을 넘길 시에는 스마트 포인터 사용, 그 외에는 raw pointer로 사용
* 
* 2. 자료형 사용 규칙
* 1) DirectX 관련 자료형 사용 시,
*	- 연산에는 XMMATRIX, XMVECTOR 같은 자료형 사용
*	- 반환, 매개변수에는 XMFLOAT4 같은 자료형 사용
* 
* 3. Scene 관련 규칙
* 1) Scene 클래스를 상속받은 자식 클래스에서는 반드시 복사 생성자, 이동 생성자, 복사 대입연산자, 이동 대입 연산자를 구현할 것
* 2) Scene 클래스에서 무조건 Camera 클래스의 인스턴스는 한 개 이상 만들 것
* 3) 새로운 Scene을 프로젝트에 추가할 때의 규칙
*	3-1) SceneClass를 상속받아서 새로운 Class를 정의할 것
*	3-2) 새로운 Scene의 ID를 config.cpp 파일에 등록할 것
*	3-3) SceneClass를 상속받은 새로운 Class를 SceneFactoryClass에 등록할 것
* 4) Scene 클래스를 상속받은 자식 클래스에서는 반드시 필요한 object, text, ui, sound를 manager를 통해서 생성할 것
*	4-1) object, text, ui는 load() 함수를 호출하여 생성할 것
*	4-2) sound는 비트마스크를 통해 필요한 sound의 ID를 표시한 후에 load() 함수를 호출하여 생성할 것 
* 
* 4. Shader 관련 규칙
* 1) Shader 클래스를 상속받은 자식 클래스에서는 반드시 복사 생성자, 이동 생성자, 복사 대입연산자, 이동 대입 연산자를 구현할 것
* 2) Shader 클래스의 인스턴스를 생성할 때 manager를 통해서 생성을 요청할 것
* 3) 새로운 Shader를 프로젝트에 추가할 때의 규칙
*	3-1) ShaderClass를 상속받아서 새로운 Class를 정의할 것
*	3-2) 새로운 Shader의 ID를 config.cpp 파일에 등록할 것
*		- vertex shader 파일은 shader 디렉토리 내의 vertex 디렉토리에 저장할 것
*		- pixel shader 파일은 shader 디렉토리 내의 pixel 디렉토리에 저장할 것
*		- vertex shader 파일의 상대 경로, pixel shader 파일의 상대 경로는 config.cpp 안에 있는 map에 ID와 함께 등록할 것
*	3-3) ShaderClass를 상속받은 새로운 Class를 ShaderFactoryClass에 등록할 것
*	3-4) 새로운 ShaderClass의 buffer를 업데이트하는 함수를 ShaderBufferUpdaterClass에 등록할 것
*		- 만약 새로운 buffer type이 필요하다면,
*			config.h 파일 안에 buffer type을 정의하고,
*			BuffersData 구조체에 등록할 것.
* 
* 5. Model 관련 규칙
* 1) Model 클래스를 상속받은 자식 클래스에서는 반드시 복사 생성자, 이동 생성자, 복사 대입연산자, 이동 대입 연산자를 구현할 것
* 2) Model 클래스의 인스턴스를 생성할 때 manager를 통해서 생성을 요청할 것
* 3) 새로운 Model을 프로젝트에 추가할 때의 규칙
*	3-1) 새로운 Model을 메모리에 load 시 ModelLoaderClass를 상속받아서 새로운 Class를 정의할 것
*	3-2) 새로운 Model의 ID를 config.cpp 파일에 등록할 것
*		- model 파일은 resource 디렉토리에 저장할 것
*		- model 파일의 상대 경로는 config.cpp 안에 있는 map에 ID와 함께 등록할 것
*		- model 파일 안에 필요한 texture의 경로는 config.cpp 안에 있는 map에 ID와 함께 등록할 것
*	3-3) ModelLoaderClass를 상속받은 새로운 Class를 ModelFactoryClass에 등록할 것
* 
* 6. Sound 관련 규칙
* 1) 새로운 sound를 프로젝트에 추가할 때의 규칙
*	1-1) 새로운 sound의 ID를 config.cpp 파일에 등록할 것
*		- sound 파일은 resource 디렉토리에 저장할 것
*		- sound 파일의 상대 경로는 config.cpp 안에 있는 map에 ID와 함께 등록할 것
*		- sound 파일은 .wav 파일을 사용할 것
* 
* 7. Object, UI, Text 관련 규칙
* 1) Object, UI, Text 클래스의 인스턴스를 생성할 때 manager를 통해서 생성을 요청할 것
* 2) Object, UI 클래스의 자식 클래스 생성 시, factory에 등록할 것
* 
* 8. 객체 생성 관련
* 1) 싱글톤 패턴 금지
* 
* 9. 예외 처리 관련
* 1) 예외 처리는 assert()를 사용할 것
* 2) WINAPI, DirectX 관련 API를 사용하는 경우 HRESULT 자료형을 이용해 리턴값을 받고, assert()를 통해 검사할 것
*/