#include <eeros/control/SocketData.hpp>
#include <eeros/core/Fault.hpp>
#include <gtest/gtest.h>

using namespace eeros;
using namespace eeros::control;
using namespace eeros::math;


// Test name
TEST(controlSocketDataTest, nan) {
	SocketData<Vector2,Vector2> s("", 9876);
	EXPECT_EQ(s.getName(), std::string(""));
	s.setName("socket data block");
	EXPECT_EQ(s.getName(), std::string("socket data block"));
}
