// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "DNKPluginPrivatePCH.h"

#include "DNKPluginStyle.h"
#include "DNKPluginCommands.h"

#include <fstream> // for file parsing
#include <UnrealEd.h> // for getting scene in viewport

#include "LevelEditor.h"

static const FName DNKPluginTabName("DNKPlugin");

#define LOCTEXT_NAMESPACE "FDNKPluginModule"

void FDNKPluginModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	FDNKPluginStyle::Initialize();
	FDNKPluginStyle::ReloadTextures();

	FDNKPluginCommands::Register();

	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FDNKPluginCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FDNKPluginModule::PluginButtonClicked),
		FCanExecuteAction());

	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");

	{
		TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender());
		MenuExtender->AddMenuExtension("WindowLayout", EExtensionHook::After, PluginCommands, FMenuExtensionDelegate::CreateRaw(this, &FDNKPluginModule::AddMenuExtension));

		LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);
	}

	{
		TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
		ToolbarExtender->AddToolBarExtension("Settings", EExtensionHook::After, PluginCommands, FToolBarExtensionDelegate::CreateRaw(this, &FDNKPluginModule::AddToolbarExtension));

		LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);
	}

	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(DNKPluginTabName, FOnSpawnTab::CreateRaw(this, &FDNKPluginModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FDNKPluginTabTitle", "DNKPlugin"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);

	DNKBegin();
}

void FDNKPluginModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FDNKPluginStyle::Shutdown();

	FDNKPluginCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(DNKPluginTabName);
}

