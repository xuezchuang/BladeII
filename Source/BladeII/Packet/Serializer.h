#pragma once

//#include "Base64.h"
//#include "Array.h"
//#include "UnrealString.h"
#include "BladeIIPlayer.h"

#define DEFINITION_SERIALIZER(type)																						\
	static																												\
	ByteStream& operator << (ByteStream& stream, type value)															\
	{																													\
		auto	size = sizeof(type);																					\
		auto*	ptr = reinterpret_cast<uint8*>(&value);																	\
																														\
		auto	old_array_size = stream.PrepareUninitialized(size);														\
		auto*	start = reinterpret_cast<void*>(stream.get_data() + old_array_size);									\
																														\
		FMemory::Memcpy(start, ptr, size);																				\
																														\
		return stream;																									\
	}

namespace packet 
{
	class ByteStream
	{
	public:
		ByteStream()
		{

		}

		explicit ByteStream(const FString& stream)
		{
			FBase64::Decode(stream, Stream);
		}

		explicit ByteStream(TArray<uint8> const& stream)
		{
			Stream = stream;
		}

		~ByteStream()
		{
			Stream.Empty();
		}

		int32 PrepareUninitialized(int32 size)
		{
			return Stream.AddUninitialized(size);
		}

		uint8 Pop()
		{
			Stream.RangeCheck(0);
			uint8 Result = MoveTemp(Stream.GetData()[0]);
			Stream.RemoveAt(0, 1, true);
			return Result;
		}

		uint32 size() const
		{
			return Stream.Num();
		}

		uint8* get_data()
		{
			return Stream.GetData();
		}

		operator FString() const
		{
			return FBase64::Encode(Stream);
		}

	private:
		TArray<uint8>	Stream;
	};

	DEFINITION_SERIALIZER(bool)

	DEFINITION_SERIALIZER(uint8)
	DEFINITION_SERIALIZER(uint16)
	DEFINITION_SERIALIZER(uint32)
	DEFINITION_SERIALIZER(uint64)

	DEFINITION_SERIALIZER(int8)
	DEFINITION_SERIALIZER(int16)
	DEFINITION_SERIALIZER(int32)
	DEFINITION_SERIALIZER(int64)

	DEFINITION_SERIALIZER(float)

	DEFINITION_SERIALIZER(EPCClass)
	DEFINITION_SERIALIZER(EAttackState)
	DEFINITION_SERIALIZER(ESkillAnimType)
	DEFINITION_SERIALIZER(ETagAttackMode)

	static
	ByteStream& operator << (ByteStream& stream, FGuid& value)
	{
		stream << value.A << value.B << value.C << value.D;

		return stream;
	}

	static
	ByteStream& operator << (ByteStream& stream, ByteStream& value)
	{
		auto	size	= value.size();
		auto*	ptr		= value.get_data();

		// firstly, write ByteStream size.
		stream << size;

		auto	old_array_size = stream.PrepareUninitialized(size);
		auto*	start = reinterpret_cast<void*>(stream.get_data() + old_array_size);

		FMemory::Memcpy(start, ptr, size);

		return stream;
	}

	static
	ByteStream& operator << (ByteStream& stream, FString& value)
	{
		uint32	size = value.Len() * sizeof(TCHAR);
		auto*	ptr = reinterpret_cast<void*>(value.GetCharArray().GetData());

		if (!size)
		{
			stream << size;
			return stream;
		}

		// firstly, write ByteStream size.
		stream << size;

		auto	old_array_size = stream.PrepareUninitialized(size);
		auto*	start = reinterpret_cast<void*>(stream.get_data() + old_array_size);

		FMemory::Memcpy(start, ptr, size);

		return stream;
	}

	template<typename T>
	T SliceStream(ByteStream& stream)
	{
		const auto	size = sizeof(T);

		uint8	buffer[size];
#if PLATFORM_LITTLE_ENDIAN
		for (int32 i = 0; i != size; ++i)
#else
		for (int32 i = size - 1; i != -1 ; --i)
#endif
		{
			buffer[i] = stream.Pop();
		}

		return *(reinterpret_cast<T*>(buffer));
	}

	static FString SliceStreamToString(ByteStream& stream)
	{
		auto	size = SliceStream<uint32>(stream);

		TArray<uint8> tmp;
#if PLATFORM_LITTLE_ENDIAN
		for (int32 i = 0; i != size; ++i)
#else
		for (int32 i = size - 1; i != -1; --i)
#endif
		{
			tmp.Add(stream.Pop());
		}

		FString Ret(size/sizeof(TCHAR), reinterpret_cast<TCHAR*>(tmp.GetData()));
		return Ret;
	}

	static ByteStream SliceStream(ByteStream& stream)
	{
		auto	size = SliceStream<uint32>(stream);

		TArray<uint8>	tmp;
#if PLATFORM_LITTLE_ENDIAN
		for (int32 i = 0; i != size; ++i)
#else
		for (int32 i = size - 1; i != -1; --i)
#endif
		{
			tmp.Add(stream.Pop());
		};

		return ByteStream(tmp);
	}
}