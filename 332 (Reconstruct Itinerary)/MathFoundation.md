# Reconstructing an Itinerary – Mathematical Foundations and Full Walk‑through

---

## Table of Contents

1. [Introduction](#introduction)
2. [Formal Problem Statement](#formalproblem)
3. [Graph–Theoretic Modelling](#graphmodel)
4. [Existence of an Eulerian Path](#existence)
5. [Hierholzer’s Algorithm in Depth](#hierholzer)
6. [Ensuring Lexicographically Smallest Path](#lexicographic)
7. [Data‑Structure Design Choices](#datastructures)
8. [Step‑by‑Step Execution on Example 1](#example1)
9. [Proof of Correctness](#correctness)
10. [Complexity Analysis](#complexity)
11. [Edge Cases & Special Scenarios](#edgecases)
12. [Relation to Classic Results](#relations)
13. [Glossary of Terms](#glossary)
14. [References](#references)

<a name="introduction"></a>

## 1  Introduction

The **Reconstruct Itinerary** problem (LeetCode 332) asks us to recover an itinerary that:

* Uses every airline ticket exactly once.
* Always begins at the airport `JFK`.
* Among all valid itineraries, returns the one with the *smallest lexical order* when read as a single comma‑separated string.

This README presents every mathematical concept required to understand and prove the provided Python implementation **without any external libraries**. We proceed meticulously, leaving no conceptual gaps.

<a name="formalproblem"></a>

## 2  Formal Problem Statement

Given a multiset

$$
   T = \{(o_i, d_i) \mid 1 \le i \le m\}
$$

where each pair $(o_i,d_i)$ represents a *directed* edge (ticket) from origin airport $o_i$ to destination airport $d_i$, construct an ordered sequence of airports
$P = [v_0, v_1, \dots, v_m]$
such that:

1. $v_0 = \texttt{"JFK"}$.
2. For every index $j\in[0,m-1]$ there exists exactly one ticket in $T$ whose ordered pair equals $(v_j, v_{j+1})$, and each ticket from $T$ is used **exactly once**.
3. Among all such sequences, choose the one whose string representation
   $  S(P) = v_0\operatorname{\small\frown}v_1\operatorname{\small\frown}\dots\operatorname{\small\frown}v_m$
   is lexicographically minimal under the usual dictionary order on ASCII strings (ties break at the earliest differing airport code).

This sequence $P$ is returned as the list of airport codes.

<a name="graphmodel"></a>

## 3  Graph–Theoretic Modelling

### 3.1  Directed Multigraph

Define a **directed multigraph** $G = (V,E)$ where

* $V$ is the set of unique airport codes appearing in any ticket.
* $E$ contains a separate *arc* for every occurrence of a ticket. Multiple identical arcs are permitted (hence *multi*‑graph).

### 3.2  Degrees

For each vertex $v\in V$:

* **Out‑degree** $\deg^+(v)$ is the number of tickets that depart from $v$.
* **In‑degree**  $\deg^-(v)$ is the number of tickets that arrive at $v$.

### 3.3  Trails and Eulerian Trails

A **trail** is a walk in which no edge is repeated. An **Eulerian trail** (a.k.a. *Euler path*) is a trail containing **every** edge of the graph exactly once. If it starts and ends at the same vertex it is an **Eulerian circuit**.

Because every ticket must be used exactly once, the itinerary corresponds to an Eulerian trail in $G$. The starting vertex is predetermined (`JFK`), so we seek an Eulerian trail *rooted* at `JFK`.

<a name="existence"></a>

## 4  Existence of an Eulerian Path

### 4.1  Necessary and Sufficient Conditions

For a directed graph, an **Eulerian trail** exists **iff** exactly one of the following holds:

1. **Circuit Case.** Every vertex satisfies $\deg^+(v) = \deg^-(v)$. Then an Eulerian *circuit* exists from every vertex.
2. **Path Case.** Exactly one vertex $s$ has $\deg^+(s) = \deg^-(s)+1$ (called *start vertex*), exactly one vertex $t$ has $\deg^-(t) = \deg^+(t)+1$ (called *end vertex*), and every other vertex satisfies equality. Then an Eulerian *trail* exists from $s$ to $t$.

*(Proof Sketch).* The proof relies on decomposing the edge set into cycles and stitching them (see Fleury 1883 and Hierholzer 1873), or applying flow conservation principles.

### 4.2  Our Instance

LeetCode’s guarantee (“you may assume all tickets form at least one valid itinerary”) implies the instance meets one of the above criteria **with** start vertex `JFK`. Specifically, `JFK` plays role $s$. If no vertex appears with a net +1 out‑degree except `JFK`, the end vertex is some $t$ with net +1 in‑degree or equal in/out when the path returns.

<a name="hierholzer"></a>

## 5  Hierholzer’s Algorithm in Depth

Hierholzer (1873) originally provided an algorithm for finding Eulerian circuits. We adapt it for trails.

### 5.1  Canonical Description

1. **Start at** the designated vertex (`JFK`).
2. **Follow edges** arbitrarily (in our variant, according to lexical rules) until you reach a vertex with no unused outgoing edges. You have formed a *dead‑end*.
3. **Backtrack**: When stuck, push the vertex onto a route list and pop the stack to the previous vertex with remaining unused outgoing edges. Continue exploring.
4. **Repeat** until all edges have been used.
5. **Reverse** the collected route to obtain the Eulerian trail.

Hierholzer’s key insight is that when you exhaust all outgoing edges of a vertex, that vertex’s final position in the Eulerian trail is *fixed*. Thus, appending it to an output list during backtracking yields the trail in reverse.

### 5.2  Stack‑Based Non‑Recursive Implementation

Instead of recursion we use an explicit **stack** `s`:

* Push the current vertex.
* While the top has unused children, push one such child and *consume* that edge.
* If no child remains, pop and append to route.

Because each edge is traversed exactly once in *forward* direction during exploration and never reconsidered, the total time is $O(|E|)$ plus the cost of selecting children.

<a name="lexicographic"></a>

## 6  Ensuring Lexicographically Smallest Path

Choosing children “arbitrarily” will not satisfy the lexical requirement. We enforce it by always selecting the smallest possible next airport code **among the remaining edges**.

### 6.1  Strategy Without External Heaps

Typical solutions employ a min‑heap (`heapq`). To avoid external libraries, we instead:

1. Build an adjacency list `d[v]` as **a list sorted in descending order**.
2. While exploring, use `list.pop()` which removes the **last** element in $O(1)$. Because the list is descending, popping returns the **lexicographically smallest available** child.

> *Why does this work?* Sorting descending once ($O(k\log k)$ per vertex) preserves global order; after that, each delivery of the smallest element is constant time. The descending trick thus simulates a priority queue.

### 6.2  Relationship to Short‑Lex Order

Our choice of smallest child at every divergence yields the *lexicographically minimal Eulerian trail* because Hierholzer’s traversal explores edges in precisely that order. Formal justification uses induction over the concrete string obtained by concatenation.

<a name="datastructures"></a>

## 7  Data‑Structure Design Choices

| Symbol | Meaning                                                        | Python Representation  | Complexity       |   |      |   |                   |
| ------ | -------------------------------------------------------------- | ---------------------- | ---------------- | - | ---- | - | ----------------- |
| `d`    | adjacency mapping: vertex → list of children sorted descending | `dict[str, list[str]]` | build: (O(       | E | \log | E | )) due to sorting |
| `s`    | exploration stack                                              | `list[str]`            | push/pop: $O(1)$ |   |      |   |                   |
| `r`    | output route in reverse order                                  | `list[str]`            | append: $O(1)$   |   |      |   |                   |

All auxiliary space is $O(|V| + |E|)$.

<a name="example1"></a>

## 8  Step‑by‑Step Execution on Example 1

**Tickets**

```
["MUC","LHR"], ["JFK","MUC"], ["SFO","SJC"], ["LHR","SFO"]
```

### 8.1  Preparation

| Airport | Children (after descending sort) |
| ------- | -------------------------------- |
| JFK     | `["MUC"]`                        |
| MUC     | `["LHR"]`                        |
| LHR     | `["SFO"]`                        |
| SFO     | `["SJC"]`                        |

### 8.2  Traversal Trace

| Step | Stack `s` (top right)   | Route `r`               | Explanation                   |
| ---- | ----------------------- | ----------------------- | ----------------------------- |
| 0    | `[JFK]`                 | `[]`                    | start                         |
| 1    | `[JFK,MUC]`             | `[]`                    | pop `MUC` from `d[JFK]`       |
| 2    | `[JFK,MUC,LHR]`         | `[]`                    | pop `LHR` from `d[MUC]`       |
| 3    | `[JFK,MUC,LHR,SFO]`     | `[]`                    | pop `SFO` from `d[LHR]`       |
| 4    | `[JFK,MUC,LHR,SFO,SJC]` | `[]`                    | pop `SJC` from `d[SFO]`       |
| 5    | `[JFK,MUC,LHR,SFO]`     | `[SJC]`                 | `SJC` dead‑end, move to route |
| 6    | `[JFK,MUC,LHR]`         | `[SJC,SFO]`             | `SFO` now dead‑end            |
| 7    | `[JFK,MUC]`             | `[SJC,SFO,LHR]`         | etc.                          |
| 8    | `[JFK]`                 | `[SJC,SFO,LHR,MUC]`     |                               |
| 9    | `[]`                    | `[SJC,SFO,LHR,MUC,JFK]` |                               |

### 8.3  Final Itinerary

Reverse `r` ⇒ `[JFK,MUC,LHR,SFO,SJC]` which matches the expected answer.

<a name="correctness"></a>

## 9  Proof of Correctness

We must show that the algorithm returns **(i)** a valid itinerary, **(ii)** uses every ticket exactly once, and **(iii)** is lexicographically minimal.

### 9.1  Validity & Completeness

*Invariant 1.* *All edges removed from `d` are appended to the stack exactly once and hence traversed exactly once.*

Proof by construction: an edge `(u,v)` is consumed exclusively when `v` is pushed via `d[u].pop()`. That edge never reappears because it is deleted from the list.

*Invariant 2.* *A vertex is appended to `r` precisely when all outgoing edges from that vertex have been processed.*

Therefore when the algorithm terminates, every edge has been consumed and appended to `r` through backtracking. Reversing `r` yields a trail containing all edges once—an Eulerian trail—starting at `JFK` because `JFK` is the first element visited and the last appended to `r`.

### 9.2  Lexicographic Minimality

Consider two valid Eulerian trails $P_{alg}$ (our output) and $P_{alt}$ (any other). Let $k$ be the first position where the airport codes differ. Up to position $k-1$ both follow identical edges. They diverge at a vertex $v$ where multiple unused outgoing edges were available. Our algorithm chose the **smallest** alphabetically. Consequently `P_alg[k] < P_alt[k]`, making `P_alg` the lexicographically smaller string. Contradiction if `P_alt` were smaller. ∎

<a name="complexity"></a>

## 10  Complexity Analysis

* **Sorting Phase:** For each vertex with $d_i$ outgoing edges, sorting costs $O(d_i\log d_i)$. Summing over vertices: $O(\sum d_i\log d_i) \le O(|E|\log |E|)$.
* **Traversal Phase:** Each edge is popped once and pushed once ⇒ $O(|E|)$.
* **Total:** $O(|E|\log |E|)$ time, $O(|V|+|E|)$ space.

If auxiliary heaps were allowed, we could trade initial sorting cost for $O(|E|\log |V|)$ dynamic selection; both are asymptotically similar.

<a name="edgecases"></a>

## 11  Edge Cases & Special Scenarios

* **Multiple Identical Tickets:** Handled naturally because adjacency lists store duplicates.
* **Graph Forms an Eulerian Circuit:** Condition 2 in §4 works; algorithm returns to `JFK`.
* **Single Ticket:** Sorting & traversal still behave; route length 2.
* **Disconnected Graph:** Impossible under problem guarantee; otherwise algorithm would visit only the reachable component.

<a name="relations"></a>

## 12  Relation to Classic Results

* **Euler (1736):** Laid foundational work in the Königsberg bridges problem (undirected circuits).
* **Hierholzer (1873):** First published constructive circuit algorithm for directed graphs.
* **Tarjan (1972):** Discussed ear decomposition and strongly connected components relevant to Eulerian conditions.

<a name="glossary"></a>

## 13  Glossary of Terms

| Term                    | Definition                                            |
| ----------------------- | ----------------------------------------------------- |
| **Vertex (Node)**       | A distinct airport code.                              |
| **Edge (Arc)**          | A single ticket (ordered pair).                       |
| **Multigraph**          | Graph permitting multiple parallel edges.             |
| **Trail**               | Walk with no repeated edges.                          |
| **Eulerian Trail**      | Trail containing every edge exactly once.             |
| **Lexicographic Order** | Dictionary order on strings with ASCII values.        |
| **Stack**               | LIFO data structure; here used to simulate recursion. |

<a name="references"></a>

## 14  References

1. Euler, L. *Solutio problematis ad geometriam situs pertinentis*, 1736.
2. Hierholzer, C. *Über die Möglichkeit, einen Linienzug ohne Wiederholung und ohne Unterbrechung zu umfahren*, 1873.
3. West, D. B. *Introduction to Graph Theory*, 2nd ed., Prentice Hall, 2001.
4. LeetCode Problem 332: *Reconstruct Itinerary*.

---

**End of Document**
