#ifndef STATIC_H
#define STATIC_H
typedef long long int ll;
#define NIL -1
#include<bits/stdc++.h>
#include <boost/graph/biconnected_components.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>
using namespace std;
namespace boost
{
  struct edge_component_t
  {
    enum
    { num = 555 };
    typedef edge_property_tag kind;
  }
  edge_component;
}
typedef boost::adjacency_list < boost::vecS, boost::vecS, boost::undirectedS,boost::no_property, boost::property < boost::edge_component_t, std::size_t > >graph_t;
typedef boost::graph_traits < graph_t >::vertex_descriptor vertex_t;
class Diameter_static
{
    public:
        unordered_map<ll,vector<ll> > adj_list;//adjacency list
        unordered_map<ll,ll> level;//vertex,level
        unordered_map<ll,vector<ll> > level_vertices;//level,vertex
        vector<ll> vertices;//vertices
        vector<pair<ll,ll> > vertex_parent;//BFS_edges
        ll max_level_vertex;
        ll mid_vertex;
        ll max_level;
        ll no_vertices;
        ll E=0;
        ll std_count=1;
        ll bfs_count=0;
        vector<vector<pair<ll,ll> > > biconns;
        unordered_map<ll,ll> arts;
        ll count=0;
        vector<ll> curr_vertices;
        vector<ll> visited;
        ll no_edges=0;
        void add_Edge(ll a,ll b)
        {
           no_edges=no_edges+1;
           if(adj_list.find(a)==adj_list.end())
           {
               add_vertex(a);
           }
           if(adj_list.find(b)==adj_list.end())
           {
               add_vertex(b);
           }
           adj_list[a].push_back(b);
           E++;
           adj_list[b].push_back(a);
        }
        void add_vertex(ll vertex)
        {
            vertices.push_back(vertex);
        }
        void BFS(ll source,ll choice)
        {
            ll maxi=INT_MIN;
            for(ll i=0;i<vertices.size();i++)
            {
                maxi=max(maxi,vertices[i]);
            }
            no_vertices=maxi+1;
            level.clear();
            level_vertices.clear();
            vertex_parent.clear();
            if(choice==0)
            {
                visited.clear();
                visited.assign(no_vertices,0);
            }
            if(choice==2)
            {
                visited.assign(no_vertices,0);
            }
            vertex_parent.assign(no_vertices,make_pair(-1,0));
            curr_vertices.clear();
            queue<ll> qu;
            qu.push(source);
            visited[source]=1;
            level[source]=0;
            ll top;
            max_level_vertex=source;
            level_vertices[level[source]].push_back(source);
            while(!qu.empty())
            {
                top=qu.front();
                curr_vertices.push_back(top);
                for(ll i=0;i<adj_list[top].size();i++)
                {
                    if(visited[adj_list[top][i]]==0)
                    {
                        qu.push(adj_list[top][i]);
                        visited[adj_list[top][i]]=1;
                        level[adj_list[top][i]]=level[top]+1;
                        level_vertices[level[adj_list[top][i]]].push_back(adj_list[top][i]);
                        vertex_parent[adj_list[top][i]]=make_pair(top,level[adj_list[top][i]]);
                        if(level[adj_list[top][i]]>level[max_level_vertex])
                        {
                            max_level_vertex=adj_list[top][i];
                        }
                    }
                }
                qu.pop();
            }
            max_level=level[max_level_vertex];
            mid_vertex=level_vertices[max_level/2][0];
        }
        ll four_sweep_start_vertex(ll choice)
        {
            ll vertex;
            if(choice==0)
            {
                ll random = rand() % no_vertices;
                vertex = vertices[random]; 
            }
            else
            {
                ll max1=INT_MIN;
                for(auto it=adj_list.begin();it!=adj_list.end();it++)
                {
                    ll temp1=(it->second.size());
                    if(temp1 > max1)
                    {
                        max1=(it->second).size();
                        vertex=it->first;
                    }
                }
            }
            return vertex;
        }
        pair<ll,ll> four_sweep(ll source)
        {
            ll lb=INT_MIN;
            BFS(source,0);//BFS with starting vertex
            source=max_level_vertex;
            BFS(source,0);//BFS with max eccentricity vertex
            lb=max(lb,max_level);
            source=mid_vertex;
            BFS(source,0);//BFS with middle vertex
            source=max_level_vertex;
            BFS(source,0);//BFS with max eccentricity vertex
            lb=max(lb,max_level);
            source=mid_vertex;
            return make_pair(source,lb);
        }
        ll ifub_helper(vector<ll> vertices_subset)
        {
            ll max_ecc=INT_MIN;
            for(ll i=0;i<vertices_subset.size();i++)
            {
                clock_t begin5=clock();
                BFS(vertices_subset[i],0);
                // cout<<"current vertex:"<<vertices_subset[i]<<" max_level:"<<max_level<<endl;
                max_ecc=max(max_ecc,max_level);
                clock_t end5=clock();
                double time_taken5=double(end5-begin5)/ CLOCKS_PER_SEC;
            }
            return max_ecc;
        }
        ll ifub(ll source,ll lb,ll choice_bicc,ll mini_deg)
        {
            BFS(source,0);
            ll ub=2*max_level;
            lb=max(max_level,lb);
            ll k=0;
            ll curr_max_ecc;
            ll curr_level=max_level;
            unordered_map<ll,vector<ll> > level_vertices_source=level_vertices;
            while(ub-lb > k)
            {
                curr_max_ecc=ifub_helper(level_vertices_source[curr_level]);
                if(max(lb,curr_max_ecc) > 2*(curr_level-1))
                {
                    return max(lb,curr_max_ecc);
                }
                else
                {
                    lb=max(lb,curr_max_ecc);
                    ub=2*(curr_level-1);
                }
                curr_level--;
            }
            return lb;
        }
};

#endif