/******************
Name: Gal Neeman
ID: 331981365
Assignment: ex5
*******************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct Song {
    char *title;
    char *artist;
    int year;
    char *lyrics;
    int streams;
} Song;

typedef struct SongNode {
    Song *data;
    struct SongNode *next;
} SongNode;

typedef struct Playlist {
    char *name;
    SongNode *songs;
    int songsNum;
} Playlist;

typedef struct PlaylistNode {
    Playlist *data;
    struct PlaylistNode *next;
} PlaylistNode;

//a function to free a song
void freeSong(Song *song) {
    free(song->title);
    free(song->artist);
    free(song->lyrics);
    free(song);
}

//free an entire playlist
void freePlaylist(Playlist *playlist) {
    while (playlist->songs != NULL) {
        freeSong(playlist->songs->data);
        playlist->songs = playlist->songs->next;
    }
    free(playlist->songs);
    free(playlist->name);
    free(playlist);
}

void printSongsDetails(Playlist *playlist) {
    int counter = 1;
    SongNode *curr = playlist->songs;
    while (curr != NULL) {
        printf("%d. Title: %s\n"
               "   Artist: %s\n"
               "   Released: %d\n"
               "   Streams: %d\n\n", counter, curr->data->title, curr->data->artist, curr->data->year,
               curr->data->streams);
        curr = curr->next;
        counter++;
    }
}

void deleteSong(Playlist *playlist) {
    printSongsDetails(playlist);
    int choice;
    printf("choose a song to delete, or enter 0 to quit:\n");
    scanf(" %d", &choice);
    //if the choice is in the range
    if (choice > 0 && choice <= playlist->songsNum) {
        playlist->songsNum--;
        if (playlist->songsNum == 0) playlist->songs = NULL;
        else {
            SongNode *curr = playlist->songs, *prev = NULL;
            int counter = 0;
            while (counter < choice - 1) {
                prev = curr;
                curr = curr->next;
                counter++;
            }
            freeSong(curr->data);
            if (prev != NULL) prev->next = curr->next;
                //if you chose to delete the first song
            else playlist->songs = curr->next;
        }
        printf("Song deleted successfully.\n");
    }
}

//plays the song and adds to streams
void playSong(Song *song) {
    song->streams++;
    printf("Now playing %s:\n$ %s $\n", song->title, song->lyrics);
}

//plays the entire playlist
void playPlaylist(Playlist *playlist) {
    SongNode *current = playlist->songs;
    while (current != NULL) {
        playSong(current->data);
        current = current->next;
        printf("\n");
    }
}

//displays all the songs in the playlist and allows to play one of them
void showPlaylists(Playlist *playlist) {
    printSongsDetails(playlist);
    int choice;
    do {
        printf("choose a song to play, or 0 to quit:\n");
        scanf(" %d", &choice);
        if (playlist->songsNum == 0) return;
        if (choice > 0 && choice <= playlist->songsNum) {
            int counter = 0;
            SongNode *curr = playlist->songs;
            while (counter < choice - 1) {
                curr = curr->next;
                counter++;
            }
            playSong(curr->data);
        } else if (choice != 0) printf("Invalid option\n");
    } while (choice != 0);
}

//a function that gets an input string
char *inputString() {
    int size = 0;
    char *str = malloc(size), chr = '\0';
    if (str == NULL) {
        printf("malloc error");
        exit(1);
    }
    scanf(" ");
    //stop at the end of the line
    while (chr != '\n') {
        size++;
        scanf("%c", &chr);
        str = realloc(str, sizeof(char) * size);
        if (str == NULL) {
            printf("malloc error");
            exit(1);
        }
        if (chr != '\n' && chr != '\r') str[size - 1] = chr;
        else str[size - 1] = '\0';
    }
    return str;
}

void addSong(Playlist *playlist) {
    playlist->songsNum++;
    Song *newSong = malloc(sizeof(Song));
    if (newSong == NULL) {
        printf("malloc error");
        exit(1);
    }
    printf("Enter song's details\nTitle:\n");
    newSong->title = inputString();
    printf("Artist:\n");
    newSong->artist = inputString();
    printf("Year of release:\n");
    scanf(" %d", &newSong->year);
    printf("Lyrics:\n");
    newSong->lyrics = inputString();
    newSong->streams = 0;
    SongNode *newNode = malloc(sizeof(SongNode));
    if (newNode == NULL) {
        printf("malloc error");
        exit(1);
    }
    newNode->data = newSong;
    newNode->next = NULL;
    if (playlist->songs == NULL) playlist->songs = newNode;
    else {
        SongNode *curr = playlist->songs;
        while (curr->next != NULL) curr = curr->next;
        curr->next = newNode;
    }
}

//swap the location of 2 adjacent songs
void swapSong(SongNode *curr) {
    Song *temp = curr->data;
    curr->data = curr->next->data;
    curr->next->data = temp;
}

//ascending
void sortYear(Playlist *playlist) {
    for (int i = 0; i < playlist->songsNum - 1; i++) {
        SongNode *curr = playlist->songs;
        while (curr->next != NULL) {
            if (curr->data->year > curr->next->data->year) swapSong(curr);
            curr = curr->next;
        }
    }
}

//the multiplier is to know if its ascending or descending(1 is ascending and -1 is descending)
void sortStreams(Playlist *playlist, int multiplier) {
    for (int i = 0; i < playlist->songsNum - 1; i++) {
        SongNode *curr = playlist->songs;
        while (curr->next != NULL) {
            if (multiplier * curr->data->streams > multiplier * curr->next->data->streams) swapSong(curr);
            curr = curr->next;
        }
    }
}


void sortAlphabet(Playlist *playlist) {
    for (int i = 0; i < playlist->songsNum - 1; i++) {
        SongNode *curr = playlist->songs;
        while (curr->next != NULL) {
            if (strcmp(curr->data->title, curr->next->data->title) > 0) swapSong(curr);
            curr = curr->next;
        }
    }
}


void printSortMenu() {
    printf("choose:\n1. sort by year\n2. sort by streams - ascending order\n3. sort by streams - descending order"
        "\n4. sort alphabetically\n");
}

void sortPlaylist(Playlist *playlist) {
    if (playlist->songsNum > 1) {
        int choice;
        printSortMenu();
        scanf(" %d", &choice);
        switch (choice) {
            case 1:
                sortYear(playlist);
                break;
            case 2:
                sortStreams(playlist, 1);
                break;
            case 3:
                sortStreams(playlist, -1);
                break;
            default:
                sortAlphabet(playlist);
                break;
        }
    }
    printf("sorted\n");
}

void printPlaylistMenu() {
    printf("\t1. Show Playlist\n\t2. Add Song\n\t3. Delete Song\n\t4. Sort\n\t5. Play\n\t6. exit\n");
}

void playlistsMenu(Playlist *playlist) {
    printf("playlist %s:\n", playlist->name);
    int choice = -1;
    while (1) {
        printPlaylistMenu();
        scanf(" %d", &choice);
        switch (choice) {
            case 1:
                showPlaylists(playlist);
                break;
            case 2:
                addSong(playlist);
                break;
            case 3:
                deleteSong(playlist);
                break;
            case 4:
                sortPlaylist(playlist);
                break;
            case 5:
                playPlaylist(playlist);
                break;
            case 6:
                return;
            default:
                printf("Invalid option\n");
                break;
        }
    }
}

int printPlaylistsList(PlaylistNode *playlists) {
    printf("Choose a playlist:\n");
    int counter = 0;
    while (playlists != NULL) {
        printf("\t%d. %s\n", counter + 1, playlists->data->name);
        playlists = playlists->next;
        counter++;
    }
    printf("\t%d. Back to main menu\n", counter + 1);
    return counter;
}

void displayPlaylists(PlaylistNode *playlists) {
    int choice = -1, size;
    do {
        size = printPlaylistsList(playlists);
        scanf(" %d", &choice);
        if (choice > size + 1 || choice < 1) printf("Invalid option.\n");
        else if (choice != size + 1) {
            int counter = 0;
            PlaylistNode *curr = playlists;
            while (counter < choice - 1) {
                curr = playlists->next;
                counter++;
            }
            playlistsMenu(curr->data);
        }
    } while (choice != size + 1);
}

void addPlaylist(PlaylistNode **playlists) {
    //creating a new playlist to append into the playlists list
    printf("Enter playlist's name:\n");
    Playlist *newPlaylist = malloc(sizeof(Playlist));
    if (newPlaylist == NULL) {
        printf("malloc error");
        exit(1);
    }
    newPlaylist->name = inputString();
    newPlaylist->songs = NULL;
    newPlaylist->songsNum = 0;
    PlaylistNode *newNode = malloc(sizeof(PlaylistNode));
    if (newNode == NULL) {
        printf("malloc error");
        exit(1);
    }
    newNode->data = newPlaylist;
    newNode->next = NULL;
    if (*playlists == NULL) *playlists = newNode;
    else {
        PlaylistNode *temp = *playlists;
        while (temp->next != NULL) temp = temp->next;
        temp->next = newNode;
    }
}

//a function to remove a playlist
void removePlaylist(PlaylistNode **playlists) {
    int choice, size = printPlaylistsList(*playlists);;
    scanf(" %d", &choice);
    if (choice > size + 1 || choice < 1) printf("Invalid option.\n");
    else if (choice != size + 1) {
        if (size == 1) *playlists = NULL;
        else {
            PlaylistNode *curr = *playlists, *prev = NULL;
            int counter = 0;
            while (counter < choice - 1) {
                prev = curr;
                curr = curr->next;
                counter++;
            }
            freePlaylist(curr->data);
            if (prev != NULL) prev->next = curr->next;
                //if you chose to delete the first playlist
            else *playlists = curr->next;
        }
        printf("Playlist deleted.\n");
    }
}

void printMainMenu() {
    printf("Please Choose:\n");
    printf("\t1. Watch playlists\n\t2. Add playlist\n\t3. Remove playlist\n\t4. exit\n");
}

int main() {
    PlaylistNode *playlists = NULL;
    int choice = -1;
    while (choice != 4) {
        printMainMenu();
        scanf(" %d", &choice);
        switch (choice) {
            case 1:
                displayPlaylists(playlists);
                break;
            case 2:
                addPlaylist(&playlists);
                break;
            case 3:
                removePlaylist(&playlists);
                break;
            case 4:
                printf("Goodbye!\n");
                break;
            default:
                printf("Invalid option\n");
                break;
        }
    }
    while (playlists != NULL) {
        freePlaylist(playlists->data);
        playlists = playlists->next;
    }
    free(playlists);
}
