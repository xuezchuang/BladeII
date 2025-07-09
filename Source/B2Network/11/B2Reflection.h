#pragma once

#include <memory>
#include <map>
#include <unordered_map>
#include <vector>
#include <algorithm>

//#include "../BladeII/Public/MacroHelper.h"
//#include "../BladeII/Public/TemplateUtility/TypeList.h"
//#include "MacroHelper.h"
#include "TemplateUtility/TypeList.h"

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wreorder"	// warning : field 'x' will be initialized after field 'y' [-Wreorder]
#endif

namespace reflection
{
	template <typename _Ty>
	class TypeInfo {};

	class B2NETWORK_API Property
	{
	public:
		template <typename _ObjTy, typename _FieldTy>
		Property(_FieldTy(_ObjTy::*Field), std::string _name)
			: PropertyName(_name), inf(new InternalImpl<_ObjTy, _FieldTy>(Field))
		{
		}
        
        virtual ~Property() {};

		const char* name() const { return PropertyName.c_str(); }

		template <typename _FieldTy>
		_FieldTy& get_ref(void* obj) const
		{
			_FieldTy* address = reinterpret_cast<_FieldTy*>(inf->ptr(obj));
			return *address;
		}

	private:
		struct Interface {
            virtual ~Interface() {};
			virtual void* ptr(void* obj_addr) const = 0;
		};

		template <typename _ObjTy, typename _FieldTy>
		struct InternalImpl : public Interface
		{
			InternalImpl(_FieldTy(_ObjTy::*Field)) : field(Field) {}
            virtual ~InternalImpl() {};

			void* ptr(void* obj_addr) const
			{
				_ObjTy* obj = reinterpret_cast<_ObjTy*>(obj_addr);
				return &(obj->*field);
			}
			_FieldTy(_ObjTy::*field);
		};

		std::shared_ptr<Interface> inf;
		std::string PropertyName;
	};

	class B2NETWORK_API ProtoClass
	{
	public:
		using field_map_t = std::vector<std::pair<std::string, Property*>>;

		template <typename _Ty>
		inline ProtoClass(const TypeInfo<_Ty>&, std::string _name)
			: class_name(_name), inf(new InternalImpl<_Ty>)
		{
		}
        virtual ~ProtoClass(){};

		static const ProtoClass* from_name(std::string name);

		const char* name() const
		{
			return class_name.c_str();
		}

		inline const Property* get_field(std::string name) const
		{
			for (auto p : field_map)
			{
				if (p.first != name)
					continue;

				return p.second;
			}

			return nullptr;
		}

		template <typename _Class>
		inline _Class* new_instance() const
		{
			return reinterpret_cast<_Class*>(inf->new_instance(class_name));
		}

		inline void add_field(std::string name, Property* field)
		{
			field_map.push_back(std::make_pair(name, field));
		}

		template <typename _Func>
		inline void enumerate_fields(_Func&& func) const
		{
			std::for_each(field_map.begin(), field_map.end(), [&](field_map_t::value_type pair) {
				func(pair.second);
			});
		}

		inline std::size_t get_field_num() const
		{
			return field_map.size();
		}

		field_map_t::const_iterator begin() const { return field_map.cbegin(); }
		field_map_t::const_iterator end()   const { return field_map.cend(); }

	private:
		struct Interface
		{
            virtual ~Interface(){};
			virtual void* new_instance(const std::string& name) const = 0;
		};

		template <typename _Ty>
		struct InternalImpl : public Interface
		{
            virtual ~InternalImpl(){};
			virtual void* new_instance(const std::string& name) const { return new _Ty(); }
		};

		std::shared_ptr<Interface>	inf;
		std::string					class_name;

		field_map_t					field_map;
	};

	class B2NETWORK_API ReflectionHandler
	{
	public:
		static ReflectionHandler& instance();

