// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Engine/Classes/Animation/AnimInstance.h"
#include "MobaTestHeroAnimInstance.generated.h"

/**
 * 
 */
UCLASS(transient, Blueprintable, hideCategories=AnimInstance, BlueprintType)
class MOBATEST_API UMobaTestHeroAnimInstance: public UAnimInstance
{
	GENERATED_BODY()
public:
	UMobaTestHeroAnimInstance();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Generic")
	float Speed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Generic")
	bool bIsDashing;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Generic")
	bool bInAttack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Generic")
	bool bInUlt;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};
