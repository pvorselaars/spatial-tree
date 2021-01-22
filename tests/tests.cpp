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

	SECTION("vector inequality"){
		REQUIRE( v0 != v1 );
	}

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

TEST_CASE("cubic geometry", "[Cuboid]"){
	
	Cuboid<int> c0(0,0,0,5,5,5);
	Cuboid<int> c1(5,5,5,1,1,1);

	Cuboid<int> c3(10,0,0,5,5,5);

	Sphere<int> s0(0,0,0,5);

	Vector<int> v0(0,0,0);
	Vector<int> v1(6,6,6);

	SECTION("cuboid assignment"){
		c0 = c1;
		REQUIRE(c0 == c1);
	}

	SECTION("cuboid contains vector"){
		REQUIRE(c0.contains(v0) == true);
		REQUIRE(c1.contains(v1) == true);
	}

	SECTION("cuboid does not contain vector"){
		REQUIRE(c0.contains(v1) == false);
		REQUIRE(c1.contains(v0) == false);
	}

	SECTION("cuboids intersect"){
		REQUIRE(c0.intersect(c1) == true);
		REQUIRE(c1.intersect(c0) == true);
	}

	SECTION("cuboids do not intersect"){
		REQUIRE(c0.intersect(c3) == false);
		REQUIRE(c3.intersect(c0) == false);
	}

	SECTION("cuboid and sphere intersect"){
		REQUIRE(c0.intersect(s0) == true);
		REQUIRE(s0.intersect(c0) == true);
	}

	SECTION("cuboid and sphere do not intersect"){
		REQUIRE(c3.intersect(s0) == false);
		REQUIRE(s0.intersect(c3) == false);
	}

}

TEST_CASE("spheric geometry", "[Sphere]"){

	Sphere<int> s0(0,0,0,5);
	Sphere<int> s1(5,5,5,1);
	Sphere<int> s3(5,0,0,2);

	Vector<int> v0(0,0,0);
	Vector<int> v1(6,5,5);
	Vector<int> v2(6,0,0);

	SECTION("sphere assignment"){
		s0 = s1;
		REQUIRE(s0 == s1);
	}

	SECTION("sphere contains vector"){
		REQUIRE(s0.contains(v0) == true);
		REQUIRE(s1.contains(v1) == true);
	}

	SECTION("sphere does not contain vector"){
		REQUIRE(s0.contains(v2) == false);
		REQUIRE(s1.contains(v0) == false);
	}

	SECTION("spheres intersect"){
		REQUIRE(s0.intersect(s3) == true);
		REQUIRE(s3.intersect(s0) == true);
	}

	SECTION("spheres do not intersect"){
		REQUIRE(s0.intersect(s1) == false);
		REQUIRE(s1.intersect(s0) == false);
	}
}
