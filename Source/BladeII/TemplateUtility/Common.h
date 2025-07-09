#pragma once

#include <sstream>

namespace TU
{
	template <std::size_t... Ts>
	struct index 
	{
	};

	template <std::size_t N, std::size_t... Ts>
	struct generator_sequences : generator_sequences<N - 1, N - 1, Ts...> 
	{
	};

	template <std::size_t... Ts>
	struct generator_sequences<0, Ts...> : index<Ts...>
	{
	};

	class NullType {};

	template <typename T, typename U>
	struct IsSameType
	{
		enum { value = false };
	};

	template <typename T>
	struct IsSameType<T, T>
	{
		enum { value = true };
	};

	template<typename Tuple>
	struct Extractor
	{
		template <typename... UnpackArgs, std::size_t... Indices>
		void ExtractFromTuple(const Tuple& t, TU::index<Indices...>, UnpackArgs&&... args)
		{
			static_assert(sizeof...(Indices) == sizeof...(UnpackArgs), "must be the same between tuple elements and arguments.");
			std::tie(args...) = std::make_tuple(std::get<Indices>(t)...);
		}

		template<typename... UnpackArgs>
		void ExtractFromTuple(const Tuple& t, UnpackArgs&... args)
		{
			static_assert(std::tuple_size<Tuple>::value == sizeof...(UnpackArgs), "must be the same between tuple elements and arguments.");
			ExtractFromTuple(t, TU::generator_sequences<sizeof...(UnpackArgs)>(), args...);
		}
	};

	template<typename T, typename U>
	struct HasAssignmentOperator
	{
		template<typename Z, T& (Z::*)(const U)>
		struct SFINAE
		{
		};

		template<typename Z>
		static char Test(SFINAE<Z, &Z::operator=>*);

		template<typename Z>
		static int	Test(...);

		static const bool Has = sizeof(Test<T>(0)) == sizeof(char);
	};

	template<typename T, typename U>
	class CopyVectorHelper
	{
	public:
		CopyVectorHelper(TArray<T>* link) : converted(link)
		{
		}

		CopyVectorHelper& operator = (const TArray<U>& rhs)
		{
			static_assert(HasAssignmentOperator<T, U>::Has, "T type must have the overrided operator =.");

			T	dest;
			for (auto elem : rhs)
			{
				dest = elem;
				converted->Add(dest);
			}

			return *this;
		}

	private:
		TArray<T>*	converted;
	};

	template<typename T>
	std::string ToString(T number)
	{
		std::ostringstream convStream;
		convStream << number;

		return convStream.str();
	}
}