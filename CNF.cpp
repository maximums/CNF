#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <regex>
#include <random>
#include <deque>

using namespace std;

int letter = 88;

struct PRO{
    string key;
    deque<string> dervation;
};

PRO makeSome(deque<string> some){
    PRO temp;
    temp.key = some[0];
    some.pop_front();
    temp.dervation = some;
    return temp;
}

vector<PRO> readFIle()
{
  vector<PRO> proj;
  string line;
  ifstream myfile("CFG.txt");
  deque<string> tmp;
  regex rgx("(\\->)|(\\|)");
    if(myfile.is_open())
    {
      while (getline(myfile, line))
      {
        if (line == "") continue;
        sregex_token_iterator iter(line.begin(), line.end(), rgx, -1);
        sregex_token_iterator end;
        for ( ; iter != end; ++iter)
             tmp.push_back(*iter);
        proj.push_back(makeSome(tmp));
        tmp.clear();    
      }
      myfile.close();
    }    
  return proj;
}

void removeNUll(vector<PRO>* ptr, int i){
for (int k = 0; k < (*ptr).size(); k++)
  {
    for (int j = 0; j < (*ptr)[k].dervation.size(); j++)
    {
      if (!((*ptr)[k].dervation[j].find((*ptr)[i].key) != std::string::npos)) continue;
      string tmp = (*ptr)[k].dervation[j];
      tmp.erase(std::remove(tmp.begin(), tmp.end(), (*ptr)[i].key[0]), tmp.end());
      (*ptr)[k].dervation.push_back(tmp); 
    }
  
  }  
}

void emptyPRO(vector<PRO>* ptr ){
  for (int i = 0; i < (*ptr).size(); i++)
  {
    for (int j = 0; j < (*ptr)[i].dervation.size(); j++)
    {
      if((*ptr)[i].dervation[j] != "*") continue;
      (*ptr)[i].dervation.erase((*ptr)[i].dervation.begin() + j);
      removeNUll(ptr, i);
    }
  }
}

void removeUnit(vector<PRO>* ptr, int index, string key){
  for (int i = 0; i < (*ptr).size(); i++)
    if ((*ptr)[i].key == key)
      (*ptr)[index].dervation.insert((*ptr)[index].dervation.end(),(*ptr)[i].dervation.begin(), (*ptr)[i].dervation.end());
}

void unitPRO(vector<PRO>* ptr){
  for (int i = 0; i < (*ptr).size(); i++)
  {
    for (int j = 0; j < (*ptr)[i].dervation.size(); j++)
    {
      if((*ptr)[i].dervation[j].size() != 1) continue;
      if((*ptr)[i].dervation[j][0] > 90 || (*ptr)[i].dervation[j][0] < 65) continue; 
      string tmp = (*ptr)[i].dervation[j];
      (*ptr)[i].dervation.erase((*ptr)[i].dervation.begin()+j);
      removeUnit(ptr,i,tmp);
    }
  }
}

void newPRO4NON(vector<PRO>* ptr, string temp){
  PRO tmp;
  tmp.key = letter;
  letter++;
  tmp.dervation.push_back(temp.substr(1,2));
  for (int i = 0; i < (*ptr).size(); i++)
  {
    for (int j = 0; j < (*ptr)[i].dervation.size(); j++)
    {
      if ((*ptr)[i].dervation[j].size()>2){
          (*ptr)[i].dervation[j].erase(1, 2);
          (*ptr)[i].dervation[j].append(tmp.key);
      }
    }
  }
    (*ptr).push_back(tmp);
}

void newPRO(vector<PRO>* ptr, string tmp){
  PRO temp;
  temp.key = letter;
  letter++;
  temp.dervation.push_back(tmp.substr(0,1));
  for (int i = 0; i < (*ptr).size(); i++)
  {
    for (int j = 0; j < (*ptr)[i].dervation.size(); j++)
    {
      if ((*ptr)[i].dervation[j][0] == tmp[0] && (*ptr)[i].dervation[j].size()>1){
        (*ptr)[i].dervation[j][0] = temp.key[0];
      }
    }
  }
  (*ptr).push_back(temp);
}

void CNF(vector<PRO>* ptr){
  for (int i = 0; i < (*ptr).size(); i++)
  {
    for (int j = 0; j < (*ptr)[i].dervation.size(); j++)
    {
      if ((*ptr)[i].dervation[j].size() > 2)
        newPRO4NON(ptr, (*ptr)[i].dervation[j]);
      if ((*ptr)[i].dervation[j].size() > 1 && !isupper((*ptr)[i].dervation[j][0]))
        newPRO(ptr,(*ptr)[i].dervation[j]);
    }
  }
}

int main(){
  vector<PRO> checkSome = readFIle();
  emptyPRO(&checkSome);
  cout<<"After removing empty productions:"<<endl;
  for (int i = 0; i < checkSome.size(); i++)
  {
    cout<<checkSome[i].key<<" --> ";
    for (int j = 0; j < checkSome[i].dervation.size(); j++)
    {
      cout<<checkSome[i].dervation[j]<<" ";
    }
    cout<<"\n";
  }
  unitPRO(&checkSome);
  cout<<"After removing unit productions:"<<endl;
  for (int i = 0; i < checkSome.size(); i++)
  {
    cout<<checkSome[i].key<<" --> ";
    for (int j = 0; j < checkSome[i].dervation.size(); j++)
    {
      cout<<checkSome[i].dervation[j]<<" ";
    }
    cout<<"\n";
  }
  CNF(&checkSome);
  cout<<"Chomsky normal form:"<<endl;
  for (int i = 0; i < checkSome.size(); i++)
  {
    cout<<checkSome[i].key<<" --> ";
    for (int j = 0; j < checkSome[i].dervation.size(); j++)
    {
      cout<<checkSome[i].dervation[j]<<" ";
    }
    cout<<"\n";
  }
    return 0;
}