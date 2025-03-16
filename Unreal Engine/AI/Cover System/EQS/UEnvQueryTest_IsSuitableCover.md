


![[Pasted image 20250316233257.png]]

This AI code relies on a blackboard.  
See here for blackboard details.  
// TODO: Write a blackboard information document.


### **Required Blackboard Keys:**

- **Vector** - `LastSeenLocation`
- **Actor** - `Hostile`
### **Summary:**

This task node is more complex than **Close Cover** as it first checks if the cover is **crouching** or positioned to the **left** or **right** of a wall. If valid, it then assigns a score from **0 to 1** based on how effective the cover is.
### **How It Works:**

1. **FindCoverType** (look here for more info (TODO: Add a note for FindCoverType utility) )
    
    - Determines if the cover is valid based on proximity to a wall.
    - Any cover that is **not close to a wall** (left or right) or **not crouching cover** will return **false**.
    
2. **Set Cover Score (0 to 1, where 1 is best and 0 is worst)**
    
    - If the score is below **0.5**, the cover is considered **invalid**.
    - Standing cover: Higher scores are given when closer to a wall.
    - Crouching cover: Higher scores are given when slightly farther from the wall for better concealment.

---

### **Score Calculation Examples:**

#### **Example 1: Distance = 40, CoverDistanceThreshold = 120**

- **Crouching Cover Score:**

    `CoverScore = Distance / CoverDistanceThreshold;`
    **Result:** `0.33`
    
- **Standing Cover Score:**
    
    `CoverScore = (CoverDistanceThreshold - Distance) / CoverDistanceThreshold;`
    **Result:** `0.67`
---

#### **Example 2: Distance = 90, CoverDistanceThreshold = 120**

- **Crouching Cover Score:**

    `CoverScore = Distance / CoverDistanceThreshold;`
    **Result:** `0.75`
    
- **Standing Cover Score:**
  
    `CoverScore = (CoverDistanceThreshold - Distance) / CoverDistanceThreshold;`
    **Result:** `0.25`
    


### **Find Player Location Function**

Unlike other cover queries, **FindCoverType** doesn’t just perform a line trace to the player’s location—it specifically traces to the **head**. This prevents the AI from mistakenly tracing to the player's feet.

To achieve this, the function attempts to find the **"head"** bone. However, since not all character models use the same bone structure, if the "head" bone isn’t found, it defaults to a standard line trace to the **player’s location**, which typically aligns with the torso. This isn’t perfect but works well in most cases.

Additionally:

- If the enemy’s location is **unknown** (e.g., they disappeared from view), the function will trace to their **last seen location**.
- If, for some reason, **both** of these fail (which should never happen since at least one should always be valid), it falls back to using the **forward vector** to prevent the game from crashing.
  
  