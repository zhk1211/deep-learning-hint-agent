// Hint6
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--) {
        int n, q;
        cin >> n >> q;
        int m = 1 << n;
        vector<int> a(m);
        for (int i = 0; i < m; ++i) {
            cin >> a[i];
        }

        vector<int> pos(m);
        iota(pos.begin(), pos.end(), 0);
        vector<int> skill = a;
        vector<int> wins(m, 0);
        vector<int> round_eliminated(m, -1);
        vector<int> eliminated_by(m, -1);
        vector<int> stack_size(m, 1);
        vector<int> xor_prefix(m + 1, 0);

        for (int r = 0; r < n; ++r) {
            int step = 1 << r;
            vector<int> new_pos, new_skill, new_wins, new_stack_size;
            for (int i = 0; i < (int)pos.size(); i += 2) {
                int left = pos[i];
                int right = pos[i + 1];
                int winner, loser;
                if (skill[left] > skill[right] || (skill[left] == skill[right] && left < right)) {
                    winner = left;
                    loser = right;
                } else {
                    winner = right;
                    loser = left;
                }
                round_eliminated[loser] = r;
                eliminated_by[loser] = winner;
                new_pos.push_back(winner);
                new_skill.push_back(skill[winner] ^ skill[loser]);
                new_wins.push_back(wins[winner] + 1);
                new_stack_size.push_back(stack_size[winner] + stack_size[loser]);
            }
            pos = move(new_pos);
            skill = move(new_skill);
            wins = move(new_wins);
            stack_size = move(new_stack_size);
        }

        for (int i = 0; i < m; ++i) {
            xor_prefix[i + 1] = xor_prefix[i] ^ a[i];
        }

        auto get_xor = [&](int l, int r) {
            return xor_prefix[r + 1] ^ xor_prefix[l];
        };

        while (q--) {
            int idx, new_val;
            cin >> idx >> new_val;
            --idx;
            int orig_val = a[idx];
            a[idx] = new_val;
            int cur = idx;
            int cur_skill = new_val;
            int above = 0;
            bool alive = true;
            for (int r = 0; r < n && alive; ++r) {
                int step = 1 << r;
                int block = cur / (2 * step);
                int left_start = block * 2 * step;
                int right_start = left_start + step;
                int opponent_start = (cur < right_start) ? right_start : left_start;
                int opponent_end = opponent_start + step - 1;
                int opp_skill = get_xor(opponent_start, opponent_end);
                if (cur < right_start) {
                    opp_skill ^= cur_skill;
                } else {
                    opp_skill ^= cur_skill;
                }
                bool win;
                if (cur_skill > opp_skill || (cur_skill == opp_skill && cur < opponent_start)) {
                    win = true;
                } else {
                    win = false;
                }
                if (!win) {
                    alive = false;
                } else {
                    cur_skill ^= opp_skill;
                    if (cur < opponent_start) {
                        above += step;
                    }
                    cur = left_start;
                }
            }
            if (alive) {
                cout << above << '\n';
            } else {
                int elim_round = -1;
                int cur2 = idx;
                int cur_skill2 = new_val;
                for (int r = 0; r < n; ++r) {
                    int step = 1 << r;
                    int block = cur2 / (2 * step);
                    int left_start = block * 2 * step;
                    int right_start = left_start + step;
                    int opponent_start = (cur2 < right_start) ? right_start : left_start;
                    int opponent_end = opponent_start + step - 1;
                    int opp_skill = get_xor(opponent_start, opponent_end);
                    if (cur2 < right_start) {
                        opp_skill ^= cur_skill2;
                    } else {
                        opp_skill ^= cur_skill2;
                    }
                    bool win;
                    if (cur_skill2 > opp_skill || (cur_skill2 == opp_skill && cur2 < opponent_start)) {
                        win = true;
                    } else {
                        win = false;
                    }
                    if (!win) {
                        elim_round = r;
                        break;
                    } else {
                        cur_skill2 ^= opp_skill;
                        cur2 = left_start;
                    }
                }
                int ans = 0;
                int cur3 = idx;
                int cur_skill3 = new_val;
                for (int r = 0; r < elim_round; ++r) {
                    int step = 1 << r;
                    int block = cur3 / (2 * step);
                    int left_start = block * 2 * step;
                    int right_start = left_start + step;
                    int opponent_start = (cur3 < right_start) ? right_start : left_start;
                    if (cur3 < opponent_start) {
                        ans += step;
                    }
                    cur3 = left_start;
                }
                cout << ans << '\n';
            }
            a[idx] = orig_val;
        }
    }
    return 0;
}
