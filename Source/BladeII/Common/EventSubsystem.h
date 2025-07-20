#pragma once

#include <tuple>
#include <functional>
#include <type_traits>
#include <map>

#include "../TemplateUtility/Common.h"
//#include "MacroHelper.h"

DEFINE_LOG_CATEGORY_STATIC(LogEventSubsystem, Log, All);

template <typename... Ts>
class EventHandlerExecutor
{
private:
	std::function<void(Ts...)>	f;
	std::tuple<Ts...>			args;

	template <typename... Args, std::size_t... Is>
	void func(std::tuple<Args...>& tup, TU::index<Is...>)
	{
		f(std::get<Is>(tup)...);
	}

	template <typename... Args>
	void func(std::tuple<Args...>& tup)
	{
		func(tup, TU::generator_sequences<sizeof...(Args)>{});
	}

public:
	template <typename F>
	EventHandlerExecutor(F&& func, Ts&&... _args)
		: f(std::forward<F>(func)),
		args(std::tuple<Ts...>(std::forward<Ts>(_args)...))
	{
	}

	void operator ()()
	{
		func(args);
	}
};

class IEventUnsubscribe
{
public:
	virtual ~IEventUnsubscribe() {}
	virtual void Unsubscribe(const uint32) = 0;
};

class EventUnsubscriber
{
public:
	static EventUnsubscriber& GetInstance()
	{
		static EventUnsubscriber	inst;
		return inst;
	}

	void Regist(FString const& name, IEventUnsubscribe* i)
	{
		Depot.emplace(name, i);
	}

	void Unsubscribe(FString const& name, uint32 Id)
	{
		auto it = Depot.find(name);
		if (it == Depot.end())
			return;

		it->second->Unsubscribe(Id);
	}

private:
	EventUnsubscriber()
	{
	}
	~EventUnsubscriber()
	{
		Depot.clear();
	}
	EventUnsubscriber(EventUnsubscriber const&) = delete;
	EventUnsubscriber& operator = (EventUnsubscriber const&) = delete;

	std::map<FString, IEventUnsubscribe*>		Depot;
};

class issue
{
public:
	explicit issue(FString const& pub, uint32 no) : publisher(pub), num(no)
	{
	}

	~issue()
	{
		EventUnsubscriber::GetInstance().Unsubscribe(publisher, num);
	}

private:
	FString		publisher;
	uint32		num;
};

using issue_ptr = std::shared_ptr<issue>;

