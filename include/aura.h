#ifndef AURA_H
#define AURA_H

#include <map>
#include <optional>
#include <string>
#include "appinfo.h"
#include "configurationbase.h"

namespace Nickvision::Aura
{
	/**
	 * @brief An application base
	 */
	class Aura
	{
	public:
		/**
		 * @brief Destructs an Aura object.
		 */
		~Aura();
		/**
		 * @brief Gets the AppInfo object for the application.
		 */
		AppInfo& getAppInfo();
		/**
		 * @brief Gets a config object.
		 * @tparam T Derived type of ConfigurationBase
		 * @param key The key of the config file
		 * @return The config object
		 */
		template<typename T>
		T& getConfig(const std::string& key)
		{
			static_assert(std::is_base_of_v<ConfigurationBase, T> == true, "T must derive from ConfigurationBase");
			if (!m_configFiles.contains(key))
			{
				m_configFiles[key] = new T(key);
			}
			return *((T*)m_configFiles[key]);
		}

	private:
		Aura(const std::string& id, const std::string& name);
		AppInfo m_appInfo;
		std::map<std::string, ConfigurationBase*> m_configFiles;

	public:
		/**
		 * @brief Initializes Aura.
		 * @param id The application id
		 * @param name The application name
		 * @return The active aura instance
		 */
		static Aura& init(const std::string& id, const std::string& name);
		/**
		 * @brief Gets the active aura instance. Aura::init() must have been called first.
		 * @throw std::logic_error Thrown if Aura::init() was not yet called
		 * @return The active aura instance
		 */
		static Aura& getActive();

	private:
		static std::optional<Aura> m_instance;
	};
}

#endif //AURA_H