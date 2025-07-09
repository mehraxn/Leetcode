class Solution:
    def maxFreeTime(self, eventTime: int, k: int, startTime: List[int], endTime: List[int]) -> int:
        n = len(startTime)
        gaps = [startTime[0]] + [startTime[i] - endTime[i - 1] for i in range(1, n)] + [eventTime - endTime[-1]]
        L = k + 1
        pref = 0
        d = deque([(0, 0)])
        ans = 0
        for i in range(1, n + 2):
            pref += gaps[i - 1]
            while d and d[0][0] < i - L:
                d.popleft()
            ans = max(ans, pref - d[0][1])
            while d and d[-1][1] >= pref:
                d.pop()
            d.append((i, pref))
        return ans
