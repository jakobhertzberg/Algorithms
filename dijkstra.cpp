#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <queue>

using namespace std;

vector<string> unvisited;

class Point {
	public:
		string node;
		float distance;
	public:
		float getdistance(){
			return distance;
		}
		string getnode(){
			return node;
		}
};

struct compareNodes
{
  bool operator()(Point& l, Point& r)
  {
      return l.getdistance() < r.getdistance();
  }
};

priority_queue <Point,vector<Point>,compareNodes> queue;

unordered_map <std::string, vector<Point>> graph;

int main (int argc, char*argv[] )
{
	 if(argc != 4) { return 2; } // valid number of arguments

	ifstream file (argv[1]);
	string sourceID = argv[2];
	string targetID = argv[3]; // arguments

	string line;

	if (!file.is_open()) { return 2; 	 }; // file exists

	while( getline(file , line)){

		if(line[0] == '#'){
			break; }
			string node = line.substr(0,line.find(" "));
			unvisited.push_back(node);
	}

	char counter = 'A';
	std::string keynode="";
	std::string targetnode="";
	float distance=0;

	while(getline(file , line, ' '))
		{
			line.erase(std::remove(line.begin(), line.end(), '\n'), line.end());
			switch(counter){
				case 'A': {cout << "Keynode:"<< line ;string keynode=line;counter='B';continue;};
				case 'B': {cout << "targetnode:"<< line ;string targetnode=line;counter='C';continue;};
				case 'C': {cout << "distance:"<< line ;distance=atof(line.c_str());
					if (graph.find(keynode) == graph.end()){
						Point point;
						point.distance=distance;
						point.node=targetnode;
						vector <Point> distancelist;
						distancelist.push_back(point);
						graph.emplace(std::make_pair(keynode,distancelist));
					}
					else{
						Point point;
						point.distance=distance;
						point.node=targetnode;
						graph[keynode].push_back(point);
					}
					counter = 'A';continue;};
		}
}


    if (file.bad()) { cout << "bad"; return 2; }; // readable?


	return 0;

}
