#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <cmath>
#include <unordered_map>
#include <fstream>
 
struct TreeNode;
struct EdgeNode;
 
typedef std::string tree_t;
 
struct EdgeNode{
    tree_t val;
    TreeNode* subtree;
    EdgeNode* next;
};
 
struct TreeNode{
    tree_t val;
    EdgeNode* subtree_l;
};
 
TreeNode* allocate_tree_node(tree_t e){
    TreeNode* tmp = new TreeNode;
    tmp->val = e;
    tmp->subtree_l = NULL;
    return tmp;
}
 
EdgeNode* cons_edge_node(TreeNode* t, EdgeNode* subtree_l, tree_t val){
    EdgeNode* tmp = new EdgeNode;
    tmp -> val = val;
    tmp->subtree = t;
    tmp->next = subtree_l;
    return tmp;
}
 
TreeNode* build_tree_root(tree_t e){
    return allocate_tree_node(e);
}
 
void deallocate_tree(TreeNode* t){
    if(t != NULL){
        EdgeNode* it = t->subtree_l;
        while(it != NULL){
            deallocate_tree(it->subtree);
            EdgeNode* tmp = it->next;
            delete it;
            it = tmp;
        }
        delete t;
    }
}

bool vector_search(const std::vector<std::string> test_vector, std::string lookup, int& index){
    for(int i = 0; i < test_vector.size(); i++){
        if(test_vector[i] == lookup){
            index = i;
            return true;
        }
    }
    return false;
}

bool special_vector_search(const std::vector<std::vector<std::string>> test_vector, std::string lookup, int& index){
    for(int i = 0; i < test_vector.size(); i++){
        if(test_vector[i][0] == lookup){
            index = i;
            return true;
        }
    }
    return false;
}

bool end_sub_group(const std::vector<std::vector<std::string>>& data, const std::vector<int> sub_group){
    int end_col_index = data[0].size() - 1;
    std::string tmp = data[sub_group[0]][end_col_index];
    for(int i : sub_group){
        if(tmp != data[i][end_col_index]){
            return false;
        }
    }
    return true;
}


std::vector<std::string> split_on_column(const std::vector<std::vector<std::string>>& data, int column_n, const std::vector<int> sub_group, std::vector<std::vector<int>>& cleaned_sub_groups_out, std::vector<std::pair<std::string,std::string>>& out_ends){
    std::vector<std::string> out = {};
    std::vector<std::vector<int>> sub_groups_out = {};
    std::vector<std::string> cleaned_out = {};
    cleaned_sub_groups_out = {};
    out_ends = {};
    int temp;
    for(int i : sub_group){
        if(vector_search(out, data[i][column_n], temp)){
            sub_groups_out[temp].push_back(i);
        }
        else{
            out.push_back(data[i][column_n]);
            sub_groups_out.push_back({i});
        }
    }  
    for(int i = 0; i < sub_groups_out.size(); i++){
        if(end_sub_group(data,sub_groups_out[i])){
            int end_col_index = data[0].size() - 1;
            std::pair<std::string, std::string> temporary_double;
            temporary_double.first = out[i];
            temporary_double.second = data[sub_groups_out[i][0]][end_col_index];
            out_ends.push_back(temporary_double);               
            }
        else{
            cleaned_out.push_back(out[i]);
            cleaned_sub_groups_out.push_back(sub_groups_out[i]);
        }
    }
    return cleaned_out;
}

std::string best_entropy_catergory(const std::vector<std::vector<std::string>>& data,const std::vector<int> sub_group, std::vector<std::string>& best_sub_group_out_titles, std::vector<std::vector<int>>& best_sub_groups_out, std::vector<std::pair<std::string,std::string>>& best_end){
    int best = -1;
    std::vector<std::string> sub_group_out_titles = {};
    best_sub_groups_out = {};
    std::vector<std::vector<int>> sub_groups_out; 
    double entropy = 0;
    double tmp;
    std::string category;
    std::vector<std::pair<std::string, std::string>> ends;
    for(int i = 0; i < data[0].size() - 1; i++){
        std::vector<std::pair<std::string, std::string>>end = {};
        double entropy = 0;
        sub_group_out_titles = split_on_column(data, i, sub_group, sub_groups_out, ends);
        for(int i = 0; i < sub_groups_out.size(); i++){
                tmp = sub_groups_out[i].size();
                entropy = entropy + tmp*std::log2(tmp);
        }
        if(entropy < best || best == -1){
            best = entropy;
            best_sub_group_out_titles = sub_group_out_titles;
            best_sub_groups_out = sub_groups_out;
            category = data[0][i];
            best_end = ends;
        }
    }
    return category;
}

