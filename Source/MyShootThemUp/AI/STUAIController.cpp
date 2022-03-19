// Nikita Kitov. All rights reserved


#include "STUAIController.h"

#include "STUAICharacter.h"
#include "STUAIPerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

ASTUAIController::ASTUAIController()
{
	STUPerceptionComponent = CreateDefaultSubobject<USTUAIPerceptionComponent>(TEXT("STUPerceptionComponent"));
	SetPerceptionComponent(*STUPerceptionComponent);
}

void ASTUAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if(const auto STUCharacter = Cast<ASTUAICharacter>(InPawn))
	{
		RunBehaviorTree(STUCharacter->BehaviourTreeAsset);
	}
}

void ASTUAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	const auto AimActor = GetFocusOnActor();
	SetFocus(AimActor);
}

AActor* ASTUAIController::GetFocusOnActor() const
{
	if(!GetBlackboardComponent()) 
	{
		return nullptr;
	}

	return Cast<AActor>(GetBlackboardComponent()->GetValueAsObject(FocusOnKeyName));
}
