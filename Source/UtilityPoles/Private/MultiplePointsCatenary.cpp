/*
 * Copyright (c) 2025 Sora Mas
 * All rights reserved.
 */


#include "MultiplePointsCatenary.h"

AMultiplePointsCatenary::AMultiplePointsCatenary()
{
    DisableComponentsSimulatePhysics();
    PrimaryActorTick.bCanEverTick = false;

    SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));

    SetRootComponent(SceneComponent);

    bRunConstructionScriptOnDrag = false;
}

void AMultiplePointsCatenary::Generate()
{
    GenerateWire();

    if (bCleanupSplines)
        CleanupSplines();
}

void AMultiplePointsCatenary::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);
}

void AMultiplePointsCatenary::GenerateWire()
{
    // Mesh check
    if (!WireMesh)
    {
        RemoveSplineMeshes();
        return;
    }

    if (CatenaryConectionPoints.Num() < 2) return;
    SetMeshLenght();

    FTransform actorTransform = GetActorTransform();
    TArray<FVector> transformedCatenaryConectionPoints;

    for (FVector& Position : CatenaryConectionPoints)
    {
        transformedCatenaryConectionPoints.Add(actorTransform.TransformPosition(Position));
    }

    const TArray<FVector> CatenaryPoints = CalculateSingleCatenary(transformedCatenaryConectionPoints);

    AllWires.SetNum(1);
    ReuseOrCreateSplines();

    AvailableSplineMeshes = AllSplineMeshes;
    AllSplineMeshes.Reset();

    for (USplineComponent* Wire : AllWires)
    {
        Wire->SetSplinePoints(CatenaryPoints, ESplineCoordinateSpace::Local);
        ConstructSplineMeshesAlongSplines(Wire);
    }

    // Remove exces spline meshes
    for (USplineMeshComponent* ExtraSplineMesh : AvailableSplineMeshes)
    {
        ExtraSplineMesh->DestroyComponent();
    }


}
