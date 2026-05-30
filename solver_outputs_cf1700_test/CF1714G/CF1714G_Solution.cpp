// Solution
#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>
#include <tuple>

using namespace std;

typedef long long ll;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int t;
    cin >> t;
    while (t--) {
        int n;
        cin >> n;
        vector<vector<int>> adj(n + 1);
        vector<ll> a_edge(n + 1), b_edge(n + 1);
        for (int j = 2; j <= n; ++j) {
            int p;
            ll a, b;
            cin >> p >> a >> b;
            adj[p].push_back(j);
            adj[j].push_back(p);
            a_edge[j] = a;
            b_edge[j] = b;
        }
        
        vector<int> ans(n + 1);
        vector<ll> b_pref;
        b_pref.reserve(n + 1);
        b_pref.push_back(0);
        ll a_sum = 0;
        
        // stack elements: node, parent, state (0 = enter, 1 = exit)
        stack<tuple<int, int, int>> st;
        st.push({1, 0, 0});
        
        while (!st.empty()) {
            auto [u, p, state] = st.top();
            st.pop();
            
            if (state == 0) {
                if (u != 1) {
                    a_sum += a_edge[u];
                    b_pref.push_back(b_pref.back() + b_edge[u]);
                    // binary search for the longest prefix with b_sum <= a_sum
                    int idx = upper_bound(b_pref.begin(), b_pref.end(), a_sum) - b_pref.begin() - 1;
                    ans[u] = idx;
                }
                st.push({u, p, 1}); // push exit state
                for (int v : adj[u]) {
                    if (v != p) {
                        st.push({v, u, 0});
                    }
                }
            } else {
                if (u != 1) {
                    a_sum -= a_edge[u];
                    b_pref.pop_back();
                }
            }
        }
        
        for (int i = 2; i <= n; ++i) {
            cout << ans[i] << (i == n ? '\n' : ' ');
        }
    }
    return 0;
}
