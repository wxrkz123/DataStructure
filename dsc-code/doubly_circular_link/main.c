#include "CircularDoublyLinkedList.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- 用户自定义数据结构 ---
typedef struct {
    char* title;
    char* artist;
    int duration_s;
} PlaylistSong;

// --- Helper function for cross-platform string duplication ---
char* portable_strdup(const char* s) {
    if (!s) return NULL;
    size_t len = strlen(s) + 1;
    char* new_str = malloc(len);
    if (new_str) memcpy(new_str, s, len);
    return new_str;
}

// --- 用户自定义的回调函数 ---
void free_song(void* data) {
    PlaylistSong* song = (PlaylistSong*)data;
    if (song) {
        free(song->title);
        free(song->artist);
        free(song);
    }
}

int compare_song_by_title(const void* data1, const void* data2) {
    const PlaylistSong* song1 = (const PlaylistSong*)data1;
    const PlaylistSong* song2 = (const PlaylistSong*)data2;
    return strcmp(song1->title, song2->title);
}

void print_song_action(void* data, void* context) {
    const PlaylistSong* song = (const PlaylistSong*)data;
    int* counter = (int*)context;
    printf("  %d. '%s' by %s (%d sec)\n", (*counter)++, song->title, song->artist, song->duration_s);
}

// --- Helper to create a song ---
PlaylistSong* create_song(const char* title, const char* artist, int duration) {
    PlaylistSong* song = malloc(sizeof(PlaylistSong));
    song->title = portable_strdup(title);
    song->artist = portable_strdup(artist);
    song->duration_s = duration;
    return song;
}

int main() {
    printf("--- Circular Doubly Linked List Demo: Music Playlist ---\n\n");

    // 1. 创建播放列表
    printf("Step 1: Creating a playlist...\n");
    CircularDoublyLinkedList* playlist = List_Create(10, free_song);

    // 2. 添加歌曲
    printf("\nStep 2: Adding songs to the playlist...\n");
    List_Append(playlist, create_song("Bohemian Rhapsody", "Queen", 355));
    List_Append(playlist, create_song("Stairway to Heaven", "Led Zeppelin", 482));
    List_Append(playlist, create_song("Hotel California", "Eagles", 391));
    List_Prepend(playlist, create_song("Imagine", "John Lennon", 183));

    // 3. 打印播放列表
    printf("\nStep 3: Displaying the full playlist (Size: %zu)\n", List_GetSize(playlist));
    int counter = 1;
    List_ForEach(playlist, print_song_action, &counter);

    // 4. 模拟播放和旋转（切换到下一首）
    printf("\nStep 4: Simulating playback and rotation...\n");


    CDListNode* current_song_node = List_GetHeadNode(playlist);
    printf("Now Playing: '%s'\n", ((PlaylistSong*)CDListNode_GetData(current_song_node))->title);

    List_RotateForward(playlist); // 切换到下一首
    current_song_node = List_GetHeadNode(playlist);
    printf("Next Song:   '%s'\n", ((PlaylistSong*)CDListNode_GetData(current_song_node))->title);

    List_RotateForward(playlist); // 再切换
    current_song_node = List_GetHeadNode(playlist);
    printf("Next Song:   '%s'\n", ((PlaylistSong*)CDListNode_GetData(current_song_node))->title);

    // 5. 打印旋转后的列表顺序
    printf("\nStep 5: Displaying playlist after rotation...\n");
    counter = 1;
    List_ForEach(playlist, print_song_action, &counter);

    // 6. 删除一首歌
    printf("\nStep 6: Removing 'Stairway to Heaven' from the playlist...\n");
    PlaylistSong search_key = { "Stairway to Heaven", NULL, 0 };
    CDListNode* node_to_delete = List_Find(playlist, &search_key, compare_song_by_title);
    if (node_to_delete) {
        List_DeleteNode(playlist, node_to_delete);
        printf("Song removed. New playlist size: %zu\n", List_GetSize(playlist));
    }

    printf("\n--- Playlist after deletion ---\n");
    counter = 1;
    List_ForEach(playlist, print_song_action, &counter);

    // 7. 销毁播放列表
    printf("\nStep 7: Shutting down and destroying the playlist...\n");
    List_Destroy(&playlist);
    printf("Playlist destroyed. Pointer is now NULL: %s\n", playlist == NULL ? "Yes" : "No");

    printf("\n--- Demo Finished ---\n");

    return 0;
}