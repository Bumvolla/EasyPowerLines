// Fill out your copyright notice in the Description page of Project Settings.


#include "UtilityPole.h"

AUtilityPole::AUtilityPole()
{
	DisableComponentsSimulatePhysics();
	PrimaryActorTick.bCanEverTick = false;
	PoleComponent = CreateDefaultSubobject<UChildActorComponent>(TEXT("UtilityPole"));
	SetRootComponent(PoleComponent);

	bRunConstructionScriptOnDrag = false;
}

void AUtilityPole::Generate()
{
	GeneratePole();
	GenerateWires();

	if (bCleanupSplines) CleanupSplines();

#if WITH_EDITOR
	PostEditChange();
	GEditor->RedrawLevelEditingViewports();
#endif
}

void AUtilityPole::GeneratePole()
{
	if (PoleComponent->StaticClass() != PresetClass)
		PoleComponent->SetChildActorClass(PresetClass);

	FTransform Transform = GetActorTransform();

	if (bSnapToTerrain)
		SnapToTerrain(Transform, Transform, ECoordSystem::COORD_World);

	if (RandomTilt != 0)
		RandomizeTilt(Transform, Transform);

	SetActorTransform(Transform);
	MarkComponentsRenderStateDirty();

}

void AUtilityPole::GenerateWires()
{
	if (!WireMesh) return;

	SetMeshLenght();

	AUtilityPolePreset* CastedUtilityPole = Cast<AUtilityPolePreset>(PoleComponent->GetChildActor());
	AUtilityPolePreset* OtherCastedPole = Cast<AUtilityPolePreset>(ActorToConectTo);

	if ((CastedUtilityPole->StaticClass() != ActorToConectTo->StaticClass()) || !CastedUtilityPole || !ActorToConectTo)
		return;

	const TArray<AUtilityPolePreset*> PolesToConect = {CastedUtilityPole, OtherCastedPole };

	const uint8 TransformsAmount = CastedUtilityPole->WireTargets.Num();

	//Handles exces previously generated splines 
	RemoveExcesSplines(TransformsAmount);

	// Create or reuse spline components
	ReuseOrCreateSplines();

	TArray<TArray<FVector>> AllCatenaryPoints = CalculateCatenariesParalel(PolesToConect);

	//Assign spline points
	for (int32 i = 0; i < AllWires.Num(); i++)
	{
		AllWires[i]->SetSplinePoints(AllCatenaryPoints[i], ESplineCoordinateSpace::Local);
	}

	//Move current spline meshes to the reuse array
	AvailableSplineMeshes = AllSplineMeshes;
	AllSplineMeshes.Reset();

	// Constructs spline meshes along each spline
	for (int32 i = 0; i < AllWires.Num(); i++)
	{
		ConstructSplineMeshesAlongSplines(AllWires[i]);
	}

	// Remove exces spline meshes
	for (USplineMeshComponent* ExtraSplineMesh : AvailableSplineMeshes)
	{
		ExtraSplineMesh->DestroyComponent();
	}

}
