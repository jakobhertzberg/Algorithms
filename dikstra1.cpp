#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <sstream>

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
	ifstream file (argv[1]);
	int	c = 0;
	string delimiter = " ";
	for (std::string line ; getline( file, line ); ){
		if (line[0] != '#') {
			if (c == 0) {
					std::string node = line.substr(0,line.find(delimiter));
					unvisited.push_back(node);
			}
			else {
				int counter = 1;
				std::string keynode="";
				std::string targetnode="";
				float distance=0;
				while (getline(file, line, ' ')){
					switch(counter){
						case 1: { cout << "Keynode:" << line << "\n"; std::string keynode=line;counter++;break;};
						case 2: { cout << "Targetnode:" << line << "\n"; std::string targetnode=line;counter++;break;};
						case 3: { cout << "Distance:" << line << "\n"; distance=::atof(line.c_str());counter = 1;
							if (graph.find(keynode) == graph.end()){
								Point point;
								point.distance=distance;
								point.node=targetnode;
								graph[keynode].push_back(point);
							}
							else{
								Point point;
								point.distance=distance;
								point.node=targetnode;
								vector <Point> distancelist;
								distancelist.push_back(point);
								graph[keynode]=distancelist;
							}
							break;};
							default: {cout << "default" <<"\n";};
					}
				}
				break;
				}
							}
		else if (line[0] == '#') { c = 1 ; }
								};

	file.close();

	return 0;

}
