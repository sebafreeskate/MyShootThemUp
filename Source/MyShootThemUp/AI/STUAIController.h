// Nikita Kitov. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "STUAIController.generated.h"

/**
 * 
 */

class USTUAIPerceptionComponent;

UCLASS()
class MYSHOOTTHEMUP_API ASTUAIController : public AAIController
{
	GENERATED_BODY()

	ASTUAIController();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USTUAIPerceptionComponent* STUPerceptionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FName FocusOnKeyName = "EnemyActor";

	virtual void OnPossess(APawn* InPawn) override;

	virtual void Tick(float DeltaSeconds) override;

private:
	AActor* GetFocusOnActor() const;
};
