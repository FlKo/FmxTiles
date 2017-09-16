//---------------------------------------------------------------------------
#include <fmx.h>
#pragma hdrstop

#include "fmxtiles_sfx.h"

#include <memory>
#include <map>

#ifndef __clang__
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#endif
//---------------------------------------------------------------------------

#pragma package(smart_init)

namespace Fmxtiles
{

#ifdef __clang__
using std::make_shared;
#else
using boost::make_shared;
#endif

TSfx::TSfx(const TLoopSync& LoopSync)
    :   FMusicPlayer(new TMediaPlayer(NULL)),
        FLoopSync(LoopSync),
        FMusicVolume(1.0),
        FMusicFadeStartTime(0.0)
        FMusicFadeInIsActive(false),
        FMusicFadeOutIsActive(false)
{

}
//---------------------------------------------------------------------------

TSfx::~TSfx()
{

}
//---------------------------------------------------------------------------

void TSfx::MusicPlay(const String& FileName, bool StartAtRandomPos, float FadeInDuration)
{
    FMusicFadeInIsActive     = false;
    FMusicFadeOutIsActive    = false;

    // Load the music file
    FMusicPlayer->FileName = FileName;

    // If it should start at a random position...
    if (StartAtRandomPos)
    {
        // ...get a such a position within...
        __int64 RandomStartPos = RandomRange(0, static_cast<int>(FMusicPlayer->Duration));

        // ...and set the player to this position...
        FMusicPlayer->CurrentTime = RandomStartPos;
    }

    if (FadeInDuration > 0.0)
    {
        FMusicPlayer->Volume    = 0.0;
        FMusicFadeStartTime     = FLoopSync.
        FMusicFadeInIsActive    = true;
    }
    else
    {
        FMusicPlayer->Volume = 1.0;
    }

    // Play the file
    FMusicPlayer->Play();
}
//---------------------------------------------------------------------------

void TSfx::MusicStop(float FadeOutDuration)
{
    if (FadeOutDuration == 0.0)
    {
        FMusicPlayer->Stop();

        return;
    }

    FMusicPlayer->Volume    = 1.0;
    FMusicFadePosition      = FadeOutDuration;
    FMusicFadeOutIsActive   = true;
}
//---------------------------------------------------------------------------

void TSfx::Process(float Delta, float Absolute)
{
    if (FMusicFadeInIsActive)
    {
        float CurrentVolume =
            (float)(GetTickCount() - FMusicFadeTicksStart) / (float)FSfxOptions.MusicFadeDuration;

        FSfxOptions.MusicPlayer->Volume = CurrentVolume < 1.0 ? CurrentVolume : 1.0;

        if ((GetTickCount() - FSfxOptions.MusicFadeTicksStart) > FSfxOptions.MusicFadeDuration)
            FSfxOptions.MusicFadeInIsActive = false;
    }

    if (FSfxOptions.MusicFadeOutIsActive)
    {
        float CurrentVolume =
            1.0 - ((float)(GetTickCount() - FSfxOptions.MusicFadeTicksStart) / (float)FSfxOptions.MusicFadeDuration);

        FSfxOptions.MusicPlayer->Volume = CurrentVolume > 0.0 ? CurrentVolume : 0.0;

        if ((GetTickCount() - FSfxOptions.MusicFadeTicksStart) > FSfxOptions.MusicFadeDuration)
            FSfxOptions.MusicFadeOutIsActive = false;
    }
}
//---------------------------------------------------------------------------

} // namespace Fmxtiles

