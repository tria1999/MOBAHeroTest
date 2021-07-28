// The MIT License (MIT)

// Copyright (c) 2015 Mark Vatsel
// Copyright (c) 2021 Triantafyllos Tsiakiris

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "RTSCameraPlayerController.h"
#include "CameraPawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "MobaTestHero.h"
#include "GameFramework/Controller.h"
#include "Engine/World.h"

ARTSCameraPlayerController::ARTSCameraPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;	
}

void ARTSCameraPlayerController::BeginPlay()
{
	Super::BeginPlay();	

	
}

void ARTSCameraPlayerController::PlayerTick(float DeltaTime)
{
    Super::PlayerTick(DeltaTime);	

	// keep updating the destination every tick while desired
	
	if (bMoveToMouseCursor)
	{
		MoveToMouseCursor();
	}
}


void ARTSCameraPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();
	
	InputComponent->BindAction("SetDestination", IE_Pressed, this, &ARTSCameraPlayerController::OnSetDestinationPressed);
	InputComponent->BindAction("SetDestination", IE_Released, this, &ARTSCameraPlayerController::OnSetDestinationReleased);
	InputComponent->BindAction("Stop", IE_Pressed, this, &ARTSCameraPlayerController::OnStop);
	InputComponent->BindAction("Q", IE_Pressed, this, &ARTSCameraPlayerController::OnQ);
	InputComponent->BindAction("W", IE_Pressed, this, &ARTSCameraPlayerController::OnW);
	InputComponent->BindAction("E", IE_Pressed, this, &ARTSCameraPlayerController::OnE);
	InputComponent->BindAction("R", IE_Pressed, this, &ARTSCameraPlayerController::OnR);


}

void ARTSCameraPlayerController::MoveToMouseCursor()
{
	// Trace to see what is under the mouse cursor
	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Visibility, false, Hit);
	if(ACameraPawn* MyPawn = Cast<ACameraPawn>(GetPawn()))
		if(AMobaTestHero* MyHero = Cast<AMobaTestHero>(MyPawn->MainCharacter))
		{
			if (Hit.bBlockingHit)
			{
				if (AMobaTestHero* HeroHit = Cast<AMobaTestHero>(Hit.GetActor()))
					{if(MyHero->HeroTeam!=HeroHit->HeroTeam)// We hit an enemy, move to attack
					{
						MyHero->CommandAttack(HeroHit);	
					}
					else
						MyHero->CommandMoveToPoint(Hit.Location);
					}
				else
					//CommandMove(MyHero->GetController(),Hit.Location);
					MyHero->CommandMoveToPoint(Hit.Location);
			}
		}
}

void ARTSCameraPlayerController::OnSetDestinationPressed()
{
	// set flag to keep updating destination until released
	bMoveToMouseCursor = true;
}

void ARTSCameraPlayerController::OnSetDestinationReleased()
{
	// clear flag to indicate we should stop updating the destination
	bMoveToMouseCursor = false;
}

void ARTSCameraPlayerController::OnStop()
{
	if(ACameraPawn* MyPawn = Cast<ACameraPawn>(GetPawn()))
		if(AMobaTestHero* MyHero = Cast<AMobaTestHero>(MyPawn->MainCharacter))
		{
			MyHero->CommandClearCommands();
			bMoveToMouseCursor = false;
		}
}

void ARTSCameraPlayerController::OnQ()
{
	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Visibility, false, Hit);
	if(ACameraPawn* MyPawn = Cast<ACameraPawn>(GetPawn()))
		if(AMobaTestHero* MyHero = Cast<AMobaTestHero>(MyPawn->MainCharacter))
		{
			if (Hit.bBlockingHit)
			{
				if (AMobaTestHero* HeroHit = Cast<AMobaTestHero>(Hit.GetActor()))
				{	if(MyHero->HeroTeam!=HeroHit->HeroTeam)// We hit an enemy, target with Q
					{
						MyHero->CommandQ(HeroHit);	
					}
				}
			}
			
		}
}

void ARTSCameraPlayerController::OnW()
{
	if(ACameraPawn* MyPawn = Cast<ACameraPawn>(GetPawn()))
		if(AMobaTestHero* MyHero = Cast<AMobaTestHero>(MyPawn->MainCharacter))
		{
			MyHero->CommandW();	
		}
}

void ARTSCameraPlayerController::OnE()
{
	if(ACameraPawn* MyPawn = Cast<ACameraPawn>(GetPawn()))
		if(AMobaTestHero* MyHero = Cast<AMobaTestHero>(MyPawn->MainCharacter))
		{
			MyHero->CommandE();	
		}
}


void ARTSCameraPlayerController::OnR()
{
	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Visibility, false, Hit);
	if(ACameraPawn* MyPawn = Cast<ACameraPawn>(GetPawn()))
		if(AMobaTestHero* MyHero = Cast<AMobaTestHero>(MyPawn->MainCharacter))
		{
			if (Hit.bBlockingHit)
			{
				if (AMobaTestHero* HeroHit = Cast<AMobaTestHero>(Hit.GetActor()))
				{	if(MyHero->HeroTeam!=HeroHit->HeroTeam)// We hit an enemy, target with Q
					{
						MyHero->CommandR(HeroHit);	
					}
				}
			}
			
		}
}