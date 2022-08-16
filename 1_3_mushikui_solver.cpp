#include <iostream>
#include <vector>
#include <string>
#include <utility>
using namespace std;

// 整数valが文字列strに整合するか
bool is_valid(int64_t val, const string& str) {
    string sval = to_string(val);

    if(sval.size() != str.size()) return false;

    for(int i = 0; i < sval.size(); ++i) {
        // □については不問
        if(str[i] == '*') continue;

        if(sval[i] != str[i]) return false;
    }
    return true;
}

// 数字vを文字列strの右からk番目に当てはめたときに整合するかどうか
bool is_valid_sub(int v, int k, const string& str) {
    // 右からk番目の文字を取得する
    char c = str[(int)str.size() - 1 - k];

    // 虫食いの場合は問題ない
    if(c == '*') return true;

    // たとえばc = '8'のとき、real_val = 8になる
    int real_val = c - '0';

    return (v == real_val);
}

// □に入れた数字から、整合全体を復元する
int64_t decode(const vector<int>& vec) {
    int64_t res = 0;  // 答え
    int64_t order = 1;  // 1, 10, 100, ...

    for(int v : vec) {
        res += order * v;
        order *= 10;
    }
    return res;
}

class Mushikuizan {
    private:
        // 被乗数、乗数、積
        string multiplicand_, multiplier_, product_;

        // 部分積
        vector<string> middle_;

        // 答えを表す配列（被乗数と乗数のペアを格納）
        vector<pair<int64_t, int64_t>> res_;

    public:
        Mushikuizan(const string& multiplicand,
                    const string& multiplier,
                    const string& product,
                    const vector<string>& middle):
            multiplicand_(multiplicand), multiplier_(multiplier),
            product_(product), middle_(middle) {
            }

        // 乗算の□に数字を入れていく再帰関数
        // plicand: 被乗数に入れた数字、vec: 乗数の□に数字を入れた結果
        void rec_plier(int64_t plicand, vector<int>& vec) {
            // 終端条件: 乗数の□のすべてに数字が入ったとき
            if(vec.size() == multiplier_.size()) {
                // 乗数を求める
                int64_t plier = decode(vec);

                // 積の整合性を確認する
                if(!is_valid(plicand * plier, product_)) return;

                // 答えを格納する
                res_.emplace_back(plicand, plier);
                return;
            }

            // 新しい□に数字を入れる（乗数に0は入らないものとする）
            for(int add = 1; add <= 9; ++add) {
                // 既に数字が入っていて矛盾している場合はスキップ
                if(!is_valid_sub(add, vec.size(), multiplier_))
                    continue;

                // □に数字addを入れて整合性を確認する
                if(!is_valid(plicand * add, middle_[vec.size()]))
                    continue;

                // vecに数字addを加えて再帰呼び出し
                vec.push_back(add);
                rec_plier(plicand, vec);
                vec.pop_back();
            }
        }

        // 被乗数の□に数字を入れていく関数
        // vec: 被乗数の□に数字を入れた結果
        void rec_plicand(vector<int>& vec) {
            // 終端条件: 被乗数の□のすべてに数字が入ったとき
            if(vec.size() == multiplicand_.size()) {
                // 今度は乗数の□に数字を入れていく
                vector<int> vec2;
                rec_plier(decode(vec), vec2);
                return;
            }

            // 新しい□に数字を入れる
            for(int add = 0; add <= 9; ++add) {
                // 左端に0は入れられない
                if(vec.empty() && add == 0) continue;

                // 既に数字が入っていて矛盾している場合はスキップ
                if(!is_valid_sub(add, vec.size(), multiplicand_))
                    continue;

                // vecに数値addを加えて再帰呼び出し
                vec.push_back(add);
                rec_plicand(vec);
                vec.pop_back();
            }
        }

        // 虫食い算を解く（被乗数と乗数のペアを格納した配列を返す）
        vector<pair<int64_t, int64_t>> solve() {
            // 答えを表す配列を空にする
            res_.clear();

            // 被乗数の□のほうから数字を入れていく
            vector<int> vec;  // 空の配列
            rec_plicand(vec);
            return res_;
        }
};

int main() {
    // 入力
    cout << "Mushikuizan Input: " << endl;
    int A, B;  // 被乗数、乗数の桁数
    cin >> A >> B;
    string hijou, jou, seki;  // 被乗数、乗数、積
    vector<string> middle(B); // 中間部
    cin >> hijou >> jou;
    for (int i = 0; i < B; ++i) cin >> middle[i];
    cin >> seki;
    //再帰的に解く
    Mushikuizan mu(hijou, jou, seki, middle);
    const vector<pair<int64_t, int64_t>>& res = mu.solve();

    // 解を出力
    cout << "The num of solutions: " << res.size() << endl;
    for(int i = 0; i < res.size(); ++i) {
        cout << i << " th solution: "
             << res[i].first << " * " << res[i].second
             << " = " << res[i].first * res[i].second << endl;
    }
}