// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSLaunchPad.h"

#include "FPSCharacter.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AFPSLaunchPad::AFPSLaunchPad()
:JumpPower(1500.F)
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = MeshComp;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("SphereComp"));
	BoxComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	BoxComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	BoxComp->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Overlap);
	BoxComp->SetupAttachment(MeshComp);

	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AFPSLaunchPad::OverlapBox);

	JumpDirection = CreateDefaultSubobject<UArrowComponent>(TEXT("JumpDirection"));
	JumpDirection->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	JumpDirection->SetupAttachment(BoxComp);
}

void AFPSLaunchPad::OverlapBox(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	AFPSCharacter* Character = Cast<AFPSCharacter>(OtherActor);
	if(Character || (OtherComp && OtherComp->IsSimulatingPhysics()))
	{
		FVector LaunchVector = JumpDirection->GetComponentTransform().Rotator().Vector() * JumpPower;
		UGameplayStatics::SpawnEmitterAtLocation(this, LaunchFX, GetActorLocation());

		if (Character)
			Character->LaunchCharacter(LaunchVector , true, true);
		else 
			OtherComp->AddImpulse(LaunchVector, NAME_None, true);
	}
}

