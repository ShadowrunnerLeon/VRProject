// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GrabCube.generated.h"

class UStaticMeshComponent;
class UGrabComponent;

UCLASS()
class VRPROJECT_API AGrabCube : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Mesh")
	class UStaticMeshComponent* mesh;

	UPROPERTY(EditDefaultsOnly, Category = "Grab")
	class UGrabComponent* grabComponent;
	
public:	
	// Sets default values for this actor's properties
	AGrabCube();
	class UGrabComponent* GetGrabComponent();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
