#include <vector>

template <class T, class U>
class Node {
	public:
	Node(U X, U Y, U Z, T d) : 
		x(X), y(Y), z(Z), data(d) {};
	U x;
	U y;
	U z;
	T data;
};

template <class U>
class AABB {
	public:
	AABB(U X, U Y, U Z, U W, U H, U D) : 
		x(X), y(Y), z(Z), w(W), h(H), d(D) {};

	AABB() : x(0), y(0), z(0), w(10), h(10), d(10) {};

	AABB& operator=(AABB other){
		x = other.x;
		y = other.y;
		z = other.z;
		w = other.w;
		h = other.h;
		d = other.d;

		return *this;
	}

	U x;
	U y;
	U z;
	U w;
	U h;
	U d;

	bool contains(U X, U Y, U Z) const {
		
		return ((X >= x - w) && (X <= x + w) &&
			(Y >= y - h) && (Y <= y + h) &&
			(Z >= z - d) && (Z <= z + d));
	}

	bool intersect(AABB other) {
		
		if ( abs(x - other.x) > (w + other.w) ) return false;
		if ( abs(y - other.y) > (h + other.h) ) return false;
		if ( abs(z - other.z) > (z + other.z) ) return false;

		return true;
	}
};

template <class T, class U>
class LinTree {
	public:
	LinTree<T,U>() : 
		boundary(),
		nodes(),
		W(),
		E(){};

	LinTree<T,U>(AABB<U> b) :
		boundary(b),
		nodes(),
		W(),
		E(){
		nodes.reserve(capacity);
	};

	LinTree<T, U> (const LinTree<T, U>& other) : boundary(other.boundary), 
						      nodes(other.nodes),
						      W(),
						      E(){};

	LinTree<T, U>& operator=(LinTree<T, U> other) {
		boundary = other.boundary;
		return *this;
	}

	~LinTree(){
		if(divided){
			delete W;
			delete E;
		}
	}

	AABB<U> boundary;
	unsigned int capacity = 2;
	std::vector<Node<T,U>> nodes;
	bool divided = false;

	LinTree * W;
	LinTree * E;
	
