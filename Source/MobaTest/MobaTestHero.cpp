// Fill out your copyright notice in the Description page of Project Settings.


#include "MobaTestHero.h"
#include "Engine/EngineTypes.h"
#include "Components/DecalComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/World.h"
#include "Math/Vector.h"
#include "TimerManager.h"
#include "RTSCameraPlayerController.h"
#include "MainCharacterAIController.h"

AMobaTestHero::AMobaTestHero()
{
}

void AMobaTestHero::BeginPlay()
{
    Super::BeginPlay();
    bIsReceivingCommands=true;
    bCanAttack = true;
	bCanUseQ = true;
	bCanUseW = true;
	bCanUseE = true;
    bCanUseR = true;
    bQ2 = false;
    AtkSpeed = BaseAtkSpeed;
    bEActive = false;
    bInAttack = false;
    EStacks = 0;
    bIsDashing = false;
    bIsInUlt = false;
    bIsInUltAnim = false;
    ClearCommands();
}
void AMobaTestHero::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    if(bIsAttacking)
        MoveToAttack();
    if(bIsMoving)
    {   
        if(this->GetActorLocation()!=TargetLocation)
            UAIBlueprintHelperLibrary::SimpleMoveToLocation(this->GetController(), TargetLocation);
        else
            ClearCommands();
    }
    if(bIsCastingQ)
    {
        MoveToUseQ();
    }
    if(bIsCastingR)
    {
        MoveToUseR();
    }

    UCharacterMovementComponent *MovementComponent =  Cast<UCharacterMovementComponent>(this->GetCharacterMovement());
    MovementComponent->MaxWalkSpeed = BaseMoveSpeed + BaseMoveSpeed*EStacks*0.15f;

    
}

void AMobaTestHero::CommandMoveToPoint(FVector Location) 
{   
    if(bIsReceivingCommands)
    {
        ClearCommands();
        bIsMoving = true;
        TargetLocation = Location;
    }    
}

void AMobaTestHero::CommandAttack(AMobaTestHero* Enemy)
{
    if(bIsReceivingCommands)
    {
        ClearCommands();
        bIsAttacking = true;
        TargetEnemy = Enemy;
    }
}

void AMobaTestHero::CommandClearCommands()
{   if(bIsReceivingCommands)
        ClearCommands();
}

void AMobaTestHero::CommandQ(AMobaTestHero* Enemy) 
{
    if(bIsReceivingCommands&&bCanUseQ)
    {
        ClearCommands();
        TargetEnemy = Enemy;
        bIsCastingQ = true;
    }
    
}

void AMobaTestHero::CommandW() 
{
    bWIsActive = true;
    CastW();
}

void AMobaTestHero::CommandE() 
{
    bEIsActive = true;
    CastE();
}

void AMobaTestHero::CommandR(AMobaTestHero* Enemy) 
{
    if(bIsReceivingCommands&&bCanUseR)
    {
        ClearCommands();
        TargetEnemy = Enemy;
        bIsCastingR = true;
    }
}

void AMobaTestHero::ClearCommands()
{
    bIsMoving = false;
    bIsAttacking = false;
    bIsCastingQ = false;
    bIsCastingR = false;
    CancelAttack();
    UAIBlueprintHelperLibrary::SimpleMoveToActor
    (this->GetController(), this);
}

void AMobaTestHero::MoveToAttack() 
{
    if(TargetEnemy)
    {
        if (this->GetDistanceTo(TargetEnemy) > AtkRange)
            UAIBlueprintHelperLibrary::SimpleMoveToActor(this->GetController(), TargetEnemy);
        else
            PerformAttack();
    }
}

void AMobaTestHero::PerformAttack()
{   
    Face();
    if(bCanAttack)
        if (this->GetDistanceTo(TargetEnemy) <= AtkRange)
            {   
                UAIBlueprintHelperLibrary::SimpleMoveToActor
                (this->GetController(), this);
                //UE_LOG(LogTemp, Warning, TEXT("Attack start"));
                if(!bInAttack)
                {
                    bInAttack = true;
                    GetWorld()->GetTimerManager().SetTimer(AtkDelayHandle,
                    this,&AMobaTestHero::HitAttack,0.4f/AtkSpeed,false);
                }
                
                //HitAttack();
            }	
    //GetWorld()->GetDeltaSeconds()
}

