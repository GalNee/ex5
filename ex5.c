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

typedef struct Playlist {
    char *name;
    Song **songs;
    int songsNum;
} Playlist;

//a function to free a song
void freeSong(Song *song) {
    free(song->title);
    free(song->artist);
    free(song->lyrics);
    free(song);
}

//free an entire playlist
void freePlaylist(Playlist *playlist) {
    for (int i = 0; i < playlist->songsNum; i++) freeSong(playlist->songs[i]);
    free(playlist->name);
    free(playlist);
}

//function to delete a song
void deleteSong(Playlist *playlist, int index) {
    freeSong(playlist->songs[index]);
    //push everything after the deleted value to the right
    for (int i = index; i < playlist->songsNum - 1; i++) playlist->songs[i] = playlist->songs[i + 1];
    playlist->songsNum--;
    playlist->songs[playlist->songsNum] = NULL;
    if (playlist->songsNum == 0) {
        free(playlist->songs);
        playlist->songs = malloc(0);
    } else {
        playlist->songs = realloc(playlist->songs, playlist->songsNum * sizeof(Song *));
        if (playlist->songs == NULL) {
            printf("Error in realloc\n");
            exit(1);
        }
    }
    printf("Song deleted successfully.\n");
}

void printSongsDetails(Playlist *playlist) {
    for (int i = 0; i < playlist->songsNum; i++) {
        printf("%d. Title: %s\n"
               "   Artist: %s\n"
               "   Released: %d\n"
               "   Streams: %d\n\n", i + 1, playlist->songs[i]->title, playlist->songs[i]->artist,
               playlist->songs[i]->year, playlist->songs[i]->streams);
    }
}

void delSong(Playlist *playlist) {
    printSongsDetails(playlist);
    int choice;
    printf("choose a song to delete, or enter 0 to quit:\n");
    scanf(" %d", &choice);
    if (choice > 0 && choice <= playlist->songsNum) deleteSong(playlist, choice - 1);
}

void playSong(Song *song) {
    song->streams++;
    printf("Now playing %s:\n$ %s $\n", song->title, song->lyrics);
}

void playPlaylist(Playlist *playlist) {
    for (int i = 0; i < playlist->songsNum; i++) {
        playSong(playlist->songs[i]);
        printf("\n");
    }
}


void printMainMenu() {
    printf("Please Choose:\n");
    printf("\t1. Watch playlists\n\t2. Add playlist\n\t3. Remove playlist\n\t4. exit\n");
}

void printPlaylistMenu() {
    printf("\t1. Show Playlist\n\t2. Add Song\n\t3. Delete Song\n\t4. Sort\n\t5. Play\n\t6. exit\n");
}


void showPlaylists(Playlist *playlist) {
    printSongsDetails(playlist);
    int choice;
    do {
        printf("choose a song to play, or 0 to quit:\n");
        scanf(" %d", &choice);
        if (playlist->songsNum == 0) return;
        if (choice > 0 && choice <= playlist->songsNum) playSong(playlist->songs[choice - 1]);
        else if (choice != 0) printf("Invalid option\n");
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
        if (chr != '\n') str[size - 1] = chr;
        else str[size - 1] = '\0';
    }
    return str;
}

void addSong(Playlist *playlist) {
    playlist->songsNum++;
    playlist->songs = realloc(playlist->songs, playlist->songsNum * sizeof(Song *));
    if (playlist->songs == NULL) {
        printf("realloc failed\n");
        exit(1);
    }
    playlist->songs[playlist->songsNum - 1] = malloc(sizeof(Song));
    if (playlist->songs[playlist->songsNum - 1] == NULL) {
        printf("malloc failed\n");
        exit(1);
    }
    printf("Enter song's details\nTitle:\n");
    playlist->songs[playlist->songsNum - 1]->title = inputString();
    printf("Artist:\n");
    playlist->songs[playlist->songsNum - 1]->artist = inputString();
    printf("Year of release:\n");
    scanf("%d", &playlist->songs[playlist->songsNum - 1]->year);
    printf("Lyrics:\n");
    playlist->songs[playlist->songsNum - 1]->lyrics = inputString();
    playlist->songs[playlist->songsNum - 1]->streams = 0;
}

//swap the location of 2 songs in a location
void swapSong(Playlist *playlist, int j) {
    Song *temp = playlist->songs[j];
    playlist->songs[j] = playlist->songs[j + 1];
    playlist->songs[j + 1] = temp;
}

