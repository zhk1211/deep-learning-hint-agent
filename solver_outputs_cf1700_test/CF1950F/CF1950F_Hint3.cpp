// Hint3
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int t;
    cin >> t;
    while (t--) {
        int a, b, c;
        cin >> a >> b >> c;
        
        // Check if the tree is possible
        // In a rooted tree, number of leaves = c
        // For a tree with given a, b, c, the total number of vertices = a + b + c
        // The number of children = 2*a + 1*b + 0*c = 2a + b
        // In any rooted tree, number of edges = total vertices - 1
        // Also number of edges = sum of children (since each edge connects a parent to a child)
        // So we must have: 2a + b = a + b + c - 1 => a = c - 1
        // Wait, that's not always true. Let's re-derive.
        // Every vertex except root has exactly one parent. So sum of children = total vertices - 1.
        // So 2a + b = (a + b + c) - 1 => 2a + b = a + b + c - 1 => a = c - 1.
        // So necessary condition: a = c - 1.
        // But wait, what about b? b can be anything? Let's check sample: a=2,b=1,c=3 => a=2, c-1=2, ok.
        // a=0,b=0,c=1 => a=0, c-1=0, ok.
        // a=0,b=1,c=1 => a=0, c-1=0, ok.
        // a=1,b=0,c=2 => a=1, c-1=1, ok.
        // a=1,b=1,c=3 => a=1, c-1=2, not equal, output -1. Correct.
        // a=3,b=1,c=4 => a=3, c-1=3, ok.
        // a=8,b=17,c=9 => a=8, c-1=8, ok.
        // a=24,b=36,c=48 => a=24, c-1=47, not equal, output -1. Correct.
        // a=1,b=0,c=0 => a=1, c-1=-1, not equal, output -1. Correct.
        // a=0,b=3,c=1 => a=0, c-1=0, ok.
        // So condition a = c - 1 is necessary.
        
        if (a != c - 1) {
            cout << "-1\n";
            continue;
        }
        
        // Now we need to find minimum height.
        // We have a vertices with 2 children, b vertices with 1 child, c leaves.
        // We want to minimize height. The tree is built level by level.
        // The root is at level 0.
        // We can think of building the tree by placing vertices in levels.
        // At each level, we have some number of "slots" for children.
        // Initially, we have 1 slot at level 0 (the root).
        // We want to use up all a and b vertices, and end up with exactly c leaves.
        // Since a = c - 1, the total number of vertices with children is a + b.
        // The number of leaves is c = a + 1.
        // We want to minimize height, so we should try to fill levels as much as possible.
        // The height is the maximum level of any vertex.
        // We can simulate the process: start with level 0 having 1 node (the root).
        // The root can be either type a or type b or leaf? But we have a and b given.
        // Actually, we need to assign types to all non-leaf vertices.
        // We have a vertices of type 2, b vertices of type 1.
        // We want to place them in the tree to minimize height.
        // The leaves are the vertices with 0 children.
        // We can think of building the tree top-down: at each level, we have some number of "available parent slots" that need to be filled with either a type-2 or type-1 vertex.
        // But we also need to end up with exactly c leaves.
        // Since a = c - 1, the total number of non-leaf vertices is a + b = c - 1 + b.
        // The total vertices = a + b + c = 2c - 1 + b.
        // The number of edges = total vertices - 1 = 2c - 2 + b.
        // Also, sum of children = 2a + b = 2(c-1) + b = 2c - 2 + b. Consistent.
        
        // To minimize height, we want to use the type-2 vertices as early as possible to create more slots, so we can place the remaining vertices in fewer levels.
        // However, we have a fixed number of type-2 vertices (a). We cannot create more.
        // The process: start with 1 node at level 0. This node must be either type 2 or type 1 or leaf? But we have a and b non-leaf vertices. The root is one of them if a+b>0.
        // Actually, the root cannot be a leaf if there are other vertices. So root is either type 2 or type 1.
        // We want to minimize height, so we should use type-2 vertices at the root and as high as possible to maximize the number of nodes at lower levels.
        // This is similar to building a tree where each type-2 node adds 2 children, type-1 adds 1 child.
        // We want to place all a+b non-leaf vertices and then the c leaves will be the children of the last level's parents.
        // The height is the number of edges from root to the deepest leaf.
        // We can simulate level by level: maintain the number of "slots" (potential parents) at the current level.
        // Initially, slots = 1 (the root). We need to assign a type to the root: it will consume one slot and produce either 2 or 1 new slots for the next level.
        // But we have a limited number of type-2 and type-1 vertices.
        // To minimize height, we should use as many type-2 as possible at each level, because they produce more children, thus reducing the number of levels needed to place all vertices.
        // However, we must ensure we don't run out of type-2 vertices too early? Actually, using type-2 early is always better for minimizing height.
        // So the greedy strategy: at each level, we have some number of slots (parents to be placed). We will fill these slots with as many type-2 vertices as possible, then type-1 if needed, and the rest become leaves? But we have exactly c leaves, and they must be placed at the end.
        // Actually, the leaves are the vertices with no children. They will be the children of the last level's parents. But we can also have leaves at higher levels if we don't fill all slots with non-leaf vertices? No, because every slot must be filled with a vertex. If we don't put a non-leaf vertex, we must put a leaf. But we have exactly c leaves. So we need to distribute the leaves among the slots that are not used by non-leaf vertices.
        // The process: we have a total of a+b non-leaf vertices. They will occupy some of the slots across levels. The remaining slots will be filled by leaves. The total number of slots created is exactly the total number of children, which is 2a+b. The total number of vertices is a+b+c. The number of slots at level 0 is 1 (the root). The root is a non-leaf vertex (if a+b>0). So we start with slots = 1, and we need to place a+b non-leaf vertices. Each non-leaf vertex placed consumes one slot and creates either 2 or 1 new slots for the next level.
        // We want to minimize the maximum level reached. This is equivalent to minimizing the number of levels needed to place all non-leaf vertices and then the leaves will be at the next level? Actually, leaves are placed at the level after their parent. So if the last non-leaf vertex is at level h, then its children (leaves) are at level h+1. So height = h+1 if there are leaves. If there are no non-leaf vertices (a=b=0, c=1), height=0.
        // So we need to find the minimum h such that we can place all a+b non-leaf vertices in levels 0..h, with the constraint that at each level we can use at most the available slots, and we can choose how many type-2 and type-1 to use, but total type-2 used = a, type-1 used = b.
        // This is like: we have a certain number of "processing units" (slots) at each level. We want to process all a+b items, where type-2 items produce 2 new slots, type-1 produce 1 new slot. We want to minimize the number of levels.
        // Since type-2 produces more slots, we should use them as early as possible to increase the number of slots for later levels, allowing us to place more items per level and thus reduce the number of levels.
        // So the greedy algorithm: simulate levels. At level 0, slots = 1. We will use as many type-2 as possible from the remaining a, but we cannot use more than slots. So we use x = min(slots, a). Then we have slots - x remaining slots at this level, which we can fill with type-1: y = min(slots - x, b). Then the remaining slots (if any) will be filled with leaves? But we don't need to place leaves yet; they will be placed at the end. Actually, if we have slots left after using all a and b, then those slots must be filled with leaves. But we have exactly c leaves. The total slots created over all levels will be exactly the number of children. The number of leaves is exactly the number of slots that are not used by non-leaf vertices. So if at some level we have leftover slots and we still have a or b left, we must use them. If we run out of a and b, the remaining slots at this level and all future levels will be leaves. But we need exactly c leaves. Since a = c - 1, the total slots created = 2a+b = 2(c-1)+b. The total non-leaf vertices = a+b = c-1+b. So total leaves = total slots - total non-leaf vertices? Wait, total slots created = sum of children = 2a+b. Total vertices = a+b+c. The number of leaves is c. The number of non-leaf vertices is a+b. The root is a non-leaf vertex (if a+b>0). The root does not have a parent, so it is not a child. The total children = total vertices - 1 (since root has no parent). So 2a+b = (a+b+c) - 1 => a = c - 1. So the condition ensures consistency.
        // In the simulation, we start with slots = 1 (the root). We will place non-leaf vertices level by level. At each level, we have some number of slots. We fill them with as many type-2 as possible, then type-1. The number of slots used by non-leaf vertices at this level is the number of non-leaf vertices we place. The remaining slots at this level will be filled by leaves? But leaves don't create new slots. So those leaves are just placed, and they don't contribute to the next level's slots. The next level's slots come only from the children of the non-leaf vertices placed at this level.
        // So the simulation: 
        // slots = 1 (the root, level 0)
        // height = 0
        // while a > 0 or b > 0:
        //   if slots == 0: we cannot place any more non-leaf vertices, but we still have some left -> impossible? But we already checked a = c-1, so it should be possible if we use slots correctly.
        //   Actually, we need to place all a and b. The total slots created will eventually be enough to place all leaves. But we must ensure we don't run out of slots before placing all non-leaf vertices.
        //   At current level, we can place up to 'slots' non-leaf vertices.
        //   We want to use type-2 first: use2 = min(slots, a)
        //   a -= use2
        //   slots -= use2
        //   use1 = min(slots, b)
        //   b -= use1
        //   slots -= use1
        //   The remaining 'slots' at this level will be leaves. They don't affect next level.
        //   The next level's slots = use2 * 2 + use1 * 1.
        //   height++ (we move to next level)
        // After placing all non-leaf vertices, we have some slots left at the current level? Actually, after the loop, a=0 and b=0. The current 'slots' value is the number of slots at the last level that were not filled with non-leaf vertices. These slots must be filled with leaves. But we also have leaves from the next level? Wait, after placing the last non-leaf vertices, they create children slots for the next level. Those children slots will all be leaves because we have no more non-leaf vertices. So the total leaves will be: the leftover slots at the level where we placed the last non-leaf vertices, plus all the slots of the next level (which are all leaves). But we need exactly c leaves. The condition a = c-1 ensures that the total leaves will be exactly c if we follow this process? Let's verify.
        // Total leaves = (slots left at the level where last non-leaf is placed) + (slots created by those non-leaf vertices). But the slots created by the last non-leaf vertices are exactly the next level's slots. So total leaves = (slots left at last non-leaf level) + (next level slots). But next level slots = use2*2 + use1*1 at that level. And slots left at that level = original slots at that level - use2 - use1. So total leaves = original slots at that level - use2 - use1 + use2*2 + use1 = original slots at that level + use2. But original slots at that level is the number of slots available at that level before placing. That number is exactly the number of children created by the previous level's non-leaf vertices. This seems complicated.
        // However, we know that if a = c-1, the total number of leaves will automatically be c if we just place all non-leaf vertices in some valid way? Actually, any tree with a vertices of degree 2 (children count), b of degree 1, and c leaves must satisfy a = c-1. And if a = c-1, does there always exist a tree for any b? Let's check: a=0, b=3, c=1. a=0, c-1=0, ok. Can we have a tree with 0 vertices of 2 children, 3 vertices of 1 child, and 1 leaf? Total vertices = 4. The tree would be a path of 4 vertices: root (1 child) -> internal (1 child) -> internal (1 child) -> leaf. That works. Height = 3. Sample output for 0 3 1 is 3. So yes.
        // So the condition a = c-1 is both necessary and sufficient? Let's test a=0, b=0, c=1: works. a=0, b=1, c=1: works (root with 1 child leaf). a=1, b=0, c=2: works (root with 2 children leaves). a=1, b=1, c=2? a=1, c-1=1, ok. Total vertices=4. Can we have a tree with one 2-child, one 1-child, two leaves? Root (2 children): one child is leaf, other child is 1-child vertex, which has one leaf child. That works. Height? Root at 0, 2-child at 0? Actually root is the 2-child. Its children: leaf at level 1, and 1-child at level 1. The 1-child has leaf at level 2. Height = 2. Let's see if we can do height 1? Height 1 means all leaves at level 1. Then root must have all children as leaves. Root would have a+b children? But root can have at most 2 children (if it's type 2) or 1 child (type 1). If root is type 2, it has 2 children, both leaves -> then a=1, b=0, c=2. Not our case. If root is type 1, it has 1 child leaf -> a=0, b=1, c=1. So height 1 not possible. So minimum height is 2. Our simulation should give 2.
        // So the greedy simulation of using as many type-2 as possible at each level should yield the minimum height. Because using type-2 earlier increases the number of slots faster, allowing us to place more vertices in the next levels, thus reducing the total number of levels needed to place all a+b non-leaf vertices.
        // But we must be careful: we might have a situation where we have many type-2 vertices, and using them all at once might create more slots than needed, but that's fine because we can just leave the extra slots as leaves. The height is determined by the level where we place the last non-leaf vertex. Using type-2 earlier can only reduce or keep the same the number of levels needed to place all non-leaf vertices.
        // So the algorithm:
        // if a != c - 1: output -1
        // else:
        //   if a == 0 and b == 0: output 0 (only one vertex, c=1)
        //   else:
        //     height = 0
        //     slots = 1
        //     while a > 0 or b > 0:
        //         if slots == 0: // should not happen if a = c-1, but just in case
        //             break
        //         use2 = min(slots, a)
        //         a -= use2
        //         slots -= use2
        //         use1 = min(slots, b)
        //         b -= use1
        //         slots -= use1
        //         // remaining slots at this level become leaves (we don't track them)
        //         // next level slots = children of placed vertices
        //         slots = use2 * 2 + use1 * 1
        //         height++
        //     // after loop, a=0, b=0. The current 'slots' are the slots at the next level, which will all be leaves.
        //     // The height we have is the level of the last placed non-leaf vertices.
        //     // The leaves at the next level will be at height+1.
        //     // But if there are no leaves at the next level? Actually, if slots == 0 after placing last non-leaf vertices, then there are no leaves at the next level. But we must have c leaves. c = a_initial + 1. Since a_initial > 0, c >= 2. So there must be leaves. The leaves are either at the same level as the last non-leaf vertices (if we had leftover slots at that level) or at the next level. The maximum distance from root to a leaf is the height of the tree.
        //     // If we placed the last non-leaf vertices at level 'height-1'? Let's trace:
        //     // Start: height=0, slots=1 (root level). We place root at level 0. After placing, we compute next slots and increment height to 1. So after the loop, 'height' is the level of the next level's slots. The last non-leaf vertices were placed at level height-1. Their children (leaves) will be at level height. So the tree height is 'height' if there are any leaves at that level. But what if we had leftover slots at the level where we placed the last non-leaf vertices? Those leftover slots become leaves at that same level. That level is height-1. So the maximum leaf level could be height-1 or height. Actually, if we have leftover slots at the last non-leaf level, those leaves are at level height-1. The children of the last non-leaf vertices are at level height. So the maximum is height. So the tree height is exactly the final 'height' value after the loop? Let's test with examples.
        // Example 1: a=2, b=1, c=3.
        // Initial: a=2,b=1, slots=1, height=0.
        // Loop:
        //   use2 = min(1,2)=1, a=1, slots=0.
        //   use1 = min(0,1)=0, b=1, slots=0.
        //   next slots = 1*2 + 0 = 2.
        //   height=1.
        //   a=1>0, continue.
        //   use2 = min(2,1)=1, a=0, slots=1.
        //   use1 = min(1,1)=1, b=0, slots=0.
        //   next slots = 1*2 + 1*1 = 3.
        //   height=2.
        //   a=0,b=0, exit.
        // Final height=2. Output 2. Correct.
        // Example 2: 0 0 1 -> a=0,b=0,c=1. a==c-1 (0==0). a=0,b=0 -> output 0. Correct.
        // Example 3: 0 1 1 -> a=0,b=1,c=1. a=0, c-1=0. 
        //   slots=1, height=0.
        //   use2=0, a=0, slots=1.
        //   use1=min(1,1)=1, b=0, slots=0.
        //   next slots = 0*2 + 1*1 = 1.
        //   height=1.
        //   a=0,b=0 exit.
        // Final height=1. Output 1. Correct.
        // Example 4: 1 0 2 -> a=1,b=0,c=2. a=1, c-1=1.
        //   slots=1, height=0.
        //   use2=min(1,1)=1, a=0, slots=0.
        //   use1=0, b=0, slots=0.
        //   next slots = 1*2 = 2.
        //   height=1.
        //   exit.
        // Final height=1. Output 1. Correct.
        // Example 5: 1 1 3 -> a=1, c-1=2, not equal -> -1. Correct.
        // Example 6: 3 1 4 -> a=3,b=1,c=4. a=3, c-1=3.
        //   slots=1, height=0.
        //   use2=min(1,3)=1, a=2, slots=0.
        //   use1=0, b=1, slots=0.
        //   next slots = 2.
        //   height=1.
        //   use2=min(2,2)=2, a=0, slots=0.
        //   use1=min(0,1)=0, b=1, slots=0.
        //   next slots = 2*2 = 4.
        //   height=2.
        //   a=0, b=1>0.
        //   use2=0, a=0, slots=4.
        //   use1=min(4,1)=1, b=0, slots=3.
        //   next slots = 0*2 + 1*1 = 1.
        //   height=3.
        //   exit.
        // Final height=3. Output 3. Correct.
        // Example 7: 8 17 9 -> a=8, c-1=8.
        //   Let's simulate quickly:
        //   level0: slots=1, use2=1, a=7, use1=0, b=17, next=2, height=1
        //   level1: slots=2, use2=2, a=5, use1=0, b=17, next=4, height=2
        //   level2: slots=4, use2=4, a=1, use1=0, b=17, next=8, height=3
        //   level3: slots=8, use2=1, a=0, use1=7, b=10, next=1*2+7=9, height=4
        //   level4: slots=9, use2=0, use1=9, b=1, next=9, height=5
        //   level5: slots=9, use2=0, use1=1, b=0, next=1, height=6
        //   exit. height=6. Output 6. Correct.
        // Example 8: 24 36 48 -> a=24, c-1=47, not equal -> -1. Correct.
        // Example 9: 1 0 0 -> a=1, c-1=-1 -> -1. Correct.
        // Example 10: 0 3 1 -> a=0, c-1=0.
        //   slots=1, height=0.
        //   use2=0, use1=min(1,3)=1, b=2, slots=0, next=1, height=1.
        //   level1: slots=1, use2=0, use1=1, b=1, next=1, height=2.
        //   level2: slots=1, use2=0, use1=1, b=0, next=1, height=3.
        //   exit. height=3. Output 3. Correct.
        // So the algorithm works.
        
        // Edge case: a=0, b=0 already handled.
        // What if a=0, b>0, c=1? Then a = c-1 holds. The tree is a path. The height should be b (since b vertices with 1 child, plus the leaf, total b+1 vertices, height = b). Our simulation: slots=1, height=0. Each level we place one type-1, b decreases, next slots=1, height increments. After b levels, height=b. Correct.
        // What if a>0, b=0? Then c = a+1. The tree is a full binary tree? Not exactly full, but all non-leaf vertices have 2 children. The height is the height of a tree with a internal nodes each having 2 children. The number of leaves is a+1. The minimum height of such a tree is ceil(log2(a+1))? Actually, if we always use type-2, we are building a tree where each internal node has 2 children. To minimize height, we want to make it as balanced as possible. Our greedy algorithm uses as many type-2 as possible at each level, which essentially builds a complete binary tree? Let's test: a=3, b=0, c=4. 
        // level0: slots=1, use2=1, a=2, next=2, height=1
        // level1: slots=2, use2=2, a=0, next=4, height=2
        // exit. height=2. Is that minimum? A tree with 3 internal nodes and 4 leaves: root has 2 children, each child has 2 children (leaves). Height=2. Could we do height=1? Height 1 would mean root has 4 children, but root can have at most 2. So height=2 is minimum. Correct.
        // Another test: a=7, b=0, c=8.
        // level0: slots=1, use2=1, a=6, next=2, h=1
        // level1: slots=2, use2=2, a=4, next=4, h=2
        // level2: slots=4, use2=4, a=0, next=8, h=3
        // height=3. log2(8)=3. Correct.
        // So it works.
        
        // Now, is there any case where the greedy choice of using type-2 first might not be optimal? Suppose we have a=2, b=2, c=3. a=2, c-1=2, ok.
        // Greedy:
        // level0: slots=1, use2=1, a=1, use1=0, b=2, next=2, h=1
        // level1: slots=2, use2=1, a=0, use1=1, b=1, next=1*2+1=3, h=2
        // level2: slots=3, use2=0, use1=1, b=0, next=1, h=3
        // height=3.
        // Can we do better? Let's try to construct a tree of height 2. Total vertices = 2+2+3=7. Height 2 means max distance 2. Root at level 0. Level 1 can have at most 2 children if root is type-2, or 1 if type-1. To minimize height, we want root to be type-2. So root has 2 children. We have a=2, so one type-2 used at root, one left. b=2. Leaves=3. At level 1, we have 2 slots. We need to place the remaining a=1 and b=2, total 3 non-leaf vertices, but we only have 2 slots at level 1. So we cannot place all non-leaf vertices by level 1. Thus height must be at least 3. So greedy gives 3, which is optimal.
        // Another test: a=3, b=1, c=4. Greedy gave height=3 earlier. Can we do height=2? Total vertices=8. Height 2: root (level 0), level 1, level 2. Root must be type-2 to have 2 children at level 1. At level 1, we have 2 slots. We need to place remaining a=2, b=1 (total 3) non-leaf vertices. Only 2 slots, so impossible. Height 3 is minimum.
        // What if we have many b and few a? Greedy still works because using type-2 early creates more slots, allowing us to place the many type-1 vertices in fewer levels. If we delay type-2, we would have fewer slots and need more levels to place the type-1 vertices. So greedy is optimal.
        
        // Thus the algorithm is correct.
        
        // Implementation details:
        // We need to handle large values: a,b,c up to 1e5, sum over test cases up to 3e5. The while loop runs at most O(a+b) per test case? Actually, the number of levels is at most O(log(a) + b)? In worst case, if a=0, b=1e5, the loop runs b times, which is 1e5 per test case. Sum of a+b+c over test cases <= 3e5, so total iterations across all test cases is at most sum of (a+b) <= 3e5. So it's fine.
        // But we can also compute height directly without simulation? Simulation is simple and fast enough.
        
        // Let's code it.
        
        if (a == 0 && b == 0) {
            cout << "0\n";
            continue;
        }
        
        int height = 0;
        int slots = 1;
        while (a > 0 || b > 0) {
            int use2 = min(slots, a);
            a -= use2;
            slots -= use2;
            int use1 = min(slots, b);
            b -= use1;
            slots -= use1;
            // remaining slots become leaves (we don't need to track them)
            slots = use2 * 2 + use1;
            height++;
        }
        // The tree height is the level of the leaves. The last non-leaf vertices were at level height-1.
        // Their children are at level height. Also, any leftover slots at level height-1 became leaves at level height-1.
        // So the maximum leaf level is height (since there is at least one leaf at level height if slots > 0 after loop? Actually, after loop, slots is the number of children of the last placed non-leaf vertices. Those children are leaves at level height. If slots == 0, that means the last non-leaf vertices had no children? But type-2 has 2 children, type-1 has 1. So if we placed any non-leaf vertex, slots > 0. The only way slots == 0 is if we placed no non-leaf vertices at the last level? But we only exit when a=0 and b=0. The last iteration placed some non-leaf vertices, so use2+use1 > 0, so slots = use2*2 + use1 > 0. So after loop, slots > 0. Thus there are leaves at level 'height'. So the tree height is exactly 'height'.
        // But wait: what if we had leftover slots at the level where we placed the last non-leaf vertices? Those leaves are at level height-1. The children are at level height. So max is height. So height is correct.
        // However, consider the case where we place the very last non-leaf vertex and it uses up all slots at that level, and it has children. Then the leaves are only at the next level. Height is still the next level. So height is correct.
        // But what if we place the last non-leaf vertex and there are leftover slots at that level? Those leftover slots become leaves at that same level. The children of the placed vertices become leaves at the next level. So the maximum is still the next level. So height is correct.
        // So output height.
        cout << height << "\n";
    }
    return 0;
}
