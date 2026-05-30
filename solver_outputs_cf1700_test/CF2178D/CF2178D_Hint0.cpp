// Hint0
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--) {
        int n, m;
        cin >> n >> m;
        vector<long long> a(n);
        for (int i = 0; i < n; ++i) {
            cin >> a[i];
        }

        if (m == n) {
            cout << -1 << '\n';
            continue;
        }

        vector<int> idx(n);
        iota(idx.begin(), idx.end(), 0);
        sort(idx.begin(), idx.end(), [&](int i, int j) {
            return a[i] < a[j];
        });

        vector<int> rank(n);
        for (int i = 0; i < n; ++i) {
            rank[idx[i]] = i;
        }

        int need_to_kill = n - m;
        if (need_to_kill == 0) {
            cout << -1 << '\n';
            continue;
        }

        if (need_to_kill == 1) {
            if (n == 2) {
                cout << -1 << '\n';
                continue;
            }
            // kill the smallest
            int victim = idx[0];
            int killer = idx[1];
            cout << 1 << '\n';
            cout << killer + 1 << ' ' << victim + 1 << '\n';
            continue;
        }

        if (need_to_kill > n) {
            cout << -1 << '\n';
            continue;
        }

        // We will kill the smallest need_to_kill elves
        vector<int> victims;
        for (int i = 0; i < need_to_kill; ++i) {
            victims.push_back(idx[i]);
        }
        vector<int> survivors;
        for (int i = need_to_kill; i < n; ++i) {
            survivors.push_back(idx[i]);
        }

        // Check if survivors can kill victims without dying themselves
        // Each survivor can attack at most once.
        // We need to assign each victim to be attacked by some survivor.
        // A survivor x attacking victim y: h_y -= a_x, h_x -= a_y.
        // We need h_y <= 0 after attack, and h_x > 0 after attack.
        // Since a_x > a_y (because survivors have larger a), h_y becomes a_y - a_x < 0, so victim dies.
        // Survivor health: a_x - a_y > 0 because a_x > a_y. So survivor survives.
        // So any survivor can kill any victim in one hit and survive.
        // But we also need that after all attacks, no survivor can attack another survivor.
        // Since all survivors have attacked (if they attacked a victim), they cannot attack again.
        // So process terminates.

        // However, we must ensure that each survivor attacks at most once.
        // We have |survivors| = m, |victims| = need_to_kill.
        // If m < need_to_kill, we cannot assign each victim a distinct survivor.
        // But a survivor can only attack once. So we need m >= need_to_kill.
        if (m < need_to_kill) {
            cout << -1 << '\n';
            continue;
        }

        // If m >= need_to_kill, we can just use the first need_to_kill survivors to kill the victims.
        // The remaining survivors will not attack, but they are alive and haven't attacked.
        // However, the process requires that no pair of distinct living elves exists where one hasn't attacked.
        // If there are survivors who haven't attacked, they could attack each other.
        // So we must ensure that after our sequence, there is no valid pair.
        // If there are >=2 survivors who haven't attacked, they could attack each other.
        // So we need to make sure that at most 1 survivor remains who hasn't attacked.
        // If m > need_to_kill, we have extra survivors who didn't attack.
        // We need to make them attack something, but they can only attack living elves.
        // They could attack each other? But if they attack each other, both might die or one dies.
        // We need exactly m alive at the end.
        // Let's analyze: we have m survivors total. We use need_to_kill of them to kill victims.
        // The remaining survivors = m - need_to_kill.
        // If this number is 0, perfect.
        // If it is 1, then we have one survivor who hasn't attacked. But there is no other living elf to attack? Wait, all other survivors have attacked, so they are alive but have attacked. The rule: choose a pair of distinct living elves x and y such that x has not attacked before. If there is exactly one such x, and there is at least one other living elf y (which has attacked), then x can attack y. That would be a valid pair. So process wouldn't terminate. So we must avoid that.
        // Actually, the condition for termination: there does not exist a pair of distinct living elves x and y such that elf x has not attacked before.
        // So if there is any living elf that hasn't attacked, and there is at least one other living elf (regardless of whether that other has attacked), then the process can continue.
        // Therefore, to terminate, either all living elves have attacked, or there is exactly 1 living elf total (so no pair exists).
        // So if m > need_to_kill, we have extra survivors who haven't attacked. We must make them attack until either they die or all survivors have attacked.
        // But we need exactly m alive at the end. So we cannot let them die.
        // Thus we need all m survivors to have attacked by the end.
        // That means we need exactly m attacks (each survivor attacks once).
        // But we only have need_to_kill victims. The extra survivors need to attack something without killing themselves or the target, and the target must be alive.
        // They could attack each other? If survivor A attacks survivor B, both are alive initially. A hasn't attacked, B may or may not have attacked.
        // After attack: h_B -= a_A, h_A -= a_B.
        // Since both are survivors, their a values are among the largest. If A > B, then B dies (a_B - a_A < 0), and A survives (a_A - a_B > 0). That would reduce the number of alive.
        // If we want to keep both alive, we need a_A - a_B > 0 and a_B - a_A > 0, impossible.
        // So any attack between survivors will kill the one with smaller a.
        // Therefore, we cannot have extra survivors attack each other without reducing m.
        // So the only way to have m > need_to_kill is impossible? Let's check sample 1: n=4, m=2, a=[1,4,2,3]. need_to_kill=2. m=2, so m == need_to_kill. Output has 2 attacks.
        // Sample 4: n=3, m=1, a=[1,2,3]. need_to_kill=2. m=1 < need_to_kill -> impossible? But sample output is 2 attacks: 1 2 then 3 2? Wait sample 4: Input: 3 1 \n 1 2 3. Output: 2 \n 1 2 \n 3 2. Let's trace: initial [1,2,3]. Attack 1: 1 attacks 2 -> h: 1-2=-1, 2-1=1 -> [-1,1,3]. Elf 1 dead, elf 2 alive, elf 3 alive. Elf 1 attacked. Attack 2: 3 attacks 2 -> h: 3-2=1, 2-3=-1 -> [-1,-1,1]. Elf 2 dead, elf 3 alive. End: m=1. Here need_to_kill=2, m=1. But they used elf 1 (which is a victim? Actually sorted a: 1,2,3. Victims would be 1 and 2? But elf 1 attacked elf 2, so elf 1 was used as attacker, not victim. So my assumption that only survivors can be attackers is wrong. Victims can also attack before they die.
        // The rule: each elf can attack at most once. They can attack even if they will die in the same attack (due to recoil). They just need to be alive before the attack.
        // So we can have victims attack each other or attack survivors.
        // Let's re-evaluate.

        // We need to select a sequence of attacks such that exactly m elves remain alive, and no further attacks possible.
        // Each elf can attack at most once.
        // Health of elf i after all attacks: h_i = a_i - sum_{j: j attacked i} a_j + sum_{j: i attacked j} ??? Wait, recoil: when x attacks y, h_y -= a_x, h_x -= a_y. So h_x decreases by a_y. So h_i = a_i - sum_{j: i attacked j} a_j - sum_{j: j attacked i} a_j? Actually, if i attacks j, i loses a_j. If j attacks i, i loses a_j. So total loss = sum of a_j over all j such that either i attacked j or j attacked i. But note: i cannot attack itself. So h_i = a_i - sum_{j in N(i)} a_j, where N(i) is the set of elves that i either attacked or was attacked by. And each elf can be in N(i) at most once? Actually, if i attacks j, then j is in N(i). If j attacks i, then j is also in N(i). But can both happen? The process says: choose x that has not attacked before. So each elf attacks at most once. So for a pair (i,j), it's possible that i attacks j and later j attacks i? Yes, if j hasn't attacked before. So both directions can occur. Then N(i) would contain j twice? But the health loss is sum of a_j for each attack involving i. So if i attacks j, i loses a_j. If j attacks i, i loses a_j again. So total loss = a_j + a_j = 2a_j. So h_i = a_i - sum_{attacks involving i} a_other.
        // This is getting complicated.

        // Let's think differently. The process is equivalent to: we have a set of elves. We repeatedly pick an elf x that hasn't attacked, and another alive elf y, and they both lose the other's attack value. This is similar to pairing elves and subtracting values.
        // Notice that the total health sum decreases by a_x + a_y each attack.
        // Since all a_i distinct, we can sort them.

        // Key observation from sample 4: n=3, m=1, a=[1,2,3]. They did: 1 attacks 2 (1 dies, 2 survives with 1), then 3 attacks 2 (2 dies, 3 survives with 1). Final alive: 3. So m=1.
        // Here, the smallest elf 1 attacked the middle elf 2, dying but reducing 2's health. Then the largest elf 3 finished off 2. So the smallest was sacrificed to weaken the middle, allowing the largest to kill the middle and survive.
        // In general, we can orchestrate a chain where smaller elves attack larger elves to weaken them, eventually leaving exactly m elves.

        // Let's analyze the condition for an elf to survive: its health must remain > 0 after all attacks it's involved in.
        // Since each elf can attack at most once, the total number of attacks is at most n.
        // We can view the attacks as a directed graph where each node has out-degree at most 1 (since each elf attacks at most once). In-degree can be arbitrary.
        // The health of node i = a_i - sum_{j: i->j} a_j - sum_{j: j->i} a_j.
        // We want exactly m nodes with health > 0, and for the rest health <= 0.
        // Also, the process must be executable in some order: we need that when an elf attacks, both it and the target are alive. And an elf can only attack if it hasn't attacked before.
        // This is equivalent to finding an ordering of the edges such that when an edge (x,y) is processed, x hasn't been used as attacker before, and both x and y have current health > 0.

        // There is a known greedy strategy: sort elves by a_i. The largest elves are most likely to survive. We can try to make the smallest n-m elves die, and the largest m elves survive.
        // But sample 4: n=3, m=1, largest survives, smallest and middle die. That fits.
        // Sample 1: n=4, m=2, a=[1,4,2,3] sorted: 1,2,3,4. m=2, so largest two (3 and 4) survive? Actually survivors in sample are elves 2 and 3 with a=4 and 2? Wait original: a1=1, a2=4, a3=2, a4=3. Sorted: 1 (elf1), 2 (elf3), 3 (elf4), 4 (elf2). m=2, survivors: elf2 (a=4) and elf3 (a=2). That's the largest and the second smallest? Not exactly. But note that elf3 (a=2) survived while elf4 (a=3) died. So it's not simply the largest m.
        // Let's trace sample 1: attacks: 3->1, 2->4. Elf3 (a=2) attacks elf1 (a=1): elf3 health: 2-1=1, elf1 health: 1-2=-1. Elf1 dies, elf3 survives with 1. Elf2 (a=4) attacks elf4 (a=3): elf2 health: 4-3=1, elf4 health: 3-4=-1. Elf4 dies, elf2 survives with 1. Final alive: elf2 (a=4, health 1), elf3 (a=2, health 1). So both survivors had their health reduced to 1. They attacked smaller elves.
        // So the pattern: each survivor attacks a smaller elf, killing it, and survives with a_i - a_j > 0. So if we pair each survivor with a distinct smaller elf to kill, the survivor survives. But we need exactly m survivors. If we have m survivors, we need at least m smaller elves to be victims. So we need n >= 2m? In sample 1, n=4, m=2, n=2m. In sample 4, n=3, m=1, n=3 >= 2. In sample 6: n=4, m=1, a=[2,3,4,5] -> output -1. n=4, m=1, n >= 2m (4>=2) but output -1. So there's another condition.

        // Let's analyze sample 6: n=4, m=1, a=[2,3,4,5]. Why impossible? We want exactly 1 survivor. We need to kill 3 elves. The survivor must be the largest (5) to have a chance. If 5 attacks 4: 5 health becomes 1, 4 dies. Then we have 2 and 3 left. They can attack each other? If 3 attacks 2: 3 health 1, 2 dies. Then we have 5 (health 1) and 3 (health 1) alive. Both have attacked? 5 attacked, 3 attacked. So process terminates with 2 alive. We need 1 alive. Could we do: 2 attacks 3? 2 dies, 3 health 1. Then 4 attacks 3? 4 health 4-3=1, 3 health 1-4=-3 dies. Then 5 attacks 4? 5 health 5-4=1, 4 health 1-5=-4 dies. Final: 5 alive. Let's check order: initial [2,3,4,5]. 2 attacks 3: 2 health 2-3=-1 (dead), 3 health 3-2=1. Alive: 3,4,5. 2 has attacked. 4 attacks 3: 4 health 4-3=1, 3 health 1-4=-3 (dead). Alive: 4,5. 4 has attacked. 5 attacks 4: 5 health 5-4=1, 4 health 1-5=-4 (dead). Alive: 5. All attackers: 2,4,5. 3 never attacked. Is this valid? Check rules: before 2 attacks 3, both alive, 2 hasn't attacked. OK. Before 4 attacks 3, both alive (3 is alive with health 1), 4 hasn't attacked. OK. Before 5 attacks 4, both alive (4 health 1), 5 hasn't attacked. OK. After, only 5 alive. No pair exists. So this seems valid! But sample output says -1. Why is this invalid? Let's re-read the problem statement carefully.

        // "When Franklin casts Mass Hysteria, the following process is repeated:
        // - Choose a pair of distinct living elves x and y (h_x,h_y>0) such that elf x has not attacked before. If no such pair exists, terminate the process.
        // - Then, elf x attacks elf y, decreasing h_y by a_x. Additionally, due to recoil, h_x is decreased by a_y. Note that a_x and a_y remain unchanged."

        // In my sequence: 2 attacks 3. h_2 = 2-3 = -1 (dead), h_3 = 3-2 = 1 (alive). OK.
        // 4 attacks 3. h_4 = 4-3 = 1, h_3 = 1-4 = -3 (dead). OK.
        // 5 attacks 4. h_5 = 5-4 = 1, h_4 = 1-5 = -4 (dead). OK.
        // Final: only 5 alive. m=1. So why does sample say -1?
        // Maybe I missed that all a_i are distinct. They are.
        // Maybe the process must terminate when no valid pair exists, but we are constructing a sequence that the process *will* follow. The process is deterministic? No, we choose the pair. The problem says: "construct a valid sequence of attacks such that exactly m elves are alive when the process ends". It doesn't say the process is forced to follow our sequence; we are just providing one possible sequence that the process could take. So my sequence should be valid.
        // Let's check sample 6 again: Input: 4 1 \n 2 3 4 5. Output: -1. So my sequence must be invalid for some reason.
        // Let's re-read the condition: "Immediately before the i-th iteration, both elves x_i and y_i are alive and elf x_i has not attacked in any previous iteration."
        // In my sequence: iteration 1: x=2, y=3. Both alive, 2 hasn't attacked. OK.
        // Iteration 2: x=4, y=3. Before this, alive: 3 (health 1), 4 (health 4), 5 (health 5). 4 hasn't attacked. OK.
        // Iteration 3: x=5, y=4. Before this, alive: 4 (health 1), 5 (health 5). 5 hasn't attacked. OK.
        // After iteration 3, alive: 5 (health 1). No pair exists. So it terminates. Exactly 1 alive. So why -1?
        // Wait, the problem says: "It can be shown that Mass Hysteria terminates after at most n iterations." My sequence has 3 iterations, n=4, so <=4. OK.
        // Maybe I misread the input: sample 6 is "4 1" and "2 3 4 5". Let's look at the sample input given in the problem:

        // 7
        // 4 2
        // 1 4 2 3
        // 2 2
        // 6 7
        // 3 0
        // 1 2 3
        // 3 1
        // 1 2 3
        // 3 2
        // 1 2 3
        // 4 1
        // 2 3 4 5
        // 6 0
        // 998244353 1000000000 314159265 676767677 999999999 987654321

        // Sample outputs:
        // 2
        // 3 1
        // 2 4
        // -1
        // 2
        // 3 2
        // 1 3
        // 2
        // 1 2
        // 3 2
        // -1
        // 2
        // 1 4
        // 4 2
        // 4
        // 3 1
        // 2 5
        // 6 1
        // 4 2

        // So the 6th test case is "4 1" with "2 3 4 5", output is -1. The 5th test case is "3 2" with "1 2 3", output is 2 lines: 1 2 and 3 2. The 4th is "3 1" with "1 2 3", output is 2 lines: 1 2 and 3 2? Wait, sample output for 4th test case: after -1 for 2nd, then 2 \n 3 2 \n 1 3? Let's parse the sample output carefully:

        // Output:
        // 2
        // 3 1
        // 2 4
        // -1
        // 2
        // 3 2
        // 1 3
        // 2
        // 1 2
        // 3 2
        // -1
        // 2
        // 1 4
        // 4 2
        // 4
        // 3 1
        // 2 5
        // 6 1
        // 4 2

        // There are 7 test cases. Outputs:
        // 1st: 2 attacks: (3,1), (2,4)
        // 2nd: -1
        // 3rd: 2 attacks: (3,2), (1,3)  --> test case 3 is "3 0" with "1 2 3". Output has 2 attacks.
        // 4th: 2 attacks: (1,2), (3,2)  --> test case 4 is "3 1" with "1 2 3". Output has 2 attacks.
        // 5th: -1  --> test case 5 is "3 2" with "1 2 3". Output -1.
        // 6th: 2 attacks: (1,4), (4,2) --> test case 6 is "4 1" with "2 3 4 5". Wait, sample output for 6th is 2 attacks? But the sample output block shows:
        // -1
        // 2
        // 1 4
        // 4 2
        // So the 5th test case output is -1, and 6th test case output is 2 attacks. Let's recount:
        // Test cases:
        // 1: 4 2 -> output 2...
        // 2: 2 2 -> -1
        // 3: 3 0 -> 2...
        // 4: 3 1 -> 2...
        // 5: 3 2 -> -1
        // 6: 4 1 -> 2... (1 4, 4 2)
        // 7: 6 0 -> 4...
        // So my earlier assignment was off. The 6th test case is "4 1" with "2 3 4 5" and it DOES have a solution! The sample output for it is:
        // 2
        // 1 4
        // 4 2
        // Let's check that: n=4, m=1, a=[2,3,4,5]. 1-indexed: elf1=2, elf2=3, elf3=4, elf4=5.
        // Attacks: 1 attacks 4: elf1 (2) attacks elf4 (5). h1 = 2-5 = -3 (dead), h4 = 5-2 = 3. Alive: 2,3,4? Wait elf2=3, elf3=4, elf4=3. So alive: elf2 (3), elf3 (4), elf4 (3). Elf1 dead.
        // Then 4 attacks 2: elf4 (health 3) attacks elf2 (health 3). h4 = 3-3 = 0 (dead? health must be >0 to be alive, so 0 is dead), h2 = 3-3 = 0 (dead). So both die. Final alive: elf3 (4). So m=1. And elf3 never attacked. But process terminates because no pair exists? Living elves: only elf3. So no pair. Valid! So my previous sequence was different but also seemed valid. Why did I think sample said -1? Because I misread the sample output order. The 5th test case (3 2) is -1, not the 6th. So my sequence for 4 1 might be valid, but the sample uses a different one. So my reasoning that m=1, n=4 is possible is correct.

        // Now, what about the 5th test case: 3 2 with 1 2 3. Output -1. Why? n=3, m=2. We want 2 alive. Sorted: 1,2,3. Can we have 2 alive? Let's try: 1 attacks 2: 1 dies, 2 health 1. Alive: 2,3. 3 attacks 2: 3 health 3-2=1, 2 health 1-3=-2 dies. Final: 3 alive. m=1. If we do 2 attacks 1: 2 health 2-1=1, 1 dies. Alive: 2,3. Then 3 attacks 2: same, 2 dies. If 3 attacks 1: 3 health 2, 1 dies. Alive: 2,3. Then 2 attacks 3: 2 health 2-3=-1 dies, 3 health 2-2=0 dies. Final: 0 alive. If 3 attacks 2: 3 health 1, 2 health 3-3=0 dies. Alive: 1,3. Then 1 attacks 3: 1 health 1-3=-2 dies, 3 health 1-1=0 dies. Final: 0. So it seems impossible to end with 2 alive. Why? Because any attack between two elves will kill at least one of them (since a_i distinct, the smaller one dies). To have 2 alive at the end, both must survive all attacks. But if they attack each other, one dies. If they don't attack each other, they must attack the third elf. But the third elf can only be attacked once? Actually, multiple elves can attack the same target. But if both attack the third elf, the third elf dies, and the two attackers survive? Let's check: 1 and 2 both attack 3. First, 1 attacks 3: 1 health 1-3=-2 dies, 3 health 3-1=2. Then 2 attacks 3: 2 health 2-3=-1 dies, 3 health 2-2=0 dies. So both attackers die because they are smaller than 3. If 2 and 3 attack 1: 2 attacks 1: 2 health 1, 1 dies. 3 attacks 1: but 1 is already dead, can't. So 3 must attack 2: 3 health 1, 2 health 1-3=-2 dies. Final: 3 alive. If 1 and 3 attack 2: 1 attacks 2: 1 dies, 2 health 1. 3 attacks 2: 3 health 1, 2 dies. Final: 3 alive. So indeed, with n=3, you can only end with 0 or 1 alive. m=2 is impossible. So the condition is not simply n >= 2m.

        // Let's find the general condition.
        // Each attack (x,y) results in:
        // h_x' = h_x - a_y
        // h_y' = h_y - a_x
        // If we consider the sorted order, an elf can only survive if it never loses more health than its a_i. Since it loses a_j when interacting with j, it can survive an interaction with a smaller elf (a_j < a_i) because a_i - a_j > 0. It cannot survive an interaction with a larger elf (a_j > a_i) because a_i - a_j <= 0.
        // Therefore, an elf can only survive if it only interacts with elves smaller than itself. Moreover, if it interacts with multiple smaller elves, its health decreases by the sum of their a_j. It can survive as long as sum < a_i.
        // Also, an elf can be attacked by multiple larger elves, but then it will die because each larger elf's attack reduces its health by a_large > a_small, so it dies immediately.
        // So for an elf to survive, it must never be attacked by a larger elf, and it must never attack a larger elf (since that would make it lose a_large > a_i). So a survivor can only attack elves smaller than itself.
        // Therefore, the set of survivors must be a prefix of the sorted order? No, if a survivor only interacts with smaller elves, then all elves larger than it must not interact with it. But larger elves could interact among themselves or with smaller ones. However, if a larger elf attacks a smaller elf, the smaller elf dies. So if we want some smaller elves to survive, they must not be attacked by larger elves.
        // This suggests that the survivors must be the largest m elves. Because if a smaller elf survives, all larger elves must not attack it. But larger elves need to attack something (if they haven't attacked) or be attacked. If a larger elf hasn't attacked and is alive, it could attack the smaller survivor, which would kill it. To prevent that, the larger elf must either be dead or have already attacked. But if it has already attacked, it must have attacked someone. If it attacked a smaller elf, that smaller elf would die. So the smaller elf cannot survive if there is a larger elf that attacks it. The only way a smaller elf survives is if all larger elves are dead or never attack it. But if larger elves are dead, they must have been killed by even larger elves or by each other. Ultimately, the largest elf has no one larger to kill it, so it can only die if it attacks a larger elf (impossible) or is attacked by a smaller elf (which would kill the smaller elf, not the largest). So the largest elf always survives unless it attacks a larger elf (impossible) or is attacked by multiple smaller elves such that sum of their a's > a_max? But a smaller elf attacking the largest elf: the smaller elf dies (since a_small - a_large < 0), and the largest elf loses a_small, so its health decreases but remains >0. So the largest elf can survive multiple attacks from smaller elves as long as sum of their a's < a_max. But eventually, if all smaller elves attack the largest, it might die if sum > a_max. But then those smaller elves would all die in the process. So the largest elf could die if enough small elves sacrifice themselves. But then who survives? The ones that didn't attack? They would be even smaller. So it's possible that the largest elf dies and some smaller elves survive? Let's test: n=3, a=[1,2,10]. Can we have m=1 with survivor 2? 1 and 2 attack 10: 1 attacks 10: 1 dies, 10 health 9. 2 attacks 10: 2 health 2-10=-8 dies, 10 health 9-2=7. Final: 10 alive. If 10 attacks 2: 10 health 8, 2 dies. Then 1 attacks 10: 1 dies, 10 health 7. Final: 10 alive. So 10 always survives. What if a=[1,8,10]? 8 and 1 attack 10: 1 attacks 10: 1 dies, 10 health 9. 8 attacks 10: 8 health 8-10=-2 dies, 10 health 9-8=1. 10 survives. If 10 attacks 8: 10 health 2, 8 dies. Then 1 attacks 10: 1 dies, 10 health 1. 10 survives. So largest always survives? What if a=[6,7,10]? 6 and 7 attack 10: 6 attacks 10: 6 dies, 10 health 4. 7 attacks 10: 7 health 7-10=-3 dies, 10 health 4-7=-3 dies. So 10 dies! Then who is alive? No one, m=0. So largest can die if sum of smaller attackers > a_max. But then all smaller attackers also die. So m=0 is possible. Can we have m=1 with survivor 7? Let's try: we want 7 to survive, 6 and 10 dead. 10 must die. How can 10 die? It must be attacked by elves whose sum of a > 10. The only elves are 6 and 7. If both attack 10, 10 dies, but 7 also dies because it attacks 10 (7-10=-3). So 7 dies. If 6 attacks 10, 6 dies, 10 health 4. Then 7 attacks 10? 7 dies, 10 health -3 dies. Both dead. If 10 attacks 7: 10 health 3, 7 dies. Then 6 attacks 10: 6 dies, 10 health -3 dies. All dead. If 10 attacks 6: 10 health 4, 6 dies. Then 7 attacks 10: 7 dies, 10 health -3 dies. All dead. So 7 cannot survive if 10 dies. So the only way to have m=1 is if the largest survives, or if the largest dies and some smaller survives? But smaller surviving requires it not to attack the largest (or it would die), and the largest must die without the smaller attacking it. But the largest can only die if it is attacked by a set of elves with sum > a_max. If the smaller survivor is part of that set, it dies. If it's not part of that set, then the set must be other elves. But if there are other elves, they would be even smaller? Actually, if we have a=[2,3,4,5], can we have m=1 with survivor 4? Sample 6 output had survivor 4 (elf3). Let's check: a=[2,3,4,5]. Sample output: 1->4 (2 attacks 5), 4->2 (5 attacks 3). Final: elf3 (4) alive. So survivor is 4, not the largest 5. Here, 5 died because it attacked 3? Wait: 5 attacked 3: 5 health 5-3=2, 3 health 3-5=-2 dies. Then later? Actually sequence: 1 attacks 4: elf1 (2) attacks elf4 (5). h1=-3 dead, h4=3. Then 4 attacks 2: elf4 (5, health 3) attacks elf2 (3, health 3). h4=3-3=0 dead, h2=3-3=0 dead. Final: elf3 (4) untouched, health 4. So survivor is 4. Largest 5 died because it attacked 3 and both died. So the largest can die if it attacks a smaller elf and the recoil kills it? But recoil is
