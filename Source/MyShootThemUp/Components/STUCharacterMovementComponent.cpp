// Nikita Kitov. All rights reserved


#include "STUCharacterMovementComponent.h"

#include "STUBaseCharacter.h"

float USTUCharacterMovementComponent::GetMaxSpeed() const
{
	const float MaxSpeed = Super::GetMaxSpeed();
	const ASTUBaseCharacter* Character = Cast<ASTUBaseCharacter>(GetPawnOwner());

	return Character && Character->IsInSprint() ? MaxSpeed * RunModifier : MaxSpeed;
}
