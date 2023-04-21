#include <iostream>
#include <string>
#include <map>
#include <list>
#include <set>

class AudioTrack {
public:
    AudioTrack(const std::string& title, const std::string& artist, const std::string& album, int track_number, int duration)
        : title_(title), artist_(artist), album_(album), track_number_(track_number), duration_(duration) {}

    const std::string& getTitle() const { return title_; }
    const std::string& getArtist() const { return artist_; }
    const std::string& getAlbum() const { return album_; }
    int getTrackNumber() const { return track_number_; }
    int getDuration() const { return duration_; }

    void setTitle(const std::string& title) { title_ = title; }
    void setArtist(const std::string& artist) { artist_ = artist; }
    void setAlbum(const std::string& album) { album_ = album; }
    void setTrackNumber(int track_number) { track_number_ = track_number; }
    void setDuration(int duration) { duration_ = duration; }

private:
    std::string title_;
    std::string artist_;
    std::string album_;
    int track_number_;
    int duration_; // Duration in seconds
};

class TrackNode {
public:
    // Constructor that initializes the TrackNode with an AudioTrack
    explicit TrackNode(const AudioTrack& track) : track_(track) {}

    // Getter for the AudioTrack held by this TrackNode
    const AudioTrack& getTrack() const { return track_; }

    // Overloading the '<' operator to compare TrackNode objects based on the title of their AudioTracks
    bool operator<(const TrackNode& other) const { return track_.getTitle() < other.track_.getTitle(); }

private:
    AudioTrack track_; // Holds the AudioTrack object
};

class TrackLibrary {
public:
    // Function to add an AudioTrack to the library
    void addTrack(const AudioTrack& track) {
        TrackNode node(track); // Create a TrackNode from the given AudioTrack
        const std::string& artist = track.getArtist(); // Get the artist name from the AudioTrack

        // If the artist is not already in the hashMap, create a new list for their tracks
        hashMap[artist].push_back(node); // Add the TrackNode to the list of tracks for this artist
        balancedBST.insert(node); // Insert the TrackNode into the balanced binary search tree
    }

    // Function to remove an AudioTrack from the library
    void removeTrack(const AudioTrack& track) {
        const std::string& artist = track.getArtist(); // Get the artist name from the AudioTrack
        auto it = hashMap.find(artist); // Find the artist in the hashMap

        // If the artist is found in the hashMap
        if (it != hashMap.end()) {
            // Find the corresponding TrackNode in the list of tracks for this artist
            auto node_it = findNodeInList(it->second, track);

            // If the TrackNode is found
            if (node_it != it->second.end()) {
                balancedBST.erase(*node_it); // Remove the TrackNode from the balanced binary search tree
                it->second.erase(node_it); // Remove the TrackNode from the list of tracks for this artist
            }
        }
    }

    // Function to search for tracks by a given artist
    std::list<TrackNode> searchByArtist(const std::string& artist) {
        auto it = hashMap.find(artist); // Find the artist in the hashMap

        // If the artist is found, return the list of their tracks; otherwise, return an empty list
        if (it != hashMap.end()) {
            return it->second;
        } else {
            return std::list<TrackNode>();
        }
    }

private:
    // HashMap to store a list of TrackNodes for each artist
    std::map<std::string, std::list<TrackNode>> hashMap;

    // Balanced binary search tree to store the TrackNodes in a sorted manner
    std::set<TrackNode> balancedBST;

    // Function to find a TrackNode with the given AudioTrack in the given list
    std::list<TrackNode>::iterator findNodeInList(std::list<TrackNode>& nodeList, const AudioTrack& track) {
        // Iterate through the nodeList
        for (auto it = nodeList.begin(); it != nodeList.end(); ++it) {
            // If the title of the AudioTrack of the current TrackNode matches the title of the given AudioTrack
            if (it->getTrack().getTitle() == track.getTitle()) {
                return it; // Return the iterator pointing to the matching TrackNode
            }
        }
    return nodeList.end();
    }
};

int main() {
    // Example usage
    TrackLibrary library;
    AudioTrack track1("Bohemian Rhapsody", "Queen", "A Night at the Opera", 1, 354);
    AudioTrack track2("We Will Rock You", "Queen", "News of the World", 2, 121);

    library.addTrack(track1);
    library.addTrack(track2);

    std::list<TrackNode> tracks_by_queen = library.searchByArtist("Queen");
    std::cout << "Tracks by Queen:" << std::endl;
    for (const auto& node : tracks_by_queen) {
        std::cout << node.getTrack().getTitle() << std::endl;
    }

    library.removeTrack(track1);
    tracks_by_queen = library.searchByArtist("Queen");
    std::cout << "Tracks by Queen after removing Bohemian Rhapsody:" << std::endl;
    for (const auto& node : tracks_by_queen) {
        std::cout << node.getTrack().getTitle() << std::endl;
    }

    return 0;
}
