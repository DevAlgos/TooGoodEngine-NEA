#pragma once

#include "DenseMap.h"
#include "Entity.h"

namespace TooGoodEngine {

	class Registry
	{
	public:
		Registry() = default;
		~Registry() = default;

		inline Entity CreateEntity(std::string_view name) { return Entity(name, m_Count++); }
		inline const size_t GetCount() const { return (size_t)m_Count; }
		
		template<typename T>
		inline bool HasComponent(const Entity& entity) { return m_Buckets[typeid(T).name()].Contains<T>(entity); }
	
		template<typename T>
		void AddComponent(const Entity& entity, const T& t);

		template<typename T, typename ...Args>
		void EmplaceComponent(const Entity& entity, Args&&... args);

		template<typename T>
		void RemoveComponent(const Entity& entity);

		template<typename T>
		T& GetComponent(const Entity& entity);

		template<typename T, typename Fun>
		void ForEach(Fun fun);

		template<typename T>
		MemoryAllocator::VariableIterator<T> View();

	private:
		void _VerifyEntity(const Entity& entity) const;

	private:
		EntityID m_Count = 0;
		std::unordered_map<std::string, DenseMap> m_Buckets; //string identifier
	};


	template<typename T>
	inline void Registry::AddComponent(const Entity& entity, const T& t)
	{
		_VerifyEntity(entity);
		m_Buckets[typeid(T).name()].Add<T>(entity, t);
	}

	template<typename T, typename ...Args>
	inline void Registry::EmplaceComponent(const Entity& entity, Args&&... args)
	{
		_VerifyEntity(entity);
		m_Buckets[typeid(T).name()].Emplace<T>(entity, std::forward<Args>(args)...);
	}

	template<typename T>
	inline void Registry::RemoveComponent(const Entity& entity)
	{
		_VerifyEntity(entity);
		m_Buckets[typeid(T).name()].Remove<T>(entity);
	}

	template<typename T>
	inline T& Registry::GetComponent(const Entity& entity)
	{
		_VerifyEntity(entity);
		return m_Buckets[typeid(T).name()].Get<T>(entity);
	}

	template<typename T, typename Fun>
	inline void Registry::ForEach(Fun fun)
	{
		m_Buckets[typeid(T).name()].ForEach<T>(fun);
	}

	template<typename T>
	inline MemoryAllocator::VariableIterator<T> Registry::View()
	{
		return m_Buckets[typeid(T).name()].ViewDense<T>();
	}

}

