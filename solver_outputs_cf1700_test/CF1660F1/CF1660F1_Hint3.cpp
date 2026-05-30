// Hint3
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
        string s;
        cin >> s;
        
        vector<int> pref(n + 1, 0);
        for (int i = 0; i < n; ++i) {
            pref[i + 1] = pref[i] + (s[i] == '+' ? 1 : -1);
        }
        
        long long ans = 0;
        for (int i = 0; i < n; ++i) {
            int min_balance = 0;
            int cur_balance = 0;
            for (int j = i; j < n; ++j) {
                cur_balance += (s[j] == '+' ? 1 : -1);
                min_balance = min(min_balance, cur_balance);
                int seg_balance = pref[j + 1] - pref[i];
                if (seg_balance <= 0 && seg_balance % 3 == 0) {
                    // Check if we can apply operations without getting stuck
                    // The condition: number of minuses >= number of pluses + 2 at any prefix?
                    // Actually, from hints: if balance <= 0 and balance % 3 == 0, it's promising
                    // But we need to ensure we can always reduce. 
                    // The necessary and sufficient condition: balance <= 0 and balance % 3 == 0
                    // However, there is a catch: if there is a prefix where minus count < plus count?
                    // Let's think: operation reduces minus count by 2, increases plus by 1, net balance change +3.
                    // Starting from a segment with balance <= 0 and %3==0, we can always apply operations?
                    // Consider "++-" balance = 1, not promising. 
                    // Consider "-+-" balance = -1, not %3. 
                    // Consider "---" balance = -3, promising.
                    // What about "--+"? balance = -1, not %3.
                    // What about "-+-+"? balance = 0, promising.
                    // The condition from hints: balance <= 0 and balance % 3 == 0.
                    // But is it sufficient? Example: "+--" balance = 0? +--: plus=1, minus=2, balance = -1? Wait: + is +1, - is -1. So "+--" balance = 1 - 2 = -1. Not %3.
                    // Example: "+++---" balance = 0, promising.
                    // Example: "-+---" balance = -2? -+---: minus=4, plus=1 => balance = -3. %3==0, promising.
                    // So condition seems correct.
                    ans++;
                }
            }
        }
        cout << ans << '\n';
    }
    return 0;
}