		inline const ProtoClass* get_class(std::string name) const
		{
			auto class_iter = NameStaticClassMap.find(name);
			if (class_iter == NameStaticClassMap.end())
				return nullptr;
			return class_iter->second;
		}

		inline const Property* get_field(std::string class_name, std::string PropertyName) const
		{
			return get_class(class_name)->get_field(PropertyName);
		}

		inline void add_class(std::string name, ProtoClass* class_impl)
		{
			auto result = NameStaticClassMap.insert(std::make_pair(name, class_impl));
		}

		inline void add_field(std::string class_name, std::string PropertyName, Property* field_impl)
		{
			NameStaticClassMap[class_name]->add_field(PropertyName, field_impl);
		}
        
        virtual ~ReflectionHandler(){};

	private:
		using class_map_t = std::unordered_map<std::string, ProtoClass*>;

		ReflectionHandler() {}
		ReflectionHandler(const ReflectionHandler&) = delete;
		ReflectionHandler& operator= (const ReflectionHandler&) = delete;

		class_map_t			NameStaticClassMap;
	};

	class B2NETWORK_API ReflectionBase
	{
	public:
		ReflectionBase(const std::string& name) : class_name(name)	{}
        virtual ~ReflectionBase(){};

		inline const ProtoClass* get_class() const
		{
			return ProtoClass::from_name(class_name);
		}

	private:
		std::string		class_name;
	};

	// register helper
	template <typename _ObjTy>
	struct ReflectionRegisterHelper
	{
		using target_type = _ObjTy;

		static void register_class(std::string class_name);
		template <typename _FieldTy>
		static void register_field(std::string class_name, _FieldTy(_ObjTy::*Field), std::string PropertyName);
	};

	template <typename _ObjTy>
	inline void ReflectionRegisterHelper<_ObjTy>::register_class(std::string class_name)
	{
		ReflectionHandler::instance().add_class(class_name, new ProtoClass(TypeInfo<_ObjTy>(), class_name));
	}

	template <typename _ObjTy>
	template <typename _FieldTy>
	inline void ReflectionRegisterHelper<_ObjTy>::register_field(std::string class_name, _FieldTy(_ObjTy::*Field), std::string PropertyName)
	{
		if (class_name.empty())
			return;

		ReflectionHandler::instance().add_field(class_name, PropertyName, new Property(Field, PropertyName));
	}
}

class B2NETWORK_API Reflection : public reflection::ReflectionBase {
public:
	Reflection(const std::string& name) : reflection::ReflectionBase(name)
	{
	}

    virtual ~Reflection(){};
};

using ReflectionPtr = std::shared_ptr<Reflection>;

#define ADD_TO_TYPELIST(COUNT,INSERTTYPE)

#define FINALIZE_TYPELIST(COUNT) \
using type_list = TU::TypeList<>;


//#define ADD_TO_TYPELIST(COUNT,INSERTTYPE)	\
//	using __CONCAT2(type_list, INC(COUNT)) = TU::TL::Append<INSERTTYPE, type_list##COUNT>::Result;
//
//#define FINALIZE_TYPELIST(COUNT) \
//	using type_list = __CONCAT2(type_list, COUNT);

using ReflectionPtr = std::shared_ptr<Reflection>;

