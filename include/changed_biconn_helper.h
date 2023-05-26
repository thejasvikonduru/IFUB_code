#include <bits/stdc++.h>
#include <changed_static.h>
#include <ctime>
#ifndef BICONN_HELPER_H
#define BICONN_HELPER_H
typedef long long int ll;
#include <boost/graph/biconnected_components.hpp>
#include <boost/graph/adjacency_list.hpp>
using namespace std;
class Biconn_ops : public Diameter_static
{
public:
    unordered_map<ll, vector<ll>> biconn_artics;
    unordered_map<string, ll> disartics;
    unordered_map<string, ll> art_eccs;
    Diameter_static block_cut;
    vector<ll> all_artics;
    unordered_map<string, ll> art_child_eccs;
    unordered_map<ll, ll> vertex_bcc;
    unordered_map<ll, ll> bcc_visited;

    // Biconn_ops read_file(Biconn_ops g,string filename,int choice)
    // {
    //     ifstream file;
    //     file.open(filename);
    //     if(file.is_open())
    //     {
    //         string line,vertex1,vertex2;
    //         while (getline(file, line))
    //         {
    //             for(ll i=0;i<line.length();i++)
    //             {
    //                 if(line[i]==' ')
    //                 {
    //                     vertex1=line.substr(0,i);
    //                     vertex2=line.substr(i+1,line.length()-(i+1));
    //                     break;
    //                 }
    //             }
    //             ll v1=stoi(vertex1);
    //             ll v2=stoi(vertex2);
    //             if(choice==0)
    //                 g.add_Edge(v1,v2);
    //         }
    //     }
    //     file.close();
    //     return g;
    // }
    // Biconn_ops extract_largest_CC(Biconn_ops g)
    // {
    //     ll num_vert=g.vertices.size();
    //     vector<ll> curr_visited(num_vert,0);
    //     vector<Biconn_ops*> connect_comp_vert(num_vert);
    //     ll comp_count=0;
    //     for(ll i=0;i<num_vert;i++)
    //     {
    //         if(curr_visited[g.vertices[i]]==0 && comp_count==0)
    //             g.BFS(g.vertices[i],0);
    //         else if(curr_visited[g.vertices[i]]==0 && comp_count!=0)
    //             g.BFS(g.vertices[i],1);
    //         vector<ll> curr_vert=g.curr_vertices;
    //         curr_visited=g.visited;
    //         connect_comp_vert[comp_count]=new Biconn_ops();
    //         for(ll tt=0;tt<curr_vert.size();tt++)
    //         {
    //             vector<ll> vert_adj_list=g.adj_list[curr_vert[tt]];
    //             for(ll ss=0;ss<vert_adj_list.size();ss++)
    //             {
    //                 (*connect_comp_vert[comp_count]).add_Edge(vert_adj_list[ss],curr_vert[tt]);
    //             }
    //         }
    //         comp_count++;
    //     }
    //     Biconn_ops max_conn_comp;
    //     ll maxiii=0;
    //     for(ll hh=0;hh<connect_comp_vert.size();hh++)
    //     {
    //         if((*connect_comp_vert[hh]).vertices.size()>maxiii)
    //         {
    //             maxiii=(*connect_comp_vert[hh]).vertices.size();
    //             max_conn_comp=(*connect_comp_vert[hh]);
    //         }
    //     }
    //     return max_conn_comp;
    // }
    ll bct_traversal_art_point(vector<Diameter_static *> &bcc_graphs, ll art, unordered_map<ll, ll> &block_visited, unordered_map<string, ll> changed_bicc, unordered_map<ll, ll> vertex_bicc_ecc)
    {
        ll ecc = INT_MIN;
        block_visited[art] = 1;
        vector<ll> bicc_vertices = block_cut.adj_list[art];
        for (ll i = 0; i < bicc_vertices.size(); i++)
        {
            ll bicc_vertex = bicc_vertices[i];
            ll actual_bicc_vertex = -bicc_vertex - 1;
            if (block_visited.find(bicc_vertex) == block_visited.end())
            {
                if (art_child_eccs.find(to_string(art) + "#" + to_string(actual_bicc_vertex)) == art_child_eccs.end())
                {
                    ll child_ecc;
                    Diameter_static *curr_gr;
                    if (art_eccs.find(to_string(art) + '#' + to_string(-bicc_vertex - 1)) == art_eccs.end())
                    {
                        // extra_calls+=1;
                        curr_gr = bcc_graphs[actual_bicc_vertex];
                        (*curr_gr).BFS(art, 0);
                        child_ecc = (*curr_gr).max_level;
                        art_eccs[to_string(art) + '#' + to_string(-bicc_vertex - 1)] = child_ecc;
                    }
                    else
                    {
                        child_ecc = art_eccs[to_string(art) + '#' + to_string(-bicc_vertex - 1)];
                    }
                    block_visited[bicc_vertex] = 1;
                    for (ll j = 0; j < block_cut.adj_list[bicc_vertex].size(); j++)
                    {
                        if (block_visited.find(block_cut.adj_list[bicc_vertex][j]) == block_visited.end())
                        {
                            string ecc_pair;
                            if (art < block_cut.adj_list[bicc_vertex][j])
                                ecc_pair = to_string(art) + "#" + to_string(block_cut.adj_list[bicc_vertex][j]);
                            else
                                ecc_pair = to_string(block_cut.adj_list[bicc_vertex][j]) + "#" + to_string(art);
                            if (dist_artics.find(ecc_pair) == dist_artics.end())
                            {
                                dist_artics[ecc_pair] = (*curr_gr).level[block_cut.adj_list[bicc_vertex][j]];
                            }
                            else
                            {
                                child_ecc = max(child_ecc, dist_artics[ecc_pair] + bct_traversal_art_point(bcc_graphs, block_cut.adj_list[bicc_vertex][j], block_visited, changed_bicc, vertex_bicc_ecc));
                            }
                        }
                    }
                    art_child_eccs[to_string(art) + "#" + to_string(actual_bicc_vertex)] = child_ecc;
                    ecc = max(ecc, child_ecc);
                }
                else
                {
                    block_visited[bicc_vertex] = 1;
                    ll child_ecc = art_child_eccs[to_string(art) + "#" + to_string(actual_bicc_vertex)];
                    ecc = max(ecc, child_ecc);
                }
            }
        }
        return ecc;
    }
    ll bct_traversal_biconn_point(vector<Diameter_static *> &bcc_graphs, Diameter_static *gr, ll biconn, ll vert, unordered_map<ll, ll> &block_visited, unordered_map<string, ll> changed_bicc, unordered_map<ll, ll> vertex_bicc_ecc)
    {
        ll ecc = INT_MIN;
        ll block_biconn = -(biconn + 1);
        block_visited[block_biconn] = 1;
        for (ll i = 0; i < block_cut.adj_list[block_biconn].size(); i++)
        {
            ll vert_art_dist = ((*gr).level)[block_cut.adj_list[block_biconn][i]];
            ll curr_ecc = vert_art_dist + bct_traversal_art_point(bcc_graphs, block_cut.adj_list[block_biconn][i], block_visited, changed_bicc, vertex_bicc_ecc);
            ecc = max(ecc, curr_ecc);
        }
        return ecc;
    }
    void create_req_data_strucs(unordered_map<ll, ll> &artics)
    {
        unordered_map<ll, ll> artics_copy;
        for (ll i = 0; i < biconns.size(); i++)
        {
            artics_copy.clear();
            if (biconns[i].size() != 0)
            {
                for (ll j = 0; j < biconns[i].size(); j++)
                {
                    if (artics.find(biconns[i][j].first) != artics.end() && artics_copy.find(biconns[i][j].first) == artics_copy.end())
                    {
                        biconn_artics[i].push_back(biconns[i][j].first);
                        artics_copy[biconns[i][j].first] = 1;
                    }
                    if (artics.find(biconns[i][j].second) != artics.end() && artics_copy.find(biconns[i][j].second) == artics_copy.end())
                    {
                        biconn_artics[i].push_back(biconns[i][j].second);
                        artics_copy[biconns[i][j].second] = 1;
                    }
                }
            }
        }
    }
    Diameter_static create_block_cut_tree()
    {
        for (ll i = 0; i < biconns.size(); i++)
        {
            if (biconns[i].size() != 0)
            {
                vector<ll> articuls = biconn_artics[i];
                ll vertex_1 = -(i + 1);
                for (ll j = 0; j < articuls.size(); j++)
                {
                    ll vertex_2 = articuls[j];
                    all_artics.push_back(vertex_2);
                    block_cut.add_Edge(vertex_1, vertex_2);
                }
            }
        }
        return block_cut;
    }
};
#endif