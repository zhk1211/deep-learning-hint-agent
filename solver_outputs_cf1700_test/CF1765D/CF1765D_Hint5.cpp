// Hint5
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    long long m;
    cin >> n >> m;
    vector<long long> a(n);
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
    }

    sort(a.begin(), a.end());

    auto can = [&](int k) -> bool {
        // k is the number of good adjacent pairs we want
        // We need to select n-k elements to be "bad" separators?
        // Actually, if we have k good pairs, we need to arrange the sequence
        // such that there are k pairs of adjacent elements with sum <= m.
        // The maximum possible k is n-1 (all adjacent pairs good).
        // We can think of it as: we want to form a sequence where we have
        // k good pairs. The number of bad pairs will be (n-1) - k.
        // A known greedy: to maximize good pairs, we can pair smallest with largest?
        // Let's test if we can achieve at least k good pairs.
        // We can try to form a sequence by taking the smallest elements and pairing them
        // with the largest elements such that their sum <= m.
        // If we can form at least k such pairs, then we can achieve k good pairs.
        // Actually, we need to check if we can arrange the array so that there are
        // at least k adjacent pairs with sum <= m.
        // Equivalent to: can we select k disjoint pairs (adjacent in the final sequence)
        // such that each pair sum <= m? But they must be adjacent in the final sequence.
        // A known result: the maximum number of good adjacent pairs is achieved by
        // sorting and then pairing a[0] with a[n-1], a[1] with a[n-2], etc.
        // But we need to form a single sequence. The maximum number of good pairs
        // is the maximum number of pairs we can form such that sum <= m,
        // but they must be adjacent in the final order. We can always arrange
        // the sequence to have these pairs adjacent. For example, if we have pairs
        // (x1,y1), (x2,y2), ... with xi <= yi and xi+yi <= m, we can order them as
        // x1, y1, x2, y2, ... and then the remaining elements can be placed at the ends
        // or between, but they might break some pairs. Actually, we want to maximize
        // the number of adjacent pairs with sum <= m. The maximum possible is
        // the maximum number of disjoint pairs we can form with sum <= m,
        // because we can always arrange the sequence to have these pairs adjacent
        // and the other elements placed such that they don't create additional good pairs
        // (or they might, but we only care about achieving at least k).
        // So we can binary search on k, and check if we can form k disjoint pairs
        // with sum <= m. To maximize disjoint pairs, we can use two pointers:
        // sort a, then try to pair smallest with largest possible.
        int pairs = 0;
        int l = 0, r = n - 1;
        // We need to select k pairs. We can greedily pair a[l] with a[r] if a[l]+a[r]<=m.
        // But we must ensure we don't reuse elements. Since we want exactly k pairs,
        // we can try to form as many as possible and see if >= k.
        // However, we need to be careful: we might need to leave some elements unpaired.
        // The greedy of pairing smallest with largest works for maximizing number of pairs.
        while (l < r) {
            if (a[l] + a[r] <= m) {
                ++pairs;
                ++l;
                --r;
            } else {
                --r;
            }
        }
        return pairs >= k;
    };

    // Binary search for maximum k (number of good pairs)
    int low = 0, high = n - 1, best = 0;
    while (low <= high) {
        int mid = (low + high) / 2;
        if (can(mid)) {
            best = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    // Total time = sum of all a_i (download times) + n (watching times) - best
    // Explanation: We have n videos. Each requires a_i download time and 1 watch time.
    // Total download time = sum a_i. Total watch time = n.
    // However, we can overlap download of next video with watching of current video
    // if the next video can start downloading immediately after the current finishes downloading.
    // The condition for overlapping is that the sum of sizes of the current video and the next video <= m,
    // because while watching the current video (1 minute), we can download the next video only if
    // there is enough space. Actually, the download of the next video can start as soon as the current
    // video finishes downloading, but we need space for both? Wait, the problem says:
    // "Once Monocarp starts the download of a video of size s, the s megabytes are immediately reserved."
    // "When Monocarp finishes watching a video, he can delete the video, instantly freeing the space."
    // So, if we have two videos A and B, and we download A, then watch A. While watching A, we can download B
    // only if there is enough free space for B. At the start of watching A, A is still on disk.
    // So we need a_A + a_B <= m to download B while watching A.
    // If a_A + a_B > m, we cannot start downloading B until A is deleted, which happens after watching A.
    // So we lose 1 minute of overlap. In general, each good adjacent pair (sum <= m) saves 1 minute
    // because we can start downloading the next video immediately after the current finishes downloading,
    // and the download of the next video will finish during the watching of the current video?
    // Let's analyze: Timeline:
    // Download A: takes a_A minutes. Then watch A: takes 1 minute. During this 1 minute, we can download B
    // if a_B <= m - a_A. If so, B's download will take a_B minutes, but we only have 1 minute of watching A.
    // If a_B > 1, we cannot finish downloading B during the watch of A. But we can continue downloading B
    // after A is deleted? Actually, after watching A, we delete A, so space becomes m. But we already started
    // downloading B, so B's download continues. The overlap is only that we started B's download 1 minute earlier
    // than if we waited until A was deleted. So we save 1 minute per good pair.
    // If a_A + a_B <= m, we can start B's download immediately after A's download finishes, and we save 1 minute
    // because we don't have to wait for A to be watched and deleted before starting B's download.
    // So total time = sum a_i + n - (number of good adjacent pairs).
    // This matches the sample: n=5, m=6, a=[1,2,3,4,5], sum=15, n=5, total=20. Best k?
    // We can pair (1,5), (2,4) -> 2 good pairs. 20-2=18? But sample output is 16.
    // Wait, sample 1 output is 16. Let's recalc: sum=15, n=5, total=20. 20-2=18, not 16.
    // So my formula is wrong. Let's re-evaluate.

    // Let's simulate sample 1: a=[1,2,3,4,5], m=6.
    // One optimal order: 1, 5, 2, 4, 3.
    // Download 1 (1 min). Watch 1 (1 min). During watch 1, download 5? 1+5=6 <=6, yes.
    // Download 5 takes 5 min. After 1 min of watch 1, 5 has downloaded 1 MB, 4 left.
    // Then watch 1 ends, delete 1. Continue downloading 5 for 4 more min. Total time so far: download 1 (1) + watch 1 (1) = 2, but download 5 started at time 1, finishes at time 1+5=6.
    // After download 5 finishes (time 6), watch 5 (1 min, time 6-7). During watch 5, download 2? 5+2=7>6, cannot. So must wait until watch 5 ends (time 7), delete 5, then download 2 (2 min, time 7-9). Watch 2 (1 min, time 9-10). During watch 2, download 4? 2+4=6 <=6, yes. Download 4 takes 4 min, starts at time 9, finishes at 13. Watch 2 ends at 10, delete 2. Continue download 4 until 13. Watch 4 (1 min, 13-14). During watch 4, download 3? 4+3=7>6, cannot. Wait until 14, delete 4, download 3 (3 min, 14-17). Watch 3 (1 min, 17-18). Total 18? But sample says 16. So my simulation is wrong.

    // Let's find a better order: maybe 3, 1, 5, 2, 4?
    // Download 3 (3 min, 0-3). Watch 3 (1 min, 3-4). During watch 3, download 1? 3+1=4<=6, yes. Download 1 takes 1 min, finishes at 4. At time 4, watch 3 ends, delete 3. Now we have 1 downloaded. Watch 1 (1 min, 4-5). During watch 1, download 5? 1+5=6<=6, yes. Download 5 takes 5 min, starts at 4, finishes at 9. Watch 1 ends at 5, delete 1. Continue download 5 until 9. Watch 5 (1 min, 9-10). During watch 5, download 2? 5+2=7>6, no. Wait until 10, delete 5, download 2 (2 min, 10-12). Watch 2 (1 min, 12-13). During watch 2, download 4? 2+4=6<=6, yes. Download 4 takes 4 min, starts at 12, finishes at 16. Watch 2 ends at 13, delete 2. Continue download 4 until 16. Watch 4 (1 min, 16-17). Total 17. Still not 16.

    // Sample 1 output is 16. Let's try to achieve 16.
    // Maybe order: 2, 4, 1, 5, 3?
    // Download 2 (2 min, 0-2). Watch 2 (1 min, 2-3). During watch 2, download 4? 2+4=6<=6, yes. Download 4 takes 4 min, starts at 2, finishes at 6. Watch 2 ends at 3, delete 2. Continue download 4 until 6. Watch 4 (1 min, 6-7). During watch 4, download 1? 4+1=5<=6, yes. Download 1 takes 1 min, starts at 6, finishes at 7. Watch 4 ends at 7, delete 4. Now we have 1 downloaded. Watch 1 (1 min, 7-8). During watch 1, download 5? 1+5=6<=6, yes. Download 5 takes 5 min, starts at 7, finishes at 12. Watch 1 ends at 8, delete 1. Continue download 5 until 12. Watch 5 (1 min, 12-13). During watch 5, download 3? 5+3=8>6, no. Wait until 13, delete 5, download 3 (3 min, 13-16). Watch 3 (1 min, 16-17). Total 17.

    // Maybe we can start downloading while previous is still downloading? No, cannot run parallel downloads.
    // Let's think differently. The total time is sum a_i + n - (number of times we can start a download during a watch). Each time we start a download during a watch, we save 1 minute of waiting. But we can only start a download during a watch if the current video being watched plus the new video sum <= m. However, we can also start a download immediately after a download finishes, without waiting for the watch to start? Actually, the process: we download video A, then we can either watch A immediately or start downloading B? The problem says: "Once a video is fully downloaded to the hard disk, Monocarp can watch it. Watching each video takes exactly 1 minute and does not occupy the Internet connection, so Monocarp can start downloading another video while watching the current one." So after downloading A, we can immediately start watching A, and during that 1 minute, we can download B. But we cannot download B before watching A because we must watch A to free space? Actually, we can delete A only after watching it. So if we want to download B, we need space for both A and B if we start downloading B before deleting A. So the condition for starting B during the watch of A is a_A + a_B <= m. If we start B after watching A and deleting A, we need only a_B <= m, which is always true. So the only way to overlap is to start the next download during the 1-minute watch of the current video. That saves exactly 1 minute per such overlap, because the download of the next video would otherwise start after the watch ends. But wait, what if the next video is large and its download takes longer than 1 minute? The overlap still saves 1 minute because we start it 1 minute earlier. So each good pair (adjacent in the watching order) where the sum of their sizes <= m saves 1 minute. So total time = sum a_i + n - (number of good adjacent pairs). But sample 1 gives 16, and sum a_i=15, n=5, so 20 - good_pairs = 16 => good_pairs = 4. Can we have 4 good pairs out of 4 adjacent pairs (since 5 videos have 4 adjacencies)? That would mean all adjacent pairs are good. Is that possible? We need an ordering of 1,2,3,4,5 such that every adjacent pair sums <=6. Pairs: (1,5)=6 ok, (5,? ) next must be <=1 to sum <=6, but we have 2,3,4 left. (5,1) is already used. (5,?) no. So impossible. So my formula must be wrong.

    // Let's re-read the problem carefully. "Once Monocarp starts the download of a video of size s, the s megabytes are immediately reserved on a hard disk. If there are less than s megabytes left, the download cannot be started until the required space is freed." So when we start downloading B during the watch of A, we need s_B <= free space. At that moment, A is still on disk, so free space = m - a_A. So condition is a_B <= m - a_A, i.e., a_A + a_B <= m. That's correct.
    // But maybe we can start downloading B not during the watch of A, but during the download of A? No, parallel downloads not allowed.
    // What about starting B after A is downloaded but before watching A? We can watch A immediately, so no gap.
    // So the only overlap is during the 1-minute watch. So each good pair saves 1 minute. But sample 1 output 16 implies 4 good pairs. How can we get 4 good pairs with 5 videos? Let's try to find an order with 4 good pairs. That means every adjacent pair sum <=6. The pairs must be (1,5), (5,1) but we can't repeat. So maybe we can have a sequence like 1,5,? but 5+? >6 unless ?=1. So impossible. So my understanding of "good pair" is wrong.

    // Let's simulate the sample 1 with the correct answer 16. How to achieve 16?
    // Maybe we can download videos in an order that allows more overlap. Consider: we can start downloading a video while watching another, but we can also start downloading a video immediately after the previous download finishes, and then watch the previous video while the new one downloads? Actually, the timeline:
    // t=0: start download A (size a_A). It takes a_A minutes.
    // t=a_A: download A finishes. Now we can start watching A (1 min) and simultaneously start download B (if space permits).
    // If we start B at t=a_A, B will finish at t=a_A + a_B.
    // Watching A finishes at t=a_A+1. At that time, we can delete A, freeing space.
    // If a_A + a_B <= m, we could start B at t=a_A. If a_A + a_B > m, we cannot start B until A is deleted, i.e., at t=a_A+1. Then B finishes at t=a_A+1+a_B.
    // So the difference is 1 minute. So each time we can start the next download during the watch, we save 1 minute.
    // But what if we have a chain? Suppose we have A, B, C. We download A, then during watch A we start B. B downloads, then during watch B we start C. That saves 2 minutes. So total time = sum a_i + n - (number of times we start a download during a watch). That number is exactly the number of good adjacent pairs in the watching sequence. So for 5 videos, max good pairs is 4. But we saw it's impossible to have 4 good pairs with these numbers. So maybe we can have good pairs that are not adjacent in the watching sequence? No, the overlap only happens between consecutive videos.

    // Let's try to find a sequence that gives time 16. Total download + watch = 20. To get 16, we need to save 4 minutes. That means we must start 4 downloads during watches. But there are only 4 transitions between 5 videos. So we must start every next download during the watch of the previous. That requires all adjacent pairs sum <=6. Let's check if there is an ordering: we need a permutation of 1,2,3,4,5 such that for every i, a_i + a_{i+1} <= 6. The pairs must be: (1,5) sum 6, (5,1) sum 6, but we can't use 1 twice. (2,4) sum 6, (4,2) sum 6. (3,3) not possible. So we can't have all four. So 4 good pairs is impossible. Thus my formula must be missing something.

    // Let's manually find a schedule that takes 16 minutes.
    // We have videos: 1,2,3,4,5. m=6.
    // Idea: Maybe we can start downloading a video while the previous is still downloading? No, not allowed.
    // Maybe we can delete a video before watching it? No, must watch after download.
    // Maybe we can download multiple videos without watching them immediately? Yes, we can download a video, then instead of watching it, we can download another video if we have space? But we can only delete after watching. So if we download A, it stays on disk until we watch it. We can download B only if there is space for both A and B. So we can accumulate downloaded videos. Then we can watch them in any order, deleting after watch. This might allow more overlap? Let's think.

    // Suppose we download 1 (size 1). Disk: 1 used. Then we download 2 (size 2). Need 1+2=3<=6, ok. Disk: 1,2. Then download 3 (size 3). Need 1+2+3=6<=6, ok. Disk full. Now we have 1,2,3 downloaded. We can watch 1 (1 min), delete it, free 1. During watch 1, we can download 4? Disk has 2,3 (total 5), free 1. 4 needs 4, not enough. So cannot. After watch 1, delete 1, free 1, total free 2. Still not enough for 4. So we watch 2 (1 min), delete 2, free 2, total free 4. During watch 2, we can download 4? Disk has 3 (size 3), free 3. 4 needs 4, not enough. After watch 2, delete 2, free 2, total free 5. Now we can download 4 (size 4). Then watch 3, etc. This might save time because we downloaded multiple videos upfront, and then we can watch them while downloading others? But the total download time is still sum a_i. The total watch time is n. The only way to reduce total time below sum a_i + n is to overlap download with watch. In this scenario, we overlapped download of 1,2,3 with nothing? Actually, we downloaded 1,2,3 sequentially without watching anything. That took 1+2+3=6 minutes. Then we started watching. During watches, we tried to download but couldn't always. So total time = download time + watch time - overlap. Overlap only happens when we download during a watch. In this schedule, we didn't download during the first three downloads because we weren't watching. So we lost potential overlap. So maybe it's better to interleave.

    // Let's try to find a schedule that achieves 16. I'll write a small script in my head. We need to finish at t=16.
    // Total download = 15, total watch = 5. So we must have 4 minutes of overlap. That means the CPU (download) must be busy for 15 minutes, and the watch must happen during 5 of those minutes, but since watch takes 1 minute each and doesn't use CPU, we can have download running during all 5 watch minutes. So the total time = download_time + watch_time - overlap_time. Overlap_time is the amount of time that download and watch happen simultaneously. Since watch is only 1 minute per video, the maximum overlap is 5 minutes (if we download during every watch). But we can't download during the watch of the last video because there's no next video. So max overlap is 4 minutes. So to get 16, we need overlap = 4 minutes. That means we must be downloading during all 4 watches of the first 4 videos. That requires that for each of the first 4 videos, during its 1-minute watch, we are downloading the next video. That means we must start the next video's download at the beginning of the current video's watch. That requires that at the start of the current video's watch, we have enough free space for the next video. At the start of the watch, the current video is still on disk. So we need a_current + a_next <= m. So indeed, we need all adjacent pairs in the sequence to have sum <= m. But we already saw that's impossible with these numbers. So how can we get 4 overlap minutes? Maybe we can download a video during the watch of a video that is not the immediate next? For example, we download A, watch A while downloading B. Then we watch B while downloading C. That's two overlaps. But what if we download A, then download B (without watching A), then watch A while downloading C? Let's see.

    // Suppose we download 1 (t=0-1). Then download 5 (t=1-6). During t=1-6, we are downloading 5. At t=1, we could start watching 1? But we are downloading 5, so we can watch 1 simultaneously. So at t=1, we start watching 1 (t=1-2) and continue downloading 5. That's overlap of 1 minute. At t=2, watch 1 ends, delete 1. Disk now has only 5 (downloading). At t=6, download 5 finishes. Now we can watch 5 (t=6-7). During t=6-7, we can download something. What can we download? Disk has 5 (size 5), free 1. We can download 1? But 1 already watched. We need to download 2,3,4. 2 needs 2, not enough. So we cannot download during watch of 5. So only 1 overlap so far. Total time would be: download 1 (1), download 5 (5) overlapping watch 1 (1) -> download 5 finishes at 6. Watch 5 (1) -> 7. Then download 2 (2) -> 9, watch 2 (1) -> 10, download 4 (4) overlapping watch 2? During watch 2 (9-10), we can download 4 if 2+4<=6, yes. So start download 4 at 9, finishes at 13. Watch 2 ends at 10, delete 2. Continue download 4. At 13, watch 4 (1) -> 14. During watch 4, download 3? 4+3=7>6, no. So download 3 at 14-17, watch 3 17-18. Total 18. Overlaps: watch 1 with download 5 (1 min), watch 2 with download 4 (1 min) = 2 overlaps. 20-2=18.

    // To get 16, we need 4 overlaps. How can we get more overlaps? Maybe we can download a video during the watch of a video that was downloaded earlier but not watched immediately? For instance, we download several videos first, then watch them in an order that allows downloading during each watch. Let's try: download 1 (0-1), download 2 (1-3) [need space 1+2=3<=6], download 3 (3-6) [1+2+3=6<=6]. Now disk has 1,2,3. At t=6, we start watching 1 (6-7). During watch 1, we want to download 4. Disk has 1,2,3 (full). Cannot download 4. So no overlap. After watch 1, delete 1, free 1. Disk: 2,3. Watch 2 (7-8). During watch 2, download 4? Disk: 2,3 (5 used), free 1. 4 needs 4, no. After watch 2, delete 2, free 2, total free 3. Disk: 3. Watch 3 (8-9). During watch 3, download 4? Disk: 3, free 3. 4 needs 4, no. After watch 3, delete 3, free 3, total free 6. Download 4 (9-13), watch 4 (13-14), download 5 (14-19), watch 5 (19-20). Total 20. No overlap.

    // What if we download 1, then watch 1 while downloading 2? That's one overlap. Then we have 2 downloaded. Then we watch 2 while downloading 3? Need 2+3=5<=6, yes. So: download 1 (0-1), watch 1 (1-2) + download 2 (1-3) -> overlap 1 min. At t=2, watch 1 ends, delete 1. Disk: 2 (downloading). At t=3, download 2 finishes. Watch 2 (3-4) + download 3 (3-6) -> overlap 1 min. At t=4, watch 2 ends, delete 2. Disk: 3 (downloading). At t=6, download 3 finishes. Watch 3 (6-7) + download 4? Need 3+4=7>6, no. So no overlap. Then download 4 (7-11), watch 4 (11-12) + download 5? 4+5=9>6, no. So total: download 1 (1), download 2 (2), download 3 (3), download 4 (4), download 5 (5) = 15. Watches: 5. Overlaps: 2. Total = 15+5-2=18.

    // To get 16, we need 4 overlaps. Let's try to order videos such that we can chain more overlaps. The condition for overlap is that the video being watched and the video being downloaded must fit together. But note: the video being downloaded doesn't have to be the immediate next to watch. We can download a video, then not watch it immediately, but watch another video that is already downloaded. For example, we download A, then download B (while watching nothing? No, we can watch A while downloading B). Then we have A and B downloaded. Then we can watch A (if not watched) or B. But we already watched A during B's download. So A is gone. So we can only watch a video once.

    // Let's think of a different strategy: we can download videos in an order, and watch them in a different order. The only constraint is that a video must be downloaded before it can be watched. We can delete a video only after watching it. So the disk stores a set of downloaded but not yet watched videos. We can start a download if there is enough free space. We can watch any already downloaded video at any time, taking 1 minute, during which we can also download.

    // This is a scheduling problem. We want to minimize makespan. This is similar to problem "Watching Videos" from Codeforces? Actually, I recall a problem: "Monocarp and the Videos" or similar. The solution often involves sorting and greedy.

    // Let's search my memory: There is a Codeforces problem "Videos" where you need to watch videos, each has size a_i, disk size m, download speed 1, watch time 1. The answer is sum a_i + n - max number of pairs we can form such that sum <= m, but the pairs are not necessarily adjacent in the final sequence? Wait, I remember a problem: "E. Watching Videos" or something. The key idea: we can think of the process as we have a set of videos. We can download them in any order, and watch them in any order. The total time is sum a_i + n - (maximum number of videos we can download during the watch of another video). Each time we download a video during the watch of another, we save 1 minute. When can we download video B during the watch of video A? We need A and B to be on disk simultaneously? Actually, during the watch of A, A is on disk. We want to start downloading B. So we need a_A + a_B <= m. But B doesn't have to be the next one we watch. We can download B, and then later watch B, and during B's watch we can download C, etc. So we can form a chain of videos where each is downloaded during the watch of the previous. The chain can be of length k+1 if we have k overlaps. The condition for the chain is that for each consecutive pair in the chain, their sum <= m. But the chain is the order in which we start downloads? Actually, the chain is the order of watching: we watch A, during which we download B. Then we watch B, during which we download C, etc. So the watching order is A, B, C, ... and we need a_A + a_B <= m, a_B + a_C <= m, etc. So it's exactly a path in a graph where edges exist if sum <= m. We want to find a Hamiltonian path that maximizes the number of edges (overlaps). That is equivalent to finding the longest path in this graph. But the graph is defined by sum <= m. Since we can watch in any order, we want to order all n videos to maximize the number of adjacent pairs with sum <= m. That's exactly what I thought initially. And we found that for sample 1, max such pairs is 2, giving 18, but answer is 16. So there must be a way to get more overlaps by not watching in the same order as downloading? But the overlap requires that we are watching the video that we previously downloaded, and during its watch we download the next video. The next video we download will be watched later. So the watching order is exactly the order in which we start downloads? Not necessarily. We could download B during the watch of A, but then not watch B immediately. We could watch some other video C that was already downloaded. But then the overlap is between A and B, but B is not watched next. However, the overlap still saves 1 minute. The total number of overlaps is the number of times we start a download during a watch. Each such event involves a pair (watched video, downloaded video). The watched video must be on disk during its watch, and the downloaded video must fit. So we need a_W + a_D <= m. After the watch, the watched video is deleted. The downloaded video remains on disk until watched. So we can have multiple downloaded videos on disk. This is more flexible.

    // Let's model: We have a set of videos. We can perform actions: download a video (takes a_i time, requires free space >= a_i), watch a video (takes 1 time, requires it is downloaded, during which we can also download). We can delete after watch. We want to minimize total time.

    // This is equivalent to: we have a single machine for downloading, and we need to schedule downloads and watches. Watches are quick (1 min) and can overlap with downloads. The only constraint is disk space. This is similar to a problem where we want to maximize the number of downloads that occur during watches. Each watch can "cover" at most one download start? Actually, during a 1-minute watch, we can start a download, and that download will continue after the watch ends. We can only start one download at a time (no parallel). So during a watch, we can start exactly one download (if we choose to). So each watch can initiate at most one download. The total number of downloads initiated during watches is at most n-1 (since the last watch has no next download). So the maximum overlap is n-1 minutes. But we can't always achieve that due to space constraints.

    // The space constraint: when we start a download during a watch, the watched video is on disk, and we also have any other downloaded but unwatched videos on disk. So the free space is m - (size of watched video) - (sum of sizes of other unwatched videos). To start a download of size s, we need s <= free space. So to maximize overlaps, we want to keep the disk as empty as possible. That means we should watch videos as soon as possible after downloading them, to free space. But we also want to chain overlaps.

    // Let's think of a greedy strategy: we want to form a sequence of videos where we download one, watch it while downloading the next, watch that while downloading the next, etc. This chain requires that at each step, the sum of the current video and the next video <= m, AND there are no other videos on disk. Because if we have other videos on disk, they consume space and might prevent the next download. So to maximize chain length, we should not accumulate unwatched videos. So the optimal strategy might be to partition the videos into chains, where within each chain we have the property that consecutive sums <= m. But we can also have multiple chains? We have to watch all videos. We can process one chain, then the next? But we can't start a new chain without downloading the first video of the new chain. That first download cannot overlap with a watch (unless we have a video from the previous chain still unwatched? But we would have watched all in the previous chain). So the first video of each chain must be downloaded without overlap. So if we have c chains, we lose c-1 overlaps? Actually, if we have one chain of length n, we get n-1 overlaps. If we have two chains, we get (len1-1) + (len2-1) = n-2 overlaps. So we want to minimize the number of chains, i.e., maximize the length of a single chain that covers all videos. But we can't always have a single chain because the sum condition might not hold for all consecutive pairs. However, we are not forced to watch videos in the
