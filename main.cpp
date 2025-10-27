#include <iostream>
#include <iomanip>
#include <windows.h>

using namespace std;

#define RED_COLOR "\033[31m"
#define GREEN_COLOR "\033[32m"
#define WHITE_COLOR "\033[0m"
#define CYAN_COLOR "\033[36m"

// Data Structures
struct Song
{
    string title;
    string artist;
    string genre;
    Song *next;
    Song *prev;
};

struct Playlist
{
    string name;
    Song *songHead;
    int songCount;
    Playlist *next;
    Playlist *prev;
};

struct SearchResult
{
    Song *song;
    SearchResult *next;
};

// Function Prototypes
void DisplayWelcomeBanner();
void DisplayMainMenu();
void DisplayLoading();
void DisplayQuitBanner();
void DisplayInvalidChoice();

// Core Functions
void AddPlaylist(Playlist *&head, string name);
void AddSongToPlaylist(Playlist *playlist, string title, string artist, string genre);
int GetValidChoice(int min, int max);

// Menu Handlers
void HandleMainMenu(Playlist *&playlists, int choice);
void HandleDisplayPlaylists(Playlist *playlists);
void HandleCreatePlaylist(Playlist *&playlists);
void HandleViewPlaylist(Playlist *playlists);
void HandlePlaylistMenu(Playlist *playlist);

// Display Functions
void ShowAllPlaylists(Playlist *head);
void ShowPlaylistSelection(Playlist *head);
void ShowSongsForward(Playlist *playlist);
void ShowSongsBackward(Playlist *playlist);
void ShowFirstSong(Playlist *playlist);
void ShowLastSong(Playlist *playlist);

// Search Functions
void SearchByTitle(Playlist *playlist, string title);
void SearchByArtist(Playlist *playlist, string artist);
void SearchByGenre(Playlist *playlist, string genre);
void DisplaySearchResults(SearchResult *results, string searchType, string searchTerm);
void ShowSearchMenu();
void HandleSearchMenu(Playlist *playlist);
void AddToSearchResults(SearchResult *&head, Song *song);
void ClearSearchResults(SearchResult *&head);
int CountSearchResults(SearchResult *head);

// Input Functions
void GetPlaylistName(Playlist *&head);
void GetSongDetails(Playlist *playlist);
Playlist* SelectPlaylist(Playlist *head);

// Menu Display Functions
void ShowPlaylistOptions();
void ShowSongOptions();
void ShowEmptyPlaylistOptions();

// Duplicate Playlist
void DuplicatePlaylist(Playlist *&head);

int main()
{
    Playlist *playlists = NULL;
    
    DisplayWelcomeBanner();
    sleep(2);
    system("cls");
    DisplayLoading();
    
    while (true)
    {
        DisplayMainMenu();
        int choice = GetValidChoice(0, 5);
        
        if (choice == 5)
        {
            DisplayLoading();
            DisplayQuitBanner();
            break;
        }
        
        HandleMainMenu(playlists, choice);
    }
    
    return 0;
}

// ==================== DISPLAY FUNCTIONS ====================

void DisplayWelcomeBanner()
{
    cout << "======================================================================" << endl;
    cout << "||                                                                  ||" << endl;
    cout << "||                                                                  ||" << endl;
    cout << "||                                                                  ||" << endl;
    cout << "||                          W E L C O M E !                         ||" << endl;
    cout << "||                                                                  ||" << endl;
    cout << "||                                                                  ||" << endl;
    cout << "||                                                                  ||" << endl;
    cout << "======================================================================" << endl;
}

void DisplayMainMenu()
{
    cout << "============================= MENU ===================================" << endl;
    cout << "||                                                                  ||" << endl;
    cout << "||                                                                  ||" << endl;
    cout << "||  [0] Display all playlists                                       ||" << endl;
    cout << "||  [1] Create a new playlist                                       ||" << endl;
    cout << "||  [2] Go to playlist                                              ||" << endl;
    cout << "||  [3] Duplicate playlist                                          ||" << endl;
    cout << "||  [4] Delete playlist                                             ||" << endl;
    cout << "||  [5] Exit Program                                                ||" << endl;
    cout << "||                                                                  ||" << endl;
    cout << "======================================================================" << endl;
}

void DisplayLoading()
{
    cout << endl << endl;
    cout << "==============================" << endl;
    cout << "||      L O A D I N G       ||" << endl;
    cout << "==============================" << endl;
    sleep(1);
    system("cls");
}

