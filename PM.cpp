#include <vector>
#include <unordered_map>
#include <iostream>

class Tournament{
    public:
        Tournament(int n, std::vector<std::vector<int>> adj) : size(n), Adjancy_matrix(adj) {}
        bool Hungarian();
        bool match(int i, std::vector<bool> visit);
        bool has_2claw();
        bool is_indomain(int vertex);
        bool is_outdomain(int vertex);
    private:
        int size; //number of vertex in tournament
        std::vector<std::vector<int>> Adjancy_matrix; // adjancy matrix of tournament
        int root; 
        std::unordered_map<int, int> matching;
};

bool Tournament::is_outdomain(int vertex){
    return (vertex != root) && Adjancy_matrix[root][vertex];
}

bool Tournament::is_indomain(int vertex){
    return (vertex != root) && Adjancy_matrix[vertex][root];
}

bool Tournament::match(int i, std::vector<bool> visit)
{
    for (int j = 0; j < size; j++)
        if (is_indomain(j) && Adjancy_matrix[i][j] && !visit[j]) 
        {
            visit[j] = true;                 
            if (matching.find(j) == matching.end() || match(matching[j], visit))
            {
                matching[i] = j;
                return true; 
            }
        }
    return false;
}

bool Tournament::Hungarian()
{
    if(!(size % 2)){ // if number of vertex is even, we need to check whether its out-domain has n/2 vertices
        int outnum = 0;
        for(auto i : Adjancy_matrix[root]){
            if(i) outnum++;
            if(outnum != size / 2) return false; 
        }
    }
    
    int count = 0;
    for (int i = 0; i < size; i++)
    {
        if(!is_outdomain(i)) continue; //if ith vertex isn't out-domain of root, we needn't to search matching
        std::vector<bool> visit(size, false);
        if (match(i, visit)) count++;
    }
    return count == (size - 1) / 2; //judge whether matching member equals to in-domain of root
}

bool Tournament::has_2claw(){
    for(int i = 0; i < size; i++){
        root = i;
        if(Hungarian()) return true;
    }
    return false;
}

int main(){
    Tournament graph1(15, {
	{0,1,1,1,1,1,1,1,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,1,1,1,1,1,1,1},
	{0,1,0,0,1,0,0,0,1,1,1,1,1,0,0},
	{0,1,1,0,0,0,0,0,1,1,1,1,1,0,0},
	{0,1,0,1,0,0,0,0,1,1,1,1,1,0,0},
	{0,1,1,1,1,0,0,1,0,0,0,0,0,1,1},
	{0,1,1,1,1,1,0,0,0,0,0,0,0,1,1},
	{0,1,1,1,1,0,1,0,0,0,0,0,0,1,1},
	{1,0,0,0,0,1,1,1,0,1,1,1,0,0,0},
	{1,0,0,0,0,1,1,1,0,0,1,1,1,0,0},
	{1,0,0,0,0,1,1,1,0,0,0,1,1,1,0},
	{1,0,0,0,0,1,1,1,0,0,0,0,1,1,1},
	{1,0,0,0,0,1,1,1,1,0,0,0,0,1,1},
	{1,0,1,1,1,0,0,0,1,1,0,0,0,0,1},
	{1,0,1,1,1,0,0,0,1,1,1,0,0,0,0}
});
    std::cout << graph1.has_2claw() << std::endl;
}
