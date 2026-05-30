// Hint0
#include <bits/stdc++.h>
using namespace std;

struct Friend {
    int h, w, id;
};

void solve() {
    int n;
    cin >> n;
    vector<Friend> a(n);
    for (int i = 0; i < n; ++i) {
        cin >> a[i].h >> a[i].w;
        a[i].id = i;
    }
    vector<int> ans(n, -1);
    // We will try two orientations: (h,w) and (w,h)
    // For each orientation, we sort by h, then keep track of minimum w seen so far.
    // We need to find a friend with both dimensions strictly smaller.
    // We'll process in two passes: one with original (h,w) and one with swapped (w,h).
    // For each pass, we sort by h ascending, and for equal h we sort by w ascending.
    // We maintain the minimum w and the index of that minimum w among processed friends.
    // When we process a friend, we check if the minimum w seen so far is < his w.
    // If yes, that friend can be placed in front.
    // But careful: we need strictly smaller h and w. Since we sort by h, all processed have h <= current h.
    // To ensure h is strictly smaller, we should only consider friends with h < current h.
    // So we can group by h, and update the minimum after processing all friends with the same h.
    
    auto process = [&](bool swapped) {
        vector<Friend> b = a;
        if (swapped) {
            for (auto& f : b) swap(f.h, f.w);
        }
        sort(b.begin(), b.end(), [](const Friend& x, const Friend& y) {
            if (x.h != y.h) return x.h < y.h;
            return x.w < y.w;
        });
        int min_w = INT_MAX;
        int min_id = -1;
        int i = 0;
        while (i < n) {
            int j = i;
            while (j < n && b[j].h == b[i].h) ++j;
            // For all friends in this group, check if there is a friend with smaller h and smaller w
            for (int k = i; k < j; ++k) {
                if (min_w < b[k].w) {
                    ans[b[k].id] = min_id + 1; // 1-indexed
                }
            }
            // Update min_w with this group
            for (int k = i; k < j; ++k) {
                if (b[k].w < min_w) {
                    min_w = b[k].w;
                    min_id = b[k].id;
                }
            }
            i = j;
        }
    };
    
    process(false);
    process(true);
    
    for (int i = 0; i < n; ++i) {
        cout << ans[i] << " \n"[i == n-1];
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t;
    cin >> t;
    while (t--) {
        solve();
    }
    return 0;
}