void DisplayQuitBanner()
{
    cout << "======================================================================" << endl;
    cout << "||                                                                  ||" << endl;
    cout << "||                                                                  ||" << endl;
    cout << "||                                                                  ||" << endl;
    cout << "||               " << GREEN_COLOR << "P R O G R A M   S U C C E S S F U L L Y" << WHITE_COLOR << "            ||" << endl;
    cout << "||                           " << GREEN_COLOR << "Q U I T !" << WHITE_COLOR << "                              ||" << endl;
    cout << "||                                                                  ||" << endl;
    cout << "||                                                                  ||" << endl;
    cout << "||                                                                  ||" << endl;
    cout << "======================================================================" << endl;
}

void DisplayInvalidChoice()
{
    cout << RED_COLOR << "Invalid choice. Please try again." << WHITE_COLOR << endl;
}

// ==================== CORE FUNCTIONS ====================

void AddPlaylist(Playlist *&head, string name)
{
    Playlist *newPlaylist = new Playlist;
    newPlaylist->name = name;
    newPlaylist->songHead = NULL;
    newPlaylist->songCount = 0;
    newPlaylist->next = NULL;
    newPlaylist->prev = NULL;

    if (!head)
    {
        head = newPlaylist;
        return;
    }

    Playlist *current = head;
    Playlist *prev = NULL;

    while (current && current->name < name)
    {
        prev = current;
        current = current->next;
    }

    if (!prev)
    {
        newPlaylist->next = head;
        head->prev = newPlaylist;
        head = newPlaylist;
    }
    else
    {
        newPlaylist->next = current;
        newPlaylist->prev = prev;
        prev->next = newPlaylist;
        if (current)
            current->prev = newPlaylist;
    }
}

void AddSongToPlaylist(Playlist *playlist, string title, string artist, string genre)
{
    if (!playlist) return;

    Song *newSong = new Song;
    newSong->title = title;
    newSong->artist = artist;
    newSong->genre = genre;
    newSong->next = NULL;
    newSong->prev = NULL;

    if (!playlist->songHead)
    {
        playlist->songHead = newSong;
        playlist->songCount++;
        return;
    }

    Song *current = playlist->songHead;
    Song *prev = NULL;

    while (current && current->title < title)
    {
        prev = current;
        current = current->next;
    }

    if (!prev)
    {
        newSong->next = playlist->songHead;
        playlist->songHead->prev = newSong;
        playlist->songHead = newSong;
    }
    else
    {
        newSong->next = current;
        newSong->prev = prev;
        prev->next = newSong;
        if (current)
            current->prev = newSong;
    }
    
    playlist->songCount++;
}

int GetValidChoice(int min, int max)
{
    int choice;
    do
    {
        cout << "Enter your choice (" << min << "-" << max << "): ";
        cin >> choice;
        
        if (choice < min || choice > max)
            DisplayInvalidChoice();
    } while (choice < min || choice > max);
    
    return choice;
}

// ==================== MENU HANDLERS ====================

void HandleMainMenu(Playlist *&playlists, int choice)
{
    switch (choice)
    {
        case 0:
            system("cls");
            DisplayLoading();
            HandleDisplayPlaylists(playlists);
            break;
        case 1:
            HandleCreatePlaylist(playlists);
            break;
        case 2:
            HandleViewPlaylist(playlists);
            break;
        case 3:
            DuplicatePlaylist(playlists);
            break;
        case 4:
            cout << "Delete playlist functionality not yet implemented." << endl;
            break;
    }
}

void HandleDisplayPlaylists(Playlist *playlists)
{
    ShowAllPlaylists(playlists);
    
    if (!playlists)
    {
        cout << "Press Enter to return to the main menu...";
        cin.ignore();
        cin.get();
        system("cls");
        DisplayLoading();
        return;
    }
    
    cout << endl << endl;
    ShowPlaylistOptions();
    
    int choice = GetValidChoice(0, 1);
    if (choice == 0)
    {
        system("cls");
        DisplayLoading();
        return;
    }
    
    if (choice == 1)
    {
        system("cls");
        DisplayLoading();
        ShowPlaylistSelection(playlists);
    }
}

