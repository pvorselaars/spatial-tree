#include <vector>
#include <ostream>
#include <cmath>

template <class U>
class Vector {
	public:

	Vector(U a = 0, U b = 0, U c = 0) : x(a), y(b), z(c) {};

	Vector& operator+=(const Vector &other)
	{
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}

	Vector& operator-=(const Vector &other)
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;
		return *this;
	}

	bool operator==(const Vector &other) const
	{
		return (x == other.x && y == other.y && z == other.z);
	}

	bool operator!=(const Vector &other) const
	{
		return (x != other.x || y != other.y || z == other.z);
	}

	void Normalize()
	{
		float mag = Magnitude();
		if(mag==0)
			return;

		x = x/mag;
		y = y/mag;
		z = z/mag;
	}

	U Dot(const Vector &other) const 
	{
		return (x * other.x + y * other.y + z * other.z);
	}

	double Magnitude(){

		return (sqrt(x*x + y*y + z*z));
	}

	void Round(){

		x = std::round(x);
		y = std::round(y);
		z = std::round(z);

	}

	U x, y, z;
};

template <class U>
Vector<U> operator+(const Vector<U> &lhs, const Vector<U> &rhs)
{
	return Vector<U>(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
}

template <class U>
Vector<U> operator-(Vector<U> &lhs, const Vector<U> &rhs)
{
	return Vector<U>(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
}

template <class U>
Vector<U> operator*(const Vector<U> &lhs, const Vector<U> &rhs)
{
	return Vector<U>(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z);
}

template <class U>
Vector<U> operator*(Vector<U> &lhs, const U &rhs)
{
	return Vector<U>(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs);
}
	
template <class U>
Vector<U> operator*(const U &lhs, Vector<U> &rhs)
{
	return Vector<U>(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z);
}

template <class U>
Vector<U> operator/(const Vector<U> &lhs, const Vector<U> &rhs)
{
	return Vector<U>(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z);
}

template <class U>
Vector<U> operator/(Vector<U> &lhs, const U &rhs)
{
	return Vector<U>(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs);
}

template <class U>
std::ostream& operator<<(std::ostream& os, const Vector<U>& v){
	os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
	return os;
}

template <class T, class U>
class Node {
	public:
	Node(U X, U Y, U Z, T d) : 
		position(X,Y,Z), data(d) {};
	Vector<U> position;
	T data;
};

template <class U>
class Sphere;

template <class U>
class Cuboid {
	public:
	Cuboid(U X, U Y, U Z, U W, U H, U D) : 
		position(X, Y, Z), dimensions(W, H, D) {};

	Cuboid() : position(0,0,0), dimensions(10,10,10) {};

	Cuboid& operator=(const Cuboid &other){
		position = other.position;
		dimensions = other.dimensions;

		return *this;
	}
	
	Vector<U> position;
	Vector<U> dimensions;

	bool operator==(const Cuboid &other) const
	{
		return (position == other.position && dimensions == other.dimensions);
	}

	bool operator!=(const Cuboid &other) const
	{
		return (position != other.position || dimensions != other.dimensions);
	}

	bool contains(const Vector<U> &v) const {
		
		return ((v.x >= position.x - dimensions.x) && (v.x <= position.x + dimensions.x) &&
			(v.y >= position.y - dimensions.y) && (v.y <= position.y + dimensions.y) &&
			(v.z >= position.z - dimensions.z) && (v.z <= position.z + dimensions.z));
	}

	bool intersect(const Cuboid<U> &other) const {
		
		if ( abs(position.x - other.position.x) >= (dimensions.x + other.dimensions.x) ) return false;
		if ( abs(position.y - other.position.y) >= (dimensions.y + other.dimensions.y) ) return false;
		if ( abs(position.z - other.position.z) >= (dimensions.z + other.dimensions.z) ) return false;

		return true;
	}
	
	bool intersect(const Sphere<U> &other) const {

		if(position == other.position) return true;

		Vector<U> C1(dimensions.x, dimensions.y, dimensions.z);
		Vector<U> C2(-dimensions.x, -dimensions.y, -dimensions.z);

		C1 += position;
		C2 += position;
		U R2 = other.radius*other.radius;

		if(other.position.x < C1.x) R2 -= (other.position.x - C1.x)*(other.position.x - C1.x);
		else if(other.position.x > C2.x) R2 -= (other.position.x - C2.x)*(other.position.x - C2.x);
		if(other.position.y < C1.y) R2 -= (other.position.y - C1.y)*(other.position.y - C1.y);
		else if(other.position.y > C2.y) R2 -= (other.position.y - C2.y)*(other.position.y - C2.y);
		if(other.position.z < C1.z) R2 -= (other.position.z - C1.z)*(other.position.z - C1.z);
		else if(other.position.z > C2.z) R2 -= (other.position.z - C2.z)*(other.position.z - C2.z);
		return R2 >= 0;
		
	}
};

template <class U>
class Sphere {
	public:
	Sphere(U X, U Y, U Z, U R) : 
		position(X, Y, Z), radius(R) {};

	Sphere() : position(0,0,0), radius(10) {};

	Sphere& operator=(const Sphere &other){
		position = other.position;
		radius = other.radius;

		return *this;
	}
	
	Vector<U> position;
	U radius;

	bool operator==(const Sphere &other) const
	{
		return (position == other.position && radius == other.radius);
	}

	bool operator!=(const Sphere &other) const
	{
		return (position != other.position || radius != other.radius);
	}


	bool contains(const Vector<U> &v) const {

		return ((v.x - position.x)*(v.x - position.x) + (v.y - position.y)*(v.y - position.y) + (v.z - position.z)*(v.z - position.z) <= radius*radius);

	}

	bool intersect(const Cuboid<U> &other) const {

		if(position == other.position) return true;

		Vector<U> C1(other.dimensions.x, other.dimensions.y, other.dimensions.z);
		Vector<U> C2(-other.dimensions.x, -other.dimensions.y, -other.dimensions.z);

		C1 += other.position;
		C2 += other.position;
		U R2 = radius*radius;

		if(position.x < C1.x) R2 -= (position.x - C1.x)*(position.x - C1.x);
		else if(position.x > C2.x) R2 -= (position.x - C2.x)*(position.x - C2.x);
		if(position.y < C1.y) R2 -= (position.y - C1.y)*(position.y - C1.y);
		else if(position.y > C2.y) R2 -= (position.y - C2.y)*(position.y - C2.y);
		if(position.z < C1.z) R2 -= (position.z - C1.z)*(position.z - C1.z);
		else if(position.z > C2.z) R2 -= (position.z - C2.z)*(position.z - C2.z);
		return R2 >= 0;
		
	}

	bool intersect(const Sphere<U> &other) const {

		return ( sqrt((position.x - other.position.x)*(position.x - other.position.x) + (position.y - other.position.y)*(position.y - other.position.y) + (position.z - other.position.z)*(position.z - other.position.z)) <= radius + other.radius );
		
	}

};

template <class T, class U>
class QuadTree {
	public:
	QuadTree<T,U>() : 
		boundary(),
		nodes(),
		NW(),
		NE(),
		SE(),
		SW(){
			nodes.reserve(4);
	};

	QuadTree<T,U>(Cuboid<U> b, unsigned int c = 4) :
		boundary(b),
		nodes(),
		NW(),
		NE(),
		SE(),
		SW(){
			nodes.reserve(c);
	};

	QuadTree<T, U> (const QuadTree<T, U>& other) : boundary(other.boundary), 
						      nodes(other.nodes),
						      NW(),
						      NE(),
						      SE(),
						      SW(){};

	QuadTree<T, U>& operator=(QuadTree<T, U> other) {
		boundary = other.boundary;
		return *this;
	}

	~QuadTree(){
		if(divided){
			delete NW;
			delete NE;
			delete SE;
			delete SW;
			
		}
	}

	Cuboid<U> boundary;
	std::vector<Node<T,U>> nodes;
	bool divided = false;

	QuadTree * NW;
	QuadTree * NE;
	QuadTree * SE;
	QuadTree * SW;
	
	bool insert(Node<T,U> node){
		if(!boundary.contains(node.position))
			return false;

		if(nodes.size() < nodes.capacity()){
			nodes.push_back(node);
			return true;
		}else{
			if(!divided && ((boundary.dimensions.x  > 1 ) && (boundary.dimensions.y > 1))){	
				subdivide();
			}else if(boundary.dimensions.x <= 1){
				return false;
			}

			if(NW->boundary.contains(node.position))
				return NW->insert(node);

			if(NE->boundary.contains(node.position))
				return NE->insert(node); 

			if(SE->boundary.contains(node.position))
				return SE->insert(node);

			if(SW->boundary.contains(node.position))
				return SW->insert(node);

		return false;
		}	
	}

	bool remove(T node){

		if(nodes.size() == 0)
			return false;

		for(auto it = nodes.begin(); it != nodes.end(); it++){
			if(it->data == node){
				nodes.erase(it);
				return true;
			}
				
		}

		if(divided){
			
			NW->remove(node);
			NE->remove(node);
			SE->remove(node);
			SW->remove(node);

		}
			
		return false;

	}

	void subdivide(){

		Cuboid<U> nw(boundary.position.x - boundary.dimensions.x / 2, boundary.position.y - boundary.dimensions.y / 2, 0, boundary.dimensions.x/2, boundary.dimensions.y/2, 0);

		this->NW = new QuadTree<T,U>(nw);
		
		Cuboid<U> ne(boundary.position.x + boundary.dimensions.x / 2, boundary.position.y + boundary.dimensions.y / 2, 0, boundary.dimensions.x/2, boundary.dimensions.y/2, 0);

		this->NE = new QuadTree<T,U>(ne);

		Cuboid<U> se(boundary.position.x + boundary.dimensions.x / 2, boundary.position.y - boundary.dimensions.y / 2, 0, boundary.dimensions.x/2, boundary.dimensions.y/2, 0);

		this->SE = new QuadTree<T,U>(se);

		Cuboid<U> sw(boundary.position.x - boundary.dimensions.x / 2, boundary.position.y + boundary.dimensions.y / 2, 0, boundary.dimensions.y/2, boundary.dimensions.y/2, 0);

		this->SW = new QuadTree<T,U>(sw);

		divided = true;
	}

	std::vector<Node<T,U>> * query(Cuboid<U> range, std::vector<Node<T,U>> * found = new std::vector<Node<T,U>>()){

		if (!boundary.intersect(range))
			return found;

		for(auto it = nodes.begin(); it != nodes.end(); it++){
			Node<T,U> n = *it;
			if(range.contains(n.position))
				found->push_back(n);
		}

		if(divided){

			NW->query(range, found);
			NE->query(range, found);
			SE->query(range, found);
			SW->query(range, found);
		}

		return found;

	}

	std::vector<Node<T,U>> * query(Sphere<U> range, std::vector<Node<T,U>> * found = new std::vector<Node<T,U>>()){

		if (!boundary.intersect(range))
			return found;

		for(auto it = nodes.begin(); it != nodes.end(); it++){
			Node<T,U> n = *it;
			if(range.contains(n.position))
				found->push_back(n);
		}

		if(divided){

			NW->query(range, found);
			NE->query(range, found);
			SE->query(range, found);
			SW->query(range, found);
		}

		return found;

	}

};


template <class T, class U>
class OcTree {
	public:
	OcTree<T,U>() : 
		boundary(),
		nodes(),
		NNW(),
		NNE(),
		NSE(),
		NSW(),
		SNW(),
		SNE(),
		SSE(),
		SSW(){};

	OcTree<T,U>(Cuboid<U> b, unsigned int c = 8) :
		boundary(b),
		nodes(),
		NNW(),
		NNE(),
		NSE(),
		NSW(),
		SNW(),
		SNE(),
		SSE(),
		SSW(){

			nodes.reserve(c);
	};

	OcTree<T, U> (const OcTree<T, U>& other) : boundary(other.boundary), 
						   nodes(other.nodes),
						   NNW(),
						   NNE(),
						   NSE(),
						   NSW(),
						   SNW(),
						   SNE(),
						   SSE(),
						   SSW(){};

	OcTree<T, U>& operator=(OcTree<T, U> other) {
		boundary = other.boundary;
		return *this;
	}

	~OcTree(){

		if(divided){
			
			delete NNE;
			delete NSE;
			delete NSW;
			delete SNW;
			delete SNE;
			delete SSE;
			delete SSW;

		}
	}

	Cuboid<U> boundary;
	std::vector<Node<T,U>> nodes;
	bool divided = false;

	OcTree * NNW;
	OcTree * NNE;
	OcTree * NSE;
	OcTree * NSW;
	OcTree * SNW;
	OcTree * SNE;
	OcTree * SSE;
	OcTree * SSW;
	
	bool insert(Node<T,U> node){
		if(!boundary.contains(node.position))
			return false;

		if(nodes.size() < nodes.capacity()){
			nodes.push_back(node);
			return true;
		}else{
			if(!divided && ((boundary.dimensions.x  > 1 ) && (boundary.dimensions.y > 1))){	
				subdivide();
			}else if(boundary.dimensions.x <= 1){
				return false;
			}

			if(NNW->boundary.contains(node.position))
				return NNW->insert(node);

			if(NNE->boundary.contains(node.position))
				return NNE->insert(node);

			if(NSE->boundary.contains(node.position))
				return NSE->insert(node);

			if(NSW->boundary.contains(node.position))
				return NSW->insert(node);

			if(SNW->boundary.contains(node.position))
				return SNW->insert(node); 

			if(SNE->boundary.contains(node.position))
				return SNE->insert(node);

			if(SSE->boundary.contains(node.position))
				return SSE->insert(node);

			if(SSW->boundary.contains(node.position))
				return SSW->insert(node);

		return false;

		}	
	}

	bool remove(T node){

		if(nodes.size() == 0)
			return false;
		
		for(auto it = nodes.begin(); it != nodes.end(); it++){
			if(it->data == node){
				nodes.erase(it);
				return true;
			}
				
		}

		if(divided){
			
			NNW->remove(node);
			NNE->remove(node);
			NSE->remove(node);
			NSW->remove(node);
			SNW->remove(node);
			SNE->remove(node);
			SSE->remove(node);
			SSW->remove(node);

		}

		return false;

	}

	void subdivide(){

		Cuboid<U> nnw(boundary.position.x - boundary.dimensions.x / 2, boundary.position.y - boundary.dimensions.y / 2, boundary.position.z - boundary.dimensions.z / 2, boundary.dimensions.x/2, boundary.dimensions.y/2, boundary.dimensions.z/2);

		this->NNW = new OcTree<T,U>(nnw);
		
		Cuboid<U> nne(boundary.position.x + boundary.dimensions.x / 2, boundary.position.y + boundary.dimensions.y / 2, boundary.position.z - boundary.dimensions.z / 2, boundary.dimensions.x/2, boundary.dimensions.y/2, boundary.dimensions.z/2);

		this->NNE = new OcTree<T,U>(nne);

		Cuboid<U> nse(boundary.position.x + boundary.dimensions.x / 2, boundary.position.y - boundary.dimensions.y / 2, boundary.position.z - boundary.dimensions.z / 2, boundary.dimensions.x/2, boundary.dimensions.y/2, boundary.dimensions.z/2);

		this->NSE = new OcTree<T,U>(nse);

		Cuboid<U> nsw(boundary.position.x - boundary.dimensions.x / 2, boundary.position.y + boundary.dimensions.y / 2, boundary.position.z - boundary.dimensions.z / 2, boundary.dimensions.x/2, boundary.dimensions.y/2, boundary.dimensions.z / 2);

		this->NSW = new OcTree<T,U>(nsw);

		Cuboid<U> snw(boundary.position.x - boundary.dimensions.x / 2, boundary.position.y - boundary.dimensions.y / 2, boundary.position.z + boundary.dimensions.z / 2, boundary.dimensions.x/2, boundary.dimensions.y/2, boundary.dimensions.z/2);

		this->SNW = new OcTree<T,U>(snw);
		
		Cuboid<U> sne(boundary.position.x + boundary.dimensions.x / 2, boundary.position.y + boundary.dimensions.y / 2, boundary.position.z + boundary.dimensions.z / 2, boundary.dimensions.x/2, boundary.dimensions.y/2, boundary.dimensions.z/2);

		this->SNE = new OcTree<T,U>(sne);

		Cuboid<U> sse(boundary.position.x + boundary.dimensions.x / 2, boundary.position.y - boundary.dimensions.y / 2, boundary.position.z + boundary.dimensions.z / 2, boundary.dimensions.x/2, boundary.dimensions.y/2, boundary.dimensions.z/2);

		this->SSE = new OcTree<T,U>(sse);

		Cuboid<U> ssw(boundary.position.x - boundary.dimensions.x / 2, boundary.position.y + boundary.dimensions.y / 2, boundary.position.z + boundary.dimensions.z / 2, boundary.dimensions.x/2, boundary.dimensions.y/2, boundary.dimensions.z / 2);

		this->SSW = new OcTree<T,U>(ssw);

		divided = true;
	}

	std::vector<Node<T,U>> * query(Cuboid<U> range, std::vector<Node<T,U>> * found = new std::vector<Node<T,U>>()){

		if (!boundary.intersect(range))
			return found;

		for(auto it = nodes.begin(); it != nodes.end(); it++){
			Node<T,U> n = *it;
			if(range.contains(n.position))
				found->push_back(n);
		}

		if(divided){

			NNW->query(range, found);
			NNE->query(range, found);
			NSE->query(range, found);
			NSW->query(range, found);
			SNW->query(range, found);
			SNE->query(range, found);
			SSE->query(range, found);
			SSW->query(range, found);
		}

		return found;

	}

	std::vector<Node<T,U>> * query(Sphere<U> range, std::vector<Node<T,U>> * found = new std::vector<Node<T,U>>()){

		if (!boundary.intersect(range))
			return found;

		for(auto it = nodes.begin(); it != nodes.end(); it++){
			Node<T,U> n = *it;
			if(range.contains(n.position))
				found->push_back(n);
		}

		if(divided){

			NNW->query(range, found);
			NNE->query(range, found);
			NSE->query(range, found);
			NSW->query(range, found);
			SNW->query(range, found);
			SNE->query(range, found);
			SSE->query(range, found);
			SSW->query(range, found);
		}

		return found;

	}
};
