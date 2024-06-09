#include <Arduino.h>
#include "Voice.h"
//drums
#include "Samples/kick1.h"
#include "Samples/kick2.h"
#include "Samples/snareB2.h"
#include "Samples/snare1.h"
#include "Samples/snare2.h"
#include "Samples/snare3.h"
#include "Samples/snareB1.h"
#include "Samples/clap1.h"
#include "Samples/crash1.h"
#include "Samples/ride1.h"
#include "Samples/hihat1.h"
#include "Samples/hihat2.h"
//sfx

#include "Samples/sfx1.h"
#include "Samples/sfx2.h"
#include "Samples/sfx3.h"
#include "Samples/sfx4.h"
#include "Samples/sfx5.h"
#include "Samples/sfx6.h"
#include "Samples/sfx7.h"
#include "Samples/sfx8.h"
#include "Samples/sfx9.h"
#include "Samples/sfx10.h"
#include "Samples/sfx11.h"
#include "Samples/sfx12.h"

//instruments

#include "Samples/bass1.h"
#include "Samples/bass2.h"
#include "Samples/pad1.h"
#include "Samples/pad2.h"
#include "Samples/pad3.h"
#include "Samples/bongo1.h"
#include "Samples/synth2.h"
#include "Samples/guitar1.h"
#include "Samples/pureSin.h"
#include "Samples/synth3.h"

Voice::Voice() {
  octave = 1;
  envelopeLength = 60000;
  volume = 1;
  voiceNum = 0;
  SetupArps();
}

int Voice::UpdateVoice() {

  int sample = 0;
  if (voiceNum > 1) {
    sample = ReadWaveform();
  } else if (voiceNum == 1) {
    sample = ReadSfxWaveform();
  } else {
    sample = ReadDrumWaveform();
  }
  if (volume == 3) {
    if (sample > 4000)
      sample = 4000;
    if (sample < -4000)
      sample = -4000;
    sample *= 2;
  }

  
  if (arpNum == 2) {
    UpdateHistory(sample);
    sample = 0;
    for (int i = 0; i < 4; i++) {
      sample += GetHistorySample(i);
    }
    sample /= 2;
  }
 if (arpNum == 1) {
    UpdateHistory(sample);
    sample = 0;
    for (int i = 0; i < 8; i++) {
      sample += GetHistorySample(i);
    }
    sample /= 4;
  }

  if (arpNum == 3) {
    UpdateHistory(sample);
    int rSample = 0;
    for (int i = 2; i < 6; i++) {
      rSample += GetHistorySample(i * 300)/(i/3+1);
    }
   
    sample = rSample;
  }
  return sample;
}

//look up how to refactor this in c++, use a list or dict not a switch
int Voice::ReadWaveform() {
  int sampleLen = 0;
  int sample = 0;
  int sampleNext = 0;
  int whole;
  float rm = sampleIndex - floor(sampleIndex);

  int sampleIndexNext = sampleIndex + 1;
  if (sampleIndexNext >= sampleLen) {
    sampleIndexNext = sampleIndex + 1 - sampleLen;
  }
  switch (voiceNum) {
    case 2:
      sampleLen = bass1Length;
      sample = bass1[(int)sampleIndex];
      break;
    case 3:
      sampleLen = bass2Length;
      sample = bass2[(int)sampleIndex]*0.5;
      break;
    case 4:
      sampleLen = pad1Length;
      sample = pad1[(int)sampleIndex];
      break;
    case 5:
      sampleLen = pad2Length;
      sample = pad2[(int)sampleIndex];
      break;
    case 6:
      sampleLen = pad3Length;
      sample = pad3[(int)sampleIndex]*.8;
      break;
    case 7:
      sampleLen = bongo1Length;
      sample = bongo1[(int)sampleIndex];
      break;
    case 8:
      sampleLen = synth2Length;
      sample = synth2[(int)sampleIndex] * .9;
      break;
    case 9:
      sampleLen = pureSinLength;
      sample = pureSin[(int)sampleIndex] * .3;
      break;
    case 10:
      sampleLen = synth3Length;
      sample = synth3[(int)sampleIndex] * .6;
      break;
    case 11:
      sampleLen = guitar1Length;
      sample = guitar1[(int)sampleIndex];
      break;
  }

  float volumeRatio = GetVolumeRatio();
  sampleIndex += baseFreq;

  if (sampleIndex >= sampleLen) {
    sampleIndex = sampleIndex - sampleLen;
  }

  if (envelope > 0) {
    envelope -= 10;
  }

  sample = (int)(sample * volume * volumeRatio);
  if (isDelay) {
    sample *= .3;
  }
  return sample;
}