void HandleCreatePlaylist(Playlist *&playlists)
{
    system("cls");
    DisplayLoading();
    GetPlaylistName(playlists);
    
    cout << endl;
    cout << "=======================================================================" << endl;
    cout << "||                                                                   ||" << endl;
    cout << "||                Press 0 to go back to the menu                     ||" << endl;
    cout << "||                Press 1 add song to this playlist                  ||" << endl;
    cout << "||                Press 2 add another playlist                       ||" << endl;
    cout << "||                                                                   ||" << endl;
    cout << "=======================================================================" << endl;
    
    int choice = GetValidChoice(0, 2);
    if (choice == 0)
    {
        system("cls");
        DisplayLoading();
    }
    else if (choice == 1)
    {
        Playlist *newPlaylist = playlists;
        while (newPlaylist && newPlaylist->next)
            newPlaylist = newPlaylist->next;
        
        if (newPlaylist)
        {
            system("cls");
            DisplayLoading();
            GetSongDetails(newPlaylist);
        }
    }
    else if (choice == 2)
    {
        HandleCreatePlaylist(playlists);
    }
}

void HandleViewPlaylist(Playlist *playlists)
{
    if (!playlists)
    {
        cout << "No playlists available. Please create a playlist first!" << endl;
        return;
    }
    
    system("cls");
    DisplayLoading();
    ShowPlaylistSelection(playlists);
}

void HandlePlaylistMenu(Playlist *playlist)
{
    if (!playlist || !playlist->songHead)
    {
        cout << endl;
        cout << "======================================================================" << endl;
        cout << "       Now viewing playlist: " << playlist->name << endl;
        cout << "======================================================================" << endl;
        cout << RED_COLOR << "This playlist has no songs yet." << WHITE_COLOR << endl;
        ShowEmptyPlaylistOptions();
        
        int choice = GetValidChoice(0, 1);
        if (choice == 0)
        {
            system("cls");
            DisplayLoading();
            return;
        }
        else if (choice == 1)
        {
            system("cls");
            DisplayLoading();
            GetSongDetails(playlist);
        }
        return;
    }
    
    int choice;
    do
    {
        cout << endl;
        cout << "======================================================================" << endl;
        cout << "       Now viewing playlist: " << playlist->name << endl;
        cout << "======================================================================" << endl;
        ShowSongOptions();
        cout << "Enter your choice: ";
        cin >> choice;
        
        system("cls");
        DisplayLoading();
        
        switch (choice)
        {
            case 1:
                ShowSongsForward(playlist);
                break;
            case 2:
                ShowSongsBackward(playlist);
                break;
            case 3:
                ShowFirstSong(playlist);
                break;
            case 4:
                ShowLastSong(playlist);
                break;
            case 5:
                HandleSearchMenu(playlist);
                // After returning from search, clear screen and show loading
                system("cls");
                DisplayLoading();
                break;
            case 0:
                cout << "Returning..." << endl;
                system("cls");
                DisplayLoading();
                break;
            default:
                cout << RED_COLOR << "Invalid choice. Try again." << WHITE_COLOR << endl;
        }
        
        if (choice != 0 && choice != 5)
        {
            cout << endl << "Press Enter to continue...";
            cin.ignore();
            cin.get();
            system("cls");
            DisplayLoading();
        }
        
    } while (choice != 0);
}

// ==================== DISPLAY FUNCTIONS ====================

void ShowAllPlaylists(Playlist *head)
{
    cout << endl;
    cout << "========================= AVAILABLE PLAYLIST ==========================" << endl;
    
    if (!head)
    {
        cout << endl << endl;
        cout << RED_COLOR << "No playlist available." << WHITE_COLOR << endl;
        cout << endl << endl;
        cout << "=======================================================================" << endl;
        cout << "Total playlists: 0" << endl;
        cout << "=======================================================================" << endl;
        cout << endl << endl;
        return;
    }
    
    int index = 1;
    int totalPlaylists = 0;
    Playlist *current = head;
    
    while (current)
    {
        cout << index << ". " << current->name << " (" << current->songCount << " songs)" << endl;
        current = current->next;
        index++;
        totalPlaylists++;
    }
    
    cout << "=======================================================================" << endl;
    cout << "Total playlists: " << totalPlaylists << endl;
    cout << "=======================================================================" << endl;
}

