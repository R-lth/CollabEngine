#pragma once
#include "Core.h"
#include <memory>

namespace Monkey
{
	class Monkey_API MonkeyObject
	{
	public:
		virtual size_t GetType() const = 0; // 현재 객체 타입 ID 반환
		virtual bool Is(size_t id) const { return false; } // 전달된 타입 ID와 현재 객체 비교

		template<typename T>
		bool IsTypeOf() const { return Is(T::TypeId()); } // 타입 질문 함수

		template<typename T, typename U>
		std::shared_ptr<T> Cast(const std::shared_ptr<U>& object) // 스마트 포인터 형변환 유틸리티 함수 
		{
			if (!object) { return nullptr; } // 예외처리

			if (object->Is(T::TypeId())) { return std::static_pointer_cast<T>(object); } // object의 실제 타입이 T(또는 T 파생)인지 확인 후 캐스팅

			return nullptr; // 형변환이 허용되지 않는 경우에는 null 반환
		}
	};
}

// 타입 시스템을 사용할 클래스(Actor 타입)에 추가할 매크로
#define TYPE_DECLARATIONS(Type, ParentType)                          \
    using super = ParentType;                                        \
protected:                                                           \
    static size_t TypeIdClass()                                      \
    {                                                                \
         static int runtimeTypeId = 0;                               \
         return reinterpret_cast<size_t>(&runtimeTypeId);            \
    }                                                                \
public:                                                              \
    static size_t TypeId()                                           \
	{														         \
         return Type::TypeIdClass();                                 \
	}                                                                \
	virtual size_t GetType() const override                          \
    {                                                                \
	     return Type::TypeIdClass();                                 \
    }                                                                \
    virtual bool Is(size_t id) const override                        \
    {                                                                \
         return (id == TypeIdClass())? true : ParentType::Is(id);    \
    }
