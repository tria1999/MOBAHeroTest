// Fill out your copyright notice in the Description page of Project Settings.


#include "MobaTestHeroAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MobaTestHero.h"

UMobaTestHeroAnimInstance::UMobaTestHeroAnimInstance()
{
    Speed = 0.f;
    bIsDashing = false;
}

void UMobaTestHeroAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);
    AActor* OwningActor = GetOwningActor();

    if(OwningActor)
    {
        Speed = OwningActor->GetVelocity().Size();
        AMobaTestHero* OwningCharacter = Cast<AMobaTestHero>(OwningActor);
        if (OwningCharacter)
        {
            bIsDashing = OwningCharacter->bIsDashing;
            bInAttack = OwningCharacter->bInAttack;
            bInUlt = OwningCharacter->bIsInUltAnim;
        }

    }
}
