/*
 *  Copyright (C) 2015 Savoir-Faire Linux Inc.
 *  Author: Eloi BAIL <eloi.bail@savoirfairelinux.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *  MA  02110-1301 USA.
 *
 *  Additional permission under GNU GPL version 3 section 7:
 *
 *  If you modify this program, or any covered work, by linking or
 *  combining it with the OpenSSL project's OpenSSL library (or a
 *  modified version of that library), containing parts covered by the
 *  terms of the OpenSSL or SSLeay licenses, Savoir-Faire Linux Inc.
 *  grants you additional permission to convey the resulting work.
 *  Corresponding Source for a non-source form of such a combination
 *  shall include the source code for the parts of OpenSSL used as well
 *  as that of the covered work.
 */

#ifndef __MEDIA_CODEC_H__
#define __MEDIA_CODEC_H__

#include "audio/audiobuffer.h" // for AudioFormat
#include "sip/sdes_negotiator.h"
#include "ip_utils.h"

#include <string>
#include <vector>
#include <iostream>
#include <unistd.h>

namespace ring {

enum CodecType : unsigned {
    CODEC_UNDEFINED = 0,
    CODEC_ENCODER = 1,
    CODEC_DECODER = 2,
    CODEC_ENCODER_DECODER = CODEC_ENCODER | CODEC_DECODER
};

enum MediaType : unsigned {
    MEDIA_UNDEFINED = 0,
    MEDIA_AUDIO = 1,
    MEDIA_VIDEO = 2,
    MEDIA_ALL = MEDIA_AUDIO | MEDIA_VIDEO
};

/*
 * SystemCodecInfo
 * represent information of a codec available on the system (using libav)
 * store default codec values
 */
struct SystemCodecInfo
{
    SystemCodecInfo(unsigned avcodecId, const std::string name,
                    std::string libName, MediaType mediaType,
                    CodecType codecType = CODEC_UNDEFINED, unsigned bitrate = 0,
                    unsigned payloadType = 0);

    virtual ~SystemCodecInfo();

    /* generic codec information */
    unsigned id; /* id of the codec used with dbus */
    unsigned  avcodecId;  /* read as AVCodecID libav codec identifier */
    std::string name;
    std::string libName;
    CodecType codecType;
    MediaType mediaType;

    /* default codec values */
    unsigned payloadType;
    unsigned bitrate;

    std::string to_string() const;
};

/*
 * SystemAudioCodecInfo
 * represent information of a audio codec available on the system (using libav)
 * store default codec values
 */
struct SystemAudioCodecInfo : SystemCodecInfo
{
    SystemAudioCodecInfo(unsigned avcodecId, const std::string name,
                         std::string libName, CodecType type,
                         unsigned bitrate = 0,
                         unsigned sampleRate = 0, unsigned nbChannels = 0,
                         unsigned payloadType = 0);

    ~SystemAudioCodecInfo();

    std::vector<std::string> getCodecSpecifications();
    bool isPCMG722() const;

    unsigned sampleRate;
    unsigned nbChannels;
};

/*
 * SystemVideoCodecInfo
 * represent information of a video codec available on the system (using libav)
 * store default codec values
 */
struct SystemVideoCodecInfo : SystemCodecInfo
{
    SystemVideoCodecInfo(unsigned avcodecId, const std::string name,
                         std::string libName, CodecType type = CODEC_UNDEFINED,
                         unsigned payloadType = 0);

    ~SystemVideoCodecInfo();

    std::vector<std::string> getCodecSpecifications();

    unsigned frameRate;
    unsigned profileId;
    std::string parameters;
};

/*
 * AccountCodecInfo
 * represent information of a codec on a account
 * store account codec values
 */
struct AccountCodecInfo
{
    AccountCodecInfo(const SystemCodecInfo& sysCodecInfo);
    ~AccountCodecInfo();

    const SystemCodecInfo& systemCodecInfo;
    unsigned order; /*used to define prefered codec list order in UI*/
    bool isActive;
    /* account custom values */
    unsigned payloadType;
    unsigned bitrate;
};

struct AccountAudioCodecInfo : AccountCodecInfo
{
    AccountAudioCodecInfo(const SystemAudioCodecInfo& sysCodecInfo);
    ~AccountAudioCodecInfo();

    std::vector<std::string> getCodecSpecifications();

    /* account custom values */
    unsigned sampleRate;
    unsigned nbChannels;
};

struct AccountVideoCodecInfo : AccountCodecInfo
{
    AccountVideoCodecInfo(const SystemVideoCodecInfo& sysCodecInfo);
    ~AccountVideoCodecInfo();

    std::vector<std::string> getCodecSpecifications();

    /* account custom values */
    unsigned frameRate;
    unsigned profileId;
    std::string parameters;
};

bool operator== (SystemCodecInfo codec1, SystemCodecInfo codec2);

} // namespace ring

#endif // __MEDIA_CODEC_H__