// Fill out your copyright notice in the Description page of Project Settings.


#include "Items//Games/SlotMachine.h"
#include "Items/GameStates.h"
#include "Algo/RandomShuffle.h"
#include "LatentActions.h"
#include "Components/SlotInterfaceComponent.h"

ASlotMachine::ASlotMachine()
{
	ItemName = "Slot Machine";
	WeightedSymbolPool.SetNum(NumOfRows);
	WinRow.SetNum(3);
	GameState = EGameState::EGS_Idle;

	SlotInterface = CreateDefaultSubobject<USlotInterfaceComponent>(TEXT("Slot Interface"));
	SlotInterface->SetupAttachment(GetRootComponent());

	InitializeSymbolList(WeightedSymbolPool, ESlotSymbols::ESS_Cherry, NumOfCherries);
	InitializeSymbolList(WeightedSymbolPool, ESlotSymbols::ESS_Lemon, NumOfLemons);
	InitializeSymbolList(WeightedSymbolPool, ESlotSymbols::ESS_Watermelon, NumOfWatermelons);
	InitializeSymbolList(WeightedSymbolPool, ESlotSymbols::ESS_Star, NumOfStars);
	InitializeSymbolList(WeightedSymbolPool, ESlotSymbols::ESS_Bell, NumOfBells);
	InitializeSymbolList(WeightedSymbolPool, ESlotSymbols::ESS_Diamond, NumOfDiamonds);
	InitializeSymbolList(WeightedSymbolPool, ESlotSymbols::ESS_Seven, NumOfSevens);

	Payouts = {
		{ESlotSymbols::ESS_Seven,	  100},
		{ESlotSymbols::ESS_Diamond,	   50},
		{ESlotSymbols::ESS_Star,	   40},
		{ESlotSymbols::ESS_Bell,	   20},
		{ESlotSymbols::ESS_Watermelon, 10},
		{ESlotSymbols::ESS_Lemon,	    8},
		{ESlotSymbols::ESS_Cherry,      2}
	};

	BaseProbabilities = {
		{ESlotSymbols::ESS_Cherry,		0.25f},
		{ESlotSymbols::ESS_Lemon,	    0.18f},
		{ESlotSymbols::ESS_Watermelon,  0.16f},
		{ESlotSymbols::ESS_Bell,	    0.14f},
		{ESlotSymbols::ESS_Star,		0.12f},
		{ESlotSymbols::ESS_Diamond,		0.10f},
		{ESlotSymbols::ESS_Seven,		0.05f},
	};

	Algo::RandomShuffle(WeightedSymbolPool);

	for (int Col = 0; Col < NumOfLines; Col++)
	{
		Reels.Emplace(FReel(NumOfRows));
		Reels[Col].InitializeReel(WeightedSymbolPool);
	}

}

void ASlotMachine::InteractAction()
{
	if (Bet > 0)
	{
		GameState = EGameState::EGS_Playing;
	}		
	
	if (GameState == EGameState::EGS_Playing)
	{
		GeneratePredeterminedWin();
	}

	if (GameState == EGameState::EGS_GameDone)
	{
		GameState = EGameState::EGS_Idle;
		TotalPayout = 0;
	}
}

void ASlotMachine::SetBet(const int32 PlayerBet)
{
	Bet = PlayerBet;
}

void ASlotMachine::SetPlayerLuck(const int32 PlayerLuck)
{
	Luck = PlayerLuck;
	NormalizedPlayerLuck = FMath::Clamp(Luck / 10.f, 0.0f, 1.0f);
}

int32 ASlotMachine::GetWinnings() const
{
	return TotalPayout;
}

EGameState ASlotMachine::GetGameState() const
{
	return GameState;
}

void ASlotMachine::BeginPlay()
{
	Super::BeginPlay();
	Tags.Add("Game");
	Tags.Add("HoldInteract");
}

uint8 ASlotMachine::RollWin()
{
	NoWinChance = 0.8f - (NormalizedPlayerLuck * 0.6f); //For Luck = 10 -> 20% no win chance

	float Roll = FMath::RandRange(0.0f, 1.f);
	if (Roll < NoWinChance) 
		return 0;
	else 
		return 3;
}

