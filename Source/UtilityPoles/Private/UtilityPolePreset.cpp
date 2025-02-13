// Fill out your copyright notice in the Description page of Project Settings.

#include "UtilityPolePreset.h"
#include "Editor.h"

// Sets default values
AUtilityPolePreset::AUtilityPolePreset()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SMComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pole Mesh"));

	SetRootComponent(SMComponent);

	SMComponent->SetStaticMesh(PoleMesh);

}

// Called when the game starts or when spawned
void AUtilityPolePreset::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AUtilityPolePreset::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AUtilityPolePreset::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	// Update the static mesh component with the current PoleMesh
	SMComponent->SetStaticMesh(PoleMesh);
}

#if WITH_EDITOR

void AUtilityPolePreset::InstancePropertiesToClassDefaults()
{

    if (!GIsEditor)
    {
        return;
    }

    UClass* Class = GetClass();
    UObject* CDO = Class->GetDefaultObject();

    if (!CDO)
    {
        return;
    }

    if (FProperty* PoleMeshProperty = Class->FindPropertyByName(TEXT("PoleMesh")))
    {
        void* SourceAddr = PoleMeshProperty->ContainerPtrToValuePtr<void>(this);
        void* DestAddr = PoleMeshProperty->ContainerPtrToValuePtr<void>(CDO);
        PoleMeshProperty->CopyCompleteValue(DestAddr, SourceAddr);
    }

    if (FProperty* WireTargetsProperty = Class->FindPropertyByName(TEXT("WireTargets")))
    {
        void* SourceAddr = WireTargetsProperty->ContainerPtrToValuePtr<void>(this);
        void* DestAddr = WireTargetsProperty->ContainerPtrToValuePtr<void>(CDO);
        WireTargetsProperty->CopyCompleteValue(DestAddr, SourceAddr);
    }

    CDO->PostEditChange();
    CDO->MarkPackageDirty();

}

#endif