void ShowPlaylistSelection(Playlist *head)
{
    Playlist *current = head;
    int index = 1;
    int playlistCount = 0;
    
    cout << endl;
    cout << "========================= SELECT PLAYLIST ==========================" << endl;
    
    while(current)
    {
        cout << index << ". " << current->name << " (" << current->songCount << " songs)" << endl;
        current = current->next;
        index++;
        playlistCount++;
    }
    
    cout << "=======================================================================" << endl;
    cout << "Total playlists: " << playlistCount << endl;
    cout << "=======================================================================" << endl;
    
    int selectedNum;
    do
    {
        cout << endl << "Enter playlist number: ";
        cin >> selectedNum;
        
        if(selectedNum < 1 || selectedNum > playlistCount)
        {
            cout << RED_COLOR << "Invalid number. Please try again." << WHITE_COLOR << endl;
        }
    } while(selectedNum < 1 || selectedNum > playlistCount);
    
    current = head;
    for(int i = 1; i < selectedNum; i++)
        current = current->next;
        
    system("cls");
    DisplayLoading();
    HandlePlaylistMenu(current);
}


void ShowSongsForward(Playlist *playlist)
{
    cout << endl;
    cout << "========================= SONGS (FORWARD ORDER) =========================" << endl;
    
    Song *song = playlist->songHead;
    int songNumber = 1;
    while(song)
    {
        cout << songNumber << ". " << song->title << " by " << song->artist << " [" << song->genre << "]" << endl;
        song = song->next;
        songNumber++;
    }
    
    cout << "=======================================================================" << endl;
    cout << "Total songs: " << playlist->songCount << endl;
    cout << "=======================================================================" << endl;
}

void ShowSongsBackward(Playlist *playlist)
{
    cout << endl;
    cout << "========================= SONGS (BACKWARD ORDER) ========================" << endl;
    
    Song *song = playlist->songHead;
    while(song && song->next)
        song = song->next;
    
    int songNumber = playlist->songCount;
    while(song)
    {
        cout << songNumber << ". " << song->title << " by " << song->artist << " [" << song->genre << "]" << endl;
        song = song->prev;
        songNumber--;
    }
    
    cout << "=======================================================================" << endl;
    cout << "Total songs: " << playlist->songCount << endl;
    cout << "=======================================================================" << endl;
}

void ShowFirstSong(Playlist *playlist)
{
    cout << endl;
    cout << "========================= FIRST SONG ==========================" << endl;
    cout << "1. " << playlist->songHead->title << " by " << playlist->songHead->artist << " [" << playlist->songHead->genre << "]" << endl;
    cout << "=======================================================================" << endl;
    cout << "Total songs: " << playlist->songCount << endl;
    cout << "=======================================================================" << endl;
}

void ShowLastSong(Playlist *playlist)
{
    cout << endl;
    cout << "========================= LAST SONG ==========================" << endl;
    Song *song = playlist->songHead;
    while(song && song->next)
        song = song->next;
    cout << playlist->songCount << ". " << song->title << " by " << song->artist << " [" << song->genre << "]" << endl;
    cout << "=======================================================================" << endl;
    cout << "Total songs: " << playlist->songCount << endl;
    cout << "=======================================================================" << endl;
}

// ==================== INPUT FUNCTIONS ====================

void GetPlaylistName(Playlist *&head)
{
    string name;
    cout << "========== ADD NEW PLAYLIST ==========" << endl << endl;
    
    cin.ignore(); 
    do
    {
        cout << "Enter playlist name: ";
        getline(cin, name);
        if (name.empty())
        {
            DisplayInvalidChoice();
        }
    } while (name.empty());
    
    AddPlaylist(head, name);
    cout << endl << GREEN_COLOR << "Playlist \"" << name << "\" added successfully!" << WHITE_COLOR << endl;
}

