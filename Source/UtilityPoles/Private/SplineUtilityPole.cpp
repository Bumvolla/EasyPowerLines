// Fill out your copyright notice in the Description page of Project Settings.

#include "SplineUtilityPole.h"
#include "UtilityPoles.h"

// Sets default values
ASplineUtilityPole::ASplineUtilityPole()
{
    
	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));

	SetRootComponent(Spline);

    bRunConstructionScriptOnDrag = false;

}

void ASplineUtilityPole::GenerateWires()
{

    // Mesh check
    if (!WireMesh) return;
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

    //Handles destruction of all previously generated spline meshes
    RemoveSplineMeshes();

    //Creates and return an array that contains all catenary points for each needed wire
    TArray<TArray<FVector>> AllCatenaryPoints = CalculateCatenariesParalel(CastedKeys, Spline->IsClosedLoop());
    if (AllCatenaryPoints.IsEmpty()) return;

    //Assign spline points
    for (int32 i = 0; i < AllWires.Num(); i++)
    {
        AllWires[i]->SetSplinePoints(AllCatenaryPoints[i], ESplineCoordinateSpace::Local);
    }

    // Constructs spline meshes along each spline
    for (int32 i = 0; i < AllWires.Num();i++)
    {
        ConstructSplineMeshesAlongSplines(AllWires[i]);
    }

}

void ASplineUtilityPole::GeneratePoles()
{
    // Generate transforms
    const TArray<FTransform> Transforms = USplineHelpers::GetTransformPointsAlongSpline(Spline, DistanceBetweenObjects);

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

    for (const FTransform& Transform : AllPoleTransforms)
    {

        UChildActorComponent* ExistingPole = i < PoleIndices.Num() ? PoleIndices[i] : nullptr;

        if (ExistingPole)
        {
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
}
