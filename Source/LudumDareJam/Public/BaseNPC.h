// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h" 
#include "NPCAIController.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h" 
#include "../LudumDareJamGameMode.h"
#include "BaseNPC.generated.h"

UCLASS()
class LUDUMDAREJAM_API ABaseNPC : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseNPC();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Base NPC Settings")
		float ScoreValue = 500.0f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Base NPC Settings")
		float TimeBetweenBehaviours = 10.0f;

	/*UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Base NPC Settings")
		float LaunchForce = 150.0f;*/

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Base NPC Settings")
		float MovementSpeed = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "NPC Sound Effects")
		USoundCue* ScreamSound;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PrimaryBehaviour() {};

	UFUNCTION()
		void OnComponentBeginOverlap(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	ANPCAIController* AIController;
	ALudumDareJamGameMode* GameMode;

	FTimerHandle BehaviourTimer;
	TArray<FVector> PathToFollow;
	int PathIndex = 1;

	UFUNCTION(BlueprintImplementableEvent)
		void PlaySound(USoundCue* SoundToPlay);

	UFUNCTION(BlueprintCallable)
		void HitByNewspaper(AActor* OtherActor);

private:


};
