// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "BaseCar.generated.h"

UENUM()
enum ECarState
{
	Parked UMETA(DisplayName = "Parked"),
	Accelerating UMETA(DisplayName = "Accelerating"),
	Driving UMETA(DisplayName = "Driving"),
	Decelerating UMETA(DisplayName = "Decelerating"),
	EmergencyBraking UMETA(DisplayName = "Emergency Braking"),
	Stopped UMETA(DisplayName = "Stopped"),
	StoppedAtPlayer UMETA(DisplayName = "Stopped At Player"),
	Busted UMETA(DisplayName = "Busted")
};

UCLASS()
class LUDUMDAREJAM_API ABaseCar : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCar();

	UPROPERTY(VisibleAnywhere)
		TArray<AActor*> PathToFollow;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Car Settings")
		float MaxMovingSpeed = 500.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Car Settings")
		float Acceleration = 250.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Car Settings")
		float EmergencyBrakingSpeed = 600.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Car Settings")
		int StopsBeforeDespawn = 4;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintPure)
		ECarState GetCarState() { return CarState; };
	UFUNCTION(BlueprintCallable)
		void SetCarState(ECarState NewState) { CarState = NewState; BehaviourState(); };

	UFUNCTION(BlueprintPure)
		float GetMovingSpeed() { return MovingSpeed; };
	UFUNCTION(BlueprintCallable)
		void SetMovementSpeed(float NewSpeed) { MovingSpeed = NewSpeed; };

	UFUNCTION(BlueprintPure)
		FVector GetStartPosition() { return StartPosition; };
	UFUNCTION(BlueprintCallable)
		void SetStartPosition(FVector NewPosition) { StartPosition = NewPosition; };

	UFUNCTION(BlueprintPure)
		TArray<AActor*> GetPathArray() { return PathToFollow; };
	UFUNCTION(BlueprintCallable)
		void SetPathArray(TArray<AActor*> NewPath);

	UFUNCTION(BlueprintCallable)
		void BeginStopping() { SetCarState(ECarState::Decelerating); };
	UFUNCTION(BlueprintCallable)
		void BeginDriving();
	UFUNCTION(BlueprintImplementableEvent)
		void CreateSplinePath();

	void BehaviourState();
	void EndStop();

private:

	ECarState CarState = ECarState::Parked;

	UPROPERTY(VisibleAnywhere)
	float MovingSpeed = 0.0f;

	FVector StartPosition = FVector::ZeroVector;

	int PathIndex = 0;
	int PositionCount = 0;

	FTimerHandle StopTimer;
	const float StopDuration = 1.0f;

};
