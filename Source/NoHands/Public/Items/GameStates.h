#pragma once

UENUM(BlueprintType)
enum class EGameState : uint8
{
	EGS_Off UMETA(DisplayName = "Off"),
	EGS_Idle UMETA(DisplayName = "Idle"),
	EGS_Playing UMETA(DisplayName = "Playing")
};

UENUM(BlueprintType)
enum class ESlotSymbols : uint8
{
	ESS_Cherry UMETA(DisplayName = "Cherry"),
	ESS_Lemon UMETA(DisplayName = "Lemon"),
	ESS_Bell UMETA(DisplayName = "Bell"),
	ESS_Watermelon UMETA(DisplayName = "Watermelon"),
	ESS_Star UMETA(DisplayName = "Star"),
	ESS_Diamond UMETA(DisplayName = "Diamond"),
	ESS_Seven UMETA(DisplayName = "Seven")
};