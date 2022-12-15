// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "ModuleManager.h"
#include "SlateBasics.h"
#include "SlateExtras.h"

class FToolBarBuilder;
class FMenuBuilder;

struct InputData
{
	int MeshType;
	float X;
	float Y;
};

class FDNKPluginModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	/** This function will be bound to Command (by default it will bring up plugin window) */
	void PluginButtonClicked();
	void DNKBegin();
	float DNKCheckHeightOfLandscape(float x, float y, UWorld* World);

	float maxH = 500;
	float minH = 100;

	const float minHLandscape = -99999999.0f;	// -1000 km
	const float maxHLandscape = 99999999.0f; // 1000 km


	FString pathTxtFile = "C:/Users/DNK/Desktop/testCoordinates.txt";

	FString ref1 = "StaticMesh'/Game/Box_Mesh.Box_Mesh'";
	FString ref2 = "StaticMesh'/Game/Cone_Mesh.Cone_Mesh'";
	FString ref3 = "StaticMesh'/Game/Cylinder_Mesh.Cylinder_Mesh'";
	FString ref4 = "StaticMesh'/Game/Sphere_Mesh.Sphere_Mesh'";
	FString ref5 = "StaticMesh'/Game/Stair_Mesh.Stair_Mesh'";

private:

	void AddToolbarExtension(FToolBarBuilder& Builder);
	void AddMenuExtension(FMenuBuilder& Builder);

	TSharedRef<class SDockTab> OnSpawnPluginTab(const class FSpawnTabArgs& SpawnTabArgs);

private:
	TSharedPtr<class FUICommandList> PluginCommands;
};