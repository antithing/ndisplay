/* =========================================================================

  Program:   Multiple Projector Library
  Language:  C++
  Date:      $Date: 2013-07-26 18:15:26 -0400 (Fri, 26 Jul 2013) $
  Version:   $Revision: 22221 $

  Copyright (c) 2013 Scalable Display Technologies, Inc.
  All Rights Reserved
  The source code contained herein is confidential and is considered a 
  trade secret of Scalable Display Technologies, Inc

===================================================================auto== */

#ifndef _EasyBlend1SDKDXApi_H_
#define _EasyBlend1SDKDXApi_H_

#include "Legacy/EasyBlendSDKDXVer.h"
#include "Legacy/EasyBlendSDKDXErrors.h"
#include "Legacy/EasyBlendSDKFrustum.h"
#include "Legacy/EasyBlendSDKDXStructs.h"

#ifdef MESHSDK_EXPORTS
#  define EasyBlend1SDKDX_API extern "C" __declspec(dllexport)
#else
#  define EasyBlend1SDKDX_API extern "C" __declspec(dllimport)
#endif /* ifdef MESHSDK_EXPORTS */


// Description:
// Initializes a Mesh structure from an EasyBlend calibration file (
// .ol file ). The given Mesh pointer should be allocated ( not NULL )
// and will be overwritten.
EasyBlend1SDKDX_API EasyBlend1SDKDXError 
EasyBlendInitialize(const char* szFileName,
                    EasyBlend1SDKDX_Mesh* msm);

// Description:
// Given a correctly initialized SDK Mesh, releases the resources used
// by the Mesh structure.  The Mesh pointer memory must still be
// externally deallocated after this call.
EasyBlend1SDKDX_API EasyBlend1SDKDXError 
EasyBlendUninitialize(EasyBlend1SDKDX_Mesh* msm);

// Description:
// The Standard DirectX Commands to deal with losing/regaining a
// DirectX device.
EasyBlend1SDKDX_API EasyBlend1SDKDXError EasyBlendInitDeviceObjects(EasyBlend1SDKDX_Mesh* msm, ID3D1XDevice* pDevice, ID3D1XDeviceContext* pDeviceContext /* can be null in DX10 */, IDXGISwapChain* pSwapChain);

EasyBlend1SDKDX_API EasyBlend1SDKDXError EasyBlendDXRender(EasyBlend1SDKDX_Mesh* msm, ID3D1XDevice* id3d, ID3D1XDeviceContext* pDeviceContext /* can be null in DX10 */, IDXGISwapChain* iswapChain, bool doPresent = false);

// Description:
// Choose a different type of sampling.
EasyBlend1SDKDX_API EasyBlend1SDKDXError EasyBlendSetSampling(EasyBlend1SDKDX_Mesh* msm, unsigned int sampling);

// Description:
// Returns a string describing an EasyBlend1SDKDXError Code.
EasyBlend1SDKDX_API const char *
EasyBlend1SDKDX_GetErrorMessage(const EasyBlend1SDKDXError &error);

// Description:
// This command returns the view angles for the instance of the SDK.
// As Heading, then Pitch then Roll. The Angles are pre-computed in
// the SDK file, so that it only works with newer version of EasyBlend
// that write this information to disk. (Newer than Nov 2011 or newer)
EasyBlend1SDKDX_API EasyBlend1SDKDXError EasyBlend1SDK_GetHeadingPitchRoll (
    double& rdDegreesHeading, double& rdDegreesPitch, double& rdDegreesRoll,
    EasyBlend1SDKDX_Mesh* msm);

// Description:
// This comand is used only for Dynamic Eyepoint. The new location of 
// the viewer is sent to the command, and a new frustum is calculated. 
// To use this function, the Inititalize command must have been called
// with a .pol file. After this call, remember to get new frustum and
// use it to update the frustum. This command is currently CPU
// bound.
EasyBlend1SDKDX_API EasyBlend1SDKDXError EasyBlendSetEyepoint(EasyBlend1SDKDX_Mesh* msm, const double& eyeX, const double& eyeY, const double& eyeZ);

// Description:
// These function are for setting input/output textures, which allows
// chaining filters
EasyBlend1SDKDX_API EasyBlend1SDKDXError EasyBlendSetInputTexture2D(EasyBlend1SDKDX_Mesh* msm, ID3D1XTexture2D* texture);
EasyBlend1SDKDX_API EasyBlend1SDKDXError EasyBlendSetOutputTexture2D(EasyBlend1SDKDX_Mesh* msm, ID3D1XTexture2D* texture);

#endif // _EasyBlend1SDKDXApi_H_
