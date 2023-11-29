//Unreal® Engine, Copyright 1998 – 2023, Epic Games, Inc. All rights reserved.

#include "PXR_DPEditorCommands.h"

#define LOCTEXT_NAMESPACE "FPICOXREditorModule"
/* UI_COMMAND takes long for the compile to optimize */
PRAGMA_DISABLE_OPTIMIZATION
void FPICOXRDPEditorCommands::RegisterCommands()
{
 	UI_COMMAND(PluginAction, "PICOXRDPEditor", "Execute PICOXRDPEditor action", EUserInterfaceActionType::Button, FInputChord());
 	UI_COMMAND(ConnectToServer, "ConnectToServer", "ConnectToServer", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(DisconnectToServer, "DisconnectToServer", "DisconnectToServer", EUserInterfaceActionType::Button, FInputChord());
 
}
PRAGMA_ENABLE_OPTIMIZATION

#undef LOCTEXT_NAMESPACE
