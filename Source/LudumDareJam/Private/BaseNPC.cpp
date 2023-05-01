// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseNPC.h"

// Sets default values
ABaseNPC::ABaseNPC()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABaseNPC::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorldTimerManager().SetTimer(BehaviourTimer, this, &ABaseNPC::PrimaryBehaviour, FMath::RandRange(TimeBetweenBehaviours * 0.5f, TimeBetweenBehaviours * 1.5f), true);

	GetCharacterMovement()->MaxWalkSpeed = MovementSpeed;

}

void ABaseNPC::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor->ActorHasTag("Newspaper") && OtherActor->GetVelocity().Length() > 1000.0f)
	{
		//Add Score to GameMode

		GetWorldTimerManager().ClearTimer(BehaviourTimer);
		GetMesh()->SetSimulatePhysics(true);
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		FVector LaunchDirection = (GetActorLocation() - OtherActor->GetActorLocation()).GetSafeNormal();
		FRotator LaunchRotator = LaunchDirection.Rotation();
		LaunchRotator.Pitch = 75.0f;
		LaunchDirection = LaunchRotator.Vector();
		FVector LaunchVector = LaunchDirection * OtherActor->GetVelocity().Length() * GetMesh()->GetMass();

		LaunchCharacter(LaunchVector, true, true);
	}
}

// Called every frame
void ABaseNPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


