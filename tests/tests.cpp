#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "spatial-tree.hpp"

TEST_CASE("vector algebra", "[Vector]"){

	Vector<int> v0(0,0,0);
	Vector<int> v1(0,1,0);
	Vector<int> v2(1,0,1);
	Vector<int> v3(1,1,1);
	Vector<int> v4(4,4,4);

	Vector<float> v5(1,1,1);
	Vector<float> v6(1.4,1.4,1.4);

	Vector<double> v7(7,0,0);
	Vector<double> v8(1,0,0);

	SECTION("vector addition"){
		REQUIRE( v1 + v2 == v3 );
		REQUIRE( (v1 += v2) == v3 );
	}
	SECTION("vector subtraction"){
		REQUIRE( v3 - v1 == v2 );
		REQUIRE( (v3 -= v1) == v2 );
	}
	SECTION("vector multiplication"){
		REQUIRE( v3 * 4 == v4 );
		REQUIRE( 4 * v3 == v4 );
		REQUIRE( v4 * 0 == v0 );
		REQUIRE( v2 * v3 == v2 );
		REQUIRE( v3 * v4 == v4 );
	}
	SECTION("vector division"){
		REQUIRE( v4 / 4 == v3 );
		REQUIRE( v4 / v4 == v3 );
	}
	SECTION("vector dot product"){
		REQUIRE( v0.Dot(v3) == 0 );
		REQUIRE( v1.Dot(v3) == 1 );
	}
	SECTION("vector magnitude"){
		REQUIRE( v1.Magnitude() == 1 );
		REQUIRE( v3.Magnitude() == sqrt(3) );
	}
	SECTION("vector normalization"){
		v7.Normalize();
		REQUIRE( v7 == v8 );
	}
	SECTION("vector rounding"){
		v6.Round();
		REQUIRE( v6 == v5 );
	}
}
