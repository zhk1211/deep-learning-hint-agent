#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
    }

    // dp[i][state] = min coins to paint first i elements (0-indexed up to i-1)
    // state 0: no pending red element to the right that can help leftwards
    // state 1: there is a red element with value >0 that can still be used to paint leftwards
    // We process left to right, but the operations can propagate in both directions.
    // Actually we can think of it as: we need to cover all zeros by either paying a coin
    // or using a nearby positive number. The positive numbers can be used to paint adjacent
    // elements, and each use reduces the number by 1. A 2 can be used twice, a 1 once.
    // This is equivalent to: we can place "sources" (coins) on some elements, and then
    // positive numbers can "spread" their value to adjacent zeros.
    // We can model it as: we need to assign each zero to be covered either by a coin on itself
    // or by a positive number that reaches it. Positive numbers can cover zeros to their left
    // and right, but each coverage consumes 1 from the number.
    // This is a classic greedy/DP problem: we can process left to right, maintaining how many
    // "unused" positive values we have that can cover zeros to the left.
    // Actually, we can think of it as: we want to minimize coins. Coins are spent on some elements.
    // If we spend a coin on a positive element, we can use its value to cover adjacent zeros.
    // If we don't spend a coin on a positive element, we cannot use its value.
    // But wait: the operation says: choose a red element (which is not 0) and a blue adjacent,
    // decrease the red by 1, paint blue red. So the red element must have been painted red first.
    // Painting it red can be done by coin or by being painted from another red.
    // So a positive element can only be used if it becomes red. It can become red by coin or by
    // being painted from an adjacent red. So it's a propagation.
    // This is equivalent to: we can start chains from coins. A coin on an element makes it red.
    // Then its value (if >0) can be used to paint adjacent elements, and those can propagate further.
    // So we need to cover all elements with such chains, minimizing coins.
    // This is similar to: we have an array, we can place "activations" (coins) on some elements.
    // An activated element with value v can activate up to v adjacent elements (each activation
    // reduces v by 1). The activation can spread left/right.
    // We want to activate all elements with minimum coins.
    // This is a known problem: we can use DP with states representing how many "pending" activations
    // we have from the right that can be used to cover zeros to the left.
    // Let's define dp[i][j] = min coins for prefix up to i, with j pending "right-to-left" activations
    // that can be used to cover zeros to the left. But j can be up to 2? Actually, a 2 can give two
    // activations, but they can be used in any direction. However, if we process left to right,
    // we can keep track of how many "unused" activations we have that are directed leftwards.
    // Wait, the activations are not directed; a red element can paint any adjacent blue.
    // But if we process left to right, we can decide for each element whether it is covered by
    // a coin, or by a previous element's activation (from left), or by a future element's activation
    // (from right). The future element's activation would be "pending" and we need to account for it.
    // This is similar to a DP with state = number of pending activations from the right that we
    // have already committed to use for covering zeros to the left. Since a_i <= 2, the maximum
    // pending activations we might need to carry is small.
    // Let's think differently: we can model it as a flow or matching? But n=2e5, so DP is needed.
    // Another perspective: each coin spent on an element gives that element "red" status and its value.
    // The total "red value" we can generate is sum of a_i over elements we coin, plus we can chain.
    // Actually, if we coin a 2, we get 2 "spreads". If we coin a 1, we get 1 spread.
    // If we don't coin a positive element, we can still use its value if it gets painted red from
    // an adjacent spread. So the value is not lost if we don't coin it; it can be activated by a neighbor.
    // So the problem reduces to: we need to select a set of elements to coin such that every element
    // is reachable from some coined element via a path where each step consumes 1 from the source's value.
    // This is like we have "energy" that can flow. Each coined element provides a_i energy.
    // Energy can be transferred to adjacent elements, but each transfer costs 1 energy.
    // We need to cover all elements with energy, and we want to minimize the number of coined elements.
    // This is exactly the problem of covering an array with intervals of lengths determined by values,
    // but intervals can overlap and energy can be shared? Actually, if we coin a 2, it can cover itself
    // and up to 2 adjacent elements. But if it covers an adjacent 1, that 1 becomes red and can then
    // cover its own adjacent elements. So it's like a chain reaction.
    // This is equivalent to: we can place "sources" (coins) on some elements. Each source provides
    // a_i "units" of coverage. Coverage can extend left and right, but each step consumes 1 unit.
    // When coverage reaches a positive element, that element adds its own value to the pool of units
    // available for further coverage. So it's like we have a resource that accumulates.
    // We want to minimize the number of sources.
    // This is a classic problem: we can use DP with state representing the "excess" units we have
    // when moving left to right. But since units can be used in both directions, it's tricky.
    // However, note that we can always assume we only use units to cover zeros, because covering a
    // positive element with a unit is beneficial (it adds its value). So we should always try to
    // cover positive elements without spending a coin if possible.
    // We can think of it as: we process left to right, and we maintain how many "unused" units we
    // have that are available to cover elements to the right. But we also might need to cover
    // elements to the left from a future source. That suggests a DP with state = number of
    // "pending" units that we will receive from the right to cover the current element.
    // Since a_i <= 2, the number of pending units we might need is at most 2? Actually, if we have
    // a zero, we might need 1 unit from the right. If we have two zeros in a row, we might need 2 units.
    // But we can also get units from the left. So we can define DP[i][j] = min coins for prefix up to i,
    // where j is the number of units we have "borrowed" from the right (i.e., we have a deficit of j
    // that must be supplied by elements to the right of i). Alternatively, j could be the number of
    // units we have available to cover elements to the right.
    // Let's try to define DP[i][k] = min coins for first i elements, with k "extra" units available
    // to cover elements to the right. k can be negative? If negative, it means we need |k| units from
    // the right to cover the current prefix.
    // But the maximum absolute value of k is small because a_i <= 2 and we only need to cover zeros.
    // Actually, we can have at most 2 extra units because a 2 gives 2 units, and we might not use them
    // immediately. We can also have deficit: if we have a zero and no units, we might decide to pay a
    // coin (which gives a_i units if a_i>0, but for zero it gives 0) or we might decide to borrow from
    // the right. Borrowing means we assume a future element will cover this zero. That future element
    // will spend 1 unit to cover this zero. So we can have a deficit of up to maybe 2? But if we have
    // many zeros in a row, we might need to borrow many. However, we can always pay a coin to cover a
    // zero, which costs 1 coin and gives 0 units. So we would only borrow if we expect a future positive
    // element to cover it, which saves a coin. But if we borrow too many, we might not have enough
    // future units. So the DP state needs to track the deficit.
    // But n=2e5, so state must be O(1). Is the deficit bounded? Suppose we have a long run of zeros.
    // We can either coin some of them, or rely on a distant 2. A 2 can cover at most 2 zeros, but if
    // it covers a 1, that 1 can cover more. So the chain can be long. So deficit could be large.
    // However, we can think greedily: we should always use available units to cover zeros, and if we
    // run out, we might need to coin a zero or wait for a future positive. But waiting means we have
    // a deficit that must be filled. The deficit can be arbitrarily large if we have many zeros and
    // a large positive later. But we can also coin a zero to reduce deficit. So the DP state might
    // need to track deficit, which could be up to n. That's too large.
    // We need a different approach.

    // Let's reconsider: The operation "decrease red element by 1 and paint adjacent blue red" can be
    // seen as moving a "token" from a red element to an adjacent blue element. The token is consumed
    // when painting a zero? No, the red element's value decreases. So a red element with value v can
    // send out v tokens. Each token paints one adjacent element red. If that adjacent element has
    // value >0, it becomes red and can send out its own tokens. So it's like we have a graph where
    // each node has a certain number of tokens. We can place a coin on a node to activate it (make it
    // red and give its tokens). Then tokens can spread. We want to activate all nodes with minimum coins.
    // This is exactly the problem of "minimum number of sources to activate all nodes in a line with
    // given initial tokens". This can be solved by a greedy algorithm: we process from left to right,
    // maintaining the number of tokens we have available. When we encounter a zero, if we have tokens,
    // we use one; otherwise, we must place a coin. But wait, if we place a coin on a zero, we get 0 tokens,
    // so it doesn't help future. If we place a coin on a positive, we get its tokens. But we might want
    // to place a coin on a positive even if we have tokens, to get more tokens for future zeros.
    // However, we can also get tokens from a positive without coining it, if we reach it via tokens from
    // the left. So the decision is: when we have a positive element, should we coin it (spending 1 coin,
    // gaining its value in tokens) or should we try to reach it via tokens from the left (spending 1 token
    // to activate it, then gaining its value in tokens)? The latter is always better if we have a token,
    // because it saves a coin and the net token change is: spend 1 token, gain a_i tokens, net + (a_i - 1).
    // If a_i = 1, net 0; if a_i = 2, net +1. So it's beneficial or neutral. If we don't have a token,
    // we must coin it (or coin something else to get a token to reach it). But coining it directly gives
    // a_i tokens without needing a token to reach it. So it seems we should always try to use tokens to
    // activate positives, and only coin when we have no tokens and need to activate something.
    // But what about zeros? A zero gives no tokens. If we have a token, we can use it to activate the zero
    // (costs 1 token, gain 0). If we don't have a token, we must coin the zero (costs 1 coin, gain 0).
    // So zeros are just token sinks.
    // This suggests a greedy strategy: scan left to right, maintain a pool of available tokens.
    // Initially tokens = 0, coins = 0.
    // For each element x:
    //   If x > 0:
    //     If tokens > 0:
    //       // We can activate this element using 1 token, then add its tokens to the pool.
    //       tokens = tokens - 1 + x
    //     Else:
    //       // We must coin this element.
    //       coins += 1
    //       tokens += x
    //   Else (x == 0):
    //     If tokens > 0:
    //       tokens -= 1
    //     Else:
    //       coins += 1
    //       // tokens remain 0
    // But wait, is it always optimal to use a token to activate a positive if we have one?
    // Consider: we have tokens=1, and we see a 2. Using the token gives tokens = 1 - 1 + 2 = 2.
    // If we instead coined the 2, we would spend 1 coin, and tokens would become 1 + 2 = 3.
    // So coining gives more tokens but costs a coin. Which is better? It depends on future zeros.
    // If we have many zeros ahead, having more tokens might save multiple coins. So maybe we should
    // sometimes coin a positive even if we have tokens, to increase our token pool.
    // For example: array [0, 2, 0, 0]. 
    // Greedy: start tokens=0, coins=0.
    // i=0: 0, tokens=0 -> coin it, coins=1, tokens=0.
    // i=1: 2, tokens=0 -> coin it, coins=2, tokens=2.
    // i=2: 0, tokens=2 -> use token, tokens=1.
    // i=3: 0, tokens=1 -> use token, tokens=0.
    // Total coins=2. But can we do better? 
    // Alternative: coin the 2 first? But we must paint all. If we coin the 2 (index 1), we get 2 tokens.
    // Then we can paint index 0 (cost 1 token), index 2 (cost 1 token). Then index 3 is left. We have 0 tokens left. We must coin index 3. Total coins=2. Same.
    // What if we coin the 2 and also coin something else? No.
    // Another example: [0, 1, 0]. Greedy: coin 0 (coins=1), then 1: tokens=0 -> coin 1 (coins=2, tokens=1), then 0: use token (tokens=0). Total 2. But optimal is 2? Sample 2: [0,0,1,1] output 2. Our greedy on [0,1,0] gives 2. Is it possible to do 1? If we coin the 1, we get 1 token. We can paint left 0 (cost 1 token, tokens=0). Then right 0 is left, no tokens, must coin. So 2. So greedy seems okay.
    // But consider: [2, 0, 0]. Greedy: i=0: 2, tokens=0 -> coin, coins=1, tokens=2. i=1: 0, use token, tokens=1. i=2: 0, use token, tokens=0. Total 1. Optimal 1.
    // What about [0, 2, 0, 0, 0]? Greedy: coin 0 (1), coin 2 (2, tokens=2), use token on 0 (tokens=1), use on 0 (0), coin last 0 (3). Total 3. Can we do 2? Coin the 2 (1 coin, tokens=2). Use token on left 0 (tokens=1). Now we have tokens=1, and we have two zeros on the right. We can only cover one. So we must coin one zero. Total 2. But wait, we didn't coin the first zero. We used a token from the 2 to paint it. So we spent 1 coin on the 2, and we need to cover the two right zeros. We have 1 token left, so we can cover one zero, and must coin the other. Total coins = 1 (for 2) + 1 (for zero) = 2. But our greedy coined the first zero because it had no tokens at that point. But we could have coined the 2 first? The order of operations doesn't matter; we can choose which elements to coin. The greedy processes left to right, so when it sees the first zero, it doesn't know about the 2 ahead. So it coins the zero. But we could have coined the 2 instead and used its token to cover the first zero. So the left-to-right greedy fails because it makes decisions too early.
    // So we need to look ahead.

    // This is a known problem: "Minimum coins to paint array" or similar. It can be solved with DP.
    // Let's define DP[i] = minimum coins to paint prefix up to i, assuming we have no pending tokens
    // from the right. But we might have tokens that we can carry over. Actually, we can think of it as:
    // we can place coins on some elements. Each coined element with value v provides v "coverage points".
    // Coverage points can be used to cover adjacent elements. When a coverage point is used on an element
    // with value v', that element also provides v' coverage points. So it's like we have a flow.
    // This is equivalent to: we want to select a set of elements to coin such that every element is
    // reachable from a coined element via a path where the sum of values along the path (excluding the
    // coined element?) Actually, if we coin an element, we get its value. Then we can move to adjacent
    // elements, each step costs 1. When we step onto a positive element, we gain its value. So the net
    // cost to reach an element is the distance minus the sum of values of positive elements on the path
    // (excluding the start?). More precisely, if we start with a coined element with value v, we can
    // reach any element within distance d if the sum of values of all positive elements encountered
    // (including the start?) is at least d. But the start's value is already v. So we need v + sum of
    // values of intermediate positives >= d.
    // This is like we have "energy" that can be replenished by positives.
    // We can think of it as: we need to cover the array with intervals of "influence" from coined elements.
    // But intervals can merge.

    // Another angle: This is similar to the problem of "minimum number of taps to water a garden" but
    // with variable ranges that extend based on other taps. Not exactly.

    // Let's search for a DP solution. Since n=2e5, O(n) or O(n * small constant) is needed.
    // We can define dp[i][state] where state represents the "balance" of tokens we have after processing
    // up to i. But as we saw, balance can be negative (deficit) if we plan to cover some elements from
    // the right. However, we can bound the deficit because we would never let deficit exceed 2 without
    // placing a coin? Actually, if we have a deficit, it means we are expecting future positives to cover
    // past zeros. But a future positive can cover at most its value in zeros to the left, plus it can
    // chain through other positives. But if we have a long run of zeros, we might need to cover many
    // zeros from a single future positive. That would require a large deficit. But we can always place
    // a coin on a zero to reduce deficit. So the optimal strategy will not have an arbitrarily large
    // deficit because placing a coin on a zero costs 1 and reduces deficit by 1. If the deficit is large,
    // it might be cheaper to place a coin earlier. So there is a trade-off. But the maximum deficit we
    // need to consider might be small? Let's think: Suppose we have a 2 far to the right. It can cover
    // at most 2 zeros directly. If there are more zeros, it must chain through a 1 or another 2. But if
    // there are no positives in between, the 2 can only cover 2 zeros. So the maximum number of zeros
    // a single positive can cover to its left is its value, unless there are other positives that can
    // be activated and extend the range. But those positives would be activated by the same token flow,
    // so they effectively increase the total tokens. So a single coined 2 can cover an arbitrary number
    // of zeros if there are enough 1s or 2s interspersed? Actually, if we have a 2 and then a 1, the 2
    // can activate the 1 (cost 1 token), and the 1 gives 1 token, so net tokens remain the same. So
    // the 2 can cover an infinite number of zeros if there is a 1 every other zero? No, each zero costs
    // 1 token. If we have a 2, we have 2 tokens. We can cover two zeros. If we encounter a 1, we can
    // spend 1 token to activate it, get 1 token back, net 0. So the 1 doesn't increase tokens; it just
    // allows us to "pass through" without losing tokens. A 2 gives +1 net token when activated (spend 1,
    // get 2). So a 2 can increase the token pool. So a single coined 2 can cover many zeros if there are
    // other 2s that can be activated to replenish tokens. But those 2s themselves could be coined instead.
    // So the deficit is not inherently bounded by a constant. However, we can use a greedy approach that
    // looks at the array and decides optimally.

    // Let's think about the problem as: we want to minimize coins. Each coin gives us a "source" of tokens.
    // We can also get tokens by activating positives without coins, but that costs 1 token.
    // So the net token gain from activating a positive without a coin is (a_i - 1). For a 1, net 0; for a 2, net +1.
    // So 1s are neutral, 2s are +1, 0s are -1.
    // If we have a sequence of elements, the total token change from activating a segment without any coins
    // is sum (a_i - 1) over the segment. We start with 0 tokens at the left boundary (if we don't coin the
    // leftmost). To activate the segment without coins, we need to have a token to start the chain. That
    // token must come from a coined element to the left, or we coin the leftmost element of the segment.
    // Actually, if we coin an element, we get its full a_i tokens without spending a token to activate it.
    // So coining an element gives a net of a_i tokens (since we don't spend a token to activate it).
    // If we don't coin it, we must spend a token to activate it, then we get a_i tokens, net a_i - 1.
    // So the difference between coining and not coining an element is: coining costs 1 coin, gives a_i tokens;
    // not coining costs 0 coins, gives a_i - 1 tokens (but requires a token to be available).
    // So we can think of it as: we have a token pool. We process elements. For each element, we can either:
    // - Coin it: coins++, tokens += a_i.
    // - Not coin it: if tokens > 0, tokens = tokens - 1 + a_i; else, we cannot (we must coin it or have deficit).
    // But we can also have a deficit: we can choose to not coin an element even if tokens == 0, by "borrowing"
    // a token from the future. This means we go into negative tokens. Later, when we have positive tokens,
    // we pay back the deficit. So tokens can go negative. The goal is to end with tokens >= 0 (no deficit).
    // And we want to minimize coins.
    // This is exactly the problem: we have an array a_i. We start with tokens = 0. For each i from 1 to n:
    // we can either pay 1 coin to add a_i to tokens, or if tokens > 0, we can subtract 1 and add a_i.
    // But we can also allow tokens to go negative, representing borrowing. At the end, tokens must be >= 0.
    // Wait, if we allow tokens to go negative, then the operation "if tokens > 0" becomes "we can always
    // choose the non-coin option, but it costs 1 token (which can make tokens negative)". So we can always
    // choose to not coin, and tokens becomes tokens - 1 + a_i. If we coin, tokens becomes tokens + a_i,
    // and coins++.
    // So we have two choices at each step:
    // 1. Coin: (coins+1, tokens + a_i)
    // 2. No coin: (coins, tokens - 1 + a_i)
    // We want to reach the end with some coins and tokens >= 0, minimizing coins.
    // This is a classic DP! But tokens can become large negative or positive. However, note that a_i <= 2.
    // The maximum tokens we might ever need is bounded? Actually, if we have many 2s, tokens can grow.
    // But we can also spend tokens on zeros. The optimal strategy will not accumulate arbitrarily many tokens
    // because tokens beyond what is needed to cover future zeros are useless. The maximum useful tokens at
    // any point is at most the number of remaining zeros plus maybe some for positives? But we can always
    // convert tokens into covering zeros without coining. So we don't need to keep more tokens than the
    // number of zeros ahead. But that could be O(n). So DP state with tokens as dimension is O(n^2) which
    // is too slow.
    // However, we can observe that the DP is convex or something? Or we can use a greedy with priority queue?
    // This is similar to the problem of "buying items with discounts" or "stock trading" where you can
    // decide to pay now or later. Actually, it's exactly the problem: we have a sequence of values a_i.
    // We want to choose a set of indices to coin. For the others, we must have enough tokens to cover them.
    // Tokens are generated by coined elements and by positive elements that are not coined (but they consume
    // a token to be activated). So the total tokens generated by a set of coined elements S is:
    // sum_{i in S} a_i + sum_{i not in S} (a_i - 1) * [i is activated without coin].
    // But every element must be activated. If we activate all elements, then for each i not in S, we must
    // have spent a token to activate it. That token must come from somewhere. So the total tokens available
    // must be at least the number of elements not in S. The tokens available are: sum_{i in S} a_i (from
    // coined elements) + sum_{i not in S} (a_i) (from non-coined elements after activation) minus the tokens
    // spent to activate non-coined elements? Wait, let's derive carefully.

    // Let S be the set of elements we coin. For each element i in S, we get a_i tokens, and we don't spend
    // a token to activate it. For each element i not in S, we must spend 1 token to activate it, and then
    // we get a_i tokens. So net token change for i not in S is a_i - 1.
    // The total tokens we have after processing all elements is: sum_{i in S} a_i + sum_{i not in S} (a_i - 1).
    // This must be >= 0 because we can't have deficit at the end? Actually, we can have leftover tokens,
    // but we cannot have negative. Also, during the process, the token count must never go negative if we
    // process in some order? But the order is fixed: we must activate elements in some sequence? The problem
    // allows us to choose the order of operations. We can paint any blue element adjacent to a red element.
    // So we can choose the order of activation. We can always activate elements in an order that respects
    // the token constraints? The constraint is that at any point, the number of tokens we have spent cannot
    // exceed the number we have generated. If we process left to right, we might need to have tokens available
    // when we want to activate a non-coined element. But we can choose to activate elements in a different
    // order. For example, we can activate a 2 on the right first (by coining it), then use its tokens to
    // activate elements to its left. So the order of activation is flexible. The only constraint is that
    // we can only activate an element if it is adjacent to an already red element. So the set of red elements
    // grows from the coined elements. This is equivalent to: we can choose any spanning tree of the line
    // graph rooted at the coined elements, and the tokens flow along edges. The total tokens generated must
    // be at least the number of edges in the tree (since each edge corresponds to spending 1 token to paint
    // the child). The number of edges is exactly the number of non-coined elements, because the tree has
    // |S| roots and total n nodes, so n - |S| edges. So the condition is: total tokens generated >= n - |S|.
    // Total tokens generated = sum_{i in S} a_i + sum_{i not in S} a_i = sum_{i=1}^n a_i. Wait, that's just
    // the sum of all a_i! Because every element eventually contributes its a_i tokens (after being activated).
    // But we also spend tokens to activate non-coined elements. So net tokens = sum a_i - (n - |S|).
    // This net must be >= 0? Actually, we don't need net tokens at the end; we just need to be able to
    // perform the activations. The total tokens we have at any point is the sum of a_i of red elements
    // minus the number of activations we have done so far. At the end, all elements are red, so total tokens
    // generated is sum a_i, and total activations done is n - |S|. So the final token count is sum a_i - (n - |S|).
    // This can be negative? If it's negative, it means we spent more tokens than we had, which is impossible.
    // So we must have sum a_i - (n - |S|) >= 0, i.e., |S| >= n - sum a_i.
    // But wait, is that the only condition? Let's test with examples.
    // Example 1: [0,2,0]. sum a_i = 2. n=3. Condition: |S| >= 3 - 2 = 1. So minimum coins = 1. Correct.
    // Example 2: [0,0,1,1]. sum=2, n=4. |S| >= 4-2=2. Minimum 2. Correct.
    // Example 3: [0,1,0,0,1,0,2]. sum=4, n=7. |S| >= 7-4=3. But output is 4. So condition is not sufficient!
    // Why? Because the tokens must be available at the right time and place. You can't just use a token from
    // a distant 2 to activate a zero if there are gaps that require tokens to cross. The tree must be connected.
    // So the simple sum condition is necessary but not sufficient. The actual constraint is that we can
    // route the tokens through the line. This is equivalent to: we can partition the array into segments
    // each rooted at a coined element, such that within each segment, the sum of a_i (excluding the root?)
    // is at least the number of non-root elements? Actually, if we have a coined element as root, it can
    // spread to adjacent elements. The tokens flow along edges. For a connected component rooted at a coined
    // element, the total tokens generated in that component is sum of a_i over the component. The number of
    // edges is (size of component - 1). So we need sum a_i >= size - 1. That is exactly the condition for
    // that component to be valid. So the problem reduces to: partition the array into contiguous components,
    // each component having at least one coined element (which can be any element in the component), such
    // that for each component, sum a_i >= size - 1. And we want to minimize the total number of coined elements.
    // But wait, a component can have multiple coined elements? If we have multiple coined elements in the same
    // component, they can share the load. But we can always consider them as separate components that are
    // adjacent and merge? Actually, if two coined elements are adjacent, they can be in the same component,
    // and the condition is sum a_i >= size - 1. Having more coined elements makes the condition easier to
    // satisfy. But we want to minimize coined elements, so we want to make components as large as possible
    // without violating the condition. However, the condition sum a_i >= size - 1 is equivalent to
    // sum (a_i - 1) >= -1. For a component, let deficit = sum (1 - a_i). Then we need deficit <= 1?
    // Actually, sum a_i >= size - 1 => sum a_i - size >= -1 => sum (a_i - 1) >= -1.
    // So a component is valid if the sum of (a_i - 1) over the component is >= -1.
    // And we need to cover the whole array with such components, and we place at least one coin in each component.
    // But wait, if a component has sum (a_i - 1) >= -1, does it mean we can activate the whole component with
    // exactly one coin? Not necessarily. The coin must be placed on some element. The token flow must be feasible.
    // For example, consider a component [0, 0, 2]. sum a_i = 2, size=3, sum a_i >= 2? 2 >= 2, so condition holds.
    // Can we do it with 1 coin? If we coin the 2, we get 2 tokens. We can paint the left adjacent 0 (cost 1),
    // then the next 0 (cost 1). Total cost 2, we have 2 tokens. Works. What if we coin a 0? We get 0 tokens,
    // can't paint anything. So the coin must be placed on an element that can reach the others. But if the
    // component satisfies sum (a_i - 1) >= -1, is there always a placement of one coin that works?
    // Consider [0,
