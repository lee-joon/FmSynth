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

#include "FM_synthesisSource.h"
#include "../FM_synthesisConfig.h"

#include <AK/AkWwiseSDKVersion.h>

AK::IAkPlugin* CreateFM_synthesisSource(AK::IAkPluginMemAlloc* in_pAllocator)
{
    return AK_PLUGIN_NEW(in_pAllocator, FM_synthesisSource());
}

AK::IAkPluginParam* CreateFM_synthesisSourceParams(AK::IAkPluginMemAlloc* in_pAllocator)
{
    return AK_PLUGIN_NEW(in_pAllocator, FM_synthesisSourceParams());
}

AK_IMPLEMENT_PLUGIN_FACTORY(FM_synthesisSource, AkPluginTypeSource, FM_synthesisConfig::CompanyID, FM_synthesisConfig::PluginID)

FM_synthesisSource::FM_synthesisSource()
    : m_pParams(nullptr)
    , m_pAllocator(nullptr)
    , m_pContext(nullptr)
    , Util(UtilLookupTable())
{
}

FM_synthesisSource::~FM_synthesisSource()
{
}

AKRESULT FM_synthesisSource::Init(AK::IAkPluginMemAlloc* in_pAllocator, AK::IAkSourcePluginContext* in_pContext, AK::IAkPluginParam* in_pParams, AkAudioFormat& in_rFormat)
{
    m_pParams = (FM_synthesisSourceParams*)in_pParams;
    m_pAllocator = in_pAllocator;
    m_pContext = in_pContext;

    m_durationHandler.Setup(m_pParams->RTPC.fDuration, in_pContext->GetNumLoops(), in_rFormat.uSampleRate);
    m_SampleRate = in_rFormat.uSampleRate;

    if ((bool)m_pParams->RTPC.e_Mode)
    {
        RandomValue.SetSeed(m_pParams->RTPC.f_RandomSeed);

        m_R_fOsc_Frequence = RandomValue.floatValue(200.0f);
        m_R_fOsc_1st_Amp = RandomValue.floatValue(20.0f);
        m_R_fOsc_2nd_Amp = RandomValue.floatValue(20.0f);
        m_R_fOsc_1st_FRQ = RandomValue.floatValue(200.0f);
        m_R_fOsc_2_Frequence = RandomValue.floatValue(200.0f);


    }


    return AK_Success;
}

AKRESULT FM_synthesisSource::Term(AK::IAkPluginMemAlloc* in_pAllocator)
{
    AK_PLUGIN_DELETE(in_pAllocator, this);
    return AK_Success;
}

AKRESULT FM_synthesisSource::Reset()
{
    return AK_Success;
}

AKRESULT FM_synthesisSource::GetPluginInfo(AkPluginInfo& out_rPluginInfo)
{
    out_rPluginInfo.eType = AkPluginTypeSource;
    out_rPluginInfo.bIsInPlace = true;
    out_rPluginInfo.uBuildVersion = AK_WWISESDK_VERSION_COMBINED;
    return AK_Success;
}

