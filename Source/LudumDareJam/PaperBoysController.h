// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PaperBoysController.generated.h"

UENUM(BlueprintType)
enum class PaperBoyDrivingState : uint8 {
	Driving = 0 UMETA(DisplayName = "Driving"),
	Braking = 1 UMETA(DisplayName = "Braking")
};


/**
 * 
 */
UCLASS()
class LUDUMDAREJAM_API APaperBoysController : public APlayerController {
	GENERATED_BODY()

	APaperBoysController();

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent);
	
	// To add mapping context
	virtual void BeginPlay();
	
};
