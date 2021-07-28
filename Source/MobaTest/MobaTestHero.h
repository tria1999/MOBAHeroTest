// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MobaTestCharacter.h"
#include "MobaTestHero.generated.h"

class AMainCharacterAIController;

/**
 * 
 */
UCLASS()
class MOBATEST_API AMobaTestHero : public AMobaTestCharacter
{
	GENERATED_BODY()
public:
	AMobaTestHero();

	virtual void BeginPlay() override;
	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;
	
	//commands
	UFUNCTION()
	void CommandMoveToPoint(FVector Location);
	UFUNCTION()
	void CommandAttack(AMobaTestHero* Enemy);
	UFUNCTION()
	void CommandClearCommands();
	UFUNCTION()
	void CommandQ(AMobaTestHero* Enemy);
	UFUNCTION()
	void CommandW();
	UFUNCTION()
	void CommandE();
	UFUNCTION()
	void CommandR(AMobaTestHero* Enemy);
	
	//helper functions
	UFUNCTION()
	void ClearCommands();
	UFUNCTION()
	void MoveToAttack();
	UFUNCTION()
	void CancelAttack();
	UFUNCTION()
	void CastQ();
	UFUNCTION()
	void CastW();
	UFUNCTION()
	void CastE();
	UFUNCTION()
	void CastR();
	UFUNCTION()
	void MoveToUseQ();
	UFUNCTION()
	void Face();
	UFUNCTION()
	void PerformDash(FVector Location);
	UFUNCTION()
	void PerformAttack();
	UFUNCTION()
	void HitAttack();
	UFUNCTION()
	void MoveToUseR();
	UFUNCTION()
	void BlinkBehind();
	UFUNCTION()
	void BlinkInFrontClose();
	UFUNCTION()
	void PerformBlink(FVector Location);

	UFUNCTION()
	void ApplyQEffect();
	UFUNCTION()
	void ApplyWEffect();
	UFUNCTION()
	void ApplyEEffect();
	UFUNCTION()
	void Blink1();
	UFUNCTION()
	void Blink2();
	void Blink3();
	UFUNCTION()
	void Blink4();
	UFUNCTION()
	void Blink5();

	//current command
	UPROPERTY()
	bool bIsMoving;
	UPROPERTY()
	FVector TargetLocation;
	UPROPERTY()
	bool bIsAttacking;
	UPROPERTY()
	AMobaTestHero* TargetEnemy;
	UPROPERTY()
	bool bInAttack;
	UPROPERTY()
	bool bIsCastingQ;
	UPROPERTY()
	bool bIsCastingR;
	UPROPERTY()
	bool bIsReceivingCommands;
	UPROPERTY()
	bool bWIsActive;
	UPROPERTY()	
	bool bEIsActive;
	UPROPERTY()	
	bool bIsInUlt;
	UPROPERTY()	
	bool bIsInUltAnim;

	//Animation
	UPROPERTY()	
	bool bIsDashing;
	//end cooldowns
	UFUNCTION()
	void EndAtkCooldown();
	UFUNCTION()
	void EndQCooldown();
	UFUNCTION()
	void EndWCooldown();
	UFUNCTION()
	void EndECooldown();
	UFUNCTION()
	void EndRCooldown();
	UFUNCTION()
	void RemoveQEffect();
	UFUNCTION()
	void RemoveWEffect();
	UFUNCTION()
	void RemoveEEffect();
	UFUNCTION()
	void RemoveREffect();

	//timer handles
	UPROPERTY()
	FTimerHandle AtkTimerHandle;
	UPROPERTY()
	FTimerHandle AtkDelayHandle;
	UPROPERTY()
	FTimerHandle QTimerHandle;
	UPROPERTY()
	FTimerHandle WTimerHandle;
	UPROPERTY()
	FTimerHandle ETimerHandle;
	UPROPERTY()
	FTimerHandle RTimerHandle;
	UPROPERTY()
	FTimerHandle QEffectTimerHandle;
	UPROPERTY()
	FTimerHandle WEffectTimerHandle;
	UPROPERTY()
	FTimerHandle EEffectTimerHandle;
	UPROPERTY()
	FTimerHandle REffectTimerHandle;

	UPROPERTY()
	bool bCanAttack;
	UPROPERTY()
	bool bCanUseQ;
	UPROPERTY()
	bool bCanUseW;
	UPROPERTY()
	bool bCanUseE;
	UPROPERTY()
	bool bCanUseR;
	UPROPERTY()
	bool bQ2;
	UPROPERTY()
	bool bEActive;


	UPROPERTY()
	int32 EStacks;

	UPROPERTY(EditAnywhere, Category ="Game", meta = (AllowPrivateAccess = "true"))
	int32 HeroTeam;
	UPROPERTY(EditAnywhere, Category ="Game", meta = (AllowPrivateAccess = "true"))
	float AtkRange = 125.f;

	UPROPERTY(EditAnywhere, Category ="Game", meta = (AllowPrivateAccess = "true"))
	float BaseAtkSpeed = 0.7f;
	UPROPERTY(EditAnywhere, Category ="Game", meta = (AllowPrivateAccess = "true"))
	float AtkSpeed;
	UPROPERTY(EditAnywhere, Category ="Game", meta = (AllowPrivateAccess = "true"))
	float BaseMoveSpeed = 600.0f;
	
	// UPROPERTY(EditAnywhere, Category = "Game", meta = (AllowPrivateAccess = "true"))
	// AMainCharacterAIController* HeroController;
};
