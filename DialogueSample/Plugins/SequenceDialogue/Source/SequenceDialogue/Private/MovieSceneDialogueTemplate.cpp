// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
#include "MovieSceneDialogueTemplate.h"
#include "Engine/EngineTypes.h"
#include "Engine/World.h"
#include "Misc/PackageName.h"
#include "MovieSceneEvaluation.h"
#include "IMovieScenePlayer.h"
#include "DialogueInterface.h"
#include "GameFramework/GameModeBase.h"


#if WITH_EDITOR
#include "LevelEditor.h"
#include "ILevelViewport.h"
#include "Editor.h"
#include "SceneViewport.h"
#include "EditorStyleSet.h"

class SDialogueInfoWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SDialogueInfoWidget) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs)
	{
		SetVisibility(EVisibility::SelfHitTestInvisible);

		ChildSlot
			[
				SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.FillWidth(1)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Bottom)
			.Padding(4.f, 4.f, 4.f,150.f)
			[
				SNew(SBorder)
				.Content()
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
			.AutoWidth()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Bottom)
			[
				SNew(STextBlock)
				.Visibility(EVisibility::Visible)
			    .TextStyle(FEditorStyle::Get(), "ContentBrowser.TopBar.Font")
				.Text(this, &SDialogueInfoWidget::GetSpeakerName)
			]

		+ SHorizontalBox::Slot()
			.AutoWidth()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Bottom)
			[
				SNew(STextBlock)
				.Visibility(EVisibility::Visible)
			    .TextStyle(FEditorStyle::Get(), "ContentBrowser.TopBar.Font")
				.Text(this, &SDialogueInfoWidget::GetDialogueInfo)
			]
			]
			]
			];
	}

	FText GetSpeakerName() const
	{
		FString ret = SpeakerName.ToString() + TEXT(": ");
		return FText::FromString(ret);
	}

	FText GetDialogueInfo() const
	{
		return DialogueInfo;
	}

	virtual bool SupportsKeyboardFocus() const override
	{
		return false;
	}

	FText DialogueInfo;
	FText SpeakerName;
};
#endif

struct FDialogueSharedTrackData : IPersistentEvaluationData
{
	FDialogueSharedTrackData():bNeedExecute(false)
	{}

	~FDialogueSharedTrackData()
	{
#if WITH_EDITOR
		SWidget* Widget = ViewportWidget.Pin().Get();
		SOverlay* Overlay = (SOverlay*)Widget;

		if (Overlay && DialogueWidget.IsValid())
		{
			Overlay->RemoveSlot(DialogueWidget.ToSharedRef());
		}

		Widget = nullptr;
		DialogueWidget = nullptr;
#endif
	}

	bool HasAnythingToDo() const
	{
		return bNeedExecute;
	}

	void SetInfo(bool show,const FText& info, const FText& name)
	{
		bShow = show;
		DialogueInfo = info;
		SpeakerName = name;
		bNeedExecute = true;
	}

	void Apply(IMovieScenePlayer& Player)
	{
		bNeedExecute = false;

		UWorld* curWorld = nullptr;
		if (Player.GetPlaybackContext())
		{
			curWorld = Player.GetPlaybackContext()->GetWorld();
		}
		else
		{
			return;
		}

#if WITH_EDITOR
		if (GIsEditor && !curWorld->IsPlayInEditor())
		{
			//编辑模式;
			if (bShow)
			{
				if (!DialogueWidget.IsValid())
				{
					FSceneViewport* SceneViewport = nullptr;
					FLevelEditorViewportClient* LevelEditorViewportClient = GEditor->LevelViewportClients[0];
					const TIndirectArray<FWorldContext>& WorldContexts = GEditor->GetWorldContexts();
					//TODO: Handle multiple viewports?
					if (WorldContexts.Num() > 0)
					{
						UEditorEngine* EditorEngine = CastChecked<UEditorEngine>(GEngine);
						FSceneViewport* EditorViewport = (FSceneViewport*)EditorEngine->GetActiveViewport();
						if (EditorViewport != nullptr)
						{
							SceneViewport = EditorViewport;
						}
						
						if (SceneViewport)
						{
							if (SceneViewport->GetViewportWidget().IsValid())
							{
								//Viewport = SceneViewport->GetViewportWidget();
								UWorld* World = WorldContexts[0].World();
								DialogueWidget = SNew(SDialogueInfoWidget);
								ViewportWidget = SceneViewport->GetViewportWidget().Pin()->GetContent();
								SWidget* Widget = ViewportWidget.Pin().Get();
								SOverlay* Overlay = (SOverlay*)Widget;
								Overlay->AddSlot()
									[
										DialogueWidget.ToSharedRef()
									];
							}
						}
					}
				}

				if (DialogueWidget.IsValid())
				{
					DialogueWidget.Get()->SpeakerName = SpeakerName;
					DialogueWidget.Get()->DialogueInfo = DialogueInfo;
				}
			}
			else
			{
				SWidget* Widget = ViewportWidget.Pin().Get();
				SOverlay* Overlay = (SOverlay*)Widget;

				if (Overlay && DialogueWidget.IsValid())
				{
					Overlay->RemoveSlot(DialogueWidget.ToSharedRef());
				}
				
				Widget = nullptr;
				DialogueWidget = nullptr;
			}
		}
		else
#endif
		{
			if (curWorld)
			{
				AGameModeBase* curGameMode = curWorld->GetAuthGameMode();
				const IDialogueInterface* RawInterface = Cast<IDialogueInterface>((UObject*)curGameMode);
				const bool bHasInterface = RawInterface || (curGameMode && curGameMode->GetClass()->ImplementsInterface(UDialogueInterface::StaticClass()));

				if (bHasInterface)
				{
					RawInterface ? RawInterface->ShowDialogueInfo(bShow, SpeakerName, DialogueInfo):
						IDialogueInterface::Execute_ShowDialogueInfo(curGameMode,bShow, SpeakerName, DialogueInfo);
				}
			}
		}
	}

private:
	bool bNeedExecute;
	bool bShow;

