
To integrate **EQS** into a **Behavior Tree**, simply add a **"Run EQS Query"** node. This node executes the query, evaluates potential cover locations, and selects the best one.

- It can choose **the best option** or pick randomly from the **top 25 or 50** best results.
- The selected location is then stored in the **Blackboard**.

In this case, the result is assigned to:  
**`Vector - CoverLocation`**

**Image**
![[Pasted image 20250316233750.png]]