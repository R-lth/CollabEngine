#pragma once
#pragma warning(disable: 4251)

#define DLLEXPORT __declspec(dllexport) // DLL이 외부에서 사용가능하도록 공개 설정
#define DLLIMPORT __declspec(dllimport) // 다른 DLL의 기능을 가져와 사용가능하도록 설정

#if defined(ENGINE_BUILD_DLL) // ENGINE_BUILD_DLL 이 정의되어 있는지 확인
#define Monkey_API DLLEXPORT  // DLL로 빌드되는 프로젝트면 자동으로 export 
#else
#define Monkey_API DLLIMPORT  // DLL를 사용하는 프로젝트면 자동으로 import
#endif