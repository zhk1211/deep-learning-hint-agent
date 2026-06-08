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
    
    long long ans = n; // watching time
    long long sum = 0;
    long long free_space = m;
    
    for (int i = 0; i < n; ++i) {
        if (free_space >= a[i]) {
            free_space -= a[i];
            sum += a[i];
        } else {
            // need to free space by watching some videos
            // we need a[i] - free_space more space
            long long need = a[i] - free_space;
            // we can free space by watching videos that are already downloaded
            // each watched video frees its size
            // we need to watch enough to free 'need' space
            // but we also need to account for the fact that while watching,
            // we can't download, so we add watching time
            // Actually, we just need to wait until enough space is freed.
            // The total time is sum of downloads + watching time.
            // The watching time is n, but some watching can overlap with downloads.
            // The bottleneck is the total download time plus the time we are forced to wait
            // because we can't download due to insufficient space.
            // The optimal strategy: download in sorted order, watch whenever possible.
            // The total time = n (watching) + total download time - overlap.
            // Overlap happens when we can watch while downloading.
            // The maximum overlap is limited by the sum of sizes of videos that can be watched
            // while others are downloading.
            // Another way: total time = n + sum(a) - (total freed space that can be used to overlap)
            // But simpler: simulate with two pointers or just compute the answer.
            // Known solution: ans = n + sum(a) - max(0, min_element_of_something)?
            // Let's derive properly.
            
            // We have already downloaded some videos, sum_downloaded = sum.
            // We have free_space left.
            // To download a[i], we need to free 'need' space.
            // We can free space by watching already downloaded videos.
            // Each watched video takes 1 minute and frees its size.
            // During that 1 minute, we cannot download.
            // So we add 1 minute of waiting for each video we watch to free space.
            // But we also have to watch all n videos anyway.
            // The total time = n (total watching) + total_download_time - overlapped_time.
            // Overlapped time = time when we are both downloading and watching.
            // Initially, we can download without watching until disk is full.
            // Then we must watch to free space.
            
            // Let's compute the answer directly:
            // We process videos in sorted order.
            // We maintain the current time.
            // But we can just use the formula:
            // ans = n + sum(a) - max_{k} (sum of smallest k a_i - k) ? 
            // Actually, the maximum time we can save is the maximum over k of (sum of k smallest a_i - k),
            // but only if that sum <= m? No.
            
            // Let's think differently: The process is equivalent to:
            // We have a buffer of size m. We can download a video if there's space.
            // We can watch a video if it's fully downloaded.
            // Watching takes 1 minute and frees its space.
            // We want to minimize total time.
            // Total time = n (watching) + total download time - (time spent watching while downloading).
            // The time spent watching while downloading is exactly the total time we are downloading
            // and simultaneously watching. This is limited by the total watching time n,
            // and also by the fact that we can only watch videos that are downloaded.
            // The maximum overlap is achieved by watching videos as soon as possible.
            // The optimal order is to download in increasing order of size.
            // Then the total time = n + sum(a) - max(0, max_{k} (sum_{i=1}^k a_i - k))? 
            // Wait, let's test with examples.
            
            // Example 1: n=5, m=6, a=[1,2,3,4,5] sorted.
            // sum=15, n=5, total=20.
            // Overlap: we can watch while downloading.
            // Let's simulate:
            // t=0: start download 1 (size 1). free=5.
            // t=1: download 1 done. free=5. Watch 1? We can watch it now, but we can also start download 2.
            // If we watch 1, time 1-2 watching, during that we can't download.
            // Better: start download 2 immediately. t=1: start download 2 (size 2). free=3.
            // t=3: download 2 done. free=3. Now we have 1 and 2 downloaded.
            // We can watch one while downloading next.
            // Start download 3 (size 3). free=0. While downloading 3 (t=3 to 6), we can watch 1 and 2.
            // Watching takes 2 minutes, but download takes 3 minutes. So we can watch both during download.
            // t=6: download 3 done. free=0. We have watched 1 and 2. Now we have 3 downloaded.
            // We need to download 4 (size 4). No space. We must watch 3 to free 3 space.
            // Watch 3: t=6-7. free becomes 3. Still not enough for 4 (need 4).
            // We have no other downloaded videos. So we must wait? Actually we can't download 4 until we have 4 space.
            // After watching 3, we have 3 space. We need 1 more. But we have no other videos downloaded.
            // So we are stuck. We must download something? We can't because no space.
            // Wait, we haven't watched 4 or 5 yet. We only have 3 space. We can't download 4.
            // So we must watch something else? But nothing else is downloaded.
            // This means we are deadlocked? No, we can delete watched videos. After watching 3, we deleted it, so we have 3 space.
            // We need 4 space to download 4. We can't. So we must have downloaded something smaller?
            // But we already downloaded 1,2,3. We can't download 4.
            // So the process fails? But the problem says we can always watch all videos.
            // Let's re-read: "Once Monocarp starts the download of a video of size s, the s megabytes are immediately reserved on a hard disk. If there are less than s megabytes left, the download cannot be started until the required space is freed."
            // So we cannot start download of 4 until we have 4 free space.
            // After watching 3, we have 3 free space. We need 4. We have no other videos downloaded to watch and free more space.
            // So we are stuck. But the example output is 16, so it must be possible.
            // Let's rethink the order. Maybe we shouldn't watch 1 and 2 immediately. Maybe we should watch 1, then download 2, etc.
            // Let's try a different order: download 1, watch 1, download 2, watch 2, download 3, watch 3, download 4, watch 4, download 5, watch 5.
            // That would take: download 1 (1 min), watch 1 (1 min) -> total 2, but during watch we can't download, so sequential: 1+1+2+1+3+1+4+1+5+1 = 20. Not 16.
            // So we need to parallelize.
            // The key is that we can watch a video while downloading another, but we can only watch videos that are already downloaded.
            // Let's find a schedule that achieves 16.
            // Total download = 15, total watch = 5, sum = 20. Overlap = 4. So we need to overlap 4 minutes of watching with downloading.
            // How to get 4 overlap?
            // Maybe: download 1 (t=0-1). Then download 2 (t=1-3). During t=1-3, we can watch 1 (t=1-2). Overlap 1 min.
            // Then at t=3, we have 2 downloaded. free=6-1-2=3? Wait, after downloading 1 and 2, used=3, free=3.
            // We want to download 3 (size 3). We have exactly 3 free, so we can start download 3 at t=3. It takes 3 min (t=3-6).
            // During t=3-6, we can watch 2 (t=3-4) and maybe something else? We only have 1 and 2 downloaded. 1 already watched? We can watch 1 again? No, each video watched once.
            // So we watch 2 during t=3-4. Overlap 1 min. Then at t=4, we have watched 1 and 2. Downloaded: 3 is still downloading until t=6. free=0.
            // At t=6, download 3 done. free=0. We have 3 downloaded. We need to download 4 (size 4). No space. We must watch 3 to free 3 space. Watch 3 (t=6-7). free=3. Still not enough for 4.
            // We have no other downloaded videos. So we are stuck again.
            // So this schedule fails.
            // Maybe we should not download 2 immediately after 1. Let's try: download 1 (t=0-1). Watch 1 (t=1-2). During t=1-2, we can't download. At t=2, free=6. Download 3 (size 3) t=2-5. free=3. During t=2-5, we can watch? We have no other downloaded videos except 1 which is already watched. So no overlap.
            // At t=5, download 3 done. free=3. Watch 3 (t=5-6). free=6. Download 4 (t=6-10). free=2. During t=6-10, we can watch? We have no downloaded videos except 3 which is watched. So no overlap.
            // This is worse.
            // Let's look for the optimal strategy. I recall a known problem: "Monocarp and the videos" or similar. The solution is to sort a, then the answer is n + sum(a) - max_{k} (sum_{i=1}^k a_i - k) but with a condition? Let's test on examples.
            // Example 1: a=[1,2,3,4,5], sum=15, n=5.
            // k=1: sum=1, 1-1=0
            // k=2: sum=3, 3-2=1
            // k=3: sum=6, 6-3=3
            // k=4: sum=10, 10-4=6
            // k=5: sum=15, 15-5=10
            // max is 10. Then ans = 5+15-10 = 10? But output is 16. So that's not it.
            // Maybe max over k such that sum_{i=1}^k a_i <= m? For m=6:
            // k=1: sum=1 <=6, value=0
            // k=2: sum=3 <=6, value=1
            // k=3: sum=6 <=6, value=3
            // k=4: sum=10 >6, stop. max=3. ans=20-3=17. But output is 16. So not that either.
            // Example 2: m=5, a=[1,2,3,4,5]. sum=15, n=5. Output 17.
            // If we use max over k with sum<=m: k=1:0, k=2:1, k=3:6>5 stop. max=1. ans=20-1=19 !=17.
            // So that's wrong.
            
            // Let's think differently. The process is equivalent to: we have a buffer of size m. We can download videos in some order. We can watch any fully downloaded video at any time, which takes 1 minute and frees its space. We want to minimize makespan.
            // This is similar to scheduling with a resource constraint.
            // Consider the total time T. In T minutes, we must download all videos (total size S = sum a_i) and watch all videos (n minutes). Downloading and watching can overlap, but we can only watch a video after it's downloaded, and we can only download if there's space.
            // The space constraint: at any time, the total size of downloaded but not yet watched videos plus the size of the currently downloading video cannot exceed m.
            // Let x_i be the time we start downloading video i, and y_i be the time we start watching it. y_i >= x_i + a_i. Watching takes 1 minute, so it finishes at y_i+1. The video occupies space from x_i to y_i+1 (since it's deleted after watching). During [x_i, x_i+a_i] it's being downloaded, so it occupies its full size. After download, it occupies its full size until it's watched and deleted.
            // So the space used at time t is sum_{i: x_i <= t < y_i+1} a_i.
            // We need this sum <= m for all t.
            // We want to minimize max_i (y_i+1).
            // This is a known problem: it's equivalent to scheduling with release times and deadlines? Actually, it's similar to the problem of watching videos with a buffer.
            // I remember a solution: sort a ascending. Then the answer is n + sum a_i - max_{k} (sum_{i=1}^k a_i - k) but with the condition that we can only take k such that sum_{i=1}^k a_i <= m + something? No.
            // Let's re-read the hints.
            // Hint 1: How many videos can Monocarp watch without downloading any? Answer: 0, because he needs to download them first.
            // Hint 2: What happens when Monocarp watches one video? He frees space equal to its size.
            // Hint 3: Can you determine the optimal order of watching the videos? Hint 4: Think of the case where Monocarp watches the videos in non-decreasing order.
            // So the optimal order is to watch in non-decreasing order of size? Or download in non-decreasing order?
            // Usually, to minimize makespan, you want to download smallest first so you can watch them quickly and free space for larger ones.
            // Let's try to simulate the process optimally with a priority queue or something.
            // Since n is up to 2e5, we need O(n log n) or O(n).
            // Let's think of the process as: we have a set of downloaded videos. We can either download a new video (if space allows) or watch a downloaded video (takes 1 minute, frees space). We want to finish all asap.
            // This is like: we have a buffer of size m. We can add items of size a_i, which take a_i time to add. We can remove items (which takes 1 time unit and frees their space). We want to add and remove all items.
            // This is exactly the problem of "minimum time to process items with a buffer".
            // I recall a greedy strategy: always download the smallest available video if there is space; if not, watch the smallest downloaded video to free space? Or watch the largest? To free space quickly, we should watch the largest downloaded video? But watching takes 1 minute regardless of size, so watching a larger video frees more space per minute of watching. So to free space efficiently, we should watch the largest downloaded video.
            // But we also need to watch all videos eventually, so watching a large video early might be good because it frees a lot of space, allowing us to download other videos sooner.
            // However, the hints say "Think of the case where Monocarp watches the videos in non-decreasing order." That suggests watching in non-decreasing order of size is optimal? That would mean watching smallest first. That contradicts the idea of freeing space quickly. But maybe the optimal download order is non-decreasing, and the watch order is also non-decreasing? Let's test with example 1.
            // If we watch in non-decreasing order, we watch 1, then 2, then 3, then 4, then 5.
            // Let's try to schedule downloads to minimize time with that watch order.
            // We must download videos before watching them. We can download in any order, but we must respect the watch order? No, the problem says order of watching does not matter. So we can choose any watch order. The hint says "Think of the case where Monocarp watches the videos in non-decreasing order." That implies we can assume without loss of generality that the optimal watch order is non-decreasing? Or maybe it's a hint to consider that specific order to derive a formula.
            // Let's assume we watch in non-decreasing order: w1 <= w2 <= ... <= wn.
            // Then we need to download them in some order. To minimize time, we should download them in the same order? Because if we download a larger one first, it occupies space for a long time and delays smaller ones. So download order should also be non-decreasing.
            // So both download and watch order are sorted ascending.
            // Now, let's simulate with sorted a: [1,2,3,4,5], m=6.
            // We want to find the minimum time T such that we can schedule downloads and watches.
            // Let's think of the process as: we have a queue of videos to download (sorted). We maintain the set of downloaded videos (sorted by size). At any time, we can either start downloading the next video if space >= a_i, or watch the smallest downloaded video (since we watch in non-decreasing order, we must watch the smallest available). But wait, if we watch in non-decreasing order, we must watch the smallest downloaded video first. That means we cannot watch a larger video before a smaller one even if it's downloaded. So the watch order is fixed to ascending.
            // Under this constraint, what is the optimal schedule?
            // We can think of it as: we process videos in order of size. For each video, we need to download it (takes a_i time) and then watch it (takes 1 time). The watching of video i can only start after it's downloaded AND after video i-1 has been watched (since watch order is ascending). So the watch start time y_i >= max(download_end_i, y_{i-1}+1).
            // The download of video i can start after we have enough space. The space is freed when we finish watching videos. Video j frees its space at y_j+1.
            // So the space used at time t is sum of a_j for videos j that have started downloading but not yet finished watching (i.e., x_j <= t < y_j+1).
            // We need this <= m.
            // This is a complex scheduling problem. But maybe we can find a closed form.
            // Let's denote the sorted sizes a_1 <= a_2 <= ... <= a_n.
            // Suppose we download and watch in this order.
            // Let's try to construct a feasible schedule and compute its makespan.
            // We can think of the process as: we have a buffer. We add videos in order. We can only remove (watch) them in order.
            // This is exactly the problem of "minimum time to process items with a FIFO buffer" or something.
            // Actually, if we watch in the same order as download, then the system is like a pipeline: download i, then when it's done, we may watch it immediately or wait if previous watch is not finished.
            // But we can also download multiple videos before watching any, as long as space permits.
            // Let's try to find the makespan for example 1 with this strategy.
            // a = [1,2,3,4,5], m=6.
            // We want to download and watch in order.
            // Let's try: download 1 (t=0-1). Space used: 1. After download, we can watch it immediately? But we might want to download 2 first to parallelize.
            // Suppose we download 1, then download 2, then watch 1, etc.
            // Let's use a greedy approach: at any time, if we can download the next video (space >= a_i), do it. Else, if there is a downloaded video that is next to watch, watch it. Else, we must wait? But we can always watch the next video if it's downloaded.
            // Let's simulate this greedy:
            // t=0: space=6. Download 1 (size 1). Space=5. Download takes 1 min, so busy until t=1.
            // t=1: download 1 done. Downloaded: [1]. Next to watch: 1. Next to download: 2 (size 2). Space=5 >=2, so we can download 2. Should we watch 1 now or download 2? If we watch 1, we can't download during t=1-2. If we download 2, we can watch 1 later. To minimize makespan, we should prioritize downloading to keep the pipeline busy? Actually, watching is also necessary and takes time. We want to overlap as much as possible. So we should download if possible, because downloading takes a_i time which might be >1, and we can watch during that download. So at t=1, start download 2 (size 2). Space=3. Download 2 from t=1 to 3.
            // During t=1-3, we are downloading. Can we watch something? We have 1 downloaded. Next to watch is 1. We can watch 1 while downloading 2. Watching takes 1 min. So we can watch 1 from t=1 to 2. During that time, download continues. So at t=1, we start both download 2 and watch 1? But the problem says: "Once the download is started, it cannot be interrupted. It is not allowed to run two or more downloads in parallel." It doesn't say we can't watch while downloading. In fact: "Watching each video takes exactly 1 minute and does not occupy the Internet connection, so Monocarp can start downloading another video while watching the current one." So yes, we can watch and download simultaneously.
            // So at t=1: start download 2 and start watch 1. Watch 1 finishes at t=2. Download 2 continues until t=3.
            // At t=2: watch 1 done. Space freed: 1. So space becomes 3+1=4? Wait, space used by 1 was 1, and it was watched and deleted, so space increases by 1. But during download, the space for 2 is reserved. So at t=2, download 2 is still ongoing (uses 2 space). 1 is deleted. So used space = 2, free = 4. Next to watch: 2 (not downloaded yet). Next to download: 3 (size 3). Space=4 >=3, so we can start download 3? But we are already downloading 2. We can't start another download until the current one finishes. So we must wait until t=3.
            // At t=3: download 2 done. Downloaded: [2]. Next to watch: 2. Next to download: 3. Space used: 2 (from 2). Free=4. We can start download 3 (size 3). Space becomes 1. Download 3 from t=3 to 6. Also, we can watch 2. Watch 2 from t=3 to 4.
            // At t=4: watch 2 done. Space freed: 2. Used space: 3 (from 3). Free=3. Next to watch: 3 (not downloaded). Next to download: 4 (size 4). Space=3 <4, cannot download 4. We must wait until download 3 finishes? But we can't download anything else. We can't watch anything because 3 is not downloaded. So we idle from t=4 to 6.
            // At t=6: download 3 done. Downloaded: [3]. Next to watch: 3. Next to download: 4. Space used: 3. Free=3. We can watch 3 (t=6-7). During watch, we can't download? Actually we can start download if space permits. But space is 3, need 4 for 4. So we must watch 3 to free space. After watch 3 (t=7), space becomes 6. Then we can download 4 (t=7-11). During download 4, we can watch? Next to watch is 4, not downloaded. So we just download. At t=11, download 4 done. Then watch 4 (t=11-12). Space becomes 6. Download 5 (t=12-17). Watch 5 (t=17-18). Total time = 18. But example output is 16. So this schedule gives 18, not optimal.
            // So watching in non-decreasing order with this greedy download might not be optimal, or my greedy choice of always downloading if possible is wrong. Maybe we should sometimes watch instead of downloading, even if space is available, to free up space for larger videos? But in this case, at t=1 we had space for 2, and we downloaded it. That seemed fine. The idle time from t=4 to 6 was because we couldn't download 4 and had nothing to watch. Could we have avoided that by not downloading 3 so early? Suppose at t=3, instead of downloading 3, we watch 2 (which we did) but then we don't download 3? But we need to download 3 eventually. If we don't download 3, we have space 4. We could download 4? But we must watch in order: we need to watch 3 before 4. So we can't watch 4 before 3. So we must download 3 before we can watch 3, and we must watch 3 before we can watch 4. So we are forced to download 3 before 4. So the order of downloading must respect the watch order? Not necessarily: we can download 4 before 3, but we can't watch 4 until 3 is watched. So we could download 4, then download 3, then watch 3, then watch 4. But if we download 4 first, it occupies 4 space, leaving 2. Then we can't download 3 (size 3) until we free space. We could free space by watching something, but we haven't watched 1 and 2 yet? In this scenario, we already watched 1 and 2. So we have no downloaded videos to watch. So we'd be stuck with 4 occupying space, unable to download 3. So we must download in an order that allows us to eventually watch them in the required order. This is like a topological constraint: we can download in any order, but we can only watch in ascending order. So the download order must be such that we don't deadlock.
            // This is getting complicated. Let's look for a known solution.
            // I recall a Codeforces problem: "Watching Videos" or "Monocarp and the Videos". The solution is: sort a, then the answer is n + sum a_i - max_{k} (sum_{i=1}^k a_i - k) but with the condition that we only consider k such that sum_{i=1}^k a_i <= m + (k-1)? Or something like that.
            // Let's search my memory: There is a problem "Videos" where you have to watch videos, download time = size, watch time = 1, disk size m. The answer is n + sum a_i - max_{k} (sum_{i=1}^k a_i - k) subject to sum_{i=1}^k a_i <= m + k - 1? Let's test.
            // Example 1: m=6. k=1: sum=1 <=6+0=6, value=0. k=2: sum=3 <=6+1=7, value=1. k=3: sum=6 <=6+2=8, value=3. k=4: sum=10 <=6+3=9? 10<=9 false. So max=3. ans=20-3=17 !=16.
            // Example 2: m=5. k=1: sum=1 <=5, value=0. k=2: sum=3 <=6, value=1. k=3: sum=6 <=7, value=3. k=4: sum=10 <=8 false. max=3. ans=20-3=17. Output is 17! So for example 2, it matches. But example 1 output is 16, not 17. So my condition is slightly off.
            // Maybe the condition is sum_{i=1}^k a_i <= m + k? For m=6: k=1:1<=7, k=2:3<=8, k=3:6<=9, k=4:10<=10 true! value=10-4=6. k=5:15<=11 false. max=6. ans=20-6=14 !=16.
            // Maybe sum_{i=1}^k a_i <= m + (k-1) + something? Let's try to derive.
            // Let's think about the maximum number of videos we can have downloaded but not watched at any time. Suppose we have k videos downloaded and not watched. Their total size is at most m. But we can also be currently downloading one more video, which takes its size. So the total size of downloaded (unwatched) + downloading <= m. If we have k videos unwatched, and we are downloading the (k+1)-th video, then the sum of sizes of the k unwatched videos + size of downloading video <= m. But the k unwatched videos are some subset. To maximize k, we should pick the smallest k videos. So if we sort a, the maximum number of videos we can have in the "pipeline" (downloaded + downloading) is the largest k such that sum_{i=1}^{k} a_i <= m? Not exactly, because the downloading video is one of them. Actually, if we have k videos downloaded and not watched, and we are downloading the next one, then total size = sum of k unwatched + a_{next} <= m. The k unwatched are the ones we haven't watched yet. If we watch in ascending order, the unwatched ones are the largest ones? No, if we watch smallest first, the unwatched ones are the larger ones. So the sum of unwatched is large. That's bad. To maximize pipeline, we should watch largest first, so unwatched are the smallest. That contradicts the hint.
            // Hint 4 says: "Think of the case where Monocarp watches the videos in non-decreasing order." That means watch smallest first. So unwatched are the larger ones. That seems suboptimal for space usage. But maybe it's optimal for makespan? Let's test with example 1 if we watch in non-increasing order (largest first).
            // Watch order: 5,4,3,2,1.
            // We can download in any order. Let's try to find a good schedule.
            // We want to minimize time. Total download=15, watch=5.
            // Let's try: download 1 (t=0-1). Then download 2 (t=1-3). During t=1-3, we can watch? We want to watch 5 first, but it's not downloaded. So we can't watch anything. At t=3, we have 1 and 2 downloaded. Space used=3, free=3. We want to eventually watch 5. We need to download 5 (size 5). Not enough space. We could watch 2 to free space? But watch order is 5 first, so we can't watch 2 before 5. So we are stuck. So watching largest first is bad because we can't watch them until they are downloaded, and they are large, so we can't download them without freeing space, but we can't free space by watching smaller ones because we must watch larger first. So we deadlock. Therefore, we must watch in an order that allows progress. The only way to avoid deadlock is to watch in non-decreasing order? Or at least, we must watch some video that is already downloaded. So the watch order must be such that whenever we need to free space, there is a downloaded video that we are allowed to watch next. That means the next video to watch should be among the downloaded ones. So we should always watch the smallest downloaded video? That ensures we can always make progress if we have any downloaded videos. So the natural greedy watch order is: always watch the smallest downloaded video. That is equivalent to watching in non-decreasing order of size, because we can only watch videos that are downloaded, and we download them in some order. If we always watch the smallest available, the sequence of watched videos will be non-decreasing? Not necessarily, because we might download a large video early, but we won't watch it until all smaller ones are watched. So the watch order is exactly the sorted order if we eventually download all. So indeed, the optimal watch order is non-decreasing. So hint 4 is correct.
            // So we must watch in sorted order.
            // Now, what is the optimal download order? We can download in any order, but we must respect that we can only watch in sorted order. To minimize makespan, we should download videos in an order that allows us to watch them as soon as possible. Since we watch in sorted order, we need to download the smallest videos first so they can be watched early. But we also need to download larger videos early enough so that they are ready when it's their turn to be watched. However, downloading a large video early occupies space and prevents downloading smaller ones. So there's a trade-off.
            // This is exactly the problem of scheduling with a buffer and a fixed output order. This is known as the "minimum makespan for a single machine with buffer" or something. I think the optimal strategy is to download videos in non-decreasing order as well. Let's test if there's a better download order for example 1.
            // We already tried downloading in order 1,2,3,4,5 and got 18. Can we do better by downloading in a different order but still watching in sorted order?
            // Watch order: 1,2,3,4,5.
            // Suppose we download 1, then 3, then 2, then 4, then 5? Let's see.
            // t=0: download 1 (0-1). space=5.
            // t=1: 1 done. We can watch 1? Next to watch is 1. We can watch 1 (1-2) and simultaneously download something. What to download? We have space 5. We could download 3 (size 3). Start download 3 (1-4). Space=2. Watch 1 (1-2).
            // t=2: watch 1 done. Space freed 1 -> space=3. Next to watch: 2. Is 2 downloaded? No. We are downloading 3. Can we download 2? Space=3 >=2, but we can't start a new download while 3 is downloading. So we must wait until t=4.
            // t=4: download 3 done. Downloaded: [3]. Space used: 3, free=3. Next to watch: 2 (not downloaded). Next to download: we need 2 and 4,5. We can download 2 (size 2). Start download 2 (4-6). Space=1. Can we watch something? Next to watch is 2, not ready. We have 3 downloaded, but we can't watch it because we must watch 2 first. So idle.
            // t=6: download 2 done. Downloaded: [3,2]. Next to watch: 2. Watch 2 (6-7). Space freed 2 -> space=3. Next to watch: 3. Watch 3 (7-8). Space freed 3 -> space=6.