int Voice::ReadDrumWaveform() {

  int sample = 0;
  int subSampleIndex = (int)sampleIndex;
  if (envelopeNum > 1) {
    subSampleIndex = sampleLen - (int)sampleIndex - 1;
  }
  switch (note) {
    case 0:
      sampleLen = kick1Length;
      sample = kick1[(int)subSampleIndex];
      break;
    case 1:
      sampleLen = snare1Length;
      sample = snare1[(int)subSampleIndex];
      break;
    case 2:
      sampleLen = snareB1Length;
      sample = snareB1[(int)subSampleIndex];
      break;
    case 3:
      sampleLen = hihat1Length;
      sample = hihat1[(int)subSampleIndex];
      break;
    case 4:
      sampleLen = kick2Length;
      sample = kick2[(int)subSampleIndex];
      break;
    case 5:
      sampleLen = snare2Length;
      sample = snare2[(int)subSampleIndex];
      break;
    case 6:
      sampleLen = snare3Length;
      sample = snare3[(int)subSampleIndex];
      break;
    case 7:
      sampleLen = hihat2Length;
      sample = hihat2[(int)subSampleIndex];
      break;
    case 8:
      sampleLen = snareB2Length;
      sample = snareB2[(int)subSampleIndex];
      break;
    case 9:
      sampleLen = clap1Length;
      sample = clap1[(int)subSampleIndex];
      break;
    case 10:
      sampleLen = crash1Length;
      sample = crash1[(int)subSampleIndex];
      break;
    case 11:
      sampleLen = ride1Length;
      sample = ride1[(int)subSampleIndex];
      break;
  }

  if (sampleIndex >= sampleLen) {
    sample = 0;
  } else {
    int octAdd = octave;
    if (octAdd < 1)
      octAdd = 1;
    sampleIndex += octAdd;
  }
  sample = (int)(sample * volume * 2);
  if (isDelay) {
    sample *= .3;
  }
  return sample;
}

int Voice::ReadSfxWaveform() {
  int sampleLen = 0;
  int sample = 0;
  int subSampleIndex = (int)sampleIndex;
  if (envelopeNum > 1) {
    subSampleIndex = sampleLen - (int)sampleIndex - 1;
  }
  switch (note) {
    case 0:
      sampleLen = sfx1Length;
      sample = sfx1[(int)subSampleIndex];
      break;
    case 1:
      sampleLen = sfx2Length;
      sample = sfx2[(int)subSampleIndex];
      break;
    case 2:
      sampleLen = sfx3Length;
      sample = sfx3[(int)subSampleIndex];
      break;
    case 3:
      sampleLen = sfx4Length;
      sample = sfx4[(int)subSampleIndex];
      break;
    case 4:
      sampleLen = sfx5Length;
      sample = sfx5[(int)subSampleIndex];
      break;
    case 5:
      sampleLen = sfx6Length;
      sample = sfx6[(int)subSampleIndex];
      break;
    case 6:
      sampleLen = sfx7Length;
      sample = sfx7[(int)subSampleIndex];
      break;
    case 7:
      sampleLen = sfx8Length;
      sample = sfx8[(int)subSampleIndex];
      break;
    case 8:
      sampleLen = sfx9Length;
      sample = sfx9[(int)subSampleIndex];
      break;
    case 9:
      sampleLen = sfx10Length;
      sample = sfx10[(int)subSampleIndex];
      break;
    case 10:
      sampleLen = sfx11Length;
      sample = sfx11[(int)sampleIndex];
      break;
    case 11:
      sampleLen = sfx12Length;
      sample = sfx12[(int)sampleIndex];
      break;
  }
  if (sampleIndex >= sampleLen) {
    sample = 0;
  } else {
    int octAdd = octave;
    if (octAdd < 1)
      octAdd = 1;
    sampleIndex += octAdd;
  }
  sample = (int)(sample * volume * 1.5);
  if (isDelay) {
    sample *= .3;
  }
  return sample;
}

