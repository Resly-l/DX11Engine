#pragma once
#include <string>

// classes above crtp class must have default constructor

#define DECLARE_CRTP(type, id_type) \
template<typename T> \
class type : public type##Base \
{ \
public: \
	inline static id_type ID = GID++; \
	static const std::string stringID; \
public : \
	id_type GetID() const override final { return ID; } \
	std::string GetStringID() const override final { return stringID; } \
}; \
\
template<typename T> \
const std::string type<T>::stringID(std::string(typeid(T).name()), 6);