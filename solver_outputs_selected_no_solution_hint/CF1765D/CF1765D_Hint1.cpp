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

    long long total_download = 0;
    for (long long x : a) {
        total_download += x;
    }

    long long low = 0, high = total_download + n; // upper bound: download all + watch all
    long long ans = high;

    auto can = [&](long long T) -> bool {
        // We need to schedule n watches, each takes 1 minute, and downloads.
        // Total download time is fixed = total_download.
        // The total time T must be at least total_download + n? Not necessarily,
        // because we can watch while downloading.
        // But we need to check if we can finish by time T.
        // We can think of it as: we have T minutes. In each minute we can either
        // download 1 MB or watch a video (if downloaded). Watching takes 1 minute.
        // We need to download all videos and watch all videos.
        // Equivalent to: we have T slots. Some slots are used for watching (n slots).
        // The remaining T - n slots can be used for downloading.
        // But we also have disk constraint m.
        // However, we can delete videos after watching, so disk space is only
        // constrained by the maximum sum of sizes of videos that are downloaded
        // but not yet watched at any time.
        // Since we can choose order, we should watch videos in increasing order of size
        // to minimize disk usage? Actually, to maximize download concurrency,
        // we want to free space quickly. Watching small videos first frees little space,
        // watching large videos first frees more space. But we need to download them first.
        // The optimal strategy: sort videos by size. We can download in any order,
        // but we must watch in some order. The disk constraint: at any time,
        // sum of sizes of completely downloaded but not yet watched videos <= m.
        // Since we can delete immediately after watching, the peak disk usage
        // is the maximum over time of (downloaded - watched_deleted).
        // If we download in order of increasing size, and watch in order of increasing size,
        // we can pipeline: download a video, then watch it, then delete.
        // But we can also download multiple videos before watching any.
        // The key is to find the minimum T such that we can schedule.
        // Another perspective: total time = total_download + n - overlap,
        // where overlap is the time we spend downloading while watching.
        // Overlap is limited by disk space: we can only have at most m MB of videos
        // downloaded ahead of watching.
        // So the maximum overlap is the maximum amount of download we can do
        // while we are watching videos, which is limited by m.
        // More precisely, we can think of the process as:
        // We start downloading. At some point we start watching.
        // While watching, we can continue downloading as long as disk space permits.
        // The total time = total_download + n - (time spent downloading during watching).
        // The time spent downloading during watching is at most m, because
        // we can only download up to m MB ahead of the current watch point.
        // Actually, if we watch videos in some order, the total download during watching
        // cannot exceed the sum of sizes of videos that are watched? No.
        // Let's formalize: Let the watching order be a permutation.
        // At the moment we start watching video i, we must have downloaded it.
        // The total downloaded before starting to watch video i is at least sum of sizes
        // of videos watched up to i (including i). The disk usage at that moment
        // is total downloaded minus total deleted (which is sum of sizes of already watched).
        // So disk usage = downloaded - watched_so_far_sizes.
        // This must be <= m at all times.
        // To minimize total time, we want to maximize the overlap between downloading and watching.
        // The maximum overlap is achieved when we keep the disk as full as possible.
        // The total time = total_download + n - overlap.
        // Overlap is the amount of download that happens after the first watch starts.
        // The first watch can start as soon as the first video is downloaded.
        // Then we can download during the n-1 remaining watches.
        // The total download during watching is at most m * (number of watches?) No.
        // Actually, during each minute of watching, we can download 1 MB if disk space allows.
        // The total download during watching is limited by the total free disk space
        // that becomes available as we watch and delete videos.
        // Each watched video frees its size. So total freed space during watching = sum of sizes
        // of all videos except possibly the first one? Actually, we start with some downloaded videos.
        // The total download during watching = min(total_download - size_of_first_video, 
        // sum of sizes of videos watched after the first + initial free space?).
        // This is getting complicated.

        // Alternative: binary search on T and simulate greedily.
        // Given T, can we finish? We have T minutes.
        // We can decide the schedule: at each minute, we can either download a part of a video
        // or watch a video (if fully downloaded). We want to finish all.
        // Since watching takes 1 minute and we have n videos, we must reserve n minutes for watching.
        // The remaining T - n minutes are for downloading. But we can download during watching too.
        // Actually, total download time needed is total_download. We have T total minutes.
        // In those T minutes, we can do at most T minutes of work (download or watch).
        // Watching takes n minutes, so we have T - n minutes left for pure downloading
        // if we never overlap. But we can overlap: while watching, we can also download.
        // So the total download we can do is (T - n) + (overlap minutes).
        // Overlap minutes = minutes where we are both watching and downloading.
        // Since we have n watch minutes, overlap <= n.
        // Also overlap is limited by disk: we can only download if there is free disk space.
        // The maximum overlap is the maximum amount of download we can do during the n watch minutes.
        // This is equivalent to: we can have at most m MB of "buffer" ahead of the watch pointer.
        // So the total download we can do = min(total_download, (T - n) + min(n, m + something?)).
        // Actually, if we have infinite disk, overlap = n (we download every minute we watch).
        // Then total download capacity = T (since we can download every minute, including watch minutes).
        // So we need T >= total_download + n? No, if we download every minute including watch minutes,
        // total download = T, we need T >= total_download, and we also need to fit n watches,
        // but watches take time from downloading? If we download during watches, we are doing both.
        // So total time T must satisfy: we can do n watches and total_download downloads.
        // Since they can overlap, the total time T must be at least max(total_download, n)?
        // But we can't watch before downloading. So we need T >= total_download + 1? No.
        // Example: one video of size 5, m=5. total_download=5, n=1.
        // We download 5 minutes, then watch 1 minute. Total = 6. T=6.
        // Here T = total_download + n = 6. Overlap = 0.
        // If we had two videos: sizes 1,1. total_download=2, n=2.
        // Download first (1 min), then watch first (1 min) while downloading second (1 min).
        // Total time: download first (1), then watch first + download second (1), then watch second (1).
        // Total = 3. T = total_download + n - overlap = 2+2-1=3.
        // Overlap = 1.
        // So T = total_download + n - overlap.
        // Overlap is the number of minutes we download while watching.
        // Maximum overlap is limited by disk: we can only download up to m MB ahead of the current watch.
        // More precisely, at any time during watching, the amount of downloaded but unwatched data
        // cannot exceed m. This means the total download during watching cannot exceed
        // the sum of sizes of videos that are watched after the download? Hmm.

        // Let's think of the process as a sequence of downloads and watches.
        // We can reorder videos arbitrarily. Optimal order: sort by size ascending?
        // Or descending? Let's test with examples.
        // Example 1: n=5, m=6, a=[1,2,3,4,5]. Sorted: 1,2,3,4,5. Output 16.
        // total_download=15, n=5. T=16 => overlap = 15+5-16=4.
        // Example 2: m=5, same a. Output 17 => overlap=3.
        // Example 3: n=4, m=3, a=[1,3,2,3] sorted: 1,2,3,3. total=9, n=4. Output 12 => overlap=1.
        // So overlap depends on m and the sizes.

        // Let's try to derive formula: minimum T = total_download + n - max_overlap.
        // max_overlap = maximum amount of download we can do during the n watch minutes.
        // During the n watch minutes, we can download at most n MB (since 1 minute = 1 MB).
        // But we are limited by disk: we can only have at most m MB of unwatched videos.
        // So the total download during watching is limited by the total size of videos
        // that we can "pre-download" before watching them, which is constrained by m.
        // Actually, consider we watch videos in some order. Let the order be p_1, p_2, ..., p_n.
        // We start by downloading some videos. At time t_0, we start watching p_1.
        // From then on, we watch one video per minute. During the minute we watch p_i,
        // we can download the next videos. The total downloaded before watching p_i
        // must be at least sum_{j=1}^i a_{p_j}. The disk usage at the start of watching p_i
        // is (total downloaded so far) - sum_{j=1}^{i-1} a_{p_j} (since previous watched are deleted).
        // This must be <= m. Also, we cannot download more than the remaining sizes.
        // To maximize overlap, we want to keep the disk as full as possible.
        // The maximum total download during watching is the sum over i=1 to n-1 of
        // min(1, free space at that minute)? Actually, each watch minute we can download 1 MB
        // if there is free disk space. The free disk space at the start of watch i is
        // m - (downloaded - watched_so_far). But we can also free space by finishing a watch.
        // This is equivalent to: we have a buffer of size m. We can fill it with downloads.
        // When we watch a video of size s, we remove s from the buffer (freeing space).
        // During the watch minute, we can add up to 1 MB to the buffer (download).
        // So the buffer size changes by (1 - s) during that minute? Actually, we watch for 1 minute,
        // and during that minute we can download 1 MB. At the end of the minute, we delete the video,
        // freeing s MB. So net change in buffer: +1 (download) - s (deletion) = 1 - s.
        // But the deletion happens at the end of the watch. The buffer constraint must hold
        // at all times. The maximum buffer size is m. Initially buffer = 0.
        // We can download before any watch to fill the buffer up to m.
        // Then we start watching. For each watch of size s:
        // - We must have at least s in the buffer? Actually, the video must be fully downloaded
        //   before we start watching it. So the buffer must contain the video's data.
        //   But the buffer represents all downloaded unwatched data. So before watching,
        //   the buffer must have at least s (the video we are about to watch is part of it).
        // - During the watch minute, we can download 1 MB, so buffer can increase by 1,
        //   but must not exceed m. Then at the end, we delete s, so buffer decreases by s.
        // So the sequence of buffer sizes: start with B0 = some initial downloads (<= m).
        // For i=1..n:
        //   require B_{i-1} >= a_{p_i} (video must be downloaded).
        //   During watch: B can increase by up to 1, so B_i' = min(m, B_{i-1} + 1).
        //   Then delete: B_i = B_i' - a_{p_i}.
        // We want to maximize total download during watches = sum of (B_i' - B_{i-1}) over i,
        // which is at most n (since each watch minute we can add at most 1).
        // But we also have initial downloads before first watch: B0.
        // Total downloads = B0 + sum (download during watches) + downloads after last watch?
        // Actually, after last watch, we don't need to download anything else.
        // So total_download = B0 + sum_{i=1}^n (B_i' - B_{i-1}) + (downloads after last watch? No, all done).
        // Wait, we must download all videos. The total downloaded is sum a_i.
        // The downloads can happen before first watch, during watches, and after last watch?
        // But after last watch, we are done, so no downloads after. So all downloads happen
        // before or during watches. So total_download = B0 + sum_{i=1}^n d_i, where d_i is download during watch i (0 or 1).
        // Also, the buffer evolution: B_i = B_{i-1} + d_i - a_{p_i}, with 0 <= d_i <= 1, B_{i-1} + d_i <= m, B_{i-1} >= a_{p_i}.
        // And B_n = 0 (all watched and deleted).
        // We want to minimize total time T = (time before first watch) + n.
        // Time before first watch = B0 (since we download B0 MB at 1 MB/min).
        // So T = B0 + n.
        // But wait, is there any download after the last watch? No, because we finish.
        // So total time = B0 + n.
        // And total_download = B0 + sum d_i.
        // We want to minimize B0 + n given that we can choose order p and d_i in {0,1} such that
        // B_i = B_{i-1} + d_i - a_{p_i}, B_{i-1} >= a_{p_i}, B_{i-1} + d_i <= m, B_n = 0.
        // Also B0 <= m.
        // Since total_download is fixed, minimizing B0 is equivalent to maximizing sum d_i.
        // Because total_download = B0 + sum d_i => B0 = total_download - sum d_i.
        // So T = total_download - sum d_i + n.
        // Thus we want to maximize sum d_i (total download during watches).
        // sum d_i <= n, and also constrained by buffer dynamics.
        // The buffer dynamics: B_i = B_{i-1} + d_i - a_{p_i}.
        // Summing over i=1..n: B_n - B0 = sum d_i - sum a_{p_i} => 0 - B0 = sum d_i - total_download.
        // So B0 = total_download - sum d_i, consistent.
        // The constraints: for each i, B_{i-1} >= a_{p_i} and B_{i-1} + d_i <= m.
        // Also B_i >= 0 (since buffer can't be negative). But B_i = B_{i-1} + d_i - a_{p_i} >= 0 is automatically
        // satisfied if B_{i-1} >= a_{p_i} and d_i >= 0? Actually, if d_i=0, B_i = B_{i-1} - a_{p_i} >= 0 by the first constraint.
        // So the main constraints: a_{p_i} <= B_{i-1} <= m - d_i.
        // Since d_i is 0 or 1, B_{i-1} <= m if d_i=0, or B_{i-1} <= m-1 if d_i=1.
        // Also B_{i-1} must be at least a_{p_i}.
        // We start with B0. We want to choose order and d_i to maximize sum d_i.
        // This is like: we have a buffer that starts at B0. We process videos in some order.
        // For each video, we must have buffer >= its size. We can optionally increase buffer by 1
        // during processing (if buffer < m), then we subtract its size.
        // We want to maximize the number of times we increase buffer (i.e., d_i=1).
        // Since total_download is fixed, B0 is determined by sum d_i.
        // But B0 must be <= m. So sum d_i >= total_download - m.
        // Also sum d_i <= n.
        // So sum d_i is bounded below by max(0, total_download - m) and above by n.
        // But we also have the ordering constraints: can we achieve a given sum d_i?
        // Let's think: we want to maximize sum d_i. The maximum possible is n (always download during watch).
        // Can we always achieve sum d_i = n? That would require B0 = total_download - n.
        // And we need B0 <= m, so total_download - n <= m => total_download <= m + n.
        // Also we need to be able to schedule the videos such that we never exceed m and always have enough buffer.
        // If we set d_i=1 for all i, then B_i = B_{i-1} + 1 - a_{p_i}.
        // We need B_{i-1} >= a_{p_i} and B_{i-1} + 1 <= m => B_{i-1} <= m-1.
        // So we need a_{p_i} <= B_{i-1} <= m-1.
        // Starting from B0 = total_download - n. We need B0 <= m-1.
        // And we need to order videos so that these constraints hold.
        // This is similar to scheduling with buffer. The optimal order is to process videos in increasing order of size?
        // Let's test: if we process small videos first, B decreases slowly, so we might keep B within bounds.
        // If we process large videos first, B drops quickly, which might violate B_{i-1} >= a_{p_i} for later large videos?
        // Actually, we need B_{i-1} >= a_{p_i}. If we process large videos when B is large, it's okay.
        // But we also need B_{i-1} <= m-1. If B0 is close to m, we might exceed if we add 1.
        // So we want to keep B as low as possible to avoid exceeding m, but as high as possible to cover large videos.
        // The classic result: to maximize the number of videos we can process with a buffer constraint,
        // we should sort videos by size? There's a known problem: "Downloading and watching videos" 
        // The optimal strategy is to sort videos by size ascending? Or descending?
        // Let's think about the buffer constraint: B_i = B_{i-1} + d_i - a_i.
        // We want to keep B_i between 0 and m.
        // If we always download during watch (d_i=1), then B_i = B_{i-1} + 1 - a_i.
        // This is like a random walk. We want to choose order to keep B_i in [0, m].
        // Since a_i >= 1, B_i tends to decrease if a_i > 1, or stay same if a_i=1.
        // To avoid hitting 0 (which would prevent downloading larger videos later), we should process larger videos earlier when B is larger.
        // To avoid hitting m, we should process smaller videos when B is near m? Actually, if B is near m, adding 1 might exceed m, so we cannot download during that watch. So we might need to set d_i=0 for some watches.
        // So the maximum sum d_i is not always n. It depends on the sizes and m.
        // Let's try to find a greedy algorithm to compute max sum d_i for a given order.
        // Since we can choose order, we want to find the maximum possible sum d_i over all orders and valid d_i sequences.
        // This is equivalent to: we have n items with sizes a_i. We want to sequence them.
        // We start with B0. For each item, we can optionally increase B by 1 (if B < m), then subtract a_i.
        // We require B >= a_i before subtraction. We want to maximize the number of increases.
        // Since total_download = B0 + sum d_i, and B0 = total_download - sum d_i, B0 is determined by sum d_i.
        // So we are choosing sum d_i, and then we need to check if there exists a sequence with that many increases.
        // Alternatively, we can binary search T, and check if T >= total_download + n - max_possible_overlap.
        // But we need to compute max_overlap efficiently.

        // Let's think differently: The total time T = total_download + n - overlap.
        // Overlap is the number of minutes we download while watching.
        // We can think of the process as: we have a buffer of size m. We can fill it by downloading.
        // Watching a video takes 1 minute and frees its size at the end.
        // During the watch minute, we can download 1 MB if buffer is not full.
        // So the maximum overlap is the maximum number of watch minutes during which we can download.
        // This is equivalent to: we have n watches. We can download during a watch if the buffer
        // has free space at the start of the watch (or during). Actually, we can always download
        // during the watch as long as buffer < m at the start of the watch? But buffer might become full
        // during the watch if we download? We can only download 1 MB per minute, so if buffer <= m-1,
        // we can download that 1 MB. So condition to download during watch i is: B_{i-1} <= m-1.
        // Also we need B_{i-1} >= a_{p_i} to have the video downloaded.
        // So we want to maximize the number of i such that B_{i-1} <= m-1 and B_{i-1} >= a_{p_i}.
        // And we can choose order and initial B0.
        // B0 = total_download - sum d_i. Since sum d_i is the number of watches with download,
        // let k = sum d_i. Then B0 = total_download - k.
        // We need B0 <= m (since buffer can't exceed m initially). So k >= total_download - m.
        // Also B0 >= 0, so k <= total_download.
        // But k <= n.
        // So k is in [max(0, total_download - m), min(n, total_download)].
        // We want to maximize k.
        // For a given k, B0 = total_download - k.
        // We need to find an ordering and a set of k watches where we download (d_i=1) and n-k watches where we don't (d_i=0).
        // The constraints: B_i = B_{i-1} + d_i - a_{p_i}.
        // B_{i-1} >= a_{p_i} for all i.
        // B_{i-1} <= m - d_i (since if d_i=1, B_{i-1} <= m-1; if d_i=0, B_{i-1} <= m).
        // Also B_i >= 0 automatically if B_{i-1} >= a_{p_i}.
        // We want to know the maximum k achievable.

        // This looks like we can process videos in some order, and we have a "resource" B that changes.
        // We can think of it as: we have a sequence of operations. Each operation is either "download then watch" (d=1) or just "watch" (d=0).
        // For d=1: B must be in [a, m-1], then B <- B + 1 - a.
        // For d=0: B must be in [a, m], then B <- B - a.
        // We start with B0. We want to end with B_n = 0.
        // Notice that the total change in B is sum (d_i - a_i) = k - total_download = -B0.
        // So B_n = B0 + k - total_download = 0, consistent.
        // We want to maximize k.
        // This is similar to: we have items with sizes a_i. We want to choose k items to be "with download" and the rest "without".
        // But the order matters because B must stay within bounds.
        // Let's try to find a greedy strategy. Suppose we decide which items get d=1 and which get d=0.
        // Can we always order them to satisfy constraints if certain conditions hold?
        // Consider the net effect on B: for d=1, B decreases by (a_i - 1); for d=0, B decreases by a_i.
        // So B decreases faster with d=0. To keep B from dropping below required a_j, we should do operations that decrease B less (i.e., d=1 or small a) earlier? Or later?
        // Actually, we need B to be large enough for large a_j. So we should process large a_j when B is large.
        // B starts at B0 and decreases. So we should process larger a_j earlier.
        // Also, to avoid exceeding m, we should process items with d=1 when B is not too close to m.
        // If we process large a_j early, B drops quickly, which might help avoid exceeding m later.
        // This suggests sorting by a_i descending might be good.
        // Let's test with examples.
        // Example 1: a=[1,2,3,4,5], m=6. total=15, n=5. Max k?
        // If we sort descending: 5,4,3,2,1.
        // Try k=5 (all d=1): B0 = 15-5=10. But B0 must be <= m=6. So k=5 impossible because B0=10>6.
        // So k <= total_download - m? Actually B0 = total_download - k <= m => k >= total_download - m = 15-6=9. But k<=n=5. Contradiction: k>=9 and k<=5 impossible. So k cannot be 5. Wait, B0 = total_download - k. If k=5, B0=10 > m=6, invalid. So we need B0 <= m => total_download - k <= m => k >= total_download - m = 9. But k<=5, so no solution with k=5. So maximum k is at most 5, but we need k>=9? That's impossible. So my formula B0 = total_download - k assumes all downloads happen either before first watch or during watches. But wait, we can also download after the last watch? No, after last watch we are done. So all downloads must happen before or during watches. So total_download = B0 + k. Thus B0 = total_download - k. Since B0 <= m, we have total_download - k <= m => k >= total_download - m. For example 1, total_download=15, m=6 => k >= 9. But k <= n=5. So it's impossible to finish if we only download before and during watches? But the answer is 16, which is total_download + n - overlap = 15+5-4=16 => overlap=4. So k=4. Then B0 = 15-4=11. But B0=11 > m=6! Contradiction.
        // So my assumption that all downloads happen before first watch or during watches is wrong. We can download after some watches? No, after all watches we are done. But we can download before the first watch, and during watches, and also between watches? There are no "between watches" because we watch consecutively? The problem says: "Once a video is fully downloaded... Monocarp can watch it. Watching each video takes exactly 1 minute... so Monocarp can start downloading another video while watching the current one." It doesn't say he must watch videos back-to-back. He can have idle time between watches? But idle time would just increase total time without benefit. So optimal schedule has no idle time: either downloading or watching. But we can have periods where we only download (no watch). That would be before the first watch, or after a watch if we choose not to start the next watch immediately? But if we don't start the next watch immediately, we are just downloading, which could be done during the next watch if we had started it. So to minimize total time, we should always be either watching or downloading. And we should start watching as soon as possible. So the schedule consists of: some initial downloading (B0 minutes), then a sequence of n watches, each taking 1 minute, during which we may also download (if possible). There is no downloading after the last watch because we are done. There is no downloading between watches because watches are back-to-back (otherwise we waste time). So the total time is B0 + n. And total downloaded = B0 + (download during watches). So B0 = total_download - k, where k is download during watches. So B0 = 15 - 4 = 11 in example 1. But B0 cannot exceed m=6. So how can B0 be 11? That means we downloaded 11 MB before starting the first watch. But disk size is only 6! That's impossible. So my model is flawed: we cannot download 11 MB before the first watch because disk is only 6. We must delete some videos before we can download more. But we can only delete after watching. So we must watch some videos to free space. Therefore, we cannot download all 11 MB before the first watch. We must interleave downloads and watches more finely. The initial download B0 is limited by m. So B0 <= m. Then during the first watch, we can download 1 MB, then delete the first video, freeing space. So the total downloaded before the first watch is at most m. The rest must be downloaded during or after some watches. But after the first watch, we have freed some space, so we can download more. So the total download is not just B0 + k; it's B0 + (download during watches) + (download between watches?) But we said no idle time. Actually, we can have a schedule where we watch a video, then download a bit, then watch another, etc. But that would mean there is a gap between watches where we only download. That gap would increase total time. However, we can instead do that download during the next watch if we had started the next watch earlier? Not necessarily, because to start the next watch, we need the next video fully downloaded. If it's not fully downloaded, we cannot start watching it. So we might have to wait (download) before starting the next watch. That waiting time is pure download time, not overlapped with watching. So the total time is: (initial download) + (watch 1) + (download after watch 1 if needed before watch 2) + (watch 2) + ... So the total time = sum of all download times + n, but some download times overlap with watches. The non-overlapped download time is the time we spend downloading while not watching. The total time = (non-overlapped download time) + n. The total download time = total_download. So non-overlapped download time = total_download - overlapped download time. Thus total time = total_download - overlap + n. This is the same formula. But now B0 is not the initial download before first watch; it's the total non-overlapped download time. That can be larger than m because we can have non-overlapped download between watches. So my previous B0 was the total non-overlapped download, not just the initial buffer. Let's redefine:

        Let T = total time.
        We have n watches, each 1 minute. The remaining T - n minutes are pure download minutes (no watch). During the n watch minutes, we can also download up to n MB, but limited by disk.
        So total download = (T - n) + overlap, where overlap is the amount downloaded during watch minutes.
        So T = total_download + n - overlap.
        Overlap is the number of watch minutes during which we download.
        Let k = overlap. Then T = total_download + n - k.
        We want to minimize T, so maximize k.
        k <= n.
        Also, the total non-overlapped download is total_download - k. This is the time we spend downloading while not watching. This time can be distributed before the first watch and between watches. The disk constraint: at any time, the amount of downloaded but unwatched data <= m.
        This imposes a constraint on how much we can download ahead of watching.
        Let's think of the process as a sequence of events. We have a buffer of unwatched data. Initially 0. We can download to increase buffer (up to m). We can watch to decrease buffer by the video size (after a 1-minute watch during which we may also download 1 MB). The total time is the number of minutes elapsed.
        We want to finish all watches and have buffer 0.
        This is exactly the problem of scheduling downloads and watches to minimize makespan.
        There is a known greedy solution: sort videos by size? Let's search memory: There is a Codeforces problem "Videos" or "Downloading" where you sort by size and then simulate or use a formula. I recall a problem: "Monocarp and the videos" or similar. The solution often involves sorting the array and then using a greedy approach with a multiset or two pointers.
        Let's think about the optimal order of watching. Since we can delete after watching, the order of watching determines when space is freed. To maximize overlap, we want to free space as early as possible so we can download more during later watches. Freeing space early means watching large videos early? Or small videos early? If we watch a large video, we free a lot of space, which then allows more download during subsequent watches. But to watch a large video, we need to have it downloaded, which requires a lot of space initially. So there's a trade-off.
        Consider we have a set of videos. We can think of the process as: we need to download all videos. We can watch them in any order. The total time is the sum of download times plus the number of watches minus the overlap. The overlap is the amount of download we can do while watching. During a watch, we can download 1 MB if there is free disk space. The free disk space at the start of a watch is m - (buffer size). The buffer size is the sum of sizes of videos that are downloaded but not yet watched. When we watch a video of size s, the buffer size decreases by s (after the watch). During the watch, we can increase buffer by 1. So the net change in buffer during watch i is 1 - s_i (if we download), or -s_i (if we don't). To maximize the number of watches with download, we want the buffer to be as small as possible so that we have free space to download. But we also need the buffer to be at least the size of the next video we want to watch.
        This is similar to the
