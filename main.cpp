#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>
#include <random>

#include <algorithm>

 class Task{
public:
     int rj; //czas dostepnosci
    int pj; //czas wykonania

    int qj; //czas stygniecia
    Task(int r, int p, int q) : rj(r), pj(p), qj(q) {

    }
};

class Problem {
public:
    std::vector<Task> tasks;

    void addTask(int r, int p, int q) {
        tasks.emplace_back(r, p, q);  //tworzy obiekt Task bezpośrednio w wektorze, unikając dodatkowej kopii.
    }
};

class Solution{
public:
    std::vector<int> order;
    int cmax;
    Solution() : cmax(INT_MAX) {

    }
};

int getCmax(const std::vector<Task> &tasks, const std::vector<int> &order) {
    int cmax = 0;
    int currentTime = 0;

    for (int i : order) {
        const Task &task = tasks[i];
        currentTime = std::max(currentTime, task.rj) + task.pj;
        cmax = std::max(cmax, currentTime + task.qj);
    }
    return cmax;
}

std::vector<int> initializeOrder(int n)
{
    std::vector<int> order(n);
    for (int i = 0; i < n; i++)
    {
        order[i] = i;
    }
    return order;
}

Solution getBestSolution(Problem &problem) {

    std::vector<int> order = initializeOrder(problem.tasks.size());
    Solution bestSolution;
    auto start = std::chrono::high_resolution_clock::now();

    do {
        int cmax = getCmax(problem.tasks, order);
        if (cmax < bestSolution.cmax)
        {
            bestSolution.cmax = cmax;
            bestSolution.order = order;
        }
    } while (std::next_permutation(order.begin(), order.end()));

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "getBestSolution time: " << elapsed.count() << " seconds\n";

    return bestSolution;
}



Solution heuristicSort(Problem &problem, const std::string& sortBy) {
    std::vector<int> order = initializeOrder(problem.tasks.size());
    auto start = std::chrono::high_resolution_clock::now();

    if (sortBy == "rj")
    {
        std::sort(order.begin(), order.end(), [&](int i, int j) {
            return problem.tasks[i].rj < problem.tasks[j].rj;
        });
    }
    else if (sortBy == "qj") {
        std::sort(order.begin(), order.end(), [&](int i, int j) {
            return problem.tasks[i].qj > problem.tasks[j].qj;
        });
    }

    Solution bestSolution;
    bestSolution.order = order;
    bestSolution.cmax = getCmax(problem.tasks, order);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "heuristicSort by " << sortBy << " time: " << elapsed.count() << " seconds\n";

    return bestSolution;
}

Solution hybridSort(Problem &problem) {
    std::vector<int> order = initializeOrder(problem.tasks.size());
    int n =problem.tasks.size();

    auto start = std::chrono::high_resolution_clock::now();




    std::sort(order.begin(), order.end(), [&](int i, int j) {
        return problem.tasks[i].rj < problem.tasks[j].rj;
    });

    std::vector<int> finalOrder;
    int currentTime = 0;
    std::vector<int> available;
    size_t index = 0;

    while (finalOrder.size() < n) {
        // Dodajemy dostępne zadania
        while (index < n && problem.tasks[order[index]].rj <= currentTime) {
            available.push_back(order[index]);
            index++;
        }

        if (!available.empty()) {
            // Wybieramy zadanie o największym qj
            auto max_qj_it = std::max_element(available.begin(), available.end(), [&](int a, int b) {
                return problem.tasks[a].qj < problem.tasks[b].qj;
            });

            int selected = *max_qj_it;
            available.erase(max_qj_it);
            finalOrder.push_back(selected);
            currentTime = std::max(currentTime, problem.tasks[selected].rj) + problem.tasks[selected].pj;
        } else {
            // Jeśli nie ma dostępnych zadań, przesuwamy czas
            currentTime = problem.tasks[order[index]].rj;
        }
    }

    Solution bestSolution;
    bestSolution.order = finalOrder;
    bestSolution.cmax = getCmax(problem.tasks, finalOrder);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "hybridSort time: " << elapsed.count() << " seconds\n";
    return bestSolution;
}





void testInstance() {
    Problem problem;
    problem.addTask(1, 5, 9);
    problem.addTask(4, 5, 4);
    problem.addTask(1, 4, 6);
    problem.addTask(7, 3, 3);
    problem.addTask(3, 6, 8);
    problem.addTask(4, 7,1);

    Solution bestSolution = getBestSolution(problem);
    Solution sortSolutionRj = heuristicSort(problem, "rj");
    Solution sortSolutionQj = heuristicSort(problem, "qj");
    Solution mySolution= hybridSort(problem);

    std::cout << "Best cmax with permautation: " << bestSolution.cmax << "\nOrder: ";
    for (int i : bestSolution.order)
    {
        std::cout << i << " ";
    }
    std::cout << std::endl;

    std::cout << "Best cmax with sort by rj: " << sortSolutionRj.cmax << "\nOrder: ";
    for (int i : sortSolutionRj.order)
    {
        std::cout << i << " ";
    }
    std::cout << std::endl;

    std::cout << "Best cmax with sort by qj: " << sortSolutionQj.cmax << "\nOrder: ";
    for (int i : sortSolutionQj.order)
    {
        std::cout << i << " ";
    }
    std::cout << std::endl;

    std::cout << "Best cmax with own algorithm: " << sortSolutionQj.cmax << "\nOrder: ";
    for (int i : mySolution.order)
    {
        std::cout << i << " ";
    }
    std::cout << std::endl;
}

void testInstance(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Cannot open file\n";
        return;
    }

    Problem problem;
    int n;
    file >> n;
    for (int i = 0; i < n; i++)
    {
        int r, p, q;
        file >> r >> p >> q;
        problem.addTask(r, p, q);
    }

    Solution bestSolution = getBestSolution(problem);
    Solution  sortSolutionRj = heuristicSort(problem, "rj");
    Solution  sortSolutionQj = heuristicSort(problem, "qj");
    Solution mySolution = hybridSort(problem);


    std::cout << "Best cmax with permautation: " << bestSolution.cmax << "\nOrder: ";
    for (int i : bestSolution.order)
    {
        std::cout << i << " ";
    }
    std::cout << std::endl;

    std::cout << "Best cmax with sort: " << sortSolutionRj.cmax << "\nOrder: ";
    for (int i : sortSolutionRj.order)
    {
        std::cout << i << " ";
    }
    std::cout << std::endl;


    std::cout << "Best cmax with sort: " << sortSolutionQj.cmax << "\nOrder: ";
    for (int i : sortSolutionQj.order)
    {
        std::cout << i << " ";
    }
    std::cout << std::endl;

    std::cout << "Best cmax with own algorithm: " << sortSolutionQj.cmax << "\nOrder: ";
    for (int i : mySolution.order)
    {
        std::cout << i << " ";
    }
    std::cout << std::endl;
}


int main() {
    testInstance();
    //testInstance("data.txt");
    return 0;
}