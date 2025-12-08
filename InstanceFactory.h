#pragma once
#include "Instance.h"
#include <memory>
#include <vector>
#include <unordered_map>
#include <functional>
#include <type_traits>

using std::shared_ptr;
class InstanceFactory {
	enum class Instances : unsigned int {
		NONE = 0,
		QuadPart
	};
public:
	static InstanceFactory& get() { // STOP USING INLINE FOR STATIC FUNCS
		static InstanceFactory v;
		return v;
	}

	template <typename T>
	void registerTemplate(std::string key);

	template <typename instanceDerivedT>
	void registerTemplate(std::string key, std::unique_ptr<instanceDerivedT>&& pRT);

	template<typename instanceDerivedT>
	std::unique_ptr<instanceDerivedT> cloneTemplate(const std::string& key);

	InstanceFactory(const InstanceFactory&) = delete;
	InstanceFactory& operator=(const InstanceFactory&) = delete;
private:
	InstanceFactory() = default;
	std::unordered_map<std::string, std::unique_ptr<Instance>> mTemplateCloneCallbacks;
};


template<typename T>
inline void InstanceFactory::registerTemplate(std::string key) {
	static_assert(std::is_base_of_v<Instance, T>, "Attempted to register a non-Instance template?");
	mTemplateCloneCallbacks[std::move(key)] = std::make_unique<T>();
}

template<typename instanceDerivedT>
inline void InstanceFactory::registerTemplate(std::string key, std::unique_ptr<instanceDerivedT>&& pRT) {
	static_assert(std::is_base_of_v<Instance, instanceDerivedT>, "Attempted to register a non-Instance template?");
	mTemplateCloneCallbacks[std::move(key)] = std::move(pRT);
}

template<typename instanceDerivedT>
inline std::unique_ptr<instanceDerivedT> InstanceFactory::cloneTemplate(const std::string& key) {
	if(auto search = mTemplateCloneCallbacks.find(key); search != mTemplateCloneCallbacks.end()) {
		return std::unique_ptr<instanceDerivedT>(dynamic_cast<instanceDerivedT*>(search->second->clone().release()));
	}
	std::cout << "Could not locate register Instance template [" << key << "]\n";
	return nullptr;
}
