#include <iostream>
#include <vector>
using namespace std;

double calc_porland(const string& exp) {
    vector<double> stack;
    for(char c : exp) {
        if('0' <= c && c <= '9') {
            int n = c - '0';
            stack.push_back(n);
        } else {
            double right = stack.back();
            stack.pop_back();
            double left = stack.back();
            stack.pop_back();

            if(c == '+') {
                stack.push_back(left + right);
            } else if(c == '-') {
                stack.push_back(left - right);
            } else if(c == '*') {
                stack.push_back(left * right);
            } else if(c == '/') {
                stack.push_back(left / right);
            }
        }
    }
    return stack.back();
}

int main() {
    cout << calc_porland("612+*8-") << endl;
}