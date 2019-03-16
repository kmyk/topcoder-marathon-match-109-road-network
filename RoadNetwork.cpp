// C++11
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>
#include <set>
#include <string>
#include <iterator>

using namespace std;

class RoadNetwork {
public:
    vector<int> findSolution(int NM, int N, int E, vector<string> edges, int R, vector<string> routes)
    {  
        vector<int> ret;

        //build connections until we run out of materials        
        for (int i = 0; i < E; i++)
        {
            vector<string> temp = split(edges[i]," ");
            int cost=atoi(temp[2].c_str());

            if (cost<=NM)
            {
                NM-=cost;
                ret.push_back(i);
            }
        }
        
        return ret;      
    }
   
    vector<string> split (string s, string delimiter) {
        size_t pos_start = 0, pos_end, delim_len = delimiter.length();
        string token;
        vector<string> res;

        while ((pos_end = s.find (delimiter, pos_start)) != string::npos) {
            token = s.substr (pos_start, pos_end - pos_start);
            pos_start = pos_end + delim_len;
            res.push_back (token);
        }

        res.push_back (s.substr (pos_start));
        return res;
    }    
};


// -------8<------- end of solution submitted to the website -------8<-------

template<class T> void getVector(vector<T>& v, int num) {
    T a;
    std::getline(std::cin, a);      //skip the first one
    for (int i = 0; i < num; ++i)
    {
        std::getline(std::cin, a);
        v.push_back(a);
    }
}

int main() {
    RoadNetwork rn;
    int NM;
    int N;
    int E;
    int R;
    cin >> NM;
    cin >> N;
    cin >> E;
    vector<string> edges;
    getVector(edges,E);  
    cin >> R;
    vector<string> routes;
    getVector(routes,R);

    vector<int> ret = rn.findSolution(NM,N,E,edges,R,routes);
    cout << ret.size() << endl;
    for (int i = 0; i < (int)ret.size(); i++)
        cout << ret[i] << endl;
    cout.flush();
}