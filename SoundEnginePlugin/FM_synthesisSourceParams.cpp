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

#include "FM_synthesisSourceParams.h"

#include <AK/Tools/Common/AkBankReadHelpers.h>

FM_synthesisSourceParams::FM_synthesisSourceParams()
{
}

FM_synthesisSourceParams::~FM_synthesisSourceParams()
{
}

FM_synthesisSourceParams::FM_synthesisSourceParams(const FM_synthesisSourceParams& in_rParams)
{
    RTPC = in_rParams.RTPC;
    NonRTPC = in_rParams.NonRTPC;
    m_paramChangeHandler.SetAllParamChanges();
}

AK::IAkPluginParam* FM_synthesisSourceParams::Clone(AK::IAkPluginMemAlloc* in_pAllocator)
{
    return AK_PLUGIN_NEW(in_pAllocator, FM_synthesisSourceParams(*this));
}

AKRESULT FM_synthesisSourceParams::Init(AK::IAkPluginMemAlloc* in_pAllocator, const void* in_pParamsBlock, AkUInt32 in_ulBlockSize)
{
    if (in_ulBlockSize == 0)
    {
        // Initialize default parameters here
        RTPC.fDuration = 10.0f;

        RTPC.e_Osc_1st = 0;
        RTPC.e_Osc_2nd = 0;
        RTPC.e_Mode = 0;
        RTPC.e_1st_2nd = 0;

        RTPC.fOsc_1st_Amp = 11.0f;
        RTPC.fOsc_2nd_Amp = 12.0f;
        RTPC.fOsc_Frequence = 13.0f;

        m_paramChangeHandler.SetAllParamChanges();
        return AK_Success;
    }

    return SetParamsBlock(in_pParamsBlock, in_ulBlockSize);
 }

AKRESULT FM_synthesisSourceParams::Term(AK::IAkPluginMemAlloc* in_pAllocator)
{
    AK_PLUGIN_DELETE(in_pAllocator, this);
    return AK_Success;
}

AKRESULT FM_synthesisSourceParams::SetParamsBlock(const void* in_pParamsBlock, AkUInt32 in_ulBlockSize)
{
    AKRESULT eResult = AK_Success;
    AkUInt8* pParamsBlock = (AkUInt8*)in_pParamsBlock;

    // Read bank data here
    RTPC.fDuration      = READBANKDATA(AkReal32, pParamsBlock, in_ulBlockSize);

    RTPC.e_Osc_1st      = READBANKDATA(AkReal32, pParamsBlock, in_ulBlockSize);
    RTPC.e_Osc_2nd      = READBANKDATA(AkReal32, pParamsBlock, in_ulBlockSize);
    RTPC.e_Mode         = READBANKDATA(AkReal32, pParamsBlock, in_ulBlockSize);
    RTPC.e_1st_2nd      = READBANKDATA(AkReal32, pParamsBlock, in_ulBlockSize);

    RTPC.fOsc_1st_Amp   = READBANKDATA(AkReal32, pParamsBlock, in_ulBlockSize);
    RTPC.fOsc_2nd_Amp   = READBANKDATA(AkReal32, pParamsBlock, in_ulBlockSize);
    RTPC.fOsc_Frequence = READBANKDATA(AkReal32, pParamsBlock, in_ulBlockSize);
    RTPC.f_RandomSeed   = READBANKDATA(AkReal32, pParamsBlock, in_ulBlockSize);

    CHECKBANKDATASIZE(in_ulBlockSize, eResult);
    m_paramChangeHandler.SetAllParamChanges();
    
    return eResult;
}

AKRESULT FM_synthesisSourceParams::SetParam(AkPluginParamID in_paramID, const void* in_pValue, AkUInt32 in_ulParamSize)
{
    AKRESULT eResult = AK_Success;

    // Handle parameter change here
    switch (in_paramID)
    {
    case PARAM_DURATION_ID:
        RTPC.fDuration = *((AkReal32*)in_pValue);
        m_paramChangeHandler.SetParamChange(PARAM_DURATION_ID);
        break;
    case PARAM_OSC_FREQUENCE_F_ID:
        RTPC.fOsc_Frequence = *((AkReal32*)in_pValue);
        m_paramChangeHandler.SetParamChange(PARAM_OSC_FREQUENCE_F_ID);
        break;
    case PARAM_OSC_1ST_AMP_F_ID:
        RTPC.fOsc_1st_Amp = *((AkReal32*)in_pValue);
        m_paramChangeHandler.SetParamChange(PARAM_OSC_1ST_AMP_F_ID);
        break;
    case PARAM_OSC_2ND_AMP_F_ID:
        RTPC.fOsc_2nd_Amp = *((AkReal32*)in_pValue);
        m_paramChangeHandler.SetParamChange(PARAM_OSC_2ND_AMP_F_ID);
        break;
    case PARAM_OSC_1st_E_ID:
        RTPC.e_Osc_1st = *((AkReal32*)in_pValue);
        m_paramChangeHandler.SetParamChange(PARAM_OSC_1st_E_ID);
        break;
    case PARAM_OSC_2nd_E_ID:
        RTPC.e_Osc_2nd = *((AkReal32*)in_pValue);
        m_paramChangeHandler.SetParamChange(PARAM_OSC_2nd_E_ID);
        break;
    case PARAM_MODE_E_ID:
        RTPC.e_1st_2nd = *((AkReal32*)in_pValue);
        m_paramChangeHandler.SetParamChange(PARAM_MODE_E_ID);
        break;
    case PARAM_1ST_2ND_E_ID:
        RTPC.e_Mode = *((AkReal32*)in_pValue);
        m_paramChangeHandler.SetParamChange(PARAM_1ST_2ND_E_ID);
        break;
    case PARAM_OSC_1ST_FEQ_F_ID:
        RTPC.fOsc_1st_FRQ = *((AkReal32*)in_pValue);
        m_paramChangeHandler.SetParamChange(PARAM_OSC_1ST_FEQ_F_ID);
        break;
    case PARAM_OSC_2_FREQUENCE_F_ID:
        RTPC.fOsc_2_Frequence = *((AkReal32*)in_pValue);
        m_paramChangeHandler.SetParamChange(PARAM_OSC_2_FREQUENCE_F_ID);
        break;
    case PARAM_Random_Seed_ID:
        RTPC.f_RandomSeed = *((AkReal32*)in_pValue);
        m_paramChangeHandler.SetParamChange(PARAM_Random_Seed_ID);
        break;
    default:
        eResult = AK_InvalidParameter;
        break;
    }

    return eResult;
}