void AMobaTestHero::HitAttack()
{   
    GetWorld()->GetTimerManager().ClearTimer(AtkDelayHandle);
    UE_LOG(LogTemp, Warning, TEXT("Dealt Basic Attack damage"));
    bInAttack = false;
    bCanAttack = false;
    GetWorld()->GetTimerManager().SetTimer(AtkTimerHandle,
    this,&AMobaTestHero::EndAtkCooldown,0.6f/AtkSpeed,true);
    if (bEActive&&EStacks<3)
        {EStacks++;}
}

void AMobaTestHero::EndAtkCooldown()
{
    bCanAttack = true;
    GetWorld()->GetTimerManager().ClearTimer(AtkTimerHandle);
}

void AMobaTestHero::CancelAttack()
{
    bInAttack = false;
    GetWorld()->GetTimerManager().ClearTimer(AtkDelayHandle);
}

void AMobaTestHero::MoveToUseQ() 
{
    if(TargetEnemy)
    {
        if (this->GetDistanceTo(TargetEnemy) <= 600.f)
        {   
            CastQ();	
        }
        else
        {
            UAIBlueprintHelperLibrary::SimpleMoveToActor(this->GetController(), TargetEnemy);
        }
    }
    
}

void AMobaTestHero::CastQ()
{   
    CancelAttack();
    if (this->GetDistanceTo(TargetEnemy) > 100.f)
    {
        bIsDashing = true;
        FVector Destination = TargetEnemy->GetActorLocation(); 
        bIsReceivingCommands=false;
        bCanUseQ = false;
        Destination = Destination-100*Destination.GetSafeNormal(1.f);
        PerformDash(Destination);
    }
    else
    {
        
        UE_LOG(LogTemp, Warning, TEXT("Distance = %f"),this->GetDistanceTo(TargetEnemy));  
        UE_LOG(LogTemp, Warning, TEXT("Dealt Q damage"));
        bIsDashing = false;
        if (bEActive&&EStacks<3)
            EStacks++;
        ClearCommands();
        if(!bQ2)
        {
            ApplyQEffect();         
        }
        else
        {    
            bCanUseQ = false;
            GetWorld()->GetTimerManager().SetTimer(QTimerHandle,
            this,&AMobaTestHero::EndQCooldown,8.f,false);
        }
        bIsReceivingCommands=true;
        CommandAttack(TargetEnemy);
        
    }
}

void AMobaTestHero::ApplyQEffect()
{
    bCanUseQ = true;
    bQ2 = true;
    GetWorld()->GetTimerManager().SetTimer(QEffectTimerHandle,
    this,&AMobaTestHero::RemoveQEffect,4.f,false);
}

void AMobaTestHero::Face()
{ 
    FVector DashVector = UKismetMathLibrary::GetDirectionUnitVector(this->GetActorLocation(),TargetEnemy->GetActorLocation());
    FVector DashVectorCleaned = FVector(DashVector.X, DashVector.Y, this->GetActorLocation().Z);
    FRotator FaceRotation = FVector(DashVectorCleaned).Rotation();
    FRotator FaceRotationCleaned = FRotator(0.f,FaceRotation.Yaw,0.f);
	this->SetActorRotation(FaceRotationCleaned);
}

void AMobaTestHero::PerformDash(FVector Location)
{
    float DeltaSeconds = GetWorld()->GetDeltaSeconds();
    FVector StartLocation = this->GetActorLocation();
    FVector DashVector = TargetEnemy->GetActorLocation()-StartLocation;
    DashVector = DashVector.GetSafeNormal(1.f)*1500.f*DeltaSeconds;
    if((this->GetActorLocation()-Location).Size()<1500.f*DeltaSeconds)
        this->SetActorLocation(StartLocation+(this->GetActorLocation()-Location));
    else
        this->SetActorLocation(DashVector+StartLocation);
    Face();
}

