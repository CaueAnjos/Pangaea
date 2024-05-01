#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AmbushZone.generated.h"

UCLASS()
class PANGAEA_API AAmbushZone : public AActor
{
	GENERATED_BODY()

public:	
	AAmbushZone();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
