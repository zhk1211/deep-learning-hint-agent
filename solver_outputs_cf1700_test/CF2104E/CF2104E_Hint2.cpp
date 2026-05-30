// Hint2
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;
    string s;
    cin >> s;

    // Precompute next occurrence array: nxt[i][c] for i from 0 to n
    // nxt[i][c] = position of next occurrence of character c starting from index i (0-based)
    // If no occurrence, set to n (sentinel)
    vector<vector<int>> nxt(n + 1, vector<int>(k, n));
    for (int c = 0; c < k; ++c) {
        nxt[n][c] = n;
    }
    for (int i = n - 1; i >= 0; --i) {
        for (int c = 0; c < k; ++c) {
            nxt[i][c] = nxt[i + 1][c];
        }
        nxt[i][s[i] - 'a'] = i;
    }

    int q;
    cin >> q;
    while (q--) {
        string t;
        cin >> t;
        int m = (int)t.size();
        // Find the earliest position in s where we can match t as subsequence
        int pos = 0; // current position in s (0-based index we are at)
        bool pleasant = true;
        for (char ch : t) {
            int c = ch - 'a';
            pos = nxt[pos][c];
            if (pos == n) {
                pleasant = false;
                break;
            }
            ++pos; // move past this character for next search
        }
        if (!pleasant) {
            cout << 0 << '\n';
            continue;
        }
        // t is a subsequence of s, we need to find minimal append length
        // We need to find the longest suffix of t that can be matched starting from some position,
        // but more efficiently: we want the minimal number of characters to append so that
        // the new string is NOT a subsequence.
        // Equivalent: find the smallest L >= 0 such that for every possible continuation of length L,
        // the resulting string is not a subsequence.
        // This is equivalent to: find the maximum length of a string that can be formed by appending
        // characters to t and still be a subsequence. Then answer = that max length + 1? No.
        // Actually, we want minimal L such that NO string of length m+L is a subsequence.
        // This is the same as: find the maximum depth of a "suffix automaton" style matching.
        // We can simulate the process: start from the end of matching t, we are at some position in s.
        // We want to see how many more characters we can greedily match.
        // The answer is the number of steps we can take before we cannot match all k possibilities?
        // Let's think: we want to append characters one by one. At each step, we have a current position in s.
        // We can choose any character c. The new position becomes nxt[pos][c] + 1.
        // We want to find the maximum number of steps we can take such that for EVERY choice of characters,
        // we can still continue? No, we want to find the minimal L such that NO choice of L characters
        // yields a subsequence. That is equivalent to: the maximum L such that THERE EXISTS a choice of L characters
        // that yields a subsequence. Then answer = L + 1? Wait.
        // If t is pleasant, we want minimal appended length to make it unpleasant.
        // That means we want the smallest L >= 1 such that for EVERY string of length L appended,
        // the result is not a subsequence. But if L=1, we check if there is any character c such that
        // t+c is a subsequence. If yes, then L=1 is not enough. We need L=2, etc.
        // So answer = minimal L >= 1 such that for all c1..cL, t + c1..cL is not a subsequence.
        // This is equivalent to: the maximum L such that there EXISTS a string of length L that can be appended
        // and still be a subsequence. Then answer = L + 1? Let's test with sample.
        // Sample 1: t = "b", pleasant. Can we append 1 char and still be subsequence? 
        // "b" + "a" = "ba" is subsequence? s=abacaba. "ba" is subsequence (positions 1,2 or 3,4 etc). Yes.
        // "bb" is subsequence? "bb" - positions 1,5? Yes. "bc" is subsequence? positions 1,4? Yes.
        // So for L=1, there exists a string ("a","b","c") that is subsequence. So L=1 is not enough.
        // For L=2, can we find a string of length 2 such that "b" + two chars is subsequence?
        // "baa"? "bab"? ... We need to check if there is any. The answer is 2, meaning minimal L=2.
        // So if max L such that there exists a continuation of length L that is subsequence is L_max,
        // then answer = L_max + 1? For "b", L_max = 1? But answer is 2. So answer = L_max + 1? 1+1=2. Yes.
        // For "bcb": pleasant. Can we append 1 char and still be subsequence? 
        // "bcb" + "a" = "bcba" - is it subsequence? s=abacaba. "bcba": b(1) c(3) b(5) a(6) - yes.
        // "bcbb": b(1) c(3) b(5) b(?) - no b after 5? s[5] is b, next b? s[6] is a, s[7] is a? Wait s length 7 indices 0..6: a b a c a b a. Positions: 0:a,1:b,2:a,3:c,4:a,5:b,6:a. After index 5, next b is none. So "bcbb" not subsequence. "bcbc": b(1) c(3) b(5) c(?) - no c after 5. So only "bcba" works. So there exists a continuation of length 1 ("a") that is subsequence. So L_max >=1. Can we do length 2? "bcbaa"? After matching "bcba", we end at position after a at index 6? Actually "bcba": b(1), c(3), b(5), a(6). After index 6, next a? none. So no continuation of length 1 from there. So L_max = 1. Answer = 2? But sample output for "bcb" is 1. Wait sample output: 
        // bcb -> 1. So my reasoning is off.
        // Let's re-read: "bcb is pleasant, so at least one letter needs to be appended to the right: bcba will not work, but bcbb and bcbc are unpleasant."
        // They say: bcba will not work (meaning appending 'a' does NOT make it unpleasant? Actually "bcba will not work" means appending 'a' still leaves it pleasant? They say: "bcba will not work, but bcbb and bcbc are unpleasant." So appending 'a' results in a pleasant string, appending 'b' or 'c' results in unpleasant. So to guarantee unpleasant, you need to append at least 1 letter, but you must choose the right letter. The question: "calculate the minimum number of allowed letters you need to append to it on the right so that it stops being pleasant." That means you can choose which letters to append. You want to append some letters such that the resulting string is guaranteed to be unpleasant? Or you want the minimum number such that there EXISTS a choice of that many letters that makes it unpleasant? The phrasing: "minimum number of allowed letters you need to append to it on the right so that it stops being pleasant." This implies you are allowed to choose the appended letters. You want to find the smallest L such that you can append L letters (of your choice) to make the string unpleasant. For "bcb", you can append 1 letter: if you append 'b', you get "bcbb", which is unpleasant. So L=1 works. So answer is 1. For "b", you cannot make it unpleasant with 1 letter, because whatever you append (a,b,c) results in a pleasant string. So you need at least 2 letters. You can append "bb" to get "bbb" which is unpleasant. So answer 2.
        // So the problem: find minimal L such that there EXISTS a string of length L that when appended makes t unpleasant. Equivalently: find the minimal L such that NOT ALL strings of length L appended keep it pleasant. That is: find the minimal L such that the set of possible continuations of length L that are subsequences is not the full set of all k^L strings. Because if all k^L strings are subsequences, then no matter what you append, it remains pleasant. So you need L+1. So answer = minimal L such that the number of subsequences of length |t|+L that extend t is less than k^L? Actually, we want to find the smallest L such that there is at least one string of length L that makes it unpleasant. That is equivalent to: the maximum L such that EVERY string of length L appended keeps it pleasant. Then answer = that max L + 1. Let's check: For "b", is it true that for L=1, every string of length 1 (a,b,c) keeps it pleasant? Yes. So max L=1. Answer = 2. For "bcb", for L=1, is every string of length 1 keeping it pleasant? No, only 'a' keeps it pleasant, 'b' and 'c' make it unpleasant. So max L such that ALL strings of length L keep it pleasant is 0. Answer = 1. For "cc", it's already unpleasant, so max L such that all strings of length L keep it pleasant? L=0: the empty string appended keeps it pleasant? But t itself is unpleasant, so the condition "all strings of length 0" (only the empty string) keeps it pleasant is false. So max L = -1? Then answer = 0. So answer = (max L where all k^L continuations are subsequences) + 1, with max L starting from 0 and if t itself is not pleasant then max L = -1 -> answer 0.
        // So we need to compute for a given t, the maximum L such that from the state after matching t, for every possible sequence of L characters, we can still match them in s.
        // This is equivalent to: starting from the position pos after matching t, we consider the automaton. We want the longest L such that from pos, the automaton can accept all strings of length L. This is like a "full k-ary tree" of depth L. We can compute this by simulating: at each step, we have a current position. If from this position, for every character c, nxt[pos][c] < n (i.e., we can match c), then we can advance one step for all branches. But we need to advance simultaneously for all branches? Actually, we need to check if ALL strings of length L are subsequences. This is equivalent to: starting from pos, the "minimum" over all paths? No, we need to check if for every sequence of L choices, we can match. This is true if and only if from pos, we can match any character, and from each resulting position, we can match any character, etc. That is, the set of reachable positions after L steps must be such that from each, we can still continue? Actually, we need that for every sequence, there is a matching. This is equivalent to: the automaton is "complete" for depth L. We can compute the maximum L by finding the first step where some character cannot be matched. But careful: after matching one character, the position advances. For the "all strings" condition, we need that for every possible first character c, nxt[pos][c] < n, AND then from the new position nxt[pos][c]+1, again for every possible second character, etc. This is like a game where we need to survive L steps regardless of choices. The maximum L is the minimum over all paths? Actually, we want the maximum L such that for all sequences of length L, they are subsequences. This is equivalent to: the "worst-case" path (the one that dies earliest) must survive at least L steps. So L is limited by the first character that is missing. But after we pick a character, we move to a new position. The missing characters might be different. So we need to find the minimum over all infinite paths of the depth at which a missing character is encountered? That is exactly the shortest path to a "dead" state where some character is missing. But we are not choosing the path; we need that ALL paths survive L steps. So L is the minimum over all paths of the length until a dead state. So we need to find the minimum number of steps we can take from pos such that we are forced to hit a missing character, regardless of choices? Actually, if we want ALL paths to survive L steps, then the "adversary" can choose the path, and we need that path to survive L steps. So the maximum L is the minimum over all paths of the length until a missing character. So we need to find the shortest path from pos to a state where some character is missing. That is a shortest path problem on the automaton. The automaton has states 0..n (positions in s). From state i, for each character c, if nxt[i][c] < n, we can go to state nxt[i][c] + 1. If nxt[i][c] == n, then that character is missing, and the path ends. We want the shortest path from pos to any state that has a missing character. The length of that shortest path is the maximum L such that all paths of length L survive? Let's verify: If the shortest path to a missing character has length d, then there exists a path of length d that hits a missing character at step d. That means there is a sequence of d characters such that after d-1 steps we are at some state, and the d-th character is missing. So for L = d, the path of length d fails at step d. So not all strings of length d are subsequences. For L = d-1, all paths of length d-1 survive? Since the shortest path to a missing character is d, any path of length d-1 cannot have hit a missing character yet, so all strings of length d-1 are subsequences. So max L = d-1. Then answer = (d-1) + 1 = d. So answer is exactly the length of the shortest path from pos to a state with a missing character. Let's test: For "b" in sample 1. s=abacaba. Match "b": first 'b' at index 1. pos after matching = 2. From state 2, what are next characters? nxt[2]['a'] = 2? Actually s[2] is 'a', so nxt[2]['a']=2, next 'b' = 5, next 'c' = 3. All < n. So from state 2, we can go to state 3 (if 'a'), state 6 (if 'b'), state 4 (if 'c'). From state 3 (after 'a' at index 2): nxt[3]['a']=4, 'b'=5, 'c'=3? Wait nxt[3]['c']: s[3] is 'c', so nxt[3]['c']=3. All < n. From state 6 (after 'b' at index 5): nxt[6]['a']=6, 'b'=n, 'c'=n. So state 6 has missing 'b' and 'c'. So distance from state 2 to state 6 is 1 (via 'b'). So shortest path to a missing character is length 1? But we need to consider that from state 2, choosing 'b' leads to state 6, which has missing characters. So the path length to missing is 1? Wait: from state 2, we choose 'b', we go to state nxt[2]['b']+1 = 5+1=6. At state 6, we check if we can continue? The missing character is encountered when we try to take a step from state 6. So the path length is the number of steps taken until we cannot take the next step. If we are at state 6, and we want to take another step, we find missing. So the path that fails at step 2 has length 2? Let's define: We start at pos. We want to find the minimum number of steps we can take such that at the next step we are stuck. That is the minimum L such that there exists a sequence of L choices that leads to a state from which some character is missing. Then max L for all surviving is that minimum L - 1. So answer = minimum L. For "b": from state 2, we can choose 'b' -> state 6. From state 6, we cannot choose 'b' or 'c'. So we took 1 step and then got stuck. So L=1? But we need to append at least 2 letters to make it unpleasant. So answer should be 2. So my definition of L is off by one. Let's re-evaluate: We want minimal number of appended letters to make it unpleasant. If we append 1 letter, say 'b', we get "bb". Is "bb" unpleasant? "bb" is a subsequence? s=abacaba. "bb": positions 1 and 5 -> yes, it is pleasant. So appending 'b' does NOT make it unpleasant. Wait earlier I thought "bb" was pleasant? In sample note: "To b, at least two letters need to be appended, since ba, bb, and bc are pleasant." So "bb" is pleasant. So appending 'b' keeps it pleasant. So from state 2, appending 'b' leads to state 6, but the resulting string "bb" is still pleasant. The unpleasantness happens when we cannot match the entire string. "bb" is matched. So we need to append until the whole string cannot be matched. So we need to find the shortest string w such that t+w is not a subsequence. The length of w is the answer. So we need the minimum length of w such that from pos, w cannot be matched. That is exactly the shortest path to a "dead" state where we cannot match the next character. But careful: The matching process: we start at pos, for each character in w, we find next occurrence. If at some point next occurrence is n, then w is not a subsequence. So the length of w is the number of characters until we fail. So we want the minimum length of w that fails. That is the minimum over all w of the first index where next occurrence is n. This is exactly the shortest path from pos to a state where some character is missing, but the path length is the number of steps taken until we attempt a step that fails. If we are at state i, and we choose a character c that is missing, then w of length 1 fails immediately. So the minimum length of a failing w is the minimum over all paths from pos of the first step where a missing character is chosen. That is exactly the distance to a "missing" character, where distance is the number of steps until we are forced to pick a missing character? Actually, we can choose the missing character right away if it exists. So if from pos there is any character c with nxt[pos][c] == n, then we can choose that c and w of length 1 fails. So answer = 1. If from pos all characters exist, then any w of length 1 succeeds. We need w of length 2. We can choose first character c1, go to pos1 = nxt[pos][c1]+1. Then from pos1, if there is a missing character c2, we can choose c2 and w of length 2 fails. So the minimal length of a failing w is the minimum over all paths of the first step where a missing character is available. That is exactly the length of the shortest path in the automaton from pos to a state that has at least one missing character, plus 1? Let's define state as position. From state i, we can transition to state nxt[i][c]+1 for each c with nxt[i][c] < n. If there is a c with nxt[i][c] == n, then state i is a "winning" state for the adversary (we can fail immediately by choosing c). So the distance from pos to a winning state is the minimum number of steps to reach a state with a missing character. Then the length of w is that distance + 1? Let's test: For "b", pos=2. From state 2, all characters exist, so no missing character. We must take one step. The shortest path to a state with a missing character: from 2, go via 'b' to state 6. State 6 has missing 'b' and 'c'. So distance = 1. Then w length = distance + 1 = 2. That matches answer 2. For "bcb": match "bcb": b(1) c(3) b(5) -> pos after matching = 6. State 6 has missing 'b' and 'c'. So distance = 0 (already at a state with missing character). Then w length = 0 + 1 = 1. Answer 1. For "cc": not pleasant, so we never reach a state after matching fully. The answer is 0. So algorithm: For a query t, first check if t is a subsequence. If not, answer 0. Else, we have final position pos. Then we need to compute the shortest distance from pos to a state that has at least one missing character. The answer is that distance + 1.
        // Now how to compute this distance efficiently for many queries? The total length of t is up to 1e6, q up to 2e5. We can precompute for each state the distance to the nearest state with a missing character. Since the automaton is a DAG? Actually, transitions go from i to nxt[i][c]+1. Note that nxt[i][c] >= i, so nxt[i][c]+1 > i. So it's a DAG with edges going to strictly larger indices. We can compute dp[i] = minimum distance from state i to a state with a missing character. For state i, if there exists c such that nxt[i][c] == n, then dp[i] = 0. Else, dp[i] = 1 + min_{c} dp[nxt[i][c]+1]. We can compute this dp from n down to 0. Then for each query, after matching t, we get pos. Answer = dp[pos] + 1. But careful: if t is not pleasant, we output 0. Also, if t is pleasant, pos is the index after the last matched character. dp[pos] gives the minimum number of steps to reach a state with a missing character. Then answer = dp[pos] + 1. Let's test sample 2: n=5, k=1, s="aaaaa". nxt: for each i, nxt[i][0] = i if i<5 else 5. dp[5] = 0 (since nxt[5][0]=5==n). dp[4]: nxt[4][0]=4 < 5, so dp[4] = 1 + dp[5] = 1. dp[3] = 1 + dp[4] = 2. dp[2] = 3, dp[1] = 4, dp[0] = 5. Queries: "a": match first 'a' at 0, pos=1. dp[1]=4, answer=5. "aa": pos=2, dp[2]=3, answer=4. ... "aaaaaa": length 6, but n=5, so not pleasant -> answer 0. Matches sample.
        // So we need to compute dp array of size n+1. n up to 1e6, k up to 26. O(n*k) time and memory for nxt is 1e6*26 = 26e6 ints, about 100 MB, which might be okay but memory limit 256 MB. However, we can optimize: we don't need full nxt if we compute dp directly. We can compute dp from right to left. For each state i, we need to know for each c, the next occurrence. We can maintain an array next_pos[c] = the smallest index >= i where c occurs. As we go from n down to 0, we can update next_pos. Then dp[i] = 0 if any next_pos[c] == n (or > n-1, meaning not found). Else, dp[i] = 1 + min_{c} dp[next_pos[c] + 1]. This is O(n*k) time but O(k) memory. Perfect.
        // Let's implement:
        // Initialize next_pos array of size k with n (meaning not found).
        // dp[n] = 0 (since from n, all next_pos are n, so it's a missing state).
        // For i from n-1 down to 0:
        //   update next_pos[s[i]-'a'] = i;
        //   check if any next_pos[c] == n -> dp[i] = 0;
        //   else dp[i] = 1 + min_{c} dp[next_pos[c] + 1];
        // But careful: dp[next_pos[c] + 1] might be out of bounds if next_pos[c] == n? We already handled that case. So next_pos[c] < n.
        // This works.
        // Then for queries: match t in s using the same next_pos? But we need to match t efficiently. We can just use the same next_pos array but we need to simulate matching from left to right. Since we process queries after precomputing dp, we can't use the same next_pos because it will be at state 0. We could precompute nxt in a 2D array, but memory might be high. Alternatively, we can answer queries by simulating with the precomputed dp? No, we need to find the final position after matching t. We can do that by using a precomputed "next occurrence" array in a compact form. Since total length of all t is 1e6, we can just simulate matching using a 2D array of size (n+1) x k? That's 26e6 ints = 104 MB. Might be acceptable. But we can also use a technique: we can store for each position i and each character c the next occurrence, but we can compress it? Actually, 1e6 * 26 * 4 bytes = 104 MB. That's within 256 MB. But we also have dp array of 1e6 ints (4 MB). So total ~108 MB. It's safe. However, we can also answer queries without full nxt by using binary lifting or by processing queries offline? But simpler: just build nxt as vector<vector<int>> nxt(n+1, vector<int>(k, n)). That's 26 million ints. In C++, vector of vectors has overhead. Better to use a single flat array of size (n+1)*k, or vector<int> nxt((n+1)*k). We'll do that to save memory and time.
        // Let's use flat array: int* nxt = new int[(n+1)*k]; or vector<int> nxt((n+1)*k);
        // We'll fill it from right to left.
        // Then dp can be computed using nxt or using the next_pos method. Actually, we can compute dp using nxt after building it, or simultaneously. Since we need nxt for queries anyway, we can build nxt first, then compute dp using nxt.
        // Steps:
        // 1. Read n, k, s.
        // 2. Allocate nxt of size (n+1)*k, initialize last row to n.
        // 3. For i from n-1 down to 0: copy row i+1 to row i, then set nxt[i*k + (s[i]-'a')] = i.
        // 4. Compute dp of size n+1: dp[n] = 0. For i from n-1 down to 0: check if any nxt[i*k + c] == n, then dp[i]=0; else dp[i] = 1 + min_{c} dp[nxt[i*k + c] + 1].
        // 5. For each query: match t using nxt. Start pos=0. For each char c in t: pos = nxt[pos*k + (c-'a')]; if pos == n then break and mark not pleasant. Else pos = pos + 1. If not pleasant: output 0. Else output dp[pos] + 1.
        // This is O(n*k + total_query_length). n=1e6, k<=26 => 26e6 operations, fine. total_query_length=1e6, fine.
        // Let's implement with flat vector for speed.
    }

    // Precompute nxt
    vector<int> nxt((n + 1) * k, n);
    // last row already n
    for (int i = n - 1; i >= 0; --i) {
        // copy from i+1
        int* cur = &nxt[i * k];
        int* nxt_row = &nxt[(i + 1) * k];
        memcpy(cur, nxt_row, k * sizeof(int));
        cur[s[i] - 'a'] = i;
    }

    // Compute dp
    vector<int> dp(n + 1);
    dp[n] = 0;
    for (int i = n - 1; i >= 0; --i) {
        int* cur = &nxt[i * k];
        bool missing = false;
        int min_dp = INT_MAX;
        for (int c = 0; c < k; ++c) {
            if (cur[c] == n) {
                missing = true;
                break;
            }
            min_dp = min(min_dp, dp[cur[c] + 1]);
        }
        if (missing) {
            dp[i] = 0;
        } else {
            dp[i] = 1 + min_dp;
        }
    }

    int q;
    cin >> q;
    while (q--) {
        string t;
        cin >> t;
        int pos = 0;
        bool pleasant = true;
        for (char ch : t) {
            int c = ch - 'a';
            pos = nxt[pos * k + c];
            if (pos == n) {
                pleasant = false;
                break;
            }
            pos = pos + 1;
        }
        if (!pleasant) {
            cout << 0 << '\n';
        } else {
            cout << dp[pos] + 1 << '\n';
        }
    }

    return 0;
}
