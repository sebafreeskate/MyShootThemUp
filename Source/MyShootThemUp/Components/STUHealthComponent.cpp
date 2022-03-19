// Nikita Kitov. All rights reserved


#include "STUHealthComponent.h"

DEFINE_LOG_CATEGORY(LogHealthComponent)

USTUHealthComponent::USTUHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void USTUHealthComponent::UpdateLastDamageTime()
{
	LastDamageTime = GetWorld()->GetTimeSeconds();
}

bool USTUHealthComponent::TryHeal(float HealAmount)
{
	if (IsDead())
	{
		return false;
	}
	if(Health >= DefaultHealth)
	{
		return false;	
	}

	SetNewHealth(FMath::Min(DefaultHealth, Health+HealAmount));

	return true;
}

void USTUHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	check(DefaultHealth > 0);

	Health = DefaultHealth;

	AActor* Owner = GetOwner();
	if(Owner)
	{
		Owner->OnTakeAnyDamage.AddDynamic(this, &USTUHealthComponent::OnTakeAnyDamageHandle);
	}

	UpdateLastDamageTime();
}

void USTUHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	if(GetWorld()
		&& GetWorld()->GetTimeSeconds() - LastDamageTime > DelayBeforeRegenerationStarts
		&& Health != DefaultHealth
		&& !IsDead()
		)
	{
		SetNewHealth(Health += RegenValuePerSecond * DeltaTime);	
	}
}

void USTUHealthComponent::OnTakeAnyDamageHandle(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
                                                AController* InstigatedBy, AActor* DamageCauser)
{
	if(Damage <= 0.f || IsDead()) return;
	
	SetNewHealth(Health - Damage);
	UpdateLastDamageTime();

	if(Health == 0.f)
	{
		OnDeath.Broadcast();
	}

	PlayCameraShake();
}

void USTUHealthComponent::PlayCameraShake() const
{
	if(IsDead())
	{
		return;
	}

	const auto OwnerPawn = Cast<APawn>(GetOwner());
	if(!OwnerPawn)
	{
		return;
	}

	const auto OwnerController = OwnerPawn->GetController<APlayerController>();
	if(!OwnerController || !OwnerController->PlayerCameraManager)
	{
		return;
	}

	OwnerController->PlayerCameraManager->StartCameraShake(DamageCameraShake);
}

void USTUHealthComponent::SetNewHealth(float NewHealth)
{
	const float NextHealth = FMath::Clamp(NewHealth, 0.f, DefaultHealth);
	const float DeltaHealth = NextHealth - Health;
	Health = NextHealth;
	OnHealthChanged.Broadcast(DeltaHealth);
	UE_LOG(LogHealthComponent, Log, TEXT("Health = %f"), Health);
}

