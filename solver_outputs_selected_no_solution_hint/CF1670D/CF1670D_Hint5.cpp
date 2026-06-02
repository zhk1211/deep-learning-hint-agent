#include <bits/stdc++.h>
using namespace std;

using ll = long long;

ll max_triangles(ll k) {
    // maximum number of triangles with k lines
    // derived from the pattern: floor(k^2 / 4) * 2 + something
    // Actually formula: if k%2==0: (k/2)*(k/2)*2 + (k/2)*2? Let's derive properly.
    // From hints: quadratic function. Let's compute small values:
    // k=1:0, k=2:2, k=3:6, k=4:10, k=5:16, k=6:22, k=7:30, k=8:38
    // Differences: 2,4,4,6,6,8,8,... 
    // So formula: if k even: (k/2)*(k/2)*2 + (k/2)*? Actually:
    // k=2: 2 = 2*1
    // k=4: 10 = 2*5? No.
    // Let's use: max triangles = 2 * floor(k^2/4) + something?
    // k=2: floor(4/4)=1 -> 2*1=2. k=3: floor(9/4)=2 -> 2*2=4 but actual is 6.
    // Better: max triangles = 2 * ( (k/2) * ((k+1)/2) )? 
    // k=2: (1)*(1)=1 -> 2. k=3: (1)*(2)=2 -> 4, not 6.
    // Let's derive from pattern: 
    // For even k=2m: triangles = 2*m^2 + 2*m? k=2: m=1 -> 2+2=4 no.
    // Actually from differences: 
    // k=1:0
    // k=2:2  (+2)
    // k=3:6  (+4)
    // k=4:10 (+4)
    // k=5:16 (+6)
    // k=6:22 (+6)
    // k=7:30 (+8)
    // k=8:38 (+8)
    // So for k=2m: sum_{i=1}^{m} (2i) + sum_{i=1}^{m-1} (2i) = 2*(m(m+1)/2) + 2*((m-1)m/2) = m(m+1) + (m-1)m = m^2+m + m^2-m = 2m^2.
    // Wait: k=2: m=1 -> 2*1=2 correct.
    // k=4: m=2 -> 2*4=8 but actual is 10. So off by 2.
    // Let's recalc differences: 
    // k=1:0
    // k=2:2  diff=2
    // k=3:6  diff=4
    // k=4:10 diff=4
    // k=5:16 diff=6
    // k=6:22 diff=6
    // k=7:30 diff=8
    // k=8:38 diff=8
    // So sequence of increments: 2,4,4,6,6,8,8,...
    // For k=2m: sum of first (2m-1) increments? Actually k lines means we added k lines, starting from 0 lines -> 0 triangles.
    // After 1 line: 0
    // After 2 lines: 2
    // After 3 lines: 6
    // So triangles after k lines = sum_{i=1}^{k-1} inc_i, where inc_1=2, inc_2=4, inc_3=4, inc_4=6, inc_5=6,...
    // So for k=2m: sum = 2 + (4+4) + (6+6) + ... + (2m + 2m?) Wait pattern: pairs of equal numbers starting from 2.
    // Actually inc_{2}=4, inc_{3}=4, inc_{4}=6, inc_{5}=6, inc_{6}=8, inc_{7}=8,...
    // So for k=2m, we have inc_1=2, then for i=2 to m: inc_{2i-2}=2i? Let's index properly.
    // Better: Let's list k and triangles T(k):
    // k=1:0
    // k=2:2
    // k=3:6
    // k=4:10
    // k=5:16
    // k=6:22
    // k=7:30
    // k=8:38
    // k=9:48? Let's check pattern: increments: 2,4,4,6,6,8,8,10,10,... So T(9)=38+10=48.
    // So formula: T(2m) = 2*m^2? For m=1:2, m=2:8 but actual 10. So not.
    // Let's compute T(2m): sum_{i=1}^{m} (2i) + sum_{i=1}^{m-1} (2i) = 2*(m(m+1)/2) + 2*((m-1)m/2) = m(m+1) + (m-1)m = 2m^2.
    // But T(4)=10, 2*2^2=8. Missing 2.
    // Because inc_1=2 is not paired? Actually inc_1=2, inc_2=4, inc_3=4, inc_4=6,... So for k=4 (m=2), increments: inc_1=2, inc_2=4, inc_3=4. Sum = 2+4+4=10. My sum formula gave 8 because I did sum_{i=1}^{2} 2i = 2+4=6, plus sum_{i=1}^{1} 2i = 2, total 8. But actual sum is 2 + 4 + 4 = 10. So inc_1 is 2, then pairs (4,4), (6,6), etc. So for k=2m, we have inc_1=2, and then (m-1) pairs of (2i) for i=2..m? Let's see: m=2 -> pairs: i=2 gives (4,4) -> one pair. So sum = 2 + 2*4 = 10. For m=3 (k=6): inc_1=2, pairs (4,4) and (6,6) -> sum = 2 + 8 + 12 = 22. Correct.
    // So T(2m) = 2 + sum_{i=2}^{m} (2 * 2i) = 2 + 4 * sum_{i=2}^{m} i = 2 + 4 * ( (m(m+1)/2) - 1 ) = 2 + 2m(m+1) - 4 = 2m(m+1) - 2.
    // Check: m=1: 2*1*2 -2 = 2, correct. m=2: 2*2*3 -2 = 12-2=10, correct. m=3: 2*3*4 -2 = 24-2=22, correct.
    // For odd k=2m+1: T(2m+1) = T(2m) + inc_{2m}? inc_{2m} is the second of the pair? Actually inc_{2m} = 2(m+1)? Let's check: k=3 (m=1): T(3)=6, T(2)=2, inc_2=4. inc_2 = 2*(1+1)=4. k=5 (m=2): T(5)=16, T(4)=10, inc_4=6 = 2*(2+1)=6. So inc_{2m} = 2(m+1). So T(2m+1) = T(2m) + 2(m+1) = 2m(m+1) - 2 + 2m+2 = 2m(m+1) + 2m = 2m(m+2).
    // Check: m=1: 2*1*3=6, correct. m=2: 2*2*4=16, correct. m=3: 2*3*5=30, correct.
    // So formulas:
    // if k even: k=2m -> T = 2m(m+1) - 2
    // if k odd: k=2m+1 -> T = 2m(m+2)
    // We can unify: T(k) = 2 * floor(k/2) * ceil(k/2) + (k%2==0? -2 : 0)? Let's test: k=2: floor=1, ceil=1 -> 2*1*1=2, even so -2? No, 2-2=0 wrong. So not.
    // Better keep piecewise.

    if (k <= 1) return 0;
    if (k % 2 == 0) {
        ll m = k / 2;
        return 2 * m * (m + 1) - 2;
    } else {
        ll m = k / 2;
        return 2 * m * (m + 2);
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--) {
        ll n;
        cin >> n;
        // binary search for minimum k such that max_triangles(k) >= n
        ll lo = 1, hi = 1;
        while (max_triangles(hi) < n) {
            hi *= 2;
        }
        while (lo < hi) {
            ll mid = lo + (hi - lo) / 2;
            if (max_triangles(mid) >= n) {
                hi = mid;
            } else {
                lo = mid + 1;
            }
        }
        cout << lo << '\n';
    }
    return 0;
}
