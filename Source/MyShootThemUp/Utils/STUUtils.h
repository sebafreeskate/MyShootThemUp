#pragma once

template<class T>
T* GetPawnComponent(APawn* Pawn)
{
	if(!Pawn)
	{
		return nullptr;
	}

	const auto Comp= Pawn->GetComponentByClass(T::StaticClass());
	return Cast<T>(Comp);
}

