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
    
    // We need to find the minimum time T such that we can schedule all downloads and watches.
    // The key insight: we can watch at most one video at a time, but we can download while watching.
    // The bottleneck is the disk space: we need to ensure that at any time, the sum of sizes of videos
    // that are downloaded but not yet watched does not exceed m.
    // Since we can delete immediately after watching, the maximum number of videos stored simultaneously
    // is the number of videos we can have downloaded but not yet watched.
    // To minimize total time, we want to maximize overlap between download and watch.
    // The optimal strategy: download videos in some order, and watch them as soon as possible.
    // The total time is at least total_download + 1 (for the last watch) if we can perfectly overlap,
    // but disk space constraints may force us to wait.
    // We can think of it as: we need to schedule n downloads of lengths a_i and n watches of length 1.
    // Watches must happen after corresponding downloads finish.
    // Disk space: at any time, sum of sizes of completely downloaded but not yet watched videos <= m.
    // Since we can delete instantly after watching, the constraint is on the set of videos that are
    // downloaded and waiting to be watched.
    // If we process videos in some order, we can start a download only if current used space + a_i <= m.
    // We can free space by watching a video (takes 1 minute) which removes its size from disk.
    // This is similar to a scheduling problem with a buffer of size m.
    // The minimum time can be found by considering the maximum prefix sum of download times minus watch times?
    // Let's think differently: we have total download time D = sum a_i, total watch time W = n.
    // Without space constraints, the minimum time would be max(D, W) + something? Actually, we can start
    // watching only after at least one download finishes. The last action is a watch. So time >= D + 1.
    // But we can watch while downloading, so total time = D + 1 if we can always keep the pipe full.
    // However, disk space limits how many videos we can have in the buffer.
    // The buffer size is m. We need to ensure that at any point, the sum of sizes of videos that have been
    // downloaded but not yet watched is <= m.
    // This is equivalent to: we can't download a video if it would exceed m, unless we free space by watching.
    // Watching takes 1 minute, during which we can also download.
    // This is a classic problem: we have items with processing times a_i on one machine, and we need to
    // move them to another machine (watching) that takes 1 unit each, with a buffer of capacity m.
    // The minimum makespan is known to be: D + max(0, max_{k} (sum_{i=1}^k a_i - (k-1)*1) - m)? Not exactly.
    // Let's derive: Suppose we order videos by some sequence. Let the download start times be s_i,
    // finish times f_i = s_i + a_i. Watch start times w_i >= f_i, watch takes 1 unit, so watch end = w_i + 1.
    // Disk usage at time t: sum of sizes of videos with f_i <= t < w_i + 1.
    // Constraint: this sum <= m for all t.
    // We want to minimize max(w_i + 1).
    // Since we can always watch as soon as possible, we can set w_i = max(f_i, time when previous watch finished).
    // But we also need to respect disk space when starting a download.
    // This is similar to a flow shop with limited buffer.
    // There is a known result: the optimal order is to sort by a_i descending? Or ascending?
    // Let's test with examples.
    // Example 1: n=5, m=6, a = [1,2,3,4,5]. Sorted: [1,2,3,4,5]. Total D=15. Output=16.
    // If we do ascending: download 1 (finish 1), watch 1 (end 2). Disk used: 0 after watch.
    // download 2 (finish 3), watch 2 (end 4). Disk used: 0.
    // download 3 (finish 6), watch 3 (end 7). Disk used: 0.
    // download 4 (finish 10), watch 4 (end 11). Disk used: 0.
    // download 5 (finish 15), watch 5 (end 16). Total 16. Works.
    // Example 2: n=5, m=5, a = [1,2,3,4,5]. Output=17.
    // Ascending: download 1 (1), watch 1 (2). download 2 (3), watch 2 (4). download 3 (6), watch 3 (7).
    // download 4: at time 7, disk free = 5, a_4=4, so start at 7, finish 11. watch 4 at 11, end 12.
    // download 5: at time 12, disk free=5, a_5=5, start 12, finish 17. watch 5 at 17, end 18? That gives 18, but output is 17.
    // So ascending doesn't give 17. Let's try descending: [5,4,3,2,1].
    // download 5 (5), watch 5 (6). download 4: at 6, disk free=5, a_4=4, start 6, finish 10. watch 4 at 10, end 11.
    // download 3: at 11, disk free=5, start 11, finish 14. watch 3 at 14, end 15.
    // download 2: at 15, start 15, finish 17. watch 2 at 17, end 18. Still 18.
    // How to get 17? Maybe order: 2,3,4,5,1? Let's simulate carefully.
    // We need to find a schedule that finishes at 17.
    // Try: download 2 (2), watch 2 (3). download 3 (5), watch 3 (6). download 4 (9), watch 4 (10).
    // download 5: at 10, disk free=5, start 10, finish 15. watch 5 at 15, end 16.
    // download 1: at 16, start 16, finish 17. watch 1 at 17, end 18. Still 18.
    // Maybe we can overlap differently: start download 5 earlier while watching something else?
    // At time 0: start download 5 (size 5). Disk used: 5. Finish at 5.
    // At time 5: we can watch 5 (takes 1 min, until 6). During this watch, we can start another download.
    // At time 5: start download 4 (size 4). Disk used: 5 (video 5 not deleted yet) + 4 = 9 > 5, so cannot!
    // So we must wait until video 5 is deleted. Video 5 is deleted at time 6 (after watch).
    // So at time 6: start download 4, finish 10. Watch 4 at 10, end 11.
    // At time 11: start download 3, finish 14. Watch 3 at 14, end 15.
    // At time 15: start download 2, finish 17. Watch 2 at 17, end 18.
    // At time 18: start download 1, finish 19. Watch 1 at 19, end 20. That's worse.
    // So descending gives 20.
    // How to get 17? Let's think: total download = 15, total watch = 5. If we could perfectly pipeline, time = 15 + 1 = 16.
    // But m=5 restricts. The maximum sum of sizes in buffer is 5. The videos are 1,2,3,4,5.
    // We need to ensure that at any time, the sum of downloaded but unwatched videos <= 5.
    // This is like: we have a buffer of size 5. We can put items into buffer (download) and remove items (watch) after they are in buffer.
    // The download time is the size, watch time is 1.
    // We want to minimize makespan.
    // This is equivalent to: we have a sequence of downloads and watches. The buffer constraint: at any point, the number of items in buffer (weighted by size) <= m.
    // We can think of it as: we need to process all downloads, and we can do watches in between.
    // The total time is sum of all download times + sum of all watch times - overlap.
    // Overlap happens when we watch while downloading. But we can only watch a video after it's downloaded.
    // The maximum overlap is limited by the buffer: we can have at most m megabytes of videos waiting to be watched.
    // So the number of videos we can have in the buffer simultaneously is limited.
    // Actually, we can watch at most one video at a time, but we can download while watching.
    // The key is: we want to keep the download pipe busy as much as possible.
    // The only reason to wait is when we want to start a download but the buffer is full (sum of sizes of downloaded but unwatched videos = m) and no watch is currently happening to free space.
    // But we can always schedule a watch to free space exactly when needed, if we have a video ready to watch.
    // So the problem reduces to: we need to ensure that whenever we need to free space, there is a video ready to watch.
    // That means we should not have all videos in the buffer too large that we can't free space quickly enough.
    // This is similar to the concept of "maximum prefix sum" of (a_i - 1) because each video occupies a_i space for at least 1 minute (the watch time) after it's downloaded? Actually, a video occupies space from the moment it finishes downloading until the moment it finishes watching. The minimum time it occupies space is 1 minute (the watch time). If we start watching immediately after download, it occupies space for exactly 1 minute. During that minute, we can download other things.
    // So each video of size a_i requires a_i time to download, and then 1 minute to watch, during which it still uses a_i space.
    // So the total time the video uses disk space is a_i (download) + 1 (watch) = a_i + 1? No, during download it's not on disk yet; it's only reserved at start of download. Actually, the problem says: "Once Monocarp starts the download of a video of size s, the s megabytes are immediately reserved on a hard disk." So the space is occupied from the start of download until the video is deleted (after watching). So the total time a video occupies space is its download time + the time it waits before watching + 1 minute watch time.
    // To minimize total time, we want to start watching as soon as possible after download finishes.
    // So the minimum occupancy for video i is a_i (download) + 1 (watch) = a_i + 1, but during the a_i download time, it's occupying space, and during the 1 watch time, it's also occupying space. So total time it ties up space is at least a_i + 1.
    // However, we can overlap downloads and watches. The buffer constraint is that the sum of sizes of videos that have started downloading but not yet finished watching must be <= m at all times.
    // This is exactly the condition for a schedule to be feasible.
    // We want to minimize the completion time of the last watch.
    // This is a known problem: scheduling with limited buffer. The optimal order is to sort videos by size? Let's analyze.
    // Consider the total time T. We have n videos. The total download time is D = sum a_i. The total watch time is n. The last action is a watch. So T >= D + 1? Not necessarily, because we might have idle time on download if buffer is full. But if we can always keep downloading, T = D + 1. If we have to wait, T > D + 1.
    // When do we have to wait? When we want to start a download but the buffer is full and no watch is currently happening. But we can always schedule a watch to happen exactly when we need space, provided there is a video ready to watch. So the only reason we can't free space is if no video is ready to watch at that moment. That means all videos in the buffer are still downloading. So if we have a set of videos downloading simultaneously? No, downloads are sequential. So at any time, there is at most one video downloading. The buffer contains videos that have finished downloading but not yet watched, plus the one currently downloading (since its space is reserved). So the buffer occupancy = size of current download (if any) + sum of sizes of completely downloaded but unwatched videos.
    // We can watch at most one video at a time. Watching takes 1 minute. So we can remove one video from the buffer every 1 minute (if there is one ready). 
    // This is like a producer-consumer with a buffer of size m. The producer produces items of size a_i, taking a_i time to produce. The consumer consumes one item at a time, taking 1 minute per item, but can only consume items that have been fully produced. The buffer holds produced but not yet consumed items. The producer can start a new item only if there is space in the buffer for its size.
    // We want to minimize makespan (time when all items are consumed).
    // This is a classic problem. The optimal strategy is to sort items by size? Actually, to minimize makespan, we should process items in decreasing order of size? Or increasing? Let's test with examples.
    // Example 2: sizes 1,2,3,4,5, m=5. Output 17.
    // Let's try order: 3,2,4,1,5? We need a systematic way.
    // I recall a known result: For such problems, the minimum makespan is max( D + 1, D + n - something? ) No.
    // Let's think about the maximum number of videos that can be in the buffer at once. Since each video takes at least 1 minute to watch, and we can watch only one at a time, the buffer can hold at most m megabytes. The sum of sizes of videos in the buffer can be at most m. But we can have multiple small videos or few large ones.
    // The total time is at least the sum of download times plus the time needed to watch all videos, minus the overlap. The maximum overlap is when we are always watching while downloading. But we can only watch if there is a video ready. So we need to ensure that the buffer never goes empty when there are still videos to download? Actually, we want to avoid idle download time.
    // Idle download time happens when we want to download the next video but the buffer is full. At that moment, we must wait for a watch to complete to free space. If a watch is already in progress, we just wait for it to finish. If no watch is in progress, we must wait until some download finishes? But if buffer is full, it means there are videos in the buffer (finished downloading) that we could watch. So we can immediately start watching one of them. So we don't have to wait for a download to finish; we can just start watching a ready video. So idle time only occurs if we have to wait for a watch to finish, but we can always start a watch if there is a ready video. So the only idle time is when we are forced to wait for the current watch to finish before we can start the next download, because the buffer is full and we can't start the download until space is freed. That space is freed exactly when the current watch finishes (and we delete that video). So if we schedule watches back-to-back, we free one video's space every 1 minute. So the download can proceed if the next video's size is <= the space that will be freed by the time we finish the current watch? Actually, we can start the download as soon as the space is freed, which is at the end of a watch. So if we have a continuous stream of watches, we free space every 1 minute. The download of the next video can start immediately after a watch finishes, if its size <= the freed space? But the freed space is exactly the size of the video we just watched. So if we watch a video of size s, we free s space. Then we can start a download of size up to m - (current buffer usage excluding that video). But if we keep the buffer full, we might need to free exactly the right amount.
    // This suggests that the order of watching matters. We can choose which video to watch next to free the right amount of space for the next download.
    // This is similar to the problem of scheduling with limited buffer where we can reorder the consumption. The optimal makespan is known to be: D + max(0, max_{k} (sum_{i=1}^k a_i - (k-1) - m)? Let's test.
    // For example 2: D=15. Sorted ascending: a=[1,2,3,4,5]. Compute prefix sums: 1,3,6,10,15. For each k, compute sum_{i=1}^k a_i - (k-1) - m? (k-1) because we can have k-1 watches completed before the k-th download finishes? Not exactly.
    // Let's derive a formula. Consider any schedule. Let the download start times be s_1 <= s_2 <= ... <= s_n. Let f_i = s_i + a_i. Let watch start times w_i >= f_i, and w_i are distinct? Actually, watches are sequential, so we can assume w_1 < w_2 < ... < w_n, with w_{i} >= w_{i-1} + 1. Also, the buffer constraint: at any time t, sum_{i: s_i <= t < w_i + 1} a_i <= m.
    // Consider the time just before the k-th download starts. At that moment, the buffer contains videos that have started but not yet finished watching. The sum of their sizes must be <= m - a_k (since we need to reserve a_k for the new download). Actually, at the exact start of download k, the space a_k is reserved, so the sum of sizes of all other videos that are still occupying space must be <= m - a_k.
    // Which videos are those? They are videos that started earlier and have not been deleted yet (i.e., their watch has not finished). 
    // Let's order the videos by their download start times. At the start of download k, the videos 1..k-1 have started. Some of them may have finished watching and been deleted. The number of watches that have been completed before s_k is at most floor(s_k - something)? Actually, watches take 1 minute each and can only happen after the corresponding download finishes. So the number of watches completed before time s_k is at most the number of videos that have finished downloading and had time to be watched. But we can assume without loss of generality that we start watches as soon as possible. So the number of watches completed before s_k is at most k-1, but could be less if some downloads finished late.
    // To minimize makespan, we want to start downloads as early as possible. So we want to keep the download pipe busy. The only delays are when we have to wait for a watch to finish to free space.
    // This is equivalent to: we have a sequence of downloads. After each download, we can schedule a watch. The total time is the sum of download times plus the number of "waiting" periods where we cannot start the next download because the buffer is full.
    // Let's think in terms of the buffer content. The buffer can hold at most m. Each video i, once downloaded, stays in the buffer for at least 1 minute (watch time). So if we download videos back-to-back, the buffer will accumulate videos. We can only start a new download if the current buffer occupancy + a_i <= m. The buffer occupancy decreases by the size of a video when we finish watching it. We can watch at most one video at a time, and each watch takes 1 minute. So the maximum rate at which we can free space is limited by the sizes of the videos we watch. But we can choose which video to watch next.
    // This is similar to the problem "Painting Fence" or "Scheduling with buffer". I recall a solution: sort videos by size descending? Let's test with example 2: descending [5,4,3,2,1]. We saw it gives 20. Ascending gave 18. So neither extreme is optimal.
    // Let's try to find a schedule for example 2 that gives 17.
    // We need total time 17. Total download = 15, total watch = 5. So total idle time = 17 - 15 - 5? No, total time = download time + watch time - overlap. Overlap = (15+5) - 17 = 3. So we have 3 minutes of overlap (watching while downloading). Since we have 5 watches, we can overlap at most 4 minutes of watch with download (because the last watch cannot overlap with any download after it). So maximum possible overlap is 4. We got 3, so we lost 1 minute of potential overlap due to buffer constraints.
    // How to achieve overlap of 3? That means during 3 of the 5 watches, a download is also happening. The other 2 watches happen without concurrent download.
    // Let's try to construct: We want to start a download, then while it's downloading, we can't watch it yet. We need to have another video ready to watch to overlap. So we need to have at least two videos in the buffer at some point.
    // Suppose we start with the largest video 5. Download 5 from 0 to 5. At time 5, we can watch 5 (5 to 6). During this watch, we cannot start download 4 because buffer would be 5 (video 5 still there) + 4 = 9 > 5. So we must wait until 6 to start 4. So no overlap during watch of 5.
    // Instead, start with a smaller video. Start with 2: download 2 (0-2). At time 2, we can watch 2 (2-3). During this watch, we can start download of something? Buffer at time 2: video 2 is still there (size 2). If we start download 3, buffer becomes 2+3=5 <=5, so we can! So at time 2, start download 3 (2-5). At time 3, watch 2 ends, free 2, buffer now has video 3 (size 3) downloading. At time 5, download 3 finishes. Now we can watch 3 (5-6). During watch 3, can we start download 4? Buffer at time 5: video 3 (size 3). Start download 4: buffer 3+4=7 >5, no. So wait until 6. At 6, watch 3 ends, free 3. Start download 4 (6-10). At 10, watch 4 (10-11). During watch 4, start download 5? Buffer: video 4 (4) + 5 = 9 >5, no. Wait until 11. Start download 5 (11-16). Watch 5 (16-17). Then download 1 (17-18), watch 1 (18-19). Total 19. Not 17.
    // Try different order: 3,2,4,1,5?
    // 0: start 3 (0-3). At 3, watch 3 (3-4). During watch 3, buffer: 3. Can we start 2? 3+2=5 <=5, yes. Start 2 at 3 (3-5). At 4, watch 3 ends, free 3. Buffer now: 2 (downloading). At 5, download 2 finishes. Watch 2 (5-6). During watch 2, buffer: 2. Start 4? 2+4=6>5, no. Wait until 6. Start 4 (6-10). Watch 4 (10-11). During watch 4, buffer: 4. Start 1? 4+1=5 <=5, yes! Start 1 at 10 (10-11). At 11, watch 4 ends, free 4. Buffer: 1 (just finished downloading at 11). Watch 1 (11-12). During watch 1, buffer: 1. Start 5? 1+5=6>5, no. Wait until 12. Start 5 (12-17). Watch 5 (17-18). Total 18.
    // Still 18.
    // Try: 4,1,5,2,3?
    // 0: start 4 (0-4). At 4, watch 4 (4-5). During watch 4, buffer:4. Start 1? 4+1=5, yes. Start 1 at 4 (4-5). At 5, watch 4 ends, free 4. Buffer: 1 (finished at 5). Watch 1 (5-6). During watch 1, buffer:1. Start 5? 1+5=6>5, no. Wait until 6. Start 5 (6-11). Watch 5 (11-12). During watch 5, buffer:5. Start 2? 5+2=7>5, no. Wait until 12. Start 2 (12-14). Watch 2 (14-15). During watch 2, buffer:2. Start 3? 2+3=5, yes. Start 3 at 14 (14-17). At 15, watch 2 ends, free 2. Buffer: 3 (downloading). At 17, download 3 finishes. Watch 3 (17-18). Total 18.
    // How to get 17? Let's try to have more overlap. We need to start a download during a watch. That requires that the sum of the video being watched and the new video <= m. So we want to pair a large video with a small video during watch. For example, watch a video of size x, and start a download of size y such that x+y <= m. Then after the watch, we free x, and we have y downloading. This allows continuous download.
    // In example 2, m=5. Pairs with sum <=5: (1,2)=3, (1,3)=4, (1,4)=5, (2,3)=5. (1,1) not possible since distinct videos. So we can overlap a watch of 1 with download of 2,3,4; watch of 2 with download of 1,3; watch of 3 with download of 1,2; watch of 4 with download of 1.
    // We want to chain these overlaps to minimize idle time.
    // Let's try to schedule: Start with download 2 (0-2). Watch 2 (2-3) and start download 3 (2-5) [2+3=5]. At 3, watch 2 ends, free 2. Buffer: 3 downloading. At 5, download 3 finishes. Watch 3 (5-6) and start download 1? 3+1=4<=5, yes. Start 1 at 5 (5-6). At 6, watch 3 ends, free 3. Buffer: 1 (finished at 6). Watch 1 (6-7) and start download 4? 1+4=5, yes. Start 4 at 6 (6-10). At 7, watch 1 ends, free 1. Buffer: 4 downloading. At 10, download 4 finishes. Watch 4 (10-11) and start download 5? 4+5=9>5, no. So wait until 11. Start 5 (11-16). Watch 5 (16-17). Total 17! Yes, that gives 17.
    // So the schedule: 2, 3, 1, 4, 5. Order of downloads: 2,3,1,4,5. Watches in same order.
    // This order is not strictly sorted. It's like: we alternate small and large to keep the sum <= m during watches.
    // This suggests a greedy strategy: always download the largest possible video that can fit with the currently watching video? But we can also choose which video to watch next to free space for the next download.
    // This is equivalent to the problem of finding the minimum makespan for a two-machine flow shop with a buffer of capacity m, where the first machine has processing times a_i, second machine has processing times 1. This is known to be solvable by a greedy algorithm: sort videos by a_i? Actually, there is a known result: the optimal makespan is D + max(0, max_{S} (sum_{i in S} a_i - (|S|-1) - m))? Let's think.
    // Consider any subset S of videos that are in the buffer simultaneously at some point. At the moment when the last of these videos finishes downloading, the sum of their sizes is at most m + (|S|-1)*1? Because during the time these videos were downloading, we could have been watching some of them. Actually, if we have a set of videos that are all in the buffer at the same time, that means they have all started downloading but none have finished watching. The total time from the start of the first download in S to the end of the last download in S is at least sum_{i in S} a_i (since downloads are sequential). During that time, we can complete at most (that time) watches? But watches take 1 minute each. The number of watches we can complete in that interval is at most the length of the interval, because we can watch at most one at a time. However, we can only watch videos that have finished downloading. So if we have a set S of videos that are all in the buffer at some time t, then at time t, none of them have been watched yet. The total space they occupy is sum_{i in S} a_i <= m. So any set of videos that are simultaneously in the buffer must have total size <= m. That's the direct constraint.
    // But the schedule can be seen as: we partition the videos into "chains" where we download a video, then while watching it, we download the next, etc. The condition for a chain is that the sum of the size of the currently watched video and the next download is <= m. If we can chain all videos, total time = D + 1. If not, we have breaks.
    // Actually, we can think of it as: we need to order the videos such that for each i from 1 to n-1, if we want to start download i+1 immediately after download i finishes, we need that at the time download i finishes, the buffer occupancy (which is the size of the video we are currently watching, if any, plus maybe other videos waiting) plus a_{i+1} <= m. But we can also choose to watch a different video to free space.
    // This is getting complicated. Let's look for a known solution. I recall a Codeforces problem: "Watching Videos" or similar. The solution often involves sorting the videos and then using a greedy approach with a multiset or two pointers.
    // Another perspective: The total time is D + n - (number of minutes we can overlap). The maximum overlap is limited by the buffer. Each minute of overlap corresponds to a minute where we are both downloading and watching. During such a minute, the buffer occupancy is the sum of the sizes of the videos that are downloaded but not yet watched. At the start of that minute, we have some videos in the buffer. We can only start a new download if the current buffer + new video <= m. So to maximize overlap, we want to keep the buffer as full as possible with small videos? Or we want to pair large videos with small ones.
    // Let's consider the following: We have n videos. We want to find the minimum T such that there exists a schedule finishing by T. This is a decision problem: can we finish by T? Then we can binary search T? But T can be up to 2e14, binary search might be possible if we have a fast check. But n=2e5, so O(n log T) might be okay if check is O(n). But we need an efficient check.
    // How to check if we can finish by T? We have total download D, total watch n. The last watch must end at T. So the last download must finish by T-1. The total idle time on download is T - D - 1? Actually, if we finish at T, the last action is a watch ending at T. So the last download finishes at some time <= T-1. The total time from start to T is T. The download machine is busy for D time. So idle download time = T - D. But we also have n watches of 1 minute each. They can overlap with downloads. The total time is D + n - overlap. So overlap = D + n - T. We need overlap >= 0, and also the buffer constraint must allow that much overlap.
    // The buffer constraint essentially says that we cannot have too many large videos in the buffer at once. The maximum total size of videos that can be in the buffer simultaneously is m. But they also take time to watch. 
    // Let's think about the "critical" condition. Suppose we sort videos by size descending. Consider the largest videos. They take a lot of space and time. If we have a very large video, say size close to m, then while it is downloading, we cannot have any other video in the buffer (since its size alone is m). So during its download, we cannot have any other video downloaded but unwatched. That means we cannot have any watch overlap during its download? Actually, we can watch a video that was already fully downloaded before we started this large download. But if we start a large download, we must ensure that the buffer is empty except for that download. So before starting it, we must have watched all other videos. That forces a sequential behavior.
    // In general, if we have a set of videos with large sizes, they might force idle time.
    // I remember a similar problem: "Downloading and watching videos" where the answer is sum a_i + n - something. Actually, there is a known greedy: sort videos by size. Then maintain the current time and buffer. But that might not be optimal because we can reorder.
    // Let's search my memory: There is a Codeforces problem "Videos" or "Monocarp" maybe from a recent contest. The solution might be: sort a in descending order. Then we try to "pair" videos: we can watch a video while downloading another if their sum <= m. We want to maximize the number of such pairs. Each pair saves 1 minute (since we overlap a watch with a download). But we can chain more than pairs.
    // Actually, if we have a sequence of videos where each adjacent pair (watched video, next download) satisfies sum <= m, we can continuously overlap. The number of overlaps we can achieve is the maximum number of videos we can chain in this way, minus 1? Let's see: In example 2, we had chain: 2 (watch) -> 3 (download) [2+3<=5], 3 (watch) -> 1 (download) [3+1<=5], 1 (watch) -> 4 (download) [1+4<=5], 4 (watch) -> 5 (download) [4+5>5, break]. So we had 3 successful overlaps, 1 break. Total overlaps = 3. Total time = D + n - overlaps = 15 + 5 - 3 = 17. Yes!
    // So the total time = D + n - (number of overlaps). Each overlap corresponds
