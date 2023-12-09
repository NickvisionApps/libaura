#include <gtest/gtest.h>
#include "network/networkmonitor.h"
#ifndef _WIN32
#include <stdlib.h>
#endif

using namespace Nickvision::Aura::Network;

TEST(NetworkTests, ConnectedGlobal)
{
	NetworkMonitor netmon;
	netmon.stateChanged() += [](const NetworkStateChangedEventArgs& e)
	{
		ASSERT_EQ(e.getState(), NetworkState::ConnectedGlobal);
	};
	netmon.checkConnectionState();
}

TEST(NetworkTests, DisableNetCheck)
{
#ifdef _WIN32
	ASSERT_EQ(_putenv("AURA_DISABLE_NETCHECK=true"), 0);
#else
	ASSERT_EQ(setenv("AURA_DISABLE_NETCHECK", "true", true), 0);
#endif
	NetworkMonitor netmon;
	netmon.stateChanged() += [](const NetworkStateChangedEventArgs& e)
	{
		ASSERT_EQ(e.getState(), NetworkState::ConnectedGlobal);
	};
	netmon.checkConnectionState();
}