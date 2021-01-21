#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include "spatial-tree.h"

TEST_CASE("vector arithmetic", "[Vector]"){

	Vector<int> v1(0,1,0);
	Vector<int> v2(1,0,1);
	Vector<int> v3(1,1,1);

	REQUIRE( v1 + v2 == v3 );
	REQUIRE( v3 - v1 == v2 );
}
