// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <stack>
#include <chrono>
#include <stdexcept>
#include <algorithm>
#include <cctype>

#include "NodeBoolTree.h"
#include "boolinterval.h"
#include "boolequation.h"
#include "BBV.h"

using namespace std;

// Функция для удаления пробельных символов с начала и конца строки
string trim(const string &s) {
    auto start = s.begin();
    while (start != s.end() && isspace(*start)) {
        ++start;
    }
    auto end = s.end();
    do {
        --end;
    } while (distance(start, end) > 0 && isspace(*end));
    return string(start, end + 1);
}

int main(int argc, char *argv[])
{
    try
    {
        auto startTimeSuperGeneral = chrono::high_resolution_clock::now();
        vector<string> full_file_list;
        std::string filepath = "/home/kali/Desktop/Lab2WithOutQtAndAlloc/SAT_DPLL/SatExamples/Sat_ex14_3.pla";
        // std::string filepath = "/home/kali/Desktop/pythonSAT/large_sat_problem.pla";
        // std::string filepath = "/home/kali/Desktop/pythonSAT/incorrect_sat_problem.pla";
        
        ifstream file(filepath);
        if (file.is_open())
        {
            string line;
            while (getline(file, line))
            {
                // Если строка заканчивается символом '\r', удаляем его
                if (!line.empty() && line.back() == '\r')
                    line.pop_back();
                full_file_list.push_back(trim(line));
            }
            file.close();

            int cnfSize = full_file_list.size();
            BoolInterval **CNF = new BoolInterval*[cnfSize];
            int rangInterval = -1;

            if (cnfSize > 0)
            {
                // Определяем размер интервала по длине первой строки
                rangInterval = full_file_list[0].length();
            }

            for (int i = 0; i < cnfSize; i++)
            {
                CNF[i] = new BoolInterval(full_file_list[i].c_str());
            }

            string rootvec, rootdnc;
            for (int i = 0; i < rangInterval; i++)
            {
                rootvec.push_back('0');
                rootdnc.push_back('1');
            }
            BBV vec(rootvec.c_str());
            BBV dnc(rootdnc.c_str());

            BoolInterval *root = new BoolInterval(vec, dnc);
            BoolEquation *boolequation = new BoolEquation(CNF, root, cnfSize, cnfSize, vec);

            // Назначаем стратегию через сеттер
            boolequation->SetStrategy(new ColumnStrategy());

            bool rootIsFinded = false;
            stack<NodeBoolTree*> BoolTree;
            NodeBoolTree *startNode = new NodeBoolTree(boolequation);
            BoolTree.push(startNode);

            do
            {
                NodeBoolTree *currentNode = BoolTree.top();

                if (currentNode->lt == nullptr && currentNode->rt == nullptr)
                {
                    // Используем геттер для получения BoolEquation
                    BoolEquation *currentEquation = currentNode->eq;
                    // Если стратегия не назначена, назначаем её
                    if (currentEquation->GetStrategy() == nullptr)
                    {
                        currentEquation->SetStrategy(new ColumnStrategy());
                    }
                    bool flag = true;
                    while (flag)
                    {
                        int a = currentEquation->CheckRules();

                        switch (a)
                        {
                        case 0:
                            BoolTree.pop();
                            flag = false;
                            break;
                        case 1:
                            if (currentEquation->GetCount() == 0 ||
                                currentEquation->GetMask().getWeight() == currentEquation->GetMask().getSize())
                            {
                                flag = false;
                                rootIsFinded = true;
                                for (int i = 0; i < cnfSize; i++)
                                {
                                    if (!CNF[i]->isEqualComponent(*currentEquation->GetRoot()))
                                    {
                                        rootIsFinded = false;
                                        BoolTree.pop();
                                        break;
                                    }
                                }
                            }
                            break;
                        case 2:
                        {
                            int indexBranching = currentEquation->GetStrategy()->chooseVarForBranching(currentEquation);
                            if (indexBranching < 0)
                            {
                                flag = false;
                                break;
                            }
                            BoolEquation *Equation0 = new BoolEquation(*currentEquation);
                            BoolEquation *Equation1 = new BoolEquation(*currentEquation);

                            Equation0->Simplify(indexBranching, '0');
                            Equation1->Simplify(indexBranching, '1');

                            NodeBoolTree *Node0 = new NodeBoolTree(Equation0);
                            NodeBoolTree *Node1 = new NodeBoolTree(Equation1);

                            currentNode->lt = Node0;
                            currentNode->rt = Node1;

                            BoolTree.push(Node1);
                            BoolTree.push(Node0);

                            flag = false;
                            break;
                        }
                        }
                    }
                }
                else
                {
                    BoolTree.pop();
                }
            } while (BoolTree.size() > 1 && !rootIsFinded);

            if (rootIsFinded)
            {
                cout << "Root is:\n ";
                BoolInterval *finded_root = BoolTree.top()->eq->GetRoot();
                cout << string(*finded_root);
            }
            else
            {
                cout << "Root does not exist!";
            }
        }
        else
        {
            cout << "File does not exist.\n";
        }
        auto endTimeSuperGeneral = chrono::high_resolution_clock::now();
        chrono::duration<double> durationSuperGeneral = endTimeSuperGeneral - startTimeSuperGeneral;
        cout << "\n Время работы программы с allocator: " 
             << chrono::duration_cast<chrono::microseconds>(durationSuperGeneral).count() 
             << " микросек.\n";
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
    return 0;
}
