#include <iostream>
#include <bits/stdc++.h>

using namespace std;

struct Edges_Vertices{
    int i_edge;
    int j_edge;
};

struct Edges{
    int value;
    int spanned;
};

// Invert the given graph
vector<vector<Edges>> invert_array(vector<vector<Edges>> arr) {
    vector<vector<Edges>> inv_arr;

    for (int i = 0; i < arr.size(); ++i) {
        vector<Edges> a;
        for (int j = 0; j < arr.size(); ++j) {
            a.push_back({0,0});
        }
        inv_arr.push_back(a);
    }

    // Inverting the given graph
    for (int i = 0; i < inv_arr.size(); ++i) {
        for (int j = 0; j < inv_arr.size(); ++j) {
            if(arr[i][j].value == 1)
                inv_arr[j][i].value = 1;
                inv_arr[j][i].spanned = arr[i][j].spanned;
        }
    }
    return inv_arr;
}

// Spanning tree of the graph. Traversing the graph on the basis of DFS
vector<Edges_Vertices> dfs(vector<vector<Edges>> inv_arr) {
    int source_node = 0;
    vector<Edges_Vertices> dfs_nodes, stack_node;
    vector<int> visited;
    stack_node.push_back({0,0});

    // Creating an array for storing record of visited nodes. Initially all are 0. Thus, no node is visited.
    for (int i = 0; i < inv_arr.size(); ++i) {
        visited.push_back(0);
    }

    while(stack_node.size() > 0) {
        int cur_node = stack_node.back().j_edge;

        if(visited[cur_node] == 0) {
            visited[cur_node] = 1;
            dfs_nodes.push_back(stack_node.back());
            stack_node.pop_back();
            int flag = 0;
            for (int i = 0; i < inv_arr.size(); ++i) {
                if(inv_arr[cur_node][i].value == 1 && visited[i] == 0) {
                    stack_node.push_back({cur_node,i});
                    flag = 1;
                }
            }
            if(flag == 0 && inv_arr[cur_node][source_node].value == 1)
                dfs_nodes.push_back({cur_node,source_node});
        }
        else {
            stack_node.pop_back();
        }
    }
    return dfs_nodes;
}

// Checking if the given graph is valid for euler tour - in_degree = out_degree for each vertex
bool check_validity(vector<vector<Edges>> arr) {
    for (int i = 0; i < arr.size(); ++i) {
        int in_degree = 0, out_degree = 0;
        for (int j = 0; j < arr.size(); ++j) {
            // Gets the out_degree
            if(arr[i][j].value == 1)
                out_degree++;
            // Gets the In_degree
            else if(arr[j][i].value == 1)
                in_degree++;
        }
        if(in_degree != out_degree)
            return false;
    }
    return true;
}

// Discovering Euler Tour
vector<Edges_Vertices> find_euler_tour(vector<vector<Edges>> arr) {
    vector<Edges_Vertices> tour;

    // Reversing the graph matrix
    vector<vector<Edges>> inv_arr = invert_array(arr);

    // Running DFS algorithm to get the spanning tree in the inverted array
    vector<Edges_Vertices> spanning_tree = dfs(inv_arr);

    // Marking the edges which were discovered.
    for (int i = 1; i < spanning_tree.size(); ++i) {
        inv_arr[spanning_tree[i].i_edge][spanning_tree[i].j_edge].spanned = 1;
    }

    // Invert back the graph matrix to the original one
    arr = invert_array(inv_arr);

    int i = 0, flag = 0;
    bool is_finished = false;
    while(!is_finished) {
        is_finished = true;
        flag = 0;

        //Discovering paths which were not in DFS traversal
        for (int j = 0; j < arr.size(); ++j) {
            if(arr[i][j].value == 1 && arr[i][j].spanned == 0) {
                tour.push_back({i,j});
                arr[i][j].spanned = 2;
                flag = 1;
                i = j;
                is_finished = false;
                break;
            }
        }

        //If no such path exists, we look for the ones which were discovered.
        if(flag == 0) {
            for (int j = 0; j < arr.size(); ++j) {
                if(arr[i][j].value == 1 && arr[i][j].spanned == 1) {
                    tour.push_back({i,j});
                    arr[i][j].spanned = 2;
                    i = j;
                    is_finished = false;
                    break;
                }
            }
        }
    }
    return tour;
}

int main() {
    int num_edges, num_vertices;
    num_edges = 6;
    num_vertices = 5;
    vector<vector<Edges>> arr = {
        {{0,0}, {0,0}, {1,0}, {1,0}, {0,0}, {1,0}, {0,0}, {0,0}},
        {{1,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}},
        {{0,0}, {1,0}, {0,0}, {0,0}, {1,0}, {0,0}, {0,0}, {0,0}},
        {{0,0}, {0,0}, {1,0}, {0,0}, {0,0}, {0,0}, {1,0}, {0,0}},
        {{0,0}, {0,0}, {0,0}, {1,0}, {0,0}, {0,0}, {1,0}, {0,0}},
        {{0,0}, {0,0}, {0,0}, {0,0}, {1,0}, {0,0}, {0,0}, {1,0}},
        {{1,0}, {0,0}, {0,0}, {0,0}, {0,0}, {1,0}, {0,0}, {0,0}},
        {{1,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}}
    };

    bool is_euler = check_validity(arr);
    if(!is_euler) {
        cout<<"Given graph is not euler.";
    }
    else {
        vector<Edges_Vertices> euler_tour_map = find_euler_tour(arr);

        cout<<endl<<"Euler tour is: "<<endl;
        for (int i = 0; i < euler_tour_map.size(); ++i) {
            cout<<euler_tour_map[i].i_edge<<" --> ";
            if(euler_tour_map.size() - i == 1)
                cout<<euler_tour_map[i].j_edge;
        }
    }
    return 0;
}