TSharedRef<SDockTab> FDNKPluginModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
				.Padding(2.0f)
				.HAlign(HAlign_Center)
				.AutoHeight()
				[
					SNew(STextBlock)
					.Text(FText::FromString(TEXT("RANDOM RANGE HEIGHT OF MESHES")))
				]

			+ SVerticalBox::Slot()
				.HAlign(HAlign_Center)
				.AutoHeight()
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
						[
							SNew(STextBlock)
							.Text(FText::FromString(TEXT("MAX Height")))
							.MinDesiredWidth(100)
						]
					+ SHorizontalBox::Slot()
						[
							SNew(SNumericEntryBox<float>)
							.MinDesiredValueWidth(100)
							.MinSliderValue(0.0f)
							.MaxSliderValue(10000.0f)
							.OnValueChanged_Lambda([&](float val) { maxH = val; })
							.Value_Lambda([&] { return maxH; })
						]
				]

			+ SVerticalBox::Slot()
				.HAlign(HAlign_Center)
				.AutoHeight()
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
						[
							SNew(STextBlock)
							.Text(FText::FromString(TEXT("MIN Height")))
							.MinDesiredWidth(100)
						]
					+ SHorizontalBox::Slot()
						[
							SNew(SNumericEntryBox<float>)
							.MinDesiredValueWidth(100)
							.MinSliderValue(0.0f)
							.MaxSliderValue(10000.0f)
							.OnValueChanged_Lambda([&](float val) { minH = val; })
							.Value_Lambda([&] { return minH; })
						]
				]

			+ SVerticalBox::Slot()
				.Padding(2.0f)
				.HAlign(HAlign_Center)
				.AutoHeight()
				[
					SNew(STextBlock)
					.Text(FText::FromString(TEXT("PATH TO TXT FILE")))
				]

			+ SVerticalBox::Slot()
				.HAlign(HAlign_Center)
				.AutoHeight()
				[
					SNew(SEditableTextBox)
					.Text(FText::FromString(pathTxtFile))
					.MinDesiredWidth(777)
					.OnTextChanged_Lambda([&](FText p) {pathTxtFile = p.ToString(); })
				]

			+ SVerticalBox::Slot()
				.Padding(2.0f)
				.HAlign(HAlign_Center)
				.AutoHeight()
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
						[
							SNew(SButton)
							.Text(FText::FromString(TEXT("Generate")))
							.OnClicked_Lambda([&]{DNKBegin(); return FReply::Handled(); })
						]
				]

			+ SVerticalBox::Slot()
				.Padding(2.0f)
				.HAlign(HAlign_Center)
				.AutoHeight()
				[
					SNew(STextBlock)
					.Text(FText::FromString(TEXT("REFERENCES TO TYPES")))
				]

			+ SVerticalBox::Slot()
				.Padding(2.0f)
				.HAlign(HAlign_Left)
				.AutoHeight()
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
						.AutoWidth()
						[
							SNew(STextBlock)
							.Text(FText::FromString(TEXT("Type 1 : ")))
						]
					+ SHorizontalBox::Slot()
						[
							SNew(SEditableTextBox)
							.MinDesiredWidth(500)
							.Text(FText::FromString(ref1))
							.OnTextChanged_Lambda([&](FText p) {ref1 = p.ToString(); })
						]
				]

			+ SVerticalBox::Slot()
				.Padding(2.0f)
				.HAlign(HAlign_Left)
				.AutoHeight()
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
						.AutoWidth()
						[
							SNew(STextBlock)
							.Text(FText::FromString(TEXT("Type 2 : ")))
						]
					+ SHorizontalBox::Slot()
						[
							SNew(SEditableTextBox)
							.MinDesiredWidth(500)
							.Text(FText::FromString(ref2))
							.OnTextChanged_Lambda([&](FText p) {ref2 = p.ToString(); })
						]
				]

			+ SVerticalBox::Slot()
				.Padding(2.0f)
				.HAlign(HAlign_Left)
				.AutoHeight()
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
						.AutoWidth()
						[
							SNew(STextBlock)
							.Text(FText::FromString(TEXT("Type 3 : ")))
						]
					+ SHorizontalBox::Slot()
						[
							SNew(SEditableTextBox)
							.MinDesiredWidth(500)
							.Text(FText::FromString(ref3))
							.OnTextChanged_Lambda([&](FText p) {ref3 = p.ToString(); })
						]
				]

			+ SVerticalBox::Slot()
				.Padding(2.0f)
				.HAlign(HAlign_Left)
				.AutoHeight()
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
						.AutoWidth()
						[
							SNew(STextBlock)
							.Text(FText::FromString(TEXT("Type 4 : ")))
						]
					+ SHorizontalBox::Slot()
						[
							SNew(SEditableTextBox)
							.Text(FText::FromString(ref4))
							.MinDesiredWidth(500)
							.OnTextChanged_Lambda([&](FText p) {ref4 = p.ToString(); })
						]
				]

			+ SVerticalBox::Slot()
				.Padding(2.0f)
				.HAlign(HAlign_Left)
				.AutoHeight()
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
						.AutoWidth()
						[
							SNew(STextBlock)
							.Text(FText::FromString(TEXT("Type 5 : ")))
						]
					+ SHorizontalBox::Slot()
						[
							SNew(SEditableTextBox)
							.Text(FText::FromString(ref5))
							.MinDesiredWidth(500)
							.OnTextChanged_Lambda([&](FText p) {ref5 = p.ToString(); })
						]
				]

		];
}

