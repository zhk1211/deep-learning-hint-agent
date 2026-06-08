#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    long long m;
    cin >> n >> m;
    
    vector<long long> a(n);
    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }
    
    sort(a.begin(), a.end());
    
    long long free_space = m;
    long long total_time = 0;
    
    for (int i = 0; i < n; i++) {
        if (free_space >= a[i]) {
            free_space -= a[i];
            total_time += a[i];
        } else {
            long long need = a[i] - free_space;
            total_time += need + a[i];
            free_space = 0;
        }
        total_time += 1;
        free_space += a[i];
    }
    
    cout << total_time << "\n";
    return 0;
}
