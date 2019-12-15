// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
#include "MovieSceneDialogueSection.h"
#include "Evaluation/MovieSceneEvalTemplate.h"
#include "MovieSceneDialogueTemplate.h"

UMovieSceneDialogueSection::UMovieSceneDialogueSection( const FObjectInitializer& ObjectInitializer )
	: Super( ObjectInitializer )
{
}

FMovieSceneEvalTemplatePtr UMovieSceneDialogueSection::GenerateTemplate() const
{
	return FMovieSceneDialogueSectionTemplate(*this);
}


TOptional<float> UMovieSceneDialogueSection::GetKeyTime(FKeyHandle KeyHandle) const
{
	return TOptional<float>();
}


void UMovieSceneDialogueSection::SetKeyTime(FKeyHandle KeyHandle, float Time)
{
}
