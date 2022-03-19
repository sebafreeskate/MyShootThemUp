// Nikita Kitov. All rights reserved


#include "STUBaseCharacter.h"


#include "STUBaseWeapon.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/TextRenderComponent.h"
#include "MyShootThemUp/Components/STUCharacterMovementComponent.h"
#include "MyShootThemUp/Components/STUHealthComponent.h"
#include "STUWeaponComponent.h"
#include "Components/CapsuleComponent.h"
#include "AnimUtills.h"
#include "STUAnimNotifyDeath.h"
#include "DrawDebugHelpers.h"

// Sets default values
ASTUBaseCharacter::ASTUBaseCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<USTUCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->bUsePawnControlRotation = true;
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);

	HealthComponent = CreateDefaultSubobject<USTUHealthComponent>("HealthComponent");

	TextRenderComponent = CreateDefaultSubobject<UTextRenderComponent>("TextRenderComponent");
	TextRenderComponent->SetupAttachment(GetMesh());
	TextRenderComponent->SetOwnerNoSee(true);

	WeaponComponent = CreateDefaultSubobject<USTUWeaponComponent>("WeaponComponent");
}

float ASTUBaseCharacter::GetDirectionAngleDegrees() const
{
	if(GetVelocity().IsNearlyZero()) return 0;
	
	const auto ForwardVector = GetActorForwardVector();

	//DrawDebugDirectionalArrow(GetWorld(), GetActorLocation(), GetActorLocation()+ForwardVector*200, 3.f, FColor::Blue);
	
	const auto VelocityVector = GetVelocity().GetSafeNormal();

	//DrawDebugDirectionalArrow(GetWorld(), GetActorLocation(), GetActorLocation()+VelocityVector*200, 3.f, FColor::Green);

	const float AngleBetween = FMath::RadiansToDegrees( FMath::Acos(FVector::DotProduct(ForwardVector, VelocityVector)));
	const auto CrossProduct = FVector::CrossProduct(ForwardVector, VelocityVector);

	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, FString::Printf(TEXT("%f"), AngleBetween));

	return CrossProduct.IsZero() ? AngleBetween : AngleBetween * FMath::Sign(CrossProduct.Z);
}

// Called when the game starts or when spawned
void ASTUBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	check(HealthComponent);
	check(TextRenderComponent);
	check(GetMovementComponent())
	check(GetMesh())
	
	DisableSprint();

	HealthComponent->OnHealthChanged.AddUObject(this, &ASTUBaseCharacter::UpdateHealthText);
	HealthComponent->OnDeath.AddDynamic(this, &ASTUBaseCharacter::OnDeath);

	UpdateHealthText(HealthComponent->GetHealth());

	LandedDelegate.AddDynamic(this, &ASTUBaseCharacter::OnGroundLanded);

	auto DeathFinishNotify = AnimUtills::FindAnimNotify<USTUAnimNotifyDeath>(DeathMontage);
	if(DeathFinishNotify)
	{
		DeathFinishNotify->STUNotifyDelegate.AddDynamic(this, &ASTUBaseCharacter::EnableRagdoll);
	}
}

// Called every frame
void ASTUBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(!CanSprint())
	{
		DisableSprint();
	}
}

// Called to bind functionality to input
void ASTUBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASTUBaseCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASTUBaseCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &ASTUBaseCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("TurnAround", this, &ASTUBaseCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASTUBaseCharacter::Jump);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ASTUBaseCharacter::EnableSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ASTUBaseCharacter::DisableSprint);

	check(WeaponComponent)
	PlayerInputComponent->BindAction("Fire", IE_Pressed, WeaponComponent, &USTUWeaponComponent::StartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, WeaponComponent, &USTUWeaponComponent::StopFire);
	PlayerInputComponent->BindAction("NextWeapon", IE_Pressed, WeaponComponent, &USTUWeaponComponent::NextWeapon);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, WeaponComponent, &USTUWeaponComponent::OnReloadPressed);
}

void ASTUBaseCharacter::MoveForward(float Amount)
{
	if(Amount == 0.f) return;
	
	AddMovementInput(GetActorForwardVector(), Amount);
}

void ASTUBaseCharacter::MoveRight(float Amount)
{
	if(Amount == 0.f) return;

	AddMovementInput(GetActorRightVector(), Amount);
}

bool ASTUBaseCharacter::CanSprint() const
{
	const float DirectionAngle = GetDirectionAngleDegrees();
	
	return	!GetMovementComponent()->IsFalling()
		&& FMath::Abs(DirectionAngle) < 40.f;
}

void ASTUBaseCharacter::EnableSprint()
{
	bInSprint = CanSprint();
}

void ASTUBaseCharacter::DisableSprint()
{
	bInSprint = false;
}

void ASTUBaseCharacter::UpdateHealthText(float Health) const
{
	TextRenderComponent->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), HealthComponent->GetHealth())));
}

void ASTUBaseCharacter::EnableRagdoll(USkeletalMeshComponent* SkeletalMesh) 
{
	if (SkeletalMesh == GetMesh())
	{
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		GetMesh()->SetSimulatePhysics(true);
	}
}

void ASTUBaseCharacter::OnDeath()
{
	GetCharacterMovement()->DisableMovement();
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;

	PlayAnimMontage(DeathMontage);

	CorpseLifespan = 5.f;
	SetLifeSpan(CorpseLifespan);

	Controller->ChangeState(NAME_Spectating);

	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
}

void ASTUBaseCharacter::OnGroundLanded(const FHitResult& HitResult)
{
	float VelocityFalling = GetVelocity().Size();
	float DamageToTake = FMath::GetMappedRangeValueClamped(FallVelocityRange, FallDamageRange, VelocityFalling);
	//TakeDamage(DamageToTake, FDamageEvent{}, nullptr, nullptr);
}

