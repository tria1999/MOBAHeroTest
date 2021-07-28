// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacterAIController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "MobaTestHero.h"
#include "Engine/World.h"

void AMainCharacterAIController::CommandAttack(AMobaTestHero* MyHero, AMobaTestHero* Enemy)
{
	if (MyHero->GetDistanceTo(Enemy) > 120.0f)
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, Enemy->GetActorLocation());
	else 
		UE_LOG(LogTemp, Warning, TEXT("Attacking enemy"));	
	
}

void AMainCharacterAIController::CommandMove(FVector Location)
{
    UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, Location);
}