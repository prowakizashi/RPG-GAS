#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "RPGAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API URPGAssetManager : public UAssetManager
{
	GENERATED_BODY()
	
public:
	static const URPGAssetManager& Get();

protected:
	virtual void StartInitialLoading() override;
};
