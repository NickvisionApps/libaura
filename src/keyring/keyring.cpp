#include "keyring/keyring.h"
#include "keyring/systemcredentials.h"

namespace Nickvision::Keyring
{
	Keyring::Keyring(const Store& store) noexcept
		: m_store{ store }
	{

	}

	const std::string& Keyring::getName() const noexcept
	{
		return m_store.getName();
	}

	std::vector<Credential> Keyring::getAllCredentials() const noexcept
	{
		return m_store.getAllCredentials();
	}

	std::optional<Credential> Keyring::getCredential(int id) const noexcept
	{
		return m_store.getCredential(id);
	}

	std::vector<Credential> Keyring::getCredentials(const std::string& name) const noexcept
	{
		return m_store.getCredentials(name);
	}

	bool Keyring::addCredential(const Credential& credential) noexcept
	{
		return m_store.addCredential(credential);
	}

	bool Keyring::updateCredential(const Credential& credential) noexcept
	{
		return m_store.updateCredential(credential);
	}

	bool Keyring::deleteCredential(int id) noexcept
	{
		return m_store.deleteCredential(id);
	}

	bool Keyring::destroy() noexcept
	{
		if (m_store.destroy())
		{
			SystemCredentials::deleteCredential(m_store.getName()); //keyring's password may not be in system credentials
			return true;
		}
		return false;
	}

	std::optional<Keyring> Keyring::access(const std::string& name, std::string password) noexcept
	{
		//If password empty, get password from system credential store
		if (password.empty())
		{
			std::optional<Credential> cred{ SystemCredentials::getCredential(name) };
			if (cred.has_value())
			{
				password = cred->getPassword();
			}
			else
			{
				cred = SystemCredentials::addCredential(name);
				password = cred ? cred->getPassword() : "";
			}
		}
		//If password not empty (a.k.a user-provided or system-provided), get store
		if (!password.empty())
		{
			try
			{
				return { { { name, password } } };
			}
			catch (...)
			{
				return std::nullopt;
			}
		}
		return std::nullopt;
	}

	bool Keyring::exists(const std::string& name) noexcept
	{
		return Store::exists(name);
	}

	bool Keyring::destroy(const std::string& name) noexcept
	{
		if (Store::destroy(name))
		{
			SystemCredentials::deleteCredential(name); //keyring's password may not be in system credentials
			return true;
		}
		return false;
	}
}