	bool insert(Node<T,U> node){
		if(!boundary.contains(node.x, node.y, node.z))
			return false;

		if(nodes.size() < capacity){
			nodes.push_back(node);
			return true;
		}else{
			if(!divided && ((boundary.w  > 1 ) && (boundary.h > 1))){	
				subdivide();
			}else if(boundary.w <= 1){
				return false;
			}

			if(W->boundary.contains(node.x, node.y, node.z))
				return W->insert(node);

			if(E->boundary.contains(node.x, node.y, node.z))
				return E->insert(node); 

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
		return false;
	}

	void subdivide(){

		AABB<U> w(boundary.x - boundary.w / 2, 0, 0, boundary.w/2, 0, 0);

		this->W = new LinTree<T,U>(w);
		
		AABB<U> e(boundary.x + boundary.w / 2, 0, 0, boundary.w/2, 0, 0);

		this->E = new LinTree<T,U>(e);

		divided = true;
	}

	std::vector<Node<T,U>> * query(AABB<U> range, std::vector<Node<T,U>> * found = new std::vector<Node<T,U>>()){

		if (!boundary.intersect(range))
			return found;

		for(auto it = nodes.begin(); it != nodes.end(); it++){
			Node<T,U> n = *it;
			if(range.contains(n.x, n.y, n.z))
				found->push_back(n);
		}

		if(divided){
			W->query(range, found);
			E->query(range, found);
		}

		return found;

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
		SW(){};

	QuadTree<T,U>(AABB<U> b) :
		boundary(b),
		nodes(),
		NW(),
		NE(),
		SE(),
		SW(){

		nodes.reserve(capacity);
	};

	QuadTree<T, U> (const QuadTree<T, U>& other) : boundary(other.boundary), 
						      nodes(other.nodes),
						      NW(),
						      NE(),
						      SE(),
						      SW() {};

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

	AABB<U> boundary;
	const long unsigned int capacity = 8;
	std::vector<Node<T,U>> nodes;
	bool divided = false;

	QuadTree * NW;
	QuadTree * NE;
	QuadTree * SE;
	QuadTree * SW;
	
	bool insert(Node<T,U> node){
		if(!boundary.contains(node.x, node.y, node.z))
			return false;

		if(nodes.size() < capacity){
			nodes.push_back(node);
			return true;
		}else{
			if(!divided && ((boundary.w  > 1 ) && (boundary.h > 1))){	
				subdivide();
			}else if(boundary.w <= 1){
				return false;
			}

			if(NW->boundary.contains(node.x, node.y, node.z))
				return NW->insert(node);

			if(NE->boundary.contains(node.x, node.y, node.z))
				return NE->insert(node); 

			if(SE->boundary.contains(node.x, node.y, node.z))
				return SE->insert(node);

			if(SW->boundary.contains(node.x, node.y, node.z))
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
		return false;
	}

	void subdivide(){

		AABB<U> nw(boundary.x - boundary.w / 2, boundary.y - boundary.h / 2, 0, boundary.w/2, boundary.h/2, 0);

		this->NW = new QuadTree<T,U>(nw);
		
		AABB<U> ne(boundary.x + boundary.w / 2, boundary.y + boundary.h / 2, 0, boundary.w/2, boundary.h/2, 0);

		this->NE = new QuadTree<T,U>(ne);

		AABB<U> se(boundary.x + boundary.w / 2, boundary.y - boundary.h / 2, 0, boundary.w/2, boundary.h/2, 0);

		this->SE = new QuadTree<T,U>(se);

		AABB<U> sw(boundary.x - boundary.w / 2, boundary.y + boundary.h / 2, 0, boundary.w/2, boundary.h/2, 0);

		this->SW = new QuadTree<T,U>(sw);

		divided = true;
	}

	std::vector<Node<T,U>> * query(AABB<U> range, std::vector<Node<T,U>> * found = new std::vector<Node<T,U>>()){

		if (!boundary.intersect(range))
			return found;

		for(auto it = nodes.begin(); it != nodes.end(); it++){
			Node<T,U> n = *it;
			if(range.contains(n.x, n.y, n.z))
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

	OcTree<T,U>(AABB<U> b) :
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

		nodes.reserve(capacity);
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

	AABB<U> boundary;
	const long unsigned int capacity = 8;
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
		if(!boundary.contains(node.x, node.y, node.z))
			return false;

		if(nodes.size() < capacity){
			nodes.push_back(node);
			return true;
		}else{
			if(!divided && ((boundary.w  > 1 ) && (boundary.h > 1))){	
				subdivide();
			}else if(boundary.w <= 1){
				return false;
			}

			if(NNW->boundary.contains(node.x, node.y, node.z))
				return NNW->insert(node);

			if(NNE->boundary.contains(node.x, node.y, node.z))
				return NNE->insert(node);

			if(NSE->boundary.contains(node.x, node.y, node.z))
				return NSE->insert(node);

			if(NSW->boundary.contains(node.x, node.y, node.z))
				return NSW->insert(node);

			if(SNW->boundary.contains(node.x, node.y, node.z))
				return SNW->insert(node); 

			if(SNE->boundary.contains(node.x, node.y, node.z))
				return SNE->insert(node);

			if(SSE->boundary.contains(node.x, node.y, node.z))
				return SSE->insert(node);

			if(SSW->boundary.contains(node.x, node.y, node.z))
				return SSW->insert(node);

		return false;

		}	
	}

	bool remove(Node<T,U> node){

		if(!boundary.contains(node.x, node.y, node.z))
			return false;

		if(nodes.size() == 0)
			return false;
		
		for(auto it = nodes.begin(); it != nodes.end(); it++){
			if(it->data == node.data){
				nodes.erase(it);
				return true;
			}
				
		}

		NNW->remove(node);
		NNE->remove(node);
		NSE->remove(node);
		NSW->remove(node);
		SNW->remove(node);
		SNE->remove(node);
		SSE->remove(node);
		SSW->remove(node);
		
		return true;


	}

	void subdivide(){

		AABB<U> nnw(boundary.x - boundary.w / 2, boundary.y - boundary.h / 2, boundary.z - boundary.d / 2, boundary.w/2, boundary.h/2, boundary.d/2);

		this->NNW = new OcTree<T,U>(nnw);
		
		AABB<U> nne(boundary.x + boundary.w / 2, boundary.y + boundary.h / 2, boundary.z - boundary.d / 2, boundary.w/2, boundary.h/2, boundary.d/2);

		this->NNE = new OcTree<T,U>(nne);

		AABB<U> nse(boundary.x + boundary.w / 2, boundary.y - boundary.h / 2, boundary.z - boundary.d / 2, boundary.w/2, boundary.h/2, boundary.d/2);

		this->NSE = new OcTree<T,U>(nse);

		AABB<U> nsw(boundary.x - boundary.w / 2, boundary.y + boundary.h / 2, boundary.z - boundary.d / 2, boundary.w/2, boundary.h/2, boundary.d / 2);

		this->NSW = new OcTree<T,U>(nsw);

		AABB<U> snw(boundary.x - boundary.w / 2, boundary.y - boundary.h / 2, boundary.z + boundary.d / 2, boundary.w/2, boundary.h/2, boundary.d/2);

		this->SNW = new OcTree<T,U>(snw);
		
		AABB<U> sne(boundary.x + boundary.w / 2, boundary.y + boundary.h / 2, boundary.z + boundary.d / 2, boundary.w/2, boundary.h/2, boundary.d/2);

		this->SNE = new OcTree<T,U>(sne);

		AABB<U> sse(boundary.x + boundary.w / 2, boundary.y - boundary.h / 2, boundary.z + boundary.d / 2, boundary.w/2, boundary.h/2, boundary.d/2);

		this->SSE = new OcTree<T,U>(sse);

		AABB<U> ssw(boundary.x - boundary.w / 2, boundary.y + boundary.h / 2, boundary.z + boundary.d / 2, boundary.w/2, boundary.h/2, boundary.d / 2);

		this->SSW = new OcTree<T,U>(ssw);

		divided = true;
	}

	std::vector<Node<T,U>> * query(AABB<U> range, std::vector<Node<T,U>> * found = new std::vector<Node<T,U>>()){

		if (!boundary.intersect(range))
			return found;

		for(auto it = nodes.begin(); it != nodes.end(); it++){
			Node<T,U> n = *it;
			if(range.contains(n.x, n.y, n.z))
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