void FDNKPluginModule::DNKBegin()
{
	UE_LOG(LogTemp, Warning, TEXT("Button pressed. maxH : %f ; minH : %f \n"), maxH, minH);
	UE_LOG(LogTemp, Warning, TEXT("%s"), *pathTxtFile);

	using namespace std;

	InputData t;

	ifstream file(*pathTxtFile);

	if (!file.is_open())
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot open file : %s"), *pathTxtFile);
	}
	else
	{
		char *str = new char[1024];
		int NumStr = 0;

		while (!file.eof())
		{
			file.getline(str, 1024, '\n');
			NumStr++;
		}

		UE_LOG(LogTemp, Warning, TEXT("Number of strings in file : %d"), NumStr);

		delete str;
		file.clear();
		file.seekg(0, ios::beg);

		if (GEditor)
		{
			// UWorld* World = GEditor->GetEditorWorldContext().World()->GetCurrentLevel()->GetWorld();
			UWorld* World = GEditor->GetEditorWorldContext().World();

			if (World)
			{

				float maxAbsX, maxAbsY;

				for (TActorIterator<AActor> ActorItr(World); ActorItr; ++ActorItr)
				{
					AActor * Actor = *ActorItr;

					if (ActorItr->GetName().Contains(FString("Landscape")) && !ActorItr->GetName().Contains("Gizmo"))
					{
						maxAbsX = FMath::Abs(ActorItr->GetActorLocation().X);
						maxAbsY = FMath::Abs(ActorItr->GetActorLocation().Y);

						UE_LOG(LogTemp, Warning, TEXT("maxAbsX : %f ; maxAbsY : %f ;"), maxAbsX, maxAbsY);

						break;
					}
				}

				TArray<AStaticMeshActor*> stMeshArray;
				char c;

				for (int i = 0; i < NumStr; i++)
				{
					file >> t.MeshType >> c
						>> t.X >> c
						>> t.Y;

					if ( (t.X > -maxAbsX && t.X < maxAbsX ) && (t.Y > -maxAbsY && t.Y < maxAbsY) )
					{
						FString tempRef;

						switch (t.MeshType)
						{
						case 1:
							tempRef = ref1; break;
						case 2:
							tempRef = ref2; break;
						case 3:
							tempRef = ref3; break;
						case 4:
							tempRef = ref4; break;
						case 5:
							tempRef = ref5; break;
						default:
							UE_LOG(LogTemp, Warning, TEXT("Type would be chosen by defualt : 1 with ref1."));
							tempRef = ref1;
							break;
						}

						float tempHL = DNKCheckHeightOfLandscape(t.X, t.Y, World);
						FVector tempLoc = { t.X, t.Y, tempHL };

						AStaticMeshActor* TempActor = Cast<AStaticMeshActor>(GEditor->AddActor(
							World->GetCurrentLevel(), AStaticMeshActor::StaticClass(),
							FTransform(tempLoc)));
						stMeshArray.Add(TempActor);
						stMeshArray[i]->GetStaticMeshComponent()->StaticMesh = LoadObject<UStaticMesh>(nullptr, *tempRef);
						FVector minBound, maxBound;
						stMeshArray[i]->GetStaticMeshComponent()->GetLocalBounds(minBound,maxBound);

						float heightMesh = FMath::Abs(minBound.Z - maxBound.Z);
						float scaleZMesh = FMath::FRandRange(minH, maxH) / heightMesh;
						FVector tempScale = { 1, 1, scaleZMesh };

						stMeshArray[i]->GetStaticMeshComponent()->SetWorldScale3D(tempScale);
						stMeshArray[i]->MarkComponentsRenderStateDirty();

						UE_LOG(LogTemp, Warning, TEXT("Type : %d ; X : %f ; Y : %f "), t.MeshType, t.X, t.Y);
					}
					else
						UE_LOG(LogTemp, Warning, TEXT("Mesh [Type : %d ; X : %f ; Y : %f] is beyond Landscape limits. Mesh didn't spawn."), t.MeshType, t.X, t.Y);

				}

				UE_LOG(LogTemp, Warning, TEXT("Spawn process completed!"));
			}
		}
	}
}

float FDNKPluginModule::DNKCheckHeightOfLandscape(float x, float y, UWorld* World)
{
	FHitResult Hit(ForceInit);

	FVector start(x, y, minHLandscape);
	FVector end(x, y, maxHLandscape);

	World->LineTraceSingleByObjectType(Hit, start, end, FCollisionObjectQueryParams(0),
										FCollisionQueryParams::DefaultQueryParam);

	UE_LOG(LogTemp, Warning, TEXT("Height of Landscape in [ %f , %f ] position : %f"), x, y, (Hit.Location.Z));

	return Hit.Location.Z;
}

void FDNKPluginModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->InvokeTab(DNKPluginTabName);
}

void FDNKPluginModule::AddMenuExtension(FMenuBuilder& Builder)
{
	Builder.AddMenuEntry(FDNKPluginCommands::Get().OpenPluginWindow);
}

void FDNKPluginModule::AddToolbarExtension(FToolBarBuilder& Builder)
{
	Builder.AddToolBarButton(FDNKPluginCommands::Get().OpenPluginWindow);
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FDNKPluginModule, DNKPlugin)