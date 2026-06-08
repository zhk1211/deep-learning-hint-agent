#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--) {
        string n_str;
        cin >> n_str;
        int n_len = (int)n_str.size();
        long long n_val = stoll(n_str);

        vector<pair<long long, long long>> ans;

        // a from 1 to 10000
        for (long long a = 1; a <= 10000; ++a) {
            long long total_len = a * n_len;
            // b must be less than total_len to have non-empty string
            // also b <= min(10000, a * n_val)
            long long max_b = min(10000LL, a * n_val);
            if (max_b >= total_len) max_b = total_len - 1;
            if (max_b < 1) continue;

            // We need to find b such that:
            // The string s = n_str repeated a times, then last b chars removed.
            // The remaining string, interpreted as integer, equals a * n_val - b.
            // Let the remaining length be L = total_len - b.
            // The integer value of the remaining string is formed by taking the first L characters of s.
            // Since s is periodic with period n_len, the first L characters are determined by L.
            // Let the integer value be X.
            // We need X = a * n_val - b.
            // Also b = total_len - L.
            // So X = a * n_val - (total_len - L) = a * n_val - total_len + L.
            // But X is also the integer formed by the first L digits of the repeated n_str.
            // We can try possible L from 1 to min(6, total_len) maybe? Actually L can be up to total_len-1.
            // But note that a*n_val - b can be up to a*n_val - 1, which has at most about 7 digits (since a<=10000, n<=100, a*n<=1e6).
            // So the correct answer has at most 7 digits. Thus L must be at most 7.
            // Also L <= total_len - 1.
            // So we can iterate L from 1 to min(7, total_len - 1).
            int max_L = min(7, (int)total_len - 1);
            for (int L = 1; L <= max_L; ++L) {
                long long b = total_len - L;
                if (b < 1 || b > 10000 || b > a * n_val) continue;
                // Build the integer from first L characters of repeated n_str
                long long X = 0;
                for (int i = 0; i < L; ++i) {
                    X = X * 10 + (n_str[i % n_len] - '0');
                }
                if (X == a * n_val - b) {
                    ans.emplace_back(a, b);
                    break; // only one b per a? Actually there could be multiple L for same a? But constraints say output all pairs.
                    // We'll collect all.
                }
            }
            // Also check L up to maybe larger? But correct answer length is limited.
            // Actually we can also check L = total_len? No, b>=1 so L<total_len.
            // To be safe, we can iterate L from 1 to total_len-1, but total_len can be up to 1e6, too large.
            // However, we only need L such that X = a*n_val - b, and X has at most 7 digits.
            // So L <= 7. So the above is sufficient.
        }

        // But wait, the sample output for n=2 has a=20, b=18; a=219, b=216; a=2218, b=2214.
        // For a=20, total_len=20*1=20, L=2, b=18, X=22, correct.
        // For a=219, total_len=219, L=3, b=216, X=222? 219*2 - 216 = 438-216=222, yes.
        // For a=2218, total_len=2218, L=4, b=2214, X=2222? 2218*2-2214=4436-2214=2222, yes.
        // So our method works.

        // However, we must output all pairs. The above loop finds one b per a if exists.
        // But could there be multiple b for same a? Let's check: For fixed a, total_len fixed.
        // b = total_len - L. Different L give different b. X depends on L.
        // We need X = a*n_val - b = a*n_val - total_len + L.
        // So X - L = a*n_val - total_len (constant for fixed a).
        // X is the integer formed by first L digits. As L increases, X increases but not linearly.
        // It's possible that for two different L, the equation holds? Unlikely but we can just collect all.
        // We'll modify to check all L from 1 to min(7, total_len-1) and collect all valid b.

        // Let's rewrite the loop properly.
        ans.clear();
        for (long long a = 1; a <= 10000; ++a) {
            long long total_len = a * n_len;
            long long max_b = min(10000LL, a * n_val);
            if (max_b >= total_len) max_b = total_len - 1;
            if (max_b < 1) continue;
            int max_L = min(7, (int)total_len - 1);
            for (int L = 1; L <= max_L; ++L) {
                long long b = total_len - L;
                if (b < 1 || b > max_b) continue;
                long long X = 0;
                for (int i = 0; i < L; ++i) {
                    X = X * 10 + (n_str[i % n_len] - '0');
                }
                if (X == a * n_val - b) {
                    ans.emplace_back(a, b);
                }
            }
        }

        cout << ans.size() << '\n';
        for (auto &p : ans) {
            cout << p.first << ' ' << p.second << '\n';
        }
    }
    return 0;
}