void FM_synthesisSource::Execute(AkAudioBuffer* out_pBuffer)
{
    /// Set current duration per iteration (in secs)./// 반복당 현재 기간을 설정합니다(초 단위).
    m_durationHandler.SetDuration(m_pParams->RTPC.fDuration);
    /// 현재 생성할 프레임 수(validFrames)와 오디오 버퍼의 출력 상태를 설정하고 내부 상태를 진행합니다.
    m_durationHandler.ProduceBuffer(out_pBuffer);

    const AkUInt32 uNumChannels = out_pBuffer->NumChannels();

    if(!((bool)m_pParams->RTPC.e_Mode))
    {
        AkReal32 Frequence = m_pParams->RTPC.fOsc_Frequence / m_SampleRate;
        AkReal32 Frequence2 = m_pParams->RTPC.fOsc_2_Frequence / m_SampleRate;/**/
        AkReal32 carrier_1_Fre = m_pParams->RTPC.fOsc_1st_FRQ / m_SampleRate;
        //AkReal32 carrier_2_Fre = m_pParams->RTPC.fOsc_2nd_FRQ / m_SampleRate;

        AkUInt16 uFramesProduced;
        for (AkUInt32 i = 0; i < uNumChannels; ++i)
        {
            AkReal32* AK_RESTRICT pBuf = (AkReal32 * AK_RESTRICT)out_pBuffer->GetChannel(i);

            uFramesProduced = 0;
            AkUInt32 Timer = m_Timer;
            while (uFramesProduced < out_pBuffer->uValidFrames)
            {
                // Generate output here
                AkReal32 Fm_Freq = fmodf(Frequence * Timer, 1.0f) + m_pParams->RTPC.fOsc_1st_Amp * (Util.Oscillator((AkUInt32)m_pParams->RTPC.e_Osc_1st, fmodf(carrier_1_Fre * Timer, 1.0f)) + 1.0f);
                if (!((bool)m_pParams->RTPC.e_1st_2nd))
                {
                    *pBuf++ = Util.sin(fmodf(Fm_Freq, 1.0f));
                }
                else
                {
                    AkReal32 Fm_Freq2 = fmodf(Frequence2 * Timer, 1.0f) + (Util.Oscillator((AkUInt32)m_pParams->RTPC.e_Osc_2nd, fmodf(Fm_Freq, 1.0f)) + 1.0f) * m_pParams->RTPC.fOsc_2nd_Amp;
                    *pBuf++ = Util.sin(fmodf(Fm_Freq2, 1.0f));
                }

                ++uFramesProduced;
                ++Timer;
            }
        }
    }
    else
    {
        AkReal32 Frequence          = (m_pParams->RTPC.fOsc_Frequence   + m_R_fOsc_Frequence)   / m_SampleRate;
        AkReal32 Frequence2         = (m_pParams->RTPC.fOsc_2_Frequence + m_R_fOsc_2_Frequence) / m_SampleRate;/**/
        AkReal32 carrier_1_Fre      = (m_pParams->RTPC.fOsc_1st_FRQ     + m_R_fOsc_1st_FRQ)     / m_SampleRate;
        //AkReal32 carrier_2_Fre = m_pParams->RTPC.fOsc_2nd_FRQ / m_SampleRate;

        AkUInt16 uFramesProduced;
        for (AkUInt32 i = 0; i < uNumChannels; ++i)
        {
            AkReal32* AK_RESTRICT pBuf = (AkReal32 * AK_RESTRICT)out_pBuffer->GetChannel(i);

            uFramesProduced = 0;
            AkUInt32 Timer = m_Timer;
            while (uFramesProduced < out_pBuffer->uValidFrames)
            {
                // Generate output here
                AkReal32 Fm_Freq = fmodf(Frequence * Timer, 1.0f) + (m_pParams->RTPC.fOsc_1st_Amp + m_R_fOsc_1st_Amp) * (Util.Oscillator((AkUInt32)m_pParams->RTPC.e_Osc_1st, fmodf(carrier_1_Fre * Timer, 1.0f)) + 1.0f);
                if (!((bool)m_pParams->RTPC.e_1st_2nd))
                {
                    *pBuf++ = Util.sin(fmodf(Fm_Freq, 1.0f));
                }
                else
                {
                    AkReal32 Fm_Freq2 = fmodf(Frequence2 * Timer, 1.0f) + (Util.Oscillator((AkUInt32)m_pParams->RTPC.e_Osc_2nd, fmodf(Fm_Freq, 1.0f)) + 1.0f) * (m_pParams->RTPC.fOsc_2nd_Amp + m_R_fOsc_2nd_Amp);
                    *pBuf++ = Util.sin(fmodf(Fm_Freq2, 1.0f));
                }

                ++uFramesProduced;
                ++Timer;
            }
        }
    }

    m_Timer = m_Timer + out_pBuffer->uValidFrames;
}

AkReal32 FM_synthesisSource::GetDuration() const
{
    return m_durationHandler.GetDuration() * 1000.0f;
}