void GetSongDetails(Playlist *playlist)
{
    if (!playlist)
    {
        cout << RED_COLOR << "No playlist available. Please create a playlist first!" << WHITE_COLOR << endl;
        return;
    }
    
    string title, artist, genre;
    cout << "========== ADD SONG TO PLAYLIST ==========" << endl << endl;
    cout << CYAN_COLOR << "CURRENT PLAYLIST: " << playlist->name << WHITE_COLOR << endl << endl;
    
    cin.ignore(); 
    do
    {
        cout << "Enter song title: ";
        getline(cin, title);
        if (title.empty())
        {
            DisplayInvalidChoice();
        }
    } while (title.empty());
    
    do
    {
        cout << "Enter artist name: ";
        getline(cin, artist);
        if (artist.empty())
        {
            DisplayInvalidChoice();
        }
    } while (artist.empty());
    
    do
    {
        cout << "Enter genre: ";
        getline(cin, genre);
        if (genre.empty())
        {
            DisplayInvalidChoice();
        }
    } while (genre.empty());
    
    AddSongToPlaylist(playlist, title, artist, genre);
    cout << endl << GREEN_COLOR << "Song \"" << title << "\" added to playlist \"" << playlist->name << "\" successfully!" << WHITE_COLOR << endl;
    
    cout << endl;
    cout << "=======================================================================" << endl;
    cout << "||                                                                   ||" << endl;
    cout << "||                Press 0 to go back to the main menu                ||" << endl;
    cout << "||                Press 1 to add another song to this playlist       ||" << endl;
    cout << "||                                                                   ||" << endl;
    cout << "=======================================================================" << endl;
    
    int choice = GetValidChoice(0, 1);
    if (choice == 0)
    {
        system("cls");
        DisplayLoading();
    }
    else if (choice == 1)
    {
        system("cls");
        DisplayLoading();
        GetSongDetails(playlist);
    }
}

// ==================== MENU DISPLAY FUNCTIONS ====================


void ShowPlaylistOptions()
{
    cout << endl;
    cout << "=======================================================================" << endl;
    cout << "||                                                                   ||" << endl;
    cout << "||                Press 0 to go back to the main menu                ||" << endl;
    cout << "||                Press 1 to display songs in playlist               ||" << endl;
    cout << "||                                                                   ||" << endl;
    cout << "=======================================================================" << endl;
}

void ShowSongOptions()
{
    cout << endl;
    cout << "======================================================================" << endl;
    cout << "||                                                                  ||" << endl;
    cout << "||                                                                  ||" << endl;
    cout << "||  [1] Display songs (forward order)                               ||" << endl;
    cout << "||  [2] Display songs (backward order)                              ||" << endl;
    cout << "||  [3] Show first song only                                        ||" << endl;
    cout << "||  [4] Show last song only                                         ||" << endl;
    cout << "||  [5] Search songs                                                ||" << endl;
    cout << "||  [0] Return to previous menu                                     ||" << endl;
    cout << "||                                                                  ||" << endl;
    cout << "======================================================================" << endl;
}

void ShowEmptyPlaylistOptions()
{
    cout << endl;
    cout << "=======================================================================" << endl;
    cout << "||                                                                   ||" << endl;
    cout << "||                Press 0 to go back to the main menu                ||" << endl;
    cout << "||                Press 1 to add song to this playlist               ||" << endl;
    cout << "||                                                                   ||" << endl;
    cout << "=======================================================================" << endl;
}

// ==================== SEARCH FUNCTIONS ====================

void AddToSearchResults(SearchResult *&head, Song *song)
{
    SearchResult *newResult = new SearchResult;
    newResult->song = song;
    newResult->next = head;
    head = newResult;
}

void ClearSearchResults(SearchResult *&head)
{
    while (head)
    {
        SearchResult *temp = head;
        head = head->next;
        delete temp;
    }
}

int CountSearchResults(SearchResult *head)
{
    int count = 0;
    SearchResult *current = head;
    while (current)
    {
        count++;
        current = current->next;
    }
    return count;
}

void SearchByTitle(Playlist *playlist, string title)
{
    SearchResult *results = NULL;
    
    if (!playlist || !playlist->songHead) 
    {
        DisplaySearchResults(results, "title", title);
        return;
    }
    
    Song *current = playlist->songHead;
    while (current)
    {
        if (current->title == title)
        {
            AddToSearchResults(results, current);
        }
        current = current->next;
    }
    
    DisplaySearchResults(results, "title", title);
    ClearSearchResults(results);
}

void SearchByArtist(Playlist *playlist, string artist)
{
    SearchResult *results = NULL;
    
    if (!playlist || !playlist->songHead) 
    {
        DisplaySearchResults(results, "artist", artist);
        return;
    }
    
    Song *current = playlist->songHead;
    while (current)
    {
        if (current->artist == artist)
        {
            AddToSearchResults(results, current);
        }
        current = current->next;
    }
    
    DisplaySearchResults(results, "artist", artist);
    ClearSearchResults(results);
}

