#pragma once
#include <memory>
#include <map>
#include <string>

template<class TResource>

class TResourceBank {
public:
	TResource* Get(const char* filePath)
	{
		auto it = m_resourceMap.find(filePath);
		if (it == m_resourceMap.end()) {
			m_resourceMap.insert(
				std::pair< std::string, TResourcePtr>(filePath, resource)
			);
		}
	}


	void Regist(const char* filePath, TResource* resource)
	{
		auto it = m_resourceMap.find(filePath);
		if (it == m_resourceMap.end()) {
			m_resourceMap.insert(
				std::pair< std::string, TResourcePtr>(filePath, resource)
			);
		}
	}
private:
	using TResourcePtr = std::unique_ptr<TResource>;
	std::map<std::string, TResourcePtr> m_resourceMap;
};