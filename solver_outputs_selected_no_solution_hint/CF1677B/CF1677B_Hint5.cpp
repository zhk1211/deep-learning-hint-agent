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
        string s;
        cin >> s;
        int total = n * m;
        
        vector<int> col_good(m, 0);
        int good_cols = 0;
        vector<int> row_last_seen(n, -1);
        int good_rows = 0;
        
        vector<int> ans(total);
        
        for (int i = 0; i < total; ++i) {
            int col = i % m;
            if (s[i] == '1') {
                if (!col_good[col]) {
                    col_good[col] = 1;
                    ++good_cols;
                }
            }
            
            int row = i / m;
            if (s[i] == '1') {
                row_last_seen[row] = i;
            }
            
            // count good rows: a row is good if it has a '1' within the last m steps
            // we can maintain a sliding window of the last m indices
            // but simpler: for each row, check if its last seen index >= i - m + 1
            // we can maintain good_rows incrementally
            // when a row becomes good, it stays good for m steps after its last '1'
            // we can use a queue of expiration times
            // but we need O(1) per step, so we can maintain an array of "active" rows
            // Actually, we can just check if the row of the current student has a '1' in the window
            // but we need to update good_rows when a row expires
            // We can maintain an array last_seen for each row, and a counter of good rows
            // At step i, a row is good if last_seen[row] >= i - m + 1
            // We can update good_rows by checking the row that is about to leave the window
            // The row that leaves the window is the row of the student who entered m steps ago
            // But careful: rows are fixed, the window slides over the sequence of students
            // The students are placed in row-major order, so the row of the student at position i is i/m
            // The window of the last m students corresponds to the last m positions
            // A row is good if among the last m students, there is at least one '1' in that row
            // Since each row appears exactly once every m steps? No, each row appears exactly once every n*m steps? Actually, the sequence of rows is 0,0,...,0 (m times), 1,1,...,1 (m times), ...
            // So in any window of size m, each row appears at most once? No, if m > n, a window of size m can contain multiple students from the same row? Wait, the sequence is row 0 repeated m times, then row 1 repeated m times, etc. So a window of size m can span at most two different rows. Specifically, it can contain some suffix of one row and some prefix of the next row. So a row can appear at most m times consecutively, but in a window of size m, a row can appear multiple times only if the window is entirely within that row's block. So a row can be good if there is a '1' in its block within the window.
            // We can maintain good_rows by tracking the last '1' in each row and checking if it's within the last m positions.
            // At step i, we can compute good_rows by iterating over all rows? That would be O(n) per step, too slow.
            // We need an incremental approach.
            // Notice that a row becomes good when a '1' appears in it, and it stays good for exactly m steps after that '1' (including the step itself). Because after m steps, that '1' leaves the window of the last m students. However, if another '1' appears in the same row within those m steps, the expiration extends.
            // So we can maintain a queue of events: when a '1' is placed in a row, we schedule an expiration at i + m. But if another '1' appears before expiration, we can just update the expiration to the new i + m. We can keep an array exp[row] = expiration time. Initially -1. At step i, if s[i]=='1', we set exp[row] = i + m. Then good_rows is the number of rows with exp[row] > i.
            // We can maintain good_rows by: when we update exp[row], if the old exp was <= i, then good_rows increases by 1 (if it becomes > i). Also, we need to decrement good_rows when some row's exp becomes <= i. But we can just check at each step if any row expires. Since we only need to decrement when a row's exp equals i (i.e., it expires exactly at this step), we can maintain an array of lists: at time i, which rows expire? But i goes up to 1e6, we can use a vector of vectors, but total events are at most total '1's, which is up to 1e6. So we can push expiration events into a bucket array of size total + m + 1.
            // However, we can do even simpler: good_rows is the number of rows that have a '1' in the last m positions. Since the sequence of rows is periodic with period m (each row appears m times consecutively), the last m positions cover at most two rows. Specifically, if we are at position i, the last m positions are from max(0, i - m + 1) to i. The rows covered are either all the same row (if i % m >= m-1? Actually, if i % m == m-1, then the last m positions are exactly one full row). Otherwise, they cover a suffix of row (i/m - 1) and a prefix of row (i/m). So at most two rows can be good from the window. But wait, a row can be good if it has any '1' in the window. Since each row appears in a contiguous block of m positions, a row can only be in the window if its block overlaps the window. So at any time, at most two rows have any students in the last m positions. Therefore, good_rows can be at most 2! Is that true? Let's check: The students are seated in row-major order. The last m students are the most recent m students. Since each row has exactly m seats, the last m students can only come from at most two different rows: the current row and possibly the previous row. So indeed, at any moment, only the current row and the previous row can have students in the last m positions. So good_rows is simply the number of rows among these two that contain at least one '1' in the last m positions. That's a crucial observation!
            // Let's verify: Suppose n=2, m=2. Sequence of rows: 0,0,1,1. At i=0: last m=1 student: row 0. At i=1: last 2 students: rows 0,0 -> only row 0. At i=2: last 2 students: rows 0,1 -> rows 0 and 1. At i=3: last 2 students: rows 1,1 -> only row 1. So indeed, at most two rows.
            // So we can compute good_rows in O(1) by just checking the current row and the previous row (if it exists and is within the window). We need to know if there is a '1' in the current row's block that falls within the last m positions, and similarly for the previous row.
            // For a row r, its block is positions [r*m, (r+1)*m - 1]. The last m positions are [i - m + 1, i]. The overlap with row r's block is [max(r*m, i - m + 1), min((r+1)*m - 1, i)]. This row contributes to good_rows if there is at least one '1' in s within this overlap.
            // We can precompute prefix sums of '1's to answer in O(1) whether a range contains a '1'.
            // Then good_rows = (current row has '1' in window? 1 : 0) + (previous row has '1' in window? 1 : 0), but only if the previous row's block actually overlaps the window. The previous row overlaps if i - m + 1 <= (r-1)*m + m - 1, i.e., i - m + 1 <= r*m - 1, which is always true if i >= m? Actually, if i < m, there is no previous row. So we can just check.
            // But wait, what about rows before the previous row? They have no students in the last m positions because their blocks ended more than m steps ago. So they cannot be good.
            // So good_rows is simply the count of rows among {current_row, previous_row} that have a '1' in the overlapping part.
            // This is O(1) per step with prefix sums.
            
            // Now good_cols: a column is good if there is at least one '1' ever placed in that column. Because columns are never cleared; once a '1' is placed in a column, that column remains good forever. The problem says: "Denote a row or a column good if and only if there is at least one serious student in this row or column." And the students move back, but they stay in the same row and column? Wait, the movement: when a new student sits in (1,1), the others shift right, and the last column of a row moves to the first column of the next row. So students change columns! Actually, they change seats. So a column is good if at the current moment, there is at least one serious student in that column. Since students move, a column might lose its serious student. So my previous assumption that columns stay good forever is WRONG. Let's re-read carefully.
            
            // "When the i-th student enters the meeting hall, he will sit in the 1-st column of the 1-st row, and the students who are already seated will move back one seat. Specifically, the student sitting in the j-th (1<=j<=m-1) column of the i-th row will move to the (j+1)-th column of the i-th row, and the student sitting in m-th column of the i-th row will move to the 1-st column of the (i+1)-th row."
            // So the students shift along a snake-like path? Actually, it's like a queue: the new student enters at (1,1), everyone shifts one position along the row-major order. So the student who was at (1,1) moves to (1,2), etc., and the last student (at (n,m)) is pushed out? Wait, there are exactly n*m students total, so no one is pushed out; the hall is exactly full after all enter. But during the process, the number of students present is i. They occupy the first i positions in row-major order. So the seating arrangement after i students is simply: the first i students in the sequence are seated in row-major order, filling rows 1 to something. So the student who entered at step k is currently at the position corresponding to the k-th position in row-major order? No, because new students enter at the front and push everyone back. So the student who entered first ends up at the last position after all have entered. At step i, the students present are the ones who entered at steps 1..i. They occupy the first i seats in row-major order, but in reverse order of entry? Let's trace: After 1st student: seat (1,1) has student 1. After 2nd: student 2 sits at (1,1), student 1 moves to (1,2). After 3rd: student 3 at (1,1), student 2 at (1,2), student 1 at (1,3) if m>=3, else if m=2, student 1 moves to (2,1). So the students are arranged in the first i seats in row-major order, with the most recent student at (1,1) and the earliest at the i-th seat. So the sequence of students in row-major order is s_i, s_{i-1}, ..., s_1. That is, the first i seats contain the last i students of the sequence in reverse order.
            // Therefore, at step i, the set of students present is exactly the last i students in the sequence? No, it's the first i students in the sequence, but they are placed in reverse order. Wait: student 1 enters first, then student 2, etc. After i students, the seats are filled with students 1..i. The student at (1,1) is the i-th student. The student at (1,2) is the (i-1)-th student, etc. So the row-major order of seats contains students: s_i, s_{i-1}, ..., s_1. So the set of students is {1,...,i}, but their order is reversed.
            // Now, a row is good if it contains at least one serious student (s_j = '1') among the students currently in that row. A column is good if it contains at least one serious student among the students currently in that column.
            // Since the students are placed in the first i seats in row-major order, we can think of the grid as being filled row by row with the reversed prefix of s.
            // Let's define the sequence of students in row-major order as t[1..i] where t[1] = s_i, t[2] = s_{i-1}, ..., t[i] = s_1.
            // Then row r (1-indexed) contains students t[(r-1)*m + 1 .. min(r*m, i)]. Column c contains students t[c], t[c+m], t[c+2m], ... as long as the index <= i.
            // So we need to compute, for each i, the number of rows that have at least one '1' in their occupied part, plus the number of columns that have at least one '1' in their occupied part.
            // This is a dynamic problem: as i increases, we add a new student at the front (t[1] becomes s_i, and everything shifts right). So we are effectively prepending to the sequence t.
            // We need to maintain good rows and good columns efficiently.
            
            // Let's re-evaluate hints:
            // Hint 1: What happens when you enter a naughty student? -> adds a '0' at the front.
            // Hint 2: What happens when you enter a serious student? -> adds a '1' at the front.
            // Hint 3: How can we use the result of Hint 1 and Hint 2 to solve this problem?
            // Hint 4: When you enter a serious student, can you get the number of good rows and columns without knowing the original arrangement of students?
            // Hint 5: Can you get the number of good rows and columns when you enter a serious student using the number of good rows and columns just before entering the student?
            
            // Let's think about the effect of adding a student at the front.
            // The grid has n rows, m columns. At step i, we have i students in the first i positions of the row-major grid.
            // When we add a new student (s_i) at the front, all existing students shift one position to the right in row-major order. This means:
            // - The student who was at (r,c) moves to (r, c+1) if c < m, or to (r+1, 1) if c = m.
            // - The student who was at the last occupied position (i) moves to position i+1.
            // So the set of occupied positions expands by one at the end.
            // Now, how do good rows and columns change?
            // A row is good if it has at least one '1'. When we shift, a '1' might move from one row to the next (if it was at the end of a row). Also, the new student might add a '1' to row 1.
            // A column is good if it has at least one '1'. When we shift, a '1' moves from column c to c+1 (or from column m to column 1 of next row). So columns can lose '1's and gain '1's.
            // This seems complicated to track directly.
            
            // Alternative perspective: Since the students are just the reversed prefix, we can think of the grid as being filled from the end? Actually, the process is equivalent to: we have a sequence of students s_1, s_2, ..., s_{n*m}. We consider prefixes of this sequence. At step i, the students present are s_1..s_i. They are seated in the first i seats in row-major order, but with s_i at (1,1) and s_1 at the i-th seat. So the seating arrangement is exactly the reverse of the prefix.
            // Let's define a new string S' = reverse(s). Then at step i, the occupied seats contain the last i characters of S'? No, s_i is the first character of the reversed prefix. Actually, if we take the prefix s[1..i] and reverse it, we get the row-major filling. So the row-major filling is reverse(s[1..i]).
            // Let r_i = reverse(s[1..i]). Then r_i has length i. The grid is filled row by row with r_i.
            // As i increases, r_i is obtained by prepending s_i to r_{i-1}. So we are building the string r by prepending characters.
            // Now, we need to count good rows and columns in the grid filled with r_i.
            // A row is good if the substring of r_i corresponding to that row contains a '1'.
            // A column is good if the subsequence of r_i at positions c, c+m, c+2m, ... contains a '1'.
            // Since we are prepending, the positions of existing characters shift by 1. So the row and column memberships change.
            // This is equivalent to: we have an infinite grid? No.
            
            // Let's look at the hints again. They suggest that entering a serious student ('1') might have a predictable effect, and we might be able to compute the answer incrementally using the previous answer.
            // Hint 4: When you enter a serious student, can you get the number of good rows and columns without knowing the original arrangement of students?
            // Hint 5: Can you get the number of good rows and columns when you enter a serious student using the number of good rows and columns just before entering the student?
            // This implies that when a '1' is added, the change in the number of good rows/columns might be determined solely by the current state (like which rows/columns are already good) and maybe the time since the last '1' in some sense.
            
            // Let's analyze the effect of adding a '1' at the front.
            // Initially, before adding, we have a certain set of good rows and columns.
            // When we add a '1' at (1,1), all existing students shift right.
            // The new '1' makes row 1 good (if it wasn't already) and column 1 good (if it wasn't already).
            // What about other rows and columns? The shift might cause some rows/columns to lose their only '1' and thus become not good.
            // Specifically, consider a '1' that was at the end of a row (column m). It moves to the next row, column 1. So it leaves its old row and column, and enters a new row and column.
            // If that '1' was the only '1' in its old row, that row might become not good. Similarly for its old column.
            // So we need to track the number of '1's in each row and column? But there are up to 1e6 rows/columns total, and sum n*m <= 1e6, so we can afford O(n*m) overall, but we need O(1) per step.
            // We can maintain counts of '1's per row and per column. When a '1' moves from (r,m) to (r+1,1), we decrement count for row r and column m, and increment for row r+1 and column 1. Then we update good rows/columns based on whether counts become 0 or 1.
            // But wait: the shift happens for all students, not just '1's. However, only '1's affect the good counts. So we only care about the positions of '1's.
            // We can maintain the positions of all '1's currently in the grid. When we add a new student, if it's '1', we add it at (1,1). Then we shift all existing '1's: each '1' moves to the next seat in row-major order. If a '1' was at the last occupied seat (position i), it moves to position i+1 (which was empty). So we can just maintain the set of positions (indices in row-major order) of '1's. When we prepend a new student, all existing positions increase by 1. If the new student is '1', we add position 1.
            // Then we need to compute the number of rows that contain at least one '1' and columns that contain at least one '1'. The row of a position p (1-indexed) is (p-1)/m + 1. The column is (p-1)%m + 1.
            // So we can maintain an array row_cnt[1..n] and col_cnt[1..m]. Initially all 0. Good_rows = number of rows with cnt>0, Good_cols = number of cols with cnt>0.
            // When we add a new student:
            // - All existing '1's shift: their positions increase by 1. This means for each '1', its row and column might change. Specifically, if a '1' was at position p, it moves to p+1. Its row becomes (p)/m + 1, column becomes (p)%m + 1. So we would need to update row_cnt and col_cnt for the old and new positions.
            // - If the new student is '1', we add position 1.
            // Doing this for all '1's per step would be O(number of '1's) per step, which is too slow.
            // But note that the relative order of '1's doesn't change; they just all shift by 1. We can instead maintain the positions of '1's relative to the current front? Or we can think in terms of time.
            // Let's consider the time when a '1' was added. Suppose a '1' enters at step t (so s_t = '1'). At step i (i >= t), where is that '1'? Since we prepend students, the '1' that entered at step t will be at position (i - t + 1) in the row-major order? Let's check: At step t, it was at position 1. At step t+1, it shifts to position 2. At step i, it has shifted (i - t) times, so its position is 1 + (i - t) = i - t + 1. Yes! So the position of a '1' added at time t, at current time i, is p = i - t + 1.
            // Therefore, at time i, the set of '1's present are exactly those with t <= i such that s_t = '1', and their positions are i - t + 1.
            // So we don't need to simulate the shift! We can directly compute the row and column of each '1' at time i: row = ((i - t) / m) + 1, column = ((i - t) % m) + 1.
            // Then good_rows is the number of distinct rows among these positions, and good_cols is the number of distinct columns.
            // But we need to compute this for each i from 1 to n*m. We can do it incrementally.
            // As i increases by 1, all existing '1's increase their position by 1. So their row and column change according to the shift.
            // We can maintain the counts of '1's per row and per column. When i increments:
            // - For each existing '1', its position p becomes p+1. So its row might increase by 1 if it was at the end of a row (p % m == 0). Its column becomes (p % m) + 1, which cycles from m to 1 when row changes.
            // - Additionally, if s_i == '1', we add a new '1' at position 1 (row 1, column 1).
            // So we need to update row_cnt and col_cnt for:
            //   * The new '1' at (1,1) if s_i == '1'.
            //   * For each existing '1' that was at a position p with p % m == 0 (i.e., at the end of a row), it moves to the next row, column 1. So we decrement row_cnt for its old row, increment for new row; decrement col_cnt for column m, increment for column 1.
            //   * For all other existing '1's, their row stays the same, but their column increases by 1. So we decrement col_cnt for old column, increment for new column.
            // This still requires iterating over all '1's. But note that the effect on columns is uniform: every '1' shifts column by +1 (with wrap). So we can maintain the columns of '1's by just shifting a bitmask or using a cyclic offset.
            // Let's think differently: Instead of tracking absolute row and column numbers, we can track the time since the last '1' in each row/column? Or we can use the fact that the grid is filled in row-major order, and we only care about the most recent '1' in each row/column?
            
            // Consider columns: A column c is good if there is at least one '1' currently in that column. The columns are just the positions modulo m. Since all '1's shift by +1 in position each step, their column indices all increase by 1 modulo m. So the entire set of occupied columns just rotates cyclically! Specifically, if at time i the set of good columns is S, then at time i+1, the set of good columns is { (c mod m) + 1 | c in S } plus possibly column 1 if s_{i+1} == '1'. This is because every '1' moves to the next column, wrapping around. So we can maintain the set of good columns by simply rotating a bitset or keeping a count of good columns and tracking the shift.
            // Let's formalize: Let col_good[c] be whether column c has a '1'. When we increment time, all '1's move from column c to c+1 (with m -> 1). So the new col_good'[c] = col_good[c-1] (with wrap) for c > 1, and col_good'[1] = col_good[m] OR (s_i == '1'). So we can update the good columns in O(1) by just maintaining an array and a shift offset, or by using a deque. Since m can be up to 1e6, but sum n*m <= 1e6, we can afford O(m) per test case? No, per test case we have n*m steps, so we need O(1) per step. We can maintain the number of good columns by keeping an array col_cnt[c] = number of '1's in column c? But we only care about >0. Since we just rotate, we can maintain a cyclic buffer and update the count of good columns when a column becomes good or stops being good. However, a column stops being good only if all '1's leave it. But since we only add '1's and shift them, a column might lose its last '1' when that '1' moves to the next column. So we need to know if a column becomes empty. That requires knowing the number of '1's in each column. But we can't easily maintain the exact counts because when we shift, all '1's move, so the counts just rotate. So we can maintain an array col_cnt of size m, and when we shift, we conceptually rotate the array. We can do this by keeping a base offset: at time i, the column that was originally column 1 is now column (1 + (i-1)) mod m? Actually, the shift is applied to the positions. If we label columns by their absolute index, then a '1' added at time t has column = ((i - t) % m) + 1. So the column of a '1' depends on (i - t) mod m. This means that the set of good columns at time i is exactly the set of (i - t) mod m for all t <= i with s_t = '1'. So good_cols = number of distinct values of (i - t) mod m among '1's.
            // As i increases by 1, (i - t) mod m increases by 1 mod m for all existing '1's. So the set of good columns just shifts cyclically. We can maintain a boolean array good_col of size m, and a count. At each step, we shift: new_good_col[c] = good_col[(c-2+m)%m + 1]? Actually, if we 1-index columns, after shift, the '1' that was in column c moves to c+1 (or 1 if c=m). So the new good column set is: good_col'[1] = good_col[m] OR (s_i == '1'), and for c=2..m, good_col'[c] = good_col[c-1]. So we can update in O(1) by just keeping track of the element that moves from m to 1, and then shifting a pointer? We can use an array and update the count by checking if good_col[m] was true and good_col[1] becomes true, etc. But we need to do this for every step, which is O(1) per step. However, we also need to add the new '1' at column 1. So we can just maintain a deque or a circular buffer with a start index. Let's define an array col_has[0..m-1] (0-indexed). At time i, the column corresponding to index j in this array is the actual column? We can keep a shift offset. Initially, offset = 0. When we shift, offset = (offset - 1 + m) % m? Let's derive.
            // Suppose we maintain an array A[0..m-1] where A[j] = 1 if the column (j + offset) mod m + 1 has a '1'. When we shift all '1's to the next column, the new actual column for the '1' that was at A[j] becomes (j + offset + 1) mod m + 1. So we can just increment offset: offset = (offset + 1) % m. Then the new '1' added at column 1 corresponds to actual column 1, which is index (1 - 1 - offset + m) % m = (-offset + m) % m. So we set A[(-offset + m) % m] = 1. And we need to maintain the count of ones in A. When we increment offset, the set of ones in A doesn't change, only the mapping to actual columns changes. But the number of good columns is just the number of ones in A, because A represents the set of columns that have at least one '1'. Wait, is that true? A[j] indicates whether there is at least one '1' in the column that currently maps to j. Since all '1's shift together, if a column had multiple '1's, they all move to the same next column. So the property of having at least one '1' is preserved under shift, except that we might add a new '1' to column 1. So we don't need counts per column; we just need a boolean per column. Because if a column has at least one '1', after shift, the next column will have at least one '1' (the same '1's moved). The only way a column becomes empty is if its '1's move out, but they move to the next column, so the next column gains them. So the set of good columns simply rotates and possibly gains column 1. So we can maintain a bitset of size m and a count of ones. At each step:
            //   - Rotate the bitset? But rotating a bitset of size up to 1e6 per step is O(m) which is too slow.
            // We can avoid rotating by using an offset and a circular array. Let's keep an array col_good[0..m-1] where col_good[k] = 1 if the column with index k (in some fixed reference) has a '1'. We maintain a variable start = 0, meaning that the actual column 1 corresponds to index start. Initially, start = 0. When we shift, the actual column 1 now corresponds to the column that was previously actual column m. So the new actual column 1 is the old actual column m. In our array, actual column c corresponds to index (start + c - 1) % m. So old actual column m corresponds to index (start + m - 1) % m. After shift, we want actual column 1 to be that index. So we set start = (start + m - 1) % m? Let's check: If we set start = (start - 1 + m) % m, then the new actual column 1 is index start. The old actual column m was index (old_start + m - 1) % m. If new_start = old_start - 1, then new_start = old_start + m - 1 mod m. So indeed, new_start equals the index of old column m. So we can just decrement start (mod m). Then the new '1' is added to actual column 1, which is index start. So we set col_good[start] = 1, and if it was 0, we increment good_cols.
            // But wait: when we shift, does any column lose its '1'? The column that was actual column 1 becomes actual column 2. Its '1's move to column 2. So the set of columns that have '1's is exactly the same set of indices in the array, just the mapping to actual columns changes. So the number of good columns doesn't change due to the shift alone! It only changes when we add a new '1' to column 1 (if it wasn't already good). Is that true? Let's verify: Suppose we have a '1' in column 1 only. After shift, that '1' moves to column 2. So column 1 becomes empty, column 2 becomes good. The number of good columns remains 1. So indeed, the shift itself doesn't change the count of good columns. The only change in good_cols comes from the new student: if s_i == '1' and column 1 was not already good, good_cols increases by 1. But wait: what if column 1 was already good? Then adding another '1' doesn't change the count. So good_cols can only increase when we add a '1' to a column that currently has no '1'. But since the set of good columns rotates, column 1 might become empty after the shift if the previous column 1 had '1's and they moved to column 2. So we need to know if column 1 is good after the shift but before adding the new student. Actually, the new student enters and sits in column 1, and then everyone shifts? The problem says: "When the i-th student enters the meeting hall, he will sit in the 1-st column of the 1-st row, and the students who are already seated will move back one seat." So the new student takes (1,1), and the others shift. So the new student is placed first, then the shift happens? Or does the shift happen to make room? The description: "he will sit in the 1-st column of the 1-st row, and the students who are already seated will move back one seat." This implies the new student sits, and the others move back. So the new student is at (1,1) after the move? Actually, if they move back, the new student sits, and the others shift to the right. So the new student is at (1,1) and the old (1,1) moves to (1,2), etc. So the new student is included in the final arrangement at (1,1). So the order is: add new student at (1,1), then shift everyone else. So
