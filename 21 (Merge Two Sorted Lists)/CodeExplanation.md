# 📘 Merging Two Sorted Linked Lists

This project provides a Python implementation to **merge two sorted linked lists** into one sorted linked list. It also explains how linked lists are represented in Python and demonstrates the algorithm with examples.

---

## 🔗 Linked List Representation in Python

Unlike arrays or lists in Python, **linked lists** don’t store elements contiguously in memory. Instead, they are composed of **nodes**, where each node contains:

* **`val`** → the value stored in the node
* **`next`** → a pointer (reference) to the next node in the sequence

### 🧩 Node Class

```python
class ListNode:
    def __init__(self, val=0, next=None):
        self.val = val      # Value stored in the node
        self.next = next    # Pointer to the next node (None if end)
```

### 📊 Example

To represent the linked list:

```
1 -> 2 -> 4
```

We construct it like this:

```python
node3 = ListNode(4)
node2 = ListNode(2, node3)
node1 = ListNode(1, node2)
head = node1
```

Here:

* `head` points to `1`
* `1.next` points to `2`
* `2.next` points to `4`
* `4.next` is `None`

---

## ⚙️ The Merge Algorithm

We merge two sorted linked lists (`list1` and `list2`) into a single sorted linked list.

### Approach:

1. Use a **dummy node** to simplify edge cases.
2. Compare the current nodes of both lists.
3. Append the smaller node to the result.
4. Move forward in that list.
5. Continue until one list is exhausted.
6. Attach the remainder of the other list.

### ⏱️ Complexity

* **Time Complexity:** `O(n + m)` (traverses both lists once)
* **Space Complexity:** `O(1)` (in-place merge)

---

## 💻 Implementation

```python
class Solution:
    def mergeTwoLists(self, list1: Optional[ListNode], list2: Optional[ListNode]) -> Optional[ListNode]:
        dummy = ListNode()
        tail = dummy
        while list1 and list2:
            if list1.val < list2.val:
                tail.next = list1
                list1 = list1.next
            else:
                tail.next = list2
                list2 = list2.next
            tail = tail.next
        tail.next = list1 if list1 else list2
        return dummy.next
```

---

## 🚀 Example Usage

```python
# Create first list: 1 -> 2 -> 4
l1 = ListNode(1, ListNode(2, ListNode(4)))

# Create second list: 1 -> 3 -> 4
l2 = ListNode(1, ListNode(3, ListNode(4)))

# Merge
solution = Solution()
merged = solution.mergeTwoLists(l1, l2)

# Print merged list: 1 -> 1 -> 2 -> 3 -> 4 -> 4
while merged:
    print(merged.val, end=" -> " if merged.next else "")
    merged = merged.next
```

### ✅ Output

```
1 -> 1 -> 2 -> 3 -> 4 -> 4
```

---

## 📌 Key Takeaways

* Linked lists in Python are built using classes (`ListNode`).
* Each node contains a **value** and a **reference** to the next node.
* The merge algorithm efficiently combines two sorted lists with **linear time complexity**.
* Dummy nodes are a powerful technique to simplify linked list manipulations.

---

## 🧠 Extensions & Ideas

* Implement a function to convert a Python list (`[1,2,4]`) into a linked list.
* Add a helper function to print a linked list in a clean format.
* Extend this solution to merge **k sorted linked lists** using a min-heap.

---

👨‍💻 **Author:** Implemented with Python & deep dive explanation to show full linked list capacity.
