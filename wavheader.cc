#include <cstdint>
#include <cmath>
#include <vector>

struct WaveHeader {
  explicit WaveHeader(int32_t raw_length) {
    subchunk2_size = raw_length;
    chunk_size = raw_length + 36;
    byte_rate = sample_rate * num_channels * bits_per_sample / 8;
    block_align = num_channels * bits_per_sample / 8;
  }
  char chunk_id[4] = {'R', 'I', 'F', 'F'};
  int32_t chunk_size;
  char format[4] = {'W', 'A', 'V', 'E'};

  char subchunk1_id[4] = {'f', 'm', 't', ' '};
  int32_t subchunk1_size = 16;
  int16_t audio_format = 1;
  int16_t num_channels = 1;
  int32_t sample_rate = 44100;
  int32_t byte_rate;
  int16_t block_align;
  int16_t bits_per_sample = 16;

  char subchunk2_id[4] = {'d', 'a', 't', 'a'};
  int32_t subchunk2_size;
};

int main(int argc, char *argv[]) {
  const double pi = acos(-1);

  std::vector<int16_t> raw(44100);
  for (int i = 0; i < 44100; ++i) {
    double t = i / 44100.0;
    raw[i] = 8192 * sin(440.0 * 2.0 * pi * t);
  }

  WaveHeader header(sizeof(int16_t) * raw.size());
  FILE *fp = fopen("test.wav", "wb");
  fwrite(&header, sizeof(WaveHeader), 1, fp);
  fwrite(&raw[0], sizeof(int16_t), raw.size(), fp);
  fclose(fp);

  return 0;
}

