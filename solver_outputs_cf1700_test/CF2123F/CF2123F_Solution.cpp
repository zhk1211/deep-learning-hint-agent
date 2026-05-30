#include <bits/stdc++.h>
using namespace std;

const int MAXN = 100000;
int lpd[MAXN + 1]; // largest prime divisor

void precompute() {
    for (int i = 2; i <= MAXN; ++i) {
        if (lpd[i] == 0) { // i is prime
            for (int j = i; j <= MAXN; j += i) {
                lpd[j] = i;
            }
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    precompute();

    int t;
    cin >> t;

    vector<int> groups[MAXN + 1];
    vector<int> used_primes;

    while (t--) {
        int n;
        cin >> n;

        vector<int> p(n + 1);
        p[1] = 1;

        for (int i = 2; i <= n; ++i) {
            int pr = lpd[i];
            if (groups[pr].empty()) {
                used_primes.push_back(pr);
            }
            groups[pr].push_back(i);
        }

        for (int pr : used_primes) {
            auto& v = groups[pr];
            int sz = v.size();
            if (sz == 1) {
                p[v[0]] = v[0];
            } else {
                for (int i = 0; i < sz; ++i) {
                    p[v[i]] = v[(i + 1) % sz];
                }
            }
            v.clear();
        }
        used_primes.clear();

        for (int i = 1; i <= n; ++i) {
            cout << p[i] << " \n"[i == n];
        }
    }

    return 0;
}
