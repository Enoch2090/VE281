#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>
using namespace std;

class point{
private:
    int x;
    int y;
public:
    point(int x, int y);
    void setX(int x);
    void setY(int y);
    int X();
    int Y();
    bool isEqual(point p1);
    double D();
    void print();
    static int ccw(point p1, point p2, point p3);
    bool comp_by_polarangle(point p1, point p2);
    static bool comp_by_coordinate(point p1, point p2);
    bool operator==(point p1);
};

std::function<bool(point,point)> comp;

point::point(int x, int y){this->x = x; this->y = y;}

void point::setX(int x){this->x = x;}

void point::setY(int y){this->y = y;}

int point::X(){return this->x;}

int point::Y(){return this->y;}

bool point::isEqual(point p1){return this->x==p1.x && this->y==p1.y;}

double point::D(){return (this->x) * (this->x) + (this->y) * (this->y);}

void point::print(){cout << this->x << " " << this->y << "\n";}

int point::ccw(point p1, point p2, point p3){return ((p2.x - p1.x) * (p3.y - p1.y) - (p2.y - p1.y) * (p3.x - p1.x));}

bool point::operator==(point p1){return (this->x==p1.x)&&(this->y==p1.y);}

bool point::comp_by_polarangle(point p1, point p2){
    int ccw = point::ccw(*this, p1, p2);
    return (ccw == 0) ? (p1.D() <= p2.D()) : (ccw < 0);
}

bool point::comp_by_coordinate(point p1, point p2){
    return (p1.y == p2.y) ? (p1.x <= p2.x) : (p1.y < p2.y);  // Inplace
}

void print_point_vec(vector<point> &vp){
    for (auto i: vp){
        i.print();
    }
}

void graham_scan(vector<point> &vp){
    int N = (int)vp.size();
    vector<point> s;
    //bubble_sort(vp, point::comp_by_coordinate);
    std::sort(vp.begin(),vp.end(),point::comp_by_coordinate);
    point p0 = vp[0];
    vp.erase(vp.begin());  // Move the first item to s.
    comp = std::bind(&point::comp_by_polarangle, p0, std::placeholders::_1, std::placeholders::_2);
    std::sort(vp.begin(),vp.end(),comp);
    //bubble_sort(vp, comp);  
    vector<point>::iterator it = vp.begin();
    vector<int> erase_list;
    for (int i=0; i<N-2; i++){
        if (point::ccw(p0,vp[i],vp[i+1])==0 && (vp[i].X()*vp[i+1].X()>=0 || vp[i].Y()*vp[i+1].Y()>=0)){
            erase_list.insert(erase_list.begin(),i);
        }
    }
    for (auto i: erase_list){
        vp.erase(it+i);
    }
    if (p0==vp[0]){
        vp.erase(it);
    }
    N = (int)vp.size();
    for (int i=0; i<N; i++){
        while(s.size()>1 && point::ccw(s[1],s[0],vp[i])>0){
            s.erase(s.begin());
        }
        s.insert(s.begin(),vp[i]);
    }
    s.insert(s.begin(),p0);
    print_point_vec(s);
} 

int main(){
    int N = 0;
    cin >> N;
    vector<point> vp;
    for (int n=0; n<N; n++){
        int x, y;
        cin >> x >> y;
        vp.push_back(point(x,y));
    }
    if (vp.size()>0){
        graham_scan(vp);
    }
    return 0;
}
