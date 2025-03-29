#include <iostream>
#include <vector>
#include <fstream>

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

Solution getBestSolution(Problem &problem) {
    int n = problem.tasks.size();
    std::vector<int> order(n);
    for (int i = 0; i < n; i++)
    {
        order[i] = i;
    }

    Solution bestSolution;

    do {
        int cmax = getCmax(problem.tasks, order);
        if (cmax < bestSolution.cmax)
        {
            bestSolution.cmax = cmax;
            bestSolution.order = order;
        }
    } while (std::next_permutation(order.begin(), order.end()));

    return bestSolution;
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

Solution heuristicSort(Problem &problem)
{
    std::vector<int> order = initializeOrder(problem.tasks.size());

    std::sort(order.begin(), order.end(), [&](int i, int j) {
        return problem.tasks[i].rj < problem.tasks[j].rj;
  });

    Solution bestSolution;
    bestSolution.order = order;
    bestSolution.cmax = getCmax(problem.tasks, order);

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
    Solution sortSolution = heuristicSort(problem);

    std::cout << "Best cmax with permautation: " << bestSolution.cmax << "\nOrder: ";
    for (int i : bestSolution.order)
    {
        std::cout << i << " ";
    }
    std::cout << std::endl;

    std::cout << "Best cmax with sort: " << sortSolution.cmax << "\nOrder: ";
    for (int i : sortSolution.order)
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
    Solution  sortSolution = heuristicSort(problem);


    std::cout << "Best cmax with permautation: " << bestSolution.cmax << "\nOrder: ";
    for (int i : bestSolution.order)
    {
        std::cout << i << " ";
    }
    std::cout << std::endl;

    std::cout << "Best cmax with sort: " << sortSolution.cmax << "\nOrder: ";
    for (int i : sortSolution.order)
    {
        std::cout << i << " ";
    }
    std::cout << std::endl;
}


int main() {
    testInstance();
    testInstance("data.txt");
    return 0;
}