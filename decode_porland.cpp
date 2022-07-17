#include <iostream>
#include <vector>
using namespace std;

string decode_porland(const string& exp) {
    vector<string> space;
    for(char c : exp) {
        if('0' <= c && c <= '9') {
            space.push_back({c});
        } else {
            string second = space.back();
            space.pop_back();
            string first = space.back();
            space.pop_back();

            if(c == '*' || c == '/') {
                if(first.size() > 1) {
                    first = "(" + first + ")";
                }
                if(second.size() > 1) {
                    second = "(" + second + ")";
                }
            }

            space.push_back(first + " " + c + " " + second);
        }
    }
    return space.back();
}

int main() {
    cout << decode_porland("612+*8-") << endl;
}