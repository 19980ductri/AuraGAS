
#include "DebugHeader.h"

void PrintLog(const FString& Message, const FColor Color)
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *Message);
}

void Print(const FString& Message, const FColor Color)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(0, 8.f, Color, Message);
	}
}
