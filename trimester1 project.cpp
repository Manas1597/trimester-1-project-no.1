#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <cmath>
#include <algorithm>
#include <filesystem>

namespace fs = std::filesystem;

const int TOTAL_BOOKS = 64;
const int TOP_PAIRS = 10;

// Function to read file and build a word frequency map
std::unordered_map<std::string, int> getWordFrequency(const std::string &filename) {
    std::unordered_map<std::string, int> wordFreq;
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Could not open file " << filename << "\n";
        return wordFreq;
    }
    std::string word;
    while (file >> word) {
        wordFreq[word]++;
    }
    return wordFreq;
}

// Function to compute cosine similarity between two frequency maps
double cosineSimilarity(const std::unordered_map<std::string, int> &freq1,
                        const std::unordered_map<std::string, int> &freq2) {
    double dotProduct = 0, magnitude1 = 0, magnitude2 = 0;
    
    // Calculate dot product
    for (const auto &pair : freq1) {
        auto it = freq2.find(pair.first);
        if (it != freq2.end()) {
            dotProduct += pair.second * it->second;
        }
    }

    // Calculate magnitudes
    for (const auto &pair : freq1) {
        magnitude1 += pair.second * pair.second;
    }
    for (const auto &pair : freq2) {
        magnitude2 += pair.second * pair.second;
    }
    
    if (magnitude1 == 0 || magnitude2 == 0) return 0.0;
    return dotProduct / (std::sqrt(magnitude1) * std::sqrt(magnitude2));
}

// Struct to store similarity result
struct SimilarityResult {
    int book1;
    int book2;
    double similarity;
    
    bool operator<(const SimilarityResult &other) const {
        return similarity > other.similarity;
    }
};

int main() {
    // Load word frequencies for each book
    std::vector<std::unordered_map<std::string, int>> bookFrequencies;
    for (int i = 1; i <= TOTAL_BOOKS; ++i) {
        std::string filename = "book" + std::to_string(i) + ".txt";
        bookFrequencies.push_back(getWordFrequency(filename));
    }

    // Calculate similarity between each pair of books
    std::vector<SimilarityResult> similarities;
    for (int i = 0; i < TOTAL_BOOKS; ++i) {
        for (int j = i + 1; j < TOTAL_BOOKS; ++j) {
            double similarity = cosineSimilarity(bookFrequencies[i], bookFrequencies[j]);
            similarities.push_back({i + 1, j + 1, similarity});
        }
    }

    // Sort pairs by similarity in descending order
    std::sort(similarities.begin(), similarities.end());

    // Output the top 10 most similar pairs
    std::cout << "Top " << TOP_PAIRS << " similar pairs of books:\n";
    for (int i = 0; i < TOP_PAIRS && i < similarities.size(); ++i) {
        std::cout << "Book " << similarities[i].book1 << " and Book " 
                  << similarities[i].book2 << " - Similarity: " 
                  << similarities[i].similarity << "\n";
    }

    return 0;
}