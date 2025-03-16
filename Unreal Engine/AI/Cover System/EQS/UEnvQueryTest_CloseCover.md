

the code is bellow this page if you jsut want it 

IMPotent like most of this ai code it releis on blackbord see here for black bord infomation
// todo write a black bord infomation losser



this spasficly needs
 (and yes the string mast be the same )

Vector - LastSeenLocation
Actor - Hostile




summry this task node is for checking if the cover points is to close for not trying to go inside one because its inside of a wall not to far so the cover wotent be that good from the start 


result - the quriey is a true or false no score so if its close engoth not to far yes not to clsoe it getts a pass 




how it works 

the ai gets the location of the hostile or if hotile is none (if the player cant see him anymore)
it takes the last seen location 






code
EnvQueryTest_CloseCover.h

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



