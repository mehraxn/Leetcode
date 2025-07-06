# Reconstruct Itinerary – Algorithm Explanation

## Problem Recap

You are given airline **tickets** where each ticket `tickets[i] = [from, to]` represents a directed flight from `from` → `to`.
All tickets belong to a traveller who always **starts from `"JFK"`**.
You must build an itinerary that:

1. **Uses every ticket exactly once** (i.e., forms an *Eulerian path* through the graph), and
2. Among all possible valid itineraries, returns the one with the **smallest lexical order** when the airport codes are concatenated.

---

## Core Idea

The task is equivalent to finding an **Eulerian path** in a directed graph, starting at `JFK`, with an additional *lexicographical‑min* tie‑breaker.

We apply **Hierholzer’s algorithm**, a classic method for traversing every edge exactly once, and tweak it to respect lexicographical order:

> *Always follow the lexicographically smallest unused outgoing edge first.*

---

## Step‑by‑Step Walk‑through

| Step | Action                                                                                                                                                                                                                                                                                                                                                                 | Purpose                                                                                                                      |
| ---- | ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ---------------------------------------------------------------------------------------------------------------------------- |
| 1    | **Build an adjacency list** `d` mapping each departure airport to a list of arrival airports.                                                                                                                                                                                                                                                                          | Represent the graph efficiently.                                                                                             |
| 2    | **Sort each adjacency list in *reverse* lexicographical order.**                                                                                                                                                                                                                                                                                                       | Allows us to `pop()` from the *end* of the list and still retrieve the next smallest airport in `O(1)` time.                 |
| 3    | **Initialize** a `stack` with `['JFK']` and an empty `route` list.                                                                                                                                                                                                                                                                                                     | `stack` tracks our current DFS path; `route` will collect the final itinerary in reverse.                                    |
| 4    | **Traverse the graph (Hierholzer’s loop)** <br>While the `stack` is not empty: <br>   • While the airport on top of the `stack` has outgoing edges left in `d`, push the next destination (obtained via `pop()`) onto the `stack`. <br>   • When an airport has **no remaining outgoing edges**, it is *dead‑ended*; pop it from the `stack` and append it to `route`. | First inner loop goes **deeper** along unused edges. When stuck, we **backtrack**, adding airports to `route` in post‑order. |
| 5    | **Reverse `route`.**                                                                                                                                                                                                                                                                                                                                                   | The backtracking order is the reverse of the required itinerary.                                                             |

### Why This Works

* Hierholzer’s algorithm guarantees every edge (ticket) is used **exactly once**.
* Because we always remove the *smallest* candidate edge first, the constructed Eulerian path is the lexicographically minimal one.

---

## Complexity Analysis

| Aspect    | Cost                                                                                                   |
| --------- | ------------------------------------------------------------------------------------------------------ |
| **Time**  | `O(E · log E)` for sorting adjacency lists (where `E` = number of tickets) + `O(E)` for the traversal. |
| **Space** | `O(V + E)` to store the graph and the recursion stack, reducible to `O(E)` since `E ≥ V`.              |

---

## Code‑to‑Concept Mapping

```python
class Solution:
    def findItinerary(self, tickets: List[List[str]]) -> List[str]:
        d = {}
        for a, b in tickets:              # ➊ build adjacency
            d.setdefault(a, []).append(b)

        for k in d:                       # ➋ sort reverse
            d[k].sort(reverse=True)

        route = []
        stack = ['JFK']                   # ➌ start DFS from JFK

        while stack:                      # ➍ Hierholzer traversal
            while stack[-1] in d and d[stack[-1]]:
                stack.append(d[stack[-1]].pop())
            route.append(stack.pop())     # backtrack dead‑end

        return route[::-1]                # ➎ reverse to final order
```

* **Lines 4‑6** — *Adjacency construction.*
* **Lines 8‑9** — Sort each list in **descending** order so that `pop()` yields the **smallest** destination.
* **Lines 11‑17** — Hierholzer’s depth‑first traversal with inline lexicographical control.
* **Line 17** — Final reversal gives the correct itinerary.

---

## Key Takeaways

* The itinerary forms an **Eulerian path** in the flight graph.
* **Hierholzer’s algorithm** efficiently constructs it in linear time after sorting.
* Sorting each adjacency list *once* and popping from the tail combines algorithmic correctness with lexicographical minimality.

Happy coding ✈️
