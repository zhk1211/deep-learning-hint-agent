// Hint0
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

    long long total_download = accumulate(a.begin(), a.end(), 0LL);
    long long total_watch = n; // 1 minute per video

    // We need to find the minimum possible makespan.
    // The process: we download videos one by one, and we can watch a video
    // while downloading the next one. The disk space constraint may force
    // us to wait before starting a download if not enough space.
    // We can delete a video immediately after watching it (which takes 1 minute).
    // The order of watching doesn't matter, but we can choose the download order.
    // Since we can delete after watching, the disk usage at any time is the sum
    // of sizes of videos that have been downloaded but not yet watched.
    // We can think of it as: we have a buffer of size m. We add videos to buffer
    // when we start download, and remove them after 1 minute of watching.
    // The download of a video of size s takes s minutes, during which it occupies s space.
    // After download finishes, we can immediately start watching it (takes 1 minute),
    // and during that 1 minute we can also download another video.
    // The key is to avoid waiting due to insufficient space.
    // A known greedy strategy: sort videos by size descending? Or ascending?
    // Let's think: we want to minimize the time when the disk is full and we can't start
    // a new download. If we download large videos first, they occupy space for a long time
    // (download time + 1 minute watch). But if we download small videos first, we can quickly
    // watch and delete them, freeing space for larger ones.
    // Actually, the optimal strategy is to process videos in some order to minimize idle time.
    // This is similar to scheduling with a buffer constraint.
    // We can simulate the process with a priority queue or just compute the minimum time.
    // Another perspective: The total time is at least total_download + total_watch.
    // But we can overlap watching and downloading. The only extra time is when we must wait
    // for space to free up before starting a download.
    // Let's try to find a lower bound and then construct a schedule achieving it.
    // Consider the sum of sizes of any subset of videos that we might have in the buffer.
    // At any time, the sum of sizes of videos currently downloaded but not yet watched cannot exceed m.
    // Also, we can only watch a video after it's fully downloaded.
    // This is reminiscent of the problem "Video Watching" or similar.
    // I recall a known solution: sort videos by size. Then we can compute the minimum time
    // by considering that we can always start a download if there is space, and we can delete
    // a video after watching it. The optimal order is to download videos in non-decreasing order of size?
    // Let's test with examples.
    // Example 1: n=5, m=6, a=[1,2,3,4,5]. Sorted: 1,2,3,4,5.
    // If we download in this order:
    // t=0: start download 1 (size 1), disk used 1.
    // t=1: download 1 finished. Start watching 1 (takes 1 min). During this minute, we can start download 2 (size 2). Disk used: 1 (video 1 still there) + 2 = 3 <=6.
    // t=2: watch 1 finished, delete 1. Disk used: 2. Download 2 finished? Download 2 started at t=1, size 2, so finishes at t=3. At t=2, we can start watching 2? No, download not finished. We can start download 3? Disk used 2, need 3, total 5 <=6, so start download 3 at t=2.
    // t=3: download 2 finishes. Start watching 2 (takes 1 min). Disk used: 2 (video 2) + 3 (video 3 downloading) = 5. Download 3 continues until t=5.
    // t=4: watch 2 finished, delete 2. Disk used: 3. Can we start download 4? Need 4, total 7 >6, so cannot. Must wait.
    // t=5: download 3 finishes. Start watching 3. Disk used: 3 (video 3) + 0? We can start download 4 now? Disk used 3, need 4, total 7 >6, still cannot. Wait until watch 3 finishes at t=6.
    // t=6: watch 3 finished, delete 3. Disk used: 0. Start download 4 (size 4). Disk used 4.
    // t=10: download 4 finishes. Start watching 4. Disk used 4. Start download 5? Need 5, total 9 >6, cannot.
    // t=11: watch 4 finished, delete 4. Disk used 0. Start download 5.
    // t=16: download 5 finishes. Start watching 5.
    // t=17: watch 5 finished. Total time 17? But example output is 16. So my simulation gave 17. Maybe I made a mistake or the order is different.
    // Let's try a different order: maybe download largest first? 5,4,3,2,1.
    // t=0: start 5 (size 5), disk 5.
    // t=5: finish 5, watch 5 (1 min). Disk 5. Can we start 4? Need 4, total 9 >6, no.
    // t=6: watch 5 done, delete 5. Disk 0. Start 4 (size 4).
    // t=10: finish 4, watch 4. Disk 4. Start 3? Need 3, total 7 >6, no.
    // t=11: watch 4 done, delete 4. Disk 0. Start 3.
    // t=14: finish 3, watch 3. Disk 3. Start 2? Need 2, total 5 <=6, yes. Start 2 at t=14.
    // t=15: watch 3 done, delete 3. Disk 2. Download 2 continues until t=16.
    // t=16: finish 2, watch 2. Disk 2. Start 1? Need 1, total 3 <=6, yes. Start 1 at t=16.
    // t=17: watch 2 done, delete 2. Disk 1. Download 1 continues until t=17.
    // t=17: finish 1, watch 1.
    // t=18: done. Total 18. Worse.
    // Maybe a different order: 3,4,5,1,2? Let's try to find the optimal.
    // The known solution for this problem (I think it's from Codeforces Round) is to sort videos by size, and then the answer is max(total_download + total_watch, something like max over i of (a_i + something)? Actually, I recall a problem "Watching Videos" or similar. Let's think differently.
    // We can model the process as: we have a sequence of downloads and watches. The total time is the sum of download times plus the sum of watch times, minus the overlap. The overlap is the time when we are downloading and watching simultaneously. The maximum overlap is limited by the disk space: we can only have videos in the buffer that fit in m. Also, we can only watch a video after it's downloaded.
    // Consider the schedule as a sequence of actions. We can think of it as: we need to order the videos. For each video, it has a download phase of length a_i, then a watch phase of length 1. During the watch phase, we can download the next video. The disk space constraint: at any time, the sum of sizes of videos that have started download but not yet finished watching must be <= m.
    // This is similar to a single-machine scheduling with buffer constraints.
    // Let's denote the start time of download of video i as s_i. Then download finishes at s_i + a_i. Watch starts immediately after download? Actually, we can start watching as soon as download finishes, but we might delay watching? The problem says: "Once a video is fully downloaded to the hard disk, Monocarp can watch it." It doesn't say he must watch it immediately. He can watch it later. But watching takes 1 minute and doesn't occupy internet, so he can download another while watching. To minimize total time, we should watch as soon as possible to free space. So we can assume that we start watching a video immediately after its download finishes, unless we are already watching something? But we can only watch one video at a time? The problem says: "Watching each video takes exactly 1 minute and does not occupy the Internet connection, so Monocarp can start downloading another video while watching the current one." It doesn't explicitly say we can watch multiple videos simultaneously. Typically, watching is a single activity, so we assume only one video can be watched at a time. So we have a single watcher. So we need to sequence the watches as well.
    // So we have two resources: downloader (always busy if there is a download) and watcher (busy for 1 minute per video). The downloader can work in parallel with the watcher. The disk space is a shared resource.
    // This is a classic problem: minimize makespan with buffer constraints. I think the optimal strategy is to sort videos by size in increasing order? Or maybe we can always achieve a time equal to max( total_download + 1, max_i (a_i + something) )? Let's test with examples.
    // Example 1: total_download = 15, total_watch = 5, sum = 20. But output is 16. So there is 4 minutes of overlap. Overlap happens when we watch and download simultaneously. The maximum possible overlap is total_watch = 5? But we can't overlap all because of disk space and sequencing.
    // Let's try to find a formula. I remember a similar problem: "Video Streaming" or "Downloading Videos". The answer is max( total_download + 1, max_{i} (a_i + i) )? Not sure.
    // Let's simulate with a greedy algorithm that always downloads the smallest available video that fits in the remaining disk space, and watches as soon as possible. Since we can choose the order, we can pre-sort.
    // Consider we have a queue of videos to download. We maintain the current disk usage. At any time, we can start a download if there is space. We also have a list of videos that are downloaded and waiting to be watched (or we watch immediately). Since watching takes 1 minute, we can interleave.
    // Actually, we can think of the process as: we have a buffer of size m. We add videos to the buffer when we start download. They stay in the buffer during download and during the 1-minute watch. After watch, they are removed. The download time is a_i, the watch time is 1. So each video occupies space for a_i + 1 time from the start of its download? Not exactly: it occupies space from start of download until the end of its watch. If we start watching immediately after download, the total time it occupies space is a_i + 1. But if we delay the watch, it occupies longer. To minimize makespan, we should not delay watching.
    // So we can assume that for each video, it occupies space for a_i + 1 consecutive minutes starting from the start of its download. However, the watch part (1 minute) can overlap with the download of the next video. So the next video's download can start during that 1 minute, meaning the space of the current video is still occupied during that 1 minute. So the next video's download starts while the current video is still in the buffer (being watched). So the buffer must have space for both the current video (size a_i) and the next video (size a_{i+1}) during that 1 minute. After that 1 minute, the current video is deleted.
    // So the constraint is: when we start downloading video i+1, the sum of sizes of all videos that have been downloaded but not yet deleted must be <= m. At the moment we start downloading video i+1, the videos that are still in the buffer are: the video currently being watched (if any), plus any videos that have finished downloading but are waiting to be watched? But we can schedule watches immediately, so there should be at most one video being watched at a time, and no waiting videos. So at the start of download of video i+1, the buffer contains: the video currently being watched (if its watch hasn't finished) and possibly the video that just finished downloading and is about to be watched? Actually, we can start download of the next video exactly at the same time we start watching the current one. So at that moment, the buffer has the current video (which we just started watching) and we add the next video. So the sum of sizes is a_current + a_next <= m. But wait, what if the current video's download finished, we start watching it, and we also want to start downloading the next video. At that instant, the current video is still in the buffer (it will be deleted after 1 minute). So we need a_current + a_next <= m. But is that always necessary? What if we don't start the next download exactly at that moment? We could wait until the current watch finishes, then start the next download. That would avoid the overlap, but then we lose the 1-minute overlap. So to maximize overlap, we want to start the next download during the watch of the current video. So we need the sum of the two sizes to be <= m. If not, we have to wait until the current watch finishes, then start the next download. In that case, there is no overlap, and the next download starts after the current video is deleted. So the condition for overlapping the next download with the current watch is a_current + a_next <= m.
    // This suggests a greedy pairing: we can pair videos such that we download one while watching the other. The total time would be: sum a_i + n - (number of successful overlaps). Each successful overlap saves 1 minute (since we do a download and watch in parallel). But we can only overlap if the two videos' sizes sum to <= m. Also, we can chain overlaps? For example, download video 1, then during its watch, download video 2; during video 2's watch, download video 3, etc. But for that to happen, we need that at the start of video 2's download, video 1 is being watched, so a1 + a2 <= m. Then when video 2 finishes, we start watching it and start downloading video 3. At that moment, video 2 is being watched, and video 1? Video 1's watch finished at the end of the 1-minute period. So when we start video 3's download, video 1 is already deleted. So the buffer contains only video 2 (being watched) and video 3 (just started). So we need a2 + a3 <= m. So we can chain overlaps as long as each consecutive pair satisfies a_i + a_{i+1} <= m. If at some point a_i + a_{i+1} > m, we cannot start the next download during the watch of i; we must wait until i's watch finishes, then start i+1. That means we lose the overlap for that transition. But we could potentially reorder videos to maximize the number of consecutive pairs that satisfy the sum condition.
    // So the problem reduces to: order the videos to maximize the number of adjacent pairs (i, i+1) such that a_i + a_{i+1} <= m, because each such pair allows a 1-minute overlap. However, we also have the initial video: the first video's download cannot overlap with any watch (since no previous video). The last video's watch cannot overlap with a next download (since no next video). So if we have a sequence of videos, the total time is: sum a_i + n - (number of indices i from 1 to n-1 such that we overlap the download of i+1 with the watch of i). But is it always possible to achieve that if we order them to satisfy the sum condition? We also need to consider that the download of i+1 might finish before the watch of i finishes? No, download takes a_{i+1} minutes, which could be >1. If a_{i+1} > 1, then the download of i+1 will continue after the watch of i finishes. That's fine; the overlap is only during that 1 minute. After that, video i is deleted, and the download of i+1 continues alone. So the condition is only about the moment we start the download: we need space for both videos at that moment. After 1 minute, video i is gone, so space is freed. So the condition is exactly a_i + a_{i+1} <= m.
    // But wait: what if a_{i+1} is very large, say > m - a_i? Then we can't start it. But if we wait until video i is deleted, we can start it. So we lose the overlap.
    // So the problem is: find an ordering of the videos that maximizes the number of adjacent pairs with sum <= m. Then the answer is total_download + n - (max number of such adjacent pairs). But is that always achievable? We also need to ensure that the first video can be downloaded (it always can, since a_i <= m). And we need to ensure that the schedule is feasible in terms of download times: we can't start a download before the previous download finishes? Actually, the downloader is single: we can only download one video at a time. So the downloads must be sequential. In our overlap scheme, we start download of i+1 at the same time we start watching i. That means the download of i must have finished before we start watching i. So the download of i and i+1 are sequential: download i, then download i+1 starts exactly when download i finishes and watch i starts. So there is no overlap between downloads. That's consistent with the single downloader.
    // So the schedule is: download video 1 (takes a1). Then simultaneously watch video 1 (1 min) and download video 2 (takes a2). After 1 minute, video 1 is deleted. Then download video 2 continues for a2 - 1 more minutes (if a2 > 1). Then when download 2 finishes, start watching video 2 and download video 3, and so on. This is exactly the process I simulated earlier. In that simulation, I got 17 for example 1 with order 1,2,3,4,5. But the optimal is 16. So maybe we can do better by reordering.
    // Let's compute the number of overlaps in my simulation: I overlapped 1&2, 2&3? Let's check: I started download 2 during watch 1 (a1=1, a2=2, sum=3<=6, so overlap). Then I started download 3 during watch 2? Watch 2 started at t=3. At t=3, I started download 3? In my simulation, I started download 3 at t=2, during watch 1? Wait, I need to re-simulate carefully.
    // Let's define the schedule precisely:
    // We have a sequence of videos v1, v2, ..., vn.
    // For i=1 to n:
    //   Download v_i. It starts at some time S_i, finishes at S_i + a_{v_i}.
    //   Watch v_i. It starts at some time W_i, finishes at W_i + 1.
    // Constraints:
    // - S_i + a_{v_i} <= W_i (can watch only after download)
    // - S_{i+1} >= S_i + a_{v_i}? No, downloads cannot overlap: S_{i+1} >= S_i + a_{v_i}? Actually, the downloader is single, so we cannot start a new download until the previous one finishes. So S_{i+1} >= S_i + a_{v_i}. But we can start a download while watching? The problem says: "Once the download is started, it cannot be interrupted. It is not allowed to run two or more downloads in parallel." So downloads are sequential. So S_{i+1} >= S_i + a_{v_i}.
    // - Watching: we can only watch one at a time? The problem doesn't explicitly say we can't watch multiple, but typically watching is a single activity. "Watching each video takes exactly 1 minute and does not occupy the Internet connection" implies we can do other things while watching, but it doesn't say we can watch multiple simultaneously. Usually, you watch one at a time. So I'll assume watching is sequential: W_{i+1} >= W_i + 1.
    // - Disk space: at any time t, the sum of sizes of videos that have started download (S_i <= t) and not yet deleted (deletion happens at W_i + 1) must be <= m.
    // We want to minimize max(W_n + 1, S_n + a_n + 1?) Actually, the total time is when the last watch finishes.
    // In our desired schedule, we want to overlap watch of i with download of i+1. That means S_{i+1} = W_i (or S_{i+1} could be later, but to maximize overlap we set S_{i+1} = W_i). Also, to minimize time, we set W_i = S_i + a_i (watch immediately after download). Then S_{i+1} = S_i + a_i. Then the download of i+1 starts exactly when download of i finishes. So downloads are back-to-back. Then the watch of i+1 starts at S_{i+1} + a_{i+1} = S_i + a_i + a_{i+1}. But wait, if we set W_i = S_i + a_i, then watch i finishes at S_i + a_i + 1. The next watch W_{i+1} cannot start until W_i + 1 if we assume sequential watching? Actually, if we start watching i+1 immediately after its download, we might have W_{i+1} = S_{i+1} + a_{i+1}. But if S_{i+1} + a_{i+1} < W_i + 1, then we would be watching two videos at once? That would violate sequential watching. So we must have W_{i+1} >= W_i + 1. So if a_{i+1} is very small, we might finish downloading i+1 before the previous watch finishes. In that case, we cannot start watching i+1 until the previous watch finishes. So we might have idle time on the watcher. But we can still start the download of i+2 during the watch of i+1? This complicates things.
    // Let's re-evaluate: The watcher is a separate resource. It takes 1 minute per video. The downloader takes a_i minutes per video. They can work in parallel. The disk space constraint is the only coupling beyond the sequential nature of each resource.
    // This is exactly a two-machine flow shop problem with a buffer of size m? Not exactly.
    // Let's think of the videos as jobs. Each job has two operations: download (length a_i) and watch (length 1). The watch operation must follow the download operation. The download machine can process one job at a time. The watch machine can process one job at a time. The buffer between the machines has capacity m in terms of total size of jobs that have completed download but not yet started watch. The objective is makespan.
    // This is a classic problem: two-machine flow shop with a buffer of limited capacity, where jobs have sizes. The buffer capacity is in terms of sum of sizes, not number of jobs. This is known to be NP-hard in general? But here the second operation time is 1 for all jobs. That might make it tractable.
    // Since watch time is 1, the watch machine is just a unit-time resource. The download times vary.
    // I recall a Codeforces problem: "Watching Videos" or similar. The solution often involves sorting by size and then using a greedy approach or binary search on the answer.
    // Let's search my memory: There is a problem "Video Watching" or "Downloading Videos" where you have to watch n videos, each takes 1 minute to watch, download takes a_i minutes, disk size m. The answer is something like: sort a_i, then the minimum time is max( sum a_i + 1, max_{i} (a_i + i) )? Let's test with examples.
    // Example 1: a=[1,2,3,4,5] sorted. sum a_i + 1 = 16. max(a_i + i) for i=1..5 (1-indexed): 1+1=2, 2+2=4, 3+3=6, 4+4=8, 5+5=10. Max is 10. So max(16,10)=16. Output is 16. Matches!
    // Example 2: n=5, m=5, a=[1,2,3,4,5]. sum a_i + 1 = 16. max(a_i + i): 1+1=2, 2+2=4, 3+3=6, 4+4=8, 5+5=10. Max=10. So max(16,10)=16. But output is 17. So that formula fails for example 2.
    // Example 2 output is 17. So m=5 changes something. In example 1, m=6. So the disk size matters.
    // Let's test another formula: maybe the answer is max( sum a_i + 1, max_{i} (a_i + i) ) but with some adjustment for m? Or maybe we need to consider the sum of the largest videos?
    // Let's simulate example 2 with a greedy strategy to see if we can achieve 16 or 17.
    // m=5, a=[1,2,3,4,5]. Sorted: 1,2,3,4,5.
    // Try order 1,2,3,4,5:
    // t=0: start download 1 (size 1), disk 1.
    // t=1: finish 1, watch 1 (until t=2). Start download 2? Need 2, disk currently 1 (video 1) + 2 = 3 <=5, so start download 2 at t=1.
    // t=2: watch 1 done, delete 1. Disk: 2 (video 2 downloading). Download 2 continues until t=3.
    // t=3: finish 2, watch 2 (until t=4). Can we start download 3? Disk: video 2 (size 2) + 3 = 5 <=5, so start download 3 at t=3.
    // t=4: watch 2 done, delete 2. Disk: 3 (video 3 downloading). Download 3 continues until t=6.
    // t=5: nothing? Download 3 still going.
    // t=6: finish 3, watch 3 (until t=7). Can we start download 4? Disk: video 3 (size 3) + 4 = 7 >5. Cannot. So wait.
    // t=7: watch 3 done, delete 3. Disk: 0. Start download 4 at t=7.
    // t=11: finish 4, watch 4 (until t=12). Start download 5? Disk: video 4 (4) + 5 = 9 >5. Cannot.
    // t=12: watch 4 done, delete 4. Disk: 0. Start download 5 at t=12.
    // t=17: finish 5, watch 5 (until t=18). Total time 18? Wait, my earlier simulation for example 1 gave 17 with this order, but here I got 18. Let's recalc example 1 with m=6: I got 17 earlier, but the output is 16. So my simulation might be suboptimal because I assumed we must watch immediately after download. But maybe we can delay watching to free space earlier? No, watching frees space only after it finishes. Delaying watching would keep the video in the buffer longer, which is worse. So we should watch as soon as possible. But maybe we can start a download before the previous watch finishes if there is space? In my simulation, I started download 3 at t=3, during watch 2. That's fine. But at t=6, I couldn't start download 4 because video 3 was being watched and occupied 3, plus 4 would be 7>5. What if I delayed watching video 3? If I don't watch video 3 immediately, it stays in the buffer, occupying 3. That doesn't help. What if I change the order?
    // Try order: 1,3,2,4,5? Or maybe we can interleave differently.
    // Let's try to find a schedule for example 2 that finishes in 17. The output is 17, so it's possible.
    // Maybe we can do: download 5 first? Let's try: 5,1,2,3,4? But 5 takes 5 minutes, during which we can't download anything else. Then watch 5, and during that 1 minute, we can download 1? But 5+1=6>5, so we can't start 1 during watch 5. So we must wait until watch 5 finishes. That loses overlap.
    // Try: 1,5,2,3,4? 
    // t=0: download 1 (1 min).
    // t=1: watch 1, start download 5? 1+5=6>5, cannot. So wait until t=2.
    // t=2: watch 1 done, delete 1. Start download 5.
    // t=7: finish 5, watch 5. Start download 2? 5+2=7>5, cannot.
    // t=8: watch 5 done, delete 5. Start download 2.
    // t=10: finish 2, watch 2, start download 3? 2+3=5<=5, yes.
    // t=11: watch 2 done, delete 2. Download 3 continues until t=13.
    // t=13: finish 3, watch 3, start download 4? 3+4=7>5, no.
    // t=14: watch 3 done, delete 3. Start download 4.
    // t=18: finish 4, watch 4. Total 19.
    // Try: 2,3,1,4,5?
    // t=0: download 2 (2 min).
    // t=2: watch 2, start download 3? 2+3=5<=5, yes.
    // t=3: watch 2 done, delete 2. Download 3 continues until t=5.
    // t=5: finish 3, watch 3, start download 1? 3+1=4<=5, yes.
    // t=6: watch 3 done, delete 3. Download 1 continues? Download 1 started at t=5, size 1, so finishes at t=6. So at t=6, download 1 finishes. Then watch 1? But we just finished watch 3 at t=6? Actually, watch 3 started at t=5, so it finishes at t=6. So at t=6, we can start watch 1? But we need to watch sequentially. So we can start watch 1 at t=6. During watch 1 (t=6 to 7), can we start download 4? Disk: video 1 (size 1) + 4 = 5 <=5, yes! So start download 4 at t=6.
    // t=7: watch 1 done, delete 1. Download 4 continues until t=10.
    // t=10: finish 4, watch 4, start download 5? 4+5=9>5, no.
    // t=11: watch 4 done, delete 4. Start download 5.
    // t=16: finish 5, watch 5. Total 17. Yes! That gives 17.
    // So the order 2,3,1,4,5 achieves 17. The number of overlaps: we overlapped download 3 with watch 2, download 1 with watch 3, download 4 with watch 1. That's 3 overlaps. Total time = sum a (15) + n (5) - overlaps (3) = 17. So we saved 3 minutes.
    // In this schedule, the pairs that overlapped: (2,3) sum=5<=5, (3,1) sum=4<=5, (1,4) sum=5<=5. The pair (4,5) sum=9>5, so no overlap. So we had 3 consecutive pairs satisfying the sum condition. The sequence length is 5, so max possible overlaps is 4 (if all adjacent pairs satisfy). But we got 3. Can we get 4? That would require all adjacent pairs sum <=5. Is there a permutation of 1,2,3,4,5 such that every adjacent pair sums to <=5? The pairs would be (x,y) with x+y<=5. The only possible pairs from the set: (1,2)=3, (1,3)=4, (1,4)=5, (2,3)=5. We need a Hamiltonian path in a graph where edges are these pairs. The graph: 1 connected to 2,3,4; 2 connected to 1,3; 3 connected to 1,2; 4 connected to 1; 5 isolated. So we cannot include 5 in any pair with sum<=5 except maybe with nothing? 5+0=5, but we don't have 0. So 5 cannot be adjacent to any other number with sum<=5. So any sequence must have 5 at an end, and its only neighbor would have sum>5. So at most 3 overlaps. So 17 is optimal.
    // Example 1: m=6, a=[1,2,3,4,5]. Pairs with sum<=6: (1,2)=3, (1,3)=4, (1,4)=5, (1,5)=6, (2,3)=5, (2,4)=6, (3,? 3+4=7>6, so no). So edges: 1-2,1-3,1-4,1-5,2-3,2-4. We need a Hamiltonian path. Can we include all? Sequence: 3,2,4,1,5? Check sums: 3+2=5<=6, 2+4=6<=6, 4+1=5<=6, 1+5=6<=6. All adjacent sums <=6! So we can have 4 overlaps. Total time = 15+5-4=16. That matches example 1 output.
    // Example 3: n=4, m=3, a=[1,3,2,3]. Sorted: 1,2,3,3. Total download=9, watch=4, sum=13. Pairs with sum<=3: (1,2)=3, (1,? 1+3=4>3), (2,? 2+3=5>3), (3,3=6>3). So only edge is 1-2. We can have at most 1 overlap? But output is 12. 13-1=12. So 1 overlap. Can we achieve that? Sequence: 1,2,3,3? Check: 1+2=3<=3 overlap; 2+3=5>3 no overlap; 3+3=6>3 no overlap. Total overlaps=1. Time=9+4-1=12. Yes.
    // So the problem reduces to: find a permutation of the videos that maximizes the number of adjacent pairs (i, i+1) such that a_i + a_{i
