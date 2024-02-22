
#include "ATestActor.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"

void ATestActor::BeginPlay()
{
	Super::BeginPlay();

	UAuraAbilitySystemLibrary::GetOverlayWidgetController(this);
}
