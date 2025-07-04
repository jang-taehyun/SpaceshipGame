#pragma once

#include "resource.h"

/*
* TODO: ShaderManagerClass, ShaderFactoryClass 추가
* TODO: object manager, ui manager, text manager 로드 방식 고민
* -> ui, object, text 생성 시에 position, rotate, scale은 받지 않도록 설정
* -> factory를 통해 instance를 생성하고, 해당 instance를 넘겨주어 scene에서 초기에 설정할 수 있도록 설정
* TODO: terrain 공부해서 추가
* TODO: GraphicsClass, IMGUIClass, SceneManagerClass, SystemClass 오류 수정
* TODO: Frustum 추가
* TODO: Scene 클래스들에 복사 생성자, 이동 생성자, 복사 대입 연산자, 이동 대입 연산자 구현
*/

/**
* CODE 규칙
* 
* 1. new, delete 사용 금지
* 2. 소유권을 넘길 시에는 스마트 포인터 사용, 그 외에는 raw pointer로 사용
* 3. DirectX 관련 자료형 사용 시,
* - 연산에는 XMMATRIX, XMVECTOR 같은 자료형 사용
* - 반환, 매개변수에는 XMFLOAT4 같은 자료형 사용
*/