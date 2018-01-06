/* 
	CS373HW1.cpp
	
*/

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <cstdlib>
#include <cstddef>
#include <vector>


using namespace std;

class Node
{
	public:
	int ID;
	int start_flag = 0;
	int accept_flag = 0;
	int reject_flag = 0;

		Node(int new_ID, int new_start_flag, int new_accept_flag, int new_reject_flag);
		int getID();
		int getStartFlag();
		int getAcceptFlag();
		int getRejectFlag();
		
};

	Node::Node(int new_ID, int new_start_flag, int new_accept_flag, int new_reject_flag)
	{
		ID = new_ID;
		start_flag = new_start_flag;
		accept_flag = new_accept_flag;
		reject_flag = new_reject_flag;
	}
	
	int Node::getID()
	{
		return ID;
	}
	int Node::getStartFlag()
	{
		return start_flag;
	}
	int Node::getAcceptFlag()
	{
		return accept_flag;
	}
	int Node::getRejectFlag()
	{
		return reject_flag;
	}

class Transition
{
	
	public:
	int start_node = -1;
	int end_node = -1;
	char symbol_read = 0;
	char symbol_tochange = 0;
	char left_right = 0;
	
	public:
		Transition(int new_start_node, int new_end_node, char new_symbol_read, char new_symbol_tochange, char new_left_right);
	
};

	Transition::Transition(int new_start_node, int new_end_node, char new_symbol_read, char new_symbol_tochange, char new_left_right)
	{
		start_node = new_start_node;
		end_node = new_end_node;
		symbol_read = new_symbol_read;
		symbol_tochange = new_symbol_tochange;
		left_right = new_left_right;
	}


int main(int argc, char** argv)
{
	ifstream File;
	string line;
	vector<string> words;
	vector<Node> nodes;
	vector<Transition> transitions;
	int start_counter = 0;

	File.open(argv[1]);

	if (File.is_open())
	{
		while (getline(File, line))
		{
			//enter all of the words individually into a vector
			istringstream iss1(line);
			copy(istream_iterator<string>(iss1), istream_iterator<string>(), back_inserter(words));
					
				
			//Trying to identify which lines are "state" and which are "transition"
			if (words[0] == "state")
			{
				if(words[2] == "start")
				{
					Node temp(stoi(words[1]),1,0,0);
					nodes.push_back(temp);
					
					if(words.size() >= 4)
					{
						cout << "Too many arguments in state" << endl;
						exit(1);
					}
					
					start_counter++;
					if(start_counter > 1)
					{
						cout << "Too many start states" << endl;
						exit(1);
					}
					
					words.clear();
				}
				else if(words[2] == "accept")
				{
					Node temp(stoi(words[1]),0,1,0);
					nodes.push_back(temp);
					
					if(words.size() >= 4)
					{
						cout << "Too many arguments in state" << endl;
						exit(1);
					}
					
					words.clear();
				}
				else if(words[2] == "reject")
				{
					Node temp(stoi(words[1]),0,0,1);
					nodes.push_back(temp);
					
					if(words.size() >= 4)
					{
						cout << "Too many arguments in state" << endl;
						exit(1);
					}
					
					words.clear();
				}
				else
				{
					cout << "Not an acceptable state" << endl;
					exit(1);
				}
				
			}
			else if (words[0] == "transition")
			{
				Transition temp(stoi(words[1]), stoi(words[3]), words[2].at(0), words[4].at(0), words[5].at(0));
				transitions.push_back(temp);
				
				words.clear();
			}
			else
			{
				cout << "Invalid line in text file" << endl;
				exit(1);
			}
		}
		File.close();
	}
	else
	{
		cout << "Unable to open file" << endl;
	}
	
	
	//Finished loading all the nodes and transitions into objects
	
	//read the string that we will move throught
	string s = argv[2];
	
	//add _'s to the front and back
	s.insert(0, "_");
	s.push_back('_');
	
	int position_pointer = 1;
	int current_state = -1;
	
	//find start state
	int i;
	for(i = 0; i < nodes.size(); i++)
	{
		if(nodes[i].getStartFlag() == 1)
		{
			current_state = nodes[i].getID();
		}
	}
	
	bool isOver = false;
	int counter = 0;
	int inevitable_counter = 0;
	
	while(!isOver)
	{
		
		inevitable_counter++;
		if(inevitable_counter > 1000000)
			exit(1);
		
		for(i = 0; i < transitions.size(); i++)
		{
			if(transitions[i].start_node == current_state)
			{
			
					
				if(transitions[i].symbol_read == s[position_pointer])
				{
					
					
					cout << current_state;
					counter++;
					
				
					if(counter == atoi(argv[3]))
					{
						cout << " quit" << endl;
						exit(1);
					}
					cout << "->";
				}
				
				//check if the symbol can be read or if the symbol is _
				//cout << transitions[i].symbol_read << " == " << s[position_pointer] << endl << endl;
				if(transitions[i].symbol_read == s[position_pointer])
				{	
					
					//check whether the end node is an accept or reject
					int j;
					
					for(j = 0; j < nodes.size(); j++)
					{
						//cout << nodes[j].getID() << " == " << transitions[i].end_node << endl;
						if(nodes[j].getID() == transitions[i].end_node) 
						{
							
							if(nodes[j].getAcceptFlag() == 1)
							{
								isOver = true;
								current_state = transitions[i].end_node;
								cout << transitions[i].end_node << " accept" << endl;
							}
							if(nodes[j].getRejectFlag() == 1)
							{
								isOver = true;
								current_state = transitions[i].end_node;
								cout << transitions[i].end_node << " reject" << endl;
							}
							
						}
					}
					
					current_state = transitions[i].end_node;
			
					//changes symbol at the spot to the change symbol if it reads the symbol probably
					
					
					s[position_pointer] = transitions[i].symbol_tochange;
			
					//moves the pointer right or left accordingly
					if(transitions[i].left_right == 'L')
					{
						if(position_pointer != 0 || position_pointer != s.length())
						{
							position_pointer--;
						}
					}
					else
					{				
						if(position_pointer != 0 || position_pointer != s.length())
						{
							position_pointer++;
						}	
					}
					
				}
			
			}
		}
	}
	
	
	
		

    return 0;
}

