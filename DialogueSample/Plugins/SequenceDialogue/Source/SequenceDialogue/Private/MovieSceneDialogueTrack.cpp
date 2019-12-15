// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "MovieSceneDialogueTrack.h"
#include "MovieSceneDialogueSection.h"
#include "Compilation/IMovieSceneTemplateGenerator.h"
#include "Evaluation/PersistentEvaluationData.h"
#include "Evaluation/MovieSceneEvaluationTrack.h"
#include "MovieSceneDialogueTemplate.h"

#define LOCTEXT_NAMESPACE "MovieSceneDialogueTrack"

void UMovieSceneDialogueTrack::PostCompile(FMovieSceneEvaluationTrack& OutTrack, const FMovieSceneTrackCompilerArgs& Args) const
{
	// Set priority to highest possible
	OutTrack.SetEvaluationPriority(GetEvaluationPriority());
}


bool UMovieSceneDialogueTrack::IsEmpty() const
{
	return Sections.Num() == 0;
}


void UMovieSceneDialogueTrack::AddSection(UMovieSceneSection& Section)
{
	Sections.Add(&Section);
}


void UMovieSceneDialogueTrack::RemoveSection( UMovieSceneSection& Section )
{
	Sections.Remove(&Section);
}


UMovieSceneSection* UMovieSceneDialogueTrack::CreateNewSection()
{
	UMovieSceneDialogueSection* new_section = NewObject<UMovieSceneDialogueSection>(this, UMovieSceneDialogueSection::StaticClass(), NAME_None, RF_Transactional);

	return new_section;
}


const TArray<UMovieSceneSection*>& UMovieSceneDialogueTrack::GetAllSections() const
{
	return Sections;
}


TRange<float> UMovieSceneDialogueTrack::GetSectionBoundaries() const
{
	TArray< TRange<float> > Bounds;

	for (int32 SectionIndex = 0; SectionIndex < Sections.Num(); ++SectionIndex)
	{
		Bounds.Add(Sections[SectionIndex]->GetRange());
	}

	return TRange<float>::Hull(Bounds);
}


bool UMovieSceneDialogueTrack::HasSection(const UMovieSceneSection& Section) const
{
	return Sections.Contains(&Section);
}

UMovieSceneDialogueSection* UMovieSceneDialogueTrack::GetSectionByID(int id)
{
	for (UMovieSceneSection* curSection : Sections)
	{
		UMovieSceneDialogueSection* dialogueSection = Cast<UMovieSceneDialogueSection>(curSection);

		if (dialogueSection && dialogueSection->GetUniqueID() == id)
		{
			return dialogueSection;
		}
	}

	return nullptr;
}
#if WITH_EDITORONLY_DATA
FText UMovieSceneDialogueTrack::GetDefaultDisplayName() const
{
	return LOCTEXT("DisplayName", "Dialogue");
}
#endif

#undef LOCTEXT_NAMESPACE
