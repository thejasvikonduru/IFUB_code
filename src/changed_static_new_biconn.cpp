#include <bits/stdc++.h>
// #include <changed_static.h>
#include <ctime>
#include <biconn_helper.h>
#include <boost/graph/biconnected_components.hpp>
#include <boost/graph/adjacency_list.hpp>
typedef long long int ll;
using namespace std;
class Biconn_algo : public Diameter_static
{
public:
    graph_t read_file_boost(graph_t g, Biconn_ops g1)
    {
        unordered_map<ll, vector<ll>> adj_list = g1.adj_list;
        for (auto it = adj_list.begin(); it != adj_list.end(); it++)
        {
            vector<ll> adj_vertices = it->second;
            for (ll i = 0; i < adj_vertices.size(); i++)
            {
                add_edge(it->first, adj_vertices[i], g);
            }
        }
        return g;
    }
    Biconn_algo read_file(Biconn_algo g, string filename, int choice)
    {
        ifstream file;
        file.open(filename);
        if (file.is_open())
        {
            string line, vertex1, vertex2;
            while (getline(file, line))
            {
                for (ll i = 0; i < line.length(); i++)
                {
                    if (line[i] == ' ')
                    {
                        vertex1 = line.substr(0, i);
                        vertex2 = line.substr(i + 1, line.length() - (i + 1));
                        break;
                    }
                }
                ll v1 = stoi(vertex1);
                ll v2 = stoi(vertex2);
                if (choice == 0)
                    g.add_Edge(v1, v2);
            }
        }
        file.close();
        return g;
    }
    Biconn_algo extract_largest_CC(Biconn_algo g)
    {
        ll num_vert = g.vertices.size();
        vector<ll> curr_visited(num_vert, 0);
        ll comp_count = 0;
        ll maxi_index = 0;
        ll maxi_num_vert = INT_MIN;
        vector<vector<ll>> connect_comp_vert;
        vector<ll> curr_vert;
        for (ll i = 0; i < num_vert; i++)
        {
            if (curr_visited[g.vertices[i]] == 0 && comp_count == 0)
            {
                g.BFS(g.vertices[i], 2);
                curr_vert = g.curr_vertices;
                connect_comp_vert.push_back(curr_vert);
                curr_visited = g.visited;
                if (curr_vert.size() > maxi_num_vert)
                {
                    maxi_num_vert = curr_vert.size();
                    maxi_index = comp_count;
                }
                comp_count++;
                curr_vert.clear();
            }
            else if (curr_visited[g.vertices[i]] == 0 && comp_count != 0)
            {
                g.BFS(g.vertices[i], 1);
                curr_vert = g.curr_vertices;
                connect_comp_vert.push_back(curr_vert);
                curr_visited = g.visited;
                if (curr_vert.size() > maxi_num_vert)
                {
                    maxi_num_vert = curr_vert.size();
                    maxi_index = comp_count;
                }
                comp_count++;
                curr_vert.clear();
            }
        }
        unordered_map<ll, ll> visited_comp_vert;
        Biconn_algo max_conn_comp;
        vector<ll> maxi_num_vert_comp = connect_comp_vert[maxi_index];
        ll max_conn_edges = 0;
        unordered_map<ll, ll> visited_vert;
        for (ll tt = 0; tt < maxi_num_vert_comp.size(); tt++)
        {
            vector<ll> vert_adj_list = g.adj_list[maxi_num_vert_comp[tt]];
            visited_vert[maxi_num_vert_comp[tt]] = 1;
            for (ll ss = 0; ss < vert_adj_list.size(); ss++)
            {
                if (visited_vert.find(vert_adj_list[ss]) == visited_vert.end())
                {
                    max_conn_edges += 1;
                    max_conn_comp.add_Edge(vert_adj_list[ss], maxi_num_vert_comp[tt]);
                }
            }
        }
        return max_conn_comp;
    }
    ll diam_calc_Old_algo(Biconn_algo g)
    {
        ll start_vertex, source, diameter;
        start_vertex = g.four_sweep_start_vertex(1);
        pair<ll, ll> four_sweep_info = g.four_sweep(start_vertex);
        source = four_sweep_info.first;
        ll info = g.ifub(source, four_sweep_info.second, 0, 0);
        return info;
    }
};

graph_t read_file_boost(graph_t g, Biconn_ops g1)
{
    unordered_map<ll, vector<ll>> adj_list = g1.adj_list;
    for (auto it = adj_list.begin(); it != adj_list.end(); it++)
    {
        vector<ll> adj_vertices = it->second;
        for (ll i = 0; i < adj_vertices.size(); i++)
        {
            add_edge(it->first, adj_vertices[i], g);
        }
    }
    return g;
}

void run_old_algo(string filename)
{
    Biconn_algo g;
    g = g.read_file(g, filename, 0);
    Biconn_algo g_temp;
    g_temp = g.extract_largest_CC(g);
    clock_t begin, end;
    double time_taken1, time_taken2;
    begin = clock();
    ll old_graph_old_algo_info = g_temp.diam_calc_Old_algo(g_temp);
    end = clock();
    time_taken1 = double(end - begin) / CLOCKS_PER_SEC;
    cout << "old:" << old_graph_old_algo_info << " time_taken:" << time_taken1 << endl;
}
void run_new_algo(string filename)
{
    clock_t begin, end;
    double time_taken1;
    Biconn_ops bicc, bicc_temp;
    bicc = bicc.read_file(bicc, filename, 0);
    bicc_temp = bicc.extract_largest_CC(bicc);
    Biconn_ops g = bicc_temp;
    ll maxl = 0;
    for (ll i = 0; i < g.vertices.size(); i++)
    {
        if (g.vertices[i] > maxl)
            maxl = g.vertices[i];
    }
    ll V = maxl + 1;
    graph_t boost_g(V);
    boost_g = read_file_boost(boost_g, g);
    begin = clock();
    ll diam = 0;
    ll start_vertex = g.four_sweep_start_vertex(1);
    pair<ll, ll> four_sweep_info = g.four_sweep(start_vertex);
    ll source = four_sweep_info.first;
    diam = g.ifub_new(filename, source, four_sweep_info.second, &g, maxl, &boost_g);
    end = clock();
    time_taken1 = double(end - begin) / CLOCKS_PER_SEC;
    cout << "new:" << diam << " time_taken:" << time_taken1 << endl;
    cout << "-------------------------------------------" << endl;
}

int main()
{
    string prefix = "all_test_cases";
    vector<string> data_vec;
    ifstream file1;
    file1.open("all_test_cases/datasets_order.txt");
    if (file1.is_open())
    {
        string line;
        ll count99 = 0;
        while (getline(file1, line))
        {
            data_vec.push_back(line);
        }
    }
    file1.close();
    vector<ll> data_veccc = {17};
    for (ll i = 0; i < data_veccc.size(); i++)
    {
        run_old_algo(data_vec[data_veccc[i]]);
        run_new_algo(data_vec[data_veccc[i]]);
    }
}
/*Add the conditions for insertion and deletion afterwards*/
