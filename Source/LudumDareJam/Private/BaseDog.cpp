// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseDog.h"

// Sets default values
ABaseDog::ABaseDog()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABaseDog::BeginPlay()
{
	Super::BeginPlay();

	SpawnDefaultController();

	SpawnPoint = GetActorLocation();

	if (!AIController)
		AIController = GetController<ADogAIController>();
	if (AIController)
		AIController->GetPathFollowingComponent()->OnRequestFinished.AddUObject(this, &ABaseDog::OnMoveCompleted);

	if (!GameMode)
		GameMode = Cast<ALudumDareJamGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	if (!Player)
		Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	
	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ABaseDog::OnComponentBeginOverlap);

	if (AIController && Player)
	{
		AIController->SetFocus(Player);
		AIController->MoveToActor(Player);
	}

	PlayBark();
}

void ABaseDog::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	if (Result.IsSuccess())
	{
		switch (GetDogState())
		{
		case EDogState::RunAway:
			Destroy();
			break;
		}
	}
}


void ABaseDog::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (CheckDogState(EDogState::ChasePlayer))
	{
		if (OtherActor == UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
		{
			if (Cast<IPaperBoyInterface>(OtherActor))
			{
				Cast<IPaperBoyInterface>(OtherActor)->PlayerHit(this);
			}
			//Hit Player
			RunAway();
		}
	}
}

// Called every frame
void ABaseDog::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CheckDogState(EDogState::ChasePlayer))
		CheckForNewspapers();
}

void ABaseDog::CheckForNewspapers()
{
	FVector Start = GetActorLocation();
	FVector End = Start + GetActorForwardVector() * 100.0f;
	float Radius = NewspaperCheckRadius;
	const TArray<AActor*> ActorsToIgnore = { GetOwner(), this };
	TArray<FHitResult> OutHits;

	const bool Hit = UKismetSystemLibrary::SphereTraceMulti(GetWorld(), Start, End, Radius,
		UEngineTypes::ConvertToTraceType(ECC_Camera), false, ActorsToIgnore,
		EDrawDebugTrace::None, OutHits, true, FLinearColor::Red, FLinearColor::Green, 0.1f);

	if (Hit)
	{
		for (FHitResult HitResult : OutHits)
		{
			if (HitResult.GetActor()->ActorHasTag("Newspaper") && HitResult.GetActor()->GetVelocity().Length() > 250.0f)
			{
				CatchNewspaper(HitResult.GetActor());
				break;
			}
		}
	}
}

void ABaseDog::CatchNewspaper(AActor* Newspaper)
{
	AIController->SetFocus(Newspaper);
	SetActorRotation(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Newspaper->GetActorLocation()));

	if (JumpMontage)
		GetMesh()->GetAnimInstance()->Montage_Play(JumpMontage);

	FVector LaunchDirection = (Newspaper->GetActorLocation() - GetActorLocation()).GetSafeNormal();
	LaunchDirection *= JumpSpeed;

	LaunchCharacter(LaunchDirection, true, true);

	Newspaper->Destroy();
	Cast<UStaticMeshComponent>(GetDefaultSubobjectByName(TEXT("Newspaper")))->SetVisibility(true);

	SetDogState(EDogState::Sit);
	AIController->StopMovement();
	GameMode->AddScore(ScoreValue);

	GetWorldTimerManager().SetTimer(RunAwayTimer, this, &ABaseDog::RunAway, RunAwayDuration, false);

	PlaySound(PantSound);
}

void ABaseDog::RunAway()
{
	AIController->ClearFocus(EAIFocusPriority::Gameplay);
	AIController->MoveToLocation(SpawnPoint);
	SetDogState(EDogState::RunAway);
}

void ABaseDog::PlayBark()
{
	if (CheckDogState(EDogState::ChasePlayer))
	{
		PlaySound(BarkSound);
		GetWorldTimerManager().SetTimer(BarkTimer, this, &ABaseDog::PlayBark, FMath::RandRange(BarkDuration * 0.8f, BarkDuration * 1.2f));
	}
}

