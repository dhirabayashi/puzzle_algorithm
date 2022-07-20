#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <cmath>
#include <utility>

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

// テンパズルソルバー
// val: 4つの数を格納した配列、target: 作りたい数
vector<string> solve(vector<int> val, int target) {
    // 答えを表す計算式を格納する配列
    vector<string> res;

    // 逆ポーランド記法の計算式expを試すための関数オブジェクト
    const double EPS = 1e-9;  // 十分に小さい数
    auto check = [&](const string& exp) -> void {
        // 計算結果と作りたい数との差が十分に小さいとき、一致とみなす
        if (abs(calc_porland(exp) - target) < EPS)
            res.push_back(decode_porland(exp));
    };

    // 4つの数valの並び替えを順に試していく
    sort(val.begin(), val.end());  // valを辞書順最小にする
    do {
        // 4つの数字を連結してできる文字列foursを作る
        string fours = "";
        for(int v : val) fours += to_string(v);

        // 4^3 = 64通りの演算子の組み合わせを試していく
        const string ops = "+-*/";
        for(char op1 : ops) {
            for(char op2 : ops) {
                for(char op3 : ops) {
                    // まず、パターン"xxxxooo"を作る
                    string exp = fours + op1 + op2 + op3;

                    // パターン"xxxxooo"を試す
                    check(exp);

                    // パターン"xxxoxoo"を試す
                    swap(exp[3], exp[4]), check(exp);

                    // パターン"xxxooxo"を試す
                    swap(exp[4], exp[5]), check(exp);

                    // パターン"xxoxoxo"を試す
                    swap(exp[2], exp[3]), check(exp);

                    // パターン"xxoxxoo"を試す
                    swap(exp[4], exp[5]), check(exp);
                }
            }
        }
    } while(next_permutation(val.begin(), val.end()));

    return res;
}

int main() {
    // 4つの数と、作りたい数の入力
    vector<int> val(4);  // 4つの数
    int target;  // 作りたい数
    for(int i = 0; i < 4; ++i) {
        cout << i + 1 << " th number: ";
        cin >> val[i];
    }
    cout << "target number: ";
    cin >> target;

    // テンパズルを解く
    vector<string> res = solve(val, target);

    // 出力
    for(const string& exp : res)
        cout << exp << " = " << target << endl;
}