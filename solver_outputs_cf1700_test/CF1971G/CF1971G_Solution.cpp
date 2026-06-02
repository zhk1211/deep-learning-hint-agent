#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int t;
    cin >> t;
    while (t--) {
        int n;
        cin >> n;
        vector<int> a(n);
        for (int i = 0; i < n; ++i) {
            cin >> a[i];
        }
        
        map<int, priority_queue<int, vector<int>, greater<int>>> groups;
        for (int x : a) {
            int key = x >> 2;
            groups[key].push(x);
        }
        
        for (int i = 0; i < n; ++i) {
            int key = a[i] >> 2;
            cout << groups[key].top() << " \n"[i == n - 1];
            groups[key].pop();
        }
    }
    return 0;
}
