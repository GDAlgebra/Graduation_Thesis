#include <vector>
#include <unordered_map>
#include <iostream>
#include <cstdlib>
#include <ctime>

class Tournament {
public:
    Tournament(int n);
    Tournament(int n, std::vector<std::vector<int>> adj) : size(n), Adjancy_matrix(adj) {}
    bool Hungarian();
    bool match(int i, std::vector<bool> visit);
    bool has_2claw();
    bool is_indomain(int vertex);
    bool is_outdomain(int vertex);
    void output();
    
private:
    int size; //number of vertex in tournament
    std::vector<std::vector<int>> Adjancy_matrix; // adjancy matrix of tournament
    int root;
    std::unordered_map<int, int> matching;
};

Tournament::Tournament(int n) {
    size = (n % 2 == 1) ? n : n + 1;
    Adjancy_matrix = std::vector<std::vector<int>>(size, std::vector<int>(size, 0));
    std::vector<int> num1inrow(size, 0), num1incol(size, 0);
    std::srand(std::time(0));
    for (int i = 0; i < size - 1; i++) {
        for (int j = i + 1; j < size; j++) {
            if (num1inrow[i] == (size - 1) / 2 || num1incol[j] == (size - 1) / 2) {
                Adjancy_matrix[i][j] = 0;
            }
            else {
                if (j - num1inrow[i] == (size + 1) / 2 || i - num1incol[j] == (size - 1) / 2)
                    Adjancy_matrix[i][j] = 1;
                else
                    Adjancy_matrix[i][j] = rand() % 2;
            }

            Adjancy_matrix[j][i] = 1 - Adjancy_matrix[i][j];

            if (Adjancy_matrix[i][j]) {
                num1inrow[i]++;
                num1incol[j]++;
            }
            else {
                num1incol[i]++;
                num1inrow[j]++;
            }
            /*
            output();
            std::cout << "number of 1 in row:" << std::endl;
            for(int k = 0; k < size; k++)
                std::cout << num1inrow[k] << " ";
            std::cout << std::endl;
            std::cout << "number of 1 in col:" << std::endl;
            for(int k = 0; k < size; k++)
                std::cout << num1incol[k] << " ";
            std::cout << std::endl;
            */            
        }
    }
    if (n % 2 == 0) {
        size--;
        Adjancy_matrix.pop_back();
        for (int i = 0; i < size; i++) {
            Adjancy_matrix[i].pop_back();
        }
    }
}

void Tournament::output() {
    std::cout << "generated matrix:" << std::endl;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            std::cout << Adjancy_matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }

    
    std::cout << "root:" << root << std::endl;
    std::cout << "matching:" << std::endl;
    for (auto it = matching.begin(); it != matching.end(); it++) {
        std::cout << it->second << " " << it->first << std::endl;
    }
    
}

bool Tournament::is_outdomain(int vertex) {
    return (vertex != root) && Adjancy_matrix[root][vertex];
}

bool Tournament::is_indomain(int vertex) {
    return (vertex != root) && Adjancy_matrix[vertex][root];
}

bool Tournament::match(int i, std::vector<bool> visit)
{
    for (int j = 0; j < size; j++) {
        if (is_indomain(j) && Adjancy_matrix[i][j] && !visit[j])
        {
            visit[j] = true;
            if (matching.find(j) == matching.end() || match(matching[j], visit))
            {
                matching[j] = i;
                return true;
            }
        }
    }
    return false;
}

bool Tournament::Hungarian()
{
    if (!(size % 2)) { // if number of vertex is even, we need to check whether its out-domain has n/2 vertices
        int outnum = 0;
        for (auto i : Adjancy_matrix[root])
            if (i) outnum++;
        if (outnum != size / 2) return false;
    }

    int count = 0;
    for (int i = 0; i < size; i++)
    {
        if (!is_outdomain(i)) continue; //if ith vertex isn't out-domain of root, we needn't to search matching
        std::vector<bool> visit(size, false);
        if (match(i, visit)) count++;
    }
    return count == (size - 1) / 2; //judge whether matching member equals to in-domain of root
}

bool Tournament::has_2claw() {
    for (int i = 0; i < size; i++) {
        root = i;
        if (Hungarian()) {
            output();
            return true;
        }
        output();
    }
    return false;
}
