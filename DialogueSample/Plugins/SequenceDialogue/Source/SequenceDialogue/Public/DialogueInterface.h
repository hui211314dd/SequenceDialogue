// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "UObject/Interface.h"
#include "DialogueInterface.generated.h"

UINTERFACE(Category = "Dialogue", Blueprintable)
class SEQUENCEDIALOGUE_API UDialogueInterface : public UInterface
{
	GENERATED_BODY()
};

class IDialogueInterface
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Dialogue", DisplayName = "ShowDialogueInfo", meta = (CallInEditor = "true"))
	void ShowDialogueInfo(bool show,const FText& name,const FText& info) const;
};


