// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Curves/KeyHandle.h"
#include "MovieSceneSection.h"
#include "MovieSceneDialogueSection.generated.h"


UCLASS(MinimalAPI)
class UMovieSceneDialogueSection
	: public UMovieSceneSection
{
	GENERATED_UCLASS_BODY()
public:
	FText GetDialogueInfo()const { return dialogueInfo; }
	void  SetDialogueInfo(const FText& info) { dialogueInfo = info; }

	FText GetDialogueSpeakerName()const { return speakerName; }
	void  SetDialogueSpeakerName(const FText& name) { speakerName = name; }
public:

	//~ UMovieSceneSection interface
	virtual FMovieSceneEvalTemplatePtr GenerateTemplate() const override;
	virtual TOptional<float> GetKeyTime(FKeyHandle KeyHandle) const override;
	virtual void SetKeyTime(FKeyHandle KeyHandle, float Time) override;
private:

	UPROPERTY(EditAnywhere, Category = Dialogue)
	FText dialogueInfo;

	UPROPERTY(EditAnywhere, Category = Dialogue)
	FText speakerName;
};
