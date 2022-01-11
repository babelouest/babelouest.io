---
layout: default
title: Taliesin
permalink: /taliesin/
---

# Taliesin

[![GitHub release (latest by date)](https://img.shields.io/github/v/release/babelouest/taliesin?style=plastic)](https://github.com/babelouest/taliesin/releases/latest)
[![GitHub last commit](https://img.shields.io/github/last-commit/babelouest/taliesin)](https://github.com/babelouest/taliesin/)
![GitHub](https://img.shields.io/github/license/babelouest/taliesin?style=plastic)

Lightweight audio media server with a REST API interface and a React JS client application.

Taliesin is an audio streaming server used to stream local music files, so you can listen to your music anywhere. You can stream as a webradio: one stream for all the songs, one after the other, or as a jukebox: one stream for each song, as you choose which one to play.

The backend sever is programmed in C, using [libav](https://libav.org/) to transcode audio files in real-time.

It uses few resources, so you can run it on a Raspbeery Pi for a single or family use.

You can test the [Online Demo](https://taliesin.babelouest.io/). To connect, you must use a valid account registered via the [Register new account](https://glewlwyd.babelouest.io/index.html?register=regis).

These are some examples of what Taliesin looks like when used.

## Dashboard

This is the Dashboard screen, where the user can manage its streams, playlists, or display recent media and random media folders.

![Dashboard](/img/dashboard.png)

![Dashboard small screen](/img/dashboard-smartphone.png)

## Data source management

To make media files available, the user must add one or more data sources. A data source is a point of entry in the server file system where media files are stored.

![Manage Data Source](/img/browse_data_source.png)

## Browse media

When browsing media files, the user can browse via file path, or categories. For each browsing type, the user can display the result as a list or as a set of icons.

![Browse categories](/img/browse_category.png)

![Browse path icon](/img/browse_icon.png)

![Browse path list](/img/browse_list.png)

## Media details

You can also display the details of a media file

![Media details](/img/media_details.png)

## Player

The player controls is available at the bottom of the screen. Clicking on the "full screen" button displays a full-screen player where you can see the current song.

![Full screen player](/img/player_fullscreen.png)

![Stream media list](/img/player_with_list.png)
