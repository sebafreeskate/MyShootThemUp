// Nikita Kitov. All rights reserved


#include "STUAnimNotifyBase.h"

void USTUAnimNotifyBase::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	STUNotifyDelegate.Broadcast(MeshComp);
}
