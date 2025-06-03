// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/SlotMachine.h"
#include "Items/GameStates.h"
#include "Algo/RandomShuffle.h"

ASlotMachine::ASlotMachine()
{
	ItemName = "Slot Machine";
	Symbols.SetNum(NumOfRows);
	GameState = EGameState::EGS_Idle;

	InitializeSymbolList(Symbols, ESlotSymbols::ESS_Cherry, CherryProbabality);
	InitializeSymbolList(Symbols, ESlotSymbols::ESS_Lemon, LemonProbabality);
	InitializeSymbolList(Symbols, ESlotSymbols::ESS_Watermelon, WatermelonProbabality);
	InitializeSymbolList(Symbols, ESlotSymbols::ESS_Star, StarProbabality);
	InitializeSymbolList(Symbols, ESlotSymbols::ESS_Bell, BellProbabality);
	InitializeSymbolList(Symbols, ESlotSymbols::ESS_Diamond, DiamondProbabality);
	InitializeSymbolList(Symbols, ESlotSymbols::ESS_Seven, SevenProbabality);

	Payouts = {
		{ESlotSymbols::ESS_Seven,	  100},
		{ESlotSymbols::ESS_Diamond,	   50},
		{ESlotSymbols::ESS_Star,	   40},
		{ESlotSymbols::ESS_Bell,	   20},
		{ESlotSymbols::ESS_Watermelon, 16},
		{ESlotSymbols::ESS_Lemon,	   10},
		{ESlotSymbols::ESS_Cherry,		8}
	};

	TArray<ESlotSymbols> Shuffled = Symbols;
	Algo::RandomShuffle(Shuffled);

	for (int Col = 0; Col < NumOfLines; Col++)
	{
		Reels.Emplace(FReel(NumOfRows));
		Reels[Col].InitializeReel(Shuffled);
	}

}

void ASlotMachine::InteractAction()
{
	
	if (Bet > 0)
	{
		StartSpin();
	}
	
}

void ASlotMachine::BeginPlay()
{
	Super::BeginPlay();
	Tags.Add("Game");
}

FString ASlotMachine::SymbolToString(ESlotSymbols Symbol)
{
	switch (Symbol)
	{
		case ESlotSymbols::ESS_Cherry:		return TEXT("Cherry");
		case ESlotSymbols::ESS_Lemon:		return TEXT("Lemon");
		case ESlotSymbols::ESS_Watermelon:	return TEXT("Watermelon");
		case ESlotSymbols::ESS_Star:		return TEXT("Star");
		case ESlotSymbols::ESS_Bell:		return TEXT("Bell");
		case ESlotSymbols::ESS_Diamond:		return TEXT("Diamond");
		case ESlotSymbols::ESS_Seven:		return TEXT("Seven");
		default:							return TEXT(" ");
	}
}

void ASlotMachine::StartSpin()
{
	//Reset spin state
	SpinSteps.SetNum(NumOfLines);
	ReelShouldSpin.SetNum(NumOfLines);
	GlobalSpinTick = 0;

	for (int i = 0; i < NumOfLines; i++)
	{
		SpinSteps[i] = 0;
		ReelShouldSpin[i] = true;
	}

	GetWorld()->GetTimerManager().SetTimer(ReelSpinTimerHandle, this, &ASlotMachine::TickSpinAllReels, 0.2f, true);
}

void ASlotMachine::TickSpinAllReels()
{
	bool bAnyReelStillSpinning = false;

	for (int Col = 0; Col < NumOfLines; Col++)
	{
		if (!ReelShouldSpin[Col])
			continue;

		bAnyReelStillSpinning = true;

		//Spin this reel
		Reels[Col].ReelStep();
		SpinSteps[Col]++;

		TArray<ESlotSymbols> Visible = Reels[Col].GetVisibleWindow();

		for (int Row = 0; Row < Visible.Num(); Row++)
		{
			FString Line = FString::Printf(TEXT("Reel: %d / Row: %d / Symbol: %s"), Col, Row, *SymbolToString(Visible[Row]));

			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, Line);
		}

		//Check if reel should stop
		if (SpinSteps[Col] >= MaxSpinStepsPerReel + (Col * DelayBetweenStops))
		{
			ReelShouldSpin[Col] = false;
		}

	}

	if (!bAnyReelStillSpinning)
	{
		GetWorld()->GetTimerManager().ClearTimer(ReelSpinTimerHandle);
		OnSpinComplete();
	}

	GlobalSpinTick++;

}

void ASlotMachine::OnSpinComplete()
{
	for (int Col = 0; Col < 3; Col++)
	{
		FString Line;
		for (int Row = 0; Row < 3; Row++)
		{
			WinGrid[Row][Col] = Reels[Col].GetVisibleWindow()[Row];

			Line += SymbolToString(WinGrid[Row][Col]);

			if (Col < 2)
			{
				Line += TEXT(" | ");
			}
		}
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Blue, Line);
		
	}
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("Spin Complete!"));
	TotalPayout = CalculatePayout(Bet);
	ClearGame();
}

void ASlotMachine::InitializeSymbolList(TArray<ESlotSymbols>& OutArray, ESlotSymbols Symbol, uint8 Weight)
{
	for (int i = 0; i < Weight; i++)
	{
		OutArray.Emplace(Symbol);
	}
}

uint8 ASlotMachine::CalculatePayout(uint8 BetInserted)
{
	TotalPayout = 0;
	
	for (const TArray<FIntPoint>& Line : PayLines)
	{
		ESlotSymbols FirstSymbol = WinGrid[Line[0].X][Line[0].Y];
		bool bAllMatch = true;

		for (int i = 1; i < Line.Num(); i++)
		{
			if (WinGrid[Line[i].X][Line[i].Y] != FirstSymbol)
			{
				bAllMatch = false;
				break;
			}
		}

		if (bAllMatch)
		{
			uint8 Win = Payouts.Contains(FirstSymbol) ? Payouts[FirstSymbol] : 0;
			TotalPayout += Win * BetInserted;

			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow,
				FString::Printf(TEXT("Line win: %s x3 = %d"), *SymbolToString(FirstSymbol), Win));
		}
	}
	return TotalPayout;
}

bool ASlotMachine::IsThreeOfAKind(ESlotSymbols SymbolA, ESlotSymbols SymbolB, ESlotSymbols SymbolC)
{
	return (SymbolA == SymbolB && SymbolB == SymbolC);
}

void ASlotMachine::ClearGame()
{
	GameState = EGameState::EGS_Idle;
	Bet = 0;
}

void FReel::InitializeReel(const TArray<ESlotSymbols>& WeightedSymbols)
{

	for (int Row = 2; Row < ReelRows; Row++)
	{
		int Index = FMath::RandRange(0, WeightedSymbols.Num() - 1);
		Rows[Row] = WeightedSymbols[Index];
	}
}
