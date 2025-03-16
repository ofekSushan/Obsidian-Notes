
The code is bellow this page if you just want it** 

**Important:** Like most of this AI code, it relies on a blackboard. 
See here for blackboard information.  
// TODO: Write a blackboard information document.

### Required Blackboard Keys:

- **Vector** - LastSeenLocation
- **Actor** - Hostile
### Summary:

This task node checks whether a cover point is too close to a wall to be viable, or too far to provide good cover.
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




image inside the envirment querry


![[Pasted image 20250316224258.png]]



CODE

#pragma once  
  
#include "CoreMinimal.h"  
#include "EnvironmentQuery/EnvQueryTest.h"  
#include "EnvQueryTest_CloseCover.generated.h"  
  
/**  
 * * IMPORTANT you need blackboard with Vector(LastSeenLocation),Actor(Hostile). * Close cover task node. * A task node that checks if you are close to the cover. */UCLASS()  
class VRPROTOTYPE_API UEnvQueryTest_CloseCover : public UEnvQueryTest  
{  
public:  
    inline static float CoverDistanceThreshold = 120.0f;  
  
    // AI can go inside the cover, which can cause issues when using move to in behavior tree.  
    inline static float CoverDistanceToClose = 30.0f;  
  
protected:  
    GENERATED_BODY()  
  
    explicit UEnvQueryTest_CloseCover(const FObjectInitializer& ObjectInitializer);  
  
    virtual void RunTest(FEnvQueryInstance& QueryInstance) const override;  
  
    virtual FText GetDescriptionTitle() const override;  
  
    virtual FText GetDescriptionDetails() const override;  
  
private:  
    FBlackboardKeySelector LastSeenLocationBlackboard;  
    FBlackboardKeySelector HostileBlackboard;  
};



EnvQueryTest_CloseCover.cpp

#include "AI/EnvQueryTests/EnvQueryTest_CloseCover.h"  
#include "AIController.h"  
#include "VRFirstPerson.h"  
#include "BehaviorTree/BlackboardComponent.h"  
#include "EnvironmentQuery/EnvQuery.h"  
#include "EnvironmentQuery/Items/EnvQueryItemType_ActorBase.h"  
#include "Math/Vector.h"  
  
  
UEnvQueryTest_CloseCover::UEnvQueryTest_CloseCover(const FObjectInitializer& ObjectInitializer) : Super(  
    ObjectInitializer)  
{  
    LastSeenLocationBlackboard.SelectedKeyName = "LastSeenLocation";  
    HostileBlackboard.SelectedKeyName = "Hostile";  
    Cost = EEnvTestCost::High;  
    ValidItemType = UEnvQueryItemType_VectorBase::StaticClass();  
    SetWorkOnFloatValues(false);  
}  
  
FText UEnvQueryTest_CloseCover::GetDescriptionTitle() const  
{  
    return FText::FromString(FString(TEXT("Close Cover")));  
}  
  
FText UEnvQueryTest_CloseCover::GetDescriptionDetails() const  
{  
    return FText::FromString(  
       FString(TEXT("IMPORTANT you need blackboard with Vector(LastSeenLocation),Actor(Hostile).")));  
}  
  
void UEnvQueryTest_CloseCover::RunTest(FEnvQueryInstance& QueryInstance) const  
{  
    if (ACharacter* Character = Cast<ACharacter>(QueryInstance.Owner.Get()))  
    {       if (AAIController* AIController = Cast<AAIController>(Character->GetController()))  
       {          FCollisionQueryParams CollisionParams(FName(TEXT("PlayerVisibilityTest")), true, Character);  
          if (UBlackboardComponent* Blackboard = AIController->GetBlackboardComponent())  
          {             FVector PlayerLocation;  
             if (AActor* Hostile = Cast<AActor>(Blackboard->GetValueAsObject(HostileBlackboard.SelectedKeyName)))  
             {                PlayerLocation = Hostile->GetActorLocation();  
             }             else  
             {  
                PlayerLocation = Blackboard->GetValueAsVector(  
                   LastSeenLocationBlackboard.SelectedKeyName);  
             }             FHitResult HitResult;  
  
             for (FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It)  
             {                bool IsCover = false;  
                const FVector ItemLocation = GetItemLocation(QueryInstance, It.GetIndex());  
                GetWorld()->LineTraceSingleByChannel(HitResult, ItemLocation,  
                                                     PlayerLocation,                                                     ECC_Visibility, CollisionParams);  
                if (HitResult.bBlockingHit)  
                {                   float Distance = FVector::Distance(HitResult.Location, ItemLocation);  
                   if (Distance < CoverDistanceThreshold && Distance > CoverDistanceToClose)  
                   {                      IsCover = true;  
                   }                }                It.SetScore(TestPurpose, FilterType, IsCover, true);  
             }          }       }    }}



