#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "WorkbenchGameMode.generated.h"
UCLASS()
class KOALITICWORKBENCH_API AWorkbenchGameMode : public AGameModeBase {
 GENERATED_BODY()
public:
 AWorkbenchGameMode();
 virtual void BeginPlay() override;
};
