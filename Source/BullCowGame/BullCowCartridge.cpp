// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "HiddenWordList.h"
void UBullCowCartridge::BeginPlay() // When the game starts
{
	Super::BeginPlay();
	Isograms = GetValidWords(Words);
	SetupGame();
}
void UBullCowCartridge::OnInput(const FString &PlayerInput) // When the player hits enter
{
	ClearScreen();
	//if game is over then clear screen and setup the game
	if (bGameOver)
	{
		ClearScreen();
		SetupGame();
	}
	else
	{
		//Process the player guess
		ProcessGuess(PlayerInput);
	}
}
void UBullCowCartridge::SetupGame()
{
	//Set up the word and lives
	int32 random = FMath::RandRange(0, Isograms.Num() - 1);
	HiddenWord = Isograms[random];
	Lives = HiddenWord.Len() * 2;
	//PrintLine(TEXT("The HiddenWord is %s"),*HiddenWord);
	//PrintLine(TEXT("The number of valid words is %i"),ValidWords.Num());
	bGameOver = false;
	//Welcoming the Player
	PrintLine(TEXT("Welcome to Bulls & Cows!!!"));
	PrintLine(TEXT("Press Tab to interact with the board\nand again to stop interacting"));
	PrintLine(FString::Printf(TEXT("Guess the %i letter word"), HiddenWord.Len()));
	PrintLine(FString::Printf(TEXT("You have %i Lives"), Lives));
}
void UBullCowCartridge::EndGame()
{
	bGameOver = true;
	if (Lives <= 0)
	{
		ClearScreen();
		PrintLine(TEXT("You have lost!"));
		PrintLine(FString::Printf(TEXT("The Hidden word was: %s\n"), *HiddenWord));
	}
	PrintLine(TEXT("Press enter to play again"));
}
void UBullCowCartridge::ProcessGuess(const FString &Guess)
{
	if (Guess == HiddenWord)
	{
		PrintLine(TEXT("You Win!!"));
		EndGame();
		return;
	}
	if (Guess.Len() != HiddenWord.Len())
	{
		PrintLine(FString::Printf(TEXT("The Hidden word is %i characters long, try again!"), HiddenWord.Len()));
		return;
	}
	if (!IsIsogram(Guess))
	{
		PrintLine(TEXT("Your word is not an isogram!!"));
		PrintLine(TEXT("Remember that isograms\ndoes not repeat letters!!"));
		PrintLine(TEXT("Try Again!!"));
		return;
	}
	--Lives;
	if (Lives <= 0)
	{
		EndGame();
		return;
	}
		PrintLine(TEXT("You have lost a life"));
		PrintLine(FString::Printf(TEXT("Remaining lives: %i"), Lives));
		PrintLine(TEXT("Try Again"));
	//Show bulls and cows
	FBullCowCount Count = GetBullCows(Guess);
	PrintLine(TEXT("You have %i Bulls and %i Cows"), Count.Bulls, Count.Cows);
}

bool UBullCowCartridge::IsIsogram(const FString &Word) const
{
	for (int32 i = 0; i < Word.Len(); i++)
	{
		for (int32 x = i + 1; x < Word.Len(); x++)
		{
			if (Word[i] == Word[x])
			{
				return false;
			}
		}
	}
	return true;
}
TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString> &WordsArray) const

{
	TArray<FString> ValidWords;
	for (FString Word : WordsArray)
	{
		if (IsIsogram(Word))
		{
			ValidWords.Emplace(Word);
		}
	}
	return ValidWords;
}

FBullCowCount UBullCowCartridge::GetBullCows(const FString &Guess) const
{
	FBullCowCount Count;
	for (int32 i = 0; i < Guess.Len(); i++)
	{
		if (Guess[i] == HiddenWord[i])
			{
				Count.Bulls++;
				continue;
			}
		for (int32 x = 0; x < Guess.Len(); x++)
		{
			if (Guess[i] == HiddenWord[x])
			{
					Count.Cows++;
					break;
			}
		}
	}
	return Count;
}