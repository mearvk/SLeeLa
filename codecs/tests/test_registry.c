#include "sleela_codecs.h"

#include <assert.h>

int main(void) {
    assert(sleela_codec_count() == SLEELA_CODEC_COUNT);

    const sleela_codec_handler *wav = sleela_codec_by_id(SLEELA_CODEC_PCM_WAV);
    assert(wav != NULL);
    assert(wav->state == SLEELA_CODEC_NATIVE);
    assert(wav->can_decode && wav->can_encode);

    const sleela_codec_handler *opus = sleela_codec_by_extension(".OpUs");
    assert(opus != NULL);
    assert(opus->id == SLEELA_CODEC_OPUS);

    const sleela_codec_handler *flac = sleela_codec_by_extension(".flac");
    assert(flac != NULL);
    assert(flac->state == SLEELA_CODEC_BACKEND);

    assert(sleela_codec_by_extension(".not-a-codec") == NULL);
    assert(sleela_codec_at(SLEELA_CODEC_COUNT) == NULL);
    return 0;
}
