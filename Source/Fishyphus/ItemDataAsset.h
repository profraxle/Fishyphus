// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemDataAsset.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class FISHYPHUS_API UItemDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Item Info")
	FString name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Info")
	UStaticMesh* mesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Info")
	FString description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Info")
	float meshScale;
};
