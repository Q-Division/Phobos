
#ifndef INDICES_H
#define INDICES_H

namespace Phobos
{
	namespace Laputa
	{
		//"Tag"
		template<size_t... Is>
		struct Indices {};

		//Recursively inherits
		template<size_t N, size_t... Is>
		struct IndicesBuilder : IndicesBuilder<N - 1, N - 1, Is...> {};

		//Base Case
		template<size_t... Is>
		struct IndicesBuilder<0, Is...>
		{
			using type = Indices<Is...>;
		};

		template<typename T>
		struct ID{};


		template <typename T>
		struct Arity {
			static const int value = 1;
		};

		template <typename... Vs>
		struct Arity<std::tuple<Vs...>> {
			static const int value = sizeof...(Vs);
		};

		template <>
		struct Arity<void> {
			static const int value = 0;
		};
	}
}

#endif //INDICES_H