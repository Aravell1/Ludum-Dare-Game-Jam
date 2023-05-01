// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DogAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h" 
#include "../LudumDareJamGameMode.h"
#include "PaperBoyInterface.h"
#include "Sound/SoundCue.h" 
#include "Components/CapsuleComponent.h" 
#include "BaseDog.generated.h"

UENUM()
enum EDogState
{
	ChasePlayer UMETA(DisplayName = "Chase Player"),
	CatchNewspaper UMETA(DisplayName = "Catch Newspaper"),
	RunAway UMETA(DisplayName = "Run Away"),
	Sit UMETA(DisplayName = "Sit")
};

UCLASS()
class LUDUMDAREJAM_API ABaseDog : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseDog();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Dog Settings")
		float ScoreValue = 400.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Dog Settings")
		float NegativeScoreValue = -300.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dog Settings")
		float NewspaperCheckRadius = 750.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dog Settings")
		float JumpSpeed = 1500.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dog Settings")
		float RunSpeed = 750.0f;

	UPROPERTY(BlueprintReadWrite)
		FVector SpawnPoint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dog Settings")
		UAnimMontage* JumpMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dog Sound Effects")
		USoundCue* BarkSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dog Sound Effects")
		USoundCue* PantSound;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result);
	UFUNCTION()
		void OnComponentBeginOverlap(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintPure)
		EDogState GetDogState() { return DogState; };
	UFUNCTION(BlueprintCallable)
		void SetDogState(EDogState NewState) { DogState = NewState; };
	UFUNCTION(BlueprintCallable)
		bool CheckDogState(EDogState StateToCheck) { return DogState == StateToCheck; };

	UFUNCTION(BlueprintImplementableEvent)
		void PlaySound(USoundCue* SoundToPlay);

private:

	void CheckForNewspapers();
	void CatchNewspaper(AActor* Newspaper);
	void RunAway();
	void PlayBark();

	EDogState DogState = EDogState::ChasePlayer;
	ADogAIController* AIController;
	AActor* Player;
	ALudumDareJamGameMode* GameMode;

	FTimerHandle RunAwayTimer;
	const float RunAwayDuration = 10.0f;

	FTimerHandle BarkTimer;
	const float BarkDuration = 10.0f;
};
