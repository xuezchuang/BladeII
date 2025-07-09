#pragma once

#include "Common.h"

#define MAKE_TYPELIST(ALIAS, ...)			\
	using ALIAS = TU::TypeList<__VA_ARGS__>;

namespace TU
{
	template <typename... Ts>
	struct TypeList {};

	template <>
	struct TypeList<>
	{
		using Result	= NullType;
	};

	template<typename H, typename... T>
	struct TypeList<H, T...>
	{
		using Head = H;
		using Tail = TypeList<T...>;
	};

	template<typename TList>
	struct MakeTuple;

	template<typename... Ts>
	struct MakeTuple<TypeList<Ts...>>
	{
		using Result = std::tuple<Ts...>;
	};

	namespace TL
	{
		/*
			TypeList Length Operator
		*/
		template<typename TList>
		struct Length;

		template <>
		struct Length<TypeList<> >
		{
			enum { value = 0 };
		};
		
		template <typename H, typename... Ts>
		struct Length<TypeList<H, Ts...> >
		{
			enum { value = 1 + Length<typename TypeList<H, Ts...>::Tail>::value };
		};

		/*
			TypeList TypeAt Operator
		*/
		template <typename TList, uint32 index>
		struct TypeAt;

		template <typename H, typename... Ts>
		struct TypeAt<TypeList<H, Ts...>, 0>
		{
			using Result = H;
		};

		template <typename H, typename... Ts, uint32 index>
		struct TypeAt<TypeList<H, Ts...>, index>
		{
			using Result = typename TypeAt<typename TypeList<H, Ts...>::Tail, index-1>::Result;
		};

		/*
			TypeList Append Operator
		*/
		template <typename InsertType, typename TList>
		struct Append;

		template <> 
		struct Append<NullType, NullType>
		{
			using Result = NullType;
		};

		template <typename InsertType>
		struct Append<InsertType, TypeList<> >
		{
			using Result = TypeList<InsertType>;
		};

		template <typename H, typename... Ts>
		struct Append<NullType, TypeList<H, Ts...>>
		{
			using Result = TypeList<H, Ts...>;
		};

		template <typename InsertType, typename... Ts>
		struct Append<InsertType, TypeList<Ts...>>
		{
			using Result = TypeList<Ts..., InsertType>;
		};

		/*
			TypeList Erase Operator
		*/
		template <typename EraseType, typename TList>
		struct Erase;

		template <typename EraseType>
		struct Erase<EraseType, TypeList<>>
		{
			using Result = TypeList<>;
		};

		template <typename EraseType, typename... Ts>
		struct Erase<EraseType, TypeList<EraseType, Ts...>>
		{
			using Result = TypeList<Ts...>;
		};

		template <typename EraseType, typename H, typename... Ts>
		struct Erase<EraseType, TypeList<H, Ts...>>
		{
			using Result = TypeList<H, typename Erase<EraseType, TypeList<Ts...>>::Result>;
		};

		/*
			TypeList EraseAll Operator
		*/
		template <typename EraseType, typename TList>
		struct EraseAll;
		
		
		template <typename EraseType>
		struct EraseAll<EraseType, TypeList<>>
		{
			using Result = TypeList<>;
		};
		
		template <typename EraseType>
		struct EraseAll<EraseType, TypeList<EraseType>>
		{
			using Result = TypeList<>;
		};
		
		template <typename EraseType, typename... Ts>
		struct EraseAll<EraseType, TypeList<EraseType, Ts...>>
		{
			using Result = typename EraseAll<EraseType, TypeList<Ts...>>::Result;
		};

		template <typename EraseType, typename H, typename... Ts>
		struct EraseAll<EraseType, TypeList<H, Ts...>>
		{
			using Result = TypeList<H, typename EraseAll<EraseType, TypeList<Ts...>>::Result>;
		};

		/*
			TypeList Reverse Operator
		*/
		template<typename TList>
		struct Reverse;

		template<>
		struct Reverse<TypeList<>>
		{
			using Result = TypeList<>;
		};

		template<typename H, typename... Ts>
		struct Reverse<TypeList<H, Ts...>>
		{
			using Result = typename Append<H, typename Reverse<TypeList<Ts...>>::Result>::Result;
		};
	}
}