/*******************************************************************************
The content of this file includes portions of the AUDIOKINETIC Wwise Technology
released in source code form as part of the SDK installer package.

Commercial License Usage

Licensees holding valid commercial licenses to the AUDIOKINETIC Wwise Technology
may use this file in accordance with the end user license agreement provided
with the software or, alternatively, in accordance with the terms contained in a
written agreement between you and Audiokinetic Inc.

Apache License Usage

Alternatively, this file may be used under the Apache License, Version 2.0 (the
"Apache License"); you may not use this file except in compliance with the
Apache License. You may obtain a copy of the Apache License at
http://www.apache.org/licenses/LICENSE-2.0.

Unless required by applicable law or agreed to in writing, software distributed
under the Apache License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES
OR CONDITIONS OF ANY KIND, either express or implied. See the Apache License for
the specific language governing permissions and limitations under the License.

  Copyright (c) 2023 Audiokinetic Inc.
*******************************************************************************/

#ifndef FM_synthesisSourceParams_H
#define FM_synthesisSourceParams_H

#include <AK/SoundEngine/Common/IAkPlugin.h>
#include <AK/Plugin/PluginServices/AkFXParameterChangeHandler.h>

// Add parameters IDs here, those IDs should map to the AudioEnginePropertyID
// attributes in the xml property definition.
static const AkPluginParamID PARAM_DURATION_ID = 0;

static const AkPluginParamID PARAM_OSC_1st_E_ID = 10;
static const AkPluginParamID PARAM_OSC_2nd_E_ID = 11;
static const AkPluginParamID PARAM_MODE_E_ID = 12;
static const AkPluginParamID PARAM_1ST_2ND_E_ID = 13;

static const AkPluginParamID PARAM_OSC_FREQUENCE_F_ID = 1;
static const AkPluginParamID PARAM_OSC_1ST_AMP_F_ID = 2;
static const AkPluginParamID PARAM_OSC_2ND_AMP_F_ID = 3;
static const AkPluginParamID PARAM_OSC_1ST_FEQ_F_ID = 4;
static const AkPluginParamID PARAM_OSC_2_FREQUENCE_F_ID = 6;
static const AkPluginParamID PARAM_Random_Seed_ID = 7;



static const AkUInt32 NUM_PARAMS = 12;

struct FM_synthesisRTPCParams
{
    AkReal32 fDuration;
    AkReal32 e_Osc_1st;
    AkReal32 e_Osc_2nd;
    AkReal32 e_Mode;
    AkReal32 e_1st_2nd;

    AkReal32 f_RandomSeed;

    AkReal32 fOsc_Frequence;
    AkReal32 fOsc_1st_Amp;
    AkReal32 fOsc_2nd_Amp;
    AkReal32 fOsc_1st_FRQ;
    //AkReal32 fOsc_2nd_FRQ;
    AkReal32 fOsc_2_Frequence;
};

struct FM_synthesisNonRTPCParams
{
};

struct FM_synthesisSourceParams
    : public AK::IAkPluginParam
{
    FM_synthesisSourceParams();
    FM_synthesisSourceParams(const FM_synthesisSourceParams& in_rParams);

    ~FM_synthesisSourceParams();

    /// Create a duplicate of the parameter node instance in its current state.
    IAkPluginParam* Clone(AK::IAkPluginMemAlloc* in_pAllocator) override;

    /// Initialize the plug-in parameter node interface.
    /// Initializes the internal parameter structure to default values or with the provided parameter block if it is valid.
    AKRESULT Init(AK::IAkPluginMemAlloc* in_pAllocator, const void* in_pParamsBlock, AkUInt32 in_ulBlockSize) override;

    /// Called by the sound engine when a parameter node is terminated.
    AKRESULT Term(AK::IAkPluginMemAlloc* in_pAllocator) override;

    /// Set all plug-in parameters at once using a parameter block.
    AKRESULT SetParamsBlock(const void* in_pParamsBlock, AkUInt32 in_ulBlockSize) override;

    /// Update a single parameter at a time and perform the necessary actions on the parameter changes.
    AKRESULT SetParam(AkPluginParamID in_paramID, const void* in_pValue, AkUInt32 in_ulParamSize) override;

    AK::AkFXParameterChangeHandler<NUM_PARAMS> m_paramChangeHandler;

    FM_synthesisRTPCParams RTPC;
    FM_synthesisNonRTPCParams NonRTPC;

};



#endif // FM_synthesisSourceParams_H
