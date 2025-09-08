// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ACDPlayerHUDWidget.h"

void UACDPlayerHUDWidget::ShowToastMessage(const FText& Message, float DisplaySeconds /*= 1.2f*/)
{
	BP_OnShowToastMessage(Message, DisplaySeconds);
}

void UACDPlayerHUDWidget::UpdateQuestProgress(const FText& Title, int32 CurrentCount, int32 TargetCount)
{
	BP_OnUpdateQuestProgress(Title, CurrentCount, TargetCount);
}

void UACDPlayerHUDWidget::SetInteractionPrompt(const FText& PromptText, bool bIsVisible)
{
	BP_OnSetInteractionPrompt(PromptText, bIsVisible);
}

void UACDPlayerHUDWidget::OpenInventory()
{
	BP_OpenInventory();
}

void UACDPlayerHUDWidget::UpdateInventoryUI(const TArray<FACDInventoryItem>& Items)
{
	BP_OnUpdateInventoryUI(Items);
}