std::vector<std::vector<std::string>> order_structure (std::vector<std::vector<std::string>> in){
    std::vector<std::vector<std::string>> out;
    for(int i = 0; i < in.size(); i++){
        for(auto line : in){
            if(line[0] == std::to_string(i)){
                out.push_back(line);
            }
        }
    }
    return out;
}



TreeNode* BuildTree(const std::vector<std::vector<std::string>>& data, const std::vector<int> sub_group, std::unordered_map<std::string,std::vector<std::string>>& map,int index, int& max_index, std::vector<std::vector<std::string>>& structure){
    TreeNode* root;
    int tmp_max_index = index;
    std::vector<std::string> tmp;
    tmp.push_back(std::to_string(index));    
    tmp.push_back("node");

    std::vector<std::string> continued_sub_group_titles_out;
    std::vector<std::vector<int>> continued_sub_groups_out;
    std::vector<std::pair<std::string,std::string>> ended_out;
    std::string category = best_entropy_catergory(data, sub_group, continued_sub_group_titles_out, continued_sub_groups_out, ended_out);
    root = build_tree_root(category);
    int dontcare;
    tmp.push_back(category);
    EdgeNode* e = NULL;
    for(int i = 0; i < continued_sub_group_titles_out.size(); i++){
            tmp.push_back(continued_sub_group_titles_out[i]);
            tmp.push_back(std::to_string(tmp_max_index + 1));
            map[continued_sub_group_titles_out[i]].push_back(category);
            std::cout << "added: " << continued_sub_group_titles_out[i] << " to " << category << " as a continued split" << std::endl;
            e = cons_edge_node(BuildTree(data,continued_sub_groups_out[i],map, tmp_max_index + 1, tmp_max_index, structure),e,continued_sub_group_titles_out[i]);
    }
    for(std::pair<std::string,std::string> end_group : ended_out){
            map[end_group.first].push_back(category);
            tmp.push_back(end_group.first);
            tmp.push_back(std::to_string(tmp_max_index + 1));
            tmp_max_index++;
            std::vector<std::string> leaf;
            leaf.push_back(std::to_string(tmp_max_index));
            leaf.push_back("leaf");
            leaf.push_back(end_group.second);
            structure.push_back(leaf);

            std::cout << "added: " << end_group.first << " to " << category << " as an end" << std::endl;
            e = cons_edge_node(allocate_tree_node(end_group.second),e,end_group.first);
        }
    root -> subtree_l = e;
    max_index = tmp_max_index;
    structure.push_back(tmp);
    return root;
}

std::vector<std::string> remove_val(std::vector<std::string> vin, std::string val){
    std::vector<std::string> out;
    for(std::string i : vin){
        if(i != val){
            out.push_back(i);
        }
    }
    return out;
}

bool all_same(std::vector<std::string> v){
    if(v.size() == 0){ //stop from checking vector with zero entries
        return false;
    }
    std::string x = v[0];
    for(std::string i : v){
        if(i != x){
            return false;
        }
    }
    return true;
}

std::unordered_map<std::string, std::string> sub_group_title_for_each_category (std::unordered_map<std::string, std::vector<std::string>> categories_for_subgroup, const std::vector<std::string> query_vector){
    std::cout << "Entered cat_to_sub with " << query_vector.size() << " queries" << std::endl;
    std::unordered_map<std::string, std::string> outmap;
    std::vector<std::string> unresolved_query = query_vector;
    while(unresolved_query.size() > 0){
        std::cout << "iteration:::" << std::endl;
        std::vector<std::string> new_found_categories = {};
        std::vector<std::string> unresolved_query_tmp = {};
        for(std::string single_query : unresolved_query){
            if(all_same(categories_for_subgroup[single_query])){
                outmap[categories_for_subgroup[single_query][0]] = single_query;
                new_found_categories.push_back(categories_for_subgroup[single_query][0]);
            }
            else if(categories_for_subgroup[single_query].size() > 1){
                unresolved_query_tmp.push_back(single_query);
            }
        }
        unresolved_query = unresolved_query_tmp;
        for(std::string query : unresolved_query){

            for(std::string found_cat : new_found_categories){
                std::cout << "Size: before of: " << query << " is " << categories_for_subgroup[query].size() << std::endl;
                categories_for_subgroup[query] = remove_val(categories_for_subgroup[query], found_cat);
                std::cout << "Size: after of: " << query << " is " << categories_for_subgroup[query].size() << std::endl;
            }
        }
    }
    return outmap;   
}



