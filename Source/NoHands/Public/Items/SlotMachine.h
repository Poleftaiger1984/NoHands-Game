// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Playable.h"
#include "SlotMachine.generated.h"

enum class ESlotSymbols : uint8;

USTRUCT()
struct FReel
{
	GENERATED_BODY()

public:
	FReel() {}
	FReel(uint8 NumOfRows) : ReelRows(NumOfRows)
	{ 
		Rows.SetNum(ReelRows); 
		ReelStartIndex = 0;
	}

	void InitializeReel(const TArray<ESlotSymbols>& WeightedSymbols);

private:

	uint8 ReelRows;	
	uint8 ReelStartIndex;
	TArray<ESlotSymbols> Rows;

public:
	FORCEINLINE TArray<ESlotSymbols> GetReelSymbols() const { return Rows; }
	FORCEINLINE int GetReelSize() const { return Rows.Num(); }
	FORCEINLINE ESlotSymbols GetSymbolAt(int Index) const { return Rows[Index % GetReelSize()]; }
	FORCEINLINE int GetStartIndex() const { return ReelStartIndex; }
	FORCEINLINE TArray<ESlotSymbols> GetVisibleWindow() const { return { GetSymbolAt(ReelStartIndex), GetSymbolAt(ReelStartIndex + 1), GetSymbolAt(ReelStartIndex + 2) }; }
	FORCEINLINE void ReelStep() { ReelStartIndex = (ReelStartIndex + 1) % GetReelSize(); }
};

UCLASS()
class NOHANDS_API ASlotMachine : public APlayable
{
	GENERATED_BODY()
	
public:
	ASlotMachine();

	virtual void InteractAction() override;

protected:
	virtual void BeginPlay() override;

	FString SymbolToString(ESlotSymbols ReelSymbols);

	void StartSpin();
	void TickSpinAllReels();
	void OnSpinComplete();
	void InitializeSymbolList(TArray<ESlotSymbols>& OutArray, ESlotSymbols Symbol, uint8 Weight);

	uint8 CalculatePayout(uint8 BetInserted);
	bool IsThreeOfAKind(ESlotSymbols SymbolA, ESlotSymbols SymbolB, ESlotSymbols SymbolC);

	void ClearGame();

private:
	//Slot basic settings and defaults

	UPROPERTY(VisibleAnywhere, Category = "Game Settings | Defaults")
	uint8 NumOfLines = 3;

	UPROPERTY(EditAnywhere, Category = "Game Settings | Defaults")
	uint8 NumOfRows = 16; // !!To keep probabilities the same if you plan to alter the number of rows aka the reel length change the probabilties too!!

	UPROPERTY(EditAnywhere, Category = "Game Settings | Defaults")
	uint8 MinimumSpins = 30;

	UPROPERTY(EditAnywhere, Category = "Game Settings | Defaults")
	uint8 MaximumSpins = 50;

	UPROPERTY(EditAnywhere, Category = "Game Settings | Defaults")
	float DelayBetweenStops = 10.f;

	UPROPERTY(VisibleAnywhere, Category = "Game Settings | Defaults")
	TArray<uint8> SpinSteps;

	TArray<bool> ReelShouldSpin;
	uint8 GlobalSpinTick = 0;
	uint8 MaxSpinStepsPerReel = FMath::RandRange(MinimumSpins, MaximumSpins);

	TArray<FReel> Reels;
	TArray<ESlotSymbols> Symbols;
	FTimerHandle ReelSpinTimerHandle;

	//Symbol Weights 
	
	UPROPERTY(EditAnywhere, Category = "Game Settings | Probabilities")
	uint8 CherryProbabality = 3;

	UPROPERTY(EditAnywhere, Category = "Game Settings | Probabilities")
	uint8 LemonProbabality = 3;

	UPROPERTY(EditAnywhere, Category = "Game Settings | Probabilities")
	uint8 WatermelonProbabality = 3;

	UPROPERTY(EditAnywhere, Category = "Game Settings | Probabilities")
	uint8 StarProbabality = 2;

	UPROPERTY(EditAnywhere, Category = "Game Settings | Probabilities")
	uint8 BellProbabality = 2;

	UPROPERTY(EditAnywhere, Category = "Game Settings | Probabilities")
	uint8 DiamondProbabality = 2;
	
	UPROPERTY(EditAnywhere, Category = "Game Settings | Probabilities")
	uint8 SevenProbabality = 2;

	//Payouts

	TArray<TArray<FIntPoint>> PayLines = {
		{{0, 0}, {0, 1}, {0, 2}}, //Top Row
		{{1, 0}, {1, 1}, {1, 2}}, //Middle Row
		{{2, 0}, {2, 1}, {2, 2}}  //Bottom Row
	};

	ESlotSymbols WinGrid[3][3];
	TMap<ESlotSymbols, uint8> Payouts;

};
