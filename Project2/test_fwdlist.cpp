#include <forward_list>
#include <vector>
#include <iostream>
using namespace std;



int main()
{
    vector<forward_list<int> > vec;
    forward_list<int> f1;
    vec.push_back(f1);
    vector<forward_list<int> >::iterator it_v = vec.begin();
    it_v->insert_after(it_v->before_begin(), 4);
    it_v->insert_after(it_v->before_begin(), 3);
    it_v->insert_after(it_v->before_begin(), 2);
    it_v->insert_after(it_v->before_begin(), 1);
    for (auto it_f = it_v->begin(); it_f != it_v->end(); ++it_f) {
        cout << *(it_f) << endl;
    }
    return 0;
}