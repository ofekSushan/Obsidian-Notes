#include "AI/EnvQueryTests/EnvQueryTest_IsSuitableCover.h"
#include "AIController.h"
#include "AI/Utils/AIEnums.h"
#include "AI/EnvQueryTests/EnvQueryTest_CloseCover.h"
#include "AI/Utils/UCoverUtils.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnvironmentQuery/EnvQuery.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_ActorBase.h"
#include "GameFramework/Character.h"
#include "Math/Vector.h"


UEnvQueryTest_IsSuitableCover::UEnvQueryTest_IsSuitableCover(const FObjectInitializer& ObjectInitializer) : Super(
	ObjectInitializer)
{
	LastSeenLocationBlackboard.SelectedKeyName = "LastSeenLocation";
	HostileBlackboard.SelectedKeyName = "Hostile";
	CoverDistanceThreshold = UEnvQueryTest_CloseCover::CoverDistanceThreshold;
	Cost = EEnvTestCost::High;
	ValidItemType = UEnvQueryItemType_VectorBase::StaticClass();
}

FText UEnvQueryTest_IsSuitableCover::GetDescriptionTitle() const
{
	return FText::FromString(FString(TEXT("Suitable Cover")));
}

FText UEnvQueryTest_IsSuitableCover::GetDescriptionDetails() const
{
	return FText::FromString(
		FString(TEXT("IMPORTANT you need blackboard with Vector(LastSeenLocation),Actor(Hostile).")));
}

void UEnvQueryTest_IsSuitableCover::RunTest(FEnvQueryInstance& QueryInstance) const
{
	UObject* QueryOwner = QueryInstance.Owner.Get();
	if (QueryOwner == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Faild To find QueryOwner"));
		return;
	}
	FloatValueMin.BindData(QueryOwner, QueryInstance.QueryID);
	float MinThresholdValue = FloatValueMin.GetValue();

	FloatValueMax.BindData(QueryOwner, QueryInstance.QueryID);
	float MaxThresholdValue = FloatValueMax.GetValue();

	if (ACharacter* Character = Cast<ACharacter>(QueryInstance.Owner.Get()))
	{
		if (AAIController* AIController = Cast<AAIController>(Character->GetController()))
		{
			if (UBlackboardComponent* Blackboard = AIController->GetBlackboardComponent())
			{
				FVector PlayerLocation = FindPlayerLocation(Blackboard, HostileBlackboard.SelectedKeyName,
				                                            LastSeenLocationBlackboard.SelectedKeyName, Character,
				                                            BoneToSearchFor);

				for (FEnvQueryInstance::ItemIterator EQSItem(this, QueryInstance); EQSItem; ++EQSItem)
				{
					// In low cover the score is higher the farther it is from the cover. (For easy movement)
					// In high cover it is the opposite, the closer he is to the cover the higher the score. (Stands with the back to the wall)
					float CoverScore = 0;
					FCollisionQueryParams CollisionParams(FName(TEXT("PlayerVisibilityTest")), false,
					                                      Character);
					FVector ItemLocation = GetItemLocation(QueryInstance, EQSItem.GetIndex());
					FHitResult HitResult;
					float Distance;
					
					switch (UCoverUtils::FindCoverType(GetWorld(), ItemLocation, PlayerLocation, CollisionParams,
					                                   false))
					{
					case EAICoverState::EACS_CoverCrouching:
						GetWorld()->LineTraceSingleByChannel(HitResult, ItemLocation, PlayerLocation,
						                                     ECC_Visibility,
						                                     CollisionParams);
						Distance = FVector::Distance(HitResult.Location, ItemLocation);
						
						// Get the score for most distance for example (40/50=0.8 || 10/50=0.2)
						CoverScore = Distance / CoverDistanceThreshold;
						break;

					// Doing both cases because it doesn't matter what side.
					case EAICoverState::EACS_CoverRight:
					case EAICoverState::EACS_CoverLeft:
						GetWorld()->LineTraceSingleByChannel(HitResult, ItemLocation, PlayerLocation,
						                                     ECC_Visibility,
						                                     CollisionParams);
						Distance = FVector::Distance(HitResult.Location, ItemLocation);

						// Get the smallest distance for example ((50-40)/50=0.2 || (50-10)/50=0.8)
						CoverScore = (CoverDistanceThreshold - Distance) / CoverDistanceThreshold;
						break;

					default: ;
					}
					EQSItem.SetScore(TestPurpose, FilterType, CoverScore, MinThresholdValue, MaxThresholdValue);
				}
			}
		}
	}
}

FVector UEnvQueryTest_IsSuitableCover::FindPlayerLocation(const UBlackboardComponent* Blackboard,
                                                          const FName HostileBlackboardName,
                                                          const FName LastSeenLocationBlackboardName,
                                                          const ACharacter* Character, const FName BoneToSearchFor)
{
	if (const AActor* Hostile = Cast<AActor>(Blackboard->GetValueAsObject(HostileBlackboardName)))
	{
		FVector PlayerCurrentLocation = Hostile->GetActorLocation();

		// If the player has a bone to search for (head) - it will focus on that bone. 
		if (const USkeletalMeshComponent* SkeletalMeshComp = Hostile->FindComponentByClass<
			USkeletalMeshComponent>(); SkeletalMeshComp && SkeletalMeshComp->DoesSocketExist(
			BoneToSearchFor))
		{
			PlayerCurrentLocation = SkeletalMeshComp->GetSocketLocation(BoneToSearchFor);
		}
		return PlayerCurrentLocation;
	}

	// If the player lost the enemy - it will return his last seen location.
	if (Blackboard->IsVectorValueSet(LastSeenLocationBlackboardName))
	{
		return Blackboard->GetValueAsVector(LastSeenLocationBlackboardName);
	}
	
	// If there is a problem and it cant find both the enemy and last seen location - it will return the forward vector because he is probably in front of him.
	// This is never supposed to happen but just in case.
	const FVector StartLocation = Character->GetActorLocation();
	const FVector ForwardVector = Character->GetActorForwardVector();
	return StartLocation + (ForwardVector * 100.0f);
}
