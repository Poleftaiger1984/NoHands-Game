// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/SlotMachine.h"
#include "Items/GameStates.h"
#include "Algo/RandomShuffle.h"
#include "LatentActions.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SlotInterfaceComponent.h"

ASlotMachine::ASlotMachine()
{
	ItemName = "Slot Machine";
	WeightedSymbolPool.SetNum(NumOfRows);
	GameState = EGameState::EGS_Idle;

	SlotInterface = CreateDefaultSubobject<USlotInterfaceComponent>(TEXT("Slot Interface"));
	SlotInterface->SetupAttachment(GetRootComponent());

	InitializeSymbolList(WeightedSymbolPool, ESlotSymbols::ESS_Cherry, CherryProbabality);
	InitializeSymbolList(WeightedSymbolPool, ESlotSymbols::ESS_Lemon, LemonProbabality);
	InitializeSymbolList(WeightedSymbolPool, ESlotSymbols::ESS_Watermelon, WatermelonProbabality);
	InitializeSymbolList(WeightedSymbolPool, ESlotSymbols::ESS_Star, StarProbabality);
	InitializeSymbolList(WeightedSymbolPool, ESlotSymbols::ESS_Bell, BellProbabality);
	InitializeSymbolList(WeightedSymbolPool, ESlotSymbols::ESS_Diamond, DiamondProbabality);
	InitializeSymbolList(WeightedSymbolPool, ESlotSymbols::ESS_Seven, SevenProbabality);

	Payouts = {
		{ESlotSymbols::ESS_Seven,	  100},
		{ESlotSymbols::ESS_Diamond,	   50},
		{ESlotSymbols::ESS_Star,	   40},
		{ESlotSymbols::ESS_Bell,	   20},
		{ESlotSymbols::ESS_Watermelon, 10},
		{ESlotSymbols::ESS_Lemon,	    8},
		{ESlotSymbols::ESS_Cherry,      2}
	};

	AllSlotSymbols = {
		ESlotSymbols::ESS_Cherry,
		ESlotSymbols::ESS_Lemon,
		ESlotSymbols::ESS_Watermelon,
		ESlotSymbols::ESS_Star,
		ESlotSymbols::ESS_Bell,
		ESlotSymbols::ESS_Diamond,
		ESlotSymbols::ESS_Seven
	};

	Algo::RandomShuffle(AllSlotSymbols);
	Algo::RandomShuffle(WeightedSymbolPool);

	for (const ESlotSymbols& Symbol : AllSlotSymbols)
	{
		int32 InsertIndex = FMath::RandRange(0, WeightedSymbolPool.Num());
		WeightedSymbolPool.Insert(Symbol, InsertIndex);
	}

	TArray<ESlotSymbols> ShuffledPool = WeightedSymbolPool;
	Algo::RandomShuffle(ShuffledPool);

	for (int Col = 0; Col < NumOfLines; Col++)
	{
		Reels.Emplace(FReel(NumOfRows));
		Reels[Col].InitializeReel(ShuffledPool);
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
		StartSpin();
	}

	if (GameState == EGameState::EGS_GameDone)
	{
		GameState = EGameState::EGS_Idle;
		TotalPayout = 0;
	}
}

void ASlotMachine::SetBet(int32 PlayerBet)
{
	Bet = PlayerBet;
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

			if (Col < 3)
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

int32 ASlotMachine::CalculatePayout(int32 BetInserted)
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
			int32 Win = Payouts.Contains(FirstSymbol) ? Payouts[FirstSymbol] : 0;
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
	Bet = 0;

	if (TotalPayout == 0)
	{
		GameState = EGameState::EGS_Idle;
	}
	else GameState = EGameState::EGS_GameDone;
}

void FReel::InitializeReel(const TArray<ESlotSymbols>& WeightedSymbols)
{
	TArray<ESlotSymbols> ReelSymbols = WeightedSymbols;

	//Shuffle once
	Algo::RandomShuffle(ReelSymbols);

	ESlotSymbols LastSymbol = ESlotSymbols::ESS_Invalid;
	for (int Row = 0; Row < ReelRows; Row++)
	{

		//Try to find different symbol from the last one
		for (int Attempt = 0; Attempt < 5; Attempt++)
		{
			int Index = FMath::RandRange(0, WeightedSymbols.Num() - 1);
			if (ReelSymbols[Index] != LastSymbol)
			{
				Rows[Row] = WeightedSymbols[Index];
				LastSymbol = Rows[Row];
				break;
			}
		}
	}
}