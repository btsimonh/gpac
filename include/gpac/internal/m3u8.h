/**
 *			GPAC - Multimedia Framework C SDK
 *
 *					Authors: Pierre Souchay - Jean Le Feuvre - Romain Bouqueau
 *			Copyright (c) Telecom ParisTech 2010-2012, Romain Bouqueau
 *					All rights reserved
 *
 *  This file is part of GPAC
 *
 *  GPAC is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  GPAC is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; see the file COPYING.  If not, write to
 *  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */
#ifndef M3U8_PLAYLIST_H
#define M3U8_PLAYLIST_H

#include <gpac/tools.h>
#include <gpac/list.h>


#define M3U8_UNKNOWN_MIME_TYPE "unknown"

/**
 * Basic Media structure
 */
typedef struct s_media {
	int i; //unused: C requires that a struct or union has at least one member
} Media;

/**
 * The playlist contains a list of elements to play
 */
struct s_playList {
	int current_media_seq;
	int media_seq_min;
	int media_seq_max;
	double target_duration;
	double computed_duration;
	Bool is_ended;
	GF_List *elements; /*PlaylistElement*/
};
typedef struct s_playList Playlist;

typedef enum e_playlistElementType  { TYPE_PLAYLIST, TYPE_MEDIA, TYPE_UNKNOWN } PlaylistElementType;

/**
 * The Structure containing the playlist element
 */
struct s_playlistElement {
	double duration_info;
	u64 byte_range_start, byte_range_end;
	int bandwidth, width, height;
	char *title;
	char *codecs;
	char *language;
	char *audio_group;
	char *video_group;
	char *url;
	PlaylistElementType element_type;
	union {
		Playlist playlist;
		Media media;
	} element;
};
typedef struct s_playlistElement PlaylistElement;

struct s_stream {
	int stream_id; //may be a real PROGRAM_ID, or a converted GROUP_ID with GROUP_ID_TO_PROGRAM_ID
	GF_List *variants; /*PlaylistElement*/
	double computed_duration;
};
typedef struct s_stream Stream;

/**
 * The root playlist, can contains several PlaylistElements structures
 */
struct s_masterPlaylist {
	GF_List *streams; /*Stream*/
	int current_stream;
	Bool playlist_needs_refresh;
};
typedef struct s_masterPlaylist MasterPlaylist;


/**
 * Parse the given m3u8 playlist file
 * \param file The file from cache to parse
 * \param playlist The playlist to fill. If argument is null, and file is valid, playlist will be allocated
 * \param baseURL The base URL of the playlist
 * \return GF_OK if playlist valid
 */
GF_Err gf_m3u8_parse_master_playlist(const char *file, MasterPlaylist **playlist, const char *baseURL);

/**
 * Parse the given playlist file as a subplaylist of an existing playlist
 * \param file The file from cache to parse
 * \param playlist The playlist to fill.
 * \param baseURL base URL of the playlist
 * \param in_program in which the playlist is parsed
 * \param sub_playlist existing subplaylist element in the playlist in which the playlist is parsed
 * \return GF_OK if playlist valid
 */
GF_Err gf_m3u8_parse_sub_playlist(const char *file, MasterPlaylist **playlist, const char *baseURL, Stream *in_program, PlaylistElement *sub_playlist);

/**
 * Deletes the given MasterPlaylist and all of its sub elements
 */
GF_Err gf_m3u8_master_playlist_del(MasterPlaylist *playlist);

#endif /* M3U8_PLAYLIST_H */