void SearchByGenre(Playlist *playlist, string genre)
{
    SearchResult *results = NULL;
    
    if (!playlist || !playlist->songHead) 
    {
        DisplaySearchResults(results, "genre", genre);
        return;
    }
    
    Song *current = playlist->songHead;
    while (current)
    {
        if (current->genre == genre)
        {
            AddToSearchResults(results, current);
        }
        current = current->next;
    }
    
    DisplaySearchResults(results, "genre", genre);
    ClearSearchResults(results);
}

void DisplaySearchResults(SearchResult *results, string searchType, string searchTerm)
{
    cout << endl;
    cout << "========================= SEARCH RESULTS =========================" << endl;
    
    int count = CountSearchResults(results);
    
    if (count == 0)
    {
        cout << RED_COLOR << "No songs found with " << searchType << ": \"" << searchTerm << "\"" << WHITE_COLOR << endl;
    }
    else
    {
        cout << "Found " << count << " song(s) with " << searchType << ": \"" << searchTerm << "\"" << endl;
        
        SearchResult *current = results;
        int index = 1;
        while (current)
        {
            cout << index << ". " << current->song->title << " by " << current->song->artist << " [" << current->song->genre << "]" << endl;
            current = current->next;
            index++;
        }
    }
    
    cout << "=======================================================================" << endl;
}

void ShowSearchMenu()
{
    cout << endl;
    cout << "======================================================================" << endl;
    cout << "||                                                                  ||" << endl;
    cout << "||                                                                  ||" << endl;
    cout << "||  [1] Search by song title                                        ||" << endl;
    cout << "||  [2] Search by artist name                                       ||" << endl;
    cout << "||  [3] Search by genre                                             ||" << endl;
    cout << "||  [0] Return to previous menu                                     ||" << endl;
    cout << "||                                                                  ||" << endl;
    cout << "======================================================================" << endl;
}

void HandleSearchMenu(Playlist *playlist)
{
    int choice;
    do
    {
        ShowSearchMenu();
        cout << "Enter your choice: ";
        cin >> choice;
        
        system("cls");
        DisplayLoading();
        
        if (choice >= 1 && choice <= 3)
        {
            string searchTerm;
            cout << "Enter " << (choice == 1 ? "song title" : choice == 2 ? "artist name" : "genre") << ": ";
            cin.ignore();
            getline(cin, searchTerm);
            
            switch (choice)
            {
                case 1:
                    SearchByTitle(playlist, searchTerm);
                    break;
                case 2:
                    SearchByArtist(playlist, searchTerm);
                    break;
                case 3:
                    SearchByGenre(playlist, searchTerm);
                    break;
            }
            
            cout << endl << "Press Enter to continue...";
            cin.get();
            system("cls");
            DisplayLoading();
        }
        else if (choice != 0)
        {
            cout << RED_COLOR << "Invalid choice. Try again." << WHITE_COLOR << endl;
        }
        
    } while (choice != 0);
}

// ==================== COPY PLAYLIST ====================
void DuplicatePlaylist(Playlist *&head)
{
	if(!head)
	{
		system("cls");
		DisplayLoading();
		cout << RED_COLOR << "No playlist available to duplicate." << WHITE_COLOR << endl;
		cout << "\nPress Enter to continue...";
		cin.ignore();
		cin.get();
		system("cls");
		DisplayLoading();
		return;
	}

	system("cls");
	DisplayLoading();
		
	cout << "\n==================== DUPLICATE PLAYLIST ====================" << endl;
	Playlist *current = head;
	int index = 1, total = 0;
	
	while(current)
	{
		cout << index << ". " << current->name << endl;
		current = current->next;
		index++;
		total++;
	}
	
	cout << "\nEnter the number of the playlist to duplicate:" << endl;
	int choice = GetValidChoice(1, total);
	
	current = head;
	for (int i=1; i<choice; i++)
		current = current->next;
		
	Playlist *original = current;
	
	string newName = original->name + " Copy";
	AddPlaylist(head, newName);
	
	Playlist *copy = head;
	while(copy && copy->name != newName)
		copy = copy->next;
		
	Song *songPtr = original->songHead;
	while(songPtr)
	{
		AddSongToPlaylist(copy, songPtr->title, songPtr->artist, songPtr->genre);
		songPtr = songPtr->next;
	}
	
	cout << endl << GREEN_COLOR << "Playlist \"" << newName << "\" duplicated suffessfully!" << WHITE_COLOR << endl;
  cout << "\nPress Enter to continue...";
  cin.ignore();
  cin.get();
  system("cls");
  DisplayLoading();	
}