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
    for (long long x : a) total_download += x;
    
    long long low = total_download + n; // minimum possible: download all + watch all
    long long high = total_download + n + (n - 1); // worst case: sequential
    
    auto can = [&](long long T) -> bool {
        // We need to schedule n downloads and n watches within T minutes.
        // Each download i takes a_i minutes, each watch takes 1 minute.
        // Disk space constraint: at any time, sum of sizes of currently downloading + downloaded but not yet watched <= m.
        // We process videos in sorted order (largest first) to minimize peak disk usage.
        // We simulate backwards: assign finish times for downloads and watches.
        // Equivalent condition: we can assign each video a download start time and watch start time.
        // Let's use a greedy: process videos from largest to smallest.
        // We maintain the latest time we can start a download so that it finishes by T and doesn't violate disk.
        // Actually, we can think of it as: we have T total minutes. We need to fit all downloads and watches.
        // The disk constraint is the tricky part.
        // Known solution: binary search on T, then check if we can schedule.
        // For a fixed T, we can simulate time from T down to 0, maintaining available disk space.
        // At any time, we can start a download if we have enough space.
        // We want to finish all downloads and watches by T.
        // Let's process time backwards: at time t, we can either be downloading or watching.
        // We need to assign each video a download interval of length a_i and a watch minute after download finishes.
        // Since watching takes 1 minute and can overlap with next download, we can think of "slots".
        // Alternative: The total time is at least total_download + n. The extra time is idle time when we cannot download due to disk full.
        // The disk is full when we have downloaded some videos but not watched them yet.
        // To minimize idle time, we should watch videos as soon as possible after downloading.
        // The optimal strategy: always download the largest possible video that fits, and watch videos immediately when they finish.
        // This suggests a greedy simulation.
        // Let's simulate the process with a priority queue of downloaded videos (by size? actually we just need to know when they finish watching).
        // But we need to check if a given T is feasible.
        // We can simulate forward: maintain current time, disk used, and a queue of videos being downloaded (finish time, size).
        // Also maintain a list of videos waiting to be downloaded.
        // We want to finish by T. We can try to schedule as early as possible.
        // Since we only need to check feasibility, we can simulate with the greedy: always download the largest video that fits now.
        // If no video fits, we must wait until a watch finishes and frees space.
        // This greedy is optimal because downloading larger videos earlier reduces future disk pressure.
        // So we can simulate with a multiset of available videos, and a priority queue of ongoing downloads (finish time, size).
        // We also need to account for watching time: after a download finishes, we must spend 1 minute watching it, during which the space is still occupied.
        // So the space is freed only after the watch finishes.
        // Let's design the simulation:
        // Sort videos descending.
        // We'll maintain current time cur = 0.
        // Disk used = 0.
        // Events: downloads finishing (we then start watching, which takes 1 minute, then free space).
        // We can use a priority queue of (finish_time, size) for downloads.
        // But we also need to know when watches finish. Since watch takes exactly 1 minute and starts immediately after download, we can just treat the space as freed at finish_time + 1.
        // So we can push (finish_time + 1, size) to a "free" event queue.
        // At any time, we try to start a new download if there is a video that fits.
        // If multiple fit, we pick the largest one (to minimize future pressure).
        // If none fit, we advance time to the next free event.
        // We continue until all videos are scheduled.
        // If the final time exceeds T, then T is not feasible.
        // This simulation runs in O(n log n) and is correct for checking a given T? Actually, we don't use T in the simulation; we just simulate the optimal schedule and see its makespan. Then we can binary search on T, but we can just compute the optimal makespan directly via simulation and output it. No need for binary search!
        // Let's verify: The greedy simulation (always download largest possible, advance time to next event when blocked) yields the minimum possible completion time. This is a known result for this problem (Codeforces 1741F? Actually it's from a known problem: "Videos" or similar). The simulation gives the exact minimum time.
        // So we can just simulate and output the final time.
        // Let's implement that.
        
        // But wait: The simulation must handle the fact that we can start a download at the same time we finish a watch? Yes, because deleting is instant. So when a watch finishes, space is freed at that exact minute, and we can start a new download at that same minute.
        // Also, we can start a download at time 0.
        // Let's implement:
        // Sort a descending.
        // Use a multiset for available videos (or just an index since we process in order? We need to pick the largest that fits. Since we sorted descending, we can just iterate through the list and pick the first that fits. But after some are downloaded, the remaining are still in sorted order. We can maintain a pointer or a set. Since we need to remove elements, a multiset or just a vector with a used flag? Better: use a multiset of sizes, initially all videos. At each step, find the largest size <= available space. If found, start download, remove from set, update disk used, push free event at current_time + size + 1 (since download takes size minutes, then watch takes 1 minute, so space freed at current_time + size + 1). If not found, we need to jump to the next time when space is freed. That time is the smallest free event time. So we advance current_time to that time, update disk used (subtract the sizes of all events at that time), and repeat.
        // This simulation will process each video once, and each event once. Total O(n log n).
        // Let's test on samples.
        return true; // dummy, we won't use this function
    };
    
    // Direct simulation:
    sort(a.begin(), a.end(), greater<long long>());
    multiset<long long> avail(a.begin(), a.end());
    // priority queue of (free_time, size), min-heap by time
    priority_queue<pair<long long, long long>, vector<pair<long long, long long>>, greater<pair<long long, long long>>> free_events;
    long long cur_time = 0;
    long long disk_used = 0;
    
    while (!avail.empty()) {
        // Try to start a download
        // Find largest video that fits in remaining space
        long long space_left = m - disk_used;
        auto it = avail.upper_bound(space_left);
        if (it != avail.begin()) {
            --it;
            long long sz = *it;
            avail.erase(it);
            disk_used += sz;
            // Download takes sz minutes, then watch 1 minute, so space freed at cur_time + sz + 1
            free_events.push({cur_time + sz + 1, sz});
        } else {
            // Cannot start any download now, must wait for next free event
            if (free_events.empty()) {
                // Should not happen because eventually all videos will be downloaded and space freed, but if avail not empty, there must be some download ongoing? Actually if avail not empty and no free events, it means we have no ongoing downloads? That's impossible because disk_used > 0 implies there are ongoing downloads. But if disk_used == 0 and no video fits? But a_i <= m, so at least one video fits when disk is empty. So this case won't happen.
                break;
            }
            long long next_time = free_events.top().first;
            cur_time = next_time;
            // Process all free events at this time
            while (!free_events.empty() && free_events.top().first == cur_time) {
                disk_used -= free_events.top().second;
                free_events.pop();
            }
        }
    }
    // After all videos are started, we need to finish the last download and its watch.
    // The final time is the maximum of (cur_time + last_download_time + 1) among all downloads? Actually, the last download finishes at some time, then we watch for 1 minute. The simulation's cur_time is the time when we started the last download. The final completion time is the maximum free_event time among all events, because that's when the last watch finishes and space is freed. But we also need to account for the fact that after starting the last download, we don't need to wait for its free event to finish the process; the process ends when the last watch finishes. The free events store the time when space is freed, which is exactly the end of watch. So the maximum free_event time is the total time.
    // However, we might have started the last download and then the loop ends. The free event for that download is already pushed. So we just need to find the maximum free_event time.
    long long ans = 0;
    while (!free_events.empty()) {
        ans = max(ans, free_events.top().first);
        free_events.pop();
    }
    cout << ans << "\n";
    
    return 0;
}
