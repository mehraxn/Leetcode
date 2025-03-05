# Greedy Algorithm for the Lemonade Change Problem

## **Introduction**
The "Lemonade Change" problem requires us to determine whether we can provide correct change to every customer in a queue, given that customers pay using $5, $10, or $20 bills. We must return `true` if we can give every customer the correct change and `false` otherwise.

To efficiently solve this problem, we use a **greedy algorithm**. A greedy algorithm makes a series of choices, each of which looks best at the moment, to arrive at an optimal solution.

---

## **Algorithm Explanation**

### **1. Understanding the Problem Constraints**
- Each customer buys one lemonade costing **$5**.
- Customers may pay using **$5, $10, or $20** bills.
- We must provide **exact change** using only the bills we have collected so far.
- Initially, we **do not have any money**.
- If we cannot give correct change to a customer, we return `false`.

### **2. Observing the Transactions**
For every bill a customer gives, we need to handle it properly:
- **$5 bill**: No change is required. Just collect it.
- **$10 bill**: Requires **one $5 bill** as change.
- **$20 bill**: Requires **$15 in change**, which can be given as either:
  1. One **$10 bill + one $5 bill** (preferred).
  2. Three **$5 bills** (if a $10 bill is not available).

If we do not have enough change, we return `false`.

---

## **3. Greedy Approach to Solving the Problem**

The greedy strategy for this problem is:
- **Always give the highest denomination change first**.
- **Track only the necessary denominations** ($5 and $10 bills) because $20 bills are only used for payments.
- **Update our cash holdings dynamically as we process customers**.

We maintain two counters:
- `five`: Number of **$5 bills** available.
- `ten`: Number of **$10 bills** available.

---

## **4. Step-by-Step Algorithm Execution**
1. **Initialize counters:** `five = 0`, `ten = 0`.
2. **Iterate through the list of bills:**
   - If the customer **pays with $5**, increase `five` count.
   - If the customer **pays with $10**:
     - Check if a **$5 bill** is available.
     - If available, give **$5 change** and update the counts (`five--`, `ten++`).
     - If no $5 bill is available, return `false` (we cannot provide correct change).
   - If the customer **pays with $20**:
     - Try to give **one $10 bill and one $5 bill** (better strategy to preserve $5 bills).
     - If no $10 bill is available, try to give **three $5 bills**.
     - If neither option is possible, return `false`.
3. **If we successfully serve all customers, return `true`**.

---

## **5. Example Walkthrough**

### ✅ **Example 1:**
#### **Input:**
```c
bills = [5, 5, 5, 10, 20]
```
#### **Execution:**
1. Customer pays **$5** → No change needed → We now have **[$5]**
2. Customer pays **$5** → No change needed → We now have **[$5, $5]**
3. Customer pays **$5** → No change needed → We now have **[$5, $5, $5]**
4. Customer pays **$10** → Give **$5 change** → We now have **[$5, $5, $10]**
5. Customer pays **$20** → Give **$10 + $5 change** → We now have **[$5, $10]**

✅ **We were able to provide correct change to all customers. Return `true`**.

---

### ❌ **Example 2:**
#### **Input:**
```c
bills = [5, 5, 10, 10, 20]
```
#### **Execution:**
1. Customer pays **$5** → No change needed → We now have **[$5]**
2. Customer pays **$5** → No change needed → We now have **[$5, $5]**
3. Customer pays **$10** → Give **$5 change** → We now have **[$5, $10]**
4. Customer pays **$10** → Give **$5 change** → We now have **[$10, $10]**
5. Customer pays **$20** → Needs **$15 change**, but we only have two **$10 bills**, no $5 bills.

❌ **We cannot provide correct change. Return `false`**.

---

## **6. Code Implementation (C)**

```c
#include <stdbool.h>

bool lemonadeChange(int* bills, int billsSize) {
    int five = 0, ten = 0; // Track count of $5 and $10 bills
    
    for (int i = 0; i < billsSize; i++) {
        if (bills[i] == 5) {
            five++;  // Collect $5 bill
        } else if (bills[i] == 10) {
            if (five == 0) return false; // Need one $5 to give change
            five--;  // Give $5 change
            ten++;   // Collect $10 bill
        } else { // bills[i] == 20
            if (ten > 0 && five > 0) {
                ten--; // Give one $10
                five--; // Give one $5
            } else if (five >= 3) {
                five -= 3; // Give three $5 bills
            } else {
                return false; // Not enough change
            }
        }
    }
    
    return true; // All customers received correct change
}
```

---

## **7. Time and Space Complexity**
- **Time Complexity:** `O(n)`, since we iterate through the `bills` array once.
- **Space Complexity:** `O(1)`, since we use only two integer variables (`five` and `ten`).

---

## **8. Why the Greedy Algorithm Works?**
- **Always give the highest denomination change first** → ensures more flexibility later.
- **Maintains a minimal set of variables (`five` and `ten`)** → ensures efficient tracking.
- **Ensures that each decision optimally preserves change for future customers.**

---

## **Conclusion**
This **greedy algorithm** is the best way to solve the **Lemonade Change** problem efficiently. By always making the best local choice at each step (giving the highest change possible first), we ensure that we can serve the maximum number of customers while keeping track of our available bills. 🚀

