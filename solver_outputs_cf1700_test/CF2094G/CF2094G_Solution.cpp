// Solution
#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--) {
        int q;
        cin >> q;
        deque<ll> dq;
        ll score = 0;
        ll sum = 0;
        ll sz = 0;
        bool rev = false;

        auto get_front = [&]() -> ll& {
            return rev ? dq.back() : dq.front();
        };
        auto get_back = [&]() -> ll& {
            return rev ? dq.front() : dq.back();
        };
        auto pop_front = [&]() {
            if (rev) dq.pop_back();
            else dq.pop_front();
        };
        auto pop_back = [&]() {
            if (rev) dq.pop_front();
            else dq.pop_back();
        };
        auto push_front = [&](ll val) {
            if (rev) dq.push_back(val);
            else dq.push_front(val);
        };
        auto push_back = [&](ll val) {
            if (rev) dq.push_front(val);
            else dq.push_back(val);
        };

        while (q--) {
            int op;
            cin >> op;
            if (op == 1) {
                // cyclic shift: move back to front
                ll back_val = get_back();
                pop_back();
                // remove back contribution
                score -= back_val * sz;
                // add front contribution
                score += sum;
                push_front(back_val);
                // sum and sz unchanged
            } else if (op == 2) {
                rev = !rev;
                // score becomes rscore = (sz + 1) * sum - score
                score = (sz + 1) * sum - score;
            } else if (op == 3) {
                ll k;
                cin >> k;
                sz++;
                sum += k;
                // append to back
                push_back(k);
                // new element contributes k * sz to score
                score += k * sz;
            }
            cout << score << '\n';
        }
    }
    return 0;
}
