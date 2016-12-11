#include <fstream>
#include <stdlib.h>
#include "pattern.cpp"
#include <memory>
#include <vector>

using namespace std;


class Miner{
  public:
    virtual void run_solver(string dataset_name, string outputfile){}
    virtual vector<unique_ptr<Pattern>> parse_solver_output(string filepath){}
    virtual ~Miner(){}

    void write_asp_patterns_to_file(vector<unique_ptr<Pattern>> patterns, string outputfilename){
      cout << "writing ASP data to the file: " << outputfilename; 
      ofstream myfile;
      myfile.open(outputfilename);
      for (auto &p: patterns){
        myfile << p->make_ASP_str();
      }
      myfile.close();
    }

};

class ItemsetMiner : public Miner{
  public:
    ItemsetMiner(){};
    ~ItemsetMiner(){};

    void run_solver(string dataset_name, string outputfile){
      string miner_name = "bin/eclat -v ':%a'";
      string command = miner_name + " " + dataset_name +" -o " + outputfile + "\n";
      cout << "executing the command " << command;
      system(command.c_str());
    };

    vector<unique_ptr<Pattern>> parse_solver_output(string filepath){
      string line;
      ifstream f (filepath);
      vector<unique_ptr<Pattern>> v;
      while(getline(f, line)) {
        v.push_back(make_unique<Itemset>(line));
      }
      return move(v);
    };

};

unique_ptr<Miner> pick_miner(string datatype){
  if(datatype == "itemset"){
    return make_unique<ItemsetMiner>();
  }
};

