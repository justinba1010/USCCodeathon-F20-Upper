#include "automata.h"
#include "util.h"

int main(int argc, char** argv) {
	
	AutomataNode* start = AutomataNewNode(false);
	AutomataNode* n1 = AutomataNewNode(false);
	AutomataNode* n2 = AutomataNewNode(false);
	AutomataNode* n3 = AutomataNewNode(false);
	AutomataNode* end = AutomataNewNode(true);
	AutomataLink* matchA = AutomataNewClassMatcher("a", 1);
	AutomataLink* matchXYZ = AutomataNewClassMatcher("xyz", 3);
	AutomataLink* matchEpsilon = AutomataNewEpsilonMatcher();
	AutomataLink* matchEpsilon2 = AutomataNewEpsilonMatcher();
	AutomataLink* matchP = AutomataNewPalindromeMatcher();

	AutomataLinkNodes(matchA, start, n1);
	AutomataLinkNodes(matchXYZ, n1, n2);
	AutomataLinkNodes(matchEpsilon, n1, n3);
	AutomataLinkNodes(matchP, n3, end);
	AutomataLinkNodes(matchEpsilon2, n2, end);


	AutomataDumpGraphviz(start, stdout);

}
