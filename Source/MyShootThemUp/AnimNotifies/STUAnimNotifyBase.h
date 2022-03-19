// Nikita Kitov. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "STUAnimNotifyBase.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSTUNotify, USkeletalMeshComponent*, MeshComponent);

UCLASS()
class MYSHOOTTHEMUP_API USTUAnimNotifyBase : public UAnimNotify
{
	GENERATED_BODY()

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	
public:

	UPROPERTY()
	FSTUNotify STUNotifyDelegate;
	
};
