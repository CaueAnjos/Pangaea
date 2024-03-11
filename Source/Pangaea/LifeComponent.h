#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LifeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PANGAEA_API ULifeComponent : public UActorComponent
{
	GENERATED_BODY()
public:	
	ULifeComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LifeStatus", meta = (ClampMin = "0"))
	float MaxLife;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LifeStatus", meta = (ClampMin = "0")
	float Defense;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
