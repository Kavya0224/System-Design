# Music Player - Low Level Design (LLD)

## Overview

This project is a simplified implementation of a Music Player using C++ and Object-Oriented Design principles.

The system allows users to:

- Add songs
- Create playlists
- Add songs to playlists
- Play songs
- Pause songs
- Play next song
- Play previous song
- Search songs
- Shuffle songs
- Repeat songs

The goal of this project is to understand how a music player internally manages songs, playlists, playback controls, and user interactions.

---

# Functional Requirements

- User should be able to add songs
- User should be able to create playlists
- User should be able to add songs to playlists
- User should be able to remove songs from playlists
- User should be able to play songs
- User should be able to pause songs
- User should be able to play next song
- User should be able to play previous song
- User should be able to search songs
- User should be able to shuffle songs
- User should be able to repeat songs

---

# Non Functional Requirements

## Availability

Music player should always be available for playback operations.

---

## Reliability

Playback operations should work consistently without losing song information.

---

## Extensibility

The design should allow future features like:

- Favorites
- Recently Played
- Multiple Playlists
- Queue Management
- Streaming Support

---

# Core Entities

## User

Represents a user of the music player.

### Fields

- userId
- userName

---

## Song

Represents a song available in the system.

### Fields

- songId
- songName
- duration

### Responsibilities

- Store song metadata
- Provide song information

---

## Playlist

Represents a collection of songs.

### Fields

- songs

### Responsibilities

- Add songs
- Remove songs
- Display songs

---

## MusicPlayer

Acts as the central controller of the application.

### Responsibilities

- Manage songs
- Control playback
- Search songs
- Shuffle songs
- Repeat songs
- Navigate between songs

---

# System Flow

## Song Playback

```text
Add Song
    ↓
Select Song
    ↓
Play Song
    ↓
Pause / Resume
    ↓
Next / Previous
```

## Playlist Flow

```text
Create Playlist
    ↓
Add Songs
    ↓
Display Playlist
    ↓
Play Songs
```

---

# Features

## Play Song

Starts playing a selected song.

```text
playSong(index)
```

---

## Pause Song

Pauses the currently playing song.

```text
pauseSong()
```

---

## Next Song

Moves to the next song in the collection.

```text
nextSong()
```

---

## Previous Song

Moves to the previous song in the collection.

```text
previousSong()
```

---

## Search Song

Searches a song by its name.

```text
searchSong(songName)
```

---

## Shuffle Play

Randomly selects and plays a song.

```text
shufflePlay()
```

---

## Repeat Mode

Repeats the currently playing song.

```text
enableRepeat()
disableRepeat()
```

---

# Data Structures Used

## vector<Song>

Stores all songs available to the player.

### Reason

Provides easy indexing for:

- Play
- Next
- Previous

---

## unordered_map<int, Song>

Used inside playlists.

### Mapping

```text
songId -> Song
```

### Reason

Fast insertion and deletion of songs.

---

# Time Complexity

| Operation | Complexity |
|------------|------------|
| Add Song | O(1) |
| Remove Song | O(1) |
| Search Song | O(n) |
| Play Song | O(1) |
| Next Song | O(1) |
| Previous Song | O(1) |
| Shuffle Song | O(1) |

---

# Edge Cases Handled

## Invalid Song Selection

```text
Song index out of range
```

System prevents playback.

---

## Empty Music Library

```text
No songs available
```

System safely handles playback requests.

---

## Previous Song on First Song

```text
Already at first song
```

---

## Next Song on Last Song

```text
Last song reached
```

---

## Search Miss

```text
Song Not Found
```

---

# Design Decisions

## Why Separate Song Entity?

Song metadata should be independent of playback logic.

This improves maintainability and reusability.

---

## Why Separate Playlist Entity?

A playlist is a collection of songs and should manage song organization independently.

---

## Why MusicPlayer Entity?

MusicPlayer acts as the orchestrator responsible for:

- Playback control
- Navigation
- Search
- Shuffle
- Repeat

---

# Limitations

Current implementation does not include:

- Persistent storage
- Audio streaming
- User authentication
- Playlist sharing
- Queue management
- Favorites
- Recently played songs
- Multi-device synchronization

---

# Future Enhancements

## Queue Support

```text
Play Next
Add to Queue
Remove from Queue
```

---

## Multiple Playlist Support

```text
Workout Playlist
Study Playlist
Road Trip Playlist
```

---

## Favorites

Allow users to mark songs as favorites.

---

## Recently Played

Track listening history.

---

## Repeat Playlist

Repeat an entire playlist instead of a single song.

---

## Smart Shuffle

Avoid repeating recently played songs.

---

## Streaming Support

Support online music playback.

---

## Search Optimization

Use:

```text
unordered_map<songName, Song>
```

for faster searches.

---

# Design Patterns That Can Be Added

## Strategy Pattern

For:

- Shuffle Strategy
- Repeat Strategy

---

## Observer Pattern

For:

- UI updates
- Playback notifications

---

## Singleton Pattern

For:

- MusicPlayer instance

---

# Learning Outcomes

This project demonstrates:

- Object-Oriented Design
- Entity Modeling
- Playback Management
- Playlist Management
- Search Functionality
- Shuffle and Repeat Features
- Low Level System Design

---
