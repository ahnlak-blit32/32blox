/*
 * daft_freak_wav.cpp
 *
 * A very handy gist lifted from DaftFreak for easily playing a WAV.
 */

#include "32blit.hpp"
#include <string.h>

using namespace blit;

struct WavState {
  int channel;
  const uint8_t *data_start, *data_end, *data_cur;
  bool loop;
  uint8_t bits_per_sample;
  uint16_t sample_rate;
};

void stop_wav(WavState *state){

  channels[state->channel].off();
  channels[state->channel].user_data = nullptr;
  channels[state->channel].wave_buffer_callback = nullptr;

  delete state;
}

void wav_callback(AudioChannel &channel) {
  auto state = reinterpret_cast<WavState *>(channel.user_data);

  int i = 0;
  for(; i < 64; i++) {
    if(state->bits_per_sample == 8) {
      channel.wave_buffer[i] = (*state->data_cur << 8) - 0x7F00;
    } else /*16*/{
      channel.wave_buffer[i] = *reinterpret_cast<const int16_t *>(state->data_cur);
    }

    // the other rate is half, so only increment on even samples
    if(state->sample_rate == 22050 || i & 1)
      state->data_cur += (state->bits_per_sample / 8);

    if(state->data_cur == state->data_end) {
      // restart if looping
      if(state->loop)
        state->data_cur = state->data_start;
      else
        break;
    }
  }

  // fill end of buffer if not looping
  for(; i < 64; i++)
    channels[state->channel].wave_buffer[i] = 0;

  if(state->data_cur == state->data_end && !state->loop)
    stop_wav(state);
}

void play_wav(int channel, const uint8_t *ptr, bool loop = false) {
  struct WAVHeader {
    char riff_id[4];
    uint32_t riff_size;
    char riff_format[4];

    char fmt_id[4];
    uint32_t fmt_size;
    uint16_t fmt_format;
    uint16_t fmt_channels;
    uint32_t fmt_sample_rate;
    uint32_t fmt_byte_rate;
    uint16_t fmt_block_align;
    uint16_t fmt_bits_per_sample;

    char data_id[4];
    uint32_t data_size;
  };

  auto head = reinterpret_cast<const WAVHeader *>(ptr);

  // some validation
  if(memcmp(head->riff_id, "RIFF", 4) != 0 || memcmp(head->riff_format, "WAVE", 4) != 0 ||
     memcmp(head->fmt_id, "fmt ", 4) != 0 || memcmp(head->data_id, "data", 4) != 0) {
    return;
  }

  // some restrictions
  // just refusing to play anything that wastes space
  if(head->fmt_format != 1 /*PCM*/ || head->fmt_channels != 1 || (head->fmt_sample_rate != 22050 && head->fmt_sample_rate != 11025))
    return;

  auto state = new WavState;

  state->channel = channel;
  state->data_start = state->data_cur = ptr + sizeof(WAVHeader);
  state->data_end = state->data_start + head->data_size;
  state->loop = loop;
  state->bits_per_sample = head->fmt_bits_per_sample;
  state->sample_rate = head->fmt_sample_rate;

  // assume that any wave callback already on the channel os ours
  if(channels[channel].user_data)
    stop_wav((WavState *)channels[channel].user_data);

  channels[channel].waveforms = Waveform::WAVE;
  channels[channel].user_data = state;
  channels[channel].wave_buffer_callback = &wav_callback;

  channels[channel].trigger_attack();
}

void stop_wav(int channel) {
  if(channels[channel].user_data)
    stop_wav((WavState *)channels[channel].user_data);
}
