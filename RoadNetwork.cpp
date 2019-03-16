#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

using namespace std;


class RoadNetwork {
public:
    vector<int> findSolution(int NM, int N, int E, vector<string> edges, int R, vector<string> routes) {
        vector<int> ret;

        // build connections until we run out of materials
        for (int i = 0; i < E; ++ i) {
            vector<string> temp = split(edges[i], " ");
            int cost = atoi(temp[2].c_str());

            if (cost <= NM) {
                NM -= cost;
                ret.push_back(i);
            }
        }

        return ret;
    }

    vector<string> split(string const & s, string const & delimiter) {
        size_t pos_start = 0;
        size_t pos_end;
        size_t delim_len = delimiter.length();
        string token;
        vector<string> res;

        while ((pos_end = s.find(delimiter, pos_start)) != string::npos) {
            token = s.substr(pos_start, pos_end - pos_start);
            pos_start = pos_end + delim_len;
            res.push_back(token);
        }

        res.push_back(s.substr(pos_start));
        return res;
    }
};
