// Fill out your copyright notice in the Description page of Project Settings.


#include "PoleBasedCatenary.h"

void APoleBasedCatenary::SnapToTerrain(const FTransform& OgTransform, FTransform& SnapedTransform, ECoordSystem CoordSystem)
{
    FVector WorldLocation = OgTransform.GetLocation();
    if (CoordSystem != ECoordSystem::COORD_World)
    {
        // Convert to worldspace for raycasting
        WorldLocation = GetActorTransform().TransformPosition(OgTransform.GetLocation());
    }
    const FVector RayStartLocation = FVector(WorldLocation.X, WorldLocation.Y, WorldLocation.Z + RayLength / 2);
    const FVector RayEndLocation = FVector(WorldLocation.X, WorldLocation.Y, WorldLocation.Z - RayLength / 2);

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

        FQuat ImpactNormalRotator;
        AlignToNormal(HitResult.ImpactNormal, ImpactNormalRotator);

        FVector ReturnPosition = HitResult.ImpactPoint;

        if (CoordSystem != ECoordSystem::COORD_World)
        {
            // Convert to worldspace for raycasting
            ReturnPosition = GetActorTransform().InverseTransformPosition(HitResult.ImpactPoint);
        }

        SnapedTransform = FTransform(bAlignToNormal ? ImpactNormalRotator : OgTransform.GetRotation(), ReturnPosition, OgTransform.GetScale3D());
    }
    else
    {
        SnapedTransform = OgTransform;
    }
}

void APoleBasedCatenary::AlignToNormal(const FVector& ImpactNormal, FQuat& NormalAlignedQuat)
{
    NormalAlignedQuat = FQuat::FindBetweenNormals(FVector::UpVector, GetActorTransform().InverseTransformVectorNoScale(ImpactNormal));
}

void APoleBasedCatenary::RandomizeTilt(const FTransform& OgTransform, FTransform& RandomRotatedTransform)
{
    FRotator storedRotator = OgTransform.Rotator();
    FRotator randomizedRotator = FRotator(FMath::RandRange(RandomTilt * -1, RandomTilt), storedRotator.Yaw, storedRotator.Roll);
    RandomRotatedTransform.SetRotation(randomizedRotator.Quaternion());
}

void APoleBasedCatenary::DrawDebugLines(FVector StartPoint, FVector EndPoint, bool bHit, FHitResult Hit)
{
    DrawDebugLine(
        GetWorld(),
        StartPoint,
        EndPoint,
        FColor::Red,
        false,
        10.f,
        0,
        5.f
    );
    if (bHit)
    {

        DrawDebugLine(
            GetWorld(),
            Hit.Location,
            Hit.Location,
            FColor::Green, false,
            10.f,
            0,
            20.f);

        DrawDebugLine(
            GetWorld(),
            Hit.Location,
            Hit.Location + Hit.ImpactNormal * 50.f,
            FColor::Blue, false,
            10.f,
            0,
            5.f);
    }
}
