#define _GNU_SOURCE
#include "automata.h"
#include "util.h"

int main(int argc, char** argv) {
	char* s1;
	char* s2;
	asprintf(&s1, "%s", "a");
	asprintf(&s2, "%s", "xyz");
	
	AutomataNode* start = AutomataNewNode(false);
	AutomataNode* n1 = AutomataNewNode(false);
	AutomataNode* n2 = AutomataNewNode(false);
	AutomataNode* n3 = AutomataNewNode(false);
	AutomataNode* end = AutomataNewNode(true);
	AutomataLink* matchA = AutomataNewClassMatcher(s1, 1);
	AutomataLink* matchXYZ = AutomataNewClassMatcher(s2, 3);
	AutomataLink* matchEpsilon = AutomataNewEpsilonMatcher();
	AutomataLink* matchEpsilon2 = AutomataNewEpsilonMatcher();
	AutomataLink* matchP = AutomataNewPalindromeMatcher();

	AutomataLinkNodes(matchA, start, n1);
	AutomataLinkNodes(matchXYZ, n1, n2);
	AutomataLinkNodes(matchEpsilon, n1, n3);
	AutomataLinkNodes(matchP, n3, end);
	AutomataLinkNodes(matchEpsilon2, n2, end);


	AutomataDumpGraphviz(start, stdout);

	AutomataFreeNode(start);
}
