#include "RoadNetwork.cpp"

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
