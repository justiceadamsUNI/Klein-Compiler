#include "catch.hpp"
#include"../src/header/Token.h"
#include"../src/header/TokenType.h"

TEST_CASE("Token GetValue Works When Value Not Null", "[Token]") {
	Token TestToken(INTEGER, "123");
	REQUIRE(TestToken.getValue() == "123");
}

TEST_CASE("Token GetValue Works When Value Is Empty", "[Token]") {
	Token TestToken(COMMA, "");
	REQUIRE(TestToken.getValue() == "");
}

TEST_CASE("Token GetTokenType Works", "[Token]") {
	Token TestToken(COMMA, "");
	REQUIRE(TestToken.getTokenType() == COMMA);
}