float Voice::LerpSample(int sampleA, int sampleB, float ratio) {
  return (float)sampleA + ((float)(sampleB - sampleA) * ratio);
}

float Voice::GetBaseFreq(int val, int ioctave) {
  float freqAdd = 2;
  //if (arpNum > 0) {
  // val += arps[arpNum][arpCount];
  //}
  float valOut = 1 + ((float)(val + (((float)ioctave) * 12)) * freqAdd * freqAdd * .023);  // + (octave * freqAdd * 12);

  return valOut;
}

float Voice::GetVolumeRatio() {
  if (voiceNum == 0) {
    return 1;
  }
  float subVal = 0;
  float subVal2 = 0;
  switch (envelopeNum) {
    case 0:
      subVal = ((float)envelope / (float)envelopeLength);
      subVal *= 2;
      if (subVal > 1) {
        subVal = 1;
      }
      return subVal;
      break;
    case 1:

      subVal = (1 - ((float)envelope / (float)envelopeLength) * 2 + 1);
      subVal2 = (1 - ((float)envelope / (float)envelopeLength));

      if (subVal2 >= 1) {
        subVal = 0;
      }

      return subVal;
      break;
    case 2:
      subVal = 1;
      subVal2 = (1 - ((float)envelope / (float)envelopeLength));
      if (subVal2 >= 1) {
        subVal = 0;
      }
      return subVal;
      break;
    case 3:
      float env = (((float)envelope / (float)envelopeLength) - 0.5) * 2;
      if (envelope <= 0) {
        envelope = envelopeLength;
      }
      return (.5 + abs(env) * .5);
      break;
  }
}

void Voice::SetNote(int val, bool delay, int optOctave, int optInstrument) {
  if (voiceNum < 2)
    sampleIndex = 0;

  note = val;
  envelope = envelopeLength;
  voiceNum = optInstrument;
  if (optOctave == -1)
    baseFreq = GetBaseFreq(val, octave);
  else
    baseFreq = GetBaseFreq(val, optOctave);

  arpCount++;
  if (arpCount > 3) {
    arpCount = 0;
  }
  isDelay = delay;
}

void Voice::SetDelay(int val) {
  delay = val;
}

void Voice::SetVolume(int val) {
  volume = val;
}

void Voice::SetOctave(int val) {
  octave = val;
}

void Voice::SetEnvelopeNum(int val) {
  envelopeNum = val;
}

void Voice::SetEnvelopeLength(int val) {
  envelopeLength = val;
}

void Voice::SetArpNum(int val) {
  arpNum = val;
}

void Voice::SetupArps() {
  arps[1][0] = -5;
  arps[1][1] = -3;
  arps[1][2] = 0;
  arps[1][3] = 3;

  arps[2][0] = 0;
  arps[2][1] = 7;
  arps[2][2] = 12;
  arps[2][3] = 5;

  arps[3][0] = 0;
  arps[3][1] = 5;
  arps[3][2] = 7;
  arps[3][3] = 12;
}

void Voice::UpdateHistory(int sample) {
  sampleHistory[sampleHistoryIndex] = sample;
  sampleHistoryIndex++;
  if (sampleHistoryIndex > 1999) {
    sampleHistoryIndex = 0;
  }
}

int Voice::GetHistorySample(int backoffset) {
  int ind = sampleHistoryIndex - backoffset;
  if (ind < 0) {
    ind = 1999 + ind;
  }
  return sampleHistory[ind];
}
