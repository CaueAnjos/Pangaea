#include "PangaeaGameInstance.h"
#include "Kismet/GameplayStatics.h"

void UPangaeaGameInstance::StartListenServer()
{
	UWorld* world = GEngine->GetCurrentPlayWorld();
	UGameplayStatics::OpenLevel(world, TEXT("FrozenCove"), true, TEXT("?listen"));
}

void UPangaeaGameInstance::JoinAsClient(FString IPAdress)
{
	UWorld* world = GEngine->GetCurrentPlayWorld();
	UGameplayStatics::OpenLevel(world, *IPAdress, true, TEXT("?join"));
}

void UPangaeaGameInstance::LeaveGame()
{
	UWorld* world = GEngine->GetCurrentPlayWorld();
	UGameplayStatics::OpenLevel(world, TEXT("LobbyMap"));
}
