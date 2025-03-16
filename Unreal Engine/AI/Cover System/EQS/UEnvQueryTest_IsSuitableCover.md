

### **Important:**

This AI code relies on a blackboard.  
See here for blackboard details.  
// TODO: Write a blackboard information document.

### **Required Blackboard Keys:**

- **Vector** - `LastSeenLocation`
- **Actor** - `Hostile`
### **Summary:**

This task node is more complex than **Close Cover** as it assigns a score rather than a simple pass/fail. The higher the score, the better the cover.

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
    



now find cover type is diffrent because not like the other we dont just try to do i line trace to a location of the player we are doing a line trace to the head so he cant try to do a line trace to the feet we are doing it with the look for ("head") bone now of corse not every one has the exsect bone stracture witha "head" bone spaficly but if not it will just a simple line trace to the location of the player witch most of the time will be in the torso witch is good but not parfect 

and of corse if he dosnt know the enmy locatino since he disbred he will just do it for the last seen location 

or even if somewhy both are not working witch sould never heppends since he cant get inside this 