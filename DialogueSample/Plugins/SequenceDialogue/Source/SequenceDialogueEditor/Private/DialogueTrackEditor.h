// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Misc/Guid.h"
#include "Templates/SubclassOf.h"
#include "Widgets/SWidget.h"
#include "ISequencer.h"
#include "MovieSceneTrack.h"
#include "ISequencerSection.h"
#include "ISequencerTrackEditor.h"
#include "MovieSceneTrackEditor.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
class FMenuBuilder;

class FDialogueTrackEditor
	: public FMovieSceneTrackEditor
{
public:
	FDialogueTrackEditor( TSharedRef<ISequencer> InSequencer );

	virtual ~FDialogueTrackEditor() { }
	static TSharedRef<ISequencerTrackEditor> CreateTrackEditor( TSharedRef<ISequencer> OwningSequencer );

public:

	virtual TSharedRef<ISequencerSection> MakeSectionInterface( UMovieSceneSection& SectionObject, UMovieSceneTrack& Track, FGuid ObjectBinding ) override;
	virtual bool SupportsType( TSubclassOf<UMovieSceneTrack> Type ) const override;
	virtual const FSlateBrush* GetIconBrush() const override;
	virtual void BuildAddTrackMenu( FMenuBuilder& MenuBuilder ) override;
	TSharedPtr<SWidget> BuildOutlinerEditWidget( const FGuid& ObjectBinding, UMovieSceneTrack* Track, const FBuildEditWidgetParams& Params ) override;

	virtual void BuildTrackContextMenu(FMenuBuilder& MenuBuilder, UMovieSceneTrack* Track) override;
	
private:

	void AddNewSection(UMovieScene* MovieScene, UMovieSceneTrack* LevelVisibilityTrack);
	/** Handles when the add track menu item is activated. */
	void OnAddTrack();

	TSharedRef<SWidget> BuildAddVisibilityTriggerMenu( UMovieSceneTrack* LevelVisibilityTrack );

	void OnAddNewSection( UMovieSceneTrack* LevelVisibilityTrack);
};
