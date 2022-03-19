// Nikita Kitov. All rights reserved


#include "STUAIPerceptionComponent.h"

#include "AIController.h"
#include "STUHealthComponent.h"
#include "Perception/AISense_Sight.h"

AActor* USTUAIPerceptionComponent::GetClosestEnemy() const
{
	TArray<AActor*> PerceivedActors;
	GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), PerceivedActors);

	if(!PerceivedActors.Num())
	{
		return nullptr;
	}

	const auto OwnerController = Cast<AAIController>(GetOwner());
	if(!OwnerController)
	{
		return nullptr;
	}

	const auto OwnerPawn = OwnerController->GetPawn();
	if(!OwnerPawn)
	{
		return nullptr;
	}

	float BestDistance = FLT_MAX;
	AActor* BestPawn = nullptr;
	for(const auto PerceiveActor : PerceivedActors)
	{
		const auto HealthComponent = PerceiveActor->FindComponentByClass<USTUHealthComponent>();
		if(HealthComponent && !HealthComponent->IsDead())
		{
			const auto CurrentDistance = FVector::DistSquared(PerceiveActor->GetActorLocation(), OwnerPawn->GetActorLocation());
			if(CurrentDistance < BestDistance)
			{
				BestDistance = CurrentDistance;
				BestPawn = PerceiveActor;
			}
		}
	}

	return BestPawn;
}
