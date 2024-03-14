#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PangaeaGameInstance.generated.h"

UCLASS()
class PANGAEA_API UPangaeaGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:	
	UFUNCTION(BlueprintCallable, Category = "NetWork")
	void StartListenServer();
	
	UFUNCTION(BlueprintCallable, Category = "NetWork")
	void JoinAsClient(FString IPAdress);

	UFUNCTION(BlueprintCallable, Category = "NetWork")
	void LeaveGame();

};
