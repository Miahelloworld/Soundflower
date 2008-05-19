/*
  File:SoundflowerClip.cpp

  Version:1.0.1
    ma++ ingalls  |  cycling '74  |  Copyright (C) 2004  |  soundflower.com
    
    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation; either version 2
    of the License, or (at your option) any later version.
    
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    
    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#include "SoundflowerEngine.h"
#include <IOKit/IOLib.h>



// here, instead of clipping, we save the buffer 
//##TODO add channel mute and gain support

IOReturn SoundflowerEngine::clipOutputSamples(const void *mixBuf, void *sampleBuf, UInt32 firstSampleFrame, UInt32 numSampleFrames, const IOAudioStreamFormat *streamFormat, IOAudioStream *audioStream)
{
    UInt32 offset;
    UInt32 numBytes = numSampleFrames * streamFormat->fNumChannels * sizeof(float);
    
    offset = firstSampleFrame * streamFormat->fNumChannels * sizeof(float);
    if (((SoundflowerDevice *)audioDevice)->mMuteIn[0])
        memset((UInt8 *)thruBuffer + offset, 0, numBytes);
    else
        memcpy((UInt8 *)thruBuffer + offset, (UInt8 *)mixBuf + offset, numBytes);
    
    return kIOReturnSuccess;
}


// This is called when client apps need input audio.  Here we give them saved audio from the clip routine.
//##TODO add channel mute and gain support

IOReturn SoundflowerEngine::convertInputSamples(const void *sampleBuf, void *destBuf, UInt32 firstSampleFrame, UInt32 numSampleFrames, const IOAudioStreamFormat *streamFormat, IOAudioStream *audioStream)
{
    UInt32 offset;
    UInt32 frameSize = streamFormat->fNumChannels * sizeof(float);
    offset = firstSampleFrame * frameSize;
              
    if (((SoundflowerDevice *)audioDevice)->mMuteOut[0])
        memset((UInt8 *)destBuf, 0, numSampleFrames * frameSize);
    else
   	memcpy((UInt8 *)destBuf, (UInt8 *)thruBuffer + offset, numSampleFrames * frameSize);

    return kIOReturnSuccess;
}
