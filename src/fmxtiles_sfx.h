//---------------------------------------------------------------------------

#ifndef fmxtiles_sfxH
#define fmxtiles_sfxH
//---------------------------------------------------------------------------

#include <FMX.Media.hpp>

#include <memory>
#include <map>

#ifndef __clang__
#include <boost/shared_ptr.hpp>
#endif

#include "fmxtiles_tilemap.h"
//---------------------------------------------------------------------------

namespace Fmxtiles
{

#ifdef __clang__
using std::shared_ptr;
#else
using boost::shared_ptr;
#endif
//---------------------------------------------------------------------------

class TLoopSync;
//---------------------------------------------------------------------------

class TSfx
{
public:
    TSfx(const TLoopSync& LoopSync);
    virtual ~TSfx();

    void MusicPlay(const String& FileName, bool StartAtRandomPos=false, float FadeInDuration=0.0);
    void MusicStop(float FadeOutDuration=0.0);

    void Process(float Delta, float Absolute);

private:
    std::unique_ptr<TMediaPlayer> FMusicPlayer;

    TLoopSync& FLoopSync;

    float   FMusicVolume;
    float   FMusicFadeStartTime;
    bool    FMusicFadeInIsActive;
    bool    FMusicFadeOutIsActive;
};
//---------------------------------------------------------------------------

} // namespace Fmxtiles

#endif
