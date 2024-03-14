#include "PangaeaGameInstance.h"
#include "Kismet/GameplayStatics.h"

void UPangaeaGameInstance::StartListenServer()
{
	UWorld* world = GEngine->GetCurrentPlayWorld();
	UGameplayStatics::OpenLevel(world, TEXT("TopDownMap"), true, TEXT("?listen"));
}

void UPangaeaGameInstance::JoinAsClient(FString IPAdress)
{
	UWorld* world = GEngine->GetCurrentPlayWorld();
	UGameplayStatics::OpenLevel(world, TEXT("TopDownMap"), true, TEXT("?join"));
}

void UPangaeaGameInstance::LeaveGame()
{
	UWorld* world = GEngine->GetCurrentPlayWorld();
	UGameplayStatics::OpenLevel(world, TEXT("LobbyMap"));
}
