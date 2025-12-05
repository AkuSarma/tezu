#include <bits/stdc++.h>
using namespace std;

struct Point
{
    double x, y;
};

bool cmpX(const Point &a, const Point &b)
{
    return a.x < b.x || (a.x == b.x && a.y < b.y);
}

bool cmpY(const Point &a, const Point &b)
{
    return a.y < b.y || (a.y == b.y && a.x < b.x);
}

double dist(const Point &a, const Point &b)
{
    return hypot(a.x - b.x, a.y - b.y);
}

void bruteForce(const vector<Point> &pts, Point &p1, Point &p2, double &d)
{
    d = 1e18;
    int n = pts.size();
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
        {
            double dd = dist(pts[i], pts[j]);
            if (dd < d)
            {
                d = dd;
                p1 = pts[i];
                p2 = pts[j];
            }
        }
}

void across(const vector<Point> &slab, Point &p1, Point &p2, double &d)
{
    int n = slab.size();
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n && (slab[j].y - slab[i].y) < d; j++)
        {
            double dd = dist(slab[i], slab[j]);
            if (dd < d)
            {
                d = dd;
                p1 = slab[i];
                p2 = slab[j];
            }
        }
}

void closestPair(const vector<Point> &px, const vector<Point> &py,
                 Point &best1, Point &best2, double &bestD)
{
    int n = px.size();
    if (n <= 3)
    {
        bruteForce(px, best1, best2, bestD);
        return;
    }

    int mid = n / 2;
    double midx = px[mid].x;

    vector<Point> pxL(px.begin(), px.begin() + mid);
    vector<Point> pxR(px.begin() + mid, px.end());

    // Create a set to identify left-side points
    unordered_set<long long> leftSet;
    for (auto &p : pxL)
    {
        long long key = ((long long)(p.x * 1e6) << 32) ^ (long long)(p.y * 1e6);
        leftSet.insert(key);
    }

    vector<Point> pyL, pyR;
    for (auto &p : py)
    {
        long long key = ((long long)(p.x * 1e6) << 32) ^ (long long)(p.y * 1e6);
        if (leftSet.count(key))
            pyL.push_back(p);
        else
            pyR.push_back(p);
    }

    Point L1, L2, R1, R2;
    double dL, dR;

    closestPair(pxL, pyL, L1, L2, dL);
    closestPair(pxR, pyR, R1, R2, dR);

    if (dL < dR)
    {
        best1 = L1;
        best2 = L2;
        bestD = dL;
    }
    else
    {
        best1 = R1;
        best2 = R2;
        bestD = dR;
    }

    vector<Point> slab;
    for (auto &p : py)
        if (fabs(p.x - midx) < bestD)
            slab.push_back(p);

    across(slab, best1, best2, bestD);
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        cerr << "Usage: " << argv[0] << " <inputfile>\n";
        return 1;
    }

    ifstream fin(argv[1]);
    if (!fin)
    {
        cerr << "Error opening file\n";
        return 1;
    }

    int n;
    fin >> n;

    vector<Point> pts(n);
    for (int i = 0; i < n; i++)
        fin >> pts[i].x >> pts[i].y;

    vector<Point> px = pts, py = pts;
    sort(px.begin(), px.end(), cmpX);
    sort(py.begin(), py.end(), cmpY);

    Point p1, p2;
    double d;

    closestPair(px, py, p1, p2, d);

    if (make_tuple(p2.x, p2.y) < make_tuple(p1.x, p1.y))
        swap(p1, p2);

    printf("Closest pair: p1(%.2f, %.2f), p2(%.2f, %.2f) d = %f\n",
           p1.x, p1.y, p2.x, p2.y, d);

    return 0;
}