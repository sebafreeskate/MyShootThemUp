#pragma once

#include "Animation/AnimSequenceBase.h"

class AnimUtills
{
public:
	template <class T>
	static T* FindAnimNotify(UAnimSequenceBase* Animation)
	{
		for (const auto AnimNotify : Animation->Notifies)
		{
			if (T* FoundNotify = Cast<T>(AnimNotify.Notify))
			{
				return FoundNotify;
			}
		}

		return nullptr;
	}
};
