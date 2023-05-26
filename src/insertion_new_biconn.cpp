#include<bits/stdc++.h>
#include<changed_static.h>
#include<ctime>
#include<biconn_helper.h>
typedef long long int ll;
using namespace std;
class Biconn_algo:public Diameter_static
{
    public:
        unordered_map<ll,ll> vertex_bcc;
        unordered_map<ll,ll> bcc_visited;
        unordered_map<string,ll> vertex_bicc_ecc;
        Biconn_algo read_file(Biconn_algo g,string filename,int choice)
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
        Biconn_algo extract_largest_CC(Biconn_algo g)
        {
            ll num_vert=g.vertices.size();
            cout<<num_vert<<endl;
            vector<ll> curr_visited(num_vert,0);
            vector<Biconn_algo*> connect_comp_vert(num_vert);
            ll comp_count=0;
            for(ll i=0;i<num_vert;i++)
            {
                if(curr_visited[g.vertices[i]]==0 && comp_count==0)
                    g.BFS(g.vertices[i],2);
                else if(curr_visited[g.vertices[i]]==0 && comp_count!=0)
                    g.BFS(g.vertices[i],1);
                vector<ll> curr_vert=g.curr_vertices;
                curr_visited=g.visited;
                connect_comp_vert[comp_count]=new Biconn_algo();
                for(ll tt=0;tt<curr_vert.size();tt++)
                {
                    vector<ll> vert_adj_list=g.adj_list[curr_vert[tt]];
                    for(ll ss=0;ss<vert_adj_list.size();ss++)
                    {
                        (*connect_comp_vert[comp_count]).add_Edge(vert_adj_list[ss],curr_vert[tt]);
                    }
                }
                comp_count++;
            }
            Biconn_algo max_conn_comp;
            ll maxiii=0;
            for(ll hh=0;hh<connect_comp_vert.size();hh++)
            {
                if((*connect_comp_vert[hh]).vertices.size()>maxiii)
                {
                    maxiii=(*connect_comp_vert[hh]).vertices.size();
                    cout<<maxiii<<endl;
                    max_conn_comp=(*(connect_comp_vert[hh]));
                }
            }
            return max_conn_comp;
        }
        ll biconn_new_algo(Biconn_ops g,unordered_map<ll,ll> changed_bicc)
        {
            clock_t begin1=clock();
            ll maxl=0;
            for(ll i=0;i<g.vertices.size();i++)
            {
                if(g.vertices[i]>maxl)
                    maxl=g.vertices[i];
            }
            g.BCC(maxl+1);
            clock_t end1=clock();
            double time_taken1=double(end1-begin1)/ CLOCKS_PER_SEC;
		    cout<<"time taken1:"<<time_taken1<<endl;	

            clock_t begin2=clock();
            g.create_req_data_strucs(g.arts);
            clock_t end2=clock();
            double time_taken2=double(end2-begin2)/ CLOCKS_PER_SEC;
		    cout<<"time taken2:"<<time_taken2<<endl;

            clock_t begin3=clock();
            vector<vector<pair<ll,ll> > > bicon_comp=g.biconns;
            vector<Biconn_algo*> bcc_graphs(bicon_comp.size());
            vector<Diameter_static*> bcc_graphs_static(bicon_comp.size());
            for(ll k=0;k<bicon_comp.size();k++)
            {
                bcc_graphs[k]=new Biconn_algo();
                bcc_graphs_static[k]=new Diameter_static();
                unordered_map<ll,ll> visited_vertices;
                for(ll s=0;s<bicon_comp[k].size();s++)
                {
                    (*bcc_graphs[k]).add_Edge(bicon_comp[k][s].first,bicon_comp[k][s].second);
                    (*bcc_graphs_static[k]).add_Edge(bicon_comp[k][s].first,bicon_comp[k][s].second);
                    if(g.arts.find(bicon_comp[k][s].first)==g.arts.end())
                        vertex_bcc[bicon_comp[k][s].first]=k;
                    if(g.arts.find(bicon_comp[k][s].second)==g.arts.end())
                        vertex_bcc[bicon_comp[k][s].second]=k;
                }
            }
            clock_t end3=clock();
            double time_taken3=double(end3-begin3)/ CLOCKS_PER_SEC;
            cout<<"time taken3:"<<time_taken3<<endl;

            cout<<"No.of articulssss:"<<g.arts.size()<<endl;
            clock_t begin4=clock();
            g.create_block_cut_tree();
            clock_t end4=clock();
            double time_taken4=double(end4-begin4)/ CLOCKS_PER_SEC;
	        cout<<"time taken4:"<<time_taken4<<endl;
          
            clock_t begin5=clock();
            ll diam=0;
            ll start_vertex=g.four_sweep_start_vertex(1);
            pair<ll,ll> four_sweep_info=g.four_sweep(start_vertex);
            ll source=four_sweep_info.first;
            diam=ifub_new(source,four_sweep_info.second,bcc_graphs,bcc_graphs_static,&g,changed_bicc);
            clock_t end5=clock();
            double time_taken5=double(end5-begin5)/ CLOCKS_PER_SEC;
	        cout<<"time taken5:"<<time_taken5<<endl;
            cout<<"1+2+3+4+5:"<<time_taken1+time_taken2+time_taken3+time_taken4+time_taken5<<endl;
            return diam;
        }
        ll ifub_helper_new(vector<ll> &vertices_subset,vector<Biconn_algo*> &bcc_graphs,vector<Diameter_static*> bcc_graphs_static,Biconn_ops* g,unordered_map<ll,ll> changed_bicc)
        {
            ll max_ecc=INT_MIN;
            ll curr_vert;
            ll bcc_id;
            vector<ll> curr_bcc_visited(bcc_graphs.size(),0);
            ll num_vert=(*g).no_vertices;
            ll count_num_calls=0;
            for(ll i=0;i<vertices_subset.size();i++)
            {
                unordered_map<ll,ll> block_visited;
                curr_vert=vertices_subset[i];
                bcc_id=vertex_bcc[curr_vert];
                Biconn_algo* gr=bcc_graphs[bcc_id];
                if(vertex_bcc.find(curr_vert)!=vertex_bcc.end())
                {
                    if((*gr).vertices.size()==2)
                    {
                        max_ecc=1;
                    }
                    else if((*gr).vertices.size()==3)
                    {
                        max_ecc=2;
                    }
                    else if(((*g).biconn_artics[bcc_id]).size()>1)
                    {
                        if(changed_bicc.find(bcc_id)!=changed_bicc.end())
                        {
                            (*gr).BFS(curr_vert,0);
                            max_ecc=max(max_ecc,(*gr).max_level);
                            string vertex_bicc_pair=to_string(curr_vert)+"#"+to_string(bcc_id);
                            vertex_bicc_ecc[vertex_bicc_pair]=(*gr).max_level;
                        }                     
                        else
                        {
                            string vertex_bicc_pair=to_string(curr_vert)+"#"+to_string(bcc_id);
                            max_ecc=max(max_ecc,vertex_bicc_ecc[vertex_bicc_pair]);
                        }
                        max_ecc=max(max_ecc,(*g).bct_traversal_biconn_point(bcc_graphs_static,bcc_graphs_static[bcc_id],bcc_id,curr_vert,block_visited,changed_bicc,vertex_bicc_ecc));
                    }
                    if(((*g).biconn_artics[bcc_id]).size()==1&&curr_bcc_visited[bcc_id]==0)
                    {
                        string vertex_bicc_pair=to_string(curr_vert)+"#"+to_string(bcc_id);
                        if(changed_bicc.find(bcc_id)!=changed_bicc.end())
                        {
                            (*gr).BFS((*g).biconn_artics[bcc_id][0],0);
                            max_ecc=max(max_ecc,(*gr).max_level+(*g).bct_traversal_art_point(bcc_graphs_static,(*g).biconn_artics[bcc_id][0],block_visited,changed_bicc,vertex_bicc_ecc));
                        }
                        else
                        {
                            max_ecc=max(max_ecc,vertex_bicc_ecc[vertex_bicc_pair]+(*g).bct_traversal_art_point(bcc_graphs_static,(*g).biconn_artics[bcc_id][0],block_visited,changed_bicc,vertex_bicc_ecc));
                        }
                        curr_bcc_visited[bcc_id]=1;
                    }
                    // else if(((*g).biconn_artics[bcc_id]).size()>1)
                    // {
                    //     max_ecc=max(max_ecc,(*g).bct_traversal_biconn_point(bcc_graphs_static,bcc_graphs_static[bcc_id],bcc_id,curr_vert,block_visited));
                    // }
                }
                else
                {
                    cout<<"helloooooo----this is an articulation point"<<endl;
                    ll curr_art_diam=(*g).bct_traversal_art_point(bcc_graphs_static,curr_vert,block_visited,changed_bicc,vertex_bicc_ecc);
                    max_ecc=max(max_ecc,curr_art_diam);
                }
            }
            return max_ecc;
        }
        ll ifub_new(ll source,ll lb,vector<Biconn_algo*> &bcc_graphs,vector<Diameter_static*> bcc_graphs_static,Biconn_ops* g,unordered_map<ll,ll> changed_bicc)
        {
            bcc_visited.clear();
            BFS(source,0);
            ll ub=2*max_level;
            lb=max(max_level,lb);
            ll k=0;
            ll curr_max_ecc;
            ll curr_level=max_level;
            unordered_map<ll,vector<ll> > level_vertices_source=level_vertices;
            while(ub-lb > k)
            {
                curr_max_ecc=ifub_helper_new(level_vertices_source[curr_level],bcc_graphs,bcc_graphs_static,g,changed_bicc);
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
        ll diam_calc_Old_algo(Biconn_algo g)
        {
            ll start_vertex,source,diameter;
            start_vertex=g.four_sweep_start_vertex(1);
            pair<ll,ll> four_sweep_info=g.four_sweep(start_vertex);
            source=four_sweep_info.first;
            ll info = g.ifub(source,four_sweep_info.second,0,0);
            return info;
        }
};

int main()
{
        string prefix="test_cases/datasets/req_datasets";
        // vector<string> data_vec={"494_bus","662_bus","685_bus","abb313","b1_ss","jnlbrng1"};
        // vector<string> data_vec={"jnlbrng1"};
        // vector<string> data_vec={"jazz","geom","ca-GrQc"};
        // vector<string> data_vec={"ca-AstroPh","ca-CondMat","ca-GrQc","ca-HepPh","geom","jazz"};
        // vector<string> data_vec={"ca-GrQc"};
        // vector<string> data_vec={"ca-AstroPh","ca-CondMat","ca-GrQc","ca-HepPh","dip20090126_MAX","french_dict","geom","HC-BIOGRID-2.0.31_MAX","IMDB","japanese_dict","jazz","spanish_dict"};
        vector<string> data_vec={"ca-AstroPh","ca-CondMat","ca-GrQc","ca-HepPh","french_dict","geom","japanese_dict","jazz","spanish_dict"};
        // vector<string> data_vec={"dip20090126_MAX","french_dict","geom","HC-BIOGRID-2.0.31_MAX","IMDB","japanese_dict","jazz","spanish_dict"};
        string filename;
        for(ll i=0;i<data_vec.size();i++)
        {
            Biconn_algo g;
            filename=prefix+"/"+data_vec[i]+"/edges.mtx";
            cout<<filename<<endl;
            g=g.read_file(g,filename,0);
            g=g.extract_largest_CC(g);
            clock_t begin,end;
            double time_taken;
            begin=clock();
            ll old_graph_old_algo_info=g.diam_calc_Old_algo(g);
            end=clock();
            cout<<"No.of vertices:"<<g.vertices.size()<<endl;
            time_taken=double(end-begin)/ CLOCKS_PER_SEC;
            cout<<"old:"<<old_graph_old_algo_info<<" time_taken:"<<time_taken<<endl;
            
            
            Biconn_ops bicc;
            bicc=bicc.read_file(bicc,filename,0);
            bicc=bicc.extract_largest_CC(bicc);
            begin=clock();
            ll old_graph_new_algo_info=g.biconn_new_algo(bicc);
            end=clock();
            time_taken=double(end-begin)/ CLOCKS_PER_SEC;
            cout<<"new:"<<old_graph_new_algo_info<<" time_taken:"<<time_taken<<endl;
            cout<<"---------"<<endl;


            string filename1=prefix+"/"+data_vec[i]+"/inserted_100.mtx";
            bicc.read_file(bicc,filename1,0);
            bicc=bicc.extract_largest_CC(bicc);
            unordered_map<ll,ll> changed_bicc;
            for(auto it=g.vertex_bcc.begin();it!=g.vertex_bcc.end();it++)
            {
                changed_bicc[it->second]=1;
            }
            begin=clock();
            ll old_graph_new_algo_info=g.biconn_new_algo(bicc,changed_bicc);
            end=clock();
            time_taken=double(end-begin)/ CLOCKS_PER_SEC;
            cout<<"new:"<<old_graph_new_algo_info<<" time_taken:"<<time_taken<<endl;
            cout<<"---------"<<endl;
        }
        //map each BCC to a number(0 or 1) ==> 0 indicates no edges are inserted or deleted
        //                                     1 indicates new edges are inserted or deleted
        // ifub_insert_new(source,four_sweep_info.second,bcc_graphs,bcc_graphs_static,&g,effected_BCCs);
}
/*Add the conditions for insertion and deletion afterwards*/