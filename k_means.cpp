#include "vector"
#include "iostream"
#include "fstream"
#include "sstream"
#include "algorithm"
#include "unordered_map"
#include "string"
#include "limits.h"
using namespace std;

struct Point {
    double x, y, n, m;     // coordinates
    int cluster;     // no default cluster
    double minDist;  // default infinite dist to nearest cluster
    Point() : 
        x(0.0), 
        y(0.0),
        n(0.0), 
        m(0.0),
        cluster(-1),
        minDist(__DBL_MAX__) {}
        
    Point(double x, double y, double n, double m) : 
        x(x), 
        y(y),
        n(n), 
        m(m),
        cluster(-1),
        minDist(__DBL_MAX__) {}

    double distance(Point p) {
        return (p.x - x) * (p.x - x) + (p.y - y) * (p.y - y) + (p.n - n) * (p.n - n) + (p.m - m) * (p.m - m);
    }
};

void read_file_iris(vector<Point> &data) {
    string line;
    ifstream file("C:\\Users\\Ke Ma\\Desktop\\590\\Week1_lab\\iris.data");
    while(getline(file, line)) {
        stringstream ss(line);
        string temp;
        vector<string> vec;
        while(getline(ss, temp, ',')) {
            vec.push_back(temp);
        }
        double a, b, c, d;
        a = stof(vec[0]);
        b = stof(vec[1]);
        c = stof(vec[2]);
        d = stof(vec[3]);
        Point *p = new Point(a, b, c, d);
        data.push_back(*p);
    }
    file.close();
}

class KMeans {
public:
    unordered_map<int, vector<Point> > hash;
    vector<Point> *data;
    vector<Point> centroids; 
    int row;
    int k;
    int iterations;
    double eps;
    vector<int> label;

    KMeans(vector<Point> *data, int k, int iterations, double eps, int row) {
        this->data = data;
        this->k = k;
        this->iterations = iterations;
        this->eps = eps;
        this->row = row;
        for (int i = 0; i < k; i++) {
            centroids.push_back(data->at(i));
        }
        for (int i = 0; i < row; i++) {
            label.push_back(0);
        }
    }

    int find_cluster(Point *p) {
        int cluster;
        for (int i = 0; i < k; i++) {
            double val = p->distance(centroids[i]);
            if (val < p->minDist) {
                p->minDist = val;
                cluster = i;
            }
        }
        p->minDist = __DBL_MAX__;
        hash[cluster].push_back(*p);
        return cluster;
    }

    void recalcuate_center() {
        centroids.erase(centroids.begin(), centroids.end());
        for (unordered_map<int, vector<Point> >::iterator it = hash.begin(); it != hash.end(); it++) {
            int x = it->second.size();
            double a = 0, b = 0, c = 0, d = 0;
            for (int j = 0; j < x; j++) {
                a = a + it->second[j].x;
                b = b + it->second[j].y;
                c = c + it->second[j].n;
                d = d + it->second[j].m;
            }
            centroids.push_back(Point(a/x, b/x, c/x, d/x));
            cout << x << " " << a/x << " " << b/x << " " << c/x << " " << d/x << endl;
        }
        hash.erase(hash.begin(), hash.end());
    }

    void kmeans() {
        for (int epoch = 0; epoch < iterations; epoch++) {
            for (int dt = 0; dt < row; dt++) {
                int cluster_id = find_cluster( &(data->at(dt)));
                label[dt] = cluster_id;
            }
            recalcuate_center();
            cout << endl << endl;
        }
    }
};


int main() {
    vector<Point> data;
    read_file_iris(data);
    KMeans obj(&data, 3, 30, 0.01, data.size());
    obj.kmeans();
    return 0;
}
