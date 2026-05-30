// Hint3
#include <bits/stdc++.h>
using namespace std;

void solve() {
    int n;
    cin >> n;
    string resp;

    // Step 1: mul 9
    cout << "mul 9" << endl;
    cin >> resp;
    if (resp == "-1") exit(0);

    // Step 2: digit
    cout << "digit" << endl;
    cin >> resp;
    if (resp == "-1") exit(0);

    // Step 3: digit
    cout << "digit" << endl;
    cin >> resp;
    if (resp == "-1") exit(0);

    // Now x is a single digit (1-9) because after mul 9 and two digits,
    // any number's digit sum eventually becomes a single digit.
    // We need to reach n. We have one command left.
    // We can use add, mul, div, or digit.
    // Since x is a single digit, we can try to reach n by adding (n - x).
    // But we don't know x. We can try to add something that works for all possible x.
    // Actually, we can use the last command to adjust.
    // We know n is up to 1e9. We can try to multiply by something to get n.
    // But x is unknown. We can use "add" with a value that makes x + y = n for all possible x?
    // That's impossible. Instead, we can use "mul" to get a number that we can then adjust?
    // Wait, we have exactly 4 commands total. We used 3. One left.
    // After two digits, x is in [1,9]. We need to reach n.
    // We can do "add (n - 1)"? But if x != 1, it might overflow or underflow.
    // Better: we can do "mul something" then "!" but we only have one command left.
    // Actually, we can use the fact that we can answer "!" at any time.
    // The "!" does not count as a command. So after 4 commands, we must answer.
    // We have one command left. We need to make x equal to n.
    // Since x is a single digit, we can try to multiply by something to get n, but n may not be a multiple of x.
    // We can instead use "add" with a value that works for all x? No.
    // Let's think: after two digits, x is the digital root of original x * 9.
    // The digital root of any number multiplied by 9 is 9, unless the original number was 0, but x>=1.
    // Actually, digital root of (x * 9) is 9 if x is not a multiple of 9? Wait.
    // The digital root of any positive integer is its value mod 9, except 0 becomes 9.
    // So digital root of (x * 9) is 9, because x*9 is divisible by 9, so digital root is 9.
    // But wait: if x*9 = 0? No, x>=1 so x*9 >=9.
    // So after "mul 9", x becomes x*9. Then "digit" gives sum of digits of x*9.
    // That sum is a multiple of 9? Not necessarily, but its digital root is 9.
    // After first digit, we get some number. After second digit, we get the digital root, which is 9.
    // So after two digits, x is exactly 9! Always!
    // Let's verify: if original x=1, mul 9 -> 9, digit -> 9, digit -> 9.
    // x=2 -> 18 -> digit -> 9 -> digit -> 9.
    // x=3 -> 27 -> 9 -> 9.
    // x=4 -> 36 -> 9 -> 9.
    // x=5 -> 45 -> 9 -> 9.
    // x=6 -> 54 -> 9 -> 9.
    // x=7 -> 63 -> 9 -> 9.
    // x=8 -> 72 -> 9 -> 9.
    // x=9 -> 81 -> 9 -> 9.
    // x=10 -> 90 -> 9 -> 9.
    // Yes! After "mul 9" and two "digit", x is always 9.
    // So we know x = 9 after 3 commands.
    // Now we have one command left to turn 9 into n.
    // We can do "add (n - 9)" if n-9 is within [-1e18, 1e18] and result in [1,1e18].
    // n up to 1e9, so n-9 is safe. Result is n, which is in [1,1e9] safe.
    // So we just add (n - 9).
    long long add_val = n - 9;
    cout << "add " << add_val << endl;
    cin >> resp;
    if (resp == "-1") exit(0);
    // The response should be "1" because n is in range.
    // Then we answer.
    cout << "!" << endl;
    cin >> resp;
    if (resp == "-1") exit(0);
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
