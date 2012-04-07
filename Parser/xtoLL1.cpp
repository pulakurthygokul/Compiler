#include <cctype>
#include <fstream>
#include <sstream>
#include <xtoLL1.hpp>
#include <Terminal_NonTerminal.hpp>
using namespace std;

void ScanGrammarFromFile(Grammar& g, char* filename)
{
	ifstream f(filename);
	string s, t;
	bool ishead = true;
	vector<std::string> tail;
	do
	{
		getline(f, s, '\n');
		
		if(!s.compare("") || s[0] == '#')
			continue;
		else
		{
			ishead = true;
			Rule *r;
			stringstream ss(s);
			while(getline(ss, t, ' '))
			{
				if(!t.compare("-->"))
				{
					ishead = false;
					continue;
				}
				else if(!t.compare("|"))
				{
					r->RuleAddTail(tail);
					tail.clear();
				}
				else if(ishead)
				{
					r = new Rule(t);
					addNonTerminal(t);
				}
				else
				{
					tail.push_back(t);
					if(isupper(t[0]))
						addNonTerminal(t);
					else
						addTerminal(t);
				}
			}
			r->RuleAddTail(tail);
			tail.clear();
			g.GrammarAddRule(*r);
			//r.~Rule();
		}
	}while(f.good());
	f.close();
}

static bool HasCycles(Grammar& g, Rule r, std::string rhead)
{
	cerr<<"HasCycles():r = ";
	r.RuleOutput();
	cerr<<"HasCycles():rhead = "<<rhead<<endl;
	std::string t;
	int ti;
	for(int i = 0; i < r.RuleNTails(); i++)
	{
		cerr<<"HasCycles():r.RuleTail("<<i<<").size() = "<<r.RuleTail(i).size()<<endl;
		if(r.RuleTail(i).size() == 1)
		{
			t = r.RuleTail(i)[0];
			cerr<<"HasCycles():t = "<<t<<endl;
			if(!rhead.compare(t))
			{
				cerr<<"HasCycles():returning true"<<endl;
				return true;
			}
			else if((ti = g.GrammarFindRule(t)) >= 0)
			{
				cerr<<"HasCycles():recuring"<<endl;
				return HasCycles(g, g.GrammarRule(ti), rhead);
			}
		}
	}
	return false;
}

bool HasCycles(Grammar& g)	// wrapper for the above function
{
	for(int i = 0; i < g.GrammarNRules(); i++)
		if(HasCycles(g, g.GrammarRule(i), g.GrammarRule(i).RuleHead()))
			return true;
	return false;
}

bool HasNonTerminatingRules(Grammar& g)
{
	Rule r;
	vector<std::string> tail;
	int tail_nonterminals;
	bool headmatches;
	std::string head;
	for(int i = 0; i < g.GrammarNRules(); i++)
	{
		r = g.GrammarRule(i);
		head = r.RuleHead();
		for(int j = 0; j < r.RuleNTails(); j++)
		{
			tail = r.RuleTail(j);
			tail_nonterminals = 0;
			headmatches = false;
			for(int k = 0; k < tail.size(); k++)
			{
				if(!head.compare(tail[k]))
					headmatches = true;
				if(isNonTerminal(tail[k]))
					tail_nonterminals++;
			}
			if(tail_nonterminals == 1 && headmatches)
				return true;
		}
	}
	return false;
}

void EliminateEpsilonProductions(Grammar& g)
{
	return;
}

void EliminateLeftRecursion(Grammar& g)
{
	return;
}

void LeftFactorize(Grammar& g)
{
	return;
}
