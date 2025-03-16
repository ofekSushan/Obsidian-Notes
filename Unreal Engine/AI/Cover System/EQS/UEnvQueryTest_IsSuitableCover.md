

The code is bellow this page if you just want it** 

**Important:** Like most of this AI code, it relies on a blackboard. 
See here for blackboard information.  
// TODO: Write a blackboard information document.

### Required Blackboard Keys:

- **Vector** - LastSeenLocation
- **Actor** - Hostile
### Summary:

this task node is alot more comlictedt then close cover

what it does is this it return a value the higer the value the more chnce the 
### Result:

The query returns **true** or **false** (no scoring).

- **True**: The cover is within an acceptable distance.
- **False**: It's either too close (inside a wall) or too far to be effective.

### How It Works:

1. The AI gets the **Hostile's** location.
    
    - If the hostile is no longer visible, it uses **LastSeenLocation** instead.
    
2. A **line trace** is performed from the query position to the hostile.
    
    - If the trace **does not hit anything**, the cover is ignored (the hostile has a clear view).
    - Otherwise, it checks if the distance is within the valid range:
    
    `if (Distance < CoverDistanceThreshold && Distance > CoverDistanceTooClose)`
    
    **Example values:**
    - `float CoverDistanceThreshold = 120.0f`
    - `float CoverDistanceTooClose = 30.0f`
    - `float Distance = FVector::Distance(HitResult.Location, ItemLocation) 
    `
    This ensures the cover is not too close but still provides good protection.

### Debugging:

Add this line inside the **for loop** to visualize the traces:

`DrawDebugLine(GetWorld(), ItemLocation, HitResult.Location, FColor::Green, false, 20.f);`
