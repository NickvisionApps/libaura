#include "keyring/store.h"
#include "userdirectories.h"

using namespace SQLite;

namespace Nickvision::Aura::Keyring
{
	std::filesystem::path getPathFromName(const std::string& name)
	{
		return Store::getStoreDir() / (name + ".ring");
	}

	Store::Store(const std::string& name, std::unique_ptr<Database> database)
		: m_name{ name },
		m_database{ std::move(database) },
		m_path{ getPathFromName(name) }
	{
		m_database->exec("CREATE TABLE IF NOT EXISTS credentials (id TEXT PRIMARY KEY, name TEXT, uri TEXT, username TEXT, password TEXT)");
	}

	const std::string& Store::getName() const
	{
		return m_name;
	}

	const std::filesystem::path& Store::getPath() const
	{
		return m_path;
	}

	std::vector<Credential> Store::getAllCredentials() const
	{
		std::vector<Credential> creds;
		Statement query{ *m_database, "SELECT * FROM credentials" };
		while (query.executeStep())
		{
			creds.push_back({ query.getColumn(0).getInt(), query.getColumn(1).getString(), query.getColumn(2).getString(), query.getColumn(3).getString(), query.getColumn(4).getString() });
		}
		return creds;
	}

	std::optional<Credential> Store::getCredential(int id) const
	{
		Statement query{ *m_database, "SELECT * FROM credentials where id = ?" };
		query.bind(1, id);
		query.executeStep();
		if (query.hasRow())
		{
			return { { query.getColumn(0).getInt(), query.getColumn(1).getString(), query.getColumn(2).getString(), query.getColumn(3).getString(), query.getColumn(4).getString() } };
		}
		return std::nullopt;
	}

	std::vector<Credential> Store::getCredentials(const std::string& name) const
	{
		std::vector<Credential> creds;

		Statement query{ *m_database, "SELECT * FROM credentials where name = ?" };
		query.bind(1, name);
		while (query.executeStep())
		{
			creds.push_back({ query.getColumn(0).getInt(), query.getColumn(1).getString(), query.getColumn(2).getString(), query.getColumn(3).getString(), query.getColumn(4).getString() });
		}
		return creds;
	}

	bool Store::addCredential(const Credential& credential)
	{
		Statement query{ *m_database, "INSERT INTO credentials (id, name, uri, username, password) VALUES (?, ?, ?, ?, ?)" };
		query.bind(1, credential.getId());
		query.bind(2, credential.getName());
		query.bind(3, credential.getUri());
		query.bind(4, credential.getUsername());
		query.bind(5, credential.getPassword());
		return query.exec() > 0;
	}

	bool Store::updateCredential(const Credential& credential)
	{
		Statement query{ *m_database, "UPDATE credentials SET name = ?, uri = ?, username = ?, password = ? where id = ?" };
		query.bind(5, credential.getId());
		query.bind(1, credential.getName());
		query.bind(2, credential.getUri());
		query.bind(3, credential.getUsername());
		query.bind(4, credential.getPassword());
		return query.exec() > 0;
	}

	bool Store::deleteCredential(int id)
	{
		Statement query{ *m_database, "DELETE FROM credentials WHERE id = ?" };
		query.bind(1, id);
		return query.exec() > 0;
	}

	bool Store::destroy()
	{
		m_database.reset();
		return std::filesystem::remove(m_path);
	}

	std::filesystem::path Store::getStoreDir()
	{
		return UserDirectories::getConfig() / "Nickvision" / "Keyring";
	}

	std::optional<Store> Store::create(const std::string& name, const std::string& password, bool overwrite)
	{
		std::filesystem::create_directories(getStoreDir());
		if (name.empty() || password.empty())
		{
			return std::nullopt;
		}
		std::filesystem::path path{ getPathFromName(name) };
		if (std::filesystem::exists(path))
		{
			if (overwrite)
			{
				std::filesystem::remove(path);
			}
			else
			{
				return std::nullopt;
			}
		}
		try
		{
			std::unique_ptr<Database> database{ std::make_unique<Database>(path.string(), OPEN_READWRITE | OPEN_CREATE) };
			database->key(password);
			return { { name, std::move(database) } };
		}
		catch (...)
		{
			return std::nullopt;
		}
	}

	std::optional<Store> Store::load(const std::string& name, const std::string& password)
	{
		if (name.empty() || password.empty())
		{
			return std::nullopt;
		}
		std::filesystem::path path{ getPathFromName(name) };
		if (!std::filesystem::exists(path))
		{
			return std::nullopt;
		}
		try
		{
			std::unique_ptr<Database> database{ std::make_unique<Database>(path.string(), OPEN_READWRITE | OPEN_CREATE) };
			database->key(password);
			return { { name, std::move(database) } };
		}
		catch (...)
		{
			return std::nullopt;
		}
	}

	bool Store::exists(const std::string& name)
	{
		return std::filesystem::exists(getPathFromName(name));
	}

	bool Store::destroy(const std::string& name)
	{
		std::filesystem::path path{ getPathFromName(name) };
		if (std::filesystem::exists(path))
		{
			return std::filesystem::remove(path);
		}
		return false;
	}
}