void AMobaTestHero::CastW()
{
   if(bCanUseW)
    {
        UE_LOG(LogTemp, Warning, TEXT("Blocking Next attack (W)"));
        bCanUseW = false;
        ApplyWEffect();
        GetWorld()->GetTimerManager().SetTimer(WTimerHandle,
        this,&AMobaTestHero::EndWCooldown,6.f,false);
    }	
   
}

void AMobaTestHero::ApplyWEffect()
{
    //block next attack
    //if hit -> RemoveWEffect
    GetWorld()->GetTimerManager().SetTimer(WEffectTimerHandle,
    this,&AMobaTestHero::RemoveWEffect,1.5f,false);
}

void AMobaTestHero::CastE()
{
    if(bCanUseE)
    {
        UE_LOG(LogTemp, Warning, TEXT("Activating E"));
        bCanUseE = false;
        ApplyEEffect();
        GetWorld()->GetTimerManager().SetTimer(ETimerHandle,
        this,&AMobaTestHero::EndECooldown,11.f,false);
    }	
}

void AMobaTestHero::ApplyEEffect()
{
    bEActive = true;
    EndAtkCooldown();
    AtkSpeed += BaseAtkSpeed*1.2f;
    GetWorld()->GetTimerManager().SetTimer(EEffectTimerHandle,
    this,&AMobaTestHero::RemoveEEffect,3.f,false);
}

void AMobaTestHero::MoveToUseR() 
{
    if(TargetEnemy)
    {
        if (this->GetDistanceTo(TargetEnemy) <= 400.f)
        {   
            CastR();	
        }
        else
        {
            UAIBlueprintHelperLibrary::SimpleMoveToActor(this->GetController(), TargetEnemy);
        }
    }
    
}

void AMobaTestHero::CastR()
{   
    bCanUseR = false;
    bIsReceivingCommands = false;
    if(!bIsInUlt)
    {
        bIsInUlt = true;
        GetWorld()->GetTimerManager().SetTimer(REffectTimerHandle,
        this,&AMobaTestHero::Blink1,0.4f,false);
        GetWorld()->GetTimerManager().SetTimer(RTimerHandle,
        this,&AMobaTestHero::EndRCooldown,10.f,false);
    }
}

void AMobaTestHero::PerformBlink(FVector Location)
{
    FVector LocationCleaned = FVector(Location.X, Location.Y, this->GetActorLocation().Z);
    this->SetActorLocation(Location); 
}

void AMobaTestHero::Blink1()
{
    GetWorld()->GetTimerManager().ClearTimer(REffectTimerHandle);
    BlinkBehind();
    
    UE_LOG(LogTemp, Warning, TEXT("Dealt R damage"));
    if (bEActive&&EStacks<3)
        EStacks++;
    bIsInUltAnim = true;
    GetWorld()->GetTimerManager().SetTimer(REffectTimerHandle,
    this,&AMobaTestHero::Blink2,0.4f,false);    
}

