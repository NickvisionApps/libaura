#include <gtest/gtest.h>
#include <mutex>
#include "aura.h"
#include "interprocesscommunicator.h"

using namespace Nickvision::Aura;
using namespace Nickvision::Aura::Events;

static std::vector<std::string> args{ "test1", "test2" };

class IPCTest : public testing::Test
{
public:
	static std::unique_ptr<InterProcessCommunicator> m_server;

	static void SetUpTestSuite()
	{
		Aura::init("org.nickvision.aura.test", "Nickvision Aura Tests");
		m_server = std::make_unique<InterProcessCommunicator>();
		m_server->commandReceived() += onCommandReceived;
	}

	static int getReceived()
	{
		std::lock_guard<std::mutex> lock{ m_mutex };
		return m_received;
	}

private:
	static std::mutex m_mutex;
	static int m_received;

	static void onCommandReceived(const ParamEventArgs<std::vector<std::string>>& e)
	{
		std::lock_guard<std::mutex> lock{ m_mutex };
		m_received += (e.getParam()[0] == args[0] && e.getParam()[1] == args[1] ? 1 : 0);
	}
};

std::mutex IPCTest::m_mutex = {};
int IPCTest::m_received = 0;
std::unique_ptr<InterProcessCommunicator> IPCTest::m_server = nullptr;

TEST_F(IPCTest, CheckServerStatus)
{
	ASSERT_TRUE(m_server->isServer());
}

TEST_F(IPCTest, Client1Send)
{
#ifdef _WIN32
	if (Aura::getEnvVar("GITHUB_ACTIONS") == "true")
	{
		ASSERT_TRUE(true);
		return;
	}
#endif
	InterProcessCommunicator client;
	ASSERT_TRUE(client.isClient());
	ASSERT_TRUE(client.communicate(args));
}

TEST_F(IPCTest, CheckServerReceived)
{
#ifdef _WIN32
	ASSERT_TRUE(Aura::getEnvVar("GITHUB_ACTIONS") == "true" || getReceived() > 0);
#elif defined(__linux__)
	ASSERT_TRUE(getReceived() > 0);
#endif
}

TEST_F(IPCTest, Cleanup)
{
	ASSERT_NO_THROW(m_server.reset());
}