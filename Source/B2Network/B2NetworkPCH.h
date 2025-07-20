#pragma once

// ---- Silence legacy C++17 deprecation warnings (must be BEFORE any STL / protobuf includes) ----
#ifndef _SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING
#  define _SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING
#endif
#ifndef _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
#  define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
#endif

// (Optional) If you still see MSVC C4996 for other reasons, you can also selectively disable:
// #pragma warning(disable: 4996)

// ---- Core Unreal minimal set ----
#include "CoreMinimal.h"

// (Optional) If you use logging categories frequently you can include more core headers here.
// #include "Containers/Array.h"
// #include "Misc/DateTime.h"

// ---- Third-party forward includes (keep light; avoid heavy headers here if not needed) ----
// Forward include a minimal protobuf header to ensure macros apply before its internals.
#include <google/protobuf/repeated_field.h>
// If you need more, you can add:
// #include <google/protobuf/message.h>

// ---- Your own common public headers (optional) ----
// #include "B2Network/YourCommonHeader.h"

// ---- Verify macros active (remove after确认) ----
#if defined(_SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING)
#  pragma message("[B2NetworkPCH] _SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING active")
#endif
#if defined(_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS)
#  pragma message("[B2NetworkPCH] _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS active")
#endif
