// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCar.h"

// Sets default values
ABaseCar::ABaseCar()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ABaseCar::BehaviourState()
{
	switch (GetCarState())
	{
	case ECarState::Driving:
		//Play looping driving sound
		break;

	case ECarState::Stopped:
		GetWorldTimerManager().SetTimer(StopTimer, this, &ABaseCar::EndStop, StopDuration);
		break;

	case ECarState::StoppedAtPlayer:
		//Play Honk Sound every so often
		break;

	case ECarState::Busted:
		//Play constant honk sound
		SetLifeSpan(10);
		break;
	}
}

void ABaseCar::SetPathArray(TArray<AActor*> NewPath)
{
	PathToFollow = NewPath; 
	CreateSplinePath();
}

void ABaseCar::BeginDriving()
{
	SetCarState(ECarState::Accelerating);
}

void ABaseCar::EndStop()
{
	PositionCount++;
	if (PositionCount >= StopsBeforeDespawn)
		Destroy();

	SetCarState(ECarState::Accelerating);
}

// Called when the game starts or when spawned
void ABaseCar::BeginPlay()
{
	Super::BeginPlay();

	SetStartPosition(GetActorLocation());

}

// Called every frame
void ABaseCar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetCarState() == ECarState::Driving || GetCarState() == ECarState::StoppedAtPlayer || GetCarState() == ECarState::Accelerating || GetCarState() == ECarState::Decelerating)
	{
		//While Driving or StoppedAtPlayer, check for player
		FVector Start = GetActorLocation();
		FVector End = Start + GetActorForwardVector() * 500.0f;
		float Radius = 250;
		const TArray<AActor*> ActorsToIgnore = { GetOwner(), this };
		TArray<FHitResult> OutHits;

		const bool Hit = UKismetSystemLibrary::SphereTraceMulti(GetWorld(), Start, End, Radius,
			UEngineTypes::ConvertToTraceType(ECC_Camera), false, ActorsToIgnore,
			EDrawDebugTrace::None, OutHits, true, FLinearColor::Red, FLinearColor::Green, 0.1f);

		if (Hit)
		{
			bool HitPlayer = false;

			for (const FHitResult HitResult : OutHits)
			{
				if (HitResult.GetActor() == UGameplayStatics::GetPlayerCharacter(GetWorld(), 0) || Cast<ABaseCar>(HitResult.GetActor()))
				{
					SetCarState(ECarState::EmergencyBraking);
					HitPlayer = true;
				}
			}

			if (GetCarState() == ECarState::StoppedAtPlayer && HitPlayer == false)
			{
				SetCarState(ECarState::Accelerating);
			}
		}

		if (GetCarState() == ECarState::Accelerating)
		{
			SetMovementSpeed(GetMovingSpeed() + Acceleration * DeltaTime);

			if (GetMovingSpeed() >= MaxMovingSpeed)
			{
				SetMovementSpeed(MaxMovingSpeed);
				SetCarState(ECarState::Driving);
			}
		}
		else if (GetCarState() == ECarState::Decelerating)
		{
			SetMovementSpeed(GetMovingSpeed() - Acceleration * DeltaTime);

			if (GetMovingSpeed() <= 0)
			{
				SetMovementSpeed(0);
				SetCarState(ECarState::Stopped);
			}
		}
	}

	if (GetCarState() == ECarState::EmergencyBraking)
	{
		SetMovementSpeed(GetMovingSpeed() - EmergencyBrakingSpeed * DeltaTime);

		if (GetMovingSpeed() <= 0)
		{
			SetMovementSpeed(0);
			SetCarState(ECarState::StoppedAtPlayer);
		}
	}
}

