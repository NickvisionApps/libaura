#include <gtest/gtest.h>
#include "aura/aura.h"
#include "filesystem/userdirectories.h"
#include "notifications/shellnotification.h"

using namespace Nickvision::Aura;
using namespace Nickvision::Filesystem;
using namespace Nickvision::Notifications;

enum class Theme
{
	Light = 0,
	Dark,
	System
};

class AppConfig : public ConfigurationBase
{
public:
	AppConfig(const std::string& key) : ConfigurationBase(key) 
	{ 

	}

	Theme getTheme() const
	{
		return (Theme)m_json.get("Theme", (int)Theme::System).asInt();
	}

	void setTheme(Theme theme)
	{
		m_json["Theme"] = (int)theme;
	}
};

class AuraTest : public testing::Test
{
public:
	static void SetUpTestSuite()
	{
		Aura::init("org.nickvision.aura.test", "Nickvision Aura Tests", "Aura Tests");
	}
};

TEST_F(AuraTest, EnsureAura)
{
	ASSERT_NO_THROW(Aura::getActive());
}

TEST_F(AuraTest, SetAppInfo)
{
	ASSERT_NO_THROW(Aura::getActive().getAppInfo().setChangelog("* Initial Release"));
	ASSERT_TRUE(!Aura::getActive().getAppInfo().getHtmlChangelog().empty());
}

TEST_F(AuraTest, EnsureDefaultAppConfig)
{
	AppConfig& config{ Aura::getActive().getConfig<AppConfig>("config") };
	ASSERT_EQ(config.getTheme(), Theme::System);
}

TEST_F(AuraTest, ChangeAppConfig)
{
	AppConfig& config{ Aura::getActive().getConfig<AppConfig>("config") };
	config.setTheme(Theme::Light);
	ASSERT_TRUE(config.save());
	ASSERT_EQ(config.getTheme(), Theme::Light);
}

TEST_F(AuraTest, EnsureChangeInAppConfig)
{
	AppConfig& config{ Aura::getActive().getConfig<AppConfig>("config") };
	ASSERT_EQ(config.getTheme(), Theme::Light);
}

TEST_F(AuraTest, ResetAppConfig)
{
	ASSERT_TRUE(std::filesystem::remove(UserDirectories::getApplicationConfig() / ("config.json")));
}

TEST_F(AuraTest, DependencyCheck)
{
#ifdef _WIN32
	std::filesystem::path dependency{ Aura::findDependency("cmd") };
#elif defined(__linux__)
	std::filesystem::path dependency{ Aura::findDependency("ls") };
#endif
	ASSERT_TRUE(!dependency.empty());
	ASSERT_TRUE(std::filesystem::exists(dependency));
	ShellNotificationSentEventArgs args{ "Dependency Found!", dependency.string(), NotificationSeverity::Success, "open", dependency.string() };
#ifdef _WIN32
	if (Aura::getEnvVar("GITHUB_ACTIONS").empty())
	{
		ASSERT_NO_THROW(ShellNotification::send(args, GetConsoleWindow()));
	}
#elif defined(__linux__)
	ASSERT_NO_THROW(ShellNotification::send(args, "Open"));
#endif
}