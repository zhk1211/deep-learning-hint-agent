// Solution
#include <iostream>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t;
    cin >> t;
    while (t--) {
        int n, k;
        cin >> n >> k; // k is always 2 in this version
        for (int i = 0; i < n; ++i) {
            int y = (i == 0) ? 0 : (i ^ (i - 1));
            cout << y << endl;
            int r;
            cin >> r;
            if (r == 1) {
                break;
            }
            if (r == -1) {
                return 0;
            }
        }
    }
    return 0;
}
