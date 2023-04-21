#include <iostream>
#include <string>
#include <map>
#include <list>
#include <set>
#include <algorithm>
#include <fstream>
#include <sstream>
#include "music.h"


AudioTrack::AudioTrack(const std::string& title, const std::string& artist, int duration) {
  this->title = title;
  this->artist = artist;
  this->duration = duration;
}

const std::string& AudioTrack::getTitle() const { return title; }
const std::string& AudioTrack::getArtist() const { return artist; }
int AudioTrack::getDuration() const { return duration; }

void AudioTrack::setTitle(const std::string& title) { this->title = title; }
void AudioTrack::setArtist(const std::string& artist) { this->artist = artist; }
void AudioTrack::setDuration(int duration) { this->duration = duration; }

// Constructor that initializes the TrackNode with an AudioTrack
TrackNode::TrackNode(const AudioTrack& track) : track(track) {}

const AudioTrack& TrackNode::getTrack() const { return track; }

bool TrackNode::operator<(const TrackNode& other) const { return track.getTitle() < other.track.getTitle(); }

    // Function to add an AudioTrack to the library
void TrackLibrary::addTrack(const AudioTrack& track) {
        TrackNode node(track); // Create a TrackNode from the given AudioTrack
        const std::string& artist = track.getArtist(); // Get the artist name from the AudioTrack
	
        // If the artist is not already in the hashMap, create a new list for their tracks
        hashMap[artist].push_back(node); // Add the TrackNode to the list of tracks for this artist
        balancedBST.insert(node); // Insert the TrackNode into the balanced binary search tree
    }
  
  // Function to remove an AudioTrack from the library
void TrackLibrary::removeTrack(const AudioTrack& track) {
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
bool TrackLibrary::removeTrackByTitle(const std::string& title) {
    auto it = std::find_if(balancedBST.begin(), balancedBST.end(),
                           [&title](const TrackNode& node) {
                               return node.getTrack().getTitle() == title;
                           });
    
    if (it != balancedBST.end()) {
      removeTrack(it->getTrack());
      return true;
    }
    
    return false;
}
// Add this function to the TrackLibrary class implementation in music.cpp
std::vector<AudioTrack> TrackLibrary::searchByArtist(const std::string& artist) {
    std::vector<AudioTrack> tracksByArtist;
    for (const TrackNode& trackNode : balancedBST) {
        const AudioTrack& track = trackNode.getTrack();
        if (track.getArtist() == artist) {
            tracksByArtist.push_back(track);
        }
    }
    return tracksByArtist;
}
void TrackLibrary::saveLibraryToFile(const std::string& filename) {
    std::ofstream file(filename);
    
    if (file.is_open()) {
        for (const TrackNode& trackNode : balancedBST) {
            const AudioTrack& track = trackNode.getTrack();
            file << track.getTitle() << "," << track.getArtist() << "," << track.getDuration() << std::endl;
        }
        file.close();
    } else {
        std::cerr << "Unable to open file: " << filename << std::endl;
    }
}
    // Function to search for a track by a given title using the balanced binary search tree
const AudioTrack* TrackLibrary::searchByTitle(const std::string& title) {
        // Create a dummy AudioTrack with the given title
        AudioTrack dummy_track(title, "", 0);

        // Create a dummy TrackNode with the dummy AudioTrack
        TrackNode dummy_node(dummy_track);

        // Search the balanced binary search tree for the dummy TrackNode
        auto it = balancedBST.find(dummy_node);

        // If the TrackNode is found, return a pointer to its AudioTrack
        if (it != balancedBST.end()) {
            return &(it->getTrack());
        }

        // If the TrackNode is not found, return a nullptr
        return nullptr;
    }

    // Helper function to find a TrackNode in a list of TrackNodes
std::list<TrackNode>::iterator TrackLibrary::findNodeInList(std::list<TrackNode>& nodeList, const AudioTrack& track) {
        for (auto it = nodeList.begin(); it != nodeList.end(); ++it) {
            if (it->getTrack().getTitle() == track.getTitle()) {
                return it;
            }
        }

        // If the TrackNode is not found, return an iterator to the end of the list
        return nodeList.end();
    }

    std::map<std::string, std::list<TrackNode>> hashMap; // HashMap where key: artist, value: list of TrackNodes
    std::set<TrackNode> balancedBST; // Balanced binary search tree to store TrackNodes

