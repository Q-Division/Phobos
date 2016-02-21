#ifndef PHOBOS_TYPENAME_H
#define PHOBOS_TYPENAME_H

template <typename T>
struct TypeName
{
	static const char* Get()
	{
		return typeid(T).name();
	}
};

#define ENABLE_TYPENAME(A, B) template<> struct TypeName<A> { static const char* Get() { return B; }};

#endif //PHOBOS_TYPENAME_H