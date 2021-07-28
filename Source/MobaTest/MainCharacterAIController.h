// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MainCharacterAIController.generated.h"

class AMobaTestHero;
/**
 * 
 */
UCLASS()
class MOBATEST_API AMainCharacterAIController : public AAIController
{
	GENERATED_BODY()

	public:
	void CommandAttack(AMobaTestHero* MyHero, AMobaTestHero* Enemy);
	void CommandMove(FVector Location);
	void MoveToMouseCursor();
};
