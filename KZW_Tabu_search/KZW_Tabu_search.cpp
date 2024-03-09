#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <limits>

using namespace std;

// Struktura reprezentująca pojedyncze zadanie
struct Task {
    int r; // Czas przygotowania
    int p; // Czas przetwarzania
    int q; // Czas dostarczenia
};

// Funkcja oceny - oblicza czas wykonywania wszystkich zadań
int evaluateSolution(const vector<int>& solution, const vector<Task>& tasks) {
    int totalProcessingTime = 0;
    int currentTime = 0;
    for (int i = 0; i < solution.size(); ++i) {
        currentTime = max(tasks[solution[i]].r, currentTime) + tasks[solution[i]].p;
        totalProcessingTime = max(currentTime + tasks[solution[i]].q, totalProcessingTime);
    }
    return totalProcessingTime;
}

// Generuje losowe rozwiązanie
vector<int> generateRandomSolution(int size) {
    vector<int> solution(size);
    for (int i = 0; i < size; ++i) {
        solution[i] = i;
    }
    random_shuffle(solution.begin(), solution.end());
    return solution;
}

int main() {
    // Wczytanie danych
    vector<Task> tasks;
    Task task;
    ifstream file("data.4.txt");
    while (file >> task.r >> task.p >> task.q) {
        tasks.push_back(task);
    }

    // Parametry algorytmu Tabu Search
    const int maxIterations = 100;
    const int tabuListSize = 15;

    // Inicjalizacja listy tabu
    vector<vector<int>> tabuList;

    // Początkowe rozwiązanie
    vector<int> bestSolution = generateRandomSolution(tasks.size());
    int bestScore = evaluateSolution(bestSolution, tasks);

    // Główna pętla algorytmu Tabu Search
    for (int iter = 0; iter < maxIterations; ++iter) {
        // Generowanie sąsiadów bieżącego rozwiązania
        vector<vector<int>> neighbors;
        for (int i = 0; i < tasks.size(); ++i) {
            for (int j = i + 1; j < tasks.size(); ++j) {
                vector<int> neighbor = bestSolution;
                swap(neighbor[i], neighbor[j]);
                neighbors.push_back(neighbor);
            }
        }

        // Ocena sąsiadów i wybór najlepszego rozwiązania
        vector<int> bestNeighbor;
        int bestNeighborScore = numeric_limits<int>::max();
        for (const auto& neighbor : neighbors) {
            int neighborScore = evaluateSolution(neighbor, tasks);
            if (neighborScore < bestNeighborScore &&
                find(tabuList.begin(), tabuList.end(), neighbor) == tabuList.end()) {
                bestNeighbor = neighbor;
                bestNeighborScore = neighborScore;
            }
        }

        // Aktualizacja najlepszego rozwiązania
        if (bestNeighborScore < bestScore) {
            bestSolution = bestNeighbor;
            bestScore = bestNeighborScore;
        }

        // Aktualizacja listy tabu
        tabuList.push_back(bestNeighbor);
        if (tabuList.size() > tabuListSize) {
            tabuList.erase(tabuList.begin());
        }
    }
    // Wyświetlenie wczytanych danych
    for (const auto& task : tasks) {
        cout << "r: " << task.r << ", p: " << task.p << ", q: " << task.q << endl;
    }
    // Wyświetlenie najlepszego rozwiązania
    cout << "Najlepsze rozwiazanie: ";
    for (int i = 0; i < bestSolution.size(); ++i) {
        cout << bestSolution[i]+1 << " ";
    }



    cout << endl;
    cout << "Wartosc funkcji celu: " << bestScore << endl;

    return 0;
}
