// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "SequenceDialogueEditor.h"
#include "Modules/ModuleManager.h"
#include "ISequencerModule.h"
#include "DialogueTrackEditor.h"

#define LOCTEXT_NAMESPACE "FSequenceDialogueEditorModule"

void FSequenceDialogueEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	ISequencerModule& SequencerModule = FModuleManager::Get().LoadModuleChecked<ISequencerModule>("Sequencer");
	dialogueTrackEditorHandle = SequencerModule.RegisterTrackEditor(FOnCreateTrackEditor::CreateStatic(&FDialogueTrackEditor::CreateTrackEditor));
}

void FSequenceDialogueEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	if (!FModuleManager::Get().IsModuleLoaded("Sequencer"))
	{
		return;
	}

	ISequencerModule& SequencerModule = FModuleManager::Get().GetModuleChecked<ISequencerModule>("Sequencer");

	// unregister property track editors
	SequencerModule.UnRegisterTrackEditor(dialogueTrackEditorHandle);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSequenceDialogueEditorModule, SequenceDialogueEditor)