std::string query_function(const TreeNode* t, std::unordered_map<std::string, std::string> sub_group_title_for_each_category){
    if(t -> subtree_l == NULL){
        return t->val;
    }
    EdgeNode* e = t -> subtree_l;
    int dontcare;

    while(e!=NULL){

        if(e->val == sub_group_title_for_each_category[t->val]){
            return query_function(e ->subtree,sub_group_title_for_each_category);
        }
        e = e->next;
    }
    return "QUERY FAIL";
}

int node_count_function(TreeNode* t){
    if(t == NULL){
        return 0;
    }
    int sum = 1;
    EdgeNode* e = t ->subtree_l;
    while(e!=NULL){
        sum = sum + node_count_function(e ->subtree);
        e = e->next;
    }
    return sum;
}

int leaf_node_count_function(TreeNode* t){
    if(t -> subtree_l == NULL){   
        return 1;
    }
    int sum = 0;
    EdgeNode* e = t ->subtree_l;
    while(e!=NULL){
        sum = sum + leaf_node_count_function(e ->subtree);
        e = e->next;
    }
    return sum;
}

void category_sub_groups(TreeNode* t, std::vector<std::vector<std::string>>& current_vector, std::vector<std::string>& leaf_nodes){
    int dontcare;
    EdgeNode* e = t -> subtree_l;
    if(t -> subtree_l == NULL){
        if(!vector_search( leaf_nodes,t -> val, dontcare)){
            leaf_nodes.push_back(t->val);
        }
    }

    else{
        int index = -1;
        for(int i = 0; i < current_vector.size(); i++){
            if(current_vector[i][0] == t->val){
                index = i;
            }
        }
        if(index == -1){
            std::vector<std::string> tmp;
            while(e != NULL){
                tmp.push_back(e->val);
                e = e->next;
            }
            current_vector.push_back(tmp);
        }
        else{
            while(e !=NULL){
                if(!vector_search(current_vector[index], e->val, dontcare)){
                    current_vector[index].push_back(e->val);
                }
                e = e->next;

            }
        }
        
    }
    e = t -> subtree_l;
    while(e != NULL){
        category_sub_groups(e->subtree, current_vector, leaf_nodes);
        e = e->next;
    }
}

std::string binary(int width, int val){
    int tmp_val = val;
    std::string s(width, '0');
    for(int i = width-1; i >= 0; i--){
        int exponent = static_cast<int>(pow(2,i));
        if(exponent <=  tmp_val){
            s[width-1-i] = '1';
            tmp_val = tmp_val - exponent;
        }
    }

    return s;
}



class A3Tree{

public:
    A3Tree(const std::vector<std::vector<std::string>>& data){
        std::vector<int> row_indexs;
        for(int i = 1; i < data.size(); i++){
            row_indexs.push_back(i);
        }
        std::unordered_map<std::string,std::vector<std::string>> map;
        int no_nodes;
        t = BuildTree(data, row_indexs, map, 0, no_nodes, structure);
        sub_group_corresponding_categorys = map;
        structure = order_structure(structure);
        results = outputs();
        categories = cats_features();
    }

    std::string query(std::vector<std::string> query_vector){
        return query_function(t, sub_group_title_for_each_category(sub_group_corresponding_categorys, query_vector));
    }

    int node_count(){
        return node_count_function(t);
    }

    int leaf_node_count(){
        return leaf_node_count_function(t);
    }

    void print_hash(std::vector<std::string> v){
        for(auto i : v){
            std::cout<< i << ": " << std::endl;
            for(auto x : sub_group_corresponding_categorys[i]){
                std::cout << x << std::endl;
            }
        }
    }

    void print_structure(){
        for(auto node : structure){
            std::string tmp;
            for(auto i : node){
                tmp = tmp+ i + " ";
            }
            std::cout<< tmp << std::endl;
        }
    }

