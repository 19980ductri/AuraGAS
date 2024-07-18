//

#include "Player/AuraPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "EnhancedInputSubsystems.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "Actor/MagicCircle.h"
#include "Aura/Aura.h"
#include "Aura/AuraLogChannels.h"
#include "Components/DecalComponent.h"
#include "Components/SplineComponent.h"
#include "Input/AuraInputComponent.h"
#include "Interaction/EnemyInterface.h"
#include "GameFramework/Character.h"
#include "Interaction/HighlightInterface.h"
#include "UI/Widget/DamageTextComponent.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
	Spline = CreateDefaultSubobject<USplineComponent>("Spline");
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	CursorTrace();
	AutoRun();
	UpdateMagicCircleLocation();
}

void AAuraPlayerController::ShowMagicCircle(UMaterialInterface* DecalMaterial)
{
	if (!IsValid(MagicCircle))
	{
		MagicCircle = GetWorld()->SpawnActor<AMagicCircle>(MagicCircleClass);
		if (DecalMaterial)
		{
			MagicCircle->MagicCircleDecal->SetMaterial(0, DecalMaterial);
		}
	}
}

void AAuraPlayerController::HideMagicCircle()
{
	if (IsValid(MagicCircle))
	{
		UE_LOG(LogAura, Warning, TEXT("Destroying Magic Circle"));
		MagicCircle->Destroy();
	}
}

void AAuraPlayerController::ShowDamageNumber_Implementation(float DamageAmount, ACharacter* TargetCharacter, bool bBlockedHit, bool bCriticalHit)
{
	if (IsValid(TargetCharacter) && DamageTextComponentClass && IsLocalController())
	{
		UDamageTextComponent* DamageText = NewObject<UDamageTextComponent>(TargetCharacter, DamageTextComponentClass);
		DamageText->RegisterComponent();
		DamageText->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		DamageText->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		DamageText->SetDamageText(DamageAmount, bBlockedHit, bCriticalHit);
	}
}

void AAuraPlayerController::AutoRun()
{
	if (bAutoRunning == false) return;
	
	if (APawn* ControlledPawn = GetPawn())
	{
		const FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation(ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
		const FVector Direction = Spline->FindDirectionClosestToWorldLocation(LocationOnSpline, ESplineCoordinateSpace::World);
		ControlledPawn->AddMovementInput(Direction);

		//const float DistanceToDestination = (CachedDestination - LocationOnSpline).Length();
		//
		DrawDebugSphere(GetWorld(), LocationOnSpline, 5, 10.0f, FColor::Red, false, 2.0f, 0, 5.0f);
		
		float DistanceToDestination = FVector::Dist(LocationOnSpline, CachedDestination);
		UE_LOG(LogAura, Warning, TEXT("Distance to destination: %f"), DistanceToDestination);
		if (DistanceToDestination <= AutoRunAcceptanceRadius)
		{
			bAutoRunning = false;
			
		}
	}
	
	
}

void AAuraPlayerController::UpdateMagicCircleLocation()
{
	if (IsValid(MagicCircle))
	{
		MagicCircle->SetActorLocation(CursorHit.ImpactPoint);
	}
}

void AAuraPlayerController::HighlightActor(AActor* InActor)
{
	if (IsValid(InActor) && InActor->Implements<UHighlightInterface>())
	{
		IHighlightInterface::Execute_HighlightActor(InActor);
	}
}

void AAuraPlayerController::UnHighlightActor(AActor* InActor)
{
	if (IsValid(InActor) && InActor->Implements<UHighlightInterface>())
	{
		IHighlightInterface::Execute_UnHighlightActor(InActor);
	}
}

void AAuraPlayerController::CursorTrace()
{
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_CursorTrace))
	{
		UnHighlightActor(LastActor);
		UnHighlightActor(ThisActor);
		if (IsValid(ThisActor) && ThisActor->Implements<UHighlightInterface>())

		LastActor = nullptr;
		ThisActor = nullptr;
		return;
	}
	const ECollisionChannel TraceChannel = IsValid(MagicCircle) ? ECC_ExcludePlayers : ECC_Visibility;
	GetHitResultUnderCursor(TraceChannel, false, CursorHit);
	if (!CursorHit.bBlockingHit) return;

	LastActor = ThisActor;
	if (IsValid(CursorHit.GetActor()) && CursorHit.GetActor()->Implements<UHighlightInterface>())
	{
		ThisActor = CursorHit.GetActor();
	}
	else
	{
		ThisActor = nullptr;
	}

	if (LastActor != ThisActor)
	{
		UnHighlightActor(LastActor);
		HighlightActor(ThisActor);
	}
}

void AAuraPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_InputPressed))
	{
		return;
	}
	if (InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		if (IsValid(ThisActor))
		{
			TargetingStatus = ThisActor->Implements<UEnemyInterface>() ? ETargetingStatus::TargetingEnemy : ETargetingStatus::TargetingNonEnemy;
		}
		else
		{
			TargetingStatus = ETargetingStatus::NotTargeting;
		}
		bAutoRunning = false;
	}
	if (GetASC()) GetASC()->AbilityInputTagPressed(InputTag);
}

void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (GetASC() == nullptr)
	{
		return;
	}
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_InputReleased))
	{
		return;
	}
	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		if (GetASC()) GetASC()->AbilityInputTagReleased(InputTag);
		return;
	}

	if (GetASC()) GetASC()->AbilityInputTagReleased(InputTag);
	const APawn* ControlledPawn = GetPawn();
	if (TargetingStatus != ETargetingStatus::TargetingEnemy && !bShiftKeyDown)
	{
		UE_LOG(LogAura,Warning, TEXT("Not Targeting"))
		if (FollowTime <= ShortPressThreshold && ControlledPawn)
		{
			if (IsValid(ThisActor) && ThisActor->Implements<UHighlightInterface>())
			{
				IHighlightInterface::Execute_SetMoveToLocation(ThisActor, CachedDestination);
			}
			else if (GetASC() && !GetASC()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_InputPressed))
			{
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ClickNiagaraSystem, CachedDestination);
			}
			
			if (UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this,ControlledPawn->GetActorLocation(),CachedDestination))
			{
				if (NavPath->PathPoints.Num() > 1)
				{
					Spline->ClearSplinePoints();
					for (const FVector& PointLoc : NavPath->PathPoints)
					{
						Spline->AddSplinePoint(PointLoc, ESplineCoordinateSpace::World);
						//DrawDebugSphere(GetWorld(), Spline, 30.f, 2, FColor::Blue, false, 20.f);
					}
					CachedDestination = NavPath->PathPoints[NavPath->PathPoints.Num() - 1];
					DrawDebugSphere(GetWorld(), CachedDestination, 30.f, 2, FColor::Cyan, false, 20.f);
					bAutoRunning = true;
					
				}
			}
		}
		FollowTime = 0.f;
		TargetingStatus = ETargetingStatus::NotTargeting;
	}
	else if (TargetingStatus == ETargetingStatus::TargetingEnemy)
	{
		if (FollowTime <= ShortPressThreshold)
		{
			if (FGameplayAbilitySpec* Spec = GetASC()->GetSpecWithSlot(InputTag))
			{
				UAuraGameplayAbility* Ability = Cast<UAuraGameplayAbility>(Spec->Ability);
				if (IsValid(Ability) && IsValid(ThisActor))
				{
					if (UAuraAbilitySystemLibrary::IsInCastRange(this, ThisActor->GetActorLocation(),
					ControlledPawn->GetActorLocation(), Ability->CastRange))
					{
						UE_LOG(LogAura, Warning, TEXT("Targeting enemy in cast range"));
						GetASC()->AbilityInputTagReleased(InputTag);
					}
					else
					{
						UE_LOG(LogAura, Warning, TEXT("Targeting Enemy but not in range yet"))
						FVector AttackableLocation = UAuraAbilitySystemLibrary::FindTargetAttackableLocation(this, ThisActor->GetActorLocation(),
						GetPawn()->GetActorLocation() ,Ability->CastRange);
						
						if (ControlledPawn)
						{
							UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this,
								ControlledPawn->GetActorLocation(),
								AttackableLocation);
							if (NavPath && NavPath->PathPoints.Num() > 0)
							{
								Spline->ClearSplinePoints();
								for (const FVector& PointLoc : NavPath->PathPoints)
								{
									DrawDebugSphere(GetWorld(), PointLoc, 30.f, 2,FColor::Blue, false, 20.f);
									Spline->AddSplinePoint(PointLoc, ESplineCoordinateSpace::World);
								}
								
								CachedDestination = NavPath->PathPoints[NavPath->PathPoints.Num() - 1];
								
								DrawDebugSphere(GetWorld(), CachedDestination, 10.f, 2, FColor::Green, false, 20.f);
								
								DrawDebugSphere(GetWorld(), AttackableLocation, 10, 10.0f, FColor::Red, false, 2.0f, 0, 5.0f);
								bAutoRunning = true;
							}
						}
					}
				}
			}
		}
	}
}
void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	
	if (GetASC() == nullptr)
	{
		return;
	}
	
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_InputHeld))
	{
		return;
	}
	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		if (GetASC()) GetASC()->AbilityInputTagHeld(InputTag);
		return;
	}
	
	if (TargetingStatus == ETargetingStatus::TargetingEnemy || bShiftKeyDown)
	{
		if (FGameplayAbilitySpec* Spec = GetASC()->GetSpecWithSlot(InputTag))
		{
			UAuraGameplayAbility* Ability = Cast<UAuraGameplayAbility>(Spec->Ability);
			if (IsValid(Ability) && IsValid(ThisActor))
			{
				if (bool bInRange = UAuraAbilitySystemLibrary::IsInCastRange(this, ThisActor->GetActorLocation(),
					GetPawn()->GetActorLocation(), Ability->CastRange))
				{
					GetASC()->AbilityInputTagHeld(InputTag);
				}
				else
				{
					//TODO: implement a function that make character go to cast ran
					FVector AttackableLocation = UAuraAbilitySystemLibrary::FindTargetAttackableLocation(this, ThisActor->GetActorLocation(),
						GetPawn()->GetActorLocation() ,Ability->CastRange);
					if (APawn* ControlledPawn = GetPawn())
					{
						// Calculate path to the attackable location
						UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this,
							ControlledPawn->GetActorLocation(),
							AttackableLocation);
        
						if (NavPath && NavPath->PathPoints.Num() > 0)
						{
							// Clear existing spline points and add new ones
							Spline->ClearSplinePoints();
							for (const FVector& PointLoc : NavPath->PathPoints)
							{
								Spline->AddSplinePoint(PointLoc, ESplineCoordinateSpace::World);
							}
            
							// Set the destination and start auto-running
							CachedDestination = AttackableLocation;
							bAutoRunning = true;
						}
					}
					
				}
			}
		}
	}
	else
	{
		FollowTime += GetWorld()->GetDeltaSeconds();
		if (CursorHit.bBlockingHit)
		{
			CachedDestination = CursorHit.ImpactPoint;
		}
		if (APawn* ControlledPawn = GetPawn())
		{
			const FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
			ControlledPawn->AddMovementInput(WorldDirection);
		}
	}
}

UAuraAbilitySystemComponent* AAuraPlayerController::GetASC()
{
	if (AuraAbilitySystemComponent == nullptr)
	{
		AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return AuraAbilitySystemComponent;
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(AuraContext);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem)
	{
		Subsystem->AddMappingContext(AuraContext, 0);
	}

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UAuraInputComponent* AuraInputComponent = CastChecked<UAuraInputComponent>(InputComponent);
	AuraInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
	AuraInputComponent->BindAction(ShiftAction, ETriggerEvent::Started, this, &AAuraPlayerController::ShiftPressed);
	AuraInputComponent->BindAction(ShiftAction, ETriggerEvent::Completed, this, &AAuraPlayerController::ShiftReleased);
	AuraInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_InputPressed))
	{
		return;
	}
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}
