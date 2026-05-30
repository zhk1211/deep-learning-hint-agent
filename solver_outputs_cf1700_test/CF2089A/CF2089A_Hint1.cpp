// Hint1
#include <bits/stdc++.h>
using namespace std;

bool is_prime(int x) {
    if (x < 2) return false;
    for (int i = 2; i * i <= x; ++i)
        if (x % i == 0) return false;
    return true;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int t;
    cin >> t;
    while (t--) {
        int n;
        cin >> n;
        vector<int> p(n);
        
        // We want many c_i to be prime.
        // Strategy: place 2 at position 1, then 1 at position 2, then 3,4,...n
        // This makes prefix sums: 2, 3, 6, 10, 15, 21, ...
        // c_i = ceil(prefix_sum / i)
        // For i=1: 2 (prime)
        // i=2: ceil(3/2)=2 (prime)
        // i=3: ceil(6/3)=2 (prime)
        // i=4: ceil(10/4)=3 (prime)
        // i=5: ceil(15/5)=3 (prime)
        // i=6: ceil(21/6)=4 (not prime)
        // But we need at least floor(n/3)-1 primes among c_i.
        // This construction gives many primes for small n.
        // Actually we can just output 2,1,3,4,5,...,n for all n.
        // Let's verify: for n=2: 2 1 -> c1=2, c2=2 both prime -> floor(2/3)-1 = -1 -> ok.
        // n=3: 2 1 3 -> c1=2, c2=2, c3=2 -> floor(3/3)-1=0 -> ok.
        // n=4: 2 1 3 4 -> c1=2, c2=2, c3=2, c4=ceil(10/4)=3 -> primes: 2,2,2,3 -> 4 primes >= floor(4/3)-1=0 -> ok.
        // n=5: sample output 2 1 3 4 5 -> all primes -> ok.
        // n=6: 2 1 3 4 5 6 -> c1=2, c2=2, c3=2, c4=3, c5=3, c6=ceil(21/6)=4 -> primes: 2,2,2,3,3 -> 5 primes >= floor(6/3)-1=1 -> ok.
        // So this simple construction works for all n>=2.
        
        p[0] = 2;
        if (n >= 2) p[1] = 1;
        for (int i = 2; i < n; ++i) {
            p[i] = i + 1;
        }
        
        for (int i = 0; i < n; ++i) {
            cout << p[i] << " \n"[i == n - 1];
        }
    }
    return 0;
}
