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
	case ECarState::Stopped:
		GetWorldTimerManager().SetTimer(StopTimer, this, &ABaseCar::EndStop, StopDuration);
		break;

	case ECarState::Busted:
		//Play constant honk sound
		SetMovementSpeed(0);
		PlaySound(BlaringHonkSound);
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

void ABaseCar::PlayHonk()
{
	if (GetCarState() == ECarState::StoppedAtPlayer || GetCarState() == ECarState::EmergencyBraking)
	{
		PlaySound(HonkSound);
		GetWorldTimerManager().SetTimer(HonkTimer, this, &ABaseCar::PlayHonk, FMath::RandRange(HonkDuration * 0.8f, HonkDuration * 1.2f));
	}
	else
	{
		PlaySound(DrivingSound);
	}
}

// Called when the game starts or when spawned
void ABaseCar::BeginPlay()
{
	Super::BeginPlay();

	SetStartPosition(GetActorLocation());

	PlaySound(DrivingSound);

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
				if (HitResult.GetActor() == UGameplayStatics::GetPlayerPawn(GetWorld(), 0) || Cast<ABaseCar>(HitResult.GetActor()) || Cast<ABaseDog>(HitResult.GetActor()))
				{
					SetCarState(ECarState::EmergencyBraking);
					HitPlayer = true;

					PlayHonk();
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

