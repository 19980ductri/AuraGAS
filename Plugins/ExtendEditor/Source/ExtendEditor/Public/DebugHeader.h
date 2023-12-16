#pragma once

#include "Misc/MessageDialog.h"
#include "Widgets/Notifications/SNotificationList.h"
#include "Framework/Notifications/NotificationManager.h"

inline void Print(const FString& Message, const FColor Color)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 8.f, Color, Message);
	}
	UE_LOG(LogTemp, Warning, TEXT("%s"), *Message);
}

inline EAppReturnType::Type ShowMessageDialog(EAppMsgType::Type MsgType, const FString& Message,
                                              bool bShowMessageAsWarning = true)
{
	if (bShowMessageAsWarning == false)
	{
		FMessageDialog::Open(MsgType, FText::FromString(Message));
		return EAppReturnType::Cancel;
	}
	FText MessageTitle = FText::FromString(TEXT("Warning"));
	return FMessageDialog::Open(MsgType, FText::FromString(Message), &MessageTitle);
}
inline void ShowNotifyInfo(const FString& Message)
{
	FNotificationInfo NotifyInfo(FText::FromString(Message));
	NotifyInfo.bUseLargeFont = true;
	NotifyInfo.FadeOutDuration = 7.f;

	FSlateNotificationManager::Get().AddNotification(NotifyInfo);
}