void AMobaTestHero::Blink2()
{
    GetWorld()->GetTimerManager().ClearTimer(REffectTimerHandle);
    BlinkBehind();
    UE_LOG(LogTemp, Warning, TEXT("Dealt R damage"));
    if (bEActive&&EStacks<3)
        EStacks++;
    GetWorld()->GetTimerManager().SetTimer(REffectTimerHandle,
        this,&AMobaTestHero::Blink3,0.4f,false);    
    
}
void AMobaTestHero::Blink3()
{
    GetWorld()->GetTimerManager().ClearTimer(REffectTimerHandle);
    BlinkBehind();
    UE_LOG(LogTemp, Warning, TEXT("Dealt R damage"));
    if (bEActive&&EStacks<3)
        EStacks++;
    GetWorld()->GetTimerManager().SetTimer(REffectTimerHandle,
        this,&AMobaTestHero::Blink4,0.4f,false);    
    
}
void AMobaTestHero::Blink4()
{
    GetWorld()->GetTimerManager().ClearTimer(REffectTimerHandle);
    BlinkBehind();
    UE_LOG(LogTemp, Warning, TEXT("Dealt R damage"));
    if (bEActive&&EStacks<3)
        EStacks++;
    GetWorld()->GetTimerManager().SetTimer(REffectTimerHandle,
        this,&AMobaTestHero::Blink5,0.4f,false);    
    
}
void AMobaTestHero::Blink5()
{
    GetWorld()->GetTimerManager().ClearTimer(REffectTimerHandle);
    BlinkInFrontClose();
    if (bEActive&&EStacks<3)
        EStacks++; 
    UE_LOG(LogTemp, Warning, TEXT("Dealt R damage"));
    bIsInUlt = false;
    bIsInUltAnim = false;
    bIsReceivingCommands = true;
    ClearCommands();
    CommandAttack(TargetEnemy);
}

void AMobaTestHero::BlinkBehind()
{
    FVector StartLocation = this->GetActorLocation();
    FVector BlinkVector = TargetEnemy->GetActorLocation()-StartLocation;
    FVector Destination = BlinkVector+150*BlinkVector.GetSafeNormal(1.f);
   
    FVector FinalDest = StartLocation + Destination;
    FVector FinalDestCleaned = FVector(FinalDest.X, FinalDest.Y, this->GetActorLocation().Z);
    PerformBlink(FinalDestCleaned);
    Face();   
}

void AMobaTestHero::BlinkInFrontClose()
{
    FVector StartLocation = this->GetActorLocation();
    FVector BlinkVector = TargetEnemy->GetActorLocation()-StartLocation;
    FVector Destination = BlinkVector-100*BlinkVector.GetSafeNormal(1.f);
   
    FVector FinalDest = StartLocation + Destination;
    FVector FinalDestCleaned = FVector(FinalDest.X, FinalDest.Y, this->GetActorLocation().Z);
    PerformBlink(FinalDestCleaned);
    Face();   
}

void AMobaTestHero::EndQCooldown()
{
    UE_LOG(LogTemp, Warning, TEXT("Q off CD"));
    bCanUseQ = true;
    GetWorld()->GetTimerManager().ClearTimer(QTimerHandle);
}
void AMobaTestHero::EndWCooldown()
{
    UE_LOG(LogTemp, Warning, TEXT("W off CD"));
    bCanUseW = true;
    GetWorld()->GetTimerManager().ClearTimer(WTimerHandle);
}
void AMobaTestHero::EndECooldown()
{
    UE_LOG(LogTemp, Warning, TEXT("E off CD"));
    bCanUseE = true;
    GetWorld()->GetTimerManager().ClearTimer(ETimerHandle);
}
void AMobaTestHero::EndRCooldown()
{
    UE_LOG(LogTemp, Warning, TEXT("R off CD"));
    bCanUseR = true;
    GetWorld()->GetTimerManager().ClearTimer(RTimerHandle);    
}

void AMobaTestHero::RemoveQEffect()
{   
    bCanUseQ = false;
    bQ2 = false;
    GetWorld()->GetTimerManager().ClearTimer(QEffectTimerHandle);
    bCanUseQ = false;
    GetWorld()->GetTimerManager().SetTimer(QTimerHandle,
    this,&AMobaTestHero::EndQCooldown,8.f,false);

}
void AMobaTestHero::RemoveWEffect()
{ 
    UE_LOG(LogTemp, Warning, TEXT("No longer blocking (W)"));
    GetWorld()->GetTimerManager().ClearTimer(EEffectTimerHandle);
}
void AMobaTestHero::RemoveEEffect()
{   
    bEActive = false;
    AtkSpeed -= BaseAtkSpeed*1.2f;
    EStacks=0;
    GetWorld()->GetTimerManager().ClearTimer(EEffectTimerHandle);
}

void AMobaTestHero::RemoveREffect()
{   

}