ESlotSymbols ASlotMachine::RollPredeterminedSymbol()
{
	TMap<ESlotSymbols, float> AdjustedProbabilities = BaseProbabilities;

	AdjustedProbabilities[ESlotSymbols::ESS_Seven]		+= NormalizedPlayerLuck * 0.10f;
	AdjustedProbabilities[ESlotSymbols::ESS_Diamond]	+= NormalizedPlayerLuck * 0.08f;
	AdjustedProbabilities[ESlotSymbols::ESS_Star]		+= NormalizedPlayerLuck * 0.07f;

	AdjustedProbabilities[ESlotSymbols::ESS_Watermelon] -= NormalizedPlayerLuck * 0.05f;
	AdjustedProbabilities[ESlotSymbols::ESS_Lemon]		-= NormalizedPlayerLuck * 0.07f;
	AdjustedProbabilities[ESlotSymbols::ESS_Cherry]		-= NormalizedPlayerLuck * 0.10f;

	for (auto& Pair : AdjustedProbabilities)
	{
		Pair.Value = FMath::Clamp(Pair.Value, 0.01f, 1.0f);
	}

	float Total = 0.f;

	for (auto& Pair : AdjustedProbabilities)
	{
		Total += Pair.Value;
	}

	float R = FMath::RandRange(0.01f, Total);
	float Accumulator = 0.0f;

	for (auto& Pair : AdjustedProbabilities)
	{
		Accumulator += Pair.Value;
		if (R <= Accumulator)
		{
			return Pair.Key;
		}
	}

	return ESlotSymbols::ESS_Cherry;
}

void ASlotMachine::GeneratePredeterminedWin()
{
	uint8 MatchCount = RollWin();
	ESlotSymbols WinningSymbol = RollPredeterminedSymbol();

	if (MatchCount != 3)
	{
		SymbolToChase = ESlotSymbols::ESS_Invalid;
		GEngine->AddOnScreenDebugMessage(1, 15.f, FColor::Blue, TEXT("No Match"));
		StartSpin();
	}

	else if (MatchCount == 3)
	{
		SymbolToChase = WinningSymbol;
		GEngine->AddOnScreenDebugMessage(1, 15.f, FColor::Blue, TEXT("Match!"));
		GEngine->AddOnScreenDebugMessage(1, 15.f, FColor::Yellow, FString::Printf(TEXT("Winning Symbol: %s"), *SymbolToString(WinningSymbol)));

		StartSpin();
	}

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

	for (int i = 0; i < NumOfLines; i++)
	{
		SpinSteps[i] = 0;
		ReelShouldSpin[i] = true;
		TargetStepsToStop[i] = 0;
	}

	GetWorld()->GetTimerManager().SetTimer(ReelSpinTimerHandle, this, &ASlotMachine::TickSpinAllReels, 0.2f, true);
}