#define DECLARE_BEGIN(class_name)								\
	struct class_name;											\
	using  class_name##Ptr = std::shared_ptr<class_name>;		\
	struct class_name : public Reflection						\
	{															\
		class_name() :											\
		Reflection(#class_name)									\
		{														\
		}														\
        virtual ~class_name(){};                                \
		using type_list0 = TU::TypeList<>;

#define DECLARE_DF_FIELD(count, type, PropertyName)				\
		type PropertyName;										\
		enum {PropertyName##_index = count};					\
		ADD_TO_TYPELIST(count, type)	

#define DECLARE_END(count)										\
		FINALIZE_TYPELIST(count)								\
		static const uint32 field_num = count;					\
	};	

#define REFLECTION_REGISTER_BEGIN(class_name)																			\
	static struct _register_##class_name : public reflection::ReflectionRegisterHelper<class_name>						\
	{																													\
		_register_##class_name()																						\
		{																												\
			std::string strClassName = #class_name;																		\
			register_class(strClassName);																				\
			const uint32 field_num = class_name::field_num;
	
#define REFLECTION_REGISTER_FIELD(type, PropertyName)																	\
			register_field<type>(strClassName, &target_type::PropertyName, #PropertyName);	

#define REFLECTION_REGISTER_END(count)																					\
			static_assert(count == field_num, "different number of field from declartion");								\
		}																												\
	} __AUTO_NAME;

template<typename Tuple, std::size_t N>
struct CopyFromStructToTuple
{
	using NthType = typename std::tuple_element<N - 1, typename std::remove_reference<Tuple>::type>::type;

	template<typename It>
	static It Copy(Tuple& t, const reflection::ProtoClass* s, ReflectionPtr obj, It begin, It end)
	{
		It next = CopyFromStructToTuple<Tuple, N - 1>::Copy(t, s, obj, begin, end);
		auto pair = *next;
		std::get<N - 1>(t) = s->get_field(pair.second->name())->template get_ref<NthType>(obj.get());
		return ++next;
	}
};

template<typename Tuple>
struct CopyFromStructToTuple<Tuple, 1>
{
	using NthType = typename std::tuple_element<0, typename std::remove_reference<Tuple>::type>::type;

	template<typename It>
	static It Copy(Tuple& t, const reflection::ProtoClass* s, ReflectionPtr obj, It begin, It end)
	{
		auto pair = *begin;
		std::get<0>(t) = s->get_field(pair.second->name())->template get_ref<NthType>(obj.get());
		return ++begin;
	}
};

template <typename... Args, typename It>
void CopyFromStruct(std::tuple<Args...>& t, const reflection::ProtoClass* s, ReflectionPtr obj, It begin, It end)
{
	checkf(s->get_field_num() == sizeof...(Args), TEXT("must has the same number of arguments between tuple_size and reflection."));
	CopyFromStructToTuple<decltype(t), sizeof...(Args)>::Copy(t, s, obj, begin, end);
}

template<typename Tuple, std::size_t N>
struct CopyFromTupleToStruct
{
	using NthType = typename std::tuple_element<N - 1, typename std::remove_reference<Tuple>::type>::type;

	template<typename It>
	static It Copy(Tuple& t, const reflection::ProtoClass* s, ReflectionPtr obj, It begin, It end)
	{
		It next = CopyFromTupleToStruct<Tuple, N - 1>::Copy(t, s, obj, begin, end);
		auto pair = *next;
		s->get_field(pair.second->name())->template get_ref<NthType>(obj.get()) = std::get<N - 1>(t);
		return ++next;
	}
};

template<typename Tuple>
struct CopyFromTupleToStruct<Tuple, 1>
{
	using NthType = typename std::tuple_element<0, typename std::remove_reference<Tuple>::type>::type;

	template<typename It>
	static It Copy(Tuple& t, const reflection::ProtoClass* s, ReflectionPtr obj, It begin, It end)
	{
		auto pair = *begin;
		s->get_field(pair.second->name())->template get_ref<NthType>(obj.get()) = std::get<0>(t);
		return ++begin;
	}
};

template <typename... Args, typename It>
void CopyFromTuple(std::tuple<Args...>& t, const reflection::ProtoClass* s, ReflectionPtr obj, It begin, It end)
{
	checkf(s->get_field_num() == sizeof...(Args), TEXT("must has the same number of arguments between tuple_size and reflection."));
	CopyFromTupleToStruct<decltype(t), sizeof...(Args)>::Copy(t, s, obj, begin, end);
}

#ifdef __clang__
#pragma clang diagnostic pop
#endif