#define DECLARE_EVENT_CLASS(EVENTNAME, ...)																										\
	template<typename... Args>																													\
	class EVENTNAME##Class{																														\
	};																																			\
																																				\
	template<>																																	\
	class EVENTNAME##Class<__VA_ARGS__> : public IEventUnsubscribe																				\
	{																																			\
	public:																																		\
		using EVENTNAME##HandlerType = std::function<void(__VA_ARGS__)>;																		\
		using EVENTNAME##ArgumentType = std::tuple<__VA_ARGS__>;																				\
																																				\
		static EVENTNAME##Class& GetInstance();																									\
																																				\
		template <typename F>																													\
		const uint32 Subscribe(F&& func)																										\
		{																																		\
			++identify_key_;																													\
			subscribers_.emplace(identify_key_, std::forward<F>(func));																			\
			return identify_key_;																												\
		}																																		\
																																				\
		template <typename F>																													\
		issue_ptr Subscribe2(F&& func)																											\
		{																																		\
			++identify_key_;																													\
			subscribers_.emplace(identify_key_, std::forward<F>(func));																			\
			return std::make_shared<issue>(TEXT(#EVENTNAME), identify_key_);																	\
		}																																		\
																																				\
		void Unsubscribe(const uint32 id)																										\
		{																																		\
			auto it = subscribers_.find(id);																									\
			auto eit = subscribers_.end();																										\
																																				\
			if (it == eit)																														\
				return;																															\
																																				\
			if (is_signalling_)																													\
			{																																	\
				have_pending_unsubscribe_requests_ = true;																						\
				pending_unsubscribe_tickets_.push_back(id);																						\
				return;																															\
			}																																	\
			subscribers_.erase(it);																												\
		}																																		\
																																				\
		void UnsubscribeAll()																													\
		{																																		\
			if (is_signalling_)																													\
			{																																	\
				have_pending_unsubscribe_requests_ = true;																						\
				pending_unsubscribe_all_ = true;																								\
				return;																															\
			}																																	\
			subscribers_.clear();																												\
		}																																		\
																																				\
		template <typename... Args>																												\
		void Signal(Args&&... args)																												\
		{																																		\
			if (subscribers_.empty())																											\
				return;																															\
			static_assert(std::tuple_size<EVENTNAME##ArgumentType>::value == sizeof...(Args),													\
						"Arguments signature is different of event signature.");																\
			is_signalling_ = true;																												\
			for (auto& elem : subscribers_)																										\
			{																																	\
				EventHandlerExecutor<Args...>(elem.second, std::forward<Args>(args)...)();														\
			}																																	\
			is_signalling_ = false;																												\
			if (have_pending_unsubscribe_requests_)																								\
				ClearPendingUnsubscribeRequests();																								\
		}																																		\
																																				\
		private:																																\
		EVENTNAME##Class() : is_signalling_(false), pending_unsubscribe_all_(false), have_pending_unsubscribe_requests_(false)					\
		{																																		\
		}																																		\
		~EVENTNAME##Class() {																													\
			subscribers_.clear();																												\
			pending_unsubscribe_tickets_.clear();																								\
		}																																		\
																																				\
		EVENTNAME##Class(EVENTNAME##Class const&) = delete;																						\
		EVENTNAME##Class& operator = (EVENTNAME##Class const&)	= delete;																		\
																																				\
		void ClearPendingUnsubscribeRequests()																									\
		{																																		\
			if(pending_unsubscribe_all_)																										\
				UnsubscribeAll();																												\
			else																																\
			{																																	\
				for (auto id : pending_unsubscribe_tickets_)																					\
				{																																\
					Unsubscribe(id);																											\
				}																																\
			}																																	\
			pending_unsubscribe_tickets_.clear();																								\
			pending_unsubscribe_all_			= false;																						\
			have_pending_unsubscribe_requests_	= false;																						\
		}																																		\
																																				\
		std::map<uint32, EVENTNAME##HandlerType>	subscribers_;																				\
		std::vector<uint32>							pending_unsubscribe_tickets_;																\
		bool										is_signalling_;																				\
		bool										pending_unsubscribe_all_;																	\
		bool										have_pending_unsubscribe_requests_;															\
		static uint32								identify_key_;																				\
	};

#define DEFINITION_EVENT_CLASS(EVENTNAME, ...)																									\
	uint32 EVENTNAME##Class<__VA_ARGS__>::identify_key_ = 0;																					\
	EVENTNAME##Class<__VA_ARGS__>& EVENTNAME##Class<__VA_ARGS__>::GetInstance()																	\
	{																																			\
		static EVENTNAME##Class<__VA_ARGS__>	inst;																							\
		static bool IsSubscribed = false;																										\
		if(!IsSubscribed)																														\
		{																																		\
			EventUnsubscriber::GetInstance().Regist(#EVENTNAME,&inst);																			\
			IsSubscribed = true;																												\
		}																																		\
		return inst;																															\
	}																																	


#define CAPTURE_UOBJECT(CLASSNAME)																												\
	TWeakObjectPtr<CLASSNAME>	Capture(this)

#define USE_CAPTURE_OBJECT(...)																													\
	[Capture](__VA_ARGS__)																														\
	{																																			\
		if (!Capture.IsValid())																													\
		{																																		\
			BII_CHECK(false);																													\
		}

#define USE_CAPTURE_OBJECT_AND_TICKET(EVENTNAME, ...)																							\
	[Capture](__VA_ARGS__)																														\
	{																																			\
		if (!Capture.IsValid())																													\
		{																																		\
			if(Capture.IsValid(true))																											\
			{																																	\
				EventUnsubscriber::GetInstance().Unsubscribe(#EVENTNAME, Capture->EVENTNAME##Ticket);											\
				UE_LOG(LogEventSubsystem, Warning, TEXT("Called Event to capture invalid object - Event Name is %s"), TEXT(#EVENTNAME));		\
				return;																															\
			}																																	\
			else																																\
			{																																	\
				UE_LOG(LogEventSubsystem, Warning, TEXT("Called Event to capture already cleaned object - Event Name is %s"), TEXT(#EVENTNAME));\
				return;																															\
			}																																	\
		}

#define END_CAPTURE_OBJECT()																													\
	}