	FText DialogueInfo;
	FText SpeakerName;

#if WITH_EDITOR
	TWeakPtr<SWidget> ViewportWidget;
	//TWeakPtr<ILevelViewport> Viewport;
	TSharedPtr<SDialogueInfoWidget> DialogueWidget;
#endif
};


struct FDialogueExecutionToken : IMovieSceneSharedExecutionToken
{
	virtual void Execute(FPersistentEvaluationData& PersistentData, IMovieScenePlayer& Player) override
	//virtual void Execute(const FMovieSceneContext& Context, const FMovieSceneEvaluationOperand& Operand, FPersistentEvaluationData& PersistentData, IMovieScenePlayer& Player) override
	{
		FDialogueSharedTrackData* TrackData = PersistentData.Find<FDialogueSharedTrackData>(FMovieSceneDialogueSharedTrack::GetSharedDataKey());
		if (TrackData)
		{
			TrackData->Apply(Player);
		}
	}
};

FMovieSceneDialogueSectionTemplate::FMovieSceneDialogueSectionTemplate(const UMovieSceneDialogueSection& Section)
	: DialogueInfo(Section.GetDialogueInfo()),SpeakerName(Section.GetDialogueSpeakerName())
{
}

void FMovieSceneDialogueSectionTemplate::Evaluate(const FMovieSceneEvaluationOperand& Operand, const FMovieSceneContext& Context, const FPersistentEvaluationData& PersistentData, FMovieSceneExecutionTokens& ExecutionTokens) const
{
	FMovieSceneSharedDataId shared_id = FMovieSceneDialogueSharedTrack::GetSharedDataKey().UniqueId;
	const FDialogueSharedTrackData* TrackData = PersistentData.Find<FDialogueSharedTrackData>(FMovieSceneDialogueSharedTrack::GetSharedDataKey());
	if (TrackData && TrackData->HasAnythingToDo() && !ExecutionTokens.FindShared(shared_id))
	{
		ExecutionTokens.AddShared(shared_id,FDialogueExecutionToken());
	}
}

void FMovieSceneDialogueSectionTemplate::Setup(FPersistentEvaluationData& PersistentData, IMovieScenePlayer& Player) const
{
	FDialogueSharedTrackData& TrackData = PersistentData.GetOrAdd<FDialogueSharedTrackData>(FMovieSceneDialogueSharedTrack::GetSharedDataKey());
	TrackData.SetInfo(true, DialogueInfo,SpeakerName);
}

void FMovieSceneDialogueSectionTemplate::TearDown(FPersistentEvaluationData& PersistentData, IMovieScenePlayer& Player) const
{
	FDialogueSharedTrackData& TrackData = PersistentData.GetOrAdd<FDialogueSharedTrackData>(FMovieSceneDialogueSharedTrack::GetSharedDataKey());
	TrackData.SetInfo(false, DialogueInfo, SpeakerName);
	TrackData.Apply(Player);
}

FSharedPersistentDataKey FMovieSceneDialogueSharedTrack::GetSharedDataKey()
{
	static FMovieSceneSharedDataId DataId(FMovieSceneSharedDataId::Allocate());
	return FSharedPersistentDataKey(DataId, FMovieSceneEvaluationOperand());
}

void FMovieSceneDialogueSharedTrack::TearDown(FPersistentEvaluationData& PersistentData, IMovieScenePlayer& Player) const
{
/*
	FDialogueSharedTrackData* TrackData = PersistentData.Find<FDialogueSharedTrackData>(GetSharedDataKey());
	if (TrackData)
	{
		FText noused;
		TrackData->SetInfo(false, noused,noused);
		TrackData->Apply(Player);

		PersistentData.Reset(GetSharedDataKey());
	}
*/
}

void FMovieSceneDialogueSharedTrack::Evaluate(const FMovieSceneEvaluationOperand& Operand, const FMovieSceneContext& Context, const FPersistentEvaluationData& PersistentData, FMovieSceneExecutionTokens& ExecutionTokens) const
{
/*
	const FDialogueSharedTrackData* TrackData = PersistentData.Find<FDialogueSharedTrackData>(GetSharedDataKey());
	if (TrackData && TrackData->HasAnythingToDo())
	{
		ExecutionTokens.Add(FDialogueExecutionToken());
	}
*/
}
