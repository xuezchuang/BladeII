#pragma once

#include <cstddef>
#include <cstring>

#define MAP(macro, ...) \
    IDENTITY(APPLY(CHOOSE_MAP_START, COUNT(__VA_ARGS__)) (macro, __VA_ARGS__))

#define CHOOSE_MAP_START(count) MAP ## count

#define APPLY(macro, ...) IDENTITY(macro(__VA_ARGS__))

// Needed to expand __VA_ARGS__ "eagerly" on the MSVC preprocessor.
#define IDENTITY(x) x

#define MAP1(m, x)      m(x)
#define MAP2(m, x, ...) m(x) IDENTITY(MAP1(m, __VA_ARGS__))
#define MAP3(m, x, ...) m(x) IDENTITY(MAP2(m, __VA_ARGS__))
#define MAP4(m, x, ...) m(x) IDENTITY(MAP3(m, __VA_ARGS__))
#define MAP5(m, x, ...) m(x) IDENTITY(MAP4(m, __VA_ARGS__))
#define MAP6(m, x, ...) m(x) IDENTITY(MAP5(m, __VA_ARGS__))
#define MAP7(m, x, ...) m(x) IDENTITY(MAP6(m, __VA_ARGS__))
#define MAP8(m, x, ...) m(x) IDENTITY(MAP7(m, __VA_ARGS__))
#define MAP9(m, x, ...) m(x) IDENTITY(MAP8(m, __VA_ARGS__))
#define MAP10(m, x, ...) m(x) IDENTITY(MAP9(m, __VA_ARGS__))
#define MAP11(m, x, ...) m(x) IDENTITY(MAP10(m, __VA_ARGS__))
#define MAP12(m, x, ...) m(x) IDENTITY(MAP11(m, __VA_ARGS__))

#define EVALUATE_COUNT(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, count, ...) count

#define COUNT(...) \
    IDENTITY(EVALUATE_COUNT(__VA_ARGS__, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1))

struct ignore_assign 
{
	ignore_assign(uint8 value) : _value(value) { } 
	operator uint8() const { return _value; }

	const ignore_assign& operator =(uint8 dummy) { return *this; }

	uint8 _value;
};

#define IGNORE_ASSIGN_SINGLE(expression) (ignore_assign)expression,
#define IGNORE_ASSIGN(...) IDENTITY(MAP(IGNORE_ASSIGN_SINGLE, __VA_ARGS__))

#define STRINGIZE_SINGLE(expression) TEXT(#expression),
#define STRINGIZE(...) IDENTITY(MAP(STRINGIZE_SINGLE, __VA_ARGS__))


#define ENUM(EnumName, ...)                                                                 \
struct EnumName                                                                              \
{                                                                                            \
    enum _enumerated { __VA_ARGS__ };                                                        \
                                                                                             \
    _enumerated _value;                                                                      \
                                                                                             \
    EnumName(_enumerated value) : _value(value) {}                                           \
    EnumName(uint8 value) : _value(static_cast<_enumerated>(value)) {}                       \
    operator _enumerated() const { return _value; }                                          \
                                                                                             \
    const TCHAR* ToString() const                                                            \
    {                                                                                        \
        for (size_t index = 0; index < _count; ++index)                                      \
        {                                                                                    \
            if (_values()[index] == _value)                                                  \
                return _names()[index];                                                      \
        }                                                                                    \
        return nullptr;                                                                      \
    }                                                                                        \
                                                                                             \
    static const size_t _count = IDENTITY(COUNT(__VA_ARGS__));                               \
                                                                                             \
    static const uint8* _values()                                                            \
    {                                                                                        \
        static const uint8 values[] = { IDENTITY(IGNORE_ASSIGN(__VA_ARGS__)) };              \
        return values;                                                                       \
    }                                                                                        \
                                                                                             \
    static const TCHAR* const* _names()                                                      \
    {                                                                                        \
        static const TCHAR* const raw_names[] = { IDENTITY(STRINGIZE(__VA_ARGS__)) };        \
        static TCHAR* processed_names[_count];                                               \
        static bool initialized = false;                                                     \
                                                                                             \
        if (!initialized)                                                                    \
        {                                                                                    \
            /* 可选：如果需要线程安全，可改成 std::once_flag + call_once */                \
            for (size_t index = 0; index < _count; ++index)                                  \
            {                                                                                \
                const TCHAR* src = raw_names[index];                                         \
                size_t length = 0;                                                           \
                while (src[length]                                                           \
                    && src[length] != TEXT(' ')                                              \
                    && src[length] != TEXT('=')                                              \
                    && src[length] != TEXT('\t')                                             \
                    && src[length] != TEXT('\n')                                             \
                    && src[length] != TEXT('\r'))                                            \
                {                                                                            \
                    ++length;                                                                \
                }                                                                            \
                                                                                             \
                TCHAR* dst = new TCHAR[length + 1];                                          \
                if (length > 0)                                                              \
                {                                                                            \
                    FMemory::Memcpy(dst, src, length * sizeof(TCHAR));                       \
                }                                                                            \
                dst[length] = 0;                                                             \
                processed_names[index] = dst;                                                \
            }                                                                                \
            initialized = true;                                                              \
        }                                                                                    \
        return processed_names;                                                              \
    }                                                                                        \
                                                                                             \
    bool operator==(const EnumName& other) const { return _value == other._value; }          \
    bool operator!=(const EnumName& other) const { return _value != other._value; }          \
};                                                                                           \
                                                                                             \
inline uint32 GetTypeHash(const EnumName& e)                                                 \
{                                                                                            \
    return static_cast<uint32>(e._value);                                                    \
}