    void to_out_structure(){
        out_structure = {};
        for(auto layer : structure){
            std::vector<std::string> out_layer;
            out_layer.push_back(layer[0]);
            if(layer[1] == "leaf"){
                out_layer.push_back("1");
                out_layer.push_back(result_encoded[layer[2]]);
                splits_per_node.push_back(1);
            }
            else{
                out_layer.push_back("0");
                out_layer.push_back(std::to_string((cat_bits[layer[2]]).first));
                out_layer.push_back(std::to_string((cat_bits[layer[2]]).second));
                for(int i = 3; i < layer.size(); i = i+2){
                    out_layer.push_back(cat_group_encoding[layer[2]][layer[i]]);
                    out_layer.push_back(layer[i+1]);
                }
                splits_per_node.push_back((layer.size()-3)/2);


            }
            out_structure.push_back(out_layer);
        }
    }

    std::vector<std::string> outputs(){
        std::vector<std::string> out;
        int dontcare;
        for(auto layer : structure){
            if(layer[1] == "leaf"){
                if(!vector_search(out, layer[2],dontcare)){
                    out.push_back(layer[2]);
                }
            }
        }
        return out;
    }

    void export_encoding(std::string file_path = "C:/Users/louja/Desktop/OwnProjects/Decision_tree/decision_tree/Decision-tree-verilog/encodings.txt"){
        int output_bit_width = ceil(log2(results.size()));
        output_width = output_bit_width; 
        std::ofstream file(file_path);
        file << "---------------- Outputs --------------" << "\n";
        file << "Bit width: " << output_bit_width << " = [" << output_bit_width-1 <<":0]" <<"\n";
        for(int i = 0; i < results.size(); i++){
            result_encoded[results[i]] = std::to_string(i);
            file << results[i] << ": " << binary(output_bit_width, i) << "\n";
        }
        file << "---------------------- Categories ------------------" << "\n";
        int bottom_bit = 0;
        for(auto category : categories){
            int c_bit_width = ceil(log2(category.size() - 1));
            cat_bits[category[0]].first = bottom_bit + c_bit_width - 1;
            cat_bits[category[0]].second = bottom_bit;
            file << category[0] << ": " << std::to_string(c_bit_width) << " bits, [" << std::to_string(bottom_bit + c_bit_width - 1) << ":" << std::to_string(bottom_bit) << "]" << "\n";
            bottom_bit = bottom_bit + c_bit_width;
            for(int i = 1; i < category.size(); i++){
                cat_group_encoding[category[0]][category[i]] = std::to_string(i-1);
                file << category[i] << ": " << binary(c_bit_width, i-1) << "\n";
            } 
        } 
        file << "--------------------- Overall input width: " << bottom_bit << " = [" << bottom_bit-1 << ":0] --------------------" << "\n"; 
        input_width = bottom_bit;


        file.close();
    }


    std::vector<std::vector<std::string>> cats_features(){
        std::vector<std::vector<std::string>> out;
        std::unordered_map<std::string, int> map;
        int index;
        for(auto layer : structure){

            if(layer[1] == "node"){
                if (!special_vector_search(out, layer[2], index)){
                    std::vector<std::string> tmp_layer;
                    tmp_layer.push_back(layer[2]);
                    for(int i = 3; i < layer.size(); i = i+2){
                        tmp_layer.push_back(layer[i]);
                    }
                    out.push_back(tmp_layer);
                }
                else{
                    for(int i = 3; i < layer.size(); i = i+2){
                        if(!vector_search(out[index], layer[i],index)){
                            out[index].push_back(layer[i]);
                        }
                    }
                }
            }
        }
        return out;    
    }