void ASlotMachine::TickSpinAllReels()
{
	bool bAnyReelStillSpinning = false;
	TArray<ESlotSymbols> VisibleSymbols;
	VisibleSymbols.SetNum(3);

	for (int Col = 0; Col < NumOfLines; Col++)
	{
		if (!ReelShouldSpin[Col])
			continue;

		bAnyReelStillSpinning = true;

		//Spin this reel
		Reels[Col].ReelStep();
		SpinSteps[Col]++;

		//For each reel send their symbols over to the interface as they are turning
		//Use the same array overwritten for each reel
		

		if (Col == 0)
		{
			for (int Row = 0; Row < VisibleSymbols.Num(); Row++)
			{
				VisibleSymbols[Row] = Reels[Col].GetVisibleWindow()[Row];
				if (SlotInterface)
				{
					SlotInterface->SetVisibleSymbols(Col,VisibleSymbols);
				}
			}
		}

		if (Col == 1)
		{
			for (int Row = 0; Row < VisibleSymbols.Num(); Row++)
			{
				VisibleSymbols[Row] = Reels[Col].GetVisibleWindow()[Row];
				if (SlotInterface)
				{
					SlotInterface->SetVisibleSymbols(Col, VisibleSymbols);
				}
			}
		}

		if (Col == 2)
		{
			for (int Row = 0; Row < VisibleSymbols.Num(); Row++)
			{
				VisibleSymbols[Row] = Reels[Col].GetVisibleWindow()[Row];
				if (SlotInterface)
				{
					SlotInterface->SetVisibleSymbols(Col, VisibleSymbols);
				}
			}
		}

		//Check if reel should stop
		if (SymbolToChase == ESlotSymbols::ESS_Invalid)
		{
			GEngine->AddOnScreenDebugMessage(1, 15.f, FColor::Blue, TEXT("No Symbol To chase"));

			if (SpinSteps[Col] >= MaxSpinStepsPerReel + (Col * DelayBetweenStops))
			{
				ReelShouldSpin[Col] = false;
			}
		}

		else if (SymbolToChase != ESlotSymbols::ESS_Invalid)
		{
			GEngine->AddOnScreenDebugMessage(2, 15.f, FColor::Yellow, FString::Printf(TEXT("Chasing Symbol: %s"), *SymbolToString(SymbolToChase)));

			uint8 DefaultSteps = MaxSpinStepsPerReel + (Col * DelayBetweenStops);
			if (TargetStepsToStop[Col] == 0 && SpinSteps[Col] >= DefaultSteps)
			{
				uint8 StepsToSymbol = Reels[Col].GetStepsToParticularSymbol(SymbolToChase, SpinSteps[Col]);
				TargetStepsToStop[Col] = SpinSteps[Col] + StepsToSymbol;
				GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, FString::Printf(
					TEXT("TargetStepsToStop[%d] = %d (SpinSteps = %d, StepsToSymbol = %d)"),
					Col, TargetStepsToStop[Col], SpinSteps[Col], StepsToSymbol));
			}

			if (TargetStepsToStop[Col] != 0 && SpinSteps[Col] >= TargetStepsToStop[Col])
			{
				ReelShouldSpin[Col] = false;
			}
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

		WinRow.EmplaceAt(Col, Reels[Col].GetVisibleWindow()[1]);

		Line += SymbolToString(WinRow[Col]);

		if (Col < 3)
		{
			Line += TEXT(" | ");
		}
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Blue, Line);
		
	}
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

int32 ASlotMachine::CalculatePayout(int32 BetInserted)
{
	TotalPayout = 0;
	
	ESlotSymbols FirstSymbol = WinRow[0];
	bool bAllMatch = true;

	
	if (!IsThreeOfAKind(WinRow[0], WinRow[1], WinRow[2]))
	{
		bAllMatch = false;
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow,TEXT("No Match"));
	}


	if (bAllMatch)
	{
		int32 Win = Payouts.Contains(FirstSymbol) ? Payouts[FirstSymbol] : 0;
		TotalPayout += Win * BetInserted;

		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow,
			FString::Printf(TEXT("Line win: %s x3 = %d"), *SymbolToString(FirstSymbol), Win));
	}
	
	return TotalPayout;
}

bool ASlotMachine::IsThreeOfAKind(ESlotSymbols SymbolA, ESlotSymbols SymbolB, ESlotSymbols SymbolC)
{
	return (SymbolA == SymbolB && SymbolB == SymbolC);
}

void ASlotMachine::ClearGame()
{
	Bet = 0;
	Reels[0].ResetStartIndex();
	Reels[1].ResetStartIndex();
	Reels[2].ResetStartIndex();

	if (TotalPayout == 0)
	{
		GameState = EGameState::EGS_Idle;
	}
	else GameState = EGameState::EGS_GameDone;
}

void FReel::InitializeReel(const TArray<ESlotSymbols>& WeightedSymbols)
{
	Rows = WeightedSymbols;
	Algo::RandomShuffle(Rows);
}

uint8 FReel::GetStepsToParticularSymbol(const ESlotSymbols SymbolLookingFor, const uint8 StepsUntilNow) const
{
	uint8 VisibleSymbolsIndex = 0;

	for (int R = 0; R < ReelRows * 2; R++)
	{
		const uint8 MiddleRowOffset = 1;
		VisibleSymbolsIndex = StepsUntilNow + R + MiddleRowOffset;
		ESlotSymbols Symbol = GetSymbolAt(VisibleSymbolsIndex);
		if (Symbol == SymbolLookingFor)
		{
			return R;
		}
	}
	return ReelRows;
}