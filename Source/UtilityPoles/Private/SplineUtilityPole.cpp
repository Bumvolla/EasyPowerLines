/*
 * Copyright (c) 2025 Sora Mas
 * All rights reserved.
 */

#include "SplineUtilityPole.h"
#include "UtilityPoles.h"

// Sets default values
ASplineUtilityPole::ASplineUtilityPole()
{
    DisableComponentsSimulatePhysics();
    PrimaryActorTick.bCanEverTick = false;
	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
    Spline->SetMobility(EComponentMobility::Static);
	SetRootComponent(Spline);

    bRunConstructionScriptOnDrag = false;

}

void ASplineUtilityPole::GenerateWires()
{

    // Mesh check
    if (!WireMesh)
    {
        RemoveSplineMeshes();
        return;
    }
    SetMeshLenght();

    // Poles setup
    const int32 PolesAmount = PoleIndices.Num();
    if (PolesAmount < 2) return;
    TArray<AUtilityPolePreset*> CastedKeys;
    CastedKeys.Reserve(PolesAmount);
    for (int32 i = 0; i < PolesAmount; i++)
    {
        if (AUtilityPolePreset* Pole = Cast<AUtilityPolePreset>(PoleIndices[i]->GetChildActor()))
        {
            CastedKeys.Add(Pole);
        }
    }
    if (CastedKeys.Num() < 2) return;

    // Retrieves how many connections the current pole have
    const uint8 TransformsAmount = CastedKeys[0]->WireTargets.Num();

    //Handles exces previously generated splines 
    RemoveExcesSplines(TransformsAmount);

    // Create or reuse spline components
    ReuseOrCreateSplines();

    //Creates and return an array that contains all catenary points for each needed wire
    TArray<TArray<FVector>> AllCatenaryPoints = CalculateCatenariesParalel(CastedKeys, Spline->IsClosedLoop());
    if (AllCatenaryPoints.IsEmpty()) return;

    //Move current spline meshes to the reuse array
    AvailableSplineMeshes = AllSplineMeshes;
    AllSplineMeshes.Reset();

    for (int32 i = 0; i < AllWires.Num(); i++)
    {
        //Assign spline points
        AllWires[i]->SetSplinePoints(AllCatenaryPoints[i], ESplineCoordinateSpace::Local);

        // Constructs spline meshes along each spline
        ConstructSplineMeshesAlongSplines(AllWires[i]);
    }

    // Remove exces spline meshes
    for (USplineMeshComponent* ExtraSplineMesh : AvailableSplineMeshes)
    {
        ExtraSplineMesh->DestroyComponent();
    }

}

void ASplineUtilityPole::GeneratePoles()
{
    // Generate transforms
    TArray<FTransform> Transforms = USplineHelpers::GetTransformPointsAlongSpline(Spline, DistanceBetweenObjects);

    if (bIsClosedLoop)
        Transforms.Pop();

    // Remove exces poles
    RemoveExcesPoles(Transforms.Num());

    // Create or reuse poles and assign its transforms
    ReuseOrCreatePoles(Transforms);

}

void ASplineUtilityPole::RemoveExcesPoles(int32 NeededPoleCount)
{

    if (NeededPoleCount < PoleIndices.Num())
    {
        TArray<UActorComponent*> ComponentsToDestroy;
        for (int32 i = NeededPoleCount; i < PoleIndices.Num(); i++)
        {
            ComponentsToDestroy.Add(PoleIndices[i]);
        }

        PoleIndices.SetNum(NeededPoleCount);

        for (UActorComponent* Component : ComponentsToDestroy)
        {
            if (Component)
            {
                Component->DestroyComponent();
            }
        }
    }
}

void ASplineUtilityPole::ReuseOrCreatePoles(TArray<FTransform> AllPoleTransforms)
{
    int32 i = 0;

    for (FTransform& Transform : AllPoleTransforms)
    {

        if (bSnapToTerrain)
        {
            SnapToTerrain(Transform, Transform);
        }

        if (RandomTilt != 0)
        {
            RandomizeTilt(Transform, Transform);
        }

        UChildActorComponent* ExistingPole = i < PoleIndices.Num() ? PoleIndices[i] : nullptr;

        if (ExistingPole)
        {
            if (ExistingPole->StaticClass() != PresetClass)
                ExistingPole->SetChildActorClass(PresetClass);

            ExistingPole->PrimaryComponentTick.bCanEverTick = false;
            ExistingPole->SetRelativeTransform(Transform);
        }
        else
        {
            FString PoleName = FString::Printf(TEXT("UtilityPole%i"), i);
            UChildActorComponent* NewPole = NewObject<UChildActorComponent>(this, *PoleName);
            NewPole->SetChildActorClass(PresetClass);
            NewPole->RegisterComponent();
            NewPole->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
            NewPole->SetRelativeTransform(Transform);
            NewPole->PrimaryComponentTick.bCanEverTick = false;

            if (PoleIndices.IsValidIndex(i))
                PoleIndices[i] = NewPole;
            else
                PoleIndices.Add(NewPole);
        }

        i++;

    }
}

void ASplineUtilityPole::Generate()
{

    Spline->SetClosedLoop(bIsClosedLoop);

    GeneratePoles();
    GenerateWires();

    if (bCleanupSplines) CleanupSplines();
}
