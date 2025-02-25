/*
 * Copyright (c) 2025 Sora Mas
 * All rights reserved.
 */

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UtilityPolePreset.generated.h"

UCLASS()
class UTILITYPOLES_API AUtilityPolePreset : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUtilityPolePreset();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	UStaticMeshComponent* SMComponent;

	UPROPERTY(EditAnywhere, Category = "Config")
	UStaticMesh* PoleMesh;

	UPROPERTY(EditAnywhere, meta = (MakeEditWidget = true), Category = "Config")
	TArray<FVector> WireTargets;


#if WITH_EDITOR

	UFUNCTION(CallInEditor, meta = (DisplayName = "Copy properies to class default"), Category = "Config")
	void InstancePropertiesToClassDefaults();

#endif
};
