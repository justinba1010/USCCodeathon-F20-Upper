#include "automata.h"

AutomataLink* AutomataNewEpsilonMatcher() {
	AutomataLink* l = MustMalloc(sizeof(AutomataLink));
	l->Matcher =AutomataEpsilonMatch;
	l->Type = AUTOMATA_MATCHER_EPSILON;
	l->Text = NULL;
	l->NText = 0;
	l->Target = NULL;
	l->next = NULL;
	dbprintf("created new epsilon matcher: %p\n", (void*) l);
	return l;
}

AutomataLink* AutomataNewClassMatcher(char* class, size_t nclass) {
	AutomataLink* l = AutomataNewEpsilonMatcher();
	l->Type = AUTOMATA_MATCHER_CLASS;
	l->Text = class;
	l->NText = nclass;
	dbprintf("created new class matcher for class '%s': %p\n", class, (void*) l);
	return l;
}

AutomataLink* AutomataNewPalindromeMatcher() {
	AutomataLink* l = AutomataNewEpsilonMatcher();
	l->Type = AUTOMATA_MATCHER_CLASS;
	l->Text = MustMalloc(sizeof(char));
	l->Text[0] = 0;
	l->NText = 0;
	dbprintf("created new palindrome matcher: %p\n", (void*) l);
	return l;
}

void AutomataFreeLink(struct AutomataLink_t* link) {
	dbprintf("free-ing link %p\n", (void*) link);

	if (link->next != NULL) {
		dbprintf("link %p contains non-NULL next field %p, free-ing it first...\n", (void*) link, (void*) link->next);
		AutomataFreeLink(link);
	}

	if (link->Text != NULL) {
		dbprintf("free-ing text field for link %p\n", (void*) link);
		free(link->Text);
	}

	if (link->Target != NULL) {
		dbprintf("link %p contains non-NULL Target field %p, freeing it...\n", (void*) link, (void*) link->Target);
		AutomataFreeNode(link->Target);
	}

	dbprintf("link fields for %p free-ed.\n", (void*) link);
	free(link);

}


bool AutomataClassMatch(struct AutomataLink_t* link, char symbol) {
	dbprintf("matching symbol '%c' link %p character class '%s'\n", symbol, (void*) link, link->Text);
	for (char* c = link->Text; c < (link->Text + link->NText) ; c++) {
		if (*c == symbol) {
			dbprintf("\tMATCH\n");
			return true;
		}
	}
	dbprintf("\tNO MATCH\n");
	return false;
}

bool AutomataPalindromeMatch(struct AutomataLink_t* link, char symbol) {
	dbprintf("checking for palindrome match for link %p and symbol '%c'\n", (void*) link, symbol);
	// TODO
	return false;
}

bool AutomataEpsilonMatch(struct AutomataLink_t* link, char symbol) {
	UNUSED(link);
	UNUSED(symbol);
	dbprintf("epsilon-match for link %p\n", (void*) link);
	return true;
}


AutomataNode* AutomataNewNode(bool accepting) {
	AutomataNode* n = MustMalloc(sizeof(AutomataNode));
	n->Active = false;
	n->Accepting = accepting;
	n->Links = NULL;
	dbprintf("created new %s node %p", (accepting) ? "accepting" : "non-accepting", (void*) n);
	return n;
}

void AutomataFreeNode(struct AutomataNode_t* node) {
	dbprintf("free-ing node %p\n", (void*) node);
	if (node->Links != NULL) {
		dbprintf("node %p has links, free-ing those first\n", (void*) node);
		AutomataFreeLink(node->Links); // handles list traversal automatically
	}
	free(node);
}
