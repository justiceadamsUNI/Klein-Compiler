#include "catch.hpp"
#include"../src/header/DummyClass.h"

TEST_CASE("Dummy Class Constructor Doesn't throw exception when valid paramater ", "[Dummy Class]") {
	REQUIRE_NOTHROW(DummyClass(10));
}

TEST_CASE("Dummy Class Constructor throws exception when invalid paramater ", "[Dummy Class]") {
	REQUIRE_THROWS(DummyClass(0));
}