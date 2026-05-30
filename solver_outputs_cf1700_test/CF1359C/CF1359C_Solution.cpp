// Solution
#include <iostream>
using namespace std;
typedef long long ll;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;
    cin >> T;
    while (T--) {
        ll h, c, t;
        cin >> h >> c >> t;
        if (t == h) {
            cout << "1\n";
            continue;
        }
        if (2 * t <= h + c) {
            cout << "2\n";
            continue;
        }
        ll k = (h - t) / (2 * t - h - c);
        ll cups1 = 2 * k + 1;
        ll cups2 = cups1 + 2;
        ll A = k * (h + c) + h - t * cups1;
        if (A < 0) A = -A;
        ll B = (k + 1) * (h + c) + h - t * cups2;
        if (B < 0) B = -B;
        if (A * cups2 <= B * cups1) {
            cout << cups1 << "\n";
        } else {
            cout << cups2 << "\n";
        }
    }
    return 0;
}
