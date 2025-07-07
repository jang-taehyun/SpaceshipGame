#pragma once

#include "resource.h"

/*
* TODO: terrain 공부해서 추가
* TODO: IMGUIClass 오류 수정
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
* 3) Scene 클래스를 상속받은 자식 클래스에서는 반드시 필요한 object, text, ui를 manager를 통해서 생성할 것
* 
* 4. Shader 관련 규칙
* 1) Shader 클래스를 상속받은 자식 클래스에서는 반드시 복사 생성자, 이동 생성자, 복사 대입연산자, 이동 대입 연산자를 구현할 것
* 2) Shader 클래스의 인스턴스를 생성할 때 manager를 통해서 생성을 요청할 것
* 3) Shader 클래스를 상속받은 자식 클래스를 factory에 등록할 것
* 
* 5. Model 관련 규칙
* 1) Model 클래스를 상속받은 자식 클래스에서는 반드시 복사 생성자, 이동 생성자, 복사 대입연산자, 이동 대입 연산자를 구현할 것
* 2) Model 클래스의 인스턴스를 생성할 때 manager를 통해서 생성을 요청할 것
* 3) Model 클래스를 상속받은 자식 클래스를 factory에 등록할 것
* 
* 6. Object, UI, Text 관련 규칙
* 1) Object, UI, Text 클래스의 인스턴스를 생성할 때 manager를 통해서 생성을 요청할 것
* 2) Object, UI 클래스의 자식 클래스 생성 시, factory에 등록할 것
* 
* 
*/