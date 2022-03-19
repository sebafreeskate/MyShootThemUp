// Nikita Kitov. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "STUBaseCharacter.h"
#include "STUAICharacter.generated.h"

/**
 * 
 */

class UBehaviorTree;

UCLASS()
class MYSHOOTTHEMUP_API ASTUAICharacter : public ASTUBaseCharacter
{
	GENERATED_BODY()
	
public:

	ASTUAICharacter(const FObjectInitializer& ObjInit);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
	UBehaviorTree* BehaviourTreeAsset;
};
