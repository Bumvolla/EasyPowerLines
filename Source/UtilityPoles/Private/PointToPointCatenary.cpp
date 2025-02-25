/*
 * Copyright (c) 2025 Sora Mas
 * All rights reserved.
 */


#include "PointToPointCatenary.h"

APointToPointCatenary::APointToPointCatenary()
{
    DisableComponentsSimulatePhysics();
    PrimaryActorTick.bCanEverTick = false;

    SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));

    SetRootComponent(SceneComponent);

    bRunConstructionScriptOnDrag = false;
}

void APointToPointCatenary::Generate()
{
    GenerateWire();

    if (bCleanupSplines)
        CleanupSplines();
}

void APointToPointCatenary::GenerateWire()
{
    // Mesh check
    if (!WireMesh)
    {
        RemoveSplineMeshes();
        return;
    }
    SetMeshLenght();

    FTransform actorTransform = GetActorTransform();
    const TArray<FVector> ConectionPoints = {actorTransform.TransformPosition(SplineStartPoint), actorTransform.TransformPosition(SplineEndPoint)};

    const TArray<FVector> CatenaryPoints = CalculateSingleCatenary(ConectionPoints);

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
