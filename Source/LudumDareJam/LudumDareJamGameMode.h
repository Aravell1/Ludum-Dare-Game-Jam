// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LudumDareJamGameMode.generated.h"

UCLASS(minimalapi)
class ALudumDareJamGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ALudumDareJamGameMode();

	UFUNCTION(BlueprintPure)
		float GetScore() { return Score; };
	UFUNCTION(BlueprintCallable)
		void AddScore(float ScoreToAdd) { Score += ScoreToAdd; };
	UFUNCTION(BlueprintCallable)
		void SetScore(float NewScore) { Score = NewScore; };

private:

	float Score = 0;
};



