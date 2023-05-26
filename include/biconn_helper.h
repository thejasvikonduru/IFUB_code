#include <bits/stdc++.h>
#include <changed_static.h>
#include <ctime>
#ifndef BICONN_HELPER_H
#define BICONN_HELPER_H
typedef long long int ll;
using namespace std;
#include <boost/graph/biconnected_components.hpp>
#include <boost/graph/adjacency_list.hpp>
class Biconn_ops : public Diameter_static
{
public:
    unordered_map<ll, vector<ll>> biconn_artics;
    unordered_map<string, ll> dist_artics;
    unordered_map<string, ll> art_eccs;
    Diameter_static block_cut;
    unordered_map<string, ll> art_child_eccs;
    // unordered_map<ll, ll> bcc_visited;
    unordered_map<ll,ll> vertex_bcc;

    /*Analysis required data*/
    unordered_map<ll,pair<ll,ll> > bcc_size_frequency;
    // Biconn_ops max_comp_g;

    Biconn_ops read_file(Biconn_ops g,string filename,int choice)
    {
        ifstream file;
        file.open(filename);
        if(file.is_open())
        {
            string line,vertex1,vertex2;
            while (getline(file, line))
            {
                for(ll i=0;i<line.length();i++)
                {
                    if(line[i]==' ')
                    {
                        vertex1=line.substr(0,i);
                        vertex2=line.substr(i+1,line.length()-(i+1));
                        break;
                    }
                }
                ll v1=stoi(vertex1);
                ll v2=stoi(vertex2);
                if(choice==0)
                    g.add_Edge(v1,v2);
            }
        }
        file.close();
        return g;
    }
    Biconn_ops extract_largest_CC(Biconn_ops g)
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
        Biconn_ops max_conn_comp;
        vector<ll> maxi_num_vert_comp = connect_comp_vert[maxi_index];
        unordered_map<ll,ll> visited_vert;
        ll max_conn_edges=0;
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
    Diameter_static create_block_cut_tree(Biconn_ops* g)
    {
        for(auto it=(*g).biconn_artics.begin();it!=(*g).biconn_artics.end();it++)
        {
            ll vertex_1 = -(it->first+1);
            for(ll j=0;j<(it->second).size();j++)
            {
                ll vertex_2 = (it->second)[j];
                // cout<<"vertex_1:"<<vertex_1<<" "<<"vertex_2:"<<vertex_2<<endl;
                (*g).block_cut.add_Edge(vertex_1,vertex_2);
            }
        }
        unordered_map<ll,vector<ll> > temp_adj_list = (*g).block_cut.adj_list;
        vector<ll> vec_temp_adj_list = temp_adj_list[142078];
        for(ll j=0;j<vec_temp_adj_list.size();j++)
        {
            cout<<vec_temp_adj_list[j]<<" ";
        }
        cout<<endl;
        return (*g).block_cut;
    }
    vector<Biconn_ops*> BCC(ll V,graph_t g,Biconn_ops* total_g)
    {
            vector<Biconn_ops*> bcc_graphs(V);
            for(ll i=0;i<V;i++)
            {
                 bcc_graphs[i]=new Biconn_ops();
            }
            clock_t begin4=clock();
            boost::property_map < graph_t, boost::edge_component_t >::type
            component = boost::get(boost::edge_component, g);
            std::size_t num_comps = boost::biconnected_components(g, component);
            std::vector<vertex_t> art_points;
            boost::articulation_points(g, std::back_inserter(art_points));
            clock_t end4=clock();
            std::cerr << "Found " << art_points.size() << " articulation points.\n";
            for(ll i=0;i<art_points.size();i++)
                (*total_g).arts[art_points[i]]=1;
            boost::graph_traits < graph_t >::edge_iterator ei, ei_end;
            ll flag=1;
            unordered_map<string,ll> visited_vert_pair;
            unordered_map<string,ll> comp_bicc_pair;
            string req_str;
            for (boost::tie(ei, ei_end) = edges(g); ei != ei_end; ++ei)
            {
                if(source(*ei,g)<=target(*ei,g))
                    req_str=to_string(source(*ei,g))+"#"+to_string(target(*ei,g));
                else
                    req_str=to_string(target(*ei,g))+"#"+to_string(source(*ei,g));
                if(visited_vert_pair.find(req_str)==visited_vert_pair.end())
                {
                    (*bcc_graphs[component[*ei]]).add_Edge(source(*ei,g),target(*ei,g));
                    (*total_g).vertex_bcc[source(*ei,g)]=component[*ei];
                    (*total_g).vertex_bcc[target(*ei,g)]=component[*ei];
                    if((*total_g).arts.find(source(*ei,g))!=(*total_g).arts.end())
                    {
                        if(comp_bicc_pair.find(to_string(component[*ei])+"#"+to_string(source(*ei,g)))==comp_bicc_pair.end())
                        {
                            (*total_g).biconn_artics[component[*ei]].push_back(source(*ei,g));
                            comp_bicc_pair[to_string(component[*ei])+"#"+to_string(source(*ei,g))]=1;
                        }
                    }
                    if((*total_g).arts.find(target(*ei,g))!=(*total_g).arts.end())
                    {
                        if(comp_bicc_pair.find(to_string(component[*ei])+"#"+to_string(target(*ei,g)))==comp_bicc_pair.end())
                        {
                            (*total_g).biconn_artics[component[*ei]].push_back(target(*ei,g));
                            comp_bicc_pair[to_string(component[*ei])+"#"+to_string(target(*ei,g))]=1;
                        }
                    }
                    visited_vert_pair[req_str]=1;
                }
            }
            double time_taken4 = double(end4 - begin4) / CLOCKS_PER_SEC; 
            cout<<"time_taken4:"<<time_taken4<<endl;
            return bcc_graphs;
    }
    vector<Biconn_ops *> biconn_new_algo(Biconn_ops *g1, string filename, ll maxl, graph_t* boost_g)
    {
        clock_t begin1=clock();
        // Biconn_ops g=(*g1);
        biconns.resize(maxl + 1);
        vector<Biconn_ops *> bcc_graphs;
        bcc_graphs=(*g1).BCC(maxl + 1, (*boost_g),g1);
        clock_t end1 = clock();
        double time_taken1 = double(end1 - begin1) / CLOCKS_PER_SEC;
        cout << "No.of articulssss:" << (*g1).arts.size() << endl;
        clock_t begin2=clock();
        block_cut=(*g1).create_block_cut_tree(g1);
        clock_t end2=clock();
        double time_taken2 = double(end2 - begin2) / CLOCKS_PER_SEC;
        cout<<time_taken1<<" "<<time_taken2<<endl;
        return bcc_graphs;
    }
    ll bct_traversal_art_point(Biconn_ops* g,vector<Biconn_ops*> &bcc_graphs,ll art,unordered_map<ll,ll> &block_visited)
    {
            ll ecc=INT_MIN;
            block_visited[art]=1;
            vector<ll> bicc_vertices=(*g).block_cut.adj_list[art];
            for(ll i=0;i<bicc_vertices.size();i++)
            {
                // cout<<"Entered-1:"<<endl;
                ll bicc_vertex=bicc_vertices[i];
                ll actual_bicc_vertex=-bicc_vertex-1;
                if(block_visited.find(bicc_vertex)==block_visited.end())
                {
                    // cout<<"Entered-2:"<<endl;
                    if((*g).art_child_eccs.find(to_string(art)+"#"+to_string(actual_bicc_vertex))==(*g).art_child_eccs.end())
                    {
                        // cout<<"Entered-3:"<<endl;
                        ll child_ecc;
                        Diameter_static* curr_gr;
                        // cout<<(*g).art_eccs.size()<<endl;
                        if((*g).art_eccs.find(to_string(art)+'#'+to_string(-bicc_vertex-1))==(*g).art_eccs.end())
                        {
                            // cout<<"Entered-9:"<<actual_bicc_vertex<<endl;
                            curr_gr=bcc_graphs[actual_bicc_vertex];
                            // cout<<"Entered-10"<<endl;
                            (*curr_gr).BFS(art,0);
                            // cout<<"Entered-11"<<endl;
                            child_ecc=(*curr_gr).max_level;
                            // cout<<"child_ecc:"<<child_ecc<<endl;
                            (*g).art_eccs[to_string(art)+'#'+to_string(-bicc_vertex-1)]=child_ecc;
                        }
                        else
                        {
                            // cout<<"Entered-4:"<<endl;
                            child_ecc=(*g).art_eccs[to_string(art)+'#'+to_string(-bicc_vertex-1)];
                        }
                        block_visited[bicc_vertex]=1;
                        for(ll j=0;j<(*g).block_cut.adj_list[bicc_vertex].size();j++)
                        {
                            // cout<<"Entered-5:"<<endl;
                            if(block_visited.find((*g).block_cut.adj_list[bicc_vertex][j])==block_visited.end())
                            {
                                string ecc_pair;
                                if(art<(*g).block_cut.adj_list[bicc_vertex][j])
                                    ecc_pair=to_string(art)+"#"+to_string((*g).block_cut.adj_list[bicc_vertex][j]);
                                else
                                    ecc_pair=to_string((*g).block_cut.adj_list[bicc_vertex][j])+"#"+to_string(art);
                                if((*g).dist_artics.find(ecc_pair)==(*g).dist_artics.end())
                                {
                                    (*g).dist_artics[ecc_pair]=(*curr_gr).level[(*g).block_cut.adj_list[bicc_vertex][j]];
                                }
                                else
                                {
                                    child_ecc=max(child_ecc,(*g).dist_artics[ecc_pair]+bct_traversal_art_point(g,bcc_graphs,(*g).block_cut.adj_list[bicc_vertex][j],block_visited));
                                }
                            }
                        }
                        (*g).art_child_eccs[to_string(art)+"#"+to_string(actual_bicc_vertex)]=child_ecc;
                        ecc=max(ecc,child_ecc);
                    }
                    else
                    {
                        // cout<<"Entered-6"<<endl;
                        block_visited[bicc_vertex]=1;
                        ll child_ecc=(*g).art_child_eccs[to_string(art)+"#"+to_string(actual_bicc_vertex)];
                        ecc=max(ecc,child_ecc);
                    }
                }
            }
            return ecc;
    }
    ll bct_traversal_biconn_point(Biconn_ops* g,vector<Biconn_ops*> &bcc_graphs,Diameter_static* gr,ll biconn,ll vert,unordered_map<ll,ll> &block_visited)
    {
            ll ecc=INT_MIN;
            ll block_biconn=-(biconn+1);
            // cout<<"bct_traversal_biconn_point-1"<<endl;
            block_visited[block_biconn]=1;
            // cout<<"bct_traversal_biconn_point-2"<<endl;
            // cout<<"bct_traversal_biconn_point:"<<block_biconn<<" "<<(*g).block_cut.adj_list[block_biconn].size()<<endl;

            for(ll i=0;i<((*g).block_cut).adj_list[block_biconn].size();i++)
            {
                // cout<<"hmmmm-1:"<<endl;
                ll vert_art_dist=((*gr).level)[((*g).block_cut).adj_list[block_biconn][i]];
                // cout<<"hmmmm-2:"<<vert_art_dist<<endl;
                ll curr_ecc=vert_art_dist+bct_traversal_art_point(g,bcc_graphs,((*g).block_cut).adj_list[block_biconn][i],block_visited);
                // cout<<"hmmmm-3:"<<curr_ecc<<endl;
                ecc=max(ecc,curr_ecc);
                // cout<<"hmmmm-4:"<<ecc<<endl;
            }
            return ecc;
    }
    ll ifub_helper_new(vector<ll> &vertices_subset, vector<Biconn_ops *> &bcc_graphs, Biconn_ops *g)
    {
        // cout<<"ifub_helper_new:"<<vertex_bcc[30299]<<endl;
        ll max_ecc = INT_MIN;
        ll curr_vert;
        ll bcc_id;
        vector<ll> curr_bcc_visited(bcc_graphs.size(), 0);
        ll num_vert = (*g).vertices.size();
        ll count_num_calls = 0;
        unordered_map<ll,ll> arts_req=(*g).arts;
        // cout<<"hello-world-1:"<<vertices_subset.size()<<endl;
        for (ll i = 0; i < vertices_subset.size(); i++)
        {
            // cout<<"size:"<<vertices_subset.size()<<endl;
            unordered_map<ll, ll> block_visited;
            // cout<<"hello-world2"<<endl;
            curr_vert = vertices_subset[i];
            // cout<<"curr_vert:"<<curr_vert<<" "<<(*g).vertex_bcc.size()<<endl;
            bcc_id = (*g).vertex_bcc[curr_vert];
            // cout<<"bcc_id:"<<bcc_id<<endl;
            // cout<<"bcc_graphs_size:"<<bcc_graphs.size()<<endl;
            Biconn_ops *gr = bcc_graphs[bcc_id];
            // bcc_size_frequency[bcc_id].first=((*g).vertices.size()+(*g).no_edges)-((*gr).vertices.size()+(*gr).no_edges);
            // bcc_size_frequency[bcc_id].second += 1;
            clock_t begin1=clock();
            ll flag_helper=0;
            if(arts_req.find(curr_vert) == arts_req.end())
            {
                if ((*gr).vertices.size() == 2)
                {
                    max_ecc = 1;
                    flag_helper=1;
                }
                else
                {
                    (*gr).BFS(curr_vert, 0);
                    max_ecc = max(max_ecc, (*gr).max_level);
                    flag_helper=1;
                }
                ll curr_curr_ecc=(*gr).bct_traversal_biconn_point(g,bcc_graphs, bcc_graphs[bcc_id], bcc_id, curr_vert, block_visited);
                max_ecc = max(max_ecc,curr_curr_ecc);
                cout<<"current vertex-1:"<<vertices_subset[i]<<" max_level:"<<curr_curr_ecc<<endl;
            }
            else
            {
                ll curr_art_diam = (*gr).bct_traversal_art_point(g,bcc_graphs, curr_vert, block_visited);
                max_ecc = max(max_ecc, curr_art_diam);
                cout<<"current vertex-2:"<<vertices_subset[i]<<" max_level:"<<curr_art_diam<<endl;
            }
            clock_t end1=clock();
            double time_taken1=double(end1 - begin1) / CLOCKS_PER_SEC;
            cout<<time_taken1<<endl;
        }
        return max_ecc;
    }
    // ll ifub_new(string filename,ll source, ll lb, Biconn_ops *g, ll maxl, graph_t* boost_g)
    ll ifub_new(string filename,ll source, ll lb, Biconn_ops *g, ll maxl, graph_t* boost_g)
    {
        ll count23=0;
        // bcc_visited.clear();
        BFS(source, 0);
        ll ub = 2 * max_level;
        lb = max(max_level, lb);
        ll k = 0;
        ll curr_max_ecc;
        ll curr_level = max_level;
        unordered_map<ll, vector<ll>> level_vertices_source = level_vertices;
        if (ub - lb <= k)
        {
            count23=0;
            return lb;
        }
        else
        {
            vector<Biconn_ops *> bcc_graphs=biconn_new_algo(g,filename,maxl,boost_g);
            clock_t begin3=clock();
            while (ub - lb > k)
            {
                curr_max_ecc = ifub_helper_new(level_vertices_source[curr_level], bcc_graphs, g);
                if (max(lb, curr_max_ecc) > 2 * (curr_level - 1))
                {
                    return max(lb, curr_max_ecc);
                }
                else
                {
                    lb = max(lb, curr_max_ecc);
                    ub = 2 * (curr_level - 1);
                }
                curr_level--;
            }
            clock_t end3=clock();
            double time_taken3 = double(end3 - begin3) / CLOCKS_PER_SEC;
            count23=0;
            return lb;
        }
    }
};
#endif