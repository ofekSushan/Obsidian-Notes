
The cover system in the **Behavior Tree** can be broken down into **four steps**:

1. **Finding Cover** – Runs an **EQS Query** to determine the best available cover location.
2. **Moving to Cover** – Uses a **Simple Move To** node to navigate the AI to the selected cover position.
3. **Getting Inside Cover** –
    - If **crouching**, plays the **crouch cover animation**.
    - If **standing**, plays the appropriate cover animation (**left or right**).
4. **Emerging from Cover** – (Requires Step 3 to be completed first)
    - Plays the **emerge animation** instead.
    - **Crouching cover:** AI stands up.
    - **Left/Right cover:** AI leans out in the appropriate direction.
      
### **Blackboard Variables**

For the **cover system** to function properly, the following **Blackboard variables** are required (**names must match exactly**):

- **`Vector - CoverLocation`** → Stores the selected cover position.
- **`Vector - LastSeenLocation`** → Stores the last known position of the hostile.
- **`Object (Actor) - Hostile`** → Stores a reference to the hostile (enemy).


![[Pasted image 20250317003506.png]]