#include "crossfade.h"
#include "utils.h"

#include <cstring>

void EFXCrossfade::configure(const char *key, int value) {
    if ( strcmp( key, "fade_time_ms") == 0 )
        fade_time_ms = value;
}

bool EFXCrossfade::render(GLuint old_texture, GLuint new_texture) {
    // Very first frame so we reset/initialize all variables properly
    if ( initial ) {
        initial = false;
        fade_current = 0.0f;
        fade_offset_ms = PI_UTILS::get_current_time_ms() % fade_time_ms;
    }

    if ( fade_offset_ms && fade_current < 1.0f ) {
        fade_current = (float) ( ( PI_UTILS::get_current_time_ms() - fade_offset_ms ) % fade_time_ms ) / fade_time_ms;

        if (fade_current < fade_last) {
            fade_last       = 0.0f;
            fade_current    = 1.0f;
            fade_offset_ms  = 0;
        } else
            fade_last = fade_current;

        // Fade out old image
        PI_UTILS::draw_image( old_texture, NULL, NULL, NULL, NULL, 1.0f - fade_current );

        // Fade in new image
        PI_UTILS::draw_image( new_texture,  NULL, NULL, NULL, NULL, fade_current );

        return false;
    }

    initial = true;

    return true;
}