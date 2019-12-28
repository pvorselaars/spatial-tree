#include <vector>

class Node {
	public:
	Node(float X, float Y, float Z, int d) : 
		x(X), y(Y), z(Z), data(d) {};
	float x;
	float y;
	float z;
	int data;
};

class AABB {
	public:
	AABB(float X, float Y, float Z, float W, float H, float D) : 
		x(X), y(Y), z(Z), w(W), h(H), d(D) {};

	float x;
	float y;
	float z;
	float w;
	float h;
	float d;

	bool contains(float X, float Y, float Z){
		
		return (X > x - w && X < x + w &&
			Y > y - h && Y < y + h &&
			Z > z - d && Z < z + d);
	}

	bool contains(Node node){

		return (node.x >= x - w && node.x <= x + w &&
			node.y >= y - h && node.y <= y + h &&
			node.z >= z - d && node.z <= z + d);
	}

	bool intersect(AABB &other){

		return true;
	}
};

class QuadTree {
	public:
	QuadTree(AABB b) :
		boundary(b) {

		nodes.reserve(capacity);
	};

	AABB boundary;
	const int capacity = 4;
	std::vector<Node> nodes;

	QuadTree * NW = nullptr;
	QuadTree * NE = nullptr;
	QuadTree * SE = nullptr;
	QuadTree * SW = nullptr;
	
	bool insert(Node node){
		if(!boundary.contains(node))
			return false;

		if(nodes.size() < capacity){
			nodes.push_back(node);
			return true;
		}else{
			subdivide();

			if(NW->insert(node)){
				return true;
			}else if(NE->insert(node)){
				return true;
			}else if(SE->insert(node)){
				return true;
			}
			
			SW->insert(node);

			return true;
		}	
	}

	void subdivide(){
		AABB nw(boundary.x - boundary.w/2, boundary.y + boundary.h/2, 0, boundary.w/2, boundary.h/2, 0);
		NW = new QuadTree(boundary);
		AABB ne(boundary.x + boundary.w/2, boundary.y + boundary.h/2, 0, boundary.w/2, boundary.h/2, 0);
		NE = new QuadTree(boundary);
		AABB se(boundary.x + boundary.w/2, boundary.y - boundary.h/2, 0, boundary.w/2, boundary.h/2, 0);
		SE = new QuadTree(boundary);
		AABB sw(boundary.x - boundary.w/2, boundary.y - boundary.h/2, 0, boundary.w/2, boundary.h/2, 0);
		SW = new QuadTree(boundary);
	}

};


