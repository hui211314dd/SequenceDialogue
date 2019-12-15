// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
#include "DialogueSection.h"
#include "Misc/PackageName.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Styling/CoreStyle.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Text/STextBlock.h"
#include "SequencerSectionPainter.h"
#include "SDropTarget.h"
#include "DragAndDrop/LevelDragDropOp.h"
#include "ScopedTransaction.h"
#include "MovieSceneDialogueSection.h"

FDialogueSection::FDialogueSection( UMovieSceneDialogueSection& InSectionObject )
	: SectionObject( InSectionObject )
{
}

UMovieSceneSection* FDialogueSection::GetSectionObject()
{
	return &SectionObject;
}

TSharedRef<SWidget> FDialogueSection::GenerateSectionWidget()
{
	return
		SNew( SDropTarget )
		.OnAllowDrop( this, &FDialogueSection::OnAllowDrop )
		.OnDrop( this, &FDialogueSection::OnDrop )
		.Content()
		[
			SNew( SBorder )
			.BorderBackgroundColor( this, &FDialogueSection::GetBackgroundColor )
			.BorderImage( FCoreStyle::Get().GetBrush( "WhiteBrush" ) )
			[
				SNew( STextBlock )
				.Text( this, &FDialogueSection::GetVisibilityText )
				.ToolTipText( this, &FDialogueSection::GetVisibilityToolTip )
			]
		];
}


int32 FDialogueSection::OnPaintSection( FSequencerSectionPainter& InPainter ) const
{
	return InPainter.PaintSectionBackground();
}


FText FDialogueSection::GetSectionTitle() const
{
	return FText();
}


FSlateColor FDialogueSection::GetBackgroundColor() const
{
	return FSlateColor(FLinearColor::Green.Desaturate(.5f));
}


FText FDialogueSection::GetVisibilityText() const
{
	return SectionObject.GetDialogueInfo();
	//return FText::Format(NSLOCTEXT("DialogueTrackEditor","DialogueSectionText", "{0}:{1}"),SectionObject.GetDialogueSpeakerName(),SectionObject.GetDialogueInfo());
}

FText FDialogueSection::GetVisibilityToolTip() const
{
	return FText::Format(NSLOCTEXT("DialogueTrackEditor", "DialogueSectionText", "{0}:\n{1}"), SectionObject.GetDialogueSpeakerName(), SectionObject.GetDialogueInfo());
}


bool FDialogueSection::OnAllowDrop( TSharedPtr<FDragDropOperation> DragDropOperation )
{
	return false;
}


FReply FDialogueSection::OnDrop( TSharedPtr<FDragDropOperation> DragDropOperation )
{
	return FReply::Unhandled();
}