    void export_vh (std::string file_path = "C:/Users/louja/Desktop/OwnProjects/Decision_tree/decision_tree/Decision-tree-verilog/param_declare.vh"){
        to_out_structure();

        std::ofstream file;
        file.open(file_path);

        if(!file.is_open()){
            std::cout << "Error opening file" << std::endl;
            return; //dont return exit failure as it is a void function
        }
        
        ////finds max width for padding purposes
        int max_width = 0;
        for(auto l : out_structure){
            if(l.size() > max_width){
                max_width = l.size();
            }
        }
        ////

        /////pads with -1
        for(auto line : out_structure){
            while(line.size() < max_width){
                line.push_back("-1");
            }
            padded_out_structure.push_back(line);
        }
        ////

        ////writes out
        file << "`ifndef PARAM_DECLARE_VH" << "\n";
        file << "`define PARAM_DECLARE_VH" << "\n";
        file << "parameter int rows = " << padded_out_structure.size() << ";\n";
        file << "parameter int width = " << max_width << ";\n";
        file << "parameter int struct_array [0:rows-1][0:width-1] = '{";


        std::vector<std::string> padded_line = padded_out_structure[0];
        file << "{";
    
        file<<padded_line[0];

        for(int i = 1; i < max_width;i++){
            file << "," << padded_line[i]; 
        }

        file << "}";
        
        for(int x = 1; x < out_structure.size(); x++){
            
            padded_line = padded_out_structure[x];
            
            file << ", {" << padded_line[0];
            for(int i = 1; i < max_width; i++){
                file << "," << padded_line[i];
            }
            file << "}";
        }        
        
        file << "};\n";

        file << "parameter int splits [0:rows-1] = '{";
        file << splits_per_node[0];
        for(int i = 1; i<splits_per_node.size();i++){
            file << "," << splits_per_node[i];
        }
        file << "};\n";

        file << "parameter int layer = 0;\n";
        file << "parameter int input_width = " << input_width << ";\n";
        file << "parameter int output_width = " << output_width << ";\n";



        file << "`endif" << "\n";
        /////

        file.close();
        

    }

    ~A3Tree(){
        deallocate_tree(t);
    }

private:
    TreeNode* t;
    std::unordered_map<std::string, std::vector<std::string>> sub_group_corresponding_categorys;
    std::vector<std::vector<std::string>> structure;
    std::vector<std::string> results;
    std::vector<std::vector<std::string>> categories;
    std::vector<std::vector<std::string>> out_structure;
    std::vector<std::vector<std::string>> padded_out_structure;
    std::unordered_map<std::string,std::pair<int,int>> cat_bits;
    std::unordered_map<std::string, std::string> result_encoded;
    std::unordered_map<std::string,std::unordered_map<std::string, std::string>> cat_group_encoding;
    std::vector<int> splits_per_node;
    int output_width;
    int input_width;
};
 
int main(){
 
    std::vector<std::vector<std::string>> input1
    {
        {"temperature", "rain", "wind", "quality"},
        {"high", "yes", "light", "acceptable"},
        {"low", "yes", "light", "acceptable"},
        {"low", "no", "moderate", "good"},
        {"high", "yes", "strong", "poor"},
        {"high", "yes", "moderate", "acceptable"},
        {"high", "no", "moderate", "good"},
        {"low", "yes", "strong", "poor"},
        {"high", "no", "light", "good"},
        {"low", "yes", "moderate", "poor"},
        {"high", "no", "strong", "poor"}
    };
 
    std::vector<std::vector<std::string>> input2
    {
        {"Feature_3", "feature2", "feature", "feature0", "not_a_feature"},
        {"a13480", "10", "a13480", "a", "1"},
        {"B_34203", "9", "1343432", "a", "a2"},
        {"a13480", "8", "57657", "a", "3"},
        {"B_34203", "B_34203", "4523", "a", "2"},
        {"B_34203", "6", "4523", "a", "some_value"},
        {"a13480", "5", "4523", "a", "1"}
    };
 
    A3Tree t1(input1);
    A3Tree t2(input2);
 
    std::vector<std::string> q;
 
    q =  {"high", "yes", "moderate"};
    std::cout << t1.query(q) << std::endl;
    /// this should print: acceptable
 
    q = {"B_34203", "9", "1343432", "a"};
    std::cout << t2.query(q) << std::endl;
    /// this should print: a2
    std::cout << t2.node_count() << std::endl;

   // t1.print_cats_and_sub_groups();
    //t2.print_cats_and_sub_groups();

    t1.print_structure();
    //t2.print_structure();
    std::vector<std::string> results = t1.outputs();
    std::cout<< "OUTPUTS"<< std::endl;
    for(auto i : results){
        std::cout<< i << std::endl;
    }
    std::vector<std::vector<std::string>> cats_subs = t1.cats_features();
    std::cout << "CATS AND FEATURES" << std::endl;
    for(auto layer: cats_subs){
        std::cout << layer[0] << ": " << std::endl;
        std::string tmp;
        for(int i = 1; i < layer.size(); i++){
            tmp = tmp + layer[i] + " ";
        }
        std::cout << tmp << std::endl;
    }

    t1.export_encoding(); //have to export encoding before vh
    t1.export_vh();
 
}