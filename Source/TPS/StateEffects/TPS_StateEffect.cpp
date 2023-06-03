// Fill out your copyright notice in the Description page of Project Settings.


#include "TPS_StateEffect.h"
#include "Character/TPSHealthComponent.h"
#include "Interface/TPS_IGameActor.h"
#include "Kismet/GameplayStatics.h"

bool UTPS_StateEffect::InitObject(AActor* Actor, FName NameBoneHit)
{
	
	myActor = Actor;

	ITPS_IGameActor* myInterface = Cast<ITPS_IGameActor>(myActor);
	if (myInterface)
	{
		myInterface->AddEffect(this);
	}

	return true;
}

void UTPS_StateEffect::DestroyObject()
{
	ITPS_IGameActor* myInterface = Cast<ITPS_IGameActor>(myActor);
	if (myInterface)
	{
		myInterface->RemoveEffect(this);
	}

	myActor = nullptr;
	if (this && this->IsValidLowLevel())
	{
		this->ConditionalBeginDestroy();
	}	
}

bool UTPS_StateEffect_ExecuteOnce::InitObject(AActor* Actor, FName NameBoneHit)
{
	Super::InitObject(Actor, NameBoneHit);
	ExecuteOnce();
	return true;
}

void UTPS_StateEffect_ExecuteOnce::DestroyObject()
{
	Super::DestroyObject();
}

void UTPS_StateEffect_ExecuteOnce::ExecuteOnce()
{
	if (myActor)
	{
		UTPSHealthComponent* myHealthComp = Cast<UTPSHealthComponent>(myActor->GetComponentByClass(UTPSHealthComponent::StaticClass()));
		if (myHealthComp)
		{
			myHealthComp->ChangeHealthValue(Power);
		}
	}	

	DestroyObject();
}

bool UTPS_StateEffect_ExecuteTimer::InitObject(AActor* Actor, FName NameBoneHit)
{
	Super::InitObject(Actor, NameBoneHit);

	GetWorld()->GetTimerManager().SetTimer(TimerHandle_EffectTimer, this, &UTPS_StateEffect_ExecuteTimer::DestroyObject, Timer, false);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_ExecuteTimer, this, &UTPS_StateEffect_ExecuteTimer::Execute, RateTime, true);
	
	if (ParticleEffect)
	{
	//ToDo for object with interface create func return offset, Name Bones, 
	//ToDo Random init Effect with available array (For)
		FName NameBoneToAttached = NameBoneHit;
		FVector Loc = FVector(0);

		
		USceneComponent* myMesh = Cast<USceneComponent>(myActor->GetComponentByClass(USkeletalMeshComponent::StaticClass()));
		if (myMesh)
		{
			ParticleEmitter = UGameplayStatics::SpawnEmitterAttached(ParticleEffect, myMesh, NameBoneToAttached, Loc, FRotator::ZeroRotator, EAttachLocation::SnapToTarget, false);
		}
		else
		{
			ParticleEmitter = UGameplayStatics::SpawnEmitterAttached(ParticleEffect, myActor->GetRootComponent(), NameBoneToAttached, Loc, FRotator::ZeroRotator, EAttachLocation::SnapToTarget, false);
		}
		
	}
	
	return true;
}

void UTPS_StateEffect_ExecuteTimer::DestroyObject()
{
	ParticleEmitter->DestroyComponent();
	ParticleEmitter = nullptr;
	Super::DestroyObject();
}

void UTPS_StateEffect_ExecuteTimer::Execute()
{
	if (myActor)
	{	
		UTPSHealthComponent* myHealthComp = Cast<UTPSHealthComponent>(myActor->GetComponentByClass(UTPSHealthComponent::StaticClass()));
		if (myHealthComp)
		{
			myHealthComp->ChangeHealthValue(Power);
		}
	}
}