void sortYear(Playlist *playlist) {
    for (int i = 0; i < playlist->songsNum - 1; i++) {
        for (int j = 0; j < playlist->songsNum - i - 1; j++) {
            if (playlist->songs[j]->year > playlist->songs[j + 1]->year) swapSong(playlist, j);
        }
    }
}

//the multiplier is to know if its ascending or descending
void sortStreams(Playlist *playlist, int multiplier) {
    for (int i = 0; i < playlist->songsNum - 1; i++) {
        for (int j = 0; j < playlist->songsNum - i - 1; j++) {
            if (multiplier * playlist->songs[j]->streams > multiplier * playlist->songs[j + 1]->streams)
                swapSong(playlist, j);
        }
    }
}

void sortAlphabet(Playlist *playlist) {
    for (int i = 0; i < playlist->songsNum - 1; i++) {
        for (int j = 0; j < playlist->songsNum - i - 1; j++) {
            if (strcmp(playlist->songs[j]->title, playlist->songs[j + 1]->title) > 0) swapSong(playlist, j);
        }
    }
}


void printSortMenu() {
    printf("choose:\n1. sort by year\n2. sort by streams - ascending order\n3. sort by streams - descending order"
           "\n4. sort alphabetically\n");
}

void sortPlaylist(Playlist *playlist) {
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
    printf("sorted\n");
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
                delSong(playlist);
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

void printPlaylistsList(Playlist **playlists, int size) {
    printf("Choose a playlist:\n");
    for (int i = 0; i < size; i++) {
        printf("\t%d. %s\n", i + 1, playlists[i]->name);
    }
    printf("\t%d. Back to main menu\n", size + 1);
}

void displayPlaylists(Playlist **playlists, int size) {
    int choice = -1;
    while (choice != size + 1) {
        printPlaylistsList(playlists, size);
        scanf(" %d", &choice);
        if (choice > size + 1 || choice < 1) printf("Invalid option.\n");
        else if (choice != size + 1) playlistsMenu(playlists[choice - 1]);
    }
}

void addPlaylist(Playlist **playlists, int *size) {
    (*size)++;
    playlists = realloc(playlists, sizeof(Playlist *) * (*size));
    if (playlists == NULL) {
        printf("malloc failed\n");
        exit(1);
    }
    //creating a new playlist to append into the playlists list
    Playlist *newPlaylist = malloc(sizeof(Playlist));
    if (newPlaylist == NULL) {
        printf("malloc failed\n");
        exit(1);
    }
    printf("Enter playlist's name:\n");
    newPlaylist->name = inputString();
    newPlaylist->songsNum = 0;
    newPlaylist->songs = malloc(0);
    if (newPlaylist->songs == NULL) {
        printf("malloc failed\n");
        exit(1);
    }
    playlists[*size - 1] = newPlaylist;
}

//a function to remove a playlist
void removePlaylist(Playlist **playlists, int *size) {
    int choice = -1;
    while (choice != *size + 1) {
        printPlaylistsList(playlists, *size);
        scanf(" %d", &choice);
        if (choice > *size + 1 || choice < 1) printf("Invalid option.\n");
        else if (choice != *size + 1) {
            (*size)--;
            freePlaylist(playlists[choice - 1]);
            for (int i = choice - 1; i < *size; i++) playlists[i] = playlists[i + 1];
            playlists[*size] = NULL;
            if (*size == 0) playlists = malloc(0);
            else playlists = realloc(playlists, sizeof(Playlist*) * (*size));
            if (playlists == NULL) {
                printf("malloc failed\n");
                exit(1);
            }
            choice = -1;
        }
    }
}


int main() {
    Playlist **playlists = malloc(0);
    if (playlists == NULL) {
        printf("malloc failed\n");
        return 1;
    }
    int size = 0;
    int choice = -1;
    while (choice != 4) {
        printMainMenu();
        scanf(" %d", &choice);
        switch (choice) {
            case 1:
                displayPlaylists(playlists, size);
                break;
            case 2:
                addPlaylist(playlists, &size);
                break;
            case 3:
                removePlaylist(playlists, &size);
                break;
            case 4:
                printf("Goodbye!\n");
                break;
            default:
                printf("Invalid option\n");
                break;
        }
    }
    for (int i = 0; i < size; i++) {
        freePlaylist(playlists[i]);
    }
    free(playlists);
}
