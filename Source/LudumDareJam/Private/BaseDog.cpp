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

	if (!AIController)
		AIController = GetController<ADogAIController>();
	if (AIController)
		AIController->GetPathFollowingComponent()->OnRequestFinished.AddUObject(this, &ABaseDog::OnMoveCompleted);

	if (!Player)
		Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	
	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;

	if (AIController && Player)
	{
		AIController->SetFocus(Player);
		AIController->MoveToActor(Player);
	}

}

void ABaseDog::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	if (Result.IsSuccess())
	{
		switch (GetDogState())
		{
		case EDogState::ChasePlayer:
			//Hit Player
			break;

		case EDogState::CatchNewspaper:
			//Socket Newspaper to mouth
			//Play Sit animation
			SetDogState(EDogState::Sit);
			break;

		case EDogState::RunAway:
			Destroy();
			break;
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
	FVector StartLocation = GetActorLocation();
	TArray<AActor*> ActorsToIgnore = { GetOwner(), this };
	TArray<AActor*> OutActors;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Camera));

	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), StartLocation, NewspaperCheckRadius, ObjectTypes, nullptr, ActorsToIgnore, OutActors);
	if (OutActors.Num() > 0)
	{
		for (AActor* HitActor : OutActors)
		{
			if (HitActor->ActorHasTag("Newspaper"))
			{
				CatchNewspaper(HitActor);
			}
		}
	}
}

void ABaseDog::CatchNewspaper(AActor* Newspaper)
{
	GetCharacterMovement()->MaxWalkSpeed = JumpSpeed;

	//Play Jump Animation

	AIController->MoveToActor(Newspaper);
}

