// Fill out your copyright notice in the Description page of Project Settings.

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

void ASplineUtilityPole::SnapToTerrain(const FTransform& OgTransform, FTransform& SnapedTransform)
{
    const FVector OgTransformLocation = OgTransform.GetLocation();
    const FVector RayStartLocation = FVector(OgTransformLocation.X, OgTransformLocation.Y, OgTransformLocation.Z + RayLength/2);
    const FVector RayEndLocation = FVector(OgTransformLocation.X, OgTransformLocation.Y, OgTransformLocation.Z - RayLength/2);

    FHitResult HitResult;

    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(this);

    bool bHit = GetWorld()->LineTraceSingleByChannel(
        HitResult,
        RayStartLocation,
        RayEndLocation,
        CollisionChannel,
        CollisionParams
    );

    if (bDrawDebugLines)
    {
        DrawDebugLines(RayStartLocation, RayEndLocation, bHit, HitResult);
    }

    if (bHit)
    {
        const FQuat ImpactNormalRotator = FRotationMatrix::MakeFromZ(HitResult.ImpactNormal).ToQuat();
        SnapedTransform = FTransform(bAlignToNormal ? ImpactNormalRotator : OgTransform.GetRotation(), HitResult.ImpactPoint, OgTransform.GetScale3D());
    }
    else
    {
        SnapedTransform = OgTransform;
    }
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
            FRotator storedRotator = Transform.Rotator();
            FRotator randomizedRotator = FRotator(FMath::RandRange(RandomTilt * -1, RandomTilt), storedRotator.Yaw, storedRotator.Roll);
            Transform.SetRotation(randomizedRotator.Quaternion());
        }

        UChildActorComponent* ExistingPole = i < PoleIndices.Num() ? PoleIndices[i] : nullptr;

        if (ExistingPole)
        {
            if (ExistingPole->StaticClass() != PresetClass)
                ExistingPole->SetChildActorClass(PresetClass);

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

void ASplineUtilityPole::DrawDebugLines(FVector StartPoint, FVector EndPoint, bool bHit, FHitResult Hit)
{
    DrawDebugLine(
        GetWorld(),
        GetActorTransform().TransformPosition(StartPoint),
        GetActorTransform().TransformPosition(EndPoint),
        FColor::Red,
        false,
        10.f,
        0,
        5.f
    );
    if (bHit)
    {

        const FVector TransformedHitPosition = GetActorTransform().TransformPosition(Hit.ImpactPoint);

        DrawDebugLine(
            GetWorld(),
            TransformedHitPosition,
            TransformedHitPosition,
            FColor::Green, false,
            10.f,
            0,
            20.f);
    }
}

void ASplineUtilityPole::Generate()
{

    Spline->SetClosedLoop(bIsClosedLoop);

    GeneratePoles();
    GenerateWires();
}
