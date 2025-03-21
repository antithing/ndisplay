// Copyright Epic Games, Inc. All Rights Reserved.

#include "DisplayClusterViewportConfiguration_Tile.h"

#include "DisplayClusterViewportConfiguration.h"
#include "DisplayClusterViewportConfigurationHelpers.h"
#include "DisplayClusterViewportConfigurationHelpers_Tile.h"

#include "Render/Viewport/DisplayClusterViewport.h"
#include "Render/Viewport/DisplayClusterViewportManager.h"
#include "Render/Viewport/DisplayClusterViewportStrings.h"
#include "Render/Viewport/RenderFrame/DisplayClusterRenderFrameSettings.h"

#include "DisplayClusterRootActor.h"
#include "DisplayClusterConfigurationTypes_Tile.h"

#include "Misc/DisplayClusterLog.h"
#include "Misc/Parse.h"

///////////////////////////////////////////////////////////////////
// FDisplayClusterViewportConfiguration_Tile
///////////////////////////////////////////////////////////////////
template <typename Predicate>
void FDisplayClusterViewportConfiguration_Tile::ForEachTileViewport(Predicate Pred) const
{
	if (FDisplayClusterViewportManager* ViewportManager = Configuration.GetViewportManagerImpl())
{
	for (const TSharedPtr<FDisplayClusterViewport, ESPMode::ThreadSafe>& ViewportIt : ViewportManager->ImplGetCurrentRenderFrameViewports())
	{
		if (ViewportIt.IsValid() && ViewportIt->GetRenderSettings().TileSettings.GetType() == EDisplayClusterViewportTileType::Source)
		{
			const FIntPoint& TileSize = ViewportIt->GetRenderSettings().TileSettings.GetSize();

				// Iterate over all tile viewports
			for (int32 PosX = 0; PosX < TileSize.X; PosX++)
			{
				for (int32 PosY = 0; PosY < TileSize.Y; PosY++)
				{
						::Invoke(Pred, *ViewportIt, FIntPoint(PosX, PosY));
				}
			}
		}
	}
	}
	else
	{
		UE_LOG(LogDisplayClusterViewport, Warning, TEXT("Tile:ForEachTileViewport - ViewportManager not found. "));
	}
}

void FDisplayClusterViewportConfiguration_Tile::Update()
{
	ImplBeginReallocateViewports();

	ForEachTileViewport([](FDisplayClusterViewport& InSourceViewport, const FIntPoint& InTilePos)
	{
		// Split the source viewports into multiple tiles.
		FDisplayClusterViewportConfigurationHelpers_Tile::GetOrCreateTileViewport(InSourceViewport, InTilePos);
	});

	ImplFinishReallocateViewports();
}

void FDisplayClusterViewportConfiguration_Tile::ImplBeginReallocateViewports() const
{
	if (FDisplayClusterViewportManager* ViewportManager = Configuration.GetViewportManagerImpl())
	{
		for (const TSharedPtr<FDisplayClusterViewport, ESPMode::ThreadSafe>& ViewportIt : ViewportManager->ImplGetCurrentRenderFrameViewports())
		{
			if (ViewportIt.IsValid())
			{
				ViewportIt->GetRenderSettingsImpl().TileSettings.SetTileStateToBeUsed(false);
			}
		}
	}
	else
	{
		UE_LOG(LogDisplayClusterViewport, Warning, TEXT("Tile:BeginReallocate - ViewportManager not found. "));
	}
}

void FDisplayClusterViewportConfiguration_Tile::ImplFinishReallocateViewports() const
{
	if (FDisplayClusterViewportManager* ViewportManager = Configuration.GetViewportManagerImpl())
	{
		TArray<TSharedPtr<FDisplayClusterViewport, ESPMode::ThreadSafe>> UnusedViewports;

		// Collect all unused viewports for remove
		for (const TSharedPtr<FDisplayClusterViewport, ESPMode::ThreadSafe>& ViewportIt : ViewportManager->ImplGetCurrentRenderFrameViewports())
		{
			if (ViewportIt.IsValid() && ViewportIt->GetRenderSettings().TileSettings.GetType() == EDisplayClusterViewportTileType::UnusedTile)
			{
				UnusedViewports.Add(ViewportIt);
			}
		}

		// Delete unused viewports:
		for (TSharedPtr<FDisplayClusterViewport, ESPMode::ThreadSafe>& RemoveViewportIt : UnusedViewports)
		{
			if (RemoveViewportIt.IsValid())
			{
				ViewportManager->DeleteViewport(RemoveViewportIt->GetId());
			}
		}

		UnusedViewports.Empty();
	}
	else
	{
		UE_LOG(LogDisplayClusterViewport, Warning, TEXT("Tile:FinishReallocate - ViewportManager not found. "));